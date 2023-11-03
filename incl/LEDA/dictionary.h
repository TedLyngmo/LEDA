/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  dictionary.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_DICTIONARY_H
#define LEDA_DICTIONARY_H

#include <LEDA/basic.h>


#define DIC_DEF_IMPL skiplist
#include <LEDA/impl/skiplist.h>
typedef skiplist_item dic_item;

/*{\Manpage {dictionary} {K,I} {Dictionaries}}*/

template <class K, class I>

class dictionary : public virtual DIC_DEF_IMPL
{
/*{\Mdefinition
An instance $D$ of the parameterized data type \name\ is a collection
of items ($dic\_item$). Every item in $D$ contains a key from the linearly
ordered data type $K$, called the key type of $D$, and an information from the
data type $I$, called the information type  of $D$. The number of items in $D$
is called the size of $D$. A dictionary of size zero is called the empty
dictionary. We use $\<k,i\>$ to denote an item with key $k$ and information
$i$ ($i$ is said to be the information associated with key $k$).  For each
$k \in K$ there is at most one $i \in I$ with $\<k,i\> \in D$.}*/


int  int_type()              const { return LEDA_INT_TYPE(K); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(K,x,y); }
int  hash_fct(GenPtr x)      const { return LEDA_HASH(K,x); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }

public:

/*{\Mcreation D }*/

dictionary() {}
/*{\Mcreate creates an instance \var\ of type \name\ and initializes it with
            the empty dictionary.}*/


dictionary(const dictionary<K,I>& D) : DIC_DEF_IMPL(D) {}

dictionary<K,I>& operator=(const dictionary<K,I>& D)
{ DIC_DEF_IMPL::operator=(D); return *this; }

virtual ~dictionary()   { DIC_DEF_IMPL::clear(); }


/*{\Moperations 2 4.2 }*/

virtual K key(dic_item it) const { return LEDA_ACCESS(K,DIC_DEF_IMPL::key(it));}

/*{\Mop   returns the key of item $it$.\\
	  \precond $it$ is an item in \var.}*/

virtual I inf(dic_item it) const { return LEDA_ACCESS(I,DIC_DEF_IMPL::inf(it));}
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in \var.}*/

virtual int  defined(K k) const
{ return (lookup(k) == nil) ? false : true; }

virtual dic_item insert(K k, I i)
{ return DIC_DEF_IMPL::insert(Convert(k),Convert(i)); }
/*{\Mop      associates the information $i$ with the key $k$.
             If there is an item $\<k,j\>$ in \var\ then $j$ is
             replaced by $i$, else a new item $\<k,i\>$ is added
       	     to \var. In both cases the item is returned.}*/

virtual dic_item lookup(K k) const
{ return DIC_DEF_IMPL::lookup(Convert(k));}
/*{\Mop        returns the item with key $k$ (nil if no such
	       item exists in \var).}*/

virtual I access(K k)  const { return inf(lookup(k));}
/*{\Mop       returns the information associated with key $k$.
              \precond there is an item with key $k$ in \var.}*/

virtual void  del(K k)          { DIC_DEF_IMPL::del(Convert(k)); }
/*{\Mop       deletes the item with key $k$ from \var\
              (null operation, if no such item exists).}*/

virtual void  del_item(dic_item it) { DIC_DEF_IMPL::del_item(it); }
/*{\Mop       removes item $it$ from \var.\\
              \precond $it$ is an item in \var.}*/

virtual void change_inf(dic_item it, I i)
{ DIC_DEF_IMPL::change_inf(it,Convert(i)); }
/*{\Mopl      makes $i$ the information of item $it$.\\
              \precond $it$ is an item in \var.}*/

virtual void     clear() { DIC_DEF_IMPL::clear(); }
/*{\Mop       makes \var\ the empty dictionary.}*/

virtual int      size()  const { return DIC_DEF_IMPL::size(); }
/*{\Mop       returns the size of \var.}*/

virtual bool     empty() const { return (size()==0) ? true : false; }
/*{\Mop       returns true if \var\ is empty, false otherwise.}*/

virtual dic_item first_item() const { return DIC_DEF_IMPL::first_item(); }
virtual dic_item next_item(dic_item it) const { return DIC_DEF_IMPL::next_item(it);}

};

/*{\Mimplementation
Dictionaries are implemented by randomized search trees \cite{AS89}. Operations
insert, lookup, del\_item, del take time $O(\log n)$, key, inf, empty, size,
change\_inf take time $O(1)$, and clear takes time $O(n)$. Here $n$ is the
current size of the dictionary. The space requirement is $O(n)$.}*/


/*{\Mexample
We count the number of occurrences of each string in a sequence of strings.

\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/dictionary.h>

int main()
{ dictionary<string,int> D;
  string s;
  dic_item it;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
  }

  forall_items(it,D) std::cout << D.key(it) << " : " <<  D.inf(it) << std::endl;

}
\endgroup

}*/


#endif
