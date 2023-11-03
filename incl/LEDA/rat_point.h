/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  rat_point.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_POINT_H
#define LEDA_RAT_POINT_H

#include <math.h>
#include <LEDA/integer.h>

class rat_point;
class rat_segment;


//------------------------------------------------------------------------------
// rat_points
//------------------------------------------------------------------------------

class rat_point_rep  : public handle_rep {

friend class rat_point;
friend class rat_segment;
   
   integer x;
   integer y;
   integer w;

   double  xd;
   double  yd;
   double  wd;

public:

   rat_point_rep() : x(0), y(0), w(1)
   { xd = 0;
     yd = 0;
     wd = 1;
    }

   rat_point_rep(integer a, integer b) : x(a), y(b), w(1)
   { xd = a.todouble(); 
     yd = b.todouble(); 
     wd = 1;
    }

   rat_point_rep(integer a, integer b, integer c) : x(a), y(b), w(c)
   { xd = a.todouble(); 
     yd = b.todouble(); 
     wd = c.todouble();
    }

  ~rat_point_rep() {}

friend int orientation(const rat_point&, const rat_point&, const rat_point&);
   
};



/*{\Manpage {rat_point} {} {Rational Points} }*/

class rat_point  : public handle_base {

/*{\Mdefinition
An instance of the data type $rat\_point$ is a point with rational coordinates 
in the two-dimensional plane. A point $(a,b)$ is represented by homogeneous
coordinates $(x,y,w)$ of arbitrary length integers (see \ref{Integers of 
Arbitrary Length}) such that $a = x/w$ and $b = y/w$. }*/


 friend class rat_segment;

 rat_point_rep* ptr() const { return (rat_point_rep*)PTR; } 

public:

static int orient_count;
static int exact_orient_count;

static int cmp_count;
static int exact_cmp_count;

static int use_filter;

/*{\Mcreation p}*/

rat_point() { PTR = new rat_point_rep; }
/*{\Mcreate introduces a variable \var\ of type \name\
initialized to the point $(0,0)$.}*/

 rat_point(integer a, integer b)       { PTR = new rat_point_rep(a,b); }
/*{\Mcreate introduces a variable \var\ of type \name\
initialized to the point $(a,b)$.}*/

 rat_point(integer x, integer y, integer w){ PTR = new rat_point_rep(x,y,w); }
/*{\Mcreate introduces a variable \var\ of type \name\
initialized to the point with homogeneous coordinates$(x,y,w)$.}*/

 rat_point(const rat_point& p) : handle_base(p) {}
~rat_point() {}

 rat_point& operator=(const rat_point& p) 
 { handle_base::operator=(p); return *this; }

/*{\Moperations 2 3.5 }*/

double xcoord() const { return ptr()->xd/ptr()->wd;}
/*{\Mop     returns a double precision floating point approximation of the 
            $x$-coordinate of \var.}*/

double ycoord() const { return ptr()->yd/ptr()->wd;}
/*{\Mop     returns a double precision floating point approximation of the 
            $y$-coordinate of \var.}*/

integer X() const { return ptr()->x; }
/*{\Mop     returns the first homogeneous coordinate of \var.}*/

integer Y() const { return ptr()->y; }
/*{\Mop     returns the second homogeneous coordinate of \var.}*/

integer W() const { return ptr()->w; }
/*{\Mop     returns the third homogeneous coordinate of \var.}*/

double XD() const { return ptr()->xd; }
/*{\Mop     returns a floating point approximation of the first homogeneous 
            coordinate of \var.}*/

double YD() const { return ptr()->yd; }
/*{\Mop     returns a floating point approximation of the second homogeneous 
            coordinate of \var.}*/

double WD() const { return ptr()->wd; }
/*{\Mop     returns a floating point approximation of the third homogeneous 
            coordinate of \var.}*/


rat_point rotate90(const rat_point& q) const;
/*{\Mopl    returns \var\ rotated by 90 degrees about $q$. }  */

rat_point rotate90() const;
/*{\Mop     returns \var\ rotated by 90 degrees about the origin. }  */

rat_point translate(const rat_point& p, int i) const;
/*{\Mopl    returns \var\ translated by ... }  */


friend bool identical(const rat_point& p, const rat_point& q)
{ return p.ptr() == q.ptr(); }
/*{\Mfuncl  Test for identity.}*/

friend bool operator==(const rat_point& p, const rat_point& q)
{ return (identical(p,q)) || rat_point::cmp(p,q) == 0; }
/*{\Mbinopfunc  Test for equality.}*/

friend bool operator!=(const rat_point& p, const rat_point& q)
{ return (!identical(p,q)) && rat_point::cmp(p,q) != 0; }
/*{\Mbinopfunc  Test for inequality.}*/

friend ostream& operator<<(ostream& O, const rat_point& p) ;
/*{\Mbinopfunc  writes the homogeneous coordinates $(x,y,w)$ of \var\ to 
                output stream $O$.}*/

friend istream& operator>>(istream& I, rat_point& p) ;
/*{\Mbinopfunc  reads the homogeneous coordinates $(x,y,w)$ of \var\ from 
                input stream $I$.}*/


friend int orientation(const rat_point& a, const rat_point& b, const rat_point& c);
/*{\Mfuncl  computes the orientation of points $a$, $b$, $c$, i.e.,
           the sign of the determinant\\
           \[ \left\Lvert \begin{array}{ccc} a_x & a_y & a_w\\
                                        b_x & b_y & b_w\\
                                        c_x & c_y & c_w
                       \end{array} \right\Lvert \] 
           }*/


static int cmp(const rat_point&, const rat_point&);

};


extern 
double area(const rat_point& a, const rat_point& b, const rat_point& c);
/*{\Mfuncl computes the signed area of the triangle determined by $a$,$b$,$c$,
           positive if $orientation(a,b,c) > 0$ and negative otherwise. }*/


extern
int incircle(const rat_point& a, const rat_point& b, const rat_point& c,
                                                     const rat_point& d);
/*{\Mfuncl returns $+1$ if point $d$ lies in the interior of the circle
           through points $a$, $b$, and $c$, $0$ if $a$,$b$,$c$,and $d$ are
           cocircular, and $-1$ otherwise. }*/


inline int compare(const rat_point& a, const rat_point& b)
{  return (identical(a,b))  ? 0 : rat_point::cmp(a,b); }


inline void Print(const rat_point& p, ostream& out) { out << p; } 
inline void Read(rat_point& p,  istream& in)        { in >> p; }


inline bool collinear(const rat_point& a,const rat_point& b,const rat_point& c)
{ return orientation(a,b,c) == 0; }
/*{\Mfuncl  returns true if points $a$, $b$, $c$ are collinear, i.e.,
           $orientation(a,b,c) = 0$, and false otherwise. }*/


inline bool right_turn(const rat_point& a,const rat_point& b,const rat_point& c)
{ return orientation(a,b,c) < 0; }
/*{\Mfuncl  returns true if points $a$, $b$, $c$ form a righ turn, i.e.,
           $orientation(a,b,c) > 0$, and false otherwise. }*/


inline bool left_turn(const rat_point& a,const rat_point& b,const rat_point& c)
{ return orientation(a,b,c) > 0; }
/*{\Mfuncl  returns true if points $a$, $b$, $c$ form a left turn, i.e.,
           $orientation(a,b,c) < 0$, and false otherwise. }*/



#endif

    
