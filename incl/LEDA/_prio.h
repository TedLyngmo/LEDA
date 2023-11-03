/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _prio.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef _LEDA_PRIO_H
#define _LEDA_PRIO_H

#include <LEDA/prio.h>

//------------------------------------------------------------------------------
//
// Priority queues with implementation parameter:
//
//   _priority_queue<keytype,inftype,prio_impl>
//
//------------------------------------------------------------------------------

template <class ktype, class I, class impl>

class _priority_queue : private impl, public priority_queue<ktype,I>
{

int int_type() const { return LEDA_INT_TYPE(I); }

int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(I,x,y); }
void clear_key(GenPtr& x) const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x) const { LEDA_CLEAR(ktype,x); }
void copy_key(GenPtr& x)  const { LEDA_COPY(I,x); }
void copy_inf(GenPtr& x)  const { LEDA_COPY(ktype,x); }
void print_key(GenPtr x)  const { LEDA_PRINT(I,x,cout); }
void print_inf(GenPtr x)  const { LEDA_PRINT(ktype,x,cout); }

public:

pq_item insert(ktype k,I i) { return pq_item(impl::insert(Convert(i),Convert(k)));}
pq_item find_min() const { return pq_item(impl::find_min());}

ktype del_min()
{ pq_item it = find_min();
  ktype x = key(it);
  del_item(it);
  return x;
 }

ktype key(pq_item x) const
{ return LEDA_ACCESS(ktype,impl::inf(impl::item(x)));}

I inf(pq_item x) const
{ return LEDA_ACCESS(I,impl::key(impl::item(x)));}

void change_key(pq_item x, ktype k)
{ impl::change_inf(impl::item(x),Convert(k)); }

void decrease_inf(pq_item x,I i)
{ impl::decrease_key(impl::item(x),Convert(i));}

void del_item(pq_item x)
{ impl::del_item(impl::item(x)); }

int  size()  const { return impl::size(); }
bool empty() const { return (size()==0) ? true : false; }

pq_item first_item() const { return pq_item(impl::first_item()); }
pq_item next_item(pq_item it) const
{ return pq_item(impl::next_item(impl::item(it))); }

_priority_queue<ktype,I,impl>& operator=(const _priority_queue<ktype,I,impl>& Q)
{ return (_priority_queue<ktype,I,impl>&)impl::operator=(Q); }

 _priority_queue() {}
 _priority_queue(int n) : impl(n) {}
 _priority_queue(const _priority_queue<ktype,I,impl>& Q) : impl(Q) {}
~_priority_queue() { impl::clear(); }

};

#endif
