/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _dictionary.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef __LEDA_DICTIONARY_H
#define __LEDA_DICTIONARY_H

#include <LEDA/dictionary.h>

//------------------------------------------------------------------------------
//
// Dictionaries with implementation parameter:
//
//   _dictionary<keytype,inftype,dic_impl> 
//
//------------------------------------------------------------------------------

/*{\Manpage {_dictionary} {K,I,impl} {Dictionaries with Implementation Parameter} }*/

/*{\Mdefinition
An instance of type \name\ is a dictionary implemented by data type $impl$.
$impl$ must be one of the dictionary implementations listed in
section \ref{Implementations Dictionaries} or a user defined data structure
fulfilling the specification given in section \ref{User Implementations
Dictionaries}. Note that depending on the actual implementation $impl$
the key type $K$ must either be linearly ordered or hashed.
}*/

/*{\Mtext
{\bf Example}\\
Using a dictionary implemented by skiplists to count the number of 
occurrences of the elements in a sequence of strings.
\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/_dictionary.h>
#include <LEDA/impl/skiplist.h>

main()
{
  _dictionary<string,int,skiplist> D;
  string s;
  dic_item it;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
  }

  forall_items(it,D) cout << D.key(it) << " : " <<  D.inf(it) << endl;

}
\endgroup
}*/



template <class K, class I, class impl> 

class _dictionary : private virtual impl, public dictionary<K,I>
{

int  int_type()              const { return LEDA_INT_TYPE(K); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(K,x,y); }
int  hash_fct(GenPtr x)      const { return LEDA_HASH(K,x); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(K,x,cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,cout); }

public:

dic_item lookup(K x) const { return dic_item(impl::lookup(Convert(x))); }

int defined(K x) const { return (lookup(x)==nil) ? false : true; }

void change_inf(dic_item it, I i)
{ impl::change_inf(impl::item(it),Convert(i));}

dic_item insert(K x,I y)
{ return dic_item(impl::insert(Convert(x),Convert(y))); }

void del(K x) { impl::del(Convert(x)); }
void del_item(dic_item it) { impl::del_item(impl::item(it)); }

K key(dic_item it) const { return LEDA_ACCESS(K,impl::key(impl::item(it))); }
I inf(dic_item it) const { return LEDA_ACCESS(I,impl::inf(impl::item(it)));}

int  size() const { return impl::size(); }
bool empty() const { return (size()==0) ? true : false; }
void clear() { impl::clear(); }

dic_item first_item() const { return dic_item(impl::first_item()); }
dic_item next_item(dic_item it) const 
{ return dic_item(impl::next_item(impl::item(it))); }

_dictionary<K,I,impl>& operator=(const _dictionary<K,I,impl>& D)
{ impl::operator=(D); return *this; }

 _dictionary() {}
 _dictionary(const _dictionary<K,I,impl>& D) : impl(D) {}
~_dictionary() { impl::clear(); }

};

#endif

