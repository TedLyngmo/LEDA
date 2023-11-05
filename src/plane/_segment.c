/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _segment.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/line.h>
#include <math.h>
#include <ctype.h>

unsigned long segment_rep::id_counter = 0;

//------------------------------------------------------------------------------
// segments 
//------------------------------------------------------------------------------

segment_rep::segment_rep()  { count = 1; id  = id_counter++; }

segment_rep::segment_rep(const point& p, const point& q) 
{ 
  count = 1; 
  start = p;
  end   = q;
  dx    = q.xcoord() - p.xcoord(); 
  dy    = q.ycoord() - p.ycoord(); 

  if (dx==0)
    { slope = MAXDOUBLE;
      y_abs = -MAXDOUBLE;
     }
  else
    { slope = dy/dx;
      y_abs =  p.ycoord() - slope * p.xcoord();
     }


  if (dx != 0 || dy != 0)
      angle = atan2(dy,dx); 
  else
      angle = 0;

  id  = id_counter++; 
}
  


segment::segment() { PTR = new segment_rep; }

segment::segment(const point& x, const point& y) 
{ PTR = new segment_rep(x,y); }

segment::segment(const point& x, const vector& v) 
{ PTR = new segment_rep(x,x+v); }

segment::segment(double x1, double y1, double x2, double y2) 
{ PTR = new segment_rep(point(x1,y1), point(x2,y2)); }

segment::segment(const point& p, double alpha, double length)
{ point q = p.translate(alpha,length);
  PTR  = new segment_rep(p,q); 
 }
  

segment segment::translate(double alpha, double d) const
{ point p = ptr()->start.translate(alpha,d);
  point q = ptr()->end.translate(alpha,d);
  return segment(p,q);
 }

segment segment::translate(const vector& v) const
{ point p = ptr()->start.translate(v);
  point q = ptr()->end.translate(v);
  return segment(p,q);
 }


ostream& operator<<(ostream& out, const segment& s) 
{ out << "[" << s.start() << "===" << s.end() << "]"; 
  return out;
 } 


istream& operator>>(istream& in, segment& s) 
{ // syntax: {[} p {===} q {]}

  point p,q; 
  char c;

  do in.get(c); while (isspace(c));
  if (c != '[') in.putback(c);

  in >> p;

  do in.get(c); while (isspace(c));
  while (c== '=') in.get(c);
  while (isspace(c)) in.get(c);
  in.putback(c);

  in >> q; 

  do in.get(c); while (c == ' ');
  if (c != ']') in.putback(c);

  s = segment(p,q); 
  return in; 

 } 




double segment::angle(const segment& s) const
{
  double cosfi,fi,norm;
  
  double dx  = ptr()->end.ptr()->x - ptr()->start.ptr()->x; 
  double dy  = ptr()->end.ptr()->y - ptr()->start.ptr()->y; 

  double dxs = s.ptr()->end.ptr()->x - s.ptr()->start.ptr()->x; 
  double dys = s.ptr()->end.ptr()->y - s.ptr()->start.ptr()->y; 
  
  cosfi=dx*dxs+dy*dys;
  
  norm=(dx*dx+dy*dy)*(dxs*dxs+dys*dys);

  if (norm == 0) return 0;

  cosfi /= sqrt( norm );

  if (cosfi >=  1.0 ) return 0;
  if (cosfi <= -1.0 ) return LEDA_PI;
  
  fi=acos(cosfi);

  if (dx*dys-dy*dxs>0) return fi;

  return -fi;

}


double segment::distance(const segment& s) const
{ if (angle(s)!=0) return 0;
  return distance(s.ptr()->start);
 }

double  segment::distance() const
{ return distance(point(0,0)); }

double segment::distance(const point& p) const
{ segment s(ptr()->start,p);
  double l = s.length();
  if (l==0) return 0;
  else return l*sin(angle(s));
 }


double  segment::y_proj(double x)  const
{ return  ptr()->start.ptr()->y - ptr()->slope * (ptr()->start.ptr()->x - x); }

double  segment::x_proj(double y)  const
{ if (vertical())  return  ptr()->start.ptr()->x;
  return  ptr()->start.ptr()->x - (ptr()->start.ptr()->y - y)/ptr()->slope; }



bool segment::intersection(const segment& s, point& inter) const
{ double cx,cy;

  if (slope() == s.slope()) return false;

  if (start() == s.start() || start() == s.end())
  { inter = start();
    return true;
   }

  if (end() == s.start() || end() == s.end())
  { inter = end();
    return true;
   }


  if (vertical())
     cx = xcoord1();
  else
     if (s.vertical())
        cx = s.xcoord1();
     else
        cx = (s.y_abs()-y_abs())/(slope()-s.slope());

  // test whether cx lies in the x-ranges of both segments
 
  if ( xcoord1()   < cx && xcoord2()   < cx ||
       xcoord1()   > cx && xcoord2()   > cx ||
       s.xcoord1() < cx && s.xcoord2() < cx ||
       s.xcoord1() > cx && s.xcoord2() > cx   ) return false;

  if (vertical())
     cy = s.slope() * cx + s.y_abs();
  else
     cy = slope() * cx + y_abs();


  // if vertical test y-ranges too

  if (vertical() && (ycoord1() < cy && ycoord2() < cy
                     || ycoord1() > cy && ycoord2() > cy) ) return false;

  if (s.vertical() && (s.ycoord1() < cy && s.ycoord2() < cy
                       || s.ycoord1() > cy && s.ycoord2() > cy) ) return false;


  inter = point(cx,cy);

  return true;
}


bool segment::intersection_of_lines(const segment& s, point& inter) const
{ double cx,cy;

  if (slope() == s.slope()) return false;

  if (start() == s.start() || start() == s.end())
  { inter = start();
    return true;
   }

  if (end() == s.start() || end() == s.end())
  { inter = end();
    return true;
   }


  if (vertical())
     cx = xcoord1();
  else
     if (s.vertical())
        cx = s.xcoord1();
     else
        cx = (s.y_abs()-y_abs())/(slope()-s.slope());

  if (vertical())
     cy = s.slope() * cx + s.y_abs();
  else
     cy = slope() * cx + y_abs();

  inter = point(cx,cy);

  return true;
}


segment segment::rotate(double alpha) const
{ point p = start();
  point q = end();
  return segment(p,q.rotate(p,alpha));
}

segment segment::rotate(const point& origin, double alpha) const
{  point p = start().rotate(origin,alpha);
   point q = end().rotate(origin,alpha);
   return segment(p,q);
}

segment segment::rotate90() const
{ return segment(start(),point(xcoord1()-dy(),ycoord1()+dx())); }

segment segment::rotate90(const point& origin) const
{  return segment(start().rotate90(origin),end().rotate90(origin)); }


