/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  rat_segment.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_RAT_SEGMENT_H
#define LEDA_RAT_SEGMENT_H

#include <LEDA/rat_point.h>

//------------------------------------------------------------------------------
// rat_segments
//------------------------------------------------------------------------------


class rat_segment_rep : public handle_rep {

static unsigned long id_counter;

friend class rat_segment;
friend class rat_line;
   
   rat_point start;
   rat_point end;

   integer dx;
   integer dy;

   double dxd;
   double dyd;

   unsigned long id;

public:
   
   rat_segment_rep(const rat_point&, const rat_point&);
   rat_segment_rep();  
  ~rat_segment_rep() {}
   
};

/*{\Manpage {rat_segment} {} {Rational Segments} }*/


class rat_segment  : public handle_base {

/*{\Mdefinition
An instance $s$ of the data type $rat\_segment$ is a directed straight line
segment with rational coordinates in the two-dimensional plane, i.e.,
a line segment $[p,q]$ connecting two rational points $p$ and $q$ (cf. 
\ref{Rational Points}). $p$ is called the start point and $q$ is called the 
end point of $s$. The segment $[(0,0),(0,0)]$ is said to be empty. }*/
 
  friend class rat_line;

  rat_segment_rep* ptr() const { return (rat_segment_rep*)PTR; }


public:

static int use_filter;

/*{\Mcreation s }*/

rat_segment() { PTR = new rat_segment_rep; }
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the empty segment. }*/

rat_segment(const rat_point& p, const rat_point& q) 
  { PTR = new rat_segment_rep(p,q); }
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the segment $(p,q)$. }*/


rat_segment(const integer& x1, const integer& y1, const integer& x2, const integer& y2) 
{ PTR = new rat_segment_rep(rat_point(x1,y1), rat_point(x2,y2)); }
/*{\Mcreate introduces a variable \var\ of type \name. \var\ is initialized
            to the segment $[(x1,y1),(x2,y2)]$. }*/


  rat_segment(const rat_segment& s) : handle_base(s) {}     
 ~rat_segment()                {}

  rat_segment& operator=(const rat_segment& s) 
  { handle_base::operator=(s); return *this;}


/*{\Moperations 2 3.5 }*/


rat_point start()  const { return ptr()->start; }
rat_point source() const { return ptr()->start; }
/*{\Mop       returns the source point of segment \var.}*/

rat_point end()    const { return ptr()->end; }
rat_point target() const { return ptr()->end; }
/*{\Mop       returns the target point of segment \var.}*/

double xcoord1() const { return ptr()->start.xcoord(); }
/*{\Mop       returns a double precision approximation of the $x$-coordinate
              of the start point of segment \var.}*/

double xcoord2() const { return ptr()->end.xcoord();   }
/*{\Mop       returns a double precision approximation of the $x$-coordinate
              of the end point of segment \var.}*/

double ycoord1() const { return ptr()->start.ycoord(); }
/*{\Mop       returns a double precision approximation of the $y$-coordinate
              of the start point of segment \var.}*/

double ycoord2() const { return ptr()->end.ycoord();   }
/*{\Mop       returns a double precision approximation of the $y$-coordinate
              of the end point of segment \var.}*/

integer X1() const { return ptr()->start.X(); }
/*{\Mop       returns the first homogeneous coordinate of the start point 
              of segment \var.}*/

integer X2() const { return ptr()->end.X();   }
/*{\Mop       returns the first homogeneous coordinate of the end point 
              of segment \var.}*/

integer Y1() const { return ptr()->start.Y(); }
/*{\Mop       returns the second homogeneous coordinate of the start point 
              of segment \var.}*/

integer Y2() const { return ptr()->end.Y();   }
/*{\Mop       returns the second homogeneous coordinate of the end point 
              of segment \var.}*/

integer W1() const { return ptr()->start.W(); }
/*{\Mop       returns the third homogeneous coordinate of the start point 
              of segment \var.}*/

integer W2() const { return ptr()->end.W();   }
/*{\Mop       returns the third homogeneous coordinate of the end point 
              of segment \var.}*/

double XD1() const { return ptr()->start.XD(); }
/*{\Mop       returns a floating point approximation of the first homogeneous 
              coordinate of the start point of segment \var.}*/

