/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _triang.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/plane_alg.h>

typedef point  POINT;


void TRIANGULATE_POINTS(list<POINT> L, GRAPH<POINT,edge>& G)
{
  L.sort();  // sort pointss lexicographically

  // eliminate multiple points

  list_item it;
  forall_items(it,L)
  { list_item it1 = L.succ(it);
    while (it1 != nil && L[it1] == L[it])
    { L.del(it1);
      it1 = L.succ(it);
     }
   }

  // initialize convex hull with first three points

  node u = G.new_node(L.pop());
  node v = G.new_node(L.pop());
  node w = G.new_node(L.pop());

  edge e1 = G.new_edge(u,v,nil);
  G[e1] = G.new_edge(v,u,e1);
  edge e2 = G.new_edge(v,w,nil);
  G[e2] = G.new_edge(w,v,e2);
  edge e3 = G.new_edge(w,u,nil);
  G[e3] = G.new_edge(u,w,e3);

  edge last =  ( orientation(G[w],G[v],G[u]) < 0 ) ? G[e2] : e3;

  // scan remaining points

  POINT p;
  forall(p,L)
  {
    node  v = G.new_node(p);

    // compute upper tangent (p,up)
    edge up = last;
    while (right_turn(p,G[source(up)],G[target(up)]))
         up = G.cyclic_adj_succ(G[up]);

    // compute lower tangent (p,low)
    edge low = G[G.cyclic_adj_pred(last)];
    while (left_turn(p,G[target(low)],G[source(low)]))
         low = G[G.cyclic_adj_pred(low)];

    // triangulate
    edge e = up;
    do { edge next = G[G.cyclic_adj_pred(e)];
         edge x = G.new_edge(v,source(e),nil);
         G[x] = G.new_edge(e,v,x,before);
         e = next;
        } while (e != low);
    last = G.first_adj_edge(v);
   }

// if (!PLANAR(G)) error_handler(1,"graph not planar");
}

