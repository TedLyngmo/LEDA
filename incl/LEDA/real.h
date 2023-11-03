/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  real.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

/* This file has been automatically generated from "real.w"
   by CTANGLE (Version 3.1 [km2c]) */

#ifndef LEDA_REAL_H
#define LEDA_REAL_H

/*{\Manpage {real} {} {Real Numbers} }*/


#define DEBUG_REAL

/*{\Mdefinition

An instance $x$ of the data type $real$ is an algebraic real number.
There are many ways to construct a $real$:
either by conversion from $double$, $integer$ or $rational$
or by applying one of the arithmetic operators $+, -, *, /$ or $\sqrt{\ } $ to
$real$ numbers.
One may test the sign of a $real$ number or compare two $real$ numbers by
any of the comparison relations $=, \neq, < , \leq, >$ and $\geq$.
The outcome of such a test is {\em exact}.
There is also a non--standard version of the sign function: the call
$x.sign(integer\ q)$ computes the sign of $x$ under the precondition
that $|x|\leq 2^{-q}$ implies $x = 0$.
This version of the sign function allows the user to assist the data type in the
computation of the sign of $x$, cf. the example below.

One can ask for $double$ approximations of a real number $x$.
The calls $x.todouble()$ and $x.get\_double\_error()$ return $double$s $xnum$ and
$xerr$ such that $|xnum - x |\leq  |xnum |* xerr$.
Note that $xerr = \infty$ is possible.
There are also several functions to compute more accurate approximations of
$real$s.
The call $x.get\_precision()$ returns an $integer$ $xerr$ such that the internal
approximation $x\_num$ satisfies $|xnum - x |\leq 2^{-xerr}$.
The user may set a bound on $xerr$.
More precisely, after the call $x.improve(integer\ q)$ the data type guarantees
$xerr \leq 2^{-q}$.
}*/

#include <LEDA/bigfloat.h>
/* this is only a preliminary version of bigfloats */
/* include "/KM/usr/burnikel/cweb/bigfloat/bigint.h" */
#include <LEDA/rational.h>
typedef rational bigrational;
/* we use the name $bigrational$ instead of the LEDA name */
typedef integer bigint;
/* we use the name $bigint$ instead of the LEDA name */

#include <LEDA/basic.h>

extern const bigfloat zero;

