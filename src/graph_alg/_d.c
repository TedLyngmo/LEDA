/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _d.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/graph.h>
#include <LEDA/p_queue.h>


void DIJKSTRA(graph& G, node s, 
              edge_array<int>&  cost, 
              node_array<int>&  dist,
              node_array<edge>& pred,
              p_queue<int,node>&   PQ)
{
  node_array<pq_item> I(G);
  node v;
                                                                               
  forall_nodes(v,G)
  { pred[v] = nil;
    dist[v] = MAXINT;
   }

  dist[s] = 0;
  I[s] = PQ.insert(0,s);

  while (! PQ.empty())
  { pq_item it = PQ.find_min();
    node u = PQ.inf(it);
    int du = dist[u];
    edge e;
    forall_adj_edges(e,u)
    { v = G.target(e);
      int c = du + cost[e];
      if (c < dist[v])
      { if (dist[v] == MAXINT)
          I[v] = PQ.insert(c,v);
        else
          PQ.decrease_p(I[v],c);
        dist[v] = c;
        pred[v] = e;
       }                                                                 
     }
    PQ.del_item(it);
   }
}


void DIJKSTRA(graph& G, node s, 
              edge_array<int>&  cost, 
              node_array<int>&  dist,
              node_array<edge>& pred)
{
  p_queue<int,node>  PQ;

  DIJKSTRA(G,s,cost,dist,pred,PQ);

 }
