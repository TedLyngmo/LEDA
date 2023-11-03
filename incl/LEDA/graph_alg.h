/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph_alg.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_GRAPHALG_H
#define LEDA_GRAPHALG_H

#include <LEDA/graph.h>
#include <LEDA/node_matrix.h>


/*{\Manpage {graph_alg} {} {Graph Algorithms}}*/

/*{\Mtext
\bigskip
This section gives a summary of the graph algorithms contained in LEDA.
All algorithms are generic, i.e., they accept instances of any user defined
parameterized graph type $GRAPH${\tt <}$vtype,etype${\tt >} as arguments.
The header file {\tt <}LEDA/graph\_alg.h{\tt >} has to be included.
}*/


//-----------------------------------------------------------------------------
// basic graph algorithms:
//-----------------------------------------------------------------------------

/*{\Mtext
\bigskip
\subsection{Basic Algorithms}

\label{Basic Algorithms}
}*/


bool        TOPSORT(const graph& G, node_array<int>& ord);
/*{\Mtext
$\bullet$ {\bf Topological Sorting}

\medskip
$bool$ TOPSORT($graph\&\ G,\ node\_array\<int\>\&\ ord$)

\medskip
TOPSORT takes as argument a directed graph $G(V,E)$. It sorts $G$ topologically
(if $G$ is acyclic) by computing for every node $v \in V$ an integer $ord[v]$
such that $1\le ord[v]\le |V|$ and $ord[v] < ord[w]$ for all edges
$(v,w) \in E$. TOPSORT returns true if $G$ is acyclic and false otherwise.

The algorithm (\cite{Ka62}) has running time $O(|V|+|E|)$.
}*/


bool        TOPSORT1(graph& G);

/*{\Mtext
\medskip

$\bullet$ {\bf Depth First Search}
}*/

list<node>  DFS(const graph& G, node s, node_array<bool>& reached) ;
/*{\Mtext
$list\<node\>$  DFS($graph\&\ G,\ node\ s,\
node\_array\<bool\>\&\ reached$)

\medskip
DFS takes as argument a directed graph $G(V,E)$, a node $s$ of $G$ and a
node\_array $reached$ of boolean values. It performs a depth first search
starting at $s$ visiting all reachable nodes $v$ with $reached[v]$ = false.
For every visited node $v$ $reached[v]$ is changed to true. DFS returns the
list of all reached nodes.

The algorithm (\cite{T72}) has running time $O(|V|+|E|)$.
}*/

list<edge>  DFS_NUM(const graph& G, node_array<int>& dfsnum,
                                    node_array<int>& compnum);
/*{\Mtext
\bigskip
$list\<edge\>$
\parbox[t]{12.5cm}{DFS\_NUM($graph\&\ G, node\_array\<int\>\&\
dfsnum, node\_array\<int\>\&$ $compnum$)}

\medskip
DFS\_NUM takes as argument a directed graph $G(V,E)$. It performs a
depth first search of $G$ numbering the nodes of $G$ in two different ways.
$dfsnum$ is a numbering with respect to the calling time and $compnum$ a
numbering with respect to the completion time of the recursive calls. DFS\_NUM
returns a depth first search forest of $G$ (list of tree edges).

The algorithm (\cite{T72}) has running time $O(|V|+|E|)$.
 }*/


list<node>  BFS(const graph& G, node s, node_array<int>& dist);
/*{\Mtext
\bigskip
$\bullet$ {\bf Breadth First Search}

\medskip
$list\<node\>$
BFS($graph\&\ G,\ node\ s,\ node\_array\<int\>\&\ dist$)

\medskip
BFS takes as argument a directed graph $G(V,E)$ and a node $s$ of $G$. It
performs a breadth first search starting at $s$ computing for every visited
node $v$ the distance $dist[v]$ from $s$ to $v$. BFS returns the list of all
reached nodes.

The algorithm (\cite{M84}) has running time $O(|V|+|E|)$.
}*/

/*{\Mtext
\bigskip
$\bullet$ {\bf Connected Components}
}*/

