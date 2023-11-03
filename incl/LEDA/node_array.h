/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_NODE_ARRAY_H
#define LEDA_NODE_ARRAY_H

//------------------------------------------------------------------------------
// node arrays
//------------------------------------------------------------------------------

#include <LEDA/graph.h>


/*{\Manpage {node_array} {E} {Node Arrays}}*/

template <class E>
class node_array : public graph_map
{

/*{\Mdefinition
An instance $A$ of the parameterized data type $node\_array\<E\>$
is a partial mapping from the node set of a graph $G$ to the set of 
variables of type $E$, called the element type of the array. The domain 
$I$ of $A$ is called the index set of $A$ and $A(v)$ is called the element 
at position $v$. $A$ is said to be valid for all nodes in $I$.}*/

E def;

void clear_entry(GenPtr& x) const { LEDA_CLEAR(E,x); }
void copy_entry(GenPtr& x)  const { LEDA_COPY(E,x);  }
void init_entry(GenPtr& x)  const { x = Copy(def); }
  
public:

int cmp_entry(GenPtr p,GenPtr q) const { return LEDA_COMPARE(E,p,q); }


/*{\Mcreation A }*/

node_array() : graph_map(nil,0) {}
/*{\Mcreate creates an instance $A$ of type $node\_array\<E\>$ with empty
            index set.}*/

node_array(const graph& G) : graph_map(&G,G.max_node_index()+1)
{ init_table(); }
/*{\Mcreate creates an instance $A$ of type $node\_array\<E\>$ and initializes
            the index set of $A$ to the current node set of graph $G$. }*/


node_array(const graph& G, E x) : graph_map(&G,G.max_node_index()+1)
{ def = x; init_table(); }
/*{\Mcreate creates an instance $A$ of type $node\_array\<E\>$, sets the
            index set of $A$ to the current node set of graph $G$ and 
            initializes $A(v)$ with $x$ for all nodes $v$ of $G$. }*/


node_array(const graph& G, int n, E x) : graph_map(&G,n)
{ def = x; init_table(); }
/*{\Mcreate creates an instance $A$ of type $node\_array\<E\>$ valid for
            up to $n$ nodes of graph $G$ and initializes $A(v)$ with $x$ 
            for all nodes $v$ of $G$.\\
	    \precond $n \ge |V|$. $A$ is also
            valid for the next $n - |V|$ nodes added to $G$.}*/


node_array(const node_array<E>& A) : graph_map(A)   {}
node_array<E>& operator=(const node_array<E>& A) 
{ graph_map::operator=(A);  
  return *this;
 }
~node_array() { clear_table(); }



/*{\Moperations 1.3 4.7 }*/

E  operator[](node v) const {  return LEDA_ACCESS(E,graph_map::array_read(v)); }
E& operator[](node v) {  return LEDA_ACCESS(E,graph_map::array_access(v)); }
/*{\Marrop    returns the variable $A(v)$.\\
	      \precond $A$ must be valid for $v$.}*/


E& entry(int i)      { return LEDA_ACCESS(E,graph_map::access(i));}
E  inf(int i)  const { return LEDA_ACCESS(E,graph_map::read(i));}
E& entry(node v)     {  return LEDA_ACCESS(E,graph_map::array_access(v)); }
E  inf(node v) const {  return LEDA_ACCESS(E,graph_map::array_read(v)); }

void init()  { graph_map::init(nil,1); }

void init(const graph& G)      
{ graph_map::init(&G,G.max_node_index()+1); 
  init_table();
 }
/*{\Mop       sets the index set $I$ of $A$ to the node  
	      set of $G$, i.e., makes $A$ valid for all nodes 
              of $G$.}*/

void init(const graph& G, E x) 
{ graph_map::init(&G,G.max_node_index()+1); 
  def = x;
  init_table();
}
/*{\Mop       makes $A$ valid for all nodes of $G$
	      and sets $A(v) = x$ for all nodes $v$ of $G$.}*/

void init(const graph& G, int n, E x) 
{ graph_map::init(&G,n); 
  def = x;
  init_table();
}
/*{\Mop       makes $A$ valid for at most $n$ nodes  of $G$ 
              and sets $A(v) = x$ for all nodes $v$  of $G$.
              \precond {$n \ge |V|$}. $A$ is also valid for 
              the next  $n - |V|$ nodes added to $G$. }*/


/*{\Mimplementation
Node arrays for a graph $G$ are implemented by \CC vectors and an
internal numbering of the nodes and edges of $G$. The access operation
takes constant time, $init$ takes time $O(n)$, where $n$ is the number of
nodes in $G$. The space requirement is $O(n)$.
    
{\bf Remark}: A node array is only valid for a bounded number of
nodes of $G$. This number is either the number
of nodes of $G$ at the moment of creation of the array or it is explicitely 
set by the user. Dynamic node arrays can be realized by node 
maps (cf. section \ref{Node Maps}). }*/

};


#endif
