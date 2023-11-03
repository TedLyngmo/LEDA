/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _bigfloat.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


/* This file has been automatically generated from "bigfloat.w"
   by CTANGLE (Version 3.1 [km2c]) */




#include <LEDA/bigfloat.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>




void binout(std::ostream & os, integer);
void exact_decimal_output(std::ostream &, bigfloat, long = MAX_PREC);
void decimal_output(std::ostream &, bigfloat);
integer powl(const integer &, long);
bigfloat dec_round(bigfloat, long);
integer b_round(const integer &, long, rounding_modes = TO_NEAREST);
void cut(integer &, long);
long bcomp(integer, integer);
long sigcomp(integer, integer);
long test_for_one(integer, long, long);
inline long get_bit(const integer &, long);
inline void set_bit(long &, long);
double compose_parts(long, long, long, long);
double pow2(long);




const double double_min = pow2(long(-1022));
const double NaN_double = compose_parts(0, 2047, 0, 1);
const double pInf_double = compose_parts(0, 2047, 0, 0);
const double Infinity = pInf_double;
const double nInf_double = -pInf_double;
const double pZero_double = compose_parts(0, 0, 0, 0);
const double nZero_double = compose_parts(1, 0, 0, 0);

const integer integer_1 = integer(1);
const integer integer_52 = (integer_1 << 52) - integer_1;
const integer integer_32 = (integer_1 << 32) - integer_1;
const integer integer_20 = (integer_1 << 20) - integer_1;

rounding_modes round_mode = TO_NEAREST;

long global_prec = 52;
rounding_modes global_mode;
bool buf_exact;
long output = DEC_OUT;




void binout(std::ostream & os, integer b)
{
  char temp[bin_maxlen];
  long flag = 0, count = 0;

  if (b < 0)
    b *= -1;

  do {
    temp[count++] = (char) (b % 2).tolong() + '0';
    if (b <= 1)
      flag = 1;
    else
      b /= 2;
  } while (!flag);
  for (long i = count - 1; i >= 0; i--)
    os << temp[i];
}

integer b_round(const integer & b, long digits, rounding_modes mode)
{
  bigfloat bb(b, 0);
  bb.round(digits, mode);
  return bb.get_significant();
}

void cut(integer & b, long prec)
{
  if (b.length() > prec)
    b = (b >> (b.length() - prec));
}

long sigcomp(integer s1, integer s2)
{
  if ((s1 == 0) && (s2 == 0))
    return 0;
  if (sign(s1) != sign(s2)) {
    if (sign(s1) == 0)
      if (sign(s2) < 0)
	return 1;
      else
	return -1;
    if (sign(s2) == 0)
      if (sign(s1) > 0)
	return 1;
      else
	return -1;

    if (s1 > 0)
      return 1;
    else
      return -1;
  }

  return bcomp(s1, s2);
}

long bcomp(integer b1, integer b2)
{
  if (b1.length() > b2.length())
    b2 = b2 << (b1.length() - b2.length());
  else if (b1.length() < b2.length())
    b1 = b1 << (b2.length() - b1.length());

  if (b1 > b2)
    return 1;
  if (b1 < b2)
    return -1;
  return 0;
}

long test_for_one(integer b, long start, long end)
{
  long len = b.length();

  if (end > len)
    end = len;

  for (long i = start; i <= end; i++) {
    if (len != i) {
      if (abs((b >> (len - i)) & 1) == 1)
	return 1;
    }
    else {
      if (abs(b & 1) == 1)
	return 1;
    }

  }
  return 0;
}

long get_bit(const integer & b, long digit)
{
  return abs(int (((b >> (b.length() - digit)) & 1).tolong()));
}

void set_bit(long &b, long digit)
{
  if (digit < 32)
    b = b & (long) pow2(32 - digit);
}

void set_globprec(long p)
{
  global_prec = p;
}

void set_round_mode(rounding_modes m)
{
  round_mode = m;
}

