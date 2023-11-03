/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _min_spanning.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


/*******************************************************************************
 *
 *                          MIN_SPANNING_TREE
 *
 *      Kruskal's Algorithm for computing a minimum spanning tree
 *
 * In order to avoid sorting the entire edge set we proceed as follows:
 * We first run Kruskal's Algorithm with the 3*n cheapest edges of the graph. 
 * In general, this gives already a good approximation. Then we collect all 
 * edges still connecting different components and run the algorithm for them.
 * 
 *       worst case running time:  m * log n
 * ????? expected running time:    m * alpha(n) + n * log n  ?????
 *
 * S.N. (1992)
 *
 ******************************************************************************/


#include <LEDA/graph_alg.h>
#include <LEDA/node_partition.h>
#include <LEDA/node_pq.h>
#include <LEDA/basic_alg.h>


static const edge_array<num_type>* edge_cost;

static int CMP_EDGES(const edge& e1, const edge& e2) 
{ return compare((*edge_cost)[e1],(*edge_cost)[e2]); }


static void KRUSKAL(list<edge>& L, node_partition& P, list<edge>& T)
{ 
  L.sort(CMP_EDGES);
  edge e;
  forall(e,L)
  { node v = source(e);
    node w = target(e);
    if (! P.same_block(v,w))
    { T.append(e);
      P.union_blocks(v,w);
     }
   }

 }

list<edge> MIN_SPANNING_TREE(const graph& G, const edge_array<num_type>& cost)
{ 
  list<edge> T;
  list<edge> L;
  node_partition P(G);
  edge e;

  edge_cost = &cost;

  int n = G.number_of_nodes();
  int m = G.number_of_edges();

/* Compute 3n-th biggest edge cost x by SELECT (from basic_alg.h)
 * and sort all edges with cost smaller than x in a list L
 */

  if (m > 3*n)
   { num_type* c = new num_type[m];
     num_type* p = c;
     forall_edges(e,G) *p++ = cost[e];

     num_type x = SELECT(c,p-1,3*n);

     delete c;

     forall_edges(e,G) 
        if (cost[e] < x) L.append(e);
    }
  else
    L = G.all_edges();


  KRUSKAL(L,P,T);


// collect and sort edges still connecting different trees into L
// and run the algorithm on L

  L.clear();

  forall_edges(e,G) 
     if (!P.same_block(source(e),target(e))) L.append(e);

  KRUSKAL(L,P,T);

  return T;
}




/*******************************************************************************
 *
 *                          MIN_SPANNING_TREE1
 *
 * priority-queue-based algorithm  for undirected graphs
 *
 * for details see Mehlhorn Vol. II, Section IV.8, Theorem 2
 *
 * Running time with Fibonnaci heap:  O(m + n * log n)   
 *                                    ( m * decrease_p + n * del_min)
 *
 * S.N. (1992)
 *
 ******************************************************************************/



list<edge> MIN_SPANNING_TREE1(const graph& G, const edge_array<num_type>& cost)
{
  // computes a minimum spanning tree of the underlying undirected graph

  list<edge> result;

  node_pq<num_type> PQ(G); 

  node_array<num_type> dist(G,MAXINT); 
                                 // dist[v] = current distance value of v
                                 // MAXINT: no edge adjacent to v visited
                                 //-MAXINT: v already in a tree

  node_array<edge> T(G,nil);      // T[v] = e with cost[e] = dist[v]


  node v;
  forall_nodes(v,G)  PQ.insert(v,MAXINT);

  while (! PQ.empty())
  { 
    node u = PQ.del_min();

    if (dist[u] != MAXINT) result.append(T[u]);

    dist[u] = -MAXINT;

    edge e;
    forall_inout_edges(e,u)
    { v = G.opposite(u,e);
      num_type c = cost[e];
      if (c < dist[v])
      { PQ.decrease_p(v,c);
        dist[v] = c;
        T[v] = e;
       }
     }
   }

 return result;
}