int         COMPONENTS(const graph& G, node_array<int>& compnum);
/*{\Mtext
$int$   COMPONENTS($graph\&\ G,\ node\_array\<int\>\&\ compnum$)

\medskip
COMPONENTS takes a graph $G(V,E)$ as argument and computes the connected
components of the underlying undirected graph, i.e., for every node $v\in V$
an integer $compnum[v]$ from $[0\dots c-1]$ where $c$ is the number of
connected components of $G$ and $v$ belongs to the $i$-th connected
component iff $compnum[v] = i$.  COMPONENTS returns $c$.

The algorithm (\cite{M84}) has running time $O(|V|+|E|)$.
}*/


int         COMPONENTS1(const graph& G, node_array<int>& compnum);

int         STRONG_COMPONENTS(const graph& G, node_array<int>& compnum);
/*{\Mtext
\bigskip
$\bullet$ {\bf Strong Connected Components}

\medskip
$int$   STRONG\_COMPONENTS($graph\&\ G,\ node\_array\<int\>\&\ compnum
$)

\medskip
STRONG\_COMPONENTS takes a directed graph $G(V,E)$ as argument and computes for
every node $v\in V$ an integer $compnum[v]$ from $[0\dots c-1]$ where
$c$ is the number of strongly connected components of $G$ and
$v$ belongs to the $i$-th strongly connected component iff $compnum[v] = i$.
STRONG\_COMPONENTS returns $c$.

The algorithm (\cite{M84}) has running time $O(|V|+|E|)$.
}*/


int         STRONG_COMPONENTS1(const graph& G, node_array<int>& compnum);

int         BICONNECTED_COMPONENTS(const graph& G, edge_array<int>& compnum);

/*{\Mtext
\bigskip
$\bullet$ {\bf Transitive Closure}
}*/

graph       TRANSITIVE_CLOSURE(const graph& G);
/*{\Mtext
$graph$ TRANSITIVE\_CLOSURE($graph\&\ G$)

\medskip
TRANSITIVE\_CLOSURE takes a directed graph $G(V,E)$ as argument and computes
the transitive closure of $G(V,E)$. It returns a directed graph $G'(V',E')$
with $V' = V$ and $(v,w) \in E'  \Leftrightarrow$ there is a path form
$v$ to $w$ in $G$.

The algorithm (\cite{GK79}) has running time $O(|V|\cdot |E|)$.
}*/




/*{\Mtext
\subsection{Network Algorithms}

\label{Network Algorithms}

Most of the following network algorithms are overloaded. They work
for both integer and real valued edge costs.
}*/

//-----------------------------------------------------------------------------
// shortest paths:
//-----------------------------------------------------------------------------
/*{\Mtext
\bigskip
$\bullet$ {\bf Single Source Shortest Paths }
}*/


void DIJKSTRA(const graph& G, node s, const edge_array<int>& cost,
                                            node_array<int>& dist,
                                            node_array<edge>& pred);
/*{\Mtext
$void$
\parbox[t]{14cm}{DIJKSTRA($graph\&\ G,\ node\ s$, $edge\_array\<int\>$ $cost$,
                 $node\_array\<int\>$ $dist$,
		 $node\_array\<edge\>$ $pred$)}

\smallskip
$void$
\parbox[t]{14.5cm}{DIJKSTRA($graph\&\ G,\ node\ s$, $edge\_array\<double\>$ $cost$,
                  $node\_array\<double\>$ $dist$,
                  $node\_array\<edge\>$  $pred$)}

\medskip
DIJKSTRA takes as arguments a directed graph $G(V,E)$, a source node $s$ and an
edge\_array $cost$ giving for each edge in $G$ a non-negative cost. It
computes for each node $v$ in $G$ the distance $dist[v]$ from $s$ (cost of the
least cost path from $s$ to $v$) and the predecessor edge $pred[v]$ in the
shortest path tree.

The algorithm (\cite{Di59}, \cite{FT87}) has running time $O(|E|+|V|\log |V|)$.
}*/


bool BELLMAN_FORD(const graph& G, node s, const edge_array<int>& cost,
                                                node_array<int>& dist,
                                                node_array<edge>& pred);
