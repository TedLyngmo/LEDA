/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _polygon.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/polygon.h>
#include <LEDA/plane_alg.h>
#include <LEDA/map.h>
#include <math.h>


//------------------------------------------------------------------------------
// polygons
//------------------------------------------------------------------------------


ostream& operator<<(ostream& out, const polygon& p) 
{ p.vertices().print(out);
  return out << endl;
 } 

istream& operator>>(istream& in,  polygon& p) 
{ list<point> L; 
  L.read(in,'\n'); 
  p = polygon(L); 
  return in;
}


double polygon::compute_area(const list<segment>& seg_list) const
{
  if (seg_list.length() < 3) return 0;

  list_item it = seg_list.item(1);
  point     p  = seg_list[it].source();

  it = seg_list.succ(it);

  double    A  = 0;

  while (it)
  { segment s = seg_list[it];
    A += ::area(p,s.source(),s.target());
    it = seg_list.succ(it);
   }

  return A;
}


static void check_simplicity(const list<segment>& seg_list)
{ GRAPH<point,segment> G;
  SWEEP_SEGMENTS(seg_list,G);
  node v;
  forall_nodes(v,G)
    if (G.degree(v) != 2)
      error_handler(1,"polygon: polygon must be simple");
}



polygon::polygon(const list<point>& pl)
{ 
  list<segment> seglist;

  for(list_item it = pl.first(); it; it = pl.succ(it))
    seglist.append(segment(pl[it],pl[pl.cyclic_succ(it)]));

  if (compute_area(seglist) < 0)
  { // reverse edge list
    seglist.clear();
    for(list_item it = pl.last(); it; it = pl.pred(it))
      seglist.append(segment(pl[it],pl[pl.cyclic_pred(it)]));
   }
  check_simplicity(seglist);

  PTR = new polygon_rep(seglist);
}


list<point> polygon::vertices() const
{ list<point> result;
  segment s;
  forall(s,ptr()->seg_list) result.append(s.start());
  return result;
}


polygon polygon::translate(double alpha, double d) const
{ list<segment> L;
  segment s;
  forall(s,ptr()->seg_list) L.append(s.translate(alpha,d));
  return polygon(L);
}


polygon polygon::translate(const vector& v) const
{ list<segment> L;
  segment s;
  forall(s,ptr()->seg_list) L.append(s.translate(v));
  return polygon(L);
}


polygon polygon::rotate(const point& origin, double alpha) const
{ list<segment> L;
  segment s;
  forall(s,ptr()->seg_list) L.append(s.rotate(origin,alpha));
  return polygon(L);
}


polygon polygon::rotate(double alpha) const
{ return rotate(point(0,0),alpha); }


bool polygon::inside(const point& p) const
{
  list<segment>& seglist = ptr()->seg_list;

  int count = 0;

  double px = p.xcoord();

  list_item it0 = seglist.first();
  list_item it1 = seglist.first();

  double x0 = seglist[it0].xcoord2();
  double x1 = seglist[it1].xcoord2();

  list_item it;

  forall_items(it,seglist)
  { segment s = seglist[it];
    if (s.xcoord2() < x0) 
    { it0 = it;
      x0 = s.xcoord2();
     }
    if (s.xcoord2() > x1) 
    { it1 = it1;
      x1 = s.xcoord2();
     }
   }

  if (px <= x0 || x1 <= px) return false;

  while (seglist[it0].xcoord2() <= px) it0 = seglist.cyclic_succ(it0);

  it = it0;
  do
  { while (seglist[it].xcoord2() >= px) it = seglist.cyclic_succ(it);
    if (orientation(seglist[it],p) > 0) count++;
    while (seglist[it].xcoord2() <= px) it = seglist.cyclic_succ(it);
    if (orientation(seglist[it],p) < 0) count++;
  } while (it != it0);

  return count % 2;

}



bool polygon::outside(const point& p) const { return !inside(p); }


list<point> polygon::intersection(const segment& s) const
{ list<point> result;
  segment t;
  point p;

  forall(t,ptr()->seg_list) 
    if (s.intersection(t,p))
     if (result.empty()) result.append(p);
     else if (p != result.tail() ) result.append(p);

  return result;
}


list<point> polygon::intersection(const line& l) const
{ list<point> result;
  segment t;
  point p;

  forall(t,ptr()->seg_list) 
    if (l.intersection(t,p))
     if (result.empty()) result.append(p);
     else if (p != result.tail() ) result.append(p);

  return result;
}


// intersection or union with polygon