double XD2() const { return ptr()->end.XD();   }
/*{\Mop       returns a floating point approximation of the first homogeneous 
              coordinate of the end point of segment \var.}*/

double YD1() const { return ptr()->start.YD(); }
/*{\Mop       returns a floating point approximation of the second homogeneous 
              coordinate of the start point of segment \var.}*/

double YD2() const { return ptr()->end.YD();   }
/*{\Mop       returns a floating point approximation of the second homogeneous 
              coordinate of the end point of segment \var.}*/

double WD1() const { return ptr()->start.WD(); }
/*{\Mop       returns a floating point approximation of the third homogeneous 
              coordinate of the start point of segment \var.}*/

double WD2() const { return ptr()->end.WD();   }
/*{\Mop       returns a floating point approximation of the third homogeneous 
              coordinate of the end point of segment \var.}*/


integer dx() const { return ptr()->dx; }
/*{\Mop       returns the normalized $x$-difference $X1\cdot W2 - X2\cdot W1$
              of the segment. }*/

integer dy() const { return ptr()->dy; }
/*{\Mop       returns the normalized $y$-difference $Y1\cdot W2 - Y2\cdot W1$
              of the segment. }*/

double dxd() const { return ptr()->dxd; }
/*{\Mop       returns the optimal floating point approximation of the 
              normalized $x$-difference of the segment. }*/

double dyd() const { return ptr()->dyd; }
/*{\Mop       returns the optimal floating point approximation of the 
              normalized $y$-difference of the segment. }*/


bool vertical()   const { return ptr()->dx == 0; }
/*{\Mop       returns true if \var\ is vertical and false otherwise. }*/

bool horizontal() const { return ptr()->dy == 0; }
/*{\Mop       returns true if \var\ is horizontal and false otherwise. }*/

int cmp_slope(const rat_segment& s1) const 
{ return sign(dy()*s1.dx()-s1.dy()*dx()); }
/*{\Mopl       compares the slopes of \var\ and $s_1$. }*/


bool intersection(const rat_segment& t, rat_point& p) const;
/*{\Mopl    if \var\ and $t$ are not collinear and intersect the
            point of intersection is assigned to $p$ and the result is
            true, otherwise the result is false. }*/


bool intersection_of_lines(const rat_segment& t, rat_point& p) const;
/*{\Mopl    if the lines supporting \var\ and $t$ are not parallel
            their point of intersection is assigned to $p$ and the result is
            true, otherwise the result is false. }*/


int operator==(const rat_segment& t) const
{ return (ptr()->start == t.ptr()->start && ptr()->end == t.ptr()->end); }
/*{\Mbinop       Test for equality.}*/

int operator!=(const rat_segment& t) const { return !operator==(t);}
/*{\Mbinop       Test for inequality.}*/


friend bool identical(const rat_segment& s1, const rat_segment& s2)
{ return s1.ptr() == s2.ptr(); }
/*{\Mbinopfunc  Test for identity ...}*/


friend ostream& operator<<(ostream& O, const rat_segment& s);
/*{\Mbinopfunc writes the homogeneous coordinates of $s$ (six $integer$ numbers)
               to output stream $O$.}*/

friend istream& operator>>(istream& I, rat_segment& s);
/*{\Mbinopfunc reads the homogeneous coordinates of $s$ (six $integer$ numbers)
               from input stream $I$.}*/

friend int orientation(const rat_segment& s, const rat_point& p);
/*{\Mfuncl      computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on segment $s$. }*/

friend int cmp_slopes(const rat_segment& s1, const rat_segment& s2);
/*{\Mfuncl      returns compare(slope($s_1$), slope($s_2$)).}*/

friend int cmp_segments_at_xcoord(const rat_segment& s1,const rat_segment& s2,
                          const rat_point& r);

friend bool intersection(const rat_segment& s1,const rat_segment& s2);
/*{\Mfuncl      decides whether $s1$ and $s2$ intersect. }*/


};


inline void Print(const rat_segment& s, ostream& out) { out << s; } 
inline void Read(rat_segment& s, istream& in) { in >> s; }


inline int parallel(const rat_segment& s1, const rat_segment& s2)
{ return cmp_slopes(s1,s2) == 0; }


#endif
