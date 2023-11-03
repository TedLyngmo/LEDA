/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  bigfloat.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


/* This file has been automatically generated from "bigfloat.w"
   by CTANGLE (Version 3.1 [km2c]) */

/*{\manual\\
\declare |bigfloat|
}*/

#ifndef BIGFLOAT_H
#define BIGFLOAT_H

#include <LEDA/integer.h>
/* include "/KM/usr/burnikel/cweb/bigfloat/integer.h" */
/* we use the LEDA data type integer to for th representaion of the bigfloats */



enum rounding_modes {
  TO_NEAREST, TO_ZERO, TO_P_INF, TO_N_INF, TO_INF, EXACT, DEFAULT
};



class bigfloat {
  integer significant, exponent;
  long precision;

 public:

/*{\manual\\
\creation

a) \create x  (|double|\ d)

b) \create x  (|integer|\ I)

c) \create x  (|long|\ I)

d) \create x  (|int|\ I)

e) \create x  (|integer|\ significant,|integer|\ exponent)

f) \create x  ()

introduces a variable \var\ of type \name\ and initializes it to a given
value (zero in variant e).

}*/

   bigfloat(const integer &, const integer &);
   bigfloat(double = 0);
   bigfloat(const integer &);
   bigfloat(long);
   bigfloat(int);
   bigfloat(const bigfloat &);

   ~bigfloat(void) {
  }

   bigfloat & operator = (const bigfloat &);

/*{\manual\\
\operations 2 4.5
}*/

  double todouble(void) const;
/*{\manual\\
\op double  todouble { }
      {converts the bigfloat into a double value and returns it }
}*/

  integer tointeger(rounding_modes = TO_ZERO) const;
/*{\manual\\
\op double  tointeger {rmode}
      {converts the bigfloat into a integer value and returns it. If rmode
       is ommitted tointeger uses default round mode. }
}*/

  void set_precision(long, rounding_modes = DEFAULT);
/*{\manual\\
\op void set\_precision {prec}
      {sets the internal significant's precision to prec
       and rounds the significant to that value if the old
       precision was bigger else it shifts it to the wanted length}

}*/

  long get_precision(void);
/*{\manual\\
\op long get\_precision { }
      {returns the current precision of the bigfloat }
}*/

  bigfloat round(long = 0, rounding_modes = DEFAULT);
/*{\manual\\
\op |bigfloat| round {len,rmode}
      {rounds the bigfloat to length len and uses a specific
       round mode rmode or the default one. round returns
       the rounded bigfloat}
}*/

  integer get_exponent() const {
    return exponent;
  }
/*{\manual\\
\op |integer| get\_exponent { }
      {returns the exponent}
}*/

  integer get_significant() const {
    return significant;
  }
/*{\manual\\
\op |integer| get\_significant { }
      {returns the significant}
}*/




  friend bigfloat add(const bigfloat &, const bigfloat &, long = -1, rounding_modes = DEFAULT);
/*{\manual\\
\func |bigfloat| add {x,y,prec,rmode}
      {returns $x+y$ computed to the precision prec and rounded in
       mode rmode}
}*/
  friend bigfloat sub(const bigfloat &, const bigfloat &, long = -1, rounding_modes = DEFAULT);
/*{\manual\\
\func |bigfloat| sub {x,y,prec,rmode}
      {returns $x-y$ computed to the precision prec and rounded in
       mode rmode}
}*/
  friend bigfloat mul(const bigfloat &, const bigfloat &, long = -1, rounding_modes = DEFAULT);
/*{\manual\\
\func |bigfloat| mul {x,y,prec,rmode}
      {returns $x*y$ computed to the precision prec and rounded in
       mode rmode}
}*/

  friend bigfloat div(const bigfloat &, const bigfloat &, long = -1, rounding_modes = DEFAULT);
/*{\manual\\
\func |bigfloat| div {x,y,prec,rmode}
      {returns $x/y$ computed to the precision prec and rounded in
       mode rmode}
}*/

  friend bigfloat sqrt(const bigfloat &, long = -1, rounding_modes = DEFAULT);
/*{\manual\\
\func |bigfloat| sqrt {x,prec,rmode}
      {returns $\sqrt{x}$ computed to the precision prec and rounded in
       mode rmode}
}*/




  friend bigfloat operator + (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop |bigfloat| + {y}
      { returns add(\var,y) and uses so default values for precision and
        round mode}
}*/

  friend bigfloat operator - (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop |bigfloat| - {y}
      { returns sub(\var,y) }
}*/

  friend bigfloat operator *(const bigfloat &, const bigfloat &);
/*{\manual\\
\binop |bigfloat| * {y}
      { returns mul(\var,y) }
}*/

  friend bigfloat operator / (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop |bigfloat| / {y}
      { returns div(\var,y) }
}*/

