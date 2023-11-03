/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  sortseq.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_SORTSEQ_H
#define LEDA_SORTSEQ_H

/*{\Manpage {sortseq} {K,I} {Sorted Sequences}}*/

/*
#include <LEDA/impl/rs_tree.h>
#define SEQ_DEF_IMPL rs_tree
#define SEQ_BASE_IMPL bin_tree
typedef rs_tree_item seq_item;
*/

#include <LEDA/impl/skiplist.h>
#define SEQ_DEF_IMPL skiplist
#define SEQ_BASE_IMPL skiplist
typedef skiplist_item seq_item;


template<class K, class I>

class sortseq : public virtual SEQ_DEF_IMPL {

/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is a sequence of
items ($seq\_item$). Every item contains a key from the linearly ordered data
type $K$, called the key type of $S$, and an information from data type $I$,
called the information type  of $S$. The number of items in $S$ is called the
size of $S$. A sorted sequence of size zero is called empty. We use $\<k,i\>$ to
denote a $seq\_item$ with key $k$ and information $i$ (called the information
associated with key $k$). For each $k \in K$ there is at most one item
$\<k,i\> \in S$.

The linear order on $K$ may be time-dependent, e.g., in an algorithm that
sweeps an arrangement of lines by a vertical sweep line we may want to order
the lines by the y-coordinates of their intersections with the sweep line.
However, whenever an operation (except for reverse\_items) is applied to a
sorted sequence $S$, the keys of $S$ must form an increasing sequence according
to the currently valid linear order on $K$. For operation reverse\_items this
must hold after the execution of the operation.}*/


int  int_type()              const { return LEDA_INT_TYPE(K); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(K,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(K,x,std::cout); }
void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,std::cout); }

public:

/*{\Mcreation S }*/

sortseq() {}
/*{\Mcreate creates an instance \var\ of type \name\ and initializes it to
            the empty sorted sequence.}*/

sortseq(const sortseq<K,I>& w) : SEQ_BASE_IMPL(w) {}
sortseq<K,I>& operator=(const sortseq<K,I>& w)
{ SEQ_DEF_IMPL::operator=(w); return *this; }
virtual ~sortseq()   { clear(); }


/*{\Moperations 2.8 3.5}*/

virtual K  key(seq_item it) const
{ return LEDA_ACCESS(K,SEQ_DEF_IMPL::key(it)); }
/*{\Mop         returns the key of item $it$.\\
	        \precond $it$ is an item in \var.}*/

virtual I  inf(seq_item it) const
{ return LEDA_ACCESS(I,SEQ_DEF_IMPL::inf(it)); }
/*{\Mop         returns the information of item $it.$\\
	        \precond $it$ is an item in \var.}*/


virtual seq_item lookup(K k) const
{ return SEQ_DEF_IMPL::lookup(Convert(k)); }
/*{\Mop         returns the item with key $k$
	        (nil if no such item exists in \var).}*/

virtual seq_item insert(K k, I i)
{ return SEQ_DEF_IMPL::insert(Convert(k),Convert(i)); }
/*{\Mop         associates information $i$ with key $k$: If
	        there is an item $\<k,j\>$ in \var\ then $j$ is
		replaced by $i$, else a new item $\<k,i\>$ is
		added to \var. In both cases the item is
		returned.}*/

virtual seq_item insert_at(seq_item it, K k, I i)
{ return SEQ_DEF_IMPL::insert_at_item(it,Convert(k),Convert(i)); }
/*{\Mopl        Like insert($k,i$), the item $it$ gives the
	        position of the item $\<k,i\>$ in the sequence.\\
		\precond $it$ is an item in $S$ with either
		key($it$) is maximal with key($it) \le k$ or
		key($it$) is minimal with key$(it) \ge k$.}*/

virtual seq_item locate_succ(K k) const
{ return SEQ_DEF_IMPL::locate_succ(Convert(k)); }
/*{\Mop         returns the item $\<k',i\>$ in \var\ such that
	        $k'$ is minimal with $k' \ge k$ (nil if no
		such item exists).}*/

virtual seq_item locate_pred(K k) const
{ return SEQ_DEF_IMPL::locate_pred(Convert(k)); }
/*{\Mop         returns the item $\<k',i\>$ in \var\ such that
	        $k'$ is maximal with $k' \le k$ (nil if no
		such item exists).}*/

virtual seq_item locate(K k) const
{ return SEQ_DEF_IMPL::locate_succ(Convert(k)); }
/*{\Mop         returns \var.locate\_succ$(K\ k)$. }*/


virtual seq_item succ(seq_item it) const { return SEQ_DEF_IMPL::succ(it); }
/*{\Mop        returns the successor item of $it$, i.e., the
	       item $\<k,i\>$ in \var\ such that $k$ is minimal
	       with $k > key(it)$ (nil if no such item exists).\\
	       \precond $it$ is an item in \var.}*/

