/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _rat_segment.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/rat_segment.h>
#include <cmath>
#include <cctype>

//------------------------------------------------------------------------------
// rat_segment : segments with rational coordinates
//------------------------------------------------------------------------------


#if defined(sparc)
#define FABS(x) (*(unsigned long*)&x) &= 0x7FFFFFFF
#else
#define FABS(x) x=fabs(x)
#endif

unsigned long rat_segment_rep::id_counter = 0;


rat_segment_rep::rat_segment_rep(const rat_point& p, const rat_point& q) : start(p),end(q)
{ dx = q.X()*p.W() - p.X()*q.W();
  dy = q.Y()*p.W() - p.Y()*q.W();
  dxd = dx.todouble();
  dyd = dy.todouble();
  id  = id_counter++;
 }

rat_segment_rep::rat_segment_rep() : start(0,0,1), end(0,0,1)
{ dx  = 0;
  dy  = 0;
  dxd = 0;
  dyd = 0;
  id  = id_counter++;
 }


int rat_segment::use_filter = true;


/*
rat_segment rat_segment::translate(const vector& v) const
{ rat_point p = ptr()->start.translate(v);
  rat_point q = ptr()->end.translate(v);
  return rat_segment(p,q);
 }

*/


std::ostream& operator<<(std::ostream& out, const rat_segment& s)
{ out << "[" << s.start() << "===" << s.end() << "]";
  return out;
 }

std::istream& operator>>(std::istream& in, rat_segment& s)
{ // syntax: {[} p {===} q {]}

  rat_point p,q;
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

  s = rat_segment(p,q);
  return in;

 }



/*
double rat_segment::distance(const rat_segment& s) const
{ if (angle(s)!=0) return 0;
  return distance(s.ptr()->start);
 }

double  rat_segment::distance() const
{ return distance(rat_point(0,0)); }

double rat_segment::distance(const rat_point& p) const
{ rat_segment s(ptr()->start,p);
  double l = s.length();
  if (l==0) return 0;
  else return l*sin(angle(s));
 }


double  rat_segment::y_proj(double x)  const
{ return  ptr()->start.ptr()->y - ptr()->slope * (ptr()->start.ptr()->x - x); }

double  rat_segment::x_proj(double y)  const
{ if (vertical())  return  ptr()->start.ptr()->x;
  return  ptr()->start.ptr()->x - (ptr()->start.ptr()->y - y)/ptr()->slope; }

*/



bool rat_segment::intersection(const rat_segment& s, rat_point& I) const
{
  // decides whether |s| and |this| segment intersect and, if so,
  // returns the intersection in |I|. It is assumed that both segments
  // have non-zero length

  integer w = dy()*s.dx() - s.dy()*dx();

  if (w == 0) return false;   // $slope(s) = slope(this)$

  integer c1 =   X2()*  Y1() -   X1()*  Y2();
  integer c2 = s.X2()*s.Y1() - s.X1()*s.Y2();


  /* The underlying lines intersect in a point $p = (x,y,w)$.
     We still have to test whether $p$ lies on both segments.
     $p$ lies on $s$ ($this$)if its x-coordinate $x$ compares
     diffently with the x-coordinates of the two endpoints of $s$ ($this).
   */

  integer x = c2*dx()-c1*s.dx();

  int s1 = sign(x*W1() - X1()*w);
  int s2 = sign(x*W2() - X2()*w);

  if (s1 == s2 && s1 !=0) return false;

  s1 = sign(x*s.W1() - s.X1()*w);
  s2 = sign(x*s.W2() - s.X2()*w);

  if (s1 == s2 && s1 !=0) return false;

  I = rat_point(x, c2*dy()-c1*s.dy(), w);

  return true;

}


bool rat_segment::intersection_of_lines(const rat_segment& s, rat_point& inter) const
{
  /* decides whether the lines induced by |s| and |this| segment
     intersect and, if so, returns the intersection in |inter|.
     It is assumed that both segments have non-zero length
   */

  integer w = dy()*s.dx() - dx()*s.dy();

  if (w == 0) return false;   //same slope

  integer c1 =   X2()*  Y1() -   X1()*  Y2();
  integer c2 = s.X2()*s.Y1() - s.X1()*s.Y2();

  inter = rat_point(c2*dx()-c1*s.dx(), c2*dy()-c1*s.dy(), w);

  return true;
}


const double eps0 = ldexp(1,-53);
const double eps2 = ldexp(1,-47); // 64 * eps0

