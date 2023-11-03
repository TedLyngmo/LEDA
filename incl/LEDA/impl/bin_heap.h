/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  bin_heap.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_BIN_HEAP_H
#define LEDA_BIN_HEAP_H

//------------------------------------------------------------------------------
// binary Heaps
//
// S. Naeher (1993)
//
//------------------------------------------------------------------------------


#include <LEDA/basic.h>


class bin_heap_elem
{
  friend class bin_heap;

  GenPtr key;
  GenPtr inf;

  int index;

  bin_heap_elem(GenPtr k, GenPtr i, int pos) { key = k; inf = i; index = pos; }

  LEDA_MEMORY(bin_heap_elem)

};


typedef bin_heap_elem* bin_heap_item;


class bin_heap  {

virtual int  cmp(GenPtr, GenPtr) const { return 0; }
virtual void copy_key(GenPtr&)   const {}
virtual void copy_inf(GenPtr&)   const {}
virtual void clear_key(GenPtr&)  const {}
virtual void clear_inf(GenPtr&)  const {}
virtual void print_key(GenPtr)   const {}
virtual void print_inf(GenPtr)   const {}

virtual int  int_type() const { return 0; }


int          count;
int          max_size;

bin_heap_item* HEAP;


void rise(int,bin_heap_item);
void sink(int,bin_heap_item);

protected:

bin_heap_item item(GenPtr p) const { return bin_heap_item(p); }

public:

bin_heap_item insert(GenPtr, GenPtr);
bin_heap_item find_min()  const      { return HEAP[1];  }
bin_heap_item first_item() const     { return HEAP[1]; }
bin_heap_item next_item(bin_heap_item it) const { return HEAP[it->index+1];}

void decrease_key(bin_heap_item, GenPtr);
void change_inf(bin_heap_item, GenPtr);
void del_item(bin_heap_item);
void del_min() { del_item(find_min()); }

GenPtr key(bin_heap_item it) const { return it->key; }
GenPtr inf(bin_heap_item it) const { return it->inf; }

int  size()    const  { return count;    }
bool empty()   const  { return count==0; }

void clear();
void print();

bin_heap& operator=(const bin_heap&);

bin_heap(int n = 1024);  // default start size 
bin_heap(const bin_heap&);

bin_heap(int,int) {}

virtual ~bin_heap();

};

#endif
