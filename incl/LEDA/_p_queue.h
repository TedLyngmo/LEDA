/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _p_queue.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef _LEDA_P_QUEUE_H
#define _LEDA_P_QUEUE_H

#include <LEDA/p_queue.h>

//------------------------------------------------------------------------------
//
// Priority queues with implementation parameter:
//
//   _p_queue<priotype,inftype,prio_impl> 
//
//------------------------------------------------------------------------------


/*{\Manpage {_p_queue} {P,I,impl} {Priority Queues with Implementation Parameter} }*/

/*{\Mdefinition
An instance of type \name\ is a priority queue implemented by data type $impl$.
$impl$ must be one of the priority queue implementations listed in
section \ref{Implementations Priority Queues} or a user defined data structure
fulfilling the specification given in section \ref{User Implementations
Priority Queues}. Note that the priority type $P$ must linearly ordered.
}*/


template <class P, class I, class impl> 

class _p_queue : private impl, public p_queue<P,I>
{

int  int_type()              const { return LEDA_INT_TYPE(P); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(P,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(P,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(P,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(P,x,cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,cout); }

public:

pq_item insert(P k,I i) { return pq_item(impl::insert(Convert(k),Convert(i)));}
pq_item find_min() const { return pq_item(impl::find_min());}

P del_min() 
{ pq_item it = find_min();
  P x = prio(it);
  del_item(it);
  return x; 
 }

P prio(pq_item x) const { return LEDA_ACCESS(P,impl::key(impl::item(x)));}
I inf(pq_item x) const { return LEDA_ACCESS(I,impl::inf(impl::item(x)));}

void change_inf(pq_item x, I i)
{ impl::change_inf(impl::item(x),Convert(i)); }

void decrease_p(pq_item x,P k)
{ impl::decrease_key(impl::item(x),Convert(k));}

void del_item(pq_item x) { impl::del_item(impl::item(x)); }

int  size() const { return impl::size(); }
bool empty() const { return impl::size()==0; }

pq_item first_item() const { return pq_item(impl::first_item()); }
pq_item next_item(pq_item it) const 
{ return pq_item(impl::next_item(impl::item(it))); }

_p_queue<P,I,impl>& operator=(const _p_queue<P,I,impl>& Q)
{ impl::operator=(Q); return *this; }

 _p_queue() {}
 _p_queue(int n) : impl(n) {}
 _p_queue(const _p_queue<P,I,impl>& Q) : impl(Q) {}
~_p_queue() { impl::clear(); }

};

#endif
