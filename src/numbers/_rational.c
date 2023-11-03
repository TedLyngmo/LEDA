/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _rational.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#include <LEDA/basic.h>
#include <LEDA/rational.h>
#include <cmath>
#include <cctype>


// hidden functions


rational& rational::normalize()
// divide numerator and denominator by their greatest common divisor
  { // den is assumed to be nonzero and positive
    if (num == den)
      {
        num = den = 1;
	return (*this);
      }
    if (-num == den)
      {
        num = -1; den = 1;
	return (*this);
      }
    integer ggt = gcd(num, den);
    if (ggt != 1)
      {
        num /= ggt;
        den /= ggt;
      };
    return (*this);
  }


rational& rational::simplify(const integer& a)
// divide numerator and denominator by number a
{
  integer r;
  den = den.div(a,r);

  if (r !=0)
    error_handler(1,"rational::simplify: argument does not divide denominator");

  num = num.div(a,r);

  if (r !=0)
    error_handler(1,"rational::simplify: argument does not divide numerator");

  return *this;
}


// Constructors
rational::rational(double x)
  { num = 0; den = 1;

    if (x != 0.0)
    { int neg = (x < 0);
      if (neg) x = -x;

      const unsigned shift = 15;   // a safe shift per step
      const double width = 32768;  // = 2^shift
      const int maxiter = 20;      // ought not be necessary, but just in case,
                                   // max 300 bits of precision
      int expt;
      double mantissa = frexp(x, &expt);
      long exponent = expt;
      double intpart;
      int k = 0;
      while (mantissa != 0.0 && k++ < maxiter) {
        mantissa *= width; // shift double mantissa
        mantissa = modf(mantissa, &intpart);
        num <<= shift;
        num += (long)intpart;
        exponent -= shift;
      }
      if (exponent > 0)
        num <<= (unsigned)exponent;
      else if (exponent < 0)
        den <<= (unsigned)(-exponent);
      if (neg)
        num = -num;
    } // if (x != 0) then
    (*this).normalize();
  }


rational::rational(int n, int d)
  { if (d == 0)
      {
        error_handler(1,"Zero denominator!");
      }
    else
      {
        if (d < 0)
          {
            num = -integer(n);   //wegen reference counting notwendig
            den = -integer(d);
          }
        else
          {
            num = integer(n);
            den = integer(d);
          }
      }
//      (*this).normalize();
  }


rational::rational(const integer& n, const integer& d)
  { if (d.PTR->sign == 0)
      {
        error_handler(1,"Zero denominator!");
      }
    else
      {
        if (d.PTR->sign == -1)
          {
            num = -integer(n);
            den = -integer(d);
          }
        else
          {
            num = integer(n);
            den = integer(d);
          }
      }
//      (*this).normalize();
  }



// Arithmetic Operators

rational& rational::operator+= (const rational& r)
  { num = num * r.den + r.num * den;
    den *= r.den;
//    return (*this).normalize();
    return (*this);
  }

rational& rational::operator-= (const rational& r)
  { num = num * r.den - r.num * den;
    den *= r.den;
//    return (*this).normalize();
    return (*this);
  }

rational& rational::operator*= (const rational& r)
  { num *= r.num;
    den *= r.den;
//    return (*this).normalize();
    return (*this);
  }

rational& rational::operator/= (const rational& r)
  { if (r.num.PTR->sign == 0)
      {
        // r == 0
        error_handler(1,"Division by 0!");
      }
    else
      {
        // r.num != 0
	if (r.num.PTR->sign == -1)
	  {
            num *= -r.den;
            den *= -r.num;
          }
        else
	  {
            num *= r.den;
            den *= r.num;
          }
      }
//    return (*this).normalize();
    return (*this);
  }



// Assignment Operator

rational& rational::operator= (const rational& r)
  { if (this == &r) return *this; // to handle r = r correctly
    num = r.num;
    den = r.den;
    return *this;
  }




// some useful member-functions

void rational::invert()
  { if (num.PTR->sign == 0)
      {
        error_handler(1,"Zero denominator!");
      }
    else
      {
        integer tmp = num;
	if (num.PTR->sign == 1)
	  {
            num = den;
            den = tmp;
          }
        else
	  {
            num = -den;
            den = -tmp;
          }
      }
  }

rational rational::inverse()
  { if (num.PTR->sign == 0)
      {
        error_handler(1,"Zero denominator!");
        return (*this);
      }
    else
      {
        if (num.PTR->sign == 1)
          {
            rational tmp(den,num);
            return tmp;
	  }
        else
	  {
            rational tmp(-den,-num);
            return tmp;
          }
      }
  }



