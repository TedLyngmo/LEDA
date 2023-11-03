/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  random.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_RANDOM_H
#define LEDA_RANDOM_H


//------------------------------------------------------------------------------
// random numbers
//------------------------------------------------------------------------------

#include <LEDA/basic.h>


/*{\Manpage {random_source} {} {Random Sources} }*/

class random_source {

/*{\Mdefinition
An instance of type \name\ is a random source. It allows to generate
uniformly distributed random bits, characters, integers, and doubles.
It can be in  either of two modes: In bit mode it generates a random bit string
of some given length $p$ ($1 \le p \le 31$) and in integer mode it generates a random
integer in some given range $[ low .. high]$  ($low \le high < low + 2^{31}$). The mode can be changed any time, either globally or for a single operation. The output of the random
source can be converted to a number of formats (using standard conversions).}*/

  unsigned long pat;
  int low;
  int diff;
  bool bit_mode;

public:

/*{\Mcreation S }*/

random_source();
/*{\Mcreate  creates an instance \var\ of type \name ,
             puts it into bit mode, and
             sets the precision to 31. }*/

random_source(int p);
/*{\Mcreate  creates an instance \var\ of type \name ,
             puts it into bit mode, and
             sets the precision to $p$ ($1 \le p \le 31$). }*/

random_source(int low, int high);
/*{\Mcreate  creates an instance \var\ of type \name ,
             puts it into integer mode, and
             sets the range to $[ low .. high]$. }*/


/*{\Moperations 3.2 5 }*/

void set_seed(int s);
/*{\Mop         resets the seed of the random number generator to $s$. }*/

void set_range(int low, int high);
/*{\Mopl        sets the mode to integer mode and
                changes the range to $[ low .. high]$. }*/

void set_precision(int p);
/*{\Mop         sets the mode to bit mode and
                changes the precision to $p$ bits. }*/


// extract random numbers with precision or range specified in constructor

random_source& operator>>(char& x);
/*{\Mbinop      extracts a character $x$ of default precision or range
                and returns \var, i.e., it first generates an unsigned
                integer of the desired precision or in the desired range
                and then converts it to a character (by standard conversion).
                 }*/

random_source& operator>>(unsigned char& x);
/*{\Mbinop      extracts an unsigned character $x$ of default precision or
                range and returns \var. }*/

random_source& operator>>(long& x);
random_source& operator>>(int& x);
/*{\Mbinop      extracts an integer $x$ of default precision or range and
                returns \var. }*/


random_source& operator>>(unsigned long& x);
random_source& operator>>(unsigned int& x);
/*{\Mbinop      extracts an unsigned integer $x$ of default precision or
                range and returns \var. }*/


random_source& operator>>(float& x);
random_source& operator>>(double& x);
/*{\Mbinop      extracts a real number $x$ in $[0,1]$, i.e,
                $u/(2^{31} -1)$ where $u$ is a random integer in
                $[0..2^{31}-1]$,
                and returns \var.}*/


#ifdef __BUILTIN_BOOL__
random_source& operator>>(bool& b);
/*{\Mbinop      extracts a random boolean value (true or false). }*/

#endif

// raw read (31 bit)

unsigned get();
/*{\Mop            returns an unsigned integer of maximal precision (31 bits). }*/

// operator()

int operator()();
/*{\Mfunop         returns an integer $x$.}*/

int operator()(int prec);
/*{\Mfunop         returns an integer $x$ of supplied precision $prec$. }*/

int operator()(int low, int high);
/*{\Mfunop         returns an integer $x$ from the supplied range $[low..high]$. }*/


};

extern random_source rand_int;   // standard random input stream

#endif
