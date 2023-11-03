/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  segment.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_SEGMENT_H
#define LEDA_SEGMENT_H

#include <LEDA/point.h>

//------------------------------------------------------------------------------
// segments
//------------------------------------------------------------------------------


class segment_rep : public handle_rep {

friend class segment;
friend class line;
friend class circle;

static unsigned long id_counter;

   point start;
   point end;
   double dx;
   double dy;

   double slope;
   double y_abs;
   double angle;

   unsigned long id;

public:

   segment_rep(const point&, const point&);
   segment_rep();

  ~segment_rep() {}

};

/*{\Manpage {segment} {} {Segments}}*/

class segment  : public handle_base
{
/*{\Mdefinition
    An instance $s$ of the data type $segment$ is a directed straight line
    segment in the two-dimensional plane, i.e., a straight line segment $[p,q]$
    connecting two points $p,q \in \real^2$. $p$ is called the start point
    and $q$ is called the end point of $s$. The length of $s$ is the Euclidean
    distance between $p$ and $q$. The angle between a right oriented horizontal
    ray and $s$ is called the direction of $s$. The segment $[(0,0),(0,0)]$
    is said to be empty.}*/

friend class line;
friend class circle;

segment_rep* ptr() const { return (segment_rep*)PTR; }

public:

/*{\Mcreation s }*/

segment(const point& p, const point& q);
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the segment $(p,q)$ }*/

segment(const point& p, const vector& v);
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the segment $(p,p+v)$. \precond $v.dim() = 2$. }*/

segment(double x1, double y1, double x2, double y2) ;
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the segment $[(x_1,y_1),(x_2,y_2)]$.}*/

segment(const point& p, double dir, double length);
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the segment with start point $p$, direction $dir$, and
            length $length$.}*/

segment();
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the empty segment.}*/


 segment(const segment& s) : handle_base(s) {}
~segment() {}
 segment& operator=(const segment& s)
 { handle_base::operator=(s); return *this;}


/*{\Moperations 2 3.5}*/

operator vector()  { return vector(xcoord2()-xcoord1(), ycoord2()-ycoord1()); }

point start()  const      { return ptr()->start; }
point source() const      { return ptr()->start; }
/*{\Mop       returns the source point of segment \var.}*/

point end()    const      { return ptr()->end; }
point target() const      { return ptr()->end; }
/*{\Mop       returns the target point of segment \var.}*/

double xcoord1() const    { return ptr()->start.ptr()->x; }
/*{\Mop       returns the x-coordinate of \var.start().}*/

double xcoord2() const    { return ptr()->end.ptr()->x;   }
/*{\Mop       returns the x-coordinate of \var.end().}*/

double ycoord1() const    { return ptr()->start.ptr()->y; }
/*{\Mop       returns the y-coordinate of \var.start().}*/

double ycoord2() const    { return ptr()->end.ptr()->y;   }
/*{\Mop       returns the y-coordinate of \var.end().}*/

double dx() const    { return ptr()->dx;   }
/*{\Mop       returns the $xcoord2 - xcoord1$.}*/

double dy() const    { return ptr()->dy;   }
/*{\Mop       returns the $ycoord2 - ycoord1$.}*/

double length() const { return start().distance(end()); }
/*{\Mop       returns the length of \var.}*/

double direction() const { return angle(); }
/*{\Mop       returns the direction of \var\ as an angle in
	      the intervall $(-\pi,\pi]$.}*/

double angle()     const { return ptr()->angle; }
/*{\Mop       returns \var.direction().}*/

double  angle(const segment& t) const;
/*{\Mop       returns the angle between \var\ and $t$, i.e.,
              $t$.direction() - \var.direction().}*/

bool vertical()   const { return xcoord1() == xcoord2(); }
/*{\Mop       returns true iff \var\ is vertical.}*/

bool horizontal() const { return ycoord1() == ycoord2(); }
/*{\Mop       returns true iff \var\ is horizontal.}*/

