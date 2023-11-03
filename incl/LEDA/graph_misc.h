/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph_misc.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_GRAPH_MISC_H
#define LEDA_GRAPH_MISC_H



//-----------------------------------------------------------------------------
// miscellaneous  (should be members or constructors)
//-----------------------------------------------------------------------------

#include <LEDA/graph.h>
#include <LEDA/node_array.h>
#include <LEDA/edge_array.h>

/*{\Manpage {graph\_misc} {} {Miscellaneous Graph Functions} }*/


extern bool Is_Bidirected(const graph& G, edge_array<edge>& rev);
/*{\Mfuncl  	computes for every edge $e = (v,w)$ in $G$ its
		reversal $rev[e] = (w,v)$ in $G$ (nil if
		not present). Returns true if every edge has a
		reversal and false otherwise. }*/


extern bool Is_Simple(graph& G);
/*{\Mfunc  returns true if $G$ is simple, i.e., has no parallel
           edges, false otherwise. }*/

extern void Make_Simple(graph& G);
/*{\Mfunc  makes $G$ simple by removing one of each pair of
           parallel edges from $G$. }*/



// for historical reasons:

inline bool compute_correspondence(const graph& G, edge_array<edge>& rev)
{ return Is_Bidirected(G,rev); }

inline void eliminate_parallel_edges(graph& G) { Make_Simple(G); }


#endif

