/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _point.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/segment.h>
#include <math.h>
#include <ctype.h>

//------------------------------------------------------------------------------
// points 
//------------------------------------------------------------------------------


point_rep::point_rep()  { count=1; x = y = 0.0; }

point_rep::point_rep(double a, double b) 
{ x = a; 
  y = b; 
  count = 1; 
}



point::point()                  { PTR = new point_rep; }
point::point(double x, double y){ PTR = new point_rep(x,y); }
point::point(vector v)          { PTR = new point_rep(v[0], v[1]); }

double point::angle(const point& q, const point& r) const
{
  double cosfi,fi,norm;
  
  double dx  = q.ptr()->x - ptr()->x; 
  double dy  = q.ptr()->y - ptr()->y; 

  double dxs = r.ptr()->x - q.ptr()->x; 
  double dys = r.ptr()->y - q.ptr()->y; 
  
  cosfi=dx*dxs+dy*dys;
  
  norm=(dx*dx+dy*dy)*(dxs*dxs+dys*dys);

  cosfi /= sqrt( norm );

  if (cosfi >=  1.0 ) return 0;
  if (cosfi <= -1.0 ) return LEDA_PI;
  
  fi=acos(cosfi);

  if (dx*dys-dy*dxs>0) return fi;

  return -fi;
}
  


// Rotations 

point point::rotate90(const point& origin) const
{ double cx = origin.xcoord();
  double cy = origin.ycoord();
  double dx = xcoord() - cx;
  double dy = ycoord() - cy;
  return point(cx-dy,cy+dx);
}

point point::rotate90() const
{ return point(-ycoord(),xcoord()); }


point point::rotate(const point& origin, double fi) const
{ double cx = origin.xcoord();
  double cy = origin.ycoord();
  double sinfi = sin(fi);
  double cosfi = cos(fi);
  double dx = xcoord() - cx;
  double dy = ycoord() - cy;
  return point(cx+dx*cosfi-dy*sinfi,cy+dx*sinfi+dy*cosfi);
}


point point::rotate(double fi) const
{ double sinfi = sin(fi);
  double cosfi = cos(fi);
  double x = xcoord();
  double y = ycoord();
  return point(x*cosfi-y*sinfi,x*sinfi+y*cosfi);
}



// Translations

/*
point point::translate(double dx, double dy) const
{ return point(xcoord()+dx,ycoord()+dy); }
*/


point point::translate(double phi, double d) const
{ double dx = cos(phi) * d;
  double dy = sin(phi) * d;
  if (fabs(dx) < 1e-12) dx = 0; 
  if (fabs(dy) < 1e-12) dy = 0; 
  return point(xcoord()+dx,ycoord()+dy);
 }

point point::translate(const vector& v) const 
{ return point(xcoord()+v[0],ycoord()+v[1]); }


// Distances

double point::distance(const point& p)  const
{ double dx = p.ptr()->x - ptr()->x; 
  double dy = p.ptr()->y - ptr()->y;
  return sqrt(dx*dx + dy*dy);
 }

double point::distance() const
{ return distance(point(0,0)); }



int point::operator==(const point& p) const 
{ return (ptr()->x == p.ptr()->x) && (ptr()->y == p.ptr()->y); }


   

int incircle(const point& a, const point& b, const point& c, const point& d)
{
   double ax = a.xcoord();
   double ay = a.ycoord();

   double bx = b.xcoord() - ax;
   double by = b.ycoord() - ay;
   double bw = bx*bx + by*by;

   double cx = c.xcoord() - ax;
   double cy = c.ycoord() - ay;
   double cw = cx*cx + cy*cy;

   double dx = d.xcoord() - ax;
   double dy = d.ycoord() - ay;
   double dw = dx*dx + dy*dy;

   //return (by*cw-cy*bw) * (bx*dw-dx*bw) > (bx*cw-cx*bw) * (by*dw-dy*bw);

   double D = (by*cx-bx*cy)*dw + (cy*bw-by*cw)*dx  + (bx*cw-cx*bw)*dy;

   if (D != 0) 
      return (D > 0) ? 1 : -1;
   else
      return 0;
 }


double area(const point& a, const point& b, const point& c)
{ return ((a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()) -
          (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()))/2; }



ostream& operator<<(ostream& out, const point& p)
{ out << "(" << p.xcoord() << "," << p.ycoord() << ")";
  return out;
 } 

istream& operator>>(istream& in, point& p) 
{ // syntax: {(} x {,} y {)}

  double x,y; 
  char c;

  do in.get(c); while (in && isspace(c));

  if (!in) return in;

  if (c != '(') in.putback(c);

  in >> x;

  do in.get(c); while (isspace(c));
  if (c != ',') in.putback(c);

  in >> y; 

  do in.get(c); while (c == ' ');
  if (c != ')') in.putback(c);

  p = point(x,y); 
  return in; 

 } 

