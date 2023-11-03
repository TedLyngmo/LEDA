/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  queue.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_QUEUE_H
#define LEDA_QUEUE_H

//------------------------------------------------------------------------------
// queue                                                                
//------------------------------------------------------------------------------

#include <LEDA/basic.h>
#include <LEDA/impl/slist.h>


/*{\Manpage {queue} {E} {Queues}}*/

template<class E>

class queue : private SLIST {

/*{\Mdefinition
An instance $Q$ of the parameterized data type \name\ is
a sequence of elements of data type $E$, called the element
type of $Q$. Elements are inserted at one end (the rear) and deleted at the
other end (the front) of $Q$. The size of $Q$ is the length of the sequence;
a queue of size zero is called the empty queue.}*/

  void copy_el(GenPtr& x)  const { LEDA_COPY(E,x); }
  void clear_el(GenPtr& x) const { LEDA_CLEAR(E,x); }

public:

/*{\Mcreation Q }*/ 

  queue() {}
/*{\Mcreate creates an instance \var\ of type \name. \var\ is initialized 
            with the empty queue.}*/

  queue(const queue<E>& Q) : SLIST(Q) {}
 ~queue() { clear(); }
  queue<E>& operator=(const queue<E>& Q) 
  { SLIST::operator=(Q); return *this; }


/*{\Moperations 2 4 }*/

E top() const { return LEDA_ACCESS(E,SLIST::head()); }
/*{\Mop       returns the front element of \var.\\
              \precond $Q$ is not empty.}*/


E pop() { E x=top(); SLIST::pop(); return x; }
/*{\Mop       deletes and returns the front element of \var.\\
	      \precond $Q$ is not empty.}*/

void append(E x) { SLIST::append(Copy(x)); }
/*{\Mop       appends $x$ to the rear end of \var.}*/

int  size() const { return SLIST::length(); }
/*{\Mop       returns the size of \var.}*/

bool  empty() const { return SLIST::empty(); }
/*{\Mop       returns true if \var\ is empty, false otherwise.}*/

void clear() { SLIST::clear(); }
/*{\Mop       makes \var\ the empty queue.}*/

};

/*{\Mimplementation
Queues are implemented by singly linked linear lists. All operations take time 
$O(1)$, except clear which takes time $O(n)$, where $n$ is the size of the 
queue.}*/

#endif