static bool test_edge(GRAPH<point,segment>& G, edge i1, int mode)

{ node v  = G.target(i1);
  edge i2 = G.cyclic_in_succ(i1);

  if (i1 == i2) return false;

  edge o1 = G.first_adj_edge(v);
  edge o2 = G.last_adj_edge(v);

  point p1 = G[o1].target();
  point p2 = G[o2].target();

  segment si1 = G[i1];
  segment si2 = G[i2];

  if (mode == 0) // intersection
  { if (orientation(si1,si2.source()) > 0)
      return orientation(si1,p1) > 0 && orientation(si2,p1) < 0 && 
             orientation(si1,p2) > 0 && orientation(si2,p2) < 0;
   else
      return (orientation(si1,p1) > 0 || orientation(si2,p1) < 0) && 
             (orientation(si1,p2) > 0 || orientation(si2,p2) < 0);
  }
  else // union
  { if (orientation(si1,si2.source()) < 0)
      return orientation(si1,p1) < 0 && orientation(si2,p1) > 0 && 
             orientation(si1,p2) < 0 && orientation(si2,p2) > 0;
   else
      return (orientation(si1,p1) < 0 || orientation(si2,p1) > 0) && 
             (orientation(si1,p2) < 0 || orientation(si2,p2) > 0);
   }
}



static edge next_edge(GRAPH<point,segment>& G, edge i1, int dir)
{ 
  // if dir = +1 turn left
  // if dir = -1 turn right

  node v = G.target(i1);

  edge o1 = G.first_adj_edge(v);
  edge o2 = G.last_adj_edge(v);

  segment si1 = G[i1];
  segment so1 = G[o1];
  segment so2 = G[o2];

  if (o2 == nil) return o1;

  int orient1 = orientation(si1,so1.target());
  int orient2 = orientation(si1,so2.target());

  if (orient1 == orient2)
     return (orientation(so1,so2.target()) == dir) ? o2 : o1;
  else
     return (orient1 - orient2 == dir) ? o1 : o2;

}



list_polygon_ polygon::intersection(const polygon& P) const
{
  list<polygon> result;
  list<segment> seg_list;
  GRAPH<point,segment> G;

  segment s;
  forall(s,ptr()->seg_list) seg_list.append(s);
  forall(s,P.ptr()->seg_list) seg_list.append(s);

  SWEEP_SEGMENTS(seg_list,G);

  bool borders_intersect = false;

  node v;
  forall_nodes(v,G) 
    if (degree(v) > 2) 
    { borders_intersect = true;
      break;
     }

  if ( ! borders_intersect )
  { // no intersections between edges of (*this) and P
    // check for inclusion

    segment s1 = ptr()->seg_list.head();
    segment s2 = P.ptr()->seg_list.head();

    if ( P.inside(s1.start()) ) result.append(*this);
    if ( inside(s2.start()) ) result.append(P);                   

    return result;

   }

  edge_array<bool> marked(G,false);

  edge e;
  forall_edges(e,G) 
    if ( ! marked[e] && test_edge(G,e,0) )
    { list<segment> pol;
      edge start = e;
      do { node v = source(e);
           node w = target(e);
           pol.append(segment(G[v],G[w]));
           marked[e] = true;
           e = next_edge(G,e,+1);
          } while (e != start);
      result.append(polygon(pol));
     }

 return result;

}


list_polygon_ polygon::unite(const polygon& P) const
{
  list<polygon> result;
  list<segment> seg_list;
  GRAPH<point,segment> G;

  segment s;
  forall(s,ptr()->seg_list) seg_list.append(s);
  forall(s,P.ptr()->seg_list) seg_list.append(s);

  SWEEP_SEGMENTS(seg_list,G);

  if (G.number_of_nodes() == size() + P.size())
  { // no intersections between edges of (*this) and P
    // check for inclusion

    segment s1 = ptr()->seg_list.head();
    segment s2 = P.ptr()->seg_list.head();

    if ( ! P.inside(s1.start())) result.append(*this);
    if ( ! inside(s2.start())) result.append(P);                   

    return result;

   }

  edge_array<bool> marked(G,false);

  edge e;
  forall_edges(e,G) 
    if ( ! marked[e] && test_edge(G,e,1) )
    { list<segment> pol;
      edge start = e;
      do { node v = source(e);
           node w = target(e);
           pol.append(segment(G[v],G[w]));
           marked[e] = true;
           e = next_edge(G,e,-1);
          } while (e != start);
      result.append(polygon(pol));
     }

 return result;

}

