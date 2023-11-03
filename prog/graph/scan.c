#include <LEDA/graph.h>

main()
{
  GRAPH<int,int> G;


  node v;
  node w;
  edge e;

  test_graph(G);

  forall_edges(e,G) G[e] = 1;

  int N = 0;

  float T = used_time();

  forall_nodes(v,G)
    forall_adj_edges(e,v) N += G[e];
  cout << string(" time = %5.2f sec\n", used_time(T));

  node v_stop = G.last_node() + 1;

  for(v=G.first_node(); v != v_stop; v++)
  { edge e_stop = G.last_adj_edge(v);
    if (e_stop == nil) continue;
    e_stop++;
       for(e=G.first_adj_edge(v); e != e_stop; e++) N += G[e];
   }
  cout << string(" time = %5.2f sec\n", used_time(T));

  cout << N << endl;
  newline;

}
