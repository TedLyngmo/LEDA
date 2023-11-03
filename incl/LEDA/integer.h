/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  integer.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_INTEGER_H
#define LEDA_INTEGER_H

//------------------------------------------------------------------------------
//
//  integer:  big integers
//
//  by Christian Uhrig and Stefan Naeher   (1994)
//
//  - 32 bit (unsigned long) vector representation
//  - use of a handle class concept (to avoid copy overhead)
//  - use of the LEDA memory management
//  - sparc assembler code for 32 bit multiplication
//
//------------------------------------------------------------------------------

/*{\Manpage {integer} {} {Integers of Arbitrary Length} }*/


#include <LEDA/basic.h>
#include <iostream>

#if _MIPS_SZLONG == 64
typedef unsigned int  int_word_type;
#else
typedef unsigned long int_word_type;
#endif

class integer;
class rational;
class integer_rep
{
  friend class integer;
  friend class rational;

/*
  unsigned short  count;
  unsigned short  size;
  unsigned short  used;
           short  sign;
*/
  unsigned int  count;
  unsigned int  size;
  unsigned int  used;
           int  sign;
  int_word_type vec[1];


  friend integer_rep* copy_integer_rep(integer_rep*);

  friend integer operator + (const integer& a, const integer& b);
  friend integer operator - (const integer& a, const integer& b);
  friend integer operator * (const integer& a, const integer& b);
  friend integer operator / (const integer& a, const integer& b);
  friend integer operator % (const integer& a, const integer& b);

  friend integer operator & (const integer& a, const integer& b);
  friend integer operator | (const integer& a, const integer& b);
//friend integer operator ^ (const integer& a, const integer& b);

  friend bool operator == (const integer& a, const integer& b);
  friend bool operator < (const integer& a, const integer& b);
  friend bool operator > (const integer& a, const integer& b);

  friend inline bool operator != (const integer& a, const integer& b);
  friend inline bool operator >= (const integer& a, const integer& b);
  friend inline bool operator <= (const integer& a, const integer& b);

  friend std::ostream & operator << (std::ostream & out, const integer& a);
  friend std::istream & operator >> (std::istream & in, integer& a);

  friend integer abs(const integer& a);
  friend integer gcd(const integer&, const integer&);

  friend int log(const integer& a);

  friend inline int sign(const integer& a);

  friend inline integer_rep* new_integer_rep(unsigned int size);
  friend void  delete_integer_rep(integer_rep *p);

};


inline integer_rep* new_integer_rep(unsigned int sz)
{ int s = sizeof(integer_rep) + (sz-1)*sizeof(int_word_type);
  integer_rep* p;
  if (s < 256) p = (integer_rep*)allocate_bytes(s);
          else p = (integer_rep*)new char[s];
  p->count = 1;
  p->size = sz;
  return p;
 }


/*
inline void delete_integer_rep(integer_rep* p)
{ int s = sizeof(integer_rep) + (p->size-1)*sizeof(int_word_type);
  if (s < 256)
     deallocate_bytes(p,s);
  else
     delete[] p;
}
*/



class integer {

  friend class rational;

/*{\Mdefinition
An instance $a$ of the data type \name\ is an integer number of
arbitrary length.
}*/


private:

 integer_rep* PTR;

 integer(integer_rep* p) { PTR = p; }

 GenPtr copy() const { PTR->count++; return PTR; }
 void   clear()      { if (PTR && --(PTR->count)==0) delete_integer_rep(PTR); }
 int    refs() const { return PTR->count; }


public:

/*{\Mcreation a }*/

   integer();

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it
with zero.}*/

   integer(int n);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it
with the value of $n$.}*/

   integer(unsigned int i);
   integer(long l);
   integer(unsigned long i);

   integer(double x);

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it
with the integral part of $x$.}*/

   integer(const integer& a)  { PTR = a.PTR;  PTR->count++; }
  ~integer() { clear(); }


/*{\Moperations 2 4.7 }*/

/*{\Mtext

The arithmetic operations $+,\ -,\ *,\ /,\ +=,\
-=,\ *=,\ /=,\ -$(unary), $++,\ --$,  the modulus operation ($\%$, $\% =$), bitwise
AND ($\&$, $\& =$), bitwise OR ($\Lvert \Lvert =$), the complement ($\ \tilde{}\ $),
the shift operations
($<<,\ >>$),
the comparison operations $<,\ <=,\ >,\
>=,\ ==,\ !=$ and the stream operations all are available.
}*/

   integer& operator=(const integer& x)
   { x.PTR->count++;
     clear();
     PTR = x.PTR;
     return *this;
    }

  // type conversion

/*
  operator GenPtr() { return (PTR->sign) ? PTR : 0; }
*/

  // member functions

  int    length()   const;
/*{\Mop returns the number of bits of the representation of \var.}*/

  bool   islong()   const;
/*{\Mop returns whether \var\ fits in the data type $long$.}*/

  bool   iszero()   const;
/*{\Mop returns whether \var\ is equal to zero.}*/

  long   tolong()   const;
/*{\Mop returns a $long$ number which is initialized with the value of
\var.\\
\precond $a$.islong() is $true$.}*/

  double todouble() const;
/*{\Mop returns a $double$ number which is initialized with the value of
\var. \\
\precond $a$ fits in the range of a $double$.}*/

  integer sqrt()     const;
