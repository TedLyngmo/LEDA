/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_ARRAY_H
#define LEDA_ARRAY_H

//------------------------------------------------------------------------------
// array
//------------------------------------------------------------------------------

#include <LEDA/impl/gen_array.h>


/*{\Manpage {array} {E} {One Dimensional Arrays} }*/


template<class E>

class array : public gen_array {

/*{\Mdefinition
    An instance $A$ of the parameterized data type \name\ is a mapping from
    an interval $I =[a..b]$ of integers, the index set of $A$, to the set of
    variables of data type $E$, the element type of $A$. $A(i)$ is called the
    element at position $i$.  }*/


int (*cmp_ptr)(const E&, const E&);   // user supplied cmp function

// define virtual functions for element type E

int cmp(GenPtr x, GenPtr y) const
{ if (cmp_ptr == 0)
     return LEDA_COMPARE(E,x,y);
  else
     return (*cmp_ptr)(LEDA_ACCESS(E,x),LEDA_ACCESS(E,y));
 }


int  int_type()                       const { return LEDA_INT_TYPE(E); }
void print_el(GenPtr& x,std::ostream& out) const { LEDA_PRINT(E,x,out);}
void read_el(GenPtr& x,std::istream& in)   const { LEDA_READ(E,x,in); }
void clear_entry(GenPtr& x)           const { LEDA_CLEAR(E,x); }
void copy_entry(GenPtr& x)            const { LEDA_COPY(E,x); }
void init_entry(GenPtr& x)            const { LEDA_CREATE(E,x); }


public:

/*{\Mcreation A}*/


array(int a, int b) : gen_array(a,b) { init(); }
/*{\Mcreate  creates an instance \var\ of type \name\ with index set $[a..b]$. }*/


array(int n) : gen_array(n)   { init(); }
/*{\Mcreate  creates an instance \var\ of type \name\ with index set $[0..n-1]$. }*/

array()  {}
/*{\Mcreate  creates an instance \var\ of type \name\ with empty index set.}*/



// copy constructor, destructor, operator=

 array(const array<E>& A) : gen_array(A) {}

~array()  { clear(); }

 array<E>& operator=(const array<E>& A)
 { gen_array::operator=(A); return *this; }




/*{\Moperations 1.2 5 }*/


#if defined(LEDA_CHECKING_OFF)
E  operator[](int x) const { return LEDA_ACCESS(E,v[x-Low]); }
E& operator[](int x)       { return LEDA_ACCESS(E,v[x-Low]); }
#else
E  operator[](int x) const { return LEDA_ACCESS(E,inf(x)); }
E& operator[](int x)       { return LEDA_ACCESS(E,entry(x)); }
#endif
/*{\Marrop     returns $A(x)$.\\
               \precond $a\le x\le b$.  }*/


int low() const {return gen_array::low();}
/*{\Mop        returns the minimal index $a$. }*/


int high() const {return gen_array::high();}
/*{\Mop        returns the maximal index $b$. }*/


void sort(int (*cmp)(const E&, const E&))
{ cmp_ptr = cmp; gen_array::sort(low(),high()); }
/*{\Mopl       sorts the elements of \var, using function $cmp$
	      to compare two elements, i.e., if $(in_a,\dots,in_b)$
	      and $(out_a,\dots,out_b)$ denote the values of the
	      variables $(A(a),\dots,A(b))$ before and after the
	      call of sort, then $cmp(out_i,out_j) \le 0$ for $i\le j$
	      and there is a permutation $\pi$ of $[a..b]$ such that
	      $out_i=in_{\pi(i)}$ for $a \le i \le b$.}*/



void sort() { cmp_ptr = 0; gen_array::sort(low(),high()); }
/*{\Mop       sorts the elements of \var\ according to the linear order
              of the element type $E$.\\
	      \precond A linear order on $E$
              must have been defined by $compare(const E\&, const E\&)$. }*/



void sort(int (*cmp)(const E&, const E&), int l, int h)
{ cmp_ptr = cmp; gen_array::sort(l,h); }
/*{\Mopl       sorts sub-array \var$[l..h]$ using compare function $cmp$.}*/



void sort(int l, int h)
{ cmp_ptr = 0; gen_array::sort(l,h); }
/*{\Mop        sorts sub-array \var$[l..h]$ using the linear order on $E$.}*/



void permute() { gen_array::permute(); }
/*{\Mop        the elemens of \var\ are randomly permuted.}*/



void permute(int l, int h) { gen_array::permute(l,h); }
/*{\Mop        the elements of \var$[l..h]$ are randomly permuted.}*/



int binary_search(int (*cmp)(const E&,const E&), E x)
{ cmp_ptr = cmp; return gen_array::binary_search(Convert(x));}
/*{\Mopl      performs a binary search for $x$. Returns $i$
	      with $A[i] = x$ if $x$ in $A$, $A$.low$()-1$
	      otherwise. Function $cmp$ is used to compare
	      two elements.\\
	      \precond $A$ must be sorted
	      according to $cmp$.}*/



int binary_search(E x)
{ cmp_ptr = 0; return gen_array::binary_search(Convert(x)); }
/*{\Mop       performs a binary search for $x$ using the default
              linear order on $E$.\\
	      \precond $A$ must be sorted.}*/


void read(std::istream& I) {gen_array::read(I);}

/*{\Mop       reads $b-a+1$ objects of type $E$ from the
	      input stream $I$ into the array $A$ using the
	      overloaded $Read$ function (cf.~section \ref{Overloading}).}*/


void read() {gen_array::read();}
/*{\Mop       calls $A$.read($cin$) to read $A$ from the
	      standard input stream $cin$.}*/



void read(string s) {gen_array::read(s);}
/*{\Mop        As above, uses string $s$ as prompt.}*/



void print(std::ostream& O, char space = ' ') const { gen_array::print(O,space);}
/*{\Mopl      prints the contents of array $A$ to the output
	      stream $O$ using the overloaded $Print$ function
	      (cf.~section \ref{Overloading}) to print each element. The
	      elements are separated by the character $space$.}*/


void print(char space = ' ') const {gen_array::print(space);}
/*{\Mop       calls $A$.print($cout$, $space$) to print $A$ on
	      the standard output stream $cout$.}*/



void print(string s, char space = ' ') const {gen_array::print(s,space);}
/*{\Mopl        As above, uses string $s$ as header.}*/


// Iteration

GenPtr forall_loop_test(GenPtr it, E& x) const
{ if (it) x = LEDA_ACCESS(E,*(GenPtr*)it);
  return it;
 }

/*
friend void Print(const array<E>& A, std::ostream& out) { A.print(out); }
friend void Read(array<E>& A, std::istream& in)         { A.read(in); }
*/

};


/*{\Mimplementation
Arrays are implemented by \CC vectors. The access operation takes time
$O(1)$, the sorting is realized by quicksort (time $O(n \log n)$) and
the binary\_search operation takes time $O(\log n)$, where $n = b-a+1$.
The space requirement is $O(|I|* sizeof(E))$.}*/

#endif
