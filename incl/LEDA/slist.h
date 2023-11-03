/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  slist.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_SLIST_H
#define LEDA_SLIST_H

/*{\Manpage {slist} {E} {Singly Linked Lists}}*/

#include <LEDA/impl/slist.h>

template <class E>

class slist : public SLIST {


/*{\Mdefinition
An instance $L$ of the parameterized data type \name\ is a sequence of items
($slist\_item$). Each item in $L$ contains an element of data type $E$, called
the element type of $L$. The number of items in $L$ is called the length of $L$. If $L$ has length zero it is called the empty list. In the sequel $\<x\>$ is
used to denote a list item containing the element $x$ and $L[i]$ is used to
denote the contents of list item $i$ in $L$.}*/



void print_el(GenPtr& x,std::ostream& out) const { LEDA_PRINT(E,x,out);  }
void read_el(GenPtr& x,std::istream& in)   const { E X; Read(X,in); x = Copy(X); }
void clear_el(GenPtr& x)              const { LEDA_CLEAR(E,x); }
void copy_el(GenPtr& x)               const { LEDA_COPY(E,x); }

int  int_type() const { return LEDA_INT_TYPE(E); }

public:


/*{\Mcreation L }*/

slist() {}
/*{\Mcreate creates  an instance \var\ of type \name\ and initializes it to
            the empty list.}*/

slist(E x) : SLIST(Convert(x)) { }
/*{\Mcreate creates  an instance \var\ of type \name\ and initializes it to
            the one-element list $\<x\>$. }*/

 slist(const slist<E>& a) : SLIST((SLIST&)a) { }
~slist() {}
 slist<E>& operator=(const slist<E>& a) { SLIST::operator=(a);  return *this; }



/*{\Moperations 2 5 }*/

int length() const {return SLIST::length();}
/*{\Mop      returns the length of $L$.}*/

int size() const {return SLIST::length();}
/*{\Mop      returns $L$.length().}*/

bool empty() const {return SLIST::empty();}
/*{\Mop      returns true if $L$ is empty, false otherwise.}*/

slist_item first() const {return SLIST::first();}
/*{\Mop      returns the first item of $L$.}*/

slist_item last() const {return SLIST::last();}
/*{\Mop      returns the last item of $L$.}*/

slist_item succ(slist_item it) const {return SLIST::succ(it);}
/*{\Mop      returns the successor item of item $it$, nil if
	     $it=L$.last().\\
	     \precond $it$ is an item in $L$.}*/

slist_item cyclic_succ(slist_item it) const {return SLIST::cyclic_succ(it);}
/*{\Mop      returns the cyclic successor of item $it$, i.e.,
	     $L$.first() if $it = L$.last(), $L$.succ($it$) otherwise.}*/

E  contents(slist_item it) const { return LEDA_ACCESS(E,SLIST::contents(it)); }
/*{\Mop      returns the contents $L[it]$ of item $it$.\\
	     \precond $it$ is an item in $L$.}*/

E  inf(slist_item it) const { return contents(it); }
/*{\Mop      returns $L$.contents($it$).\\
             \precond $it$ is an item in $L$.}*/


E  head() const { return LEDA_ACCESS(E,SLIST::head()); }
/*{\Mop      returns the first element of $L$, i.e. the contents
	     of $L$.first().\\
	     \precond $L$ is not empty.}*/

E  tail() const { return LEDA_ACCESS(E,SLIST::tail()); }
/*{\Mop      returns the last element of $L$, i.e. the contents
	     of $L$.last().\\
	     \precond $L$ is not empty.}*/


slist_item push(E x)   { return SLIST::push(Copy(x));}
/*{\Mop      adds a new item $\<x\>$ at the front of $L$ and returns it. }*/

slist_item append(E x) { return SLIST::append(Copy(x));}
/*{\Mop      appends a new item $\<x\>$ to $L$ and returns it. }*/

slist_item insert(E x, slist_item it) { return SLIST::insert(Copy(x),it); }
/*{\Mopl     inserts a new item $\<x\>$ after item $it$ into $L$ and
             returns it.\\
	     \precond $it$ is an item in $L$.}*/


E pop() { GenPtr x=SLIST::pop();
          E   y=LEDA_ACCESS(E,x);
          Clear(LEDA_ACCESS(E,x));
          return y; }
/*{\Mop      deletes the first item from $L$ and returns its
             contents. \\
	     \precond $L$ is not empty.}*/


void conc(slist<E>& L1)      { SLIST::conc((SLIST&)L1); }
/*{\Mop      appends list $L_1$ to list $L$ and makes $L_1$ the empty list.\\
             \precond $L\ !=\ L_1$.}*/


E   operator[](slist_item it) const { return contents(it); }

E&  operator[](slist_item it) { return LEDA_ACCESS(E,SLIST::entry(it)); }
/*{\Marrop   returns a reference to the contents of $it$.}*/

slist_item operator+=(E x)  { return append(x); }
/*{\Mbinop      appends a new item $\<x\>$ to $L$ and returns it. }*/


bool current_element(E& x) const {GenPtr y; bool b=SLIST::current_element(y);
                                     if (b) x = LEDA_ACCESS(E,y); return b; }

bool next_element(E& x) const { GenPtr y; bool b = SLIST::next_element(y);
                                   if (b) x = LEDA_ACCESS(E,y); return b; }

GenPtr forall_loop_test(GenPtr it, E& x) const
{ if (it) x = contents(slist_item(it));
  return it;
 }

};


#endif
