/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  stack.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_STACK_H
#define LEDA_STACK_H

#include <LEDA/basic.h>
#include <LEDA/impl/slist.h>

//------------------------------------------------------------------------------
// stacks
//------------------------------------------------------------------------------


/*{\Manpage {stack} {E} {Stacks}}*/

template<class E>

class stack : private SLIST {

/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is
a sequence of elements of data type $E$, called the element
type of $S$. Insertions or deletions of elements take place only at one end of
the sequence, called the top of $S$. The size of $S$ is the length of the
sequence, a stack of size zero is called the empty stack.}*/

  void copy_el(GenPtr& x)  const { LEDA_COPY(E,x); }
  void clear_el(GenPtr& x) const { LEDA_CLEAR(E,x); }

public:


/*{\Mcreation S }*/

stack() {}
/*{\Mcreate creates an instance \var\ of type \name. \var\ is initialized
            with the empty stack.}*/

 stack(const stack<E>& S) : SLIST(S) {}
~stack() { clear(); }
 stack<E>& operator=(const stack<E>& S)
                     { return (stack<E>&)SLIST::operator=(S); }


/*{\Moperations 2 4}*/

E top() const { return LEDA_ACCESS(E,SLIST::head());}
/*{\Mop      returns the top element of \var.\\
             \precond $S$ is not empty.}*/

void push(E x) { SLIST::push(Copy(x)); }
/*{\Mop      adds $x$ as new top element to \var.}*/


E pop() { E x=top(); SLIST::pop(); return x; }
/*{\Mop      deletes and returns the top element of \var.\\
             \precond $S$ is not empty.}*/


int  size() const { return SLIST::length(); }
/*{\Mop      returns the size of \var.}*/

bool  empty() const { return SLIST::empty(); }
/*{\Mop      returns true if \var\ is empty, false otherwise.}*/

void clear() { SLIST::clear(); }
/*{\Mop      makes \var\ the empty stack.}*/

};

/*{\Mimplementation
Stacks are implemented by singly linked linear lists. All operations take
time $O(1)$, except clear which takes time $O(n)$, where $n$ is the size of
the stack.}*/

#endif


