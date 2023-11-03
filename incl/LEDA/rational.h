/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  rational.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_rational_H
#define LEDA_rational_H

/*{\Manpage {rational} {} {Rational Numbers}}*/

#include <LEDA/integer.h>

class rational
{

/*{\Mdefinition An instance $q$ of type \name\ is a rational number
where the numerator and the denominator are both of type
$integer$.}*/

protected:
  integer num; // numerator
  integer den; // denominator, always nonzero and positive


public:

/*{\Mcreation q}*/

  rational();

/*{\Mcreate creates an instance \var\ of type \name.}*/

  rational(double x);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it
with the value of $x$.}*/

  rational(int n);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it
with the value of $n$.}*/

  rational(int m, int n);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes its
numerator with $m$ and its denominator with $n$.}*/

  rational(const integer& a);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it
with the value of $a$.}*/

  rational(const integer& a, const integer& b);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes its
numerator with $a$ and its denominator with $b$.}*/

  rational(const rational&);

  rational& operator= (const rational&);

 ~rational();


/*{\Moperations 2 5}*/

/*{\Mtext
The arithmetic operations $+,\ -,\ *,\ /,\ +=,\
-=,\ *=,\ /=,\ -$(unary), $++,\ --$,  
the comparison operations $<,\ <=,\ >,\ 
>=,\ ==,\ !=$ and the stream operations are all available.}*/


// friend functions, first arithmetic operators

  friend inline rational operator+ (rational q, const rational& r);

/*{\Xfunc returns \var\ $+\ r$.}*/

  friend inline rational operator- (rational q, const rational& r);

/*{\Xfunc returns \var\ $-\ r$.}*/

  friend inline rational operator* (rational q, const rational& r);

/*{\Xfunc returns \var\ $*\ r$.}*/

  friend inline rational operator/ (rational q, const rational& r);
  
/*{\Xfunc returns \var\ $/\ r$.}*/

// unary minus 

  friend inline rational operator- (const rational&);

/*{\Xfunc returns --\var.}*/

  rational& operator+= (const rational& r);

/*{\Xbinop returns \var\ $+\ r$.}*/

  rational& operator-= (const rational&);
  
/*{\Xbinop returns \var\ $-\ r$.}*/

  rational& operator*= (const rational&);

/*{\Xbinop returns \var\ $*\ r$.}*/
  rational& operator/= (const rational&);

/*{\Xbinop returns \var\ $/ r$.}*/

  rational& operator++ ();

/*{\Xunop increments \var\ by $1$.}*/

  rational& operator-- ();

/*{\Xunop decrements \var\ by $1$.}*/

// comparison operators

  static int cmp(const rational&, const rational&);

  friend inline bool operator== (const rational& q, const rational& r);

/*{\Xfunc Equality test.}*/

  friend inline bool operator== (const rational& q, int n);

/*{\Xfunc Equality test.}*/

  friend inline bool operator== (int n, const rational& q);

/*{\Xfunc Equality test.}*/

  friend inline bool operator!= (const rational& q, const rational& r);

/*{\Xfunc Inequality test.}*/

  friend inline bool operator!= (const rational& q, int n);

/*{\Xfunc Inequality test.}*/

  friend inline bool operator!= (int n, const rational& q);

/*{\Xfunc Inequality test.}*/

  friend inline bool operator< (const rational& q, const rational& r);

/*{\Xfunc returns $q < r$.}*/

  friend inline bool operator<= (const rational& q, const rational& r);

/*{\Xfunc returns $q <= r$.}*/

  friend inline bool operator> (const rational& q, const rational& r);

/*{\Xfunc returns $q > r$.}*/

  friend inline bool operator>= (const rational& q, const rational& r);

/*{\Xfunc returns $q >= r$.}*/



  integer numerator() const;
/*{\Mop returns the numerator of $q$.}*/

  integer denominator() const;
/*{\Mop returns the denominator of $q$.}*/

  rational& simplify(const integer& a);
/*{\Mop simplifies $q$ by $a$.\\
        \precond $a$ divides the numerator and the denominator of $q$.}*/

  rational& normalize();
/*{\Mop normalizes \var.}*/



  void negate(); 
/*{\Mop negates \var.}*/

  void invert(); 
/*{\Mop inverts \var.}*/

  rational inverse();  
/*{\Mop returns the inverse of \var.}*/



/*{\Mtext
\smallskip
{\bf Non-member functions}
\smallskip }*/

  friend inline int sign(const rational& q);
