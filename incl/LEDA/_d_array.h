/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _d_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef _LEDA_D_ARRAY_H
#define _LEDA_D_ARRAY_H

#include <LEDA/d_array.h>

//------------------------------------------------------------------------------
//
// Dictionary arrays with implementation parameter:
//
//   _d_array<I,E,impl> 
//
//------------------------------------------------------------------------------

/*{\Manpage {_d_array} {I,E,impl} {Dictionary Arrays with Implementation Parameter} }*/

/*{\Mdefinition
An instance of type \name\ is a dictionary array implemented by data type 
$impl$. $impl$ must be one of the dictionary implementations listed in
section \ref{Implementations Dictionaries} or a user defined data structure
fulfilling the specification given in section \ref{User Implementations
Dictionaries}. Note that depending on the actual implementation $impl$
the index type $I$ must either be linearly ordered or hashed.
}*/
  
/*{\Mtext
{\bf Example}\\
Using a dictionary array implemented by hashing with chaining  ($ch\_hash$)
to count the number of occurences of the elements in a sequence of strings.
\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/_d_array.h>
#include <LEDA/impl/ch_hash.h>

//we first have to define a hash function for strings

int Hash(const string& x) { return (x.length() > 0) ? x[0] : 0; }

main()
{ 
  _d_array<string,int,ch_hash> N(0);
  string s;

  while (cin >> s) N[s]++;

  forall_defined(s,N) cout << s << "  " << N[s] << endl;

}
\endgroup

}*/





template <class I, class E, class impl> 

class _d_array : private virtual impl, public d_array<I,E>
{

E init;

int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(I,x,y); }
int  hash_fct(GenPtr x)      const { return LEDA_HASH(I,x); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(E,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(I,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(E,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(I,x,cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(E,x,cout); }
int  int_type()              const { return LEDA_INT_TYPE(I); }

public:

E& operator[](const I& y)
{ d_array_item i=(d_array_item)impl::lookup(Convert(y));
  if (i==nil) i=(d_array_item)impl::insert(Convert(y),Convert(this->init));
  return LEDA_ACCESS(E,impl::info(impl::item(i)));
}

bool defined(I y) const
{ return (impl::lookup(Convert(y))!=nil); }

void undefine(I y) { impl::del(Convert(y)); }


// iteration

void loop_to_succ(GenPtr& x) const { x = impl::next_item(impl::item(x)); }


GenPtr forall_defined_test(GenPtr it, I& x) const
{ if (it) x = LEDA_ACCESS(I,impl::key(impl::item(it)));
  return it;
}

GenPtr forall_loop_test(GenPtr it, E& x) const
{ if (it) x = LEDA_ACCESS(E,impl::inf(impl::item(it)));
  return it;
}


_d_array<I,E,impl>& operator=(const _d_array<I,E,impl>& A)
{ impl::operator=(A); this->init=A.init; return *this; }

 _d_array() {}
 _d_array(E i) { this->init=i; }
 _d_array(const _d_array<I,E,impl>& A) : impl(A) { this->init=A.init; }
~_d_array() { impl::clear(); } 

};


#endif
