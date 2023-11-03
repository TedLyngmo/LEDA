/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  point.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_POINT_H
#define LEDA_POINT_H

#include <LEDA/list.h>
#include <LEDA/vector.h>

class point;
class segment;

//------------------------------------------------------------------------------
// points
//------------------------------------------------------------------------------

class point_rep  : public handle_rep {

friend class point;
friend class segment;
friend class line;
friend class circle;

   double x;
   double y;

public:

   point_rep();
   point_rep(double a, double b);

  ~point_rep() {}

};

/*{\Manpage {point} {} {Points}}*/

class point  : public handle_base
{
/*{\Mdefinition
An instance of the data type $point$ is a point in the two-dimensional
plane $\real^2$. We use $(a,b)$ to denote a point with first (or x-)
coordinate $a$ and second (or y-) coordinate $b$.}*/

friend class segment;
friend class line;
friend class circle;


point_rep* ptr() const { return (point_rep*)PTR; }

public:

/*{\Mcreation p }*/

 point();
/*{\Mcreate introduces a variable \var\ of type \name\ initialized
            to the point $(0,0)$.}*/

 point(double x, double y);
/*{\Mcreate introduces a variable \var\ of type \name\ initialized to the point
    $(x,y)$.}*/

 point(vector v);
/*{\Mcreate introduces a variable \var\ of type \name\ initialized to the point
    defined by vector $v$.}*/

 point(const point& p) : handle_base(p) {}
~point() {}

point& operator=(const point& p) { handle_base::operator=(p); return *this; }


/*{\Moperations 2 4}*/

operator vector()         { return vector(ptr()->x,ptr()->y); }

double  xcoord()  const   { return ptr()->x; }
/*{\Mop     returns the first coordinate of \var.}*/

double  ycoord()  const   { return ptr()->y; }
/*{\Mop     returns the second coordinate of \var.}*/

double  angle(const point&, const point&) const;

double  distance(const point& q) const;
/*{\Mop     returns the Euclidean distance between \var\ and $q$.}*/

double  distance() const;
/*{\Mop     returns the Euclidean distance between \var\ and $(0,0)$.}*/

point   translate(double a, double d) const;
/*{\Mopl    returns the point created by translating
	    \var\ in direction $a$ by distance $d$. The
	    direction is given by its angle with a
	    right oriented horizontal ray.}*/

point   translate(const vector& v) const;
/*{\Mop     returns \var$+v$, i.e., \var\ translated by vector
	    $v$.\\
	    \precond $v$.dim() = 2.}*/

point   rotate(const point& q,double a) const;
/*{\Mopl    returns the point created by a rotation of \var\
	    about point $q$ by angle $a$.}*/

point   rotate90(const point& q) const;
/*{\Mopl    returns the point created by a rotation of \var\
	    about point $q$ by an angle of 90 degree.}*/

point   rotate(double a) const;
/*{\Mop     returns \var.rotate($point(0,0),\ a$). }*/

point   rotate90() const;
/*{\Mop     returns \var.rotate90($point(0,0)$). }*/


int operator==(const point& q) const;
/*{\Mbinop  Test for equality.}*/

bool operator&=(const point& q) const { return ptr() == q.ptr(); }

int operator!=(const point& q)  const { return !operator==(q);}
/*{\Mbinop  Test for inequality.}*/

point operator+(const vector& v) const { return translate(v); }
/*{\Mbinop  Translation by vector $v$.}*/

friend std::ostream& operator<<(std::ostream& O, const point& p) ;
/*{\Mbinopfunc  writes \var\ to output stream $O$.}*/

friend std::istream& operator>>(std::istream& I, point& p) ;
/*{\Mbinopfunc  reads the coordinates of \var\ (two $double$ numbers)
	        from input stream $I$.}*/

friend inline bool identical(const point& p, const point& q);

static int cmp(const point& a, const point& b)
{ int r = compare(a.xcoord(),b.xcoord());
  return (r!=0) ? r : compare(a.ycoord(),b.ycoord());
 }

};

inline void Print(const point& p, std::ostream& out) { out << p; }
inline void Read(point& p,  std::istream& in)        { in >> p; }

inline int compare(const point& a, const point& b)
{ int r = compare(a.xcoord(),b.xcoord());
  return (r!=0) ? r : compare(a.ycoord(),b.ycoord());
 }


// geometric primitives

/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

inline bool identical(const point& p, const point& q)
{ return p.ptr() == q.ptr(); }
/*{\Mfuncl  Test for identity.}*/


extern double area(const point& a, const point& b, const point& c);
/*{\Mfuncl computes the signed area of the triangle determined by $a$,$b$,$c$,
           positive if $orientation(a,b,c) > 0$ and negative otherwise. } */


inline int orientation(const point& a, const point& b, const point& c)
{ return compare((a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()),
                 (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord())); }
/*{\Mfuncl computes the orientation of points $a$, $b$, $c$, i.e.,
           the sign of the determinant\\
           \[ \left\Lvert \begin{array}{ccc} a_x & a_y & 1\\
                                        b_x & b_y & 1\\
                                        c_x & c_y & 1
                       \end{array} \right\Lvert \]
            }*/


inline bool collinear(const point& a, const point& b, const point& c)
{ return (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()) ==
         (a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()); }
/*{\Mfuncl returns true if points $a$, $b$, $c$ are collinear, i.e.,
           $orientation(a,b,c) = 0$, and false otherwise. }*/


inline bool right_turn(const point& a, const point& b, const point& c)
{ return (a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()) <
         (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()); }
/*{\Mfuncl returns true if points $a$, $b$, $c$ form a righ turn, i.e.,
           $orientation(a,b,c) > 0$, and false otherwise. }*/


inline bool left_turn(const point& a, const point& b, const point& c)
{ return (a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()) >
         (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()); }
/*{\Mfuncl returns true if points $a$, $b$, $c$ form a left turn, i.e.,
           $orientation(a,b,c) < 0$, and false otherwise. }*/


extern
int incircle(const point& a, const point& b, const point& c, const point& d);
/*{\Mfuncl returns $+1$ if point $d$ lies in the interior of the circle
           through points $a$, $b$, and $c$, $0$ if $a$,$b$,$c$,and $d$ are
           cocircular, and $-1$ otherwise. }*/

#endif