bigfloat dec_round(bigfloat b, long prec)
{
/* calculate needed binary accuracy for operations div and mul */
  long flag = 0, b_prec = (long) ((prec + 2) / log10(2)) + 1;

/* keep sign */
  if (sign(b) == -1) {
    flag = 1;
    b = fabs(b);
  }

/* decimal shift of b */
  int i;
  for (i = 0; i < prec; i++)
    b = mul(b, 10, b_prec);

/* test whether we have to round up or not */
  bigfloat a = b.tointeger() + integer(1);
  if ((a - b) <= 0.5)
    b = bigfloat(a.tointeger(), b.get_exponent());
  else
    b = bigfloat(b.tointeger(), b.get_exponent());

/* decimal shiftback */
  for (i = 0; i < prec; i++)
    b = div(b, 10, b_prec);

/* sign correction */
  if (flag)
    b = (-b);

  return b;
}



/*
void exact_decimal_output (std::ostream &os,bigfloat b,long prec)
{
  if (sign(b)==-1)
  {
    os << "-";
    b=fabs(b);
  }

  long max_pr=(long)(b.get_precision()*log10(2));

  if ((prec>max_pr)||(prec==MAX_PREC))
    prec=max_pr;

  if (prec<max_pr)
    b=dec_round(b,prec);

  os << b.tointeger() << ".";
  b=sub(b,b.tointeger(),1,EXACT);

  for (prec;prec>0;prec--)
  {
    b=mul(b,10,1,EXACT);
    os << b.tointeger();
    b=sub(b,b.tointeger(),1,EXACT);
  }
}
*/


void decimal_output(std::ostream & os, bigfloat b)
{
  if (!b.get_exponent().islong())
    error_handler(1, "decimal_output: not implemented for large exponents");

  if (sign(b) < 0) {
    b = -b;
    os << "-";
  }

  integer int_part = b.tointeger(TO_N_INF);
  os << int_part;

  bigfloat rest = sub(b, bigfloat(int_part), 32, EXACT);
  if (rest == 0)
    return;

  integer least_prec = abs(rest.get_exponent()) + integer(rest.get_precision());
  if (!least_prec.islong())
    error_handler(1, "decimal_output: not implemented for large precisions");

  os << ".";

  long mul_log = (long) ceil(log10(2) * least_prec.tolong());
  integer pow10 = powl(integer(10), mul_log);
  bigfloat multiplicator = pow10;

  bigfloat rest_sig = mul(rest, multiplicator, 32, EXACT);
  os << rest_sig.tointeger(TO_NEAREST);

  long dec_exp = (long) ceil(log10(2) * rest.get_exponent().tolong());
  if (dec_exp != 0)
    os << "E" << dec_exp;
}


integer powl(const integer & x, long n)
{
  integer y = x, z = 1;
  long n_prefix = n;
  while (n_prefix > 0) {
    if (n_prefix % 2)
      z = z * y;
    n_prefix = n_prefix / 2;
    y = y * y;
  }
  return z;
}







double pow2(long exp)
{
  return compose_parts(0, exp + 1023, 0, 0);
}

double compose_parts(long sign_1, long exp_11, long most_sig_20, long least_sig_32)
{
  double a;
  long *p;
  p = (long *) &a;

  long highword = 0, lowword = 0;

/* calculate highword */
  if (sign_1)
    highword = highword | 0x80000000;
  exp_11 = exp_11 << 20;
  highword = highword | exp_11;
  highword = highword | most_sig_20;

/* calculate lowword */
  lowword = least_sig_32;

/* put the word together */
  (*p) = highword;
  p++;
  (*p) = lowword;

  return a;
}



bigfloat outofchar(char *rep)
{
/* There are in principle two possible formats for the representation
     of a float:   1) +/-abc.def
                   2) +/-0.0100111e+/-11110
  */

  bigfloat temp;

/* check whether first character is a sign */

  long sign = positive;
  if (rep[0] == '-') {
    sign = negative;
    rep++;
  }
  if (rep[0] == '+')
    rep++;

/* the big branch into the two main cases */

  if (rep[0] == '0') {
/* scan rep-string until point is passed */

    long flag = 0;
    while (!flag)
      if (*(rep++) == '.')
	flag = 1;

/* scan  significant up to 'e'-character */

    long count = 0;
    flag = 0;
    while (!flag)
      if ((rep[count++] == 'e') || (rep[count - 1] == 'E'))
	flag = 1;
    count--;

/* Now we know how many digits we have in the  significant */
/* It is now possible two calculate our  significant */

    long i;
    for (i = 0; i < count; i++)
      temp.significant += (long) ((*(rep++) - '0') * pow2(count - 1 - i));

/* skip e/E */
    rep++;

/* scan exponent */

/* check for sign */

    long s = positive;
    if (((*rep) == '-') || ((*rep) == '+'))
      if ((*rep++) == '-')
	s = negative;

    count = strlen(rep);
    for (i = 0; i < count; i++)
      temp.exponent += (long) ((*(rep++) - '0') * pow2(count - 1 - i));

    if (s == negative)
      temp.exponent *= (-1);
  }
  else {
    long pr = strlen(rep) * 10;

/* scan number up to decimal point */

    while ((*rep) != '.')
      temp = add(mul(temp, 10, pr), (*(rep++) - '0'), pr);
    rep++;

    integer dd = 10;
    while ((*rep) != 0) {
      temp = add(temp, div(*(rep++) - '0', bigfloat(dd), pr), pr);
      dd *= 10;
    }
  }
  if (sign == negative)
    temp.significant *= -1;

  temp.precision = temp.significant.length();
  return temp;
}