virtual seq_item pred(seq_item it) const { return SEQ_DEF_IMPL::pred(it); }
/*{\Mop        returns the predecessor item of $it$, i.e., the
	       item $\<k,i\>$ in \var\ such that $k$ is maximal
	       with $k < key(it)$ (nil if no such item exists).\\
	       \precond $it$ is an item in \var.}*/

virtual seq_item min() const { return SEQ_DEF_IMPL::min(); }
/*{\Mop         returns the item with minimal key
	        (nil if \var\ is empty).}*/

virtual seq_item max() const { return SEQ_DEF_IMPL::max(); }
/*{\Mop         returns the item with maximal key
	        (nil if \var\ is empty).}*/


virtual void flip_items(seq_item a, seq_item b)    { reverse_items(a,b); }

virtual void del(K k)         { SEQ_DEF_IMPL::del(Convert(k)); }
/*{\Mop         removes the item with key $k$ from \var
	        (null operation if no such item exists).}*/

virtual void del_item(seq_item it)  { SEQ_DEF_IMPL::del_item(it); }
/*{\Mopl        removes the item $it$ from \var.\\
	        \precond $it$ is an item in \var.}*/

virtual void change_inf(seq_item it, I i) { SEQ_DEF_IMPL::change_inf(it,Convert(i));}
/*{\Mopl         makes $i$ the information of item $it$.\\
	        \precond $it$ is an item in \var.}*/

virtual void reverse_items(seq_item a, seq_item b) { SEQ_DEF_IMPL::reverse_items(a,b); }
/*{\Mopl       the subsequence of $S$ from $a$ to $b$ is reversed.\\
	       \precond $a$ appears before $b$ in $S$.}*/

virtual void split(seq_item it,sortseq<K,I>& S1, sortseq<K,I>& S2)
{ SEQ_DEF_IMPL::split_at_item(it,(SEQ_DEF_IMPL&)S1,(SEQ_DEF_IMPL&)S2); }
/*{\Mopl      splits $S$ at item $it$ into sequences $S_1$ and $S_2$
	      and makes $S$ empty. More precisely, if
	      $S = x_1,\dots,x_{k-1},it,x_{k+1},\dots,x_n$ then
	      $S_1 = x_1,\dots,x_{k-1},it$ and $S_2 = x_{k+1},\dots,x_n$.\\
	      \precond $it$ is an item in $S$. }*/

virtual sortseq<K,I>& conc(sortseq<K,I>& S1)
{ SEQ_DEF_IMPL::conc((SEQ_DEF_IMPL&)S1); return *this; }
/*{\Mopl      appends $S_1$ to $S$, makes $S_1$ empty and
	     returns $S$.\precond
	     $S$.key($S$.max()) $<$ $S_1$.key($S_1$.min()).}*/

virtual void     clear() { SEQ_DEF_IMPL::clear(); }
/*{\Mop      makes \var\ the empty sorted sequence.}*/

virtual int      size()  const { return SEQ_DEF_IMPL::size(); }
/*{\Mop      returns the size of \var.}*/

virtual bool     empty() const { return (size()==0) ? true : false; }
/*{\Mop      returns true if \var\ is empty, false otherwise.}*/

virtual seq_item first_item() const
{ return SEQ_DEF_IMPL::first_item(); }

virtual seq_item next_item(seq_item it) const
{ return SEQ_DEF_IMPL::next_item(it);}


};


/*{\Mimplementation
Sorted sequences are implemented by (2,4)-trees. Operations lookup, locate,
insert, del, split, conc take time $O(\log n)$, operations succ, pred, max,
min, key, inf, insert\_at and del\_item take time $O(1)$. Clear takes
time $O(n)$ and reverse\_items $O(\ell)$, where $\ell$ is the length of the
reversed subsequence. The space requirement is $O(n)$. Here $n$ is the current
size of the sequence.}*/

/*{\Mexample
We use a sorted sequence to list all elements in a sequence of strings lying
lexicographically between two given search strings. We first read a sequence of strings terminated by "stop" and then a pair of search strings. We output all
strings that lie lexicographically between the two search strings (inclusive).
\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/sortseq.h>

int main()
{
 sortseq<string,int> S;
 string s1,s2;

 while ( cin >> s1 &&  s1 != "stop" )  S.insert(s1,0);

 while ( cin >> s1 >> s2 )
 { seq_item start = S.locate_succ(s1);
   seq_item stop  = S.locate_pred(s2);
   if (S.key(start) <= S.key(stop))
   { for (seq_item it = start; it != stop; it = S.succ(it))
      std::cout << S.key(it) << std::endl;
   }
  }

}
\endgroup

}*/

#endif