/*{\Mtext
\bigskip
$bool$
\parbox[t]{13.5cm}{BELLMAN\_FORD($graph\&\ G,\ node\ s$,
                 $edge\_array\<int\>$ $cost$,
		 $node\_array\<int\>$ $dist$,
		 $node\_array\<int\>$ $pred$)}


\smallskip
$bool$
\parbox[t]{12cm}{BELLMAN\_FORD($graph\&\ G,\ node\ s$,
                $edge\_array\<double\>$ $cost$,
		$node\_array\<double\>$ $dist$,
		$node\_array\<edge\>$ $pred$)}

\medskip
BELLMAN\_FORD takes as arguments a graph $G(V,E)$, a source node $s$ and an
edge\_array $cost$ giving for each edge in $G$ a real (integer) cost. It
computes for each node $v$ in $G$ the distance $dist[v]$ from $s$ (cost of
the least cost path from $s$ to $v$) and the predecessor edge $pred[v]$ in
the shortest path tree. BELLMAN\_FORD returns false if there is a negative
cycle in $G$ and true otherwise

The algorithm (\cite{Be58}) has running time $O(|V|\cdot |E|)$.
}*/


bool ALL_PAIRS_SHORTEST_PATHS(graph& G, const edge_array<int>&  cost,
                                              node_matrix<int>& dist);
/*{\Mtext
\bigskip
$\bullet$ {\bf All Pairs Shortest Paths }

\medskip
$bool$
\parbox[t]{12cm}{ALL\_PAIRS\_SHORTEST\_PATHS($graph\&\ G$,
                 $edge\_array\<int\>\&$ $cost$,
		 $node\_matrix\<int\>\&$ $dist$)}

\smallskip
$bool$
\parbox[t]{12cm}{ALL\_PAIRS\_SHORTEST\_PATHS($graph\&\ G$,
                 $edge\_array\<double\>\&$ $cost$,
		 $node\_matrix\<double\>\&$ $dist$)}

\medskip
ALL\_PAIRS\_SHORTEST\_PATHS takes as arguments a graph $G(V,E)$ and an
edge\_array $cost$ giving for each edge in $G$ a real (integer) valued cost.
It computes for each node pair $(v,w)$ of $G$ the distance $dist(v,w)$ from
$v$ to $w$ (cost of the least cost path from $v$ to $w$).
ALL\_PAIRS\_SHORTEST\_PATHS returns false if there is a negative cycle in $G$
and true otherwise.

The algorithm (\cite{Be58}, \cite{Fl62}) has running time $O(|V|\cdot |E| + |V|^2
 \log|V|)$.
}*/





//-----------------------------------------------------------------------------
// maximum flow:
//-----------------------------------------------------------------------------

/*{\Mtext
\bigskip
$\bullet$ {\bf Maximum Flow }
}*/



int  MAX_FLOW(graph& G, node s, node t, const edge_array<int>& cap,
                                              edge_array<int>& flow);
/*{\Mtext
$int$
\parbox[t]{11cm}{MAX\_FLOW($graph\&\ G,\ node\ s,\ node\ t$,
                 $edge\_array\<int\>\&\ cap$,
		 $edge\_array\<int\>\&\ flow$)}

\smallskip
$int$
\parbox[t]{11.5cm}{MAX\_FLOW($graph\&\ G,\ node\ s,\ node\ t$,
                 $edge\_array\<double\>\&\ cap$,
		 $edge\_array\<double\>\&\ flow$)}

\medskip
MAX\_FLOW takes as arguments a directed graph $G(V,E)$, a source node $s$, a
sink node $t$ and an edge\_array $cap$ giving for each edge in $G$ a capacity.
It computes for every edge $e$ in $G$ a flow $flow[e]$ such that the total
flow from $s$ to $t$ is maximal and $flow[e] \le cap[e]$ for all edges $e$.
MAX\_FLOW returns the total flow from $s$ to $t$.

The algorithm (\cite{GT88}) has running time $O(|V|^3)$.
}*/


//-----------------------------------------------------------------------------
// min cost flow:
//-----------------------------------------------------------------------------

int MIN_COST_MAX_FLOW(graph& G, node s, node t, const edge_array<int>& cap,
                                                const edge_array<int>& cost,
                                                edge_array<int>& flow);