bigfloat::bigfloat(const integer & m, const integer & e)
{
  significant = m;
  exponent = e;
  precision = significant.length();

  if ((significant == 0) && (exponent != 1024) && (exponent != 1025) && (exponent != 1026)
    && (exponent != -1023) && (exponent != -1024))
    exponent = -1023;
}
bigfloat::bigfloat(const bigfloat & b)
{
  significant = b.significant;
  exponent = b.exponent;
  precision = b.precision;
}
bigfloat::bigfloat(double d)
{
  if (d != 0) {
/* If d is lower than double\_min we can be sure that d is denormalized.
    Thus we can multiply d with $2^{1023}$ to get a normalized number. */

    long flag = 0;
    if (fabs(d) < double_min) {
      d = d * pow2(long(1023));
      flag = 1;
    }

    unsigned long mh = 0, ml = 0, sign = 0;

    long e;
    long *p;

/* convert double a pointer to an integer */
    p = (long *) &d;

/* let mh represent the highword of the 64-bit field of the double */
    mh = *p;
    p++;

/* let ml hold the least significant 32 digits of d */
    ml = *p;

/* binary and to get the 11 bits of the exponent */
    e = mh & 0x7ff00000;
/* shift the result to get the right value */
    e = e >> 20;
/* unbias the exponent */
    e -= 1023;

    e++;			/* implicit one */

/* get the sign */
    sign = mh & 0x80000000;
    sign = sign >> 31;

/* get the significant bits out of the highword */
    mh = mh & 0x000fffff;
/* we have ensured that d is normalized $\Rightarrow$ add the leading one
       bit */
    mh = mh | 0x00100000;

    exponent = integer(e);

/* check if d was denormalized */
    if (flag)
      exponent -= 1023;

/* compose significant */
    significant = integer(mh);
    significant = significant << 32;
    significant = significant + integer(ml);

    if (sign == 1)
      significant = significant * integer(-1);
    precision = significant.length();

    if ((exponent == 1025) && (significant != 0)) {	/* NaN-Case */
      significant = 0;
      exponent = 1026;
    }
    if ((exponent == 1025) && (significant == 0)) {	/* Inf-cases */
      if (sign == 1)
	exponent = 1025;
      else
	exponent = 1024;
      significant = 0;
    }
    if (exponent == -1022) {	/* Zero-cases */
      if (sign == 1)
	exponent = -1024;
      exponent = -1023;
    }
  }
  else {
    significant = 0;
    exponent = 0;
    precision = 0;
  }
}
bigfloat::bigfloat(const integer & b)
{
  significant = b;
  if (b == 0)
    exponent = -1023;
  else
    exponent = significant.length();
  precision = exponent.tolong();
}
bigfloat::bigfloat(long a)
{
  significant = a;
  if (a == 0)
    exponent = -1023;
  else
    exponent = significant.length();
  precision = exponent.tolong();
}
bigfloat::bigfloat(int a)
{
  significant = a;
  if (a == 0)
    exponent = -1023;
  else
    exponent = significant.length();
  precision = exponent.tolong();
}






void bigfloat::set_precision(long prec, rounding_modes mode)
{

  if (mode != EXACT) {
    if (prec > precision)
      significant = significant << (prec - precision);
    else if (precision > prec) {
/* if the new precision is lower then the old one we firstly have to
         round the old  significant to the new precision */

      round(prec, mode);
    }
    precision = prec;
  }
}



long bigfloat::get_precision(void)
{
  return precision;
}



