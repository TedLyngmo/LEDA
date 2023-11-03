/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_pq.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_NODE_PQ_H
#define LEDA_NODE_PQ_H

//------------------------------------------------------------------------------
// node priority queues
//------------------------------------------------------------------------------

#include <LEDA/graph.h>
#include <LEDA/impl/bin_heap.h>

#define PRIO_IMPL bin_heap
#define PRIO_ITEM bin_heap_item


/*{\Manpage {node_pq} {P} {Node Priority Queues}}*/

template <class P>

class node_pq : private PRIO_IMPL
{
/*{\Mdefinition
An instance $Q$ of the parameterized data type \name\ is a partial
function from the nodes of a graph $G$ to a linearly ordered type $P$
of priorities. The priority of a node is sometimes called the information
of the node. For every graph $G$ only one \name\ may be used and every node
of $G$ may be contained in the queue at most once (cf. section
\ref{Priority Queues} for general priority queues). }*/

int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(P,x,y); }
int  int_type()              const { return LEDA_INT_TYPE(P); }
void print_key(GenPtr x)     const { LEDA_PRINT(P,x,std::cout); }
void print_inf(GenPtr x)     const { std::cout << index(node(x));  }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(P,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(P,x);  }

public:

/*{\Mcreation Q }*/

node_pq(const graph& G) { init_node_data(G,1,nil); }
/*{\Mcreate creates an instance $Q$ ot type \name\ for the nodes of graph $G$
    with $dom(Q)=\emptyset$.}*/

~node_pq()             { clear(); }


/*{\Moperations 1.3 4.7}*/

void insert(node v, P x)
{ v->data[1] = PRIO_IMPL::insert(Convert(x),v);}
/*{\Mop          adds the node $v$ with priority $x$ to
          	 \var.\\ \precond $v\not\in dom(Q)$.}*/

P prio(node v) const
{ return LEDA_ACCESS(P,PRIO_IMPL::key((PRIO_ITEM)v->data[1])); }
/*{\Mop         returns the priority of node $v$.}*/



bool member(node v) const {return v->data[1] != nil;};
/*{\Mop         returns true if $v$ in \var, false otherwise.}*/

void decrease_p(node v, P x)
{ PRIO_IMPL::decrease_key(PRIO_ITEM(v->data[1]),Convert(x)); }
/*{\Mop          makes $x$ the new priority of node $v$.\\
        	 \precond $x \le Q$.prio$(v)$.}*/

node find_min() const
{ return (node)PRIO_IMPL::inf(PRIO_IMPL::find_min());   }
/*{\Mop          returns a node with minimal priority
                 (nil if \var\ is empty).}*/

void del(node v)
{ PRIO_IMPL::del_item(PRIO_ITEM(v->data[1])); v->data[1] = 0; }
/*{\Mop          removes the node $v$ from \var. }*/

node del_min()
{ node v = find_min(); PRIO_IMPL::del_min(); v->data[1] = 0;  return v; }
/*{\Mop          removes a node with minimal priority from \var\
                 and returns it (nil if \var\ is empty).}*/

void clear()     { PRIO_IMPL::clear(); }
/*{\Mop          makes $Q$ the empty node priority queue.}*/

int size()   const { return PRIO_IMPL::size(); }
/*{\Mop          returns $|dom(Q)|$. }*/

int empty()  const { return PRIO_IMPL::empty(); }
/*{\Mop          returns true if \var\ is the empty node
		 priority queue, false otherwise.}*/

P inf(node v) const
{ return LEDA_ACCESS(P,PRIO_IMPL::key((PRIO_ITEM)v->data[1])); }
/*{\Mop         returns the priority of node $v$.}*/

void decrease_inf(node v, P x)
{ PRIO_IMPL::decrease_key(PRIO_ITEM(v->data[1]),Convert(x)); }
/*{\Xop (for backward compatibility) makes $x$ the new priority of node $v$.\\
        	 \precond $p \le Q$.prio$(v)$.}*/


};

/*{\Mimplementation
Node priority queues are implemented by fibonacci heaps and node arrays.
Operations insert, del\_node, del\_min take time $O(\log n)$, find\_min,
decrease\_inf, empty take time $O(1)$ and clear takes time $O(m)$, where
$m$ is the size of $Q$. The space requirement is $O(n)$, where $n$ is the
number of nodes of $G$.}*/

#endif

