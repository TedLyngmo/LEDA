/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  int_set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_INTSET_H
#define LEDA_INTSET_H

//------------------------------------------------------------------------------
/* int_set: integer sets implemented by bit vectors                           */
//------------------------------------------------------------------------------

#include <LEDA/basic.h>

#define SIZE_OF_ULONG  (8 * sizeof(unsigned long))

/*{\Manpage {int_set} {} {Integer Sets}}*/

class int_set {

/*{\Mdefinition
An instance $S$ of the data type $int\_set$ is a subset of a fixed interval
$[a..b]$ of the integers.}*/


  unsigned long*  V;
  int size;
  int low;

public:

/*{\Mcreation S }*/

int_set(int a, int b); 
/*{\Mcreate creates an instance \var\ of type $int\_set$ for elements from 
            $[a..b]$ and initializes it to the empty set.}*/

 int_set(int n); 
/*{\Mcreate creates an instance \var\ of type $int\_set$ for elements from 
            $[0..n-1]$ and initializes it to the empty set.}*/



 int_set(const int_set&);
~int_set() { delete V; } 



/*{\Moperations 1.8 5 }*/

void insert(int x);
/*{\Mop      adds $x$ to \var.\\
	     \precond $a\le x\le b$.}*/

void del(int x);
/*{\Mop     deletes $x$ from \var.\\
            \precond $a\le x\le b$.}*/

int  member(int x) const;
/*{\Mop      returns true if $x$ in \var, false otherwise.\\
	     \precond $a\le x\le b$.}*/

void clear();
/*{\Mop      makes \var\ the empty set.}*/

int_set& join(const int_set& );
int_set& intersect(const int_set&);
int_set& complement();


int_set& operator=(const int_set& S1);
/*{\Mbinop    assignment.}*/

int_set& operator|=(const int_set&);
int_set& operator&=(const int_set&);

int_set  operator|(const int_set& S1);
/*{\Mbinop     returns the union of $S$ and $S1$}*/

int_set  operator&(const int_set& S1);
/*{\Mbinop     returns the intersection of $S$ and $S1$}*/

int_set  operator~();
/*{\Munop     returns the complement of $S$.}*/

};

inline int  int_set::member(int x)  const
{ int i = x-low; 
  return V[i/SIZE_OF_ULONG] & (1 << (i%SIZE_OF_ULONG)); 
 }

inline void int_set::insert(int x) 
{ int i  =  x-low; 
  V[i/SIZE_OF_ULONG] |= (1 << (i%SIZE_OF_ULONG)); 
 }

inline void int_set::del(int x)    
{ int i   = x-low; 
  V[i/SIZE_OF_ULONG] &= ~(1 << (i%SIZE_OF_ULONG)); 
 }

inline int_set& int_set::operator|=(const int_set& s) { return join(s); }

inline int_set& int_set::operator&=(const int_set& s) { return intersect(s); }


/*{\Mimplementation
Integer sets are implemented by bit vectors. Operations insert, delete,
member, empty, and size take constant time. clear, intersection, union 
and complement take time $O(b-a+1)$.}*/

#endif