integer bigfloat::tointeger(rounding_modes rmode) const
{
  if ((isNaN(*this)) || (isInf(*this)))
    error_handler(1, "bigfloat::tointeger : special values cannot be converted to integer");
  if (isZero(*this))
    return integer(0);

  return b_round(significant, exponent.tolong(), rmode);
}



double bigfloat::todouble(void) const
{
  long s = 0;			/* sign bit of returned value */
  long t_exp = 0;		/* exponent of returned value */
  integer t_sig = 0;		/* significant of returned value */



/* special case checking */

  if (isNaN(*this))
    return NaN_double;
  if (ispInf(*this))
    return pInf_double;
  if (isnInf(*this))
    return nInf_double;
  if (ispZero(*this))
    return pZero_double;
  if (isnZero(*this))
    return nZero_double;





  long denorm = 0;

  if (exponent > 1024)
    return non_zero_sign(*this) * pInf_double;
  if (exponent < -1074)
    return non_zero_sign(*this) * pZero_double;
  if (exponent < -1021)
    denorm = 1;




  if (!denorm) {
    t_exp = exponent.tolong() + 1023 - 1;
    s = sign(significant);

/* significant's length has to be at least 53 digits */
    if (significant.length() < 53)
      t_sig = significant << (53 - significant.length());
    else
      t_sig = significant;

/* Vorz positiv, weil integer-\& auch negatives zur"uckgibt */
    if (s == -1)
      t_sig = integer(-1) * t_sig;

/* remove leading one */
    t_sig = b_round(t_sig, 53) & integer_52;
  }


  else {
    t_exp = 0;

/* significant's length has to be at least 52 digits */
    if (significant.length() < 52)
      t_sig = significant << (52 - significant.length());
    else
      t_sig = b_round(significant, 52);

    t_sig = t_sig >> (-1022 - exponent.tolong());
  }






  double a;

  long sign, h_sig, l_sig;

  sign = (s == (-1));

  h_sig = ((t_sig >> 32) & integer_20).tolong();
  l_sig = (t_sig & integer_32).tolong();

  a = compose_parts(sign, t_exp, h_sig, l_sig);

  return a;


}



bigfloat bigfloat::round(long digits, rounding_modes mode)
{
  if (digits <= 0) {
    (*this) = pZero;
    return pZero;
  }

  rounding_modes m;
  if (mode == DEFAULT)
    m = round_mode;
  else
    m = mode;

  if (digits < significant.length()) {
    if (m == TO_NEAREST) {


      integer temp = significant;

      cut(temp, digits + 1);

/* first case: the next digit is zero and therefor temp >> 1 is right */
      if (abs(temp & 1) == 0) {
	significant = (temp >> 1);
      }
      else {
/* the leftmost digit after the rounding digits is a one */
/* check if there occur other ones in the following digits */

	long flag = test_for_one(significant, digits + 2, significant.length());

/* if a one-digit occurred round up */
	if (flag) {
	  cut(significant, digits);
	  if (significant > 0)
	    significant += 1;
	  else
	    significant -= 1;

	}
	else {
	  cut(significant, digits);
	  if (abs(significant & 1) == 1) {
/* same as above */

	    if (significant > 0)
	      significant += 1;
	    else
	      significant -= 1;

	  }
	  else
	    significant = temp;
	}
      }

      if (significant.length() > digits)
	cut(significant, digits);

/* set new precision */
      precision = digits;

      return (*this);


    }

    if (m == TO_ZERO) {


      cut(significant, digits);

/* set new precision */
      precision = digits;

      return (*this);


    }

    if (m == TO_INF) {


      if (significant > 0)
	return round(digits, TO_P_INF);
      if (significant < 0)
	return round(digits, TO_N_INF);
      return (*this);


    }

    if (m == TO_P_INF) {


      long flag = test_for_one(significant, digits + 1, significant.length());

      cut(significant, digits);
      if ((significant > 0) && (flag)) {
	significant = significant + integer(1);
	if (significant.length() > digits)
	  cut(significant, digits);
      }

/* set new precision */
      precision = digits;

      return (*this);


    }

    if (m == TO_N_INF) {


      long flag = test_for_one(significant, digits + 1, significant.length());

      cut(significant, digits);
      if ((significant < 0) && (flag)) {
	significant = significant - integer(1);
	if (significant.length() > digits)
	  cut(significant, digits);
      }

/* set new precision */
      precision = digits;

      return (*this);


    }
  }
  return (*this);
}



