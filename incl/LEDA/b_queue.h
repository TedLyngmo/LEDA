/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  b_queue.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_BQUEUE_H
#define LEDA_BQUEUE_H

//------------------------------------------------------------------------------
// bounded queues
//------------------------------------------------------------------------------

#include <LEDA/basic.h>

/*{\Manpage {b_queue} {E} {Bounded Queues}}*/

template<class E> 

class b_queue  
{
/*{\Mdefinition
An instance $Q$ of the parameterized data type \name\ is a queue
(see section \ref{Queues}) of bounded size.}*/


E* first;     // first element of array
E* stop;      // one position behind last element of array
E* start;     // current start of queue (wraps around)
E* end;       // one position behind end of queue (wraps around)

public:										

/*{\Mcreation Q}*/


b_queue(int n) 
{
#if !defined(LEDA_CHECKING_OFF)
  if (n<1) error_handler(88,"_b_queue: bad size");
#endif
  first = new E[n];
  if (first==0) error_handler(88,"_b_queue: out of memory");
  stop  = first+n;
  start = end = first; 
}

/*{\Mcreate creates an instance \var\ of type \name\ that can hold up to $n$ 
           elements.  \var\ is initialized with the empty queue.}*/


virtual ~b_queue() { delete [] first; }

/*{\Moperations 2 5}*/

E top() const
{
#if !defined(LEDA_CHECKING_OFF)
  if (start==end) error_handler(88, "_b_queue empty");
#endif
  return *start;
}

/*{\Mop  returns the front element of \var.\\
         \precond $Q$ is not empty.}*/


E pop()
{
#if !defined(LEDA_CHECKING_OFF)
 if (start==end) error_handler(88, "_b_queue underflow");
#endif
  E x = *start++;
  if (start == stop) start = first;
  return x;
}

/*{\Mop  deletes and returns the front element of \var.\\
         \precond $Q$ is not empty.}*/

void append(E& x)
{ *end++ = x;
  if (end == stop) end = first;
#if !defined(LEDA_CHECKING_OFF)
  if (start==end) error_handler(88, "_b_queue overflow");
#endif
}

/*{\Mop   appends $x$ to the rear end of \var.\\
	  \precond $Q$.size()$ < n$.}*/


void clear() { start = end = first; }
/*{\Mop    makes \var\ the empty queue.}*/


int size() const 
{ int s = end-start;
  return (s<0) ?  (stop-first+s) : s;
}
/*{\Mop    returns the size of \var.}*/


bool empty() const { return (size()==0) ? true : false; }
/*{\Mop    returns true if \var\ is empty, false otherwise.}*/


};


/*{\Mimplementation
Bounded queues are implemented by circular arrays. All operations take 
time $O(1)$. The space requirement is $O(n)$.}*/

#endif
