/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  d_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_D_ARRAY_H
#define LEDA_D_ARRAY_H

//------------------------------------------------------------------------------
// d_array 
//------------------------------------------------------------------------------

#include <LEDA/impl/skiplist.h> 

/*{\Manpage {d_array} {I,E} {Dictionary Arrays}}*/


#define DA_DEF_IMPL skiplist
typedef skiplist_item d_array_item;

template<class I, class E> 

class d_array : public virtual DA_DEF_IMPL 
{
/*{\Mdefinition
An instance $A$ of the parameterized data type \name\ (dictionary
array) is an injective mapping from the linearly ordered data type $I$, called
the index type of $A$, to the set of variables of data type $E$, called the
element type of $A$. We use $A(i)$ to denote the variable with index $i$.}*/

E init;

int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(I,x,y); }
/*
int  hash_fct(GenPtr x)      const { return LEDA_HASH(I,x); }
*/
void clear_key(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(E,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(I,x);  }
void copy_inf(GenPtr& x)     const { LEDA_COPY(E,x);  }
int  int_type()              const { return LEDA_INT_TYPE(I); }

public:

/*{\Mcreation A }*/

d_array()        { }

d_array(E x) { init=x; }
/*{\Mcreate 
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, assigns $x$ to all variables in the range of $a$ and initializes $A$
with $a$.}*/


d_array<I,E>& operator=(const d_array<I,E>& A)
{ DA_DEF_IMPL::operator=(A); init=A.init; return *this; }

d_array(const d_array<I,E>& A) : DA_DEF_IMPL(A) {init=A.init;}

virtual ~d_array() { clear(); }

/*{\Moperations 2 4 }*/

virtual E&  operator[](const I& i) 
{ d_array_item it=DA_DEF_IMPL::lookup(Convert(i));
  if (it==nil) 
  { GenPtr p = Convert(init);
    it=DA_DEF_IMPL::insert(Convert(i),p);
   }
  return LEDA_ACCESS(E,info(it)); 
}
/*{\Marrop    returns the variable $A(i)$.}*/


virtual bool defined(I i)  const 
{ return (DA_DEF_IMPL::lookup(Convert(i))!=nil); }
/*{\Mop      returns true if $i \in dom(A)$, false otherwise; here
             $dom(A)$ is the set of all $i\in I$ for which $A[i]$ has
             already been executed.}*/


virtual void undefine(I i)
{ DA_DEF_IMPL::del(Convert(i)); }
/*{\Mop      removes $i$ from $dom(A)$. } */


// iteration

virtual void loop_to_succ(GenPtr& x) const 
   { x = DA_DEF_IMPL::next_item(d_array_item(x)); }


virtual GenPtr forall_defined_test(GenPtr it, I& x) const
{ if (it) x = LEDA_ACCESS(I,key(d_array_item(it)));
  return it;
}

virtual GenPtr forall_loop_test(GenPtr it, E& x) const
{ if (it) x = LEDA_ACCESS(E,inf(d_array_item(it)));
  return it;
}

#if defined(__ELSE_SCOPE_BUG__)
GenPtr forall_loop_item;
GenPtr& Forall_Loop_Item() const { return (GenPtr&)forall_loop_item; }
#endif

};

/*{\Mtext     
\bigskip
{\bf Iteration} }*/


/*{\Mtext
{\bf forall\_defined}($i,A$) 
$\{$ ``the elements from $dom(A)$ are successively assigned to $i$'' $\}$ }*/

/*{\Mtext
{\bf forall}($x,A$) 
$\{$ ``for all $i \in dom(A)$ the entries $A[i]$ are successively assigned to $x$'' $\}$ }*/



/*{\Mimplementation
Dictionary arrays are implemented by randomized search trees \cite{AS89}. 
Access operations $A[i]$ take time $O(\log dom(A))$.
The space requirement is $O(dom(A))$.}*/
  
/*{\Mexample

{\bf Program 1}:
We use a dictionary array to count the number of occurrences of the elements in a 
sequence of strings.


\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/d_array.h>

main()
{ 
  d_array<string,int> N(0);
  string s;

  while (cin >> s) N[s]++;

  forall_defined(s,N) cout << s << "  " << N[s] << endl;

}
\endgroup


\bigskip
{\bf Program 2}:
We use a $d\_array\<string,string\>$ to realize an english/german dictionary.
\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

#include <LEDA/d_array.h>

main()
{ 
  d_array<string,string> dic;

  dic["hello"] = "hallo";
  dic["world"] = "Welt";
  dic["book"]  = "Buch";
  dic["key"]   = "Schluessel";
  
  string s;
  forall_defined(s,dic) cout << s << "  " << dic[s] << endl;

}
\endgroup


}*/

#endif