// friend functions

int rational::cmp(const rational& x, const rational& y)
  { int xsign = sign(x.num);
    int ysign = sign(y.num);
    if (xsign == 0) return -ysign;
    if (ysign == 0) return xsign;
    // now (x != 0) && (y != 0)
    int diff = xsign - ysign;
    if (diff == 0)
    { integer leftop  = x.num * y.den;
      integer rightop = y.num * x.den;
      if (leftop < rightop) return -1;
      else return (leftop > rightop);
     }
    else return diff;
  }



/*
int rational::cmp(const rational& x, int y)
{
    int  xsign = sign(x.num), ysign;
    if (y == 0) ysign = 0;

    else ysign = (y > 0) ? 1 : -1;
    if (xsign == 0)
      return -ysign;
    if (ysign == 0)
      return xsign;
    // now (x != 0) && (y != 0)
    return compare(x.num, y*x.den);
    int diff = xsign - ysign;
    if (diff == 0)
      {
        if (x.num < y * x.den)
          return -1;
        else
	  return (x.num > y * x.den);
      }
    else return diff;
}

int rational::cmp(int x, const rational& y)
  {
    int ysign = sign(y.num), xsign;
    if (x == 0) xsign = 0;
    else xsign = (x > 0) ? 1 : -1;
    if (xsign == 0)
      return -ysign;
    if (ysign == 0)
      return xsign;
    // now (x != 0) && (y != 0)
    return compare(x*y.den, y.num);
    int diff = xsign - ysign;
    if (diff == 0) {
      integer  leftop = integer(x) * y.den, rightop = y.num;
      if (leftop < rightop) { return -1; }
      else { return (leftop > rightop); }
    }
    else return diff;
    return 0;
  }

*/

// other useful friend functions

rational pow(const rational& r, int l)
// no need to normalize since num and den are relatively prime
  { rational mul(r), result(1,1);
    if (l < 0) {
      mul = mul.inverse();
      l = -l;
    }
    for (int i = 1; i <= l; i++) {
      result.num *= mul.num;
      result.den *= mul.den;
    }
    return result;
  }

rational pow(const rational& r, integer I)
// no need to normalize since num and den are relatively prime
  { rational mul(r), result(1,1);
    if (sign(I) == -1) {
      mul = mul.inverse();
      I=-I;
    }
    for (integer i = 1; i < I; i++) {
      result.num *= mul.num;
      result.den *= mul.den;
    }
    return result;
  }
/*
rational::operator double() const
  { integer
      numvar = num,
      denvar = den;

    if (numvar == integer(0)) { return 0; }

    const integer MDP = 1000000;    // my_double_precision
    long s = 0;
    integer quot = (numvar / denvar); // integer quotient

    while (abs(quot) < MDP) {
      numvar *= 10;
      s++;
      quot = (numvar / denvar);
    }
    // |quot| > MDP
    // num_new == 10^s * num_old

    while (abs(quot) > MDP) {
      quot /= 10;
      s--;
    }
    // MDP/10 < |quot| < MDP
    // num_old/den_old == quot * 10^{-s}

    double result = (double)longasI(quot);
    // transform integer into double via long

    if (s >= 0) {
      for (int i = 0; i < s; i++) { result /= 10; };
    }
    else {
      for (int i = 0; i > s; i--) { result *= 10; };
    }
    return result;
  };
*/
// floor, ceil und round besser durch schooldiv
integer floor(const rational& r)
  { integer x;
    x = r.num/r.den;
    if ((sign(r.num) == -1) && (r.num%r.den != 0)) x--;
    return x;
  }

integer ceil(const rational& r)
  { integer x;
    x = r.num/r.den;
    if ((sign(r.num) == -1) && (r.num%r.den != 0)) x++;
    return x;
  };

/* Noch ausgeklammert
integer round(const rational& r)
  { integer rem, quot;
    Idiv(quot, rem, r.num, r.den);
    rem <<= 1;
    if (rem >= r.den) {
      if (sign(r.num) >= 0) { quot++; }
      else { quot--; }
    }
    return quot;
  }
*/

std::istream& operator>> (std::istream& in, rational& r)
  { // Format: "r.num / r.den"
    integer rx, ry;
    char c;
    do in.get(c); while (isspace(c));
    in.putback(c);

    in >> rx;

    do in.get(c); while (isspace(c));
    if (c != '/') { error_handler(1,"/ expected"); }

    do in.get(c); while (isspace(c));
    in.putback(c);

    in >> ry;
    r = rational(rx,ry);
    // to guarantee the value is normalized, denominator is nonzero ...
    return in;
  }
