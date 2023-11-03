/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  polygon.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_POLYGON_H
#define LEDA_POLYGON_H

#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/line.h>

//------------------------------------------------------------------------------
// polygons
//------------------------------------------------------------------------------



class polygon_rep : public handle_rep {

friend class polygon;

  list<segment> seg_list;

public:

  polygon_rep() {}
  polygon_rep(const list<segment>& L) : seg_list(L) {}

 ~polygon_rep() {}

};


/*{\Manpage {polygon} {} {Polygons} }*/

class list_polygon_;

class polygon   : public handle_base
{
/*{\Mdefinition
An instance $P$ of the data type $polygon$ is a simple polygon
in the two-dimensional plane defined by the sequence of its vertices
in clockwise order. The number of vertices is called the size
of $P$. A polygon with empty vertex sequence is called empty.}*/

polygon_rep* ptr() const { return (polygon_rep*)PTR; }

polygon(const list<segment>& sl) { PTR = new polygon_rep(sl); }

polygon(const list<point>& pl, bool check);

double compute_area(const list<segment>&) const;

public:

/*{\Mcreation P }*/

polygon(const list<point>& pl);
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the
polygon with vertex sequence $pl$.\\
\precond The vertices in $pl$ are given
in clockwise order and define a simple polygon.}*/

polygon() { PTR = new polygon_rep; }
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the
empty polygon.}*/


 polygon(const polygon& P) : handle_base(P) {}
~polygon() {}
 polygon& operator=(const polygon& P)
 { handle_base::operator=(P); return *this;}


/*{\Moperations 2.5 4.6}*/

list<point>   vertices() const;
/*{\Mop     returns the vertex sequence of \var.}*/

list<segment> segments() const { return ptr()->seg_list; }
/*{\Mop     returns the sequence of bounding segments
	    of \var\ in clockwise order.}*/

list<point> intersection(const segment& s) const;
/*{\Mopl    returns $P \cap s$ as a list of points.}*/

list<point> intersection(const line& l) const;
/*{\Mopl    returns $P \cap l$ as a list of points.}*/


list_polygon_ unite(const polygon& Q) const;
/*
list<polygon> unite(const polygon& Q) const;
*/
/*{\Mopl    returns $P \cup Q$ as a list of polygons. The first polygon
            in the list gives the outer boundary of the contour of the union.
            Possibly following polygons define the inner boundaries (holes)
            of the contour (holes). }*/


list_polygon_ intersection(const polygon& Q) const;
/*
list<polygon> intersection(const polygon& Q) const;
*/
/*{\Mopl    returns $P \cap Q$ as a list of polygons.}*/

bool        inside(const point& p) const;
/*{\Mop     returns true if $p$ lies inside of $P$,
	    false otherwise.}*/

bool        outside(const point& p) const;
/*{\Mop     returns !\var.inside($p$).}*/

double      area() const { return compute_area(ptr()->seg_list); }
/*{\Mop     returns the area of $P$.}*/

polygon     translate(double a, double d) const;
/*{\Mopl    returns the polygon created by a translation
	    of \var\ in direction $a$ by distance $d$.}*/

polygon     translate(const vector& v) const;
/*{\Mop     returns $P+v$, i.e., the polygon created by
	    translating $P$ by vector $v$.\\
	    \precond $v.dim() = 2$.}*/

polygon     rotate(const point& q, double a) const;
/*{\Mopl    returns the polygon created by a rotation of
	    $P$ about point $q$ by angle $a$.}*/

polygon     rotate(double) const;

int         size()   const  { return ptr()->seg_list.size(); }
/*{\Mop     returns the size of \var.}*/

bool        empty()  const  { return ptr()->seg_list.empty(); }
/*{\Mop     returns true if \var\ is empty, false otherwise.}*/

polygon operator+(const vector& v) const { return translate(v); }

bool    operator==(const polygon& P) const { return P.ptr() == ptr(); }

friend std::ostream& operator<<(std::ostream& out, const polygon& p);
friend std::istream& operator>>(std::istream& in,  polygon& p);


};

inline void Print(const polygon& P, std::ostream& out) { out << P; }
inline void Read(polygon& P, std::istream& in)         { in  >> P; }


struct list_polygon_: public list<polygon>
{
  list_polygon_(const list<polygon>& L) : list<polygon>(L) {}
  list_polygon_() {}
 ~list_polygon_() {}
};


#endif
