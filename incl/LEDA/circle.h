/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  circle.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_CIRCLE_H
#define LEDA_CIRCLE_H

#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/line.h>

//------------------------------------------------------------------------------
// circles
//------------------------------------------------------------------------------


class circle_rep : public handle_rep {

friend class circle;

  double  radius;
  point   center; 
  
public:

  circle_rep() {}
  circle_rep(const point& p, double r)  { center = p; radius = r; }

 ~circle_rep() {}

};


/*{\Manpage {circle} {} {Circles} }*/ 

class circle   : public handle_base 
{
/*{\Mdefinition
An instance $C$ of the data type $circle$ is a circle in the two-dimensional
plane, i.e., the set of points having a certain distance $r$ from a given
point $p$. $r$ is called the radius and $p$ is called the center of $C$.
The circle with center $(0,0)$ and radius $0$ is called the empty circle.}*/

circle_rep* ptr() const { return (circle_rep*)PTR; }

public:

/*{\Mcreation C }*/ 


circle(const point& c, double r);
/*{\Mcreate introduces a variable $C$ of type $circle$. $C$ is initialized to 
            the circle with center $c$ and radius $r$.}*/

circle(double x, double y, double r);
/*{\Mcreate introduces a variable $C$ of type $circle$. $C$ is initialized to 
            the circle with center $(x,y)$ and radius $r$.}*/

circle(const point& a, const point& b, const point& c);
/*{\Mcreate introduces a variable $C$ of type $circle$. $C$ is initialized to 
            the circle through points $a$, $b$, and $c$. \precond $a$, $b$,
            and $c$ are not collinear. }*/

circle();
/*{\Mcreate introduces a variable $C$ of type $circle$. $C$ is initialized to 
            the empty circle.}*/

 circle(const circle& c) : handle_base(c) {}
~circle() {}

 circle& operator=(const circle& C) { handle_base::operator=(C); return *this; }


/*{\Moperations 2.2 4.7 }*/

point center()  const { return ptr()->center; } 
/*{\Mop  returns the center of \var.}*/

double radius() const { return ptr()->radius; }
/*{\Mop  returns the radius of \var.}*/

list<point> intersection(const circle& D) const;
/*{\Mop    returns $C \cap D$ as a list of points.}*/

list<point> intersection(const line& l) const;
/*{\Mop  returns $C \cap l$ as a list of points.}*/

list<point> intersection(const segment& s) const;
/*{\Mop   returns $C \cap s$ as a list of points.}*/

segment left_tangent(const point& p) const;
/*{\Mop   returns the line segment starting in $p$ tangent 
	  to \var\ and left of segment $[p,C.center()]$.}*/

segment right_tangent(const point& p) const;
/*{\Mop   returns the line segment starting in $p$ tangent 
	  to \var\ and right of segment $[p,C.center()]$.}*/

double  distance(const point& p) const;
/*{\Mop   returns the distance between \var\ and $p$ 
	  (negative if $p$ inside \var).}*/

double  distance(const line& l) const;
/*{\Mop    returns the distance between \var\ and $l$  
	   (negative if $l$ intersects \var).}*/

double  distance(const circle& D) const;
/*{\Mop    returns the distance between \var\ and $D$ 
	   (negative if $D$ intersects \var).}*/

bool    inside(const point& p) const;
/*{\Mop   returns true if $p$ lies inside of \var, 
	  false otherwise.}*/

bool    outside(const point& p) const { return !inside(p); };
/*{\Mop     returns !\var.inside($p$).}*/

circle  translate(double a, double d) const;
/*{\Mopl    returns the circle created by a translation of 
	    $C$ in direction $a$ by distance $d$.}*/

circle  translate(const vector& v) const; 
/*{\Mop    returns $C+v$, i.e., the circle created by 
	   translating $C$ by vector $v$.\\
	   \precond
	   $v$.dim() = 2.}*/

circle  operator+(const vector& v) const { return translate(v); }

circle  rotate(const point& q, double a) const;
/*{\Mopl    returns the circle created by a rotation of $C$ 
	    about point $q$ by angle $a$.}*/

circle  rotate(double a) const;
/*{\Mop    returns the circle created by a rotation of $C$ 
	    about the origin by angle $a$.}*/

bool operator==(const circle& D) const;
/*{\Mbinop    Test for equality.}*/

bool operator!=(const circle& D) const { return !operator==(D); };
/*{\Mbinop    Test for inequality.}*/

friend ostream& operator<<(ostream& out, const circle& c);
friend istream& operator>>(istream& in, circle& c);  


};

inline void Print(const circle& c, ostream& out) { out << c; } 
inline void Read(circle& c,  istream& in)        { in >> c; }


#endif
