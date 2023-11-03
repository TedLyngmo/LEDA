#define LEDA_CHECKING_OFF


#include <LEDA/graph_alg.h>
#include <LEDA/_p_queue.h>



void dijkstra(graph& G,
              node s,
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


#include <LEDA/impl/k_heap.h>
#include <LEDA/impl/bin_heap.h>
#include <LEDA/impl/m_heap.h>
#include <LEDA/impl/p_heap.h>
#include <LEDA/impl/r_heap.h>
#include <LEDA/impl/list_pq.h>



int main()
{
  GRAPH<int,int> G;

  for(;;)
  {

  int n = read_int("# nodes = ");
  int m = read_int("# edges = ");

  if (n==0) break;

  random_graph(G,n,m);

  edge_array<int>  cost(G);
  node_array<int>  dist0(G);
  node_array<int>  dist(G);
  node_array<edge> pred(G);

  int M = read_int("max edge cost = ");

  node s = G.choose_node();

  edge e;
  forall_edges(e,G) G[e] = cost[e] = rand_int(0,M);

  p_queue<int,node>* PQ[8];

  PQ[0] = new p_queue<int,node>;

  PQ[1] = new _p_queue<int,node,k_heap>(n);
  PQ[2] = new _p_queue<int,node,m_heap>(M);
  PQ[3] = new _p_queue<int,node,list_pq>;
  PQ[4] = new _p_queue<int,node,p_heap>;
  PQ[5] = new _p_queue<int,node,r_heap>(M);
  PQ[6] = new _p_queue<int,node,bin_heap>(n);

  float T  = used_time();
  std::cout << "DIJKSTRA: ";
  std::cout.flush();
  DIJKSTRA(G,s,cost,dist0,pred);
  std::cout << string(" %6.2f sec\n",used_time(T));
  newline;

  string choice = "0:f_heap 1:k_heap 2:m_heap 3:list_pq 4:p_heap 5:r_heap 6:bin_heap --> ";

  for(;;)
  { int i = read_int(choice);

    if (i>6) break;

    float T  = used_time();
    dijkstra(G,s,cost,dist,pred,*(PQ[i]));

    std::cout << string("time: %6.2f sec\n",used_time(T));
    newline;

    node v;
    forall_nodes(v,G)
       if( dist[v] != dist0[v])
       { G.print_node(v);
         std::cout << string("   dist =  %d   dist0 = %d\n",dist[v],dist0[v]);
        }

   }

 }

 return 0;
}
