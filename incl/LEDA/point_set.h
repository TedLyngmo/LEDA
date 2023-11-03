/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  point_set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_POINT_SET_H
#define LEDA_POINT_SET_H

#include <LEDA/point.h>
#include <LEDA/impl/delaunay_tree.h>

typedef DT_item       ps_item;


class Point_Set : public delaunay_tree {

void* ptr;  // d2_dictionary(double,double,DT_item)*

public:

 Point_Set();
~Point_Set();

ps_item       lookup(point);

list<ps_item> range_search(double, double, double, double);

list<point>  all_points();

ps_item      insert(point p, void* i);

ps_item      nearest_neighbor(point p){ return delaunay_tree::neighbor(p); }

void         change_inf(ps_item it, void* i) { delaunay_tree::change_inf(it,i);}

void         del(point);

void         del_item(ps_item it) { del(key(it)); }

list<ps_item> all_items();
list<ps_item> convex_hull();

void          clear();
int           size();
bool          empty()   { return (size()==0) ? true:false; }

};




/*{\Manpage {point_set} {I} {Sets of Two-Dimensional Points}}*/

template<class I>

class point_set : public Point_Set {

/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is a collection
of items ($ps\_item$). Every item in $S$ contains a two-dimensional point as
key (data type $point$), and an information from data type $I$, called the
information type of $S$. The number of items in $S$ is called the size of $S$.
A point set of size zero is said to be empty. We use $\<p,i\>$ to denote the
item with point $p$, and information $i$. For each  point $p$ there is at most
one item $\<p,i\> \in S$. Beside the normal dictionary operations, the data
type $point\_set$ provides operations for rectangular range queries and
nearest neighbor queries.}*/

void clear_inf(GenPtr& x) { LEDA_CLEAR(I,x); }
void copy_inf(GenPtr& x)  { LEDA_COPY(I,x);  }

public:

/*{\Mcreation S }*/

 point_set()   {}
/*{\Mcreate creates an instance \var\ of type \name\ and initializes \var\ to
            the empty set.}*/

~point_set()   { clear(); }

/*{\Moperations 2.5 5}*/

point key(ps_item it) {return Point_Set::key(it);}
/*{\Mop     returns the point of item $it$.\\
            \precond $it$ is an item in \var.}*/

I   inf(ps_item it)          { return LEDA_ACCESS(I,Point_Set::inf(it)); }
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in \var.}*/

ps_item insert(point p, I i) { return Point_Set::insert(p,Convert(i));}
/*{\Mop    associates the information $i$ with point $p$.
	   If there is an item $\<p,j\>$ in \var\ then $j$
	   is replaced by $i$, else a new item $\<p,i\>$
	   is added to $S$. In both cases the item is
	   returned.}*/

ps_item lookup(point p) {return Point_Set::lookup(p);}
/*{\Mop    returns the item with point $p$ (nil if no
	   such item exists in \var).}*/

ps_item nearest_neighbor(point q) {return Point_Set::nearest_neighbor(q);}
/*{\Mop    returns the item $\<p,i\>\ \in\ S$ such that
	   the distance between $p$ and $q$ is minimal.}*/

list<ps_item> range_search(double x0, double x1, double y0, double y1)
{ return Point_Set::range_search(x0,x1,y0,y1);}
/*{\Mopl   returns all items $\<p,i\>\ \in\ S$ with\\
	   $x_0 \le p$.xcoord() $\le x_1$ and\\
	   $y_0 \le p$.ycoord() $\le y_1$.}*/

list<ps_item> convex_hull() {return Point_Set::convex_hull();}
/*{\Mop    returns the list of items containing all points
	   of the convex hull of \var\ in clockwise order.}*/

void del(point p) {Point_Set::del(p);}
/*{\Mop    deletes the item with point $p$ from \var.}*/

void del_item(ps_item it) {Point_Set::del_item(it);}
/*{\Mop    removes item $it$ from \var.\\
           \precond $it$ is an item in \var.}*/

void    change_inf(ps_item it, I i) { Point_Set::change_inf(it,Convert(i));}
/*{\Mop    makes $i$ the information of item $it$.\\
	   \precond $it$ is an item in \var.}*/

list<ps_item> all_items() {return Point_Set::all_items();}
/*{\Mop    returns the list of all items in $S$.}*/

list<point> all_points() {return Point_Set::all_points();}
/*{\Mop    returns the list of all points in $S$.}*/

void clear() {Point_Set::clear();}
/*{\Mop    makes \var\ the empty point\_set.}*/

bool empty() {return Point_Set::empty();}
/*{\Mop    returns true iff \var\ is empty.}*/

int size() {return Point_Set::size();}
/*{\Mop    returns the size of \var.}*/

};

#define forall_ps_items(i,D) forall(i, (D.all_items()) )

/*{\Mimplementation
Point sets are implemented by a combination of two-dimensional range trees
\cite{Wi85,Lu78} and Voronoi diagrams.  Operations insert, lookup, del\_item,
del take time $O(\log^2 n)$, key, inf, empty, size, change\_inf take time
$O(1)$, and clear takes time $O(n\log n)$. A range\_search operation takes time
$O(k+\log^2 n)$, where $k$ is the size of the returned list. A nearest\_neighbor
query takes time $O(n^2)$, if it follows any update operation (insert or
delete) and $O(\log n)$ otherwise. Here $n$ is the current size of the
point set. The space requirement is $O(n^2)$.}*/

#endif

