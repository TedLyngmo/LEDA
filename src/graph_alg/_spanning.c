/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _spanning.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


/*******************************************************************************
*                                                                              *
*  SPANNING_TREE  (spanning tree)                                              *
*                                                                              *
*******************************************************************************/


#include <LEDA/graph_alg.h>
#include <LEDA/node_partition.h>


list<edge> SPANNING_TREE(const graph& G)
{ node v,w;
  edge e;
  list<edge> EL;
  node_partition P(G);

  forall_edges(e,G)
     { v = source(e);
       w = target(e);
       if (! P.same_block(v,w))
        { EL.append(e);
          P.union_blocks(v,w);
         }
      }

  return EL;
}