  friend bigfloat operator - (const bigfloat &);
/*{\manual\\
\unop |bigfloat| -
      { returns $-x$ }
}*/





  friend bool operator == (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop bool == {y}
      { returns true if \var is equal to y }
}*/

  friend bool operator != (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop bool != {y}
      { returns true if \var is not equal to y }
}*/

  friend bool operator > (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop bool > {y}
      { returns true if \var is greater than y }
}*/

  friend bool operator >= (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop bool >= {y}
      { returns true if \var is greater than or equal to y }
}*/

  friend bool operator < (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop bool < {y}
      { returns true if \var is smaller than y }
}*/

  friend bool operator <= (const bigfloat &, const bigfloat &);
/*{\manual\\
\binop bool <= {y}
      { returns true if \var is smaller than or equal to y }
}*/




  friend bool isNaN(const bigfloat &);
/*{\manual\\
\func bool isNaN {x}
      { returns true if x is a NaN}
}*/

  friend bool isnInf(const bigfloat &);
/*{\manual\\
\func bool isnInf {x}
      { returns true if x is $-\infty$}
}*/

  friend bool ispInf(const bigfloat &);
/*{\manual\\
\func bool ispInf {x}
      { returns true if x is $+\infty$}
}*/

  friend bool isnZero(const bigfloat &);
/*{\manual\\
\func bool isnZero {x}
      { returns true if x is $-0$}
}*/

  friend bool ispZero(const bigfloat &);
/*{\manual\\
\func bool ispZero {x}
      { returns true if x is $+0$}
}*/

  friend bool isZero(const bigfloat &);
/*{\manual\\
\func bool isZero {x}
      { returns true if x is $\pm 0$}
}*/

  friend bool isInf(const bigfloat &);
/*{\manual\\
\func bool isInf {x}
      { returns true if x is $\pm \infty$}
}*/


/*{\manual\\
\implementation

A \type\ is a floating point number of arbitrary precision.\type s are
represented by a significant and an exponent which are |integers|.
They are defined on addition, subtraction, multiplication, division
and square root. Furthermore the user is given some comparison functions on
\type s. The user is also provided with a couple of mathematical functions.
In addition to that the user is given a rounding function that performs
an exact rounding in different modes. The rounding modes conform to IEEE
standard.
}*/



  friend integer ceil(const bigfloat & b);
/*{\manual\\
\func |integer| ceil { }
      { rounds \var\ up to the next integer value}
}*/

  friend integer floor(const bigfloat & b);
/*{\manual\\
\func |integer| floor { }
      { rounds \var\ down to the next integer value}
}*/

  friend bigfloat pow2(const integer &);
/*{\manual\\
\func |bigfloat| pow2 { y }
      { returns $2^y$}
}*/

  friend long sign(const bigfloat &);
/*{\manual\\
\func long sign { }
      { if \var\ is negative sign return -1, if it is zero it returns zero else
       it returns 1}
}*/

  friend long non_zero_sign(const bigfloat &);
/*{\manual\\
\func long non\_zero\_sign { y }
      { returns a sign either 1 or -1 as values. This function is needed
        for special cases in which a signed zero can appear.}
}*/

  friend bigfloat fabs(const bigfloat &);
/*{\manual\\
\func |bigfloat| fabs { }
      { returns $|\var\|$}
}*/



  friend bigfloat outofchar(char *);
/*{\manual\\
\func |bigfloat| outofchar { string }
      { this routine converts a string of the format $\pm 0.bb\ldots bbEex$ or
        $\pm d.ddd\ldots$, where $b\in [0,1]$ and $d\in [0,9]$into |bigfloat|,
        to |bigfloat| format}
}*/

  friend ostream & operator << (ostream &, const bigfloat &);
/*{\manual\\
\strop ostream\& << {O}
    { writes |real| number \name\ to the output stream $O$ }
}*/

  friend istream & operator >> (istream &, bigfloat &);
/*{\manual\\
\strop ostream\& >> {I}
    { reads |real| number \name\ from the output stream $I$ (in |double|
      format) }
}*/

  void print_contents(void) const;
};
#endif

const long BIN_OUT = 0;
const long DEC_OUT = 1;
const long MAX_PREC = -1;



const long negative = 0;
const long positive = 1;
const long bin_maxlen = 10000;
const bigfloat NaN = bigfloat(0, 1026);	/* the representation of NaN is does */
/* not conform to IEEE because there */
/* are no maximum exponents */
const bigfloat pInf = bigfloat(0, 1024);
const bigfloat nInf = bigfloat(0, 1025);
const bigfloat pZero = bigfloat(0, -1023);
const bigfloat nZero = bigfloat(0, -1024);



/* globale Variablen */

extern long global_prec;
/* extern long output; */
extern rounding_modes round_mode;
extern const double Infinity;

/* globale Funktionen */

void set_glob_prec(long);
void set_round_mode(rounding_modes = TO_NEAREST);
