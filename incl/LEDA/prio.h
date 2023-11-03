/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  prio.h
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

/*{\Manpage {priority_queue} {K,I} {Old-Style Priority Queues}}*/

template<class K, class I> 

class priority_queue: private f_heap
{
/*{\Mdefinition
An instance $Q$ of the parameterized data type \name\ is a
collection of items (type $pq\_item$). Every item contains a key from type
$K$ and an information from the linearly ordered type $I$. $K$ is called the
key type of $Q$ and $I$ is called the information type of $Q$. The number of
items in $Q$ is called the size of $Q$. If $Q$ has size zero it is called the
empty priority queue. We use $\<k,i\>$ to denote a $pq\_item$\ with key $k$ and
information $i$.

The type \name\ is identical to the type $p\_queue$ except that the meanings
of $K$ and $I$ are interchanged. We now believe that the semantics of
$p\_queue$ is the more natural one and keep \name\ only for historical 
reasons. We recommend to use $p\_queue$ instead.}*/

int  int_type()              const { return LEDA_INT_TYPE(I); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(I,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(I,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(K,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(I,x,cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(K,x,cout); }

public:

/*{\Mcreation Q }*/

priority_queue()  {}
/*{\Mcreate 
creates an instance \var\ of type \name\ and initializes it with the
empty priority queue. }*/

 priority_queue(const priority_queue<K,I>& Q):f_heap(Q) {}
~priority_queue()  { f_heap::clear(); }

priority_queue<K,I>& operator=(const priority_queue<K,I>& Q) 
{ return (priority_queue<K,I>&)f_heap::operator=(Q); }


/*{\Moperations 2 5.5}*/

virtual K key(pq_item it) const { return LEDA_ACCESS(K,f_heap::inf(it)); }
/*{\Mop     returns the key of item $it$.\\
            \precond $it$ is an item in \var.}*/

virtual I inf(pq_item it) const { return LEDA_ACCESS(I,f_heap::key(it)); }
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in \var.}*/

virtual pq_item insert(K k, I i) 
                              { return f_heap::insert(Convert(i),Convert(k)); }
/*{\Mop     adds a new item $\<k,i\>$ to \var\ and returns it.}*/


virtual pq_item find_min() const { return f_heap::find_min();}
/*{\Mop     returns an item with minimal information 
	    (nil if \var\ is empty).}*/


virtual K   del_min() 
{ K x = key(find_min()); f_heap::del_min(); return x; }
/*{\Mop    removes the item $it$ = \var.find\_min() from \var
	   and returns the key of $it$.\\
           \precond \var\ is not empty.}*/

virtual void del_item(pq_item it) { f_heap::del_item(it); }
/*{\Mop    removes the item $it$ from \var.\\
	   \precond $it$ is an item in \var.}*/

virtual void change_key(pq_item it, K k) 
{ f_heap::change_inf(it,Convert(k)); }
/*{\Mop    makes $k$ the new key of item $it$.\\
	   \precond $it$ is an item in \var.}*/

virtual void decrease_inf(pq_item it, I i)
{ f_heap::decrease_key(it,Convert(i)); }
/*{\Mop     makes $i$ the new information of item $it$.\\
	    \precond $it$ is an item in \var\ and $i$
	    is not larger then $inf(it)$.}*/


virtual int  size()    const { return f_heap::size(); }
/*{\Mop     returns the size of \var.}*/

virtual bool empty()   const { return (size()==0) ? true : false; }
/*{\Mop     returns true, if \var\ is empty, false otherwise}*/

void clear() {f_heap::clear();}
/*{\Mop     makes \var\ the empty priority queue. }*/


virtual pq_item first_item() const { return f_heap::first_item(); }
virtual pq_item next_item(pq_item it) const { return f_heap::next_item(it); }

};


/*{\Mimplementation
Priority queues are implemented by Fibonacci heaps \cite{FT87}. Operations
insert, del\_item, del\_min take time $O(\log n)$, find\_min, decrease\_inf, 
key, inf, empty take time $O(1)$ and clear takes time $O(n)$, where $n$ is the 
size of \var. The space requirement is $O(n)$.}*/


/*{\Mexample
Dijkstra's Algorithm (cf. section \ref{Graph and network algorithms})}*/

#endif