bigfloat fabs(const bigfloat & b)
{
  if (isNaN(b))
    return NaN;
  if ((isnInf(b)) || (ispInf(b)))
    return pInf;
  if ((isnZero(b)) || (ispZero(b)))
    return pZero;

  return bigfloat(abs(b.significant), b.exponent);
}



integer ceil(const bigfloat & b)
{
  return b.tointeger(TO_P_INF);
}



integer floor(const bigfloat & b)
{
  return b.tointeger(TO_N_INF);
}



bigfloat pow2(const integer & p)
{
  return bigfloat(1, p + integer(1));
}



long sign(const bigfloat & b)
{
  return sign(b.significant);
}



long non_zero_sign(const bigfloat & b)
{
  if ((ispInf(b)) || (ispZero(b)))
    return 1;
  if ((isnInf(b)) || (isnZero(b)))
    return -1;
  return sign(b);
}



void bigfloat::print_contents(void) const
{
  integer s = significant, e = exponent;
  std::cout << "Significant: ";
  if (s < 0) {
    std::cout << "-";
    s = s * integer(-1);
  }
  binout(std::cout, s);
  std::cout << "\n";
  std::cout << "Exponent: ";
  if (e < 0) {
    std::cout << "-";
    e = e * integer(-1);
  }
  binout(std::cout, e);
  std::cout << "\n";
}




bigfloat add(const bigfloat & aa, const bigfloat & bb, long prec, rounding_modes mode)
{


  if ((isNaN(aa)) || (isNaN(bb)))
    return NaN;

  if ((ispInf(aa)) || (isnInf(aa))) {
    if ((bb != pInf) && (bb != nInf))
      return aa;
    return NaN;
  }

  if ((ispInf(bb)) || (isnInf(bb))) {
    if ((aa != pInf) && (aa != nInf))
      return bb;
    return NaN;
  }

  if (sign(aa) == 0)
    return bb;
  if (sign(bb) == 0)
    return aa;



  bigfloat a = aa, b = bb, c;
  if (prec == -1)
    prec = global_prec;

/* We firstly have to adjust the significants' precisions to
     guaranty an exact result */

  if (a.precision > b.precision)
    b.set_precision(a.precision);
  if (b.precision > a.precision)
    a.set_precision(b.precision);

/* now we have to branch into 3 main cases */

  if (a.exponent == b.exponent) {


    long temp1, temp2;

    c.significant = a.significant + b.significant;

    temp1 = c.significant.length();
    temp2 = a.significant.length();
    c.exponent = a.exponent + integer(temp1 - temp2);
    c.precision = c.significant.length();

    c.set_precision(prec, mode);

    if (c.significant == 0)
      c = pZero;

    return c;


  }
  if (a.exponent > b.exponent) {


    integer temp = a.exponent - b.exponent;
    a.significant = (a.significant << temp.tolong());
    c.significant = a.significant + b.significant;

    c.exponent = a.exponent;
    if (c.significant.length() != a.significant.length())
      c.exponent += (c.significant.length() - a.significant.length());

    c.precision = c.significant.length();
    c.set_precision(prec, mode);

    if (c.significant == 0)
      c = pZero;

    return c;


  }



/* siehe oben */
  integer temp = b.exponent - a.exponent;
  b.significant = (b.significant << temp.tolong());
  c.significant = a.significant + b.significant;

  c.exponent = b.exponent;

  if (c.significant.length() != b.significant.length())
    c.exponent += (c.significant.length() - b.significant.length());

  c.precision = c.significant.length();
  c.set_precision(prec, mode);

  if (c.significant == 0)
    c = pZero;

  return c;


}



bigfloat sub(const bigfloat & a, const bigfloat & b, long prec, rounding_modes mode)
{
  bigfloat bb(-b.significant, b.exponent);
  return add(a, bb, prec, mode);
}