/*{\Mop returns the largest $integer$ which is not larger than the
squareroot of \var. }*/

  int_word_type highword() const;

  void hex_print(std::ostream&);



  // friend functions & operators

  friend integer operator + (const integer& a, const integer& b);
  friend integer operator - (const integer& a, const integer& b);
  friend integer operator * (const integer& a, const integer& b);
  friend integer operator / (const integer& a, const integer& b);
  friend integer operator % (const integer& a, const integer& b);
  friend integer operator & (const integer& a, const integer& b);
  friend integer operator | (const integer& a, const integer& b);
//friend integer operator ^ (const integer& a, const integer& b);

  friend bool operator == (const integer& a, const integer& b);
  friend bool operator != (const integer& a, const integer& b);
  friend bool operator <  (const integer& a, const integer& b);
  friend bool operator >  (const integer& a, const integer& b);
  friend bool operator >= (const integer& a, const integer& b);
  friend bool operator <= (const integer& a, const integer& b);

/*{\Mtext
\smallskip
{\bf Non-member functions}
\smallskip }*/

  friend integer abs(const integer& a);
/*{\Mfunc returns the absolute value of $a$.}*/

  friend integer gcd(const integer& a, const integer& b);
/*{\Mfunc returns the greatest common divisor of $a$ and $b$.}*/

  friend int   sign(const integer& a);
/*{\Mfunc returns the sign of $a$.}*/

  friend int    log(const integer& a);
/*{\Mfunc returns the logarithm of $a$ to the basis 2.}*/


  // member functions and operators

  int  used_words() { return PTR->used; };
  int  zeros() const;
  void absolute();

  int_word_type contents(int k) const { return PTR->vec[k]; };

  integer       div(const integer&, integer&);

  integer operator-() const;
  integer operator~() const;

  integer operator<<(long n) const;
  integer operator>>(long n) const;
  integer operator+= (const integer& b) { return *this = *this + b; }
  integer operator-= (const integer& b) { return *this = *this - b; }
  integer operator*= (const integer& b) { return *this = *this * b; }
  integer operator/= (const integer& b) { return *this = *this / b; }
  integer operator%= (const integer& b) { return *this = *this % b; }
  integer operator&= (const integer& b) { return *this = *this & b; }
  integer operator|= (const integer& b) { return *this = *this | b; }
  integer operator<<=(int n) { return *this = *this << n; }
  integer operator>>=(int n) { return *this = *this >> n; }
  integer operator++ ();
  integer operator++ (int);
  integer operator-- ();
  integer operator-- (int);

  // "long-versions" should be implemented more efficiently!
  //
  //integer operator+(long i) const;
  //integer operator-(long i) const;
  //integer operator*(long i) const;
  //integer operator/(long i) const;
  //integer operator%(long i) const;
  //integer operator+=(long i);
  //integer operator-=(long i);
  //integer operator*=(long i);
  //integer operator/=(long i);
  //integer operator%=(long i);

  bool operator==(int n) const;
  bool operator!=(int n) const;
  bool operator< (int n) const;
  bool operator> (int n) const;
  bool operator>=(int n) const;
  bool operator<=(int n) const;

  // static members

  static integer random(int n);


  // more friend s

  friend std::ostream& operator << (std::ostream& O, const integer& a);
  friend std::istream& operator >> (std::istream& I, integer& a);

#if !defined(__TEMPLATE_FUNCTIONS__)
  friend GenPtr  Create(const integer*) { integer x; return x.copy(); }
  friend void    Clear(integer& y)      { y.clear();}
  friend GenPtr  Convert(integer& y)    { return y.PTR; }
  friend GenPtr  Copy(const integer& y) { return y.copy();}
  friend const char*   Type_Name(const integer*) { return "integer"; }
#endif


  static int  cmp(const integer& a, const integer& b);


  LEDA_MEMORY(integer)

};

inline void Print(const integer& a,std::ostream & out) { out << a; }
inline void Read(integer& a,std::istream & in) { in >> a; }

inline int compare(const integer& a, const integer& b)
{ return integer::cmp(a,b); }


inline bool operator != (const integer& a, const integer& b) { return !(a==b); }
inline bool operator <= (const integer& a, const integer& b) { return !(a>b);  }
inline bool operator >= (const integer& a, const integer& b) { return !(a<b);  }

inline int sign(const integer& a) { return a.PTR->sign; }




inline bool integer::iszero() const { return (PTR->sign == 0); }


inline bool integer::islong() const
{ if (PTR->sign == 0) return true;
  else return (PTR->used == 1 && PTR->vec[0] <= 0x8FFFFFFF);
 }


inline long integer::tolong() const
{ if (PTR->sign == 0) return 0;
  else return  (PTR->sign > 0) ? PTR->vec[0] : -PTR->vec[0];
 }

inline int_word_type integer::highword() const
{ if (PTR->sign == 0) return 0;
  else return PTR->vec[PTR->used-1];
 }


inline bool integer::operator!=(int a) const { return !operator==(a); }
inline bool integer::operator<=(int a) const { return !operator>(a);  }
inline bool integer::operator>=(int a) const { return !operator<(a);  }


inline integer integer::operator++ () { return *this = *this + 1; }
inline integer integer::operator-- () { return *this = *this - 1; }

inline integer integer::operator++ (int)
{ integer i = *this;
  *this = *this + 1;
  return i;
 }

inline integer integer::operator-- (int)
{ integer i = *this;
  *this = *this - 1;
  return i;
 }


/*{\Mimplementation
An $integer$ is essentially implemented by a
vector $vec$ of $unsigned\ long$ numbers.
The sign and the size are stored in extra variables.
Some time critical functions are also implemented in sparc assembler code.}*/



#endif
