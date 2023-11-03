/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  b_prio.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_BPRIO_H
#define LEDA_BPRIO_H

//------------------------------------------------------------------------------
// b_priority_queues: bounded priority queues implemented by b_heaps 
//------------------------------------------------------------------------------

#include <LEDA/impl/b_heap.h>

/*{\Manpage {b_priority_queue} {K} {Bounded Priority Queues}}*/

typedef b_heap_item b_pq_item;

template<class K> 

class b_priority_queue : public b_heap 
{

/*{\Mdefinition 
An instance $Q$ of the parameterized data type \name\ is a
priority\_queue (cf. section \ref{Priority Queues}) whose information type is a
fixed interval $[a..b]$ of integers.}*/

public:

/*{\Mcreation Q}*/

b_priority_queue(int a, int b): (a,b)  {}

/*{\Mcreate creates an instance \var\ of type \name\ with information type 
            $[a..b]$ and initializes it with the empty priority queue. }*/


virtual ~b_priority_queue()  { }


/*{\Moperations 2 5.5 }*/

/*{\Mtext See section \ref{Priority Queues}.}*/

b_pq_item insert(K k,int info)
                           { return b_heap::insert(info,Convert(k)); }

void decrease_inf(b_pq_item it,int newinf)
                           { b_heap::decrease_key(it,newinf); }

void del_item(b_pq_item x) { b_heap::delete_item(x); }

int      inf(b_pq_item x)  { return b_heap::key(x); }

K  key(b_pq_item x)  { return LEDA_ACCESS(K,b_heap::info(x)); }

K  del_min()         { return LEDA_ACCESS(K,b_heap::del_min()); }

b_pq_item find_min()       { return b_heap::find_min(); }

void clear()               { b_heap::clear(); }

int empty()                { return (find_min()==0) ? true : false; }

};

 
/*{\Mimplementation
Bounded priority queues are implemented by arrays of linear lists.
Operations insert, find\_min, del\_item, decrease\_inf, key, inf,
and empty take time $O(1)$, del\_min (=  del\_item for the minimal
element) takes time $O(d)$, where $d$ is the distance of the minimal
element to the next bigger element in the queue (= $O(b-a)$ in the
worst case). clear takes time $O(b-a+n)$ and the space requirement is
$O(b-a+n)$, where $n$ is the current size of the queue.}*/
#endif

