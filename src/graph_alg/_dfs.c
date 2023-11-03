/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _dfs.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


/*******************************************************************************
*                                                                              *
*  DFS  (depth first search)                                                   *
*                                                                              *
*******************************************************************************/


#include <LEDA/graph_alg.h>
#include <LEDA/b_stack.h>

list<node> DFS(const graph& G, node v, node_array<bool>& reached)
{ 
  int n = G.number_of_nodes();

  b_stack<node> S(n);
  list<node>    L;

  if (!reached[v])
  { reached[v] = true;
    S.push(v);
   }

  while (!S.empty())
  { v = S.pop(); 
    L.append(v);
    node w;
    forall_adj_nodes(w,v) 
      if (!reached[w]) 
       { reached[w] = true;
         S.push(w);
        }
   }

  return L;
 
} 