/*{\Mtext
$int$
\parbox[t]{11cm}{MIN\_COST\_MAX\_FLOW($graph\&\ G,\ node\ s,\ node\ t$,
                 $edge\_array\<int\>\&\ cap$,
                 $edge\_array\<int\>\&\ cost$,
		 $edge\_array\<int\>\&\ flow$)}

\medskip
MIN\_COST\_MAX\_FLOW takes as arguments a directed graph $G(V,E)$, a source
node $s$, a sink node $t$, an edge\_array $cap$ giving for each edge in $G$ a
capacity, and an edge\_array $cost$ specifying for each edge an integer cost.
It computes for every edge $e$ in $G$ a flow $flow[e]$ such that the total
flow from $s$ to $t$ is maximal, the total cost of the flow is minimal,
and $flow[e] \le cap[e]$ for all edges $e$.
MIN\_CONST\_MAX\_FLOW returns the total flow from $s$ to $t$.
}*/

//-----------------------------------------------------------------------------
// minimum cut :
//-----------------------------------------------------------------------------

// Stoer and Wagner's algorithm

/*{\Mtext
\bigskip
$\bullet$ {\bf Minimum Cut}
}*/

list<node> MIN_CUT(const graph& G, edge_array<int>& weight);

/*{\Mtext
$list\<node\>$ MIN\_CUT($graph\&\ G$, $edge\_array\<int\>\&\ weight$)

\medskip
MIN\_CUT($G$, $weight$) takes as arguments a graph $G$ and an
edge\_array
giving for each edge an integer weight. The algorithm
(\cite{SW94}) computes
the cut of minimum weight and returns it as a list of nodes. It
has running time
$O(|V|\cdot |E| + |V|^2 \log |V|)$.
}*/

//-----------------------------------------------------------------------------
// matchings:
//-----------------------------------------------------------------------------

// Edmond's algorithm

/*{\Mtext
\bigskip
$\bullet$ {\bf Maximum Cardinality Matching}
}*/


list<edge> MAX_CARD_MATCHING(graph& G, int heur = 1);
/*{\Mtext
$list\<edge\>$ MAX\_CARD\_MATCHING($graph\&\ G$)

\medskip
MAX\_CARD\_MATCHING($G$) computes a maximum cardinality matching of $G$, i.e.,
a maximal set of edges $M$ such that no two edges in $M$ share an end point.
It returns $M$ as a list of edges.

The algorithm (\cite{E65}, \cite{T83}) has running time $O(|V|\cdot |E|\cdot\alpha(|E|))$.
}*/



// Hopcroft/Karp

/*{\Mtext
\bigskip
$\bullet$ {\bf Maximum Cardinality Bipartite Matching}
}*/


list<edge> MAX_CARD_BIPARTITE_MATCHING(graph& G, const list<node>& A, const list<node>& B);
/*{\Mtext
$list{\tt <}edge\>$ MAX\_CARD\_BIPARTITE\_MATCHING($graph\&\ G$,
               $list\<node\>\&\ A$,
	       $list\<node\>\&\ B$)

\medskip
MAX\_CARD\_BIPARTITE\_MATCHING takes as arguments a directed graph $G(V,E)$ and
two lists $A$ and $B$ of nodes. All edges in $G$ must be directed from nodes
in $A$ to nodes in $B$. It returns a maximum cardinality matching of $G$.

The algorithm (\cite{HK75}) has running time $O(|E|\sqrt{|V|})$.
}*/


list<edge> MAX_CARD_BIPARTITE_MATCHING(graph& G);

list<edge> MAX_CARD_BIPARTITE_MATCHING1(graph& G, const list<node>& A, const list<node>& B);

list<edge> MAX_CARD_BIPARTITE_MATCHING1(graph& G);

/*{\Mtext
\bigskip
$\bullet$ {\bf Maximum Weighted Matching}
}*/

list <edge> MAX_WEIGHT_MATCHING (const ugraph&,
                                 const edge_array<int>&);

/*{\Mtext
$list\<edge\>$
\parbox[t]{12.5cm}{MAX\_WEIGHT\_MATCHING($ugraph\&\ G$,
		   $edge\_array\<int\>\&\ weight$)}

\smallskip
$list\<edge\>$
\parbox[t]{12.5cm}{MAX\_WEIGHT\_MATCHING($ugraph\&\ G$,
		 $edge\_array\<double\>\&\ weight$)}

\medskip
MAX\_WEIGHT\_MATCHING takes as arguments an undirected graph $G$
and an edge\_array giving for each edge an
integer (real) weight.
It computes a maximum weighted matching of $G$, i.e., a set of edges $M$
such that the sum of weights of all edges in $M$ is maximal and no two edges
in $M$ share an end point. MAX\_WEIGHT\_MATCHING returns $M$ as a
list of edges.

The algorithm (\cite{E65}, \cite{G74}, \cite{L76}) has running
time $O(|V|^3)$.
}*/

