/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  k_heap.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_KHEAP_H
#define LEDA_KHEAP_H

//------------------------------------------------------------------------------
// k-nary Heaps
//------------------------------------------------------------------------------


#include <LEDA/basic.h>

class k_heap_elem
{
  friend class k_heap;

  GenPtr key;
  GenPtr inf;

  int index;

  k_heap_elem(GenPtr k, GenPtr i, int pos) { key = k; inf = i; index = pos; }

  LEDA_MEMORY(k_heap_elem)

};


typedef k_heap_elem* k_heap_item;


class k_heap  {

virtual int cmp(GenPtr, GenPtr) const { return 0; }
virtual void copy_key(GenPtr&)  const {}
virtual void copy_inf(GenPtr&)  const {}
virtual void clear_key(GenPtr&) const {}
virtual void clear_inf(GenPtr&) const {}
virtual void print_key(GenPtr)  const {}
virtual void print_inf(GenPtr)  const {}

virtual int  int_type() const { return 0; }


int          K;
int          count;
int          max_size;
k_heap_item* HEAP;

void rise(int,k_heap_item);
void sink(int,k_heap_item);

void check(k_heap_item);

protected:

k_heap_item item(GenPtr p) const { return k_heap_item(p); }

public:

k_heap_item insert(GenPtr, GenPtr);
k_heap_item find_min()  const      { return HEAP[1];  }
k_heap_item first_item() const     { return HEAP[1]; }
k_heap_item next_item(k_heap_item it) const { return HEAP[it->index+1];}

void decrease_key(k_heap_item, GenPtr);
void change_inf(k_heap_item, GenPtr);
void del_item(k_heap_item);
void del_min() { del_item(find_min()); }

GenPtr key(k_heap_item it) const { return it->key; }
GenPtr inf(k_heap_item it) const { return it->inf; }

int  size()    const  { return count;    }
bool empty()   const  { return count==0; }

void clear();
void print();

k_heap& operator=(const k_heap&);

k_heap(int n=0,int k=2);  // default: binary heap
k_heap(const k_heap&);

virtual ~k_heap();

};

#endif
