/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  h_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_H_ARRAY_H
#define LEDA_H_ARRAY_H

//------------------------------------------------------------------------------
// h_array
//------------------------------------------------------------------------------
#include <LEDA/basic.h>
#include <LEDA/impl/ch_array.h>


/*{\Manpage {h_array} {I,E} {Hashing Arrays} }*/

template<class I, class E>

class h_array : private ch_array {

/*{\Mdefinition
An instance $A$ of the parameterized data type \name\ (hashing array)
is an injective mapping from a hashed data type $I$ (cf. section
\ref{Hashed Types}), called the index type of $A$, to the set of variables
of arbitrary type $E$, called the element type of $A$. We use $A(i)$ to
denote the variable indexed by $i$.}*/

 E X;

 int  int_type()           const { return LEDA_INT_TYPE(I); }
 int  hash_fct(GenPtr x)   const { return LEDA_HASH(I,x); }
 void copy_inf(GenPtr& x)  const { LEDA_COPY(E,x);  }
 void clear_inf(GenPtr& x) const { LEDA_CLEAR(E,x); }
 void init_inf(GenPtr& x)  const { x = Copy(X); }


 public:

/*{\Mcreation A }*/


h_array(E x, int sz) : ch_array(sz) { X = x; }

h_array(E x) : ch_array(1) { X = x; }

/*{\Mcreate
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, assigns $x$ to all variables in the range of $a$ and initializes $A$
with $a$. }*/

 h_array() { }
 h_array(const h_array<I,E>& A): ch_array((ch_array&)A) { X = A.X; }
~h_array() { }



/*{\Moperations 2 4.5 }*/

E& operator[](I i) { return LEDA_ACCESS(E,access(Convert(i))); }
/*{\Marrop        returns the variable $A(i)$}*/

E operator[](I i) const { return LEDA_ACCESS(E,access(Convert(i))); }

bool defined(I i) const { return (lookup(Convert(i)) != nil); }
/*{\Mop      returns true if $i \in dom(A)$, false otherwise; here
	     $dom(A)$ is the set of all $i\in I$ for which $A[i]$ has
	     already been executed.}*/

// iteration

ch_array_item first_item() const { return ch_array::first_item(); }

void loop_to_succ(GenPtr& x) const
{ x = ch_array::next_item(*(ch_array_item*)&x); }

GenPtr forall_loop_test(GenPtr it, E& x) const
{ if (it) x = LEDA_ACCESS(E,ch_array::inf(ch_array_item(it)));
  return it;
}

GenPtr forall_defined_test(GenPtr it, I& x) const
{ if (it) x = LEDA_ACCESS(I,ch_array::key(ch_array_item(it)));
  return it;
}

};


/*{\Mtext
\bigskip
{\bf forall\_defined}($i,A$)
$\{$ ``the elements from $dom(A)$ are successively assigned to $i$'' $\}$ }*/


/*{\Mimplementation
Hashing arrays are implemented by hashing with chaining. Access operations
take expected time $O(1)$. In many cases, hashing arrays are more efficient
than dictionary arrays (cf. \ref{Dictionary Arrays}).}*/

#endif
