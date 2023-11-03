/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph_gen.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_GRAPH_GEN_H
#define LEDA_GRAPH_GEN_H

//-----------------------------------------------------------------------------
// graph generators
//-----------------------------------------------------------------------------

#include <LEDA/graph.h>
#include <LEDA/node_array.h>


/*{\Manpage {} {graph_gen} {Graph Generators} }*/


extern void complete_graph(graph& G, int n);
/*{\Mfuncl   creates a complete graph $G$ with $n$ nodes. }*/


extern void random_graph(graph& G, int n, int m);
/*{\Mfuncl   creates a random graph $G$ with $n$ nodes and $m$ edges. }*/


extern void test_graph(graph& G);
/*{\Mfuncl   creates interactively a user defined graph $G$. }*/


extern void complete_bigraph(graph& G, int a, int b, list<node>& A, list<node>& B);
/*{\Mfuncl   creates a complete bipartite graph $G$ with $a$ nodes
            on side $A$ and $b$ nodes on side $B$. All edges are
            directed from $A$ to $B$. }*/

extern void random_bigraph(graph& G, int a, int b, int m, list<node>& A, list<node>& B);
/*{\Mfuncl   creates a random bipartite graph $G$ with $a$ nodes on
            side $A$, $b$ nodes on side $B$, and $m$ edges. All
            edges are directed from $A$ to $B$. }*/


extern void test_bigraph(graph& G, list<node>& A, list<node>& B);
/*{\Mfuncl   creates interactively a user defined bipartite
	    graph $G$ with sides $A$ and $B$. All edges are
	    directed from $A$ to $B$. }*/




extern void random_planar_graph(graph& G, int n);
/*{\Mfuncl   creates a random planar graph $G$ with $n$ nodes. }*/

extern void random_planar_graph(graph& G, node_array<double>& xcoord, node_array<double>& ycoord, int n);

/* cheat the manual extractor:
void random_planar_graph(graph& G, node_array<double>& xcoord, ycoord, int n);
*/
/*{\Mfuncl   creates a random planar graph $G$ with $n$ nodes embedded into
            the unit sqare. The embedding is given by $xcoord[v]$ and
            $ycoord[v]$ for every node $v$ of $G$. }*/


extern void triangulated_planar_graph(graph& G, int n);
/*{\Mfuncl   creates a triangulated planar graph $G$ with $n$ nodes. }*/


extern void triangulated_planar_graph(graph& G, node_array<double>& xcoord, node_array<double>& ycoord, int n);

/*
void triangulated_planar_graph(graph& G, node_array<double>& xcoord, ycoord, int n);
*/
/*{\Mfuncl   creates a triangulated planar graph $G$ with $n$ nodes embedded
            into the unit sqare. The embedding is given by $xcoord[v]$ and
            $ycoord[v]$ for every node $v$ of $G$. }*/


extern void grid_graph(graph& G,int n);
/*{\Mfuncl   creates a grid graph $G$ of size $n\times n$ nodes. }*/


extern void grid_graph(graph&,node_array<double>& xcoord, node_array<double>& ycoord, int n);
/*{\Mfuncl   creates a grid graph $G$ of size $n\times n$ nodes embedded
            into the unit sqare. The embedding is given by $xcoord[v]$ and
            $ycoord[v]$ for every node $v$ of $G$. }*/



extern void cmdline_graph(graph& G, int argc, char** argv);
/*{\Mfuncl   builds graph $G$ as specified by the command line arguments:\\
prog \hbox to 1.5cm{\hfill}       $\longrightarrow$ test\_graph()\\
prog \hbox to 1.5cm{$n$\hfill}    $\longrightarrow$ complete\_graph($n$)\\
prog \hbox to 1.5cm{$n$ $m$\hfill} $\longrightarrow$ test\_graph($n,m$)\\
prog \hbox to 1.5cm{$file$\hfill} $\longrightarrow$ $G$.read\_graph($file$).
}*/



#endif