int cmp_slopes(const rat_segment& s1, const rat_segment& s2)
{
  if (rat_segment::use_filter)
  {
    double dy1dx2 = s1.dyd()*s2.dxd();
    double dy2dx1 = s2.dyd()*s1.dxd();
    double E = dy1dx2 - dy2dx1;

    //-----------------------------------------------------------------------
    //  ERROR BOUNDS
    //-----------------------------------------------------------------------
    // mes(E) = 2 * (mes(dy1*dx2) + mes(dy2*dx2))
    //        = 2 * (mes(dy1)*mes(dx2) + mes(dy2)*mes(dx2))
    //        = 2 * (4*fabs(dy1dx2) + 4*fabs(dy2dx2))
    //        = 8 * (fabs(dy1dx2) + fabs(dy2dx2))
    //
    // ind(E) = (ind(dy1*dx2) + ind(dy2*dx2) + 1)/2
    //        = (ind(dy1) + ind(dx2) + 0.5 + ind(dy2) + ind(dx2) + 0.5 + 1)/2
    //        = (0.5 + 0.5 + 0.5 + 0.5 + 0.5 + 0.5 + 1)/2
    //        = 2
    //
    // eps(E) = ind(E) * mes(E) * eps0
    //        = 16 * (fabs(dy1dx2) + fabs(dy2dx2)) * eps0
    //-----------------------------------------------------------------------

    FABS(dy1dx2);
    FABS(dy2dx1);
    double eps = 16 * (dy1dx2+dy2dx1) * eps0;

    if (E >  eps) return  1;
    if (E < -eps) return -1;
    if (eps < 1)  return 0;
  }

  // use big integers

  return sign(s1.dy() * s2.dx() - s2.dy() * s1.dx());
}



int orientation(const rat_segment& s, const rat_point& p)
{
  rat_point a = s.start();

  if (rat_segment::use_filter)
  {
    double dx = s.dxd();
    double dy = s.dyd();
    double axpw = a.XD() * p.WD();
    double aypw = a.YD() * p.WD();
    double pxaw = p.XD() * a.WD();
    double pyaw = p.YD() * a.WD();

    double E =  dy * (axpw - pxaw) - dx * (aypw - pyaw);

    //-----------------------------------------------------------------------
    //  ERROR BOUNDS
    //-----------------------------------------------------------------------
    //
    // mes(E) = 2 * (mes(dx) * 2*(mes(aypw)+mes(pyaw)) +
    //               mes(dy) * 2*(mes(axpw)+mes(pxaw)))
    //        = 4 * (mes(dx) * (mes(aypw)+mes(pyaw)) +
    //               mes(dy) * (mes(axpw)+mes(pxaw)))
    //        = 8 * (fabs(dx) * (4*fabs(aypw)+4*fabs(pyaw)) +
    //               fabs(dy) * (4*fabs(axpw)+4*fabs(pxaw)))
    //        =16 * (fabs(dx) * (fabs(aypw)+fabs(pyaw)) +
    //               fabs(dy) * (fabs(axpw)+fabs(pxaw)))
    //
    // ind(E) =  (ind(dx) + ind(aypw - pyaw) + 0.5 +
    //            ind(dy) + ind(axpw - pxaw) + 0.5 + 1)/2
    //        =  (ind(dx) + (ind(aypw) + ind(pyaw) + 1)/2 + 0.5 +
    //            ind(dy) + (ind(axpw) + ind(pxaw) + 1)/2 + 0.5 + 1)/2
    //        =  (0.5 + (1.5 + 1.5 + 1)/2 + 0.5
    //            0.5 + (1.5 + 1.5 + 1)/2 + 0.5 + 1)/2
    //        =  (0.5 + 2 + 0.5 + 0.5 + 2 + 0.5 + 1)/2
    //        =  3.5
    //
    // mes(E) = ind(E) * mes(E) * eps0
    //        = 56 * (fabs(dx) * (fabs(aypw)+fabs(pyaw)) +
    //                fabs(dy) * (fabs(axpw)+fabs(pxaw))) * eps0
    //-----------------------------------------------------------------------

    FABS(aypw);
    FABS(pyaw);
    FABS(axpw);
    FABS(pxaw);
    FABS(dx);
    FABS(dy);

    double eps = 56 * ((aypw+pyaw)*dx + (axpw+pxaw)*dy) * eps0;

    if (E >  eps) return  1;
    if (E < -eps) return -1;
    if (eps < 1)  return 0;
   }

  // big integers

  return sign( s.dy() * (a.X()*p.W() - p.X()*a.W()) -
               s.dx() * (a.Y()*p.W() - p.Y()*a.W()) );

 }




int cmp_segments_at_xcoord(const rat_segment& s1, const rat_segment& s2,
                           const rat_point& r)
{

  // filter : still to do

  integer px = s1.X1();
  integer py = s1.Y1();
  integer pw = s1.W1();

  integer spx = s2.X1();
  integer spy = s2.Y1();
  integer spw = s2.W1();

  integer rx = r.X();
  integer ry = r.Y();
  integer rw = r.W();

  integer dx = s1.dx();
  integer dy = s1.dy();

  integer sdx = s2.dx();
  integer sdy = s2.dy();

  integer D = sdx * spw * (py * dx * rw + dy * (rx * pw - px * rw))
              - dx * pw * (spy * sdx * rw + sdy * (rx * spw - spx * rw));

  return sign(D);

}



bool intersection(const rat_segment& s1, const rat_segment& s2)
{
  // decides whether |s1| and |s2| intersect

  int o1 = orientation(s1,s2.start());
  int o2 = orientation(s1,s2.end());
  int o3 = orientation(s2,s1.start());
  int o4 = orientation(s2,s1.end());

  return o1 != o2 && o3 != o4;
}