double slope() const { return ptr()->slope; }
/*{\Mop       returns the slope of $s$.\\
	      \precond  \var\  is not vertical.}*/

double y_abs() const { return ptr()->y_abs; }

bool intersection(const segment& t, point& p) const;
/*{\Mopl    if \var\ and $t$ are not collinear and intersect the
	    intersection point is assigned to $p$ and true is
	    returned, otherwise false is returned.}*/

bool intersection_of_lines(const segment& t, point& p) const;
/*{\Mopl    if \var\ and $t$ are not collinear and the underlying
	    lines intersect the point of intersection is assigned
            to $p$ and true is returned, otherwise false is returned.}*/


segment translate(double a, double d) const;
/*{\Mopl    returns the segment created by a translation of
	    $s$ in direction $a$ by distance $d$.}*/

segment translate(const vector& v) const;
/*{\Mop     returns $s+v$, i.e., the segment created by
	    translating $s$ by vector $v$.\\
	    \precond $v$.dim() = 2.}*/

double  distance(const segment&) const;
double  distance(const point&) const;
double  distance() const;
double  x_proj(double) const;
double  y_proj(double) const;

double operator()(double x) { return y_proj(x); }

segment rotate(const point& q, double a) const;
/*{\Mopl    returns the segment created by a rotation of $s$
	    about point $q$ by angle $a$.}*/

segment rotate(double a) const;
/*{\Mop     returns $s$.rotate($s$.start(), $a$).}*/

segment rotate90(const point& q) const;
/*{\Mopl    returns the segment created by a rotation of $s$
	    about point $q$ by an angle of 90 degrees.}*/

segment rotate90() const;
/*{\Mop     returns $s$.rotate90($s$.start(), $a$).}*/

bool  right()  const     { return ptr()->start.ptr()->x < ptr()->end.ptr()->x; }
bool  left()   const     { return ptr()->start.ptr()->x > ptr()->end.ptr()->x; }
bool  up()     const     { return ptr()->start.ptr()->y < ptr()->end.ptr()->y; }
bool  down()   const     { return ptr()->start.ptr()->y > ptr()->end.ptr()->y; }


int operator==(const segment& t) const
{ return (ptr()->start == t.ptr()->start && ptr()->end == t.ptr()->end); }
/*{\Mbinop       Test for equality.}*/

int operator!=(const segment& t) const { return !operator==(t);}
/*{\Mbinop       Test for inequality.}*/

segment operator+(const vector& v) const { return translate(v); }
/*{\Mbinop       Translation by vector $v$.}*/


friend std::ostream& operator<<(std::ostream& O, const segment& s);
/*{\Mbinopfunc     writes \var\ to output stream $O$.}*/

friend std::istream& operator>>(std::istream& I, segment& s);
/*{\Mbinopfunc     reads the coordinates of \var\ (four $double$ numbers)
	           from input stream $I$.}*/

friend inline bool identical(const segment& s1, const segment& s2);

};

inline void Print(const segment& s, std::ostream& out) { out << s; }
inline void Read(segment& s,  std::istream& in)        { in >> s; }



/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/


inline bool identical(const segment& s1, const segment& s2)
{ return s1.ptr() == s2.ptr(); }
/*{\Mfuncl Test for identity.}*/


inline int orientation(const segment& s, const point& p)
/*{\Mfuncl      computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on segment $s$. }*/
{ return compare(s.dy()*(s.xcoord1()-p.xcoord()),
                 s.dx()*(s.ycoord1()-p.ycoord()));
 }


inline int cmp_slopes(const segment& s1, const segment& s2)
/*{\Mfuncl      returns compare(slope($s_1$), slope($s_2$)).}*/
{ return compare(s1.slope(), s2.slope()); }

inline bool parallel(const segment& s1, const segment& s2)
/*{\Mfuncl      returns (cmp\_slopes($s_1$, $s_2$) == 0).}*/
{ return cmp_slopes(s1,s2) == 0; }


#endif