bigfloat mul(const bigfloat & aa, const bigfloat & bb, long prec, rounding_modes mode)
{
  if (prec == -1)
    prec = global_prec;

  if ((isNaN(aa)) || (isNaN(bb)))
    return NaN;

  if (((ispZero(aa)) || (isnZero(aa))) && (!isInf(bb))) {
    if (bb > 0)
      return aa;
    if (ispZero(aa))
      return nZero;
    return pZero;
  }
  if (((ispZero(bb)) || (isnZero(bb))) && (!isInf(aa))) {
    if (aa > 0)
      return bb;
    if (ispZero(bb))
      return nZero;
    return pZero;
  }

  if (ispInf(aa)) {
    if (bb > 0)
      return pInf;
    if (bb < 0)
      return nInf;
    return NaN;
  }
  if (isnInf(aa)) {
    if (bb > 0)
      return nInf;
    if (bb < 0)
      return pInf;
    return NaN;
  }
  if (ispInf(bb)) {
    if (aa > 0)
      return pInf;
    if (aa < 0)
      return nInf;
    return NaN;
  }
  if (isnInf(bb)) {
    if (aa > 0)
      return nInf;
    if (aa < 0)
      return pInf;
    return NaN;
  }



  bigfloat c;

  c.significant = aa.significant * bb.significant;
  c.exponent = aa.exponent + bb.exponent;
  c.exponent -= (aa.significant.length() + bb.significant.length()) - c.significant.length();

  c.precision = c.significant.length();
  c.set_precision(prec, mode);

  return c;


}



bigfloat div(const bigfloat & aa, const bigfloat & bb, long prec, rounding_modes mode)
{
/* special checking for zero-cases */
  if (prec == -1)
    prec = global_prec;



  if ((isNaN(aa)) || (isNaN(bb)))
    return NaN;
  if (((isZero(aa)) && (isZero(bb))) || ((isInf(aa)) && (isInf(bb))))
    return NaN;
  if ((isInf(aa)) || (isZero(bb)))
    return (non_zero_sign(aa) * non_zero_sign(bb) * pInf);
  if ((isZero(aa)) || (isInf(bb)))
    return (non_zero_sign(aa) * non_zero_sign(bb) * pZero);






  bigfloat a = aa, c;
  integer temp;

  long d = prec + bb.significant.length() - a.significant.length() + 2;

  if (d > 0)
    a.significant = a.significant << d;;




  c.significant = a.significant / bb.significant;
  c.exponent = a.exponent - bb.exponent;
  c.precision = prec;

/* $\delta$-Test */
  if (bcomp(fabs(a).significant, fabs(bb).significant) == 1)
    c.exponent += 1;

/* cut the significant to prec+1 digits */
  cut(c.significant, prec + 1);



  if (mode == DEFAULT)
    mode = round_mode;
  switch (mode) {
      case TO_NEAREST:


	if (abs(c.significant & 1) == 0) {
	  c.significant = (c.significant >> 1);
	  return c;
	}
	else {
/* now we have to check whether the remainder is zero */

	  bigfloat temp = mul(c, bb, 1, EXACT);
	  temp = mul(c, bb, 1, EXACT);

	  if (temp == aa)
	    return c.round(prec, TO_NEAREST);
	  return c.round(prec, TO_INF);
	}


      case TO_ZERO:


	c.significant = c.significant >> 1;
	return c;


      case TO_P_INF:


	if (mul(c, bb, 1, EXACT) == aa)
	  return c.round(prec, TO_ZERO);
	if (c.significant > 0)
	  return c.round(prec, TO_INF);
	if (c.significant < 0)
	  return c.round(prec, TO_ZERO);
	return c;


      case TO_N_INF:


	if (mul(c, bb, 1, EXACT) == aa)
	  return c.round(prec, TO_ZERO);
	if (c.significant > 0)
	  return c.round(prec, TO_ZERO);
	if (c.significant < 0)
	  return c.round(prec, TO_INF);
	return c;


      case TO_INF:


	if (mul(c, bb, 1, EXACT) == aa)
	  return c.round(prec, TO_ZERO);
	if (c.significant > 0)
	  return c.round(prec, TO_P_INF);
	if (c.significant < 0)
	  return c.round(prec, TO_N_INF);
	return c;


      case EXACT:
	error_handler(1, "div(bigfloat) : not implemented case EXACT!");
      case DEFAULT:;
  }






  if (mode == DEFAULT)
    mode = round_mode;
  switch (mode) {
      case TO_NEAREST:


	if (abs(c.significant & 1) == 0) {
	  c.significant = (c.significant >> 1);
	  return c;
	}
	else {
/* now we have to check whether the remainder is zero */

	  bigfloat temp = mul(c, bb, 1, EXACT);
	  temp = mul(c, bb, 1, EXACT);

	  if (temp == aa)
	    return c.round(prec, TO_NEAREST);
	  return c.round(prec, TO_INF);
	}


      case TO_ZERO:


	c.significant = c.significant >> 1;
	return c;


      case TO_P_INF:


	if (mul(c, bb, 1, EXACT) == aa)
	  return c.round(prec, TO_ZERO);
	if (c.significant > 0)
	  return c.round(prec, TO_INF);
	if (c.significant < 0)
	  return c.round(prec, TO_ZERO);
	return c;


      case TO_N_INF:


	if (mul(c, bb, 1, EXACT) == aa)
	  return c.round(prec, TO_ZERO);
	if (c.significant > 0)
	  return c.round(prec, TO_ZERO);
	if (c.significant < 0)
	  return c.round(prec, TO_INF);
	return c;


      case TO_INF:


	if (mul(c, bb, 1, EXACT) == aa)
	  return c.round(prec, TO_ZERO);
	if (c.significant > 0)
	  return c.round(prec, TO_P_INF);
	if (c.significant < 0)
	  return c.round(prec, TO_N_INF);
	return c;


      case EXACT:
	error_handler(1, "div(bigfloat) : not implemented case EXACT!");
      case DEFAULT:;
  }


  return 0; // never reached

}



