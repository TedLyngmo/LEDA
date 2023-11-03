/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  map.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_MAP_H
#define LEDA_MAP_H

//------------------------------------------------------------------------------
// map
//------------------------------------------------------------------------------
#include <LEDA/basic.h>
#include <LEDA/impl/ch_map.h>


/*{\Manpage {map} {I,E} {Maps}}*/

template<class I, class E>

class map : private ch_map {

/*{\Mdefinition
An instance $M$ of the parameterized data type \name\ is an injective
mapping from the data type $I$, called the index type of $M$,
to the set of variables of data type $E$, called the element type of $M$.
$I$ must be a pointer, item, or handle type or the type int. We use
$M(i)$ to denote the variable indexed by $i$.}*/


 E X;

 void copy_inf(GenPtr& x)   const { LEDA_COPY(E,x);  }
 void clear_inf(GenPtr& x)  const { LEDA_CLEAR(E,x); }
 void init_inf(GenPtr& x)   const { x = Copy((E&)X); }


 public:

/*{\Mcreation M }*/

map() { }
/*{\Mcreate
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, initializes all variables in the range of $a$ using the
default constructor of type $E$ and assigns $a$ to $M$.}*/

map(E x,int index_sz, int table_sz) : ch_map(index_sz,table_sz) { X = x; }
map(E x,int index_sz) : ch_map(index_sz) { X = x; }
map(E x) : ch_map(1) { X = x; }
/*{\Mcreate
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, assigns $x$ to all variables in the range of $a$ and initializes $M$
with $a$.}*/

 map(const map<I,E>& M): ch_map((ch_map&)M) { X = M.X; }
~map() { clear_entries(); }


/*{\Moperations 2 4.5 }*/

E& operator[](I i) { return LEDA_ACCESS(E,access(ID_Number(i))); }
/*{\Marrop        returns the variable $M(i)$.}*/

E operator[](I i) const { return LEDA_ACCESS(E,access(ID_Number(i))); }

bool defined(I i) const { return lookup(ID_Number(i)) != nil; }
/*{\Mop      returns true if $i \in dom(M)$, false otherwise; here
             $dom(M)$ is the set of all $i\in I$ for which $M[i]$ has
             already been executed.}*/

};


/*{\Mimplementation
Maps are implemented by hashing with chaining and table doubling.
Access operations $M[i]$ take expected time $O(1)$. }*/



#endif
