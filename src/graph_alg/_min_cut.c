/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _min_cut.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


/* This file has been automatically generated from "min_cut.w"
   by CTANGLE (Version 3.1 [km2c]) */

#include <LEDA/graph_alg.h>
#include <LEDA/ugraph.h>
#include <LEDA/stream.h>
#include <LEDA/node_pq.h>

list <node >MIN_CUT(const graph &G0, edge_array <int >&weight)
{
  typedef list <node >nodelist;
  UGRAPH < nodelist *, int >G;
  node v, x;
  edge e;
  node_array <node >partner(G0);
  forall_nodes (x, G0) {
    partner[x] = G.new_node(new nodelist);
    G[partner[x]]->append(x);
  }

  forall_edges (e, G0)
    if (source(e) != target(e))
      G.new_edge(partner[source(e)], partner[target(e)], weight[e]);



  node a = G.first_node();
  int n = G.number_of_nodes();
  list <node >best_cut;
  int best_value = MAXINT;
  int cut_weight;



/* |n| is now the number of nodes of the current graph and |a|
     is a fixed vertex */
  while (n >= 2) {

    node t = a;
    node s;
    node_array <bool >in_PQ(G);

    node_pq <int >PQ(G);

    forall_nodes (v, G)
      if (v != a) {
	PQ.insert(v, 0);	/* $w(v,A) = 0$ if there is no edge
				 * connecting |v| to |A| */
	in_PQ[v] = true;
      }
      else {
	in_PQ[v] = false;
      }
    {
      forall_adj_edges (e, a)
	PQ.decrease_inf(G.opposite(a, e), PQ.inf(G.opposite(a, e)) - G[e]);
    }
    while (!PQ.empty()) {
      s = t;
      cut_weight = -PQ.inf(PQ.find_min());
      t = PQ.del_min();
      in_PQ[t] = false;
      forall_adj_edges (e, t) {
	if (in_PQ[v = G.opposite(t, e)])
	  PQ.decrease_inf(v, PQ.inf(v) - G[e]);

      }
    }

    ;

    if (cut_weight < best_value) {
      best_cut = *(G[t]);
      best_value = cut_weight;
    }

    G[s]->conc(*(G[t]));

    node_array <edge >s_edge(G, nil);
    {
      forall_adj_edges (e, s)
	s_edge[G.opposite(s, e)] = e;
    }
    {
      forall_adj_edges (e, t) {
	if (s_edge[v = G.opposite(t, e)] == nil)
	  G.new_edge(s, v, G[e]);
	else
	  G[s_edge[v]] += G[e];
      }
    }

    delete G[t];
    G.del_node(t);


    ;
    n--;


  }

  return best_cut;


}
