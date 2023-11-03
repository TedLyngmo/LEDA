#include <LEDA/graph_alg.h>
#include <LEDA/ugraph.h>

int main(int argc, char** argv)
{
  graph G;

  cmdline_graph(G,argc,argv);

  float T = used_time();

  std::cout << "MAX_CARD_MATCHING (heur=1)           ";
  std::cout.flush();
  list<edge> M = MAX_CARD_MATCHING(G,1);
  std::cout << string("time %.2f sec    |M| = %d\n",used_time(T), M.length());

  std::cout << "MAX_CARD_MATCHING (heur=2)           ";
  std::cout.flush();
  M = MAX_CARD_MATCHING(G,2);
  std::cout << string("time %.2f sec    |M| = %d\n",used_time(T), M.length());


  ugraph U = G;
  edge_array<int> cost(U);
  edge e;
  forall_edges(e,U) cost[e] = rand_int(0,1000);

  std::cout << "MAX_WEIGHT_MATCHING                  ";
  std::cout.flush();
  M = MAX_WEIGHT_MATCHING(U,cost);
  std::cout << string("time %.2f sec    |M| = %d\n",used_time(T), M.length());


  return 0;
}
