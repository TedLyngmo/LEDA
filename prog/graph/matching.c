#include <LEDA/graph_alg.h>
#include <LEDA/ugraph.h>

main(int argc, char** argv)
{
  graph G;

  cmdline_graph(G,argc,argv);

  float T = used_time();

  cout << "MAX_CARD_MATCHING (heur=1)           ";
  cout.flush();
  list<edge> M = MAX_CARD_MATCHING(G,1);
  cout << string("time %.2f sec    |M| = %d\n",used_time(T), M.length());

  cout << "MAX_CARD_MATCHING (heur=2)           ";
  cout.flush();
  M = MAX_CARD_MATCHING(G,2);
  cout << string("time %.2f sec    |M| = %d\n",used_time(T), M.length());


  ugraph U = G;
  edge_array<int> cost(U);
  edge e;
  forall_edges(e,U) cost[e] = rand_int(0,1000);

  cout << "MAX_WEIGHT_MATCHING                  ";
  cout.flush();
  M = MAX_WEIGHT_MATCHING(U,cost);
  cout << string("time %.2f sec    |M| = %d\n",used_time(T), M.length());
  

  return 0;
}
