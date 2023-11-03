
/* This file has been automatically generated from "r_heap.w"
   by CTANGLE (Version 3.1 [km2c]) */




#include "r_heap.h"
#include <LEDA/random.h>
#include <LEDA/p_queue.h>
#include <LEDA/_p_queue.h>
#include <LEDA/graph.h>
#include <LEDA/graph_alg.h>
#include <fstream.h>
#include <string.h>
#include <math.h>




void dijkstra(graph &G,
   node s,
   edge_array <int >&cost,
   node_array <int >&dist,
   node_array <edge >&pred,
   p_queue < int, node >&PQ)
{

  node_array <pq_item >I(G);
  node v;

  forall_nodes (v, G) {
    pred[v] = nil;
    dist[v] = MAXINT;
  }

  dist[s] = 0;
  I[s] = PQ.insert(0, s);

  while (!PQ.empty()) {

    pq_item it = PQ.find_min();
    node u = PQ.inf(it);
    int du = dist[u];
    edge e;
    forall_adj_edges (e, u) {
      v = G.target(e);
      int c = du + cost[e];
      if (c < dist[v]) {
	if (dist[v] == MAXINT)
	  I[v] = PQ.insert(c, v);
	else
	  PQ.decrease_p(I[v], c);
	dist[v] = c;
	pred[v] = e;
      }
    }
    PQ.del_item(it);
  }
}




int main(void)
{
  GRAPH <int, int >G;
  int n = read_int("# nodes = ");
  int m = read_int("# edges = ");
  random_graph(G, n, m);
  edge_array <int >cost(G);
  node_array <int >dist(G);
  node_array <edge >pred(G);
  int M = read_int("max edge cost = ");
  node s = G.choose_node();
  edge e;
  forall_edges (e, G)
    G[e] = cost[e] = rand_int(0, M);
  p_queue < int, node >*PQ[2];
  PQ[0] = new _p_queue < int, node, f_heap >;
  PQ[1] = new _p_queue < int, node, r_heap > (M);
  float T = used_time();
  float t_f = 0.0, t_r = 0.0;
  dijkstra(G, s, cost, dist, pred, *(PQ[0]));
  t_f = used_time(T);
  dijkstra(G, s, cost, dist, pred, *(PQ[1]));
  t_r = used_time(T);
  cout << string ("f_heap: %6.2f sec, r_heap: %6.2f sec\n", t_f, t_r);
}
