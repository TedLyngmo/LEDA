/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  interval_set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_INTERVAL_SET_H
#define LEDA_INTERVAL_SET_H

#include <LEDA/d2_dictionary.h>

typedef dic2_item is_item;





class Interval_Set : public d2_dictionary<double,double,GenPtr> {

public:

double    left(is_item it) { return key1(it); }
double    right(is_item it){ return key2(it); }

list<is_item>  intersection(double x, double y)
{ list<dic2_item> l = range_search(-MAXFLOAT,y,x,MAXFLOAT);
  return *((list<is_item>*)&l);
}

 Interval_Set()  {}
~Interval_Set()  {}
};


/*{\Manpage {interval_set} {I} {Sets of Intervals}}*/

template <class I>

class interval_set : private Interval_Set {

/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is a
collection of items ($is\_item$). Every item in $S$ contains a closed
interval of the $double$ numbers as key and an information from data type $I$,
called the information type of $S$. The number of items in $S$ is called
the size of $S$. An interval set of size zero is said to be empty. We use
$\<x,y,i\>$ to denote the item with interval $[x,y]$ and information $i$;
$x$ ($y$) is called the left (right) boundary of the item. For each
interval $[x,y]$ there is at most one item $\<x,y,i\> \in S$.}*/

public:

/*{\Mcreation S }*/

interval_set()  {}
/*{\Mcreate creates an instance \var\ of type \name\ and initializes \var\ 
            to the empty set.}*/

~interval_set()  {}


/*{\Moperations 2.3 5 }*/

double left(is_item it) {return Interval_Set::left(it);}
/*{\Mop      returns the left boundary of item $it$.\\
	     \precond $it$ is an item in \var.}*/

double right(is_item it) {return Interval_Set::right(it);}
/*{\Mop      returns the right boundary of item $it$.\\
	     \precond $it$ is an item in \var.}*/

I   inf(is_item it)  { return LEDA_ACCESS(I,Interval_Set::inf(it)); }
/*{\Mop      returns the information of item $it$.\\
	     \precond $it$ is an item in \var.}*/

is_item insert(double x, double y, I i)
                      { return Interval_Set::insert(x,y,Copy(i)); }
/*{\Mopl     associates the information $i$ with interval 
	     $[x,y]$. If there is an item $\<x,y,j\>$ in \var
	     then $j$ is replaced by $i$, else a new item
	     $\<x,y,i\>$ is added to $S$. In both cases
	     the item is returned.}*/

is_item lookup(double x, double y) {return Interval_Set::lookup(x,y);}
/*{\Mop      returns the item with interval $[x,y]$ 
	     (nil if no such item exists in \var).}*/

list<is_item>  intersection(double a, double b)
                         { return Interval_Set::intersection(a,b); }
/*{\Mopl     returns all items $\<x,y,i\>\ \in\ S$ with 
	     $[x,y] \cap [a,b] \neq \emptyset$.}*/

void del(double x, double y) {Interval_Set::del(x,y);}
/*{\Mop      deletes the item with interval $[x,y]$ from \var.}*/

void del_item(is_item it) {Interval_Set::del_item(it);}
/*{\Mop      removes item $it$ from \var.\\
	     \precond $it$ is an item in \var.}*/

void    change_inf(is_item it, I i)
                      { Interval_Set::change_inf(it,Copy(i)); }
/*{\Mop      makes $i$ the information of item $it$.\\
	     \precond $it$ is an item in \var.}*/

void clear() {Interval_Set::clear();}
/*{\Mop      makes \var\ the empty interval\_set.}*/

bool empty() {return Interval_Set::empty();}
/*{\Mop      returns true iff \var\ is empty.}*/

int size() {return Interval_Set::size();}
/*{\Mop      returns the size of \var.}*/

};

#define forall_is_items(i,D) forall_dic2_items(i,D)

/*{\Mimplementation
Interval sets are implemented by two-dimensional range trees \cite{Wi85,Lu78}.
Operations insert, lookup, del\_item and del take time $O(\log^2 n)$,
intersection takes time $O(k + \log^2 n)$, where $k$ is the size
of the returned list. Operations left, right, inf, empty, and size
take time $O(1)$, and clear $O(n\log n)$. Here $n$ is always the 
current size of the interval set. The space requirement is $O(n\log n)$.}*/

#endif

