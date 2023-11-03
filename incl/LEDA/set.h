/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_SET_H
#define LEDA_SET_H

//------------------------------------------------------------------------------
// set             
//------------------------------------------------------------------------------

#include <LEDA/basic.h>
#include <LEDA/impl/rs_tree.h>


/*{\Manpage {set} {E} {Sets}}*/

template<class E>

class set : private rs_tree {

/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is a collection of
elements of the linearly ordered type $E$, called the element type of $S$. The
size of $S$ is the number of elements in $S$, a set of size zero is called the
empty set.}*/

int  int_type()              const { return LEDA_INT_TYPE(E); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(E,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(E,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(E,x); }

public:

/*{\Mcreation S }*/

 set() {}
/*{\Mcreate creates an instance \var\ of type \name\ and initializes it to 
            the empty set.}*/

 set(const set<E>& S) : rs_tree(S) {}
~set() { clear(); }
 set<E>& operator=(const set<E>& S) 
 { rs_tree::operator=(S); return *this;}


/*{\Moperations 1.8 5}*/

virtual void insert(E x) { rs_tree::insert(Convert(x),0); }
/*{\Mop        adds $x$ to \var.}*/

virtual void del(E x) { rs_tree::del(Convert(x)); }
/*{\Mop        deletes $x$ from \var.}*/

virtual bool member(E x) const { return (rs_tree::lookup(Convert(x))!=nil); }
/*{\Mop        returns true if $x$ in \var, false otherwise.}*/

virtual E choose() const 
{ return LEDA_ACCESS(E,rs_tree::key(rs_tree::min())); }
/*{\Mop        returns an element of \var.\\
               \precond \var\ is not empty.}*/

virtual bool empty() const {return rs_tree::empty();}
/*{\Mop        returns true if \var\ is empty, false otherwise.}*/

virtual int size() const {return rs_tree::size();}
/*{\Mop        returns the size of \var.}*/

virtual void clear() { rs_tree::clear(); }
/*{\Mop        makes \var\ the empty set.}*/


// iteration

virtual GenPtr first_item()  const { return rs_tree::first_item(); }

virtual void loop_to_succ(GenPtr& x) const 
{ x=rs_tree::next_item(rs_tree_item(x)); }

virtual GenPtr forall_loop_test(GenPtr it, E& y) const
{ if (it) y = LEDA_ACCESS(E,rs_tree::key(rs_tree_item(it)));
  return it;
 }

#if defined(__ELSE_SCOPE_BUG__)
  GenPtr forall_loop_item;
  GenPtr& Forall_Loop_Item() const { return (GenPtr&)forall_loop_item; }
#endif


/*{\Mtext
\bigskip
{\bf Iteration}

\medskip
{\bf forall}($x,S$) 
$\{$  ``the elements of $S$ are successively assigned to $x$''  $\}$ }*/


};

/*{\Mimplementation
Sets are implemented by randomized search trees \cite{AS89}. Operations insert,
del, member take time $O(\log n)$, empty, size take time $O(1)$, and clear 
takes time $O(n)$, where $n$ is the current size of the set.}*/

#endif

