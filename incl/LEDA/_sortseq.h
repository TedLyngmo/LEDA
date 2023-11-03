/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _sortseq.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LEDA_SORTSEQ_H
#define _LEDA_SORTSEQ_H

#include <LEDA/sortseq.h>


//------------------------------------------------------------------------------
//
// Sorted sequences with implementation parameter:
//
//   _sortseq<K,I,seq_impl> 
//
//------------------------------------------------------------------------------

/*{\Manpage {_sortseq} {K,I,impl} {Sorted Sequences with Implementation Parameter} }*/

/*{\Mdefinition
An instance of type \name\ is a sorted sequence implemented by data type
$impl$. $impl$ must be one of the sorted sequence implementations listed in
section \ref{Implementations Dictionaries} or a user defined data structure
fulfilling the specification given in section \ref{User Implementations
Sorted Sequences}. Note that the key type $K$ must be linearly ordered.
}*/

/*{\Mtext
{\bf Example}\\
Using a sorted sequence implemented by skiplists to list all elements in a 
sequence of strings lying lexicographically between two given search strings.
\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/_sortseq.h>
#include <LEDA/impl/skiplist.h>

main()
{ 
 _sortseq<string,int,skiplist> S;
 string s1,s2;

 while ( cin >> s1 &&  s1 != "stop" )  S.insert(s1,0);

 while ( cin >> s1 >> s2 )
 { seq_item start = S.locate(s1);
   seq_item stop  = S.locate(s2);
   for (seq_item it = start; it != stop; it = S.succ(it))
      cout << S.key(it) << endl; 
  }

}
\endgroup

}*/



template <class K, class I, class impl> 

class _sortseq : private virtual impl, public sortseq<K,I>
{

int  int_type()              const { return LEDA_INT_TYPE(K); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(K,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(K,x,cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,cout); }

public:

K key(seq_item it) const { return LEDA_ACCESS(K,impl::key(impl::item(it))); }
I inf(seq_item it) const { return LEDA_ACCESS(I,impl::inf(impl::item(it))); }

seq_item lookup(K y) const { return (seq_item)impl::lookup(Convert(y)); }
seq_item locate(K y) const { return (seq_item)impl::locate(Convert(y)); }

seq_item locate_succ(K y) const 
{ return (seq_item)impl::locate_succ(Convert(y)); }

seq_item locate_pred(K y) const 
{ return (seq_item)impl::locate_pred(Convert(y)); }

seq_item min() const { return (seq_item)impl::min(); }
seq_item max() const { return (seq_item)impl::max(); }

seq_item succ(seq_item x) const { return (seq_item)impl::succ(impl::item(x)); }
seq_item succ(K y) const { return locate_succ(y); }

seq_item pred(seq_item x) const { return (seq_item)impl::pred(impl::item(x)); }
seq_item pred(K y) const { return locate_pred(y); }

seq_item insert(K y,I x)
{ return (seq_item)impl::insert(Convert(y),Convert(x));}

seq_item insert_at(seq_item it,K y,I x)
{ return (seq_item)impl::insert_at_item(impl::item(it),Convert(y),Convert(x));}

void reverse_items(seq_item it1, seq_item it2) 
{ impl::reverse_items(impl::item(it1),impl::item(it2)); }

void flip_items(seq_item it1, seq_item it2) { reverse_items(it1,it2); }

void del(K y) { impl::del(Convert(y)); }
void del_item(seq_item it) { impl::del_item(impl::item(it)); }
void change_inf(seq_item it, I i) 
{ impl::change_inf(impl::item(it),Convert(i));}

int  size()  const { return impl::size(); }
bool empty() const { return (size()==0) ? true : false; }
void clear() { impl::clear(); }

void split(seq_item it,sortseq<K,I>& S1,sortseq<K,I>& S2)
{ impl::split_at_item(impl::item(it),*(impl*)&S1,*(impl*)&S2); }

sortseq<K,I>& conc(sortseq<K,I>& S)
{ impl::conc(*(impl*)&S); return *this; }

 _sortseq() {}
 _sortseq(const _sortseq<K,I,impl>& S) : impl(S) {}
 _sortseq<K,I,impl>& operator=(const _sortseq<K,I,impl>& S)
 { impl::operator=(S); return *this; }
~_sortseq() { impl::clear(); }

};

#endif
