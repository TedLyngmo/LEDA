/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _delaunay.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#include <LEDA/plane_alg.h>
#include <LEDA/planar_map.h>

typedef point   POINT;

// in the current version we use GRAPH<POINT,edge> for representing
// planar maps ( G[e] = rev(e) )


static int flip_count = 0;

inline bool flip_test(const GRAPH<POINT,edge>& G, edge e)
{ POINT a = G[source(e)];
  POINT b = G[target(G.cyclic_adj_pred(e))];
  POINT c = G[target(e)];
  POINT d = G[target(G.cyclic_adj_succ(e))];
  return right_turn(b,a,d) && right_turn(b,d,c) && incircle(a,b,c,d);
 }


static void DELAUNAY_FLIPPING(GRAPH<POINT,edge>& G)
{
  edge_map<list_item> It(G,nil);
  list<edge> L;
  edge E[4];


  edge e;
  forall_edges(e,G)
    if (It[e] == nil && flip_test(G,e)) It[G[e]] = It[e] = L.append(e);

  while ( !L.empty() )
  { flip_count++;

    edge e = L.pop();
    edge x = G.cyclic_adj_pred(e);

    // delete diagonal
    G.del_edge(G[e]);
    G.del_edge(e);

    // collect face edges of quadriliteral
    for(int i = 0; i < 4; i++)
    { E[i] = x;
      x = G.cyclic_adj_pred(G[x]);
     }

    // insert new diagonal
    e = G.new_edge(E[1],source(E[3]),nil);
    G[e] = G.new_edge(E[3],source(E[1]),e);


    // test collected edges
    for(int j=0; j<4; j++)
    { edge e = E[j];
      if (flip_test(G,e))
        { if (It[e] == nil)
          It[G[e]] = It[e] = L.push(e);
         }
      else
          if (It[e] != nil)
          { L.del_item(It[e]);
            It[G[e]] = It[e] = nil;
           }
     }

   }
 }



void DELAUNAY_TRIANG(const list<POINT>& L, GRAPH<POINT,edge>& T)
{
  TRIANGULATE_POINTS(L,T);
  DELAUNAY_FLIPPING(T);
}


void DELAUNAY_TRIANG(const list<POINT>& L, graph& T,
                                           node_array<POINT>& pos,
                                           edge_array<edge>&  rev)
{ GRAPH<POINT,edge> G;
  DELAUNAY_TRIANG(L,G);

  T = G;

  node v;
  pos.init(T);
  forall_nodes(v,G) pos[v] = G[v];

  edge e;
  rev.init(T);
  forall_edges(e,G) rev[e] = G[e];
}


void DELAUNAY_TRIANG(const list<POINT>& L, planar_map& T,
                                           node_array<POINT>& pos)
{ GRAPH<POINT,edge> G;
  DELAUNAY_TRIANG(L,G);

  T = G;

  node v;
  pos.init(T);
  forall_nodes(v,G) pos[v] = G[v];
}

