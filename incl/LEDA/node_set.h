/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_NODE_SET_H
#define LEDA_NODE_SET_H

//------------------------------------------------------------------------------
// node_set  
//------------------------------------------------------------------------------

#include <LEDA/graph.h>

/*{\Manpage {node_set} {} {Sets of Nodes} }*/

class node_set 
{
/*{\Mdefinition
An instance $S$ of the data type $node\_set$ is a subset of
the nodes of a graph $G$. $S$ is said to be valid for the nodes
of $G$.}*/

  graph* g;
  list<node> L;
  node_array<list_item> A;

public:


/*{\Mcreation S }*/

node_set(const graph& G) : A(G,nil) { g = (graph*)&G; }
/*{\Mcreate creates an instance $S$ of type $node\_set$ valid for all 
            nodes currently contained in graph $G$ and initializes it 
            to the empty set.}*/

~node_set() {}


/*{\Moperations 1.3 4 }*/

void insert(node x)  { if (A[x] == nil) A[x] = L.append(x); }
/*{\Mop       adds node $x$ to $S$. }*/

void del(node x)     { if (A[x] != nil) { L.del(A[x]); A[x] = nil;} }
/*{\Mop       removes node $x$ from $S$. }*/

bool member(node x)  { return (A[x] != nil); }
/*{\Mop      returns true if $x$ in $S$, false otherwise. }*/

node choose()  const { return L.head(); }
/*{\Mop      returns a node of $S$. }*/

int  size()    const { return L.length(); }
/*{\Mop      returns the size of $S$. }*/

bool empty()   const { return L.empty(); }
/*{\Mop      returns true iff $S$ is the empty set. }*/

void clear()         { L.clear(); A.init(*g,nil); }
/*{\Mop      makes $S$ the empty set. }*/

};

/*{\Mimplementation
A node set $S$ for a graph $G$ is implemented by a combination of a 
list  $L$ of nodes and a node array of list\_items 
associating with each node its position in $L$. All operations 
take constant time, except for clear which takes time $O(|S|)$. The space 
requirement is $O(n)$, where $n$ is the number of nodes of $G$.}*/

#endif
