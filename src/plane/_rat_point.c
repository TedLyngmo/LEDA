/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _rat_point.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <math.h>
#include <ctype.h>
#include <LEDA/rat_point.h>

//------------------------------------------------------------------------------
// rat_points :  points with rational (homogeneous) coordinates
//------------------------------------------------------------------------------


// the fabs function is used very often therefore we provide
// a fast version for sparc machines (should work on all big-endian 
// architectures) that simply clears the sign bit to zero  
//
// FABS(x) clears the sign bit of (double) floating point number x

#if defined(sparc)
#define FABS(x) (*(unsigned long*)&x) &= 0x7FFFFFFF
#else
#define FABS(x) x=fabs(x)
#endif


// static members used for statistics

int rat_point::orient_count= 0;
int rat_point::exact_orient_count = 0;

int rat_point::cmp_count= 0;
int rat_point::exact_cmp_count = 0;

// use_filter flag controls whether the floating point filter is used 

int rat_point::use_filter = 1;


rat_point rat_point::rotate90(const rat_point& p) const
{ integer x0 = p.X();
  integer y0 = p.Y();
  integer w0 = p.W();
  integer x1 = X();
  integer y1 = Y();
  integer w1 = W();
  integer x = (x0 + y0) * w1 - y1 * w0;
  integer y = (y0 - x0) * w1 + x1 * w0;
  return rat_point(x,y,w0*w1);
 }


rat_point rat_point::rotate90() const
{ return rat_point(-Y(),X(),W()); }



rat_point rat_point::translate(const rat_point& p, int i) const
{ integer x0 = p.X();
  integer y0 = p.Y();
  integer w0 = p.W();
  integer x1 = X();
  integer y1 = Y();
  integer w1 = W();
  integer x = x0 * (i-1) * w1 + x1 * w0;
  integer y = y0 * (i-1) * w1 + y1 * w0;
  return rat_point(x,y,w0*w1*i);
 }


// basic stream I/O operations

ostream& operator<<(ostream& out, const rat_point& p)
{ out << "(" << p.X() << "," << p.Y() << "," << p.W() << ")";
  return out;
 } 

istream& operator>>(istream& in, rat_point& p) 
{ // syntax: {(} x {,} y {,} w {)}   

  int x,y,w;
  char c;

  do in.get(c); while (in && isspace(c));

  if (!in) return in;

  if (c != '(') in.putback(c);

  in >> x;

  do in.get(c); while (isspace(c));
  if (c != ',') in.putback(c);

  in >> y; 

  do in.get(c); while (isspace(c));
  if (c != ',') in.putback(c);

  in >> w; 

  do in.get(c); while (c == ' ');
  if (c != ')') in.putback(c);

  p = rat_point(x,y,w ); 
  return in; 

 } 



// machine precision

const double eps0 = ldexp(1,-53);


// cmp - primitive

int rat_point::cmp(const rat_point& a, const rat_point& b)
{ 
  rat_point::cmp_count++;

/*
  if (rat_point::use_filter == -1) 
  { double E = a.XD()*b.WD() - b.XD()*a.WD();
    if (E == 0) E = a.YD()*b.WD() - b.YD()*a.WD();
    if (E > 0) return +1;
    if (E < 0) return -1;
    return 0;
  }
*/

  if (use_filter != 0)
  { 
    double axbw = a.XD()*b.WD();
    double bxaw = b.XD()*a.WD();
    double E    = axbw - bxaw;       // floating point result

    //----------------------------------------------------------------
    // ERROR BOUND:
    //----------------------------------------------------------------
    //
    // mes(E) = mes(ax*bw - bx*aw)
    //        = 2*(mes(ax)*mes(bw) + mes(bx)*mes(aw))
    //        = 2*(fabs(ax)*fabs(bw) + fabs(bx)*fabs(aw)) 
    //        = 2*(fabs(axbw) + fabs(bxaw)) 
    //
    // ind(E) = ind(ax*bw - bx*aw)
    //        = (ind(ax*bw) + ind(bx*aw) + 1)/2
    //        = ((ind(ax)+ind(bw)+0.5) + (ind(bx)+ind(aw)+0.5) + 1)/2
    //        = (1.5 + 1.5 + 1)/2
    //        = 2
    //
    // eps(E) = ind(E) * mes(E) * eps0 
    //        = 4 * (fabs(axbw) + fabs(bxaw)) * eps0
    //----------------------------------------------------------------


    FABS(axbw);
    FABS(bxaw);
    double eps = 4 * (axbw+bxaw) * eps0;

    if (E > +eps) return +1;
    if (E < -eps) return -1;
   
    if (eps < 1)  // compare y-coordinates
    { double aybw = a.YD()*b.WD();
      double byaw = b.YD()*a.WD();
      double E    = aybw - byaw;
      FABS(aybw);
      FABS(byaw);
      double eps = 4 * (aybw+byaw) * eps0;
      if (E > +eps) return +1;
      if (E < -eps) return -1;
      if (eps < 1)  return  0; 
     }
   }
  
    //use big integer arithmetic

    rat_point::exact_cmp_count++;

    integer axbw = a.X()*b.W();
    integer bxaw = b.X()*a.W();
    if (axbw > bxaw) return  1;
    if (axbw < bxaw) return -1;

    integer aybw = a.Y()*b.W();
    integer byaw = b.Y()*a.W();
    if (aybw > byaw) return  1;
    if (aybw < byaw) return -1;
    return 0;
}



