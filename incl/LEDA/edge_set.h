/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  edge_set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_EDGE_SET_H
#define LEDA_EDGE_SET_H

#include <LEDA/graph.h>


//------------------------------------------------------------------------------
// edge_set
//------------------------------------------------------------------------------

/*{\Manpage {edge_set} {} {Sets of Edges} }*/

class edge_set
{
/*{\Mdefinition
An instance $S$ of the data type $edge\_set$ is a subset of
the edges of a graph $G$. $S$ is said to be valid for the
edges of $G$.}*/

  graph* g;
  list<edge> L;
  edge_array<list_item> A;

public:

/*{\Mcreation S }*/

edge_set(const graph& G) :A(G,nil) { g = (graph*)&G; }
/*{\Mcreate creates an instance $S$ of type $edge\_set$ valid for all edges
            currently in graph $G$ and initializes it to the empty set.}*/



virtual ~edge_set()  { }



/*{\Moperations 1.3 4 }*/

void insert(edge x)  { if (A[x] == nil) A[x] = L.append(x); }
/*{\Mop     adds edge $x$ to $S$. }*/

void del(edge x)     { if (A[x] != nil) { L.del(A[x]); A[x] = nil;} }
/*{\Mop       removes edge $x$ from $S$. }*/

bool member(edge x)      { return (A[x] != nil); }
/*{\Mop       returns true if $x$ in $S$, false otherwise. }*/

edge choose()  const     { return L.head(); }
/*{\Mop       returns an edge of $S$. }*/

int  size()    const     { return L.length(); }
/*{\Mop       returns the size of $S$. }*/

bool empty()   const     { return L.empty(); }
/*{\Mop       returns true iff $S$ is the empty set. }*/

void clear()             { L.clear(); A.init(*g,nil); }
/*{\Mop       makes $S$ the empty set. }*/

};

/*{\Mimplementation
An edge set $S$ for a graph $G$ is implemented by a combination of a
list  $L$ of edges and an edge array of list\_items
associating with each edge its position in $L$. All operations
take constant time, except for clear which takes time $O(|S|)$. The space
requirement is $O(n)$, where $n$ is the number of edges of $G$.}*/

#endif