/*{\Mfunc returns the sign of $q$.}*/

  friend inline rational abs(const rational& q);
/*{\Mfunc returns the absolute value of $q$.}*/

  friend inline rational sqr(const rational& q);
/*{\Mfunc returns the square of $q$.}*/

  friend inline integer trunc(const rational& q);
/*{\Mfunc returns the $integer$ with the next smaller absolute value.}*/



  friend rational pow(const rational& q, int n); 
/*{\Mfunc returns the $n$-th power of $q$.}*/

  friend rational pow(const rational& q, integer a); 
/*{\Mfunc returns the $a$-th power of $q$.}*/

  friend integer floor(const rational& q);
/*{\Mfunc returns the next smaller $integer$.}*/

  friend integer ceil(const rational& q);
/*{\Mfunc returns the next bigger $integer$.}*/

  friend integer round(const rational& q);
/*{\Mfunc rounds $q$ to the nearest $integer$.}*/







// comparison functions
/*
  friend bool LRge0(const rational&);
  friend bool LRgt0(const rational&);
  friend bool LRle0(const rational&);
  friend bool LRlt0(const rational&);
  friend bool LReq0(const rational&);
  friend bool LReq1(const rational&);
*/


// conversion

//  operator double () const; // rational to double



// input/output

  friend inline istream& operator>> (istream& in, rational& q);

/*{\Xfunc reads \var\ from the istream $in$.}*/

  friend inline ostream& operator<< (ostream& out, const rational& q);

/*{\Xfunc writes \var\ to the ostream $out$.}*/
};



inline int compare(const rational& x, const rational& y)
{ return rational::cmp(x,y); } 


  inline rational::rational()
    { num = 0; den = 1; }

  inline rational::rational(int n)
    { num = integer(n); den = 1; }

  inline rational::rational(const integer& i)
    { num = i; den = 1; }

  inline rational::rational(const rational& r)
    { num = r.num; den = r.den; }

  inline rational::~rational() {}

  inline rational& rational::operator++ ()
    { num += den; return (*this).normalize(); }
  inline rational& rational::operator-- ()
    { num -= den; return (*this).normalize(); }

  inline integer rational::numerator() const
    { return num; }
  inline integer rational::denominator() const
    { return den; }


  inline void rational::negate()
    { num = - num; }

  inline rational operator+ (rational x, const rational& y)
    { return x += y; }
  inline rational operator- (rational x, const rational& y)
    { return x -= y; }
  inline rational operator* (rational x, const rational& y)
    { return x *= y; }
  inline rational operator/ (rational x, const rational& y)
    { return x /= y; }

  inline rational operator- (const rational& x)
    { return rational(-x.num,x.den); }

  inline int sign(const rational& r)
    { return sign(r.num); }

  inline rational abs(const rational& r)
    { if (sign(r.num) > -1) { return r; } else { return -r; } }

  inline rational sqr(const rational& r)
    { return rational(r.num*r.num, r.den*r.den); }

  inline integer trunc(const rational& r)
    { return (r.num / r.den); }


  inline ostream& operator<< (ostream& s, const rational& r)
    {  s << r.num << "/" << r.den; return s; }


  inline bool operator== (const rational& x, const rational& y)
    { return ((x.num == y.num) && (x.den == y.den)); }

  inline bool operator== (const rational& x, int y)
    { return ((x.den==1) && (x.num == integer(y))); }

  inline bool operator== (int x, const rational& y)
    { return ((y.den==1) && (y.num == integer(x))); }

  inline bool operator!= (const rational& x, const rational& y)
    { return ((x.num != y.num) || (x.den != y.den)); }

  inline bool operator!= (const rational& x, int y)
    { return ((x.den != 1) || (x.num != integer(y))); }

  inline bool operator!= (int x, const rational& y)
    { return ((y.den != 1) || (y.num != integer(x))); }

  inline bool operator< (const rational& x, const rational& y)
    { return rational::cmp(x,y) < 0; }

  inline bool operator<= (const rational& x, const rational& y)
    { return rational::cmp(x,y) <= 0; }

  inline bool operator> (const rational& x, const rational& y)
    { return rational::cmp(x,y) > 0; }

  inline bool operator>= (const rational& x, const rational& y)
    { return rational::cmp(x,y) >= 0; }

/*{\Mimplementation A $rational$ is implemented by two $integer$
numbers which represent the numerator and the denominator. The sign
is represented by the sign of the numerator.}*/

#endif
