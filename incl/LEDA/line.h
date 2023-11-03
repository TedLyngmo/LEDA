/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  line.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_LINE_H
#define LEDA_LINE_H

#include <LEDA/point.h>
#include <LEDA/segment.h>

//------------------------------------------------------------------------------
// straight lines
//------------------------------------------------------------------------------


class line_rep : public handle_rep {

friend class line;

  segment  seg;

public:

  line_rep() {}
  line_rep(const segment& s)  { seg = s; }

 ~line_rep() {}

friend inline int cmp_slopes(const line&, const line&);
friend inline int orientation(const line&, const point&);

};

/*{\Manpage {line} {} {Straight Lines}}*/

class line   : public handle_base
{
/*{\Mdefinition
An instance $l$ of the data type $line$ is a directed straight line
in the two-dimensional plane. The angle between a right oriented horizontal
line and $l$ is called the direction of $l$.}*/

line_rep* ptr() const { return (line_rep*)PTR; }

public:

/*{\Mcreation l }*/

 line(const point& p, const point& q);
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the line
passing through points $p$ and $q$ directed form $p$ to $q$.}*/


 line(const segment& s);
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the line
supporting segment $s$.}*/

 line(const point& p, const vector& v);
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the line
of all poinnts $p + \lambda v$. \precond $v.dim() = 2$ and $v.length() > 0$. }*/


 line(const point& p, double a);
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the line
passing through point $p$ with direction $a$.}*/

 line();
/*{\Mcreate
introduces a variable \var\ of type \name. \var\ is initialized to the line
passing through the origin with direction 0.}*/


 line(const line& l) : handle_base(l) {};
 line& operator=(const line& l) { handle_base::operator=(l); return *this; }
~line() {}



/*{\Moperations 2 4.5 }*/

double direction() const { return angle(); }
/*{\Mop     returns the direction of \var.}*/

double angle(const line& g) const { return ptr()->seg.angle(g.ptr()->seg); }
/*{\Mop     returns the angle between \var\ and $g$, i.e.,
	    $g$.direction() $-$ \var.direction().}*/

double angle() const     { return ptr()->seg.angle();     }
/*{\Mop     returns \var.direction().}*/

bool vertical() const    { return ptr()->seg.vertical();  }
/*{\Mop     returns true iff \var\ is vertical.}*/

bool horizontal() const  { return ptr()->seg.horizontal();}
/*{\Mop     returns true iff \var\ is horizontal.}*/

double distance() const  { return ptr()->seg.distance();  }
double distance(point p) const { return ptr()->seg.distance(p); }

double slope() const     { return ptr()->seg.slope();     }
/*{\Mop     returns the slope of \var.\\
	    \precond  \var\  is not vertical.}*/

segment seg()  const     { return ptr()->seg; }

double y_proj(double x) const  { return ptr()->seg.y_proj(x); };
/*{\Mop     returns $p$.ycoord(), where $p \in l$ with $p$.xcoord()
	    = $x$.\\ \precond \var\ is not vertical.}*/

double x_proj(double y) const  { return ptr()->seg.x_proj(y); };
/*{\Mop     returns $p$.xcoord(), where $p \in l$ with $p$.ycoord()
	    = $y$.\\ \precond \var\ is not horizontal.}*/

double y_abs() const { return ptr()->seg.y_proj(0); }
/*{\Mop     returns the y-abscissa of \var\ (\var.y\_proj(0)).\\
	    \precond  \var\  is not vertical.}*/

bool intersection(const line& g, point& inter) const;
/*{\Mopl    if $l$ and $g$ are not collinear and intersect the
            intersection point is assigned to $inter$ and true is
            returned, otherwise false is returned.}*/

bool intersection(const segment& s, point& inter) const;
/*{\Mopl    if $l$ and $s$ are not collinear and intersect the
	    intersection point is assigned to $inter$ and true is
	    returned, otherwise false is returned.}*/

line translate(double a, double d) const
{ return ptr()->seg.translate(a,d); }
/*{\Mopl     returns the line created by a translation of
	    \var\ in direction $a$ by distance $d$.}*/

line translate(const vector& v)  const
{ return ptr()->seg.translate(v); }
/*{\Mop     returns $l+v$, i.e., the line created by
            translating $l$ by vector $v$.\\
	    \precond $v$.dim() = 2.}*/

line rotate(const point& q, double a) const
{ return ptr()->seg.rotate(q,a); }
/*{\Mopl     returns the line created by a rotation of $l$
	    about point $q$ by angle $a$.}*/

line rotate(double a) const
{ return rotate(point(0,0),a);}
/*{\Mop     returns $l$.rotate($point(0,0),\ a$). }*/

segment perpendicular(const point& p) const;
/*{\Mop     returns the normal of $p$ with respect to \var.}*/


bool contains(const point&) const;
bool contains(const segment&) const;

line operator+(const vector& v) const { return translate(v); }

bool operator==(const line& g) const { return contains(g.ptr()->seg); }
/*{\Mbinop      Test for equality.}*/

bool operator!=(const line& g) const { return !contains(g.ptr()->seg); }
/*{\Mbinop      Test for inequality.}*/

friend int orientation(const line& l, const point& p);
/*{\Mfunc      computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on line $l$. }*/

friend int cmp_slopes(const line& l1, const line& l2);
/*{\Mfunc      returns compare(slope($l_1$), slope($l_2$)).}*/


friend std::ostream& operator<<(std::ostream& out, const line& l);
friend std::istream& operator>>(std::istream& in, line& l);

};

inline  int orientation(const line& l, const point& p)
{ return orientation(l.ptr()->seg,p); }

inline  int cmp_slopes(const line& l1, const line& l2)
{ return cmp_slopes(l1.ptr()->seg,l2.ptr()->seg); }

inline bool parallel(const line& l1, const line& l2)
{ return cmp_slopes(l1,l2) == 0; }


inline void Print(const line& l, std::ostream& out) { out << l; }
inline void Read(line& l, std::istream& in)         { in >> l; }

extern line p_bisector(const point& p, const point& q);


#endif
