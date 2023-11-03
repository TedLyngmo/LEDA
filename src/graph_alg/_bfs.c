/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _bfs.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


/*******************************************************************************
*                                                                              *
*  BFS  (breadth first search)                                                 *
*                                                                              *
*******************************************************************************/


#include <LEDA/graph_alg.h>

list<node> BFS(const graph&, node s, node_array<int>& dist)
{
  // precondition: dist[v] < 0 for all nodes v

  list<node> Q(s);
  node v,w;
  list_item it;

  dist[s] = 0;
  it = Q.first();

  while (it != nil)
    { v = Q[it];
      forall_adj_nodes(w,v)
         if (dist[w] < 0) { Q.append(w);
                            dist[w] = dist[v]+1;
                           }
      it = Q.succ(it);
     }
  return Q;
}

