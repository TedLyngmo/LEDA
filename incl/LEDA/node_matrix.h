/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_matrix.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_MATRIX_H
#define LEDA_NODE_MATRIX_H


//------------------------------------------------------------------------------
// node matrices
//------------------------------------------------------------------------------

#include <LEDA/graph.h>
#include <LEDA/node_array.h>


class Node_Matrix {

graph* g;
node_array<graph_map*> M;

virtual void init_entry(GenPtr&)  { }
virtual void copy_entry(GenPtr&)  const { }
virtual void clear_entry(GenPtr&) const { }

public:

 graph_map& row(node v)               { return *(M[v]); }
 GenPtr&    entry(node v, node w)     { return M[v]->array_access(w); }
 GenPtr     inf(node v, node w) const { return M[v]->array_read(w); }

 void init(const graph&, int, GenPtr);
 void init(const Node_Matrix&);

 void clear_entries();

 Node_Matrix()  {}

 virtual ~Node_Matrix();

};



/*{\Manpage {node_matrix} {E} {Two Dimensional Node Arrays}}*/

template<class E>

class node_matrix: public Node_Matrix {

/*{\Mdefinition
An instance $M$ of the parameterized data type \name\ is
a partial mapping from the set of node pairs $V\times V$
of a graph to the set of variables of data type $E$, called the element type
of $M$. The domain $I$ of $M$ is called the index set of $M$. $M$ is said to
be valid for all node pairs in $I$. A node matrix can also be viewed as a
node array with element type $node\_array\<E\>$ ($node\_array\<node\_array\<E\>\>$).
}*/

E X;

void copy_entry(GenPtr& x) const { LEDA_COPY(E,x); }
void clear_entry(GenPtr& x)const { LEDA_CLEAR(E,x);  }
void init_entry(GenPtr& x)       { x = Copy(X);   }

public:

/*{\Mcreation M }*/

node_matrix() {}
/*{\Mcreate creates an instance $M$ of type \name\ and initializes the index set
of $M$ to the empty set. }*/

node_matrix(const graph& G)                 { init(G);   }
/*{\Mcreate creates an instance $M$ of type \name\ and initializes the index set
to be the set of all node pairs of graph $G$, i.e., $M$ is made valid for all
pairs in $V \times V$ where $V$ is the set of nodes currently contained in $G$.
}*/

node_matrix(const graph& G, E x)         { init(G,x); }
/*{\Mcreate creates an instance $M$ of type \name\ and initializes the index set
of $M$ to be the set of all node pairs of graph $G$, i.e., $M$ is made valid 
for all pairs in $V \times V$ where $V$ is the set of nodes currently 
contained in $G$.  In addition, $M(v,w)$ is initialized with $x$ for all 
nodes $v,w \in V$.}*/


node_matrix(const graph& G, int n, E x)  { init(G,n,x); }


 node_matrix(const node_matrix<E>& M) { init(M); }
 node_matrix<E>& operator=(const node_matrix<E>& M)
                                         { init(M); return *this;}
~node_matrix() { clear_entries(); }


/*{\Moperations 3.2 3.7}*/

void  init(const graph& G)                { init(G,X); }
/*{\Mop       sets the index set of $M$ to $V\times V$, where  
	      $V$ is the set of all nodes of $G$. }*/

void  init(const graph& G, int n, E x) { Node_Matrix::init(G,n,Convert(x)); }

void  init(const graph& G, E x)        { init(G,G.max_node_index()+1,x); }
/*{\Mop       sets the index set of $M$ to $V\times V$, where
	      $V$ is the set of all nodes of $G$ and initializes
	      $M(v,w)$ to $x$ for all $v,w \in V$.}*/

void  init(const node_matrix<E>& M) { Node_Matrix::init(M); }

node_array<E>& operator[](node v)
{ return *(node_array<E>*)&row(v); }
/*{\Marrop    returns the node\_array  $M(v)$.}*/


E  operator()(node v, node w) const { return LEDA_ACCESS(E,inf(v,w));}
E& operator()(node v, node w)       { return LEDA_ACCESS(E,entry(v,w));}
/*{\Mfunop    returns the variable $M(v,w)$.\\
	      \precond $M$ must be valid for $v$ and $w$.}*/


};

/*{\Mimplementation
Node matrices for a graph $G$ are implemented by vectors of node arrays and an 
internal numbering of the nodes of $G$. The access operation 
takes constant time, the init operation takes time $O(n^2)$, where $n$ is the 
number of nodes currently contained in $G$. The space requirement is $O(n^2)$.
Note that a node matrix is only valid for the nodes contained in $G$ at the 
moment of the matrix declaration or initialization ($init$). Access operations 
for later added nodes are not allowed.}*/


#endif
