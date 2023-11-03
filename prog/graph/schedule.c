//---------------------------------------------------------------
//
//
//		 A List Scheduling Algorithm
//
//   S. Naeher (August 1993)
//---------------------------------------------------------------

#include <LEDA/graph_alg.h>
#include <LEDA/node_pq.h>

static int compute_depth(graph& G, node_array<int>& depth);

static void make_acyclic(graph& G)
{ node_array<int> dfsnum(G);
  node_array<int> compnum(G);
  DFS_NUM(G,dfsnum,compnum);

  list<edge> back;
  edge e;

  forall_edges(e,G)
  { node v = source(e);
    node w = target(e);
    if (v == w || (dfsnum[v] > dfsnum[w] && compnum[v] < compnum[w]))
      back.append(e);
   }

  forall(e,back) G.del_edge(e);
}


void list_schedule(graph& G, int k)
{
  node u;
  node v;

  node_array<int> path(G);     // path[v] = length of longest path starting in v

  node_array<int> degree(G);   // degree[v] = current indegree of v

  list<node> zero_deg;         // set of nodes v with degree[v] = 0

  node_pq<int> Vd(G);

  forall_nodes(v,G)
  { degree[v] = G.indeg(v);
    if ( G.indeg(v) == 0) zero_deg.append(v);
   }

  int max_depth = compute_depth(G,path);

  forall(v,zero_deg) Vd.insert(v,max_depth-path[v]);
  zero_deg.clear();

  for(int level=0; !Vd.empty(); level++)
  {
    //cout << level << ": ";

    for(int i=0; i < k && !Vd.empty(); i++)
    { v = Vd.del_min();
      //G.print_node(v);
      //cout <<  " ";
      forall_adj_nodes(u,v)
      if (--degree[u] == 0) zero_deg.append(u);
     }
    //newline;

    forall(v,zero_deg) Vd.insert(v,max_depth-path[v]);
    zero_deg.clear();
  }
}



static int compute_depth(graph& G, node_array<int>& depth)
{
  // reverse all edges and compute longest paths in topological order
  // starting in nodes with indegree 0
  // PRECONDITION: G is acyclic

  G.rev();

  node_array<int>  degree(G);
  list<node>       zero_deg;
  node v;
  edge e;

  int count = 0;
  int max_depth = 0;

  forall_nodes(v,G)
  { depth[v] = 0;
    degree[v] = G.indeg(v);
    if (G.indeg(v) == 0) zero_deg.append(v);
   }

  while(! zero_deg.empty())
  { count++;
    v = zero_deg.pop();
    forall_adj_edges(e,v)
    { int  d = depth[v]+1;
      node w = G.target(e);
      if (depth[w] < d)
      { depth[w] = d;
        if (max_depth < d) max_depth = d;
       }
      if (--degree[w] == 0) zero_deg.append(w);
     }
   }

  if (count < G.number_of_nodes())
  { std::cerr << "Error: cyclic graph\n";
    exit(1);
   }

  G.rev();  // restore original graph

  return max_depth;

}



int main()
{

  graph G;

  test_graph(G);
  int k = read_int("k = ");

  make_acyclic(G);

  std::cout << "|E| = " << G.number_of_edges() << std::endl;
  newline;


  float T = used_time();
  list_schedule(G,k);
  std::cout << string(" %f sec",used_time(T));
  newline;

}