bigfloat sqrt(const bigfloat & a, long prec, rounding_modes mode)
{
  bigfloat ne, it;
  long t_prec;



  if (isNaN(a))
    return NaN;
  if ((isnZero(a)) || (ispZero(a)))
    return a;
  if (ispInf(a))
    return pInf;
  if (isnInf(a))
    return NaN;
  if (a < 0)
    return NaN;



  if (prec == -1)
    prec = global_prec;



  double d = bigfloat(a.significant, 0).todouble();
  if (!(a.exponent % 2 == 0))
    d *= 2;

  ne = sqrt(d);
  ne.exponent = ne.exponent + (a.exponent / integer(2));
  t_prec = 52;

/* std::cout Startwert */



  int flag = 0;
  do {
    it = ne;
    ne = add(mul(it, it, 1, EXACT), a, 1, EXACT);	/* compute numerator of
							 * (1) */
    ne = div(ne, mul(2, it, 1, EXACT), t_prec, TO_P_INF);	/* compute next
								 * iteration value */
    t_prec *= 2;
  } while (t_prec < prec);

  ne.round(prec, TO_P_INF);
  it.round(prec, TO_P_INF);

  while (ne != it) {
    it = ne;
    ne = add(mul(it, it, 1, EXACT), a, 1, EXACT);	/* compute numerator of
							 * (1) */
    ne = div(ne, mul(2, it, 1, EXACT), prec, TO_P_INF);	/* compute next
							 * iteration value */
  }



  bigfloat c(ne.significant - integer(1), ne.exponent);
  if ((c * c) >= a)
    it = c;




  bigfloat mid;
  if (mode == DEFAULT)
    mode = round_mode;

  switch (mode) {
      case TO_P_INF:
	return it;
      case TO_ZERO:
      case TO_N_INF:
	if ((it * it) != a)
	  return bigfloat(ne.significant - integer(1), ne.exponent);
	else
	  return it;
      case TO_NEAREST:
	mid = bigfloat((it.significant >> 1) + integer(1), it.exponent);
/* $mid=\frac{it+(it-\Delta)}{2}$ */
	if ((mid * mid) < a) {
	  mid.round(prec, TO_ZERO);
	  return mid;
	}
	else {
	  if ((mid * mid) != a)
	    return it;
	  else {
	    mid.round(prec, TO_NEAREST);
	    return mid;
	  }
	}
      case EXACT:
	error_handler(1, "bigfloat::sqrt : the EXACT-rounding mode is not provided!");

  }

  return 0; // never reached

}




bigfloat & bigfloat::operator = (const bigfloat & b) {
  significant = b.significant;
  exponent = b.exponent;
  precision = b.precision;
  return (*this);
}




