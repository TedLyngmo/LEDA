/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  p_aux_heap.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_PAIRING_AUX_HEAP_H
#define LEDA_PAIRING_AUX_HEAP_H

#include <LEDA/impl/p_heap.h>

// by Markus Neukirch
//
// Implementation following John T. Stasko and Jeffrey Scott Vitter
// ( Communications of the ACM   March 1987 Volume 30 Number 3   S. 234-240 )



class p_aux_heap:public p_heap
{
	
	ph_item* head,*minptr;
	int item_count;

	virtual int cmp(GenPtr, GenPtr) const {return 0; }
	virtual void print_key(GenPtr)  const {}
	virtual void print_inf(GenPtr)  const {}
	virtual void clear_key(GenPtr&) const {}
	virtual void clear_inf(GenPtr&) const {}
	virtual void copy_key(GenPtr&)  const {}
	virtual void copy_inf(GenPtr&)  const {}
 
	virtual int  int_type() const { return 0; }

	void do_copy(ph_item*,ph_item*,bool);
	ph_item* twopass(ph_item*);
	ph_item* multipass(ph_item*);
	
public:
	
	
	p_aux_heap();

	p_aux_heap(int)     { error_handler(1,"no p_heap(int) constructor");}
        p_aux_heap(int,int) { error_handler(1,"no p_heap(int,int) constructor");}

	p_aux_heap(const p_aux_heap& init);		// construct

	p_aux_heap& operator=(const p_aux_heap&);
	~p_aux_heap() {clear();}

	ph_item* insert(GenPtr,GenPtr);
	void decrease_key(ph_item*,GenPtr);
	ph_item* find_min() const
		{return minptr;}
	void delete_min_multipass();			// auxiliary version
	void delete_min_twopass();			// auxiliary version

	GenPtr key(ph_item* x) const {return x->key;}
	GenPtr inf(ph_item* x) const {return x->inf;}
	
	void change_inf(ph_item* x,GenPtr inf)
		{clear_inf(x->inf);copy_inf(inf);x->inf=inf;}

	void del_item(ph_item* x)
		{decrease_key(x,minptr->key);delete_min_twopass();}
	
	void clear() 
		{p_heap::clear();item_count=0;}
	int  size() const { return item_count;}
	int  empty() const {return item_count;}

		
};	



#endif
