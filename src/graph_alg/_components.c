/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _components.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

/*******************************************************************************
*                                                                              *
*  COMPONENTS  (connected components)                                          *
*                                                                              *
*******************************************************************************/


#include <LEDA/graph_alg.h>
#include <LEDA/node_partition.h>
#include <LEDA/stack.h>

static int count;

static void dfs(const graph& G, node v, node_array<int>& compnum)
{
  stack<node>  S;

  S.push(v);
  compnum[v] = count;

  while (!S.empty())
   { v = S.pop();
     edge e;
     forall_inout_edges(e,v)
     { node w = G.opposite(v,e);
        if (compnum[w] == -1)
        { compnum[w] = count;
          S.push(w);
         }
      }
    }

}

int COMPONENTS(const graph& G, node_array<int>& compnum)
{ // computes the connected components of the underlying undirected graph
  // compnum[v] = i  iff v in component i
  // number of components is returned

  node v;

  forall_nodes(v,G) compnum[v] = -1;

  count = 0;

  forall_nodes(v,G)
    if (compnum[v] == -1)
    { dfs(G,v,compnum);
      count++;
     }

  return count;
}



int COMPONENTS1(const graph& G, node_array<int>& compnum)
{
  // an alternative implementation using node partitions (union-find)

  node_partition P(G);
  edge e;
  node v;

  forall_nodes(v,G) compnum[v] = -1;

  forall_edges(e,G) P.union_blocks(source(e),target(e));

  int count = 0;
  forall_nodes(v,G)
   { node w = P.find(v);
     if (compnum[w]==-1) compnum[w] = count++;
     compnum[v] = compnum[w];
    }

  return count;
}