std::ostream & operator << (std::ostream & os, const bigfloat & b) {
  if (1) {			/* if isSpecial */
    if (isNaN(b))
      return os << "NaN";
    if (ispInf(b))
      return os << "+Inf";
    if (isnInf(b))
      return os << "-Inf";
    if (ispZero(b))
      return os << "+0";
    if (isnZero(b))
      return os << "-0";
  }
  int sign_b = sign(b.significant);
  if (output == BIN_OUT) {
    if (sign_b < 0)
      os << "-";
    os << "0.";
    if (sign_b >= 0)
      binout(os, b.significant);
    else
      binout(os, -b.significant);
    os << "E";
    if (b.exponent < 0)
      os << "-";
    else
      os << "+";
    binout(os, b.exponent);
  }
  if (output == DEC_OUT)
    decimal_output(os, b);
  return os;
}
std::istream & operator >> (std::istream & is, bigfloat & b) {
  char temp[bin_maxlen];	/* z.Z. nur binaere-Exponentialeingabe
				 * moeglich */
  is >> temp;
  b = outofchar(temp);
  return is;
}




bool operator == (const bigfloat & a, const bigfloat & b) {
/* check whether NaN or infinity cases appear */
  if ((isNaN(a)) || (isNaN(b)) || (ispInf(a)) || (ispInf(b))
    || (isnInf(a)) || (isnInf(b)))
    error_handler(1, "bigfloat::operator == : NaN or Infinity case occurred!");

  if ((a.significant == 0) && (b.significant == 0))
    return 1;
  if (sign(a.significant) != sign(b.significant))
    return 0;
  if (a.exponent == b.exponent)
    return (bcomp(a.significant, b.significant) == 0);
  else
    return 0;
}

bool operator != (const bigfloat & a, const bigfloat & b) {
  return !(a == b);
}

bool operator > (const bigfloat & a, const bigfloat & b) {
  if ((isNaN(a)) || (isNaN(b)))
    error_handler(1, "bigfloat::operator > : NaN case occurred!");
  if (((ispInf(a)) && (ispInf(b))) || ((isnInf(a)) && (isnInf(b))))
    error_handler(1, "bigfloat::operator > : both operands +(-)infinity");
  if (ispInf(a))
    return 1;
  if (isnInf(a))
    return 0;
  if (ispInf(b))
    return 0;
  if (isnInf(b))
    return 1;

  if ((a.significant == 0) && (b.significant == 0))
    return 0;
  if (sign(a.significant) != sign(b.significant)) {
    if (sign(a.significant) == 0)
      if (sign(b.significant) < 0)
	return 1;
      else
	return 0;
    if (sign(b.significant) == 0)
      if (sign(a.significant) > 0)
	return 1;
      else
	return 0;

    if (a.significant > 0)
      return 1;
    return 0;
  }
  if (a.exponent != b.exponent)
    return (a.exponent > b.exponent);
  else
    return (bcomp(a.significant, b.significant) == 1);
}

bool operator >= (const bigfloat & a, const bigfloat & b) {
  return ((a > b) || (a == b));
}

bool operator < (const bigfloat & a, const bigfloat & b) {
  return (!(a >= b));
}

bool operator <= (const bigfloat & a, const bigfloat & b) {
  return (!(a > b));
}



bigfloat operator + (const bigfloat & a, const bigfloat & b) {
  return add(a, b);
}

bigfloat operator - (const bigfloat & a, const bigfloat & b) {
  return sub(a, b);
}

bigfloat operator *(const bigfloat & a, const bigfloat & b) {
  return mul(a, b);
}

bigfloat operator / (const bigfloat & a, const bigfloat & b) {
  return div(a, b);
}

bigfloat operator - (const bigfloat & a) {
  return bigfloat(-a.significant, a.exponent);
}




bool isNaN(const bigfloat & b)
{
  if ((b.significant == 0) && (b.exponent == 1026))
    return true;
  return false;
}

bool isnInf(const bigfloat & b)
{
  if ((b.significant == 0) && (b.exponent == 1025))
    return true;
  return false;
}

bool ispInf(const bigfloat & b)
{
  if ((b.significant == 0) && (b.exponent == 1024))
    return true;
  return false;
}

bool isnZero(const bigfloat & b)
{
  if ((b.significant == 0) && (b.exponent == (-1024)))
    return true;
  return false;
}

bool ispZero(const bigfloat & b)
{
  if ((b.significant == 0) && (b.exponent == (-1023)))
    return true;
  return false;
}

bool isInf(const bigfloat & b)
{
  return (ispInf(b) || isnInf(b));
}

bool isZero(const bigfloat & b)
{
  return (ispZero(b) || isnZero(b));
}