/*{\Mtext
\bigskip
$\bullet$ {\bf Maximum Weight Bipartite Matching}
}*/


list<edge> MAX_WEIGHT_BIPARTITE_MATCHING(graph& G, const list<node>&A,
                                                   const list<node>&B,
                                                   const edge_array<int>&);

/*{\Mtext
$list\<edge\>$
\parbox[t]{12.5cm}{MAX\_WEIGHT\_BIPARTITE\_MATCHING($graph\&\ G$,
                   $list\<node\>\&\ A$,
		   $list\<node\>\&\ B$,
		   $edge\_array\<int\>\&\ weight$)}

\smallskip
$list\<edge\>$
\parbox[t]{12.5cm}{MAX\_WEIGHT\_BIPARTITE\_MATCHING($graph\&\ G$,
                 $list\<node\>\&\ A$,
		 $list\<node\>\&\ B$,
		 $edge\_array\<double\>\&\ weight$)}

\medskip
MAX\_WEIGHT\_BIPARTITE\_MATCHING takes as arguments a directed graph $G$,
two lists $A$ and $B$ of nodes and an edge\_array giving for each edge an
integer (real) weight. All edges in $G$ must be directed from nodes in $A$
to nodes in $B$.
It computes a maximum weight bipartite matching of $G$, i.e., a set of edges $M$
such that the sum of weights of all edges in $M$ is maximal and no two edges
in $M$ share an end point. MAX\_WEIGHT\_BIPARTITE\_MATCHING returns $M$ as a
list of edges.

The algorithm (\cite{FT87}) has running time $O(|V|\cdot |E|)$.
}*/




//-----------------------------------------------------------------------------
// spanning trees:
//-----------------------------------------------------------------------------

/*{\Mtext
\bigskip
$\bullet$ {\bf Spanning Tree}
}*/

list<edge> SPANNING_TREE(const graph& G);
/*{\Mtext
$list\<edge\>$ SPANNING\_TREE($graph\&\ G$)

\medskip
SPANNING\_TREE takes as argument a graph $G(V,E)$. It computes a spanning
tree $T$ of of the underlying undirected graph, SPANNING\_TREE returns the
list of edges of $T$.

The algorithm (\cite{M84}) has running time $O(|V|+|E|)$.
}*/


/*{\Mtext
\bigskip
$\bullet$ {\bf Minimum Spanning Tree}
}*/

list<edge> MIN_SPANNING_TREE(const graph& G, const edge_array<int>& cost);
/*{\Mtext
\medskip
$list\<edge\>$ MIN\_SPANNING\_TREE($graph\& G,\ edge\_array\<int\>\&\ cost$)
}*/

list<edge> MIN_SPANNING_TREE1(const graph& G, const edge_array<int>& cost);
/*{\Mtext
\smallskip
$list\<edge\>$ MIN\_SPANNING\_TREE1($graph\& G,\ edge\_array\<double\>\&\ cost$)

\medskip
MIN\_SPANNING\_TREE takes as argument an undirected graph $G(V,E)$ and an
edge\_array $cost$ giving for each edge an integer cost.
It computes a minimum spanning
tree $T$ of $G$, i.e., a spanning tree such that the sum of all edge costs
is minimal. MIN\_SPANNING\_TREE returns the list of edges of $T$.

The algorithm (\cite{Kr56}) has running time $O(|E|\log|V|)$.
}*/




//-----------------------------------------------------------------------------
// planar graphs
//-----------------------------------------------------------------------------

/*{\Mtext
\subsection{Algorithms for Planar Graphs}

\label{Algorithms for Planar Graphs}
}*/


/*{\Mtext
\bigskip
$\bullet$ {\bf Planarity Test}
}*/

bool PLANAR(graph&, bool embed=false);

