/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  edge_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_EDGE_ARRAY_H
#define LEDA_EDGE_ARRAY_H

//------------------------------------------------------------------------------
// edge arrays
//------------------------------------------------------------------------------

#include <LEDA/graph.h>


/*{\Manpage {edge_array} {E} {Edge Arrays}}*/

template <class E>
class edge_array : public graph_map
{

/*{\Mdefinition
An instance $A$ of the parameterized data type $edge\_array\<E\>$
is a partial mapping from the edge set of a graph $G$ to the set of
variables of type $E$, called the element type of the array. The domain
$I$ of $A$ is called the index set of $A$ and $A(e)$ is called the element
at position $e$. $A$ is said to be valid for all edges in $I$.}*/

E def;

void clear_entry(GenPtr& x) const { LEDA_CLEAR(E,x); }
void copy_entry(GenPtr& x)  const { LEDA_COPY(E,x);  }
void init_entry(GenPtr& x)  const { x = Copy(def); }

public:

int cmp_entry(GenPtr p,GenPtr q) const { return LEDA_COMPARE(E,p,q); }


/*{\Mcreation A }*/

edge_array() : graph_map(nil,0) {}
/*{\Mcreate creates an instance $A$ of type $edge\_array\<E\>$ with empty
            index set.}*/

edge_array(const graph& G) : graph_map(&G,G.max_edge_index()+1)
{ init_table(); }
/*{\Mcreate creates an instance $A$ of type $edge\_array\<E\>$ and initializes
            the index set of $A$ to be the current edge set of graph $G$. }*/


edge_array(const graph& G, E x) : graph_map(&G,G.max_edge_index()+1)
{ def = x; init_table(); }
/*{\Mcreate creates an instance $A$ of type $edge\_array\<E\>$, sets the
            index set of $A$ to the current edge set of graph $G$ and
            initializes $A(v)$ with $x$ for all edges $v$ of $G$. }*/


edge_array(const graph& G, int n, E x) : graph_map(&G,n)
{ def = x; init_table(); }
/*{\Mcreate creates an instance $A$ of type $edge\_array\<E\>$ valid for
            up to $n$ edges of graph $G$ and initializes $A(e)$ with $x$
            for all edges $e$ of $G$.\\
	    \precond $n \ge |E|$.\\
	    $A$ is also
            valid for the next $n - |E|$ edges added to $G$. }*/


edge_array(const edge_array<E>& A) : graph_map(A)   {}
edge_array<E>& operator=(const edge_array<E>& A)
{ graph_map::operator=(A);
  return *this;
 }
~edge_array() { clear_table(); }



/*{\Moperations 1.3 4.7 }*/

E  operator[](edge v) const {  return LEDA_ACCESS(E,graph_map::array_read(v)); }
E& operator[](edge e) {  return LEDA_ACCESS(E,graph_map::array_access(e)); }
/*{\Marrop    returns the variable $A(e)$.\\
	      \precond $A$ must be valid for $e$.}*/


E& entry(int i)      { return LEDA_ACCESS(E,graph_map::access(i));}
E  inf(int i)  const { return LEDA_ACCESS(E,graph_map::read(i));}
E& entry(edge v)     {  return LEDA_ACCESS(E,graph_map::array_access(v)); }
E  inf(edge v) const {  return LEDA_ACCESS(E,graph_map::array_read(v)); }

void init()  { graph_map::init(nil,1); }

void init(const graph& G)
{ graph_map::init(&G,G.max_edge_index()+1);
  init_table();
 }
/*{\Mop       sets the index set $I$ of $A$ to the edge
	      set of $G$, i.e., makes $A$ valid for all edges
              of $G$.}*/

void init(const graph& G, E x)
{ graph_map::init(&G,G.max_edge_index()+1);
  def = x;
  init_table();
}
/*{\Mop       makes $A$ valid for all edges of $G$
	      and sets $A(e) = x$ for all edges $e$ of $G$. }*/

void init(const graph& G, int n, E x)
{ graph_map::init(&G,n);
  def = x;
  init_table();
}
/*{\Mop       makes $A$ valid for at most $n$ edges
	      of $G$ and sets $A(e) = x$ for all edges $e$
	      of $G$.\\
	      \precond $n \ge |E|$.\\
	      $A$ is also
              valid for the next $n - |E|$ edges added to $G$. }*/


/*{\Mimplementation
Edge arrays for a graph $G$ are implemented by \CC vectors and an
internal numbering of the edges and edges of $G$. The access operation
takes constant time, $init$ takes time $O(n)$, where $n$ is the number of
edges in $G$. The space requirement is $O(n)$.

{\bf Remark}: An edge array is only valid for a bounded number of
edges of $G$. This number is either the number
of edges of $G$ at the moment of creation of the array or it is explicitely
set by the user.  Dynamic edge arrays can be realized by edge
maps (cf. section \ref{Edge Maps}). }*/

};


#endif
