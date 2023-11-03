#include <LEDA/graph.h>
#include <LEDA/ugraph.h>
#include <LEDA/graph_alg.h>

#include <cctype>



int main()
{
  GRAPH<string,double> G;

  random_graph(G,10,30);

  G.print();
  newline;

  edge_array<int>  cost(G);
  node_array<int>  dist(G);
  node_array<edge> pred(G);

  node_array<int>  ord(G);
  node_array<int>  compnum(G);
  edge_array<int>  flow(G) ;
  node_array<bool> reached(G,false);
  node_array<int>  dfs_num(G);
  node_array<int>  comp_num(G);
  node_array<int>  layer(G,-1);

  node_matrix<int> M(G);

  list<node> nl;
  list<edge> el;

  node       v,w,s,t;
  edge       e;


  UGRAPH<string,double> U = G;

  node_array<int> compnum1(U);

  random_source ran(0,1000);

  forall_edges(e,G)  G[e] = cost[e] = ran();



  std::cout << "TOPSORT:\n";
  if (TOPSORT(G,ord))
     std::cout << "graph is acyclic\n";
  else
     std::cout << "graph is cyclic\n";
  newline;

  newline;
  std::cout << "DFS:\n";
  newline;
  nl = DFS(G,G.choose_node(),reached);
  std::cout << "DFS:\n";
  forall(v,nl) { G.print_node(v); newline; }
  newline;

  newline;
  std::cout << "DFS_NUM:\n";
  DFS_NUM(G,dfs_num,comp_num);
  forall_nodes(v,G)
  { G.print_node(v);
    std::cout << string("  dfsnum = %2d  compnum = %2d \n",dfs_num[v],comp_num[v]);
   }
  newline;

  newline;
  std::cout << "BFS:\n";
  nl = BFS(G,G.first_node(),layer);
  forall_nodes(v,G)
  { G.print_node(v);
    std::cout << string("  layer = %2d\n",layer[v]);
   }
  newline;


  newline;
  std::cout << "COMPONENTS:\n";
  COMPONENTS(G,compnum);
  forall_nodes(v,G)
  { G.print_node(v);
    std::cout << string("  compnum = %2d \n",compnum[v]);
   }
  newline;

  newline;
  std::cout << "COMPONENTS1:\n";
  COMPONENTS1(G,compnum);
  forall_nodes(v,G)
  { G.print_node(v);
    std::cout << string("  compnum = %2d \n",compnum[v]);
   }
  newline;


  newline;
  std::cout << "TRANSITIVE_CLOSURE:\n";
  graph G1 = TRANSITIVE_CLOSURE(G);
  G1.print("Graph G1 = transitive closure of G");
  newline;


  newline;
  std::cout << "SPANNING_TREE: \n";
  el = SPANNING_TREE(G);
  forall(e,el)
    { G.print_edge(e);;
      newline;
     }
  newline;


  std::cout << "MIN_SPANNING_TREE: \n";
  el = MIN_SPANNING_TREE(G,cost);
  forall(e,el)
  { G.print_edge(e);;
    newline;
   }


  newline;
  std::cout << "STRONG_COMPONENTS:\n";
  STRONG_COMPONENTS(G,compnum);
  forall_nodes(v,G)
  { G.print_node(v);
    std::cout << string("  compnum = %d\n",compnum[v]);
   }
  newline;


  s = G.first_node();

  float T = used_time();

  newline;
  std::cout << "DIJKSTRA <int>      ";
  std::cout.flush();
  DIJKSTRA(G,s,cost,dist,pred);
  std::cout << string("%6.2f sec  \n",used_time(T));
  newline;

  std::cout << "BELLMAN_FORD <int>  ";
  std::cout.flush();
  BELLMAN_FORD(G,s,cost,dist,pred);
  std::cout << string("%6.2f sec  \n",used_time(T));
  newline;

  std::cout << "ALL PAIRS SHORTEST PATHS <int> ";
  std::cout.flush();
  ALL_PAIRS_SHORTEST_PATHS(G,cost,M);
  std::cout << string("%.2f sec\n",used_time(T));
  forall_nodes(v,G)
  { forall_nodes(w,G) std::cout << string("%7d ",M(v,w));
    newline;
   }
  newline;


  std::cout << "MAX_FLOW<int>:  ";
  std::cout.flush();
  s = G.first_node();
  t = G.last_node();
  int val = MAX_FLOW(G,s,t,cost,flow) ;
  std::cout << string("total flow = %d \n",val);
  newline;




  edge_array<double>  cost1(G);
  node_array<double>  dist1(G);
  edge_array<double>  flow1(G) ;
  node_matrix<double> M1(G);

  forall_edges(e,G)  cost1[e] = cost[e];


  used_time(T);
  std::cout << "DIJKSTRA <double>     ";
  std::cout.flush();
  DIJKSTRA(G,s,cost1,dist1,pred);
  std::cout << string("%6.2f sec  \n",used_time(T));
  newline;


  std::cout << "BELLMAN_FORD <double> ";
  std::cout.flush();
  BELLMAN_FORD(G,s,cost1,dist1,pred);
  std::cout << string("%6.2f sec  \n",used_time(T));
  newline;


  std::cout << "ALL PAIRS SHORTEST PATHS <double>  ";
  std::cout.flush();
  ALL_PAIRS_SHORTEST_PATHS(G,cost1,M1);
  std::cout << string("%.2f sec\n",used_time(T));
  newline;

  std::cout << "MAX_FLOW<double>: ";
  std::cout.flush();
  double val1 = MAX_FLOW(G,s,t,cost1,flow1) ;
  std::cout << string("total flow = %f \n",val1);
  newline;


/*
  if (PLANAR(G))
    { std::cout << "G is planar\n";
      //cout << "STRAIGHT_LINE_EMBEDDING: \n";
      //node_array<int>   xcoord(G);
      //node_array<int>   ycoord(G);
      //STRAIGHT_LINE_EMBEDDING(G,xcoord,ycoord);
      //forall_nodes(v,G)
      //{ G.print_node(v);
      //  std::cout << string("  x = %3d   y = %3d\n",xcoord[v],ycoord[v]);
      // }
      // newline;
     }
  else
    std::cout << "G is not planar\n";
*/

  newline;

  return 0;

}
