/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _line.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/line.h>
#include <math.h>

//------------------------------------------------------------------------------
// lines 
//------------------------------------------------------------------------------


line::line()
{ PTR = new line_rep; }

line::line(const segment& s) 
{ PTR = new line_rep(s); }

line::line(const point& x, const point& y)    
{ PTR = new line_rep(segment(x,y)); }

line::line(const point& p, const vector& v) 
{ PTR = new line_rep(segment(p,v)); }

line::line(const point& p, double alpha) 
{ PTR = new line_rep(segment(p,alpha,1)); }
  


bool line::contains(const point& p) const
{ return orientation(ptr()->seg,p) == 0; }

bool line::contains(const segment& s) const
{ return contains(s.start()) && contains(s.end()); }


ostream& operator<<(ostream& out, const line& l) 
{ return out << l.seg(); }

istream& operator>>(istream& in, line& l)  
{ segment s; 
  in >> s; 
  l = line(s); 
  return in; 
 }


bool line::intersection(const line& s, point& inter) const
{ return ptr()->seg.intersection_of_lines(s.ptr()->seg,inter) ; }



bool line::intersection(const segment& s, point& inter) const
{ 
  double orient1 = orientation(ptr()->seg,s.start());
  double orient2 = orientation(ptr()->seg,s.end());

  if ( orient1*orient2 <= 0) 
     { ptr()->seg.intersection_of_lines(s,inter);
       return true;
      }
  else
     return false;
}


segment line::perpendicular(const point& q) const
{ segment s0 = ptr()->seg;
  segment s1 = s0.rotate90();
  point r;
  s0.intersection_of_lines(s1,r);
  return segment(q,r);
 }


line p_bisector(const point& p, const point& q)
{ double dx = q.xcoord() - p.xcoord();
  double dy = q.ycoord() - p.ycoord();
  double x1 = (p.xcoord() + q.xcoord())/2;
  double y1 = (p.ycoord() + q.ycoord())/2;
  return line(point(x1,y1),point(x1+dy,y1-dx));
}