double area(const rat_point& a, const rat_point& b, const rat_point& c)
{ return ((a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()) -
          (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()))/2; }


int orientation(const rat_point& a, const rat_point& b, const rat_point& c)
{  
    rat_point::orient_count++;

    if (rat_point::use_filter != 0)
    { 
      double ax =  a.ptr()->xd; 
      double bx =  b.ptr()->xd; 
      double cx =  c.ptr()->xd; 
      double ay =  a.ptr()->yd;
      double by =  b.ptr()->yd;
      double cy =  c.ptr()->yd;
      double aw =  a.ptr()->wd;
      double bw =  b.ptr()->wd;
      double cw =  c.ptr()->wd;
 
      double aybw = ay*bw;
      double byaw = by*aw;
      double axcw = ax*cw;
      double cxaw = cx*aw;
      double axbw = ax*bw;
      double bxaw = bx*aw;
      double aycw = ay*cw;
      double cyaw = cy*aw;
 
      double E = (axbw-bxaw) * (aycw-cyaw) - (aybw-byaw) * (axcw-cxaw);
 
  //---------------------------------------------------------------------------
  // ERROR BOUNDS
  //---------------------------------------------------------------------------
  // mes(E) = 2*(mes(aybw-byaw)*mes(axcw-cxaw) + mes(axbw-bxaw)*mes(aycw-cyaw))
  //        = 2*(4*(fabs(aybw)+fabs(byaw)) * (fabs(axcw)+fabs(cxaw)) +
  //             4*(fabs(axbw)+fabs(bxaw)) * (fabs(aycw)+fabs(cyaw)))
  //        = 8*((fabs(aybw)+fabs(byaw)) * (fabs(axcw)+fabs(cxaw)) + 
  //             (fabs(axbw)+fabs(bxaw)) * (fabs(aycw)+fabs(cyaw)))
  //
  // ind(E) = ((ind(aybw-byaw) + ind(axcw-cxaw) +0.5) +
  //           (ind(axbw-bxaw) + ind(aycw-cyaw) +0.5) + 1 ) / 2
  //        = (4.5 + 4.5 + 1) / 2  =  5
  //
  // eps(E) = ind(E) * mes(E) * eps0
  //        = 40 * ((fabs(aybw)+fabs(byaw))*(fabs(axcw)-fabs(cxaw)) +
  //               (fabs(axbw)-fabs(bxaw))*(fabs(aycw)-fabs(cyaw))) * eps0;
  //---------------------------------------------------------------------------
 
      FABS(aybw);
      FABS(byaw);
      FABS(axcw);
      FABS(cxaw);
      FABS(axbw);
      FABS(bxaw);
      FABS(aycw);
      FABS(cyaw);
 
      double eps = 40*((aybw+byaw)*(axcw+cxaw)+(axbw+bxaw)*(aycw+cyaw))*eps0;
   
      if (E > eps)  return  1;
      if (E < -eps) return -1;
      if (eps < 1)  return  0;
     }
 
     // big integer arithmetic

     rat_point::exact_orient_count++;

     integer AX = a.X(); integer AY = a.Y(); integer AW = a.W();
     integer BX = b.X(); integer BY = b.Y(); integer BW = b.W();
     integer CX = c.X(); integer CY = c.Y(); integer CW = c.W();
     integer D = (AX*BW-BX*AW) * (AY*CW-CY*AW) - (AY*BW-BY*AW) * (AX*CW-CX*AW);
     return sign(D);
}



int incircle(const rat_point& a, const rat_point& b, const rat_point& c, 
                                                     const rat_point& d)
{
  integer AX = a.X();
  integer AY = a.Y();
  integer AW = a.W();

  integer BX = b.X();
  integer BY = b.Y();
  integer BW = b.W();

  integer CX = c.X();
  integer CY = c.Y();
  integer CW = c.W();

  integer DX = d.X();
  integer DY = d.Y();
  integer DW = d.W();

  integer bx,by,bw,cx,cy,cw,dx,dy,dw;

  if (AW==1 && BW==1 && CW==1 && DW==1)
  { bx = BX - AX;
    by = BY - AY;
    bw = bx*bx + by*by;

    cx = CX - AX;
    cy = CY - AY;
    cw = cx*cx + cy*cy;
    
    dx = DX - AX;
    dy = DY - AY;
    dw = dx*dx + dy*dy;
   }
  else
  { integer b1 = BX*AW - AX*BW;
    integer b2 = BY*AW - AY*BW;
    integer c1 = CX*AW - AX*CW;
    integer c2 = CY*AW - AY*CW;
    integer d1 = DX*AW - AX*DW;
    integer d2 = DY*AW - AY*DW;

    bx = b1 * AW * BW;
    by = b2 * AW * BW;
    bw = b1*b1 + b2*b2;

    cx = c1 * AW * CW;
    cy = c2 * AW * CW;
    cw = c1*c1 + c2*c2;

    dx = d1 * AW * DW;
    dy = d2 * AW * DW;
    dw = d1*d1 + d2*d2;
   }

  return sign((by*cx-bx*cy)*dw + (cy*bw-by*cw)*dx  + (bx*cw-cx*bw)*dy);

}