/*{\Mtext
$bool$ PLANAR($graph\&\ G,\ bool\ embed=false$)

\medskip
PLANAR takes as input a directed graph $G(V,E)$ and performs a planarity test
for $G$. If the second argument $embed$ has value $true$ and $G$ is a planar
graph it is transformed into a planar map (a combinatorial embedding such that
the edges in all adjacency lists are in clockwise ordering). PLANAR returns
true if $G$ is planar and false otherwise.

The algorithm (\cite{HT74}) has running time $O(|V|+|E|)$.
}*/


bool PLANAR(graph&, list<edge>&, bool embed=false);

/*{\Mtext
$bool$ PLANAR($graph\& G,\ list\<edge\>\&\ el$)

\medskip
PLANAR takes as input a directed graph $G(V,E)$ and performs a planarity test
for $G$. PLANAR returns true if $G$ is planar and false otherwise.
If $G$ is not planar a Kuratowsky-Subgraph is computed and returned in $el$.
}*/

list<edge> make_biconnected_graph(graph&G);

/*{\Mtext
\bigskip
$\bullet$ {\bf Triangulation}
}*/

list<edge> TRIANGULATE_PLANAR_MAP(graph&);
/*{\Mtext
$list{\tt <}edge{\tt >}$ TRIANGULATE\_PLANAR\_MAP($graph\&\ G$)

\medskip
TRIANGULATE\_PLANAR\_MAP takes a directed graph $G$ representing a planar map.
It triangulates the faces of $G$ by inserting additional edges. The list of
inserted edges is returned. \precond $G$ must be connected.

The algorithm (\cite{HU89}) has running time $O(|V|+|E|)$.
}*/


/*{\Mtext
\bigskip
$\bullet$ {\bf Straight Line Embedding}
}*/

int STRAIGHT_LINE_EMBEDDING(graph& G,node_array<int>& xcoord,
                                     node_array<int>& ycoord);
/*{\Mtext
$int$
\parbox[t]{12.5cm}{STRAIGHT\_LINE\_EMBEDDING($graph\&\ G$,
                 $node\_array\<int\>\&\ xcoord$,
		 $node\_array\<int\>\&\ ycoord$)}

\medskip
STRAIGHT\_LINE\_EMBEDDING takes as argument a directed graph $G$ representing
a planar map. It computes a straight line embedding of $G$ by assigning
non-negative integer coordinates ($xcoord$ and $ycoord$) in the range
$0..2(n-1)$ to the nodes. STRAIGHT\_LINE\_EMBEDDING returns the maximal
coordinate.

The algorithm (\cite{Fa48}) has running time $O(|V|^2)$.
}*/

int STRAIGHT_LINE_EMBEDDING2(graph& G, node_array<int>& xcoord,
                                       node_array<int>& ycoord);


//-----------------------------------------------------------------------------
// floating point (double) versions
//-----------------------------------------------------------------------------


void DIJKSTRA(const graph& G, node s, const edge_array<double>& cost,
                                            node_array<double>& dist,
                                            node_array<edge>& pred);

bool BELLMAN_FORD(const graph& G, node s, const edge_array<double>& cost,
                                                node_array<double>& dist,
                                                node_array<edge>& pred);


bool ALL_PAIRS_SHORTEST_PATHS(graph& G, const edge_array<double>&  cost,
                                              node_matrix<double>& dist);


double MAX_FLOW(graph& G, node s, node t, const edge_array<double>& cap,
                                                edge_array<double>& flow);

list <edge> MAX_WEIGHT_MATCHING (const ugraph&,
                                   const edge_array<double>&);


list<edge> MAX_WEIGHT_BIPARTITE_MATCHING(graph& G, const list<node>&,
                                                   const list<node>&,
                                                   const edge_array<double>&);



int STRAIGHT_LINE_EMBEDDING(graph& G,node_array<double>& xcoord,
                                     node_array<double>& ycoord);

int STRAIGHT_LINE_EMBEDDING2(graph& G,node_array<double>& xcoord,
                                      node_array<double>& ycoord);


list<edge> MIN_SPANNING_TREE(const graph& G, const edge_array<double>& cost);

list<edge> MIN_SPANNING_TREE(const graph& G, const edge_array<double>& cost);


#endif
