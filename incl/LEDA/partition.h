/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  partition.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_PARTITION_H
#define LEDA_PARTITION_H

//------------------------------------------------------------------------------
// partition   (union find)
//------------------------------------------------------------------------------

#include <LEDA/basic.h>


/*{\Manpage {partition} {} {Partitions} }*/

class partition_node {
/*{\Mdefinition
An instance  $P$ of the data type $partition$ consists of a finite set of
items ($partition\_item$) and a partition of this set
into blocks.}*/

friend class partition;

partition_node* father;
partition_node* next;
int size;
GenPtr info;

public:

partition_node(GenPtr x, partition_node* n)  
{ 
  father=0; size=1; info=x; next = n; 
 }

  LEDA_MEMORY(partition_node)

};


// a partition item is a pointer to a partition node:

typedef partition_node* partition_item;



class partition {

virtual void clear_inf(GenPtr&) const {}

partition_item used_items;                 // List of used partition items

public:

/*{\Mcreation P }*/

partition() { used_items = 0; }  
/*{\Mcreate creates an instance $P$ of type $partition$ and initializes it to 
            the empty partition.}*/

virtual ~partition() { clear(); }

/*{\Moperations 3 4}*/

partition_item make_block(GenPtr x) 
{ used_items = new partition_node(x,used_items); 
  return used_items; 
 }

partition_item make_block() 
{ return make_block(nil);
}

/*{\Mopl      returns a new $partition\_item$ $it$ and adds
	     the block $\{it\}$ to partition $P$.}*/

partition_item find(partition_item p);
/*{\Mopl     returns a canonical item of the block that
	     contains item $p$, i.e., if $P$.same\_block($p,q$)
	     then $P$.find($p$) = $P$.find($q$).\\
	     \precond $p$ is an item in $P$. }*/

bool  same_block(partition_item p, partition_item q) 
{ return find(p)==find(q); }
/*{\Mopl      returns true if $p$ and $q$ belong to the same
	      block of partition $P$.\\
	      \precond $p$ and $q$ are items in $P$.}*/

void  union_blocks(partition_item p, partition_item q);
/*{\Mopl      unites the blocks of partition $P$ containing
	     items $p$ and $q$.\\
	     \precond $p$ and $q$ are items in $P$.}*/

GenPtr   inf(partition_item a) { return find(a)->info; }

void  set_inf(partition_item a, GenPtr x) { find(a)->info = x; }

void clear();                      // deletes all used items


partition_item first_item() const  { return used_items;  }
partition_item next_item(partition_item it) const  { return it->next; }


};

/*{\Mimplementation
Partitions are implemented by the union find algorithm with weighted union
and path compression (cf.~\cite{T83}).  Any sequence of $n$ make\_block and 
$m \ge n$ other operations takes time $O(m\alpha(m,n))$.}*/

/*{\Mexample
    Spanning Tree Algorithms (cf. section \ref{Graph Algorithms}).}*/


//------------------------------------------------------------------------------
// PARTITION  (named partitions)
//-----------------------------------------------------------------------------

template <class E>

class PARTITION : public partition {

void clear_inf(GenPtr& x) const { LEDA_CLEAR(E,x); }

public:

partition_item make_block(E x) { return partition::make_block(Copy(x)); }

E  inf(partition_item a)       { return (E)partition::inf(a); }

void  set_inf(partition_item a, E x) { partition::set_inf(a,Copy(x)); }

 PARTITION() {}
~PARTITION() {}
};


#endif
