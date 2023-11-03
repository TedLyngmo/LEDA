/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _convex_hull.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

struct ch_edge {

Point   source;
Point   target;
ch_edge* succ;
ch_edge* pred;
ch_edge* link;
bool     outside;

ch_edge(const Point& a, const Point& b) : source(a), target(b)
{ outside = true; }

~ch_edge() {}

};


list<Point>  CONVEX_HULL(const list<Point>& L)
{

  list_item it;

/*
  // move the four extreme points to the front of |L|

  list_item xmax_it = L.first();
  list_item xmin_it = L.first();
  list_item ymax_it = L.first();
  list_item ymin_it = L.first();

  forall_items(it,L)
  { if (L[it].xcoord() > L[xmax_it].xcoord()) xmax_it = it;
    if (L[it].xcoord() < L[xmin_it].xcoord()) xmin_it = it;
    if (L[it].ycoord() > L[ymax_it].ycoord()) ymax_it = it;
    if (L[it].ycoord() > L[ymin_it].ycoord()) ymin_it = it;
   }

  L.push(L[xmax_it]); L.del(xmax_it);
  L.push(L[xmin_it]); L.del(xmin_it);
  L.push(L[ymax_it]); L.del(ymax_it);
  L.push(L[ymin_it]); L.del(ymin_it);
*/


  it = L.first();

  Point A = L[it];
  it = L.succ(it);

  Point B = L[it];
  it = L.succ(it);

  ch_edge* T1 = new ch_edge(A,B);
  ch_edge* T2 = new ch_edge(B,A);

  ch_edge* last_edge = T2;
  T2->link = T1;
  T1->link = nil;

  T1->succ = T2;
  T1->pred = T2;
  T2->succ = T1;
  T2->pred = T1;

  // scan remaining points


  while (it != nil)
  { Point P = L[it];
    it = L.succ(it);

    ch_edge* p = (right_turn(A,B,P)) ?  T1 : T2;

    while ( ! p->outside) //(p->succ->pred != p)
    { ch_edge* r0 = p->pred;
      if (right_turn(r0->source,r0->target,P)) p = r0;
      else { ch_edge* r1 = p->succ;
             if (right_turn(r1->source,r1->target,P)) p = r1;
             else { p =  nil; break; }
            }
     }

    if (p==nil) continue;  // P lies inside convex hull


    // compute "upper" tangent (p,high->source)

    ch_edge* high = p->succ;
    while (right_turn(high->source,high->target,P)) high = high->succ;


    // compute "lower" tangent (p,low->target)

    ch_edge* low = p->pred;
    while (right_turn(low->source,low->target,P)) low = low->pred;


    p = low->succ;  // p = successor of low edge

    // insert two edges between "low" and "high"

    ch_edge* e_l = new ch_edge(low->target,P);
    ch_edge* e_h = new ch_edge(P,high->source);

    e_h->link = e_l;
    e_l->link = last_edge;
    last_edge = e_h;

    e_h->succ = high;
    e_l->pred = low;
    high->pred = e_l->succ = e_h;
    low->succ  = e_h->pred = e_l;


    // mark edges between low and high as "inside" and define refinements

    while (p != high)
    { ch_edge* q = p->succ;
      p->pred = e_l;
      p->succ = e_h;
      p->outside = false;
      p = q;
     }

   }


  // return list of vertices

  list<Point> CH;

  CH.append(last_edge->source);
  for(ch_edge* p = last_edge->succ; p != last_edge; p = p->succ)
     CH.append(p->source);


 // clean up

  while (last_edge)
  { ch_edge* p = last_edge;
    last_edge = last_edge->link;
    delete p;
   }

  return CH;

}


