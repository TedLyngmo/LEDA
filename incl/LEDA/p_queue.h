/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  p_queue.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_PRIORITY_QUEUE_H
#define LEDA_PRIORITY_QUEUE_H

#include <LEDA/impl/f_heap.h>

typedef f_heap_item pq_item;

/*{\Manpage {p_queue} {P,I} {Priority Queues}}*/

template<class P, class I>

class p_queue: private f_heap
{
/*{\Mdefinition
An instance $Q$ of the parameterized data type \name\ is a collection of items
(type $pq\_item$). Every item contains a priority from a linearly ordered type
$P$ and an information from an arbitrary type $I$. $P$ is called the priority
type of $Q$ and $I$ is called the information type of $Q$. The number of items
in $Q$ is called the size of $Q$. If $Q$ has size zero it is called the empty
priority queue. We use $\<p,i\>$ to denote a $pq\_item$\ with priority $p$ and
information $i$.}*/

int  int_type()              const { return LEDA_INT_TYPE(P); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(P,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(P,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(P,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(P,x,std::cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,std::cout); }

public:

/*{\Mcreation Q }*/

p_queue()  {}
/*{\Mcreate
creates an instance \var\ of type \name\ and initializes it with the
empty priority queue. }*/

 p_queue(const p_queue<P,I>& Q):f_heap(Q) {}
~p_queue()  { f_heap::clear(); }

p_queue<P,I>& operator=(const p_queue<P,I>& Q)
{ return (p_queue<P,I>&)f_heap::operator=(Q); }


/*{\Moperations 2 5}*/

virtual P prio(pq_item it) const { return LEDA_ACCESS(P,f_heap::key(it)); }
/*{\Mop     returns the priority of item $it$.\\
            \precond $it$ is an item in \var.}*/

virtual I inf(pq_item it) const { return LEDA_ACCESS(I,f_heap::inf(it)); }
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in \var.}*/

virtual pq_item insert(P x, I i)
                         { return f_heap::insert(Convert(x),Convert(i)); }
/*{\Mop     adds a new item $\<x,i\>$ to \var\ and returns it.}*/


virtual pq_item find_min() const { return f_heap::find_min();}
/*{\Mop     returns an item with minimal priority
	    (nil if \var\ is empty).}*/


virtual P   del_min()
{ P x = prio(find_min()); f_heap::del_min(); return x; }
/*{\Mop    removes the item $it$ = \var.find\_min() from \var\
	   and returns the priority of it.\\
           \precond \var\ is not empty.}*/

virtual void del_item(pq_item it) { f_heap::del_item(it); }
/*{\Mop    removes the item $it$ from \var.\\
	   \precond $it$ is an item in \var.}*/

virtual void change_inf(pq_item it, I i)
{ f_heap::change_inf(it,Convert(i)); }
/*{\Mop    makes $i$ the new information of item $it$.\\
	   \precond $it$ is an item in \var.}*/

virtual void decrease_p(pq_item it, P x)
{ f_heap::decrease_key(it,Convert(x)); }
/*{\Mopl    makes $x$ the new priority of item $it$.\\
	    \precond $it$ is an item in \var\ and $x$
	    is not larger then $prio(it)$.}*/


virtual int  size()    const { return f_heap::size(); }
/*{\Mop     returns the size of \var.}*/

virtual bool empty()   const { return (size()==0) ? true : false; }
/*{\Mop     returns true, if \var\ is empty, false otherwise.}*/

void clear() {f_heap::clear();}
/*{\Mop     makes \var\ the empty priority queue. }*/


virtual pq_item first_item() const { return f_heap::first_item(); }
virtual pq_item next_item(pq_item it) const { return f_heap::next_item(it); }

};


/*{\Mimplementation
Priority queues are implemented by Fibonacci heaps \cite{FT87}. Operations
insert, del\_item, del\_min take time $O(\log n)$, find\_min, decrease\_p,
prio, inf, empty take time $O(1)$ and clear takes time $O(n)$, where $n$ is the
size of \var. The space requirement is $O(n)$.}*/


/*{\Mexample
Dijkstra's Algorithm (cf. section \ref{Graph and network algorithms})}*/

#endif