class real_rep;
class real
{

real_rep*PTR;


GenPtr copy() const;
void   clear();


public:


/*{\Mcreation x
}*/

real();
/*{\Mcreate
introduces a variable \var\ of type $real$ and initializes it to zero.
}*/

real(double y);
/*{\Mcreate
introduces a variable \var\ of type $real$ and initializes it to the
value of $y$.
}*/

real(int n);
/*{\Mcreate
introduces a variable \var\ of type $real$ and initializes it to the
value of $n$.
}*/

real(const integer& a);
/*{\Mcreate
introduces a variable \var\ of type $real$ and initializes it to the
value of $a$.
}*/

real(const bigfloat&);

real(const rational& q);
/*{\Mcreate
introduces a variable \var\ of type $real$ and initializes it to the
value of $q$
}*/

real(const real&);
real(real_rep&);

~real();

real&operator= (const real&);

/*{\Moperations 1.5 5
}*/

/*{\Mtext
The arithmetic operations $+,\ -,\ *,\ /,\ +=,\
-=,\ *=,\ /=,\ -$(unary), the comparison operations $<,\ <=,\ >,\
>=,\ ==,\ !=$ and the stream operations all are available.
}*/


friend real operator+(const real&,const real&);
/*{\Xbinopfunc addition
}*/
friend real operator-(const real&,const real&);
/*{\Xbinopfunc subtraction
}*/
friend real operator*(const real&,const real&);
/*{\Xbinopfunc multiplication
}*/
friend real operator/(const real&,const real&);
/*{\Xbinopfunc division
}*/
friend real operator-(const real&);
/*{\Xunop negation
}*/
friend real sqrt(const real&);
/*{\Mop squareroot operation.
}*/



friend real& operator+= (real&,const real&);
/*{\Xbinopfunc $\var\ = \var\ + y$
}*/
friend real& operator-= (real&,const real&);
/*{\Xbinopfunc $\var\ = \var\ - y$
}*/
friend real& operator*= (real&,const real&);
/*{\Xbinopfunc $\var\ = \var\ * y$
}*/




friend int operator<(const real&,const real&);
/*{\Xbinopfunc returns true if \var\ is smaller than $y$
}*/
friend int operator<=(const real&,const real&);
/*{\Xbinopfunc returns true if \var\ is smaller than or equal to $y$
}*/
friend int operator>(const real&,const real&);
/*{\Xbinopfunc returns true if \var\ is greater than $y$
}*/
friend int operator>=(const real&,const real&);
/*{\Xbinopfunc returns true if \var\ is greater than or equal to $y$
}*/
friend int operator==(const real&,const real&);
/*{\Xbinopfunc returns true if \var\ is equal to $y$
}*/
friend int operator!=(const real&,const real&);
/*{\Xbinopfunc returns true if \var\ is not equal to $y$
}*/



int sign();
/*{\Mop returns $-1$ if (the exact value of) \var\ $< 0$, $1$ if \var\
   $> 0$, and $0$ if \var\ $= 0$.
}*/

int sign(const integer& a);
/*{\Mop as above. Precondition: if $|\mbox{\var} |\leq  2^{-a}$
   then $\mbox{\var} = 0$.
}*/

int sign(long);

void improve(const integer& a);
/*{\Mop (re-)computes the approximation of \var\ such that its final quality
    is bounded by $a$, i.e., \var.$get\_precision() \geq a$ after the
    call \var.$improve(a)$.
}*/

void improve(long);

void compute_in(long k);
/*{\Mop (re-)computes the approximation of \var; each numerical operation
   is carried out with $k$ binary places.
}*/

void compute_up_to(long k);
/*{\Mop (re-)computes an approximation of \var\ such that the error of the
    approximation lies in the $k$-th binary place.
}*/




double todouble()const;
/*{\Mop returns the current double approximation of \var.
}*/

bigfloat tobigfloat()const;

double get_double_error()const;
/*{\Mop returns the quality of the current double approximation of \var,
    i.e., $|x- x.todouble() |\leq
    x.get\_double\_error() *  |x.todouble() |$.
}*/

integer get_precision()const;
/*{\Mop returns the quality of the current internal approximation $\var.num$
   of \var, i.e., $|x - x.num |\leq 2^{-x.get\_precision()}$.\\
}*/

friend std::ostream& operator<<(std::ostream& O, const real& x);
/*{\Xbinopfunc writes the best known approximation of the $real$
    number $x$ to the output stream $O$.\\
}*/
friend std::istream& operator>>(std::istream& I,real& x);
/*{\Xbinopfunc reads $real$ number $x$ from the input stream $I$
   (in $double$ format).
}*/


#if !defined(__TEMPLATE_FUNCTIONS__)
friend GenPtr  Create(const real*) { real x; return x.copy(); }
friend void    Clear(real& y)      { y.clear();}
friend GenPtr  Convert(real& y)    { return y.PTR; }
friend GenPtr  Copy(const real& y) { return y.copy();}
friend const char*   Type_Name(const real*) { return "real"; }
#endif


};


/*{\Mtext
\smallskip
{\bf Non-member functions}
\smallskip }*/

real fabs(real& x);
/*{\Mfunc absolute value of x.
}*/
real sq(const real& x);
/*{\Mfunc square of x.
}*/
real hypot(const real& x, const real& y);
/*{\Mfunc euclidean distance of vector (x,y) to the origin.
}*/
real powi(const real& x,int n);
/*{\Mfunc n-th power of x.
}*/



//  Functions necessary to use real as a type parameter in parameterized
//  data types.

inline void Print(const real& a, std::ostream & out) { out << a; }
inline void Read(real& a, std::istream & in) { in >> a; }

inline int compare(const real& x, const real& y)
{ return (x < y) ? -1 : ((x > y) ? 1 : 0); }




/*{\Mimplementation

A \name\ is represented by the expression which defines it and a $double$
approximation $\hat{x}$ together with a relative error bound
$\epsilon_{x}$.
The arithmetic operators $+, -, *, /, \sqrt{\ }$ take constant time.
When the sign of a \name\ number needs to be determined, the data type first
computes a number $q$, if not already given as an argument to $sign$, such that
$|\mbox{\var} |\leq  q$ implies $x = 0$.
The bound $q$ is computed as described in \cite{Mignotte:Buch}.
The data type then computes an internal approximation $xnum$ for \var\ with
error bound $xerr \leq q$.
The sign of $xnum$ is then returned as the sign of \var.

Two shortcuts are used to speed up the computation of the sign.
Firstly, if the $double$ approximation already suffices to determine the sign,
then no further approximation is computed at all.
Secondly, the internal approximation is first computed only with small
precision.
The precision is then doubled until either the sign can be decided (i.e.,
if $xerr < |xnum |$) or the full precision $q$ is reached.\\
}*/

/*{\Mexample
Examples can be found in  \cite{BMS:ESA}.
}*/






#endif


