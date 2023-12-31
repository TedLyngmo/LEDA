/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  b_stack.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_BSTACK_H
#define LEDA_BSTACK_H

//------------------------------------------------------------------------------
// bounded stacks 
//------------------------------------------------------------------------------

#include <LEDA/basic.h>

/*{\Manpage {b_stack} {E} {Bounded Stacks}}*/


template<class E> 

class b_stack
{
/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is a stack
(see section \ref{Stacks}) of bounded size.}*/

E* first;  // start of array
E* stop;   // one position behind last element
E* free;   // pointer to first free element

public:

/*{\Mcreation S }*/


b_stack(int n)
{
#if !defined(LEDA_CHECKING_OFF)
  if (n<1) error_handler(99,"bounded stack: bad size");
#endif
  free = first = new E[n];
  stop = first + n;
  if (first==0) error_handler(99,"bounded stack: out of memory");
 }
/*{\Mcreate creates an instance \var\ of type \name\ that can hold up to $n$ 
            elements.  \var\ is initialized with the empty stack.}*/


virtual ~b_stack() { delete [] first; }

/*{\Moperations 2 4}*/

E top() const
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==first) error_handler(99,"bounded stack: empty");
#endif
  return *(free-1);
  }

/*{\Mop     returns the top element of \var.\\
	    \precond $S$ is not empty.}*/

E pop()
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==first) error_handler(99,"b_stack underflow");
#endif
return *--free;
}
/*{\Mop   deletes and returns the top element of \var.\\
	  \precond $S$ is not empty.}*/

void push(const E& x)
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==stop) error_handler(99,"bounded stack: overflow");
#endif
  *free++ = x;
}
/*{\Mop  adds $x$ as new top element to \var.\\
	 \precond $S$.size() $< n$.}*/

void clear() { free = first; }
/*{\Mop   makes \var\ the empty stack.}*/

int   size()  const { return free - first; }
/*{\Mop    returns the size of \var.}*/

bool   empty() const { return (free==first) ? true : false; }
/*{\Mop  returns true if \var\ is empty, false otherwise.}*/

};

/*{\Mimplementation
Bounded stacks are implemented by \CC vectors. All operations take
time $O(1)$. The space requirement is $O(n)$.}*/

#endif
