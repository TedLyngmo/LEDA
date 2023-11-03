/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _integer.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


//------------------------------------------------------------------------------
//
//  integer:  big integers
//
//  by Christian Uhrig and Stefan Naeher   (1994/1995)
//
//  - 32 bit (unsigned long) vector representation
//  - use of a handle class concept (to avoid copy overhead)
//  - use of the LEDA memory management
//  - sparc assembler code for 32 bit multiplication and division
//  - Karatsuba multiplication for long integers
//  - loop unrolling
//
//  - sparc assembler for add and sub (S. Naeher, 1995)
//
//------------------------------------------------------------------------------


#include <LEDA/integer.h>
#include <math.h>
#include <ctype.h>


typedef int_word_type word;

typedef unsigned int sz_t;


#define WORD_LENGTH  32 
#define HALF_LENGTH  16 
#define KARA_LIMIT    8 

#define MAX_WORD     0xFFFFFFFF
#define RIGHT_MASK   0x0000FFFF
#define LEFT_MASK    0xFFFF0000

#define LOW_WORD(w)  (w & RIGHT_MASK)
#define HIGH_WORD(w) (w >> HALF_LENGTH)

enum {ZERO=0, NEGATIVE=-1, POSITIVE=1 };

const double pow32 = ldexp(1,WORD_LENGTH);



inline int next_power(int s)
{ int p = 1;
  while (p < s) p <<= 1;
  return p;
}


void delete_integer_rep(integer_rep* p)
{ int s = sizeof(integer_rep) + (p->size-1)*sizeof(unsigned long);
  if (s < 256)
     deallocate_bytes(p,s);
  else
     delete[] p;
}


integer::integer()
{ PTR = new_integer_rep(1);
  PTR->vec[0] = 0;
  PTR->sign = ZERO;
  PTR->used = 0;
 }



integer::integer(unsigned int n)
{
  PTR = new_integer_rep(1);

  PTR->vec[0] = n;

  if (n == 0)
     { PTR->sign = ZERO;
       PTR->used = 0; }
  else
     { PTR->sign = POSITIVE;
       PTR->used = 1; }
}



integer::integer(int n)
{
  PTR = new_integer_rep(1);

  PTR->used = 1;
  PTR->vec[0] = n;
  PTR->sign = POSITIVE;

  if (n == 0)
  { PTR->sign = ZERO;
    PTR->used = 0;
   }

  if (n < 0) 
  { PTR->sign = NEGATIVE;
    PTR->vec[0] = -n;
   }
}


integer::integer(long n)
{
  PTR = new_integer_rep(1);

  PTR->used = 1;
  PTR->vec[0] = n;
  PTR->sign = POSITIVE;

  if (n == 0)
  { PTR->sign = ZERO;
    PTR->used = 0;
   }

  if (n < 0) 
  { PTR->sign = NEGATIVE;
    PTR->vec[0] = -n;
   }
}


integer::integer(unsigned long n)
{
  PTR = new_integer_rep(1);

  if (n == 0)
  { PTR->sign = ZERO;
    PTR->used = 0;
   }
  else
  { PTR->sign = POSITIVE;
    PTR->used = 1;
    PTR->vec[0] = n;
   }

}



integer::integer(double x) 
{ 
  int sig = POSITIVE;

  if (x < 0) 
  { sig = NEGATIVE;
    x = -x;
   }

  if (x < 1)
  { PTR = new_integer_rep(1);
    PTR->used = 0;
    PTR->sign = ZERO;
    return;
   }

  if (x < double(MAX_WORD))
  { PTR = new_integer_rep(1);
    PTR->vec[0] = word(x);
    PTR->used = 1;
    PTR->sign = sig;
    return;
   }


  int expt;
  double mantissa = frexp(x, &expt);
  double int_mant;

  int r = expt % WORD_LENGTH;
  int q = expt / WORD_LENGTH;

  int used = q;
  if (r) used++;

  int sz = next_power(used);

  PTR = new_integer_rep(sz);
  PTR->used = used;
  PTR->sign = sig;

  word* p = PTR->vec+used-1;

  if (r)
  { mantissa *= ldexp(1,r);
    mantissa = modf(mantissa, &int_mant);
    *p--= (unsigned long)int_mant; // first r bits
   }

  while (q--)
    if (mantissa != 0)
    { mantissa *= pow32;
      mantissa = modf(mantissa, &int_mant);
      *p-- = (unsigned long)int_mant; // next 32 bits
     }
    else *p-- = 0;

  if (used == 0) PTR->sign = ZERO;

}



//------------------------------------------------------------------------------
// local functions
//------------------------------------------------------------------------------

inline word DivTwoWordsByOne(word high, word low, word D, word* q)
{ 
  // precondition:  high < D  
  // *q = [high,low] / D, returns remainder
  // the result has at most 32 bits  
  // we can compute it with double precision floating point division

  double H = high;
  double L = low;
  word   Q = word((H/D)*pow32 + L/D);

  D *= Q;

  *q = Q;
  return low-D;
}


static int absolute_cmp(word* a, int a_used, word* b, int b_used)
{ 
  if (a_used > b_used) return  1;
  if (a_used < b_used) return -1;
  word* ap = a + a_used;
  word* bp = b + b_used;
  while(ap > a)
  { word A = *--ap;
    word B = *--bp;
    if (A > B) return  1;
    if (A < B) return -1;
   }
  return 0;
}


static void Init_Vector(word* a, sz_t a_used)
{ 
  switch (a_used % 16)
  { case 15:*a++ = 0;
    case 14:*a++ = 0;
    case 13:*a++ = 0;
    case 12:*a++ = 0;
    case 11:*a++ = 0;
    case 10:*a++ = 0;
    case  9:*a++ = 0;
    case  8:*a++ = 0;
    case  7:*a++ = 0;
    case  6:*a++ = 0;
    case  5:*a++ = 0;
    case  4:*a++ = 0;
    case  3:*a++ = 0;
    case  2:*a++ = 0;
    case  1:*a++ = 0;
   }

  int n = a_used / 16;

  while (n--)
    { *a++ = 0;  *a++ = 0;  *a++ = 0;  *a++ = 0;
      *a++ = 0;  *a++ = 0;  *a++ = 0;  *a++ = 0;
      *a++ = 0;  *a++ = 0;  *a++ = 0;  *a++ = 0;
      *a++ = 0;  *a++ = 0;  *a++ = 0;  *a++ = 0; }
}



static word* Copy_Vector(word* a, word* b, sz_t b_used)
{ 
  // copy b[0..b_used-1] to a[]

  switch (b_used % 16)
  { case 15:*a++ = *b++;
    case 14:*a++ = *b++;
    case 13:*a++ = *b++;
    case 12:*a++ = *b++;
    case 11:*a++ = *b++;
    case 10:*a++ = *b++;
    case  9:*a++ = *b++;
    case  8:*a++ = *b++;
    case  7:*a++ = *b++;
    case  6:*a++ = *b++;
    case  5:*a++ = *b++;
    case  4:*a++ = *b++;
    case  3:*a++ = *b++;
    case  2:*a++ = *b++;
    case  1:*a++ = *b++; }

  int n = b_used / 16;

  while (n--)
    { *a++ = *b++;  *a++ = *b++;  *a++ = *b++;  *a++ = *b++;
      *a++ = *b++;  *a++ = *b++;  *a++ = *b++;  *a++ = *b++;
      *a++ = *b++;  *a++ = *b++;  *a++ = *b++;  *a++ = *b++;
      *a++ = *b++;  *a++ = *b++;  *a++ = *b++;  *a++ = *b++; }

  return a;
}



static void Print_Vector(word* a, int a_used, ostream& out) 
{
  char* digits   = new char[10*a_used];
  word* tmp      = new word[a_used];
  word* tmp_high = Copy_Vector(tmp,a,a_used) - 1;

  int i = 0;
  while (tmp_high >= tmp)
  { word r = 0;
    for (word* p = tmp_high; p >= tmp; p--) 
       r = DivTwoWordsByOne(r,*p,10,p);
    if (*tmp_high == 0) tmp_high--;
    digits[i++] = '0' + char(r);
   }

  while (i--) out << digits[i];

  delete[] digits;
  delete[] tmp;
}



#if defined(sparc)

//------------------------------------------------------------------------------
// sparc assembler code
//------------------------------------------------------------------------------
// _sparc_mult.s: Multiply_Words & Mult_Inner_Loop
// _sparc_div.s : Div_Inner_Loop
// _sparc_add.s : School_Add
// _sparc_sub.s : School_Sub
//------------------------------------------------------------------------------

#if defined(__svr4__)
#define Multiply_Words  _Multiply_Words
#define Mult_Inner_Loop _Mult_Inner_Loop
#define Div_Inner_Loop  _Div_Inner_Loop
#define School_Add      _School_Add
#define School_Sub      _School_Sub
#endif

extern "C" sz_t School_Add(word*,sz_t,word*,sz_t,word*);
extern "C" sz_t School_Sub(word*,sz_t,word*,sz_t,word*);
extern "C" word Multiply_Words(word a, word b, word* high);
extern "C" word Mult_Inner_Loop(word* p, word* a, word* a_stop, word B);
extern "C" word Div_Inner_Loop(word* p, word* a, word* a_stop, word B);

#else
/* if not sparc */

inline word Multiply_Words(word a, word b, word* high)
{ word al = LOW_WORD(a);
  word ah = HIGH_WORD(a);
  word bl = LOW_WORD(b);
  word bh = HIGH_WORD(b);
  word c,L,H;
  c = bl*al;           
  L = LOW_WORD(c);
  c = HIGH_WORD(c) + bl*ah;
  H = HIGH_WORD(c);
  c = LOW_WORD(c)  + bh*al;
  L |= (LOW_WORD(c) << HALF_LENGTH);
  *high =  H + HIGH_WORD(c) + bh*ah;
  return L;
}


inline word Mult_Inner_Loop(word* p, word* a, word* a_stop, word B)
{
   // p[] += a[]*B, return carry

   word low,high;
   word carry = 0;
   while (a < a_stop)
   { word P = *p;
     low = Multiply_Words(*a++,B,&high);
     P += low;
     if (P < low )  high++; // carry in addition
     P += carry;
     if (P < carry) high++; // carry in addition
     *p++ = P;
     carry = high;
   }
   *p = carry;
   return carry;
}


inline word Div_Inner_Loop(word* p, word* a, word* a_stop, word B)
{ 
   // p[] -= a[]*B, return carry

   word low,high;
   word carry = 0;
   word P;
   while (a < a_stop)
   { P = *p;
     low = Multiply_Words(*a++,B,&high);
     if (P < low) high++;  // carry
     P -= low;
     if (P < carry) high++;  // carry
     P -= carry;
     *p++ = P;
     carry = high;
   }
   P = *p;
   *p = P - carry;
  
   return (P < carry);
}



static sz_t School_Add(word *a, sz_t a_used, word *b, sz_t b_used, word* sum)
{
  // compute sum = a + b   (a_used >= b_used)

  int carry = 0;
  word aa;
  word bb;
  word* s_last = sum + a_used;

#define ADD_LOOP_BODY {\
  aa = *a++;\
  bb = *b++;\
  aa += bb;\
  if (carry) { if (++aa > bb) carry = false; }\
  else if (aa < bb) carry = true;\
  *sum++ = aa; }

  int r = b_used % 16;

  switch (r) { 
      case 15: ADD_LOOP_BODY;
      case 14: ADD_LOOP_BODY;
      case 13: ADD_LOOP_BODY;
      case 12: ADD_LOOP_BODY;
      case 11: ADD_LOOP_BODY;
      case 10: ADD_LOOP_BODY;
      case  9: ADD_LOOP_BODY;
      case  8: ADD_LOOP_BODY;
      case  7: ADD_LOOP_BODY;
      case  6: ADD_LOOP_BODY;
      case  5: ADD_LOOP_BODY;
      case  4: ADD_LOOP_BODY;
      case  3: ADD_LOOP_BODY;
      case  2: ADD_LOOP_BODY;
      case  1: ADD_LOOP_BODY;
     }

  int n = b_used / 16;

  while (n--)
  { ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; 
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; 
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; 
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY;
   }

  if (sum != a) // copy rest of a to sum
  { Copy_Vector(sum,a,a_used-b_used);
    *s_last = 0;
   }

  if (carry) // propagate carry
  { while (++*sum == 0) sum++;
    if (sum == s_last) a_used++;
   }

  return a_used;
}


static sz_t School_Sub(word *a, sz_t a_used, word *b, sz_t b_used, word* diff)
{
  // compute diff = a - b    (a > b)

#define SUB_LOOP_BODY {\
  aa = *a++;\
  bb = *b++;\
  if (borrow && ++bb) borrow = false;\
  if (aa < bb)  borrow = true;\
  *diff++ = aa - bb; }

  word  aa;
  word  bb;
  bool  borrow = false;

  word* d_stop = diff + a_used;

  switch (b_used % 16) 
   { case 15: SUB_LOOP_BODY;
     case 14: SUB_LOOP_BODY;
     case 13: SUB_LOOP_BODY;
     case 12: SUB_LOOP_BODY;
     case 11: SUB_LOOP_BODY;
     case 10: SUB_LOOP_BODY;
     case  9: SUB_LOOP_BODY;
     case  8: SUB_LOOP_BODY;
     case  7: SUB_LOOP_BODY;
     case  6: SUB_LOOP_BODY;
     case  5: SUB_LOOP_BODY;
     case  4: SUB_LOOP_BODY;
     case  3: SUB_LOOP_BODY;
     case  2: SUB_LOOP_BODY;
     case  1: SUB_LOOP_BODY;
    }

  int n = b_used / 16;

  while (n--)
  { SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; 
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; 
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; 
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY;
   }

  if (diff != a) Copy_Vector(diff,a,a_used-b_used);

  if (borrow)
    while (--*diff == MAX_WORD) diff++;

  while (*--d_stop == 0) a_used--;

  return a_used;
}

/* end of not-sparc section */
#endif


static sz_t School_Mult(word *a, sz_t a_used, word *b,int b_used, word* prod) 
{ 
  sz_t p_used = a_used + b_used;
  word* a_stop = a + a_used;
  word* b_last = b + b_used - 1;

  Init_Vector(prod,a_used);

  while (b < b_last)
      Mult_Inner_Loop(prod++,a,a_stop,*b++);

  if (Mult_Inner_Loop(prod,a,a_stop,*b) == 0) p_used--;   // no carry

  return p_used;
}



static void Karatsuba(word* prod, word* a, word* b, word* tmp, int n)
{       

 /*  Input: 
                4n      3n      2n       n        0

                                         A1       A0
                                 +-------+--------+
   a[0..2n-1]                    |  a1   |  a0    |
                                 +-------+--------+

                                         B1       B0
                                 +-------+--------+
   b[0..2n-1]                    |  b1   |  b0    |
                                 +-------+--------+

                                 T2      T1       T0
                 +-------+-------+-------+--------+
   tmp[0..4n-1]  |       |       |       |        |    space for temporaries
                 +-------+-------+-------+--------+


  Output:                P3      P2      P1       P0
                 +-------+-------+-------+--------+
  prod[0..4n-1]  |           a  *  b              | 
                 +-------+-------+-------+--------+



 Let  
         a = a1*B^n + a0 
         b = b1*B^n + b0

 then
       a*b = (a1*b1)*B^2n + (a1*b0)*B^n + (a0*b1)*B^n + (a0*b0)

           = (a0*b0) + (a1*b1)*B^2n + (a1*b1 + a0*b0 + (a1-a0)(b0-b1))*B^n 


       4n      3n      2n       n        0
        +-------+-------+-------+--------+
        |    a1 * b1    |    a0 * b0     |
        +-------+-------+-------+--------+
        +-------+-------+-------+--------+
      + |   0   |    a1 * b1    |   0    |
        +-------+-------+-------+--------+
        +-------+-------+-------+--------+
      + |   0   |    a0 * b0    |   0    |
        +-------+-------+-------+--------+
        +-------+-------+-------+--------+
      + |   0   |(a1-a0)*(b0-b1)|   0    |
        +-------+-------+-------+--------+
     ----------------------------------------
        +--------------------------------+
      = |         a     *     b          |
        +--------------------------------+


   We compute the three partial products a1*b1, a0*b0, and (a1-a0)*(b0-b1)
   recursively as long as n is even and greater or equal to KARA_LIMIT
   and by a call of School_Mult otherwise.

  */


  if (n & 1 || n < KARA_LIMIT)
  { School_Mult(a,2*n,b,2*n,prod);
    return;
   }


  word* A0 = a;
  word* A1 = a + n;

  word* B0 = b;
  word* B1 = b + n;

  word* P0 = prod;
  word* P1 = prod + n;
  word* P2 = prod + 2*n;
  word* P3 = prod + 3*n;

  word* T0 = tmp;
  // word* T1 = tmp + n; (never used)
  word* T2 = tmp + 2*n;


  int a_sign = absolute_cmp(A1,n,A0,n);
  int b_sign = absolute_cmp(B0,n,B1,n);

  Karatsuba(P0,A0,B0,T0,n/2);   // prod[0..2n]  := a0 * b0 

  if (a_sign >= 0)
     School_Sub(A1,n,A0,n,P2);  // prod[2n..3n] := a1 - a0
  else 
     School_Sub(A0,n,A1,n,P2);  // prod[2n..3n] := a0 - a1

  if (b_sign >= 0)
     School_Sub(B0,n,B1,n,P3);  // prod[3n..4n] := b0 - b1
  else
     School_Sub(B1,n,B0,n,P3);  // prod[3n..4n] := b1 - b0


  Karatsuba(T2,P2,P3,T0,n/2);   // tmp[2n..4n]  := |a1-a0|*|b0-b1|
  Karatsuba(P2,A1,B1,T0,n/2);   // prod[2n..4n] := a1 * b1

  Copy_Vector(T0,P0,2*n);

  /* now we have 
                      P3      P2      P1      P0
              +-------+-------+-------+-------+
     prod =   |     a1*b1     |     a0*b0     |
              +-------+-------+-------+-------+
              
                              T2      T1      T0
              +-------+-------+-------+-------+
      tmp =   ||a1-a0|*|b0-b1||     a0*b0     |
              +-------+-------+-------+-------+
  */


  // add up partial products

  School_Add(P1,3*n,P2,2*n,P1);   // prod[1n..3n] += prod[2n..4n]
  School_Add(P1,3*n,T0,2*n,P1);   // prod[1n..3n] +=  tmp[0n..2n]

  if (a_sign == b_sign)  
    School_Add(P1,3*n,T2,2*n,P1); // prod[1n..3n] += tmp[2n..4n]
  else
    School_Sub(P1,3*n,T2,2*n,P1); // prod[1n..3n] -= tmp[2n..4n]

}




static sz_t Kara_Mult(word* a, sz_t a_used, word* b, sz_t b_used, word* prod)
{ 
  // a_used >= b_used

  if (b_used < 4*KARA_LIMIT)
     return School_Mult(a, a_used, b, b_used, prod);

  sz_t p_used = a_used + b_used;

  int m = next_power(b_used)/KARA_LIMIT;
  int n = 0;

  while (n < b_used) n += m; // n = i*2^k with i < KARA_LIMIT

  word* ap = a;
  word* bp = b;

  // extend length of b to n (append zeroes) 
  if (n > b_used)
  { bp = new word[n];
    word* p = Copy_Vector(bp,b,b_used);
    Init_Vector(p,n-b_used);
   }

  // partition a into t intervals of size n each
  // r is size of remainder

  int t = a_used / n;
  int r = a_used % n;

  if (r > n/2) // make length(a) a muliple of n
  { t++;
    r = 0;
    ap  = new word[t*n];
    word* p = Copy_Vector(ap,a,a_used);
    Init_Vector(p,t*n-a_used);
    a_used = t*n;
   }

  word* res = new word[a_used + n]; // result
  Init_Vector(res,a_used+n);

  word* tpr = new word[2*n];        // temporary product
  word* tmp = new word[2*n];        // temporary space used by Karatsuba

  word* q = ap;   // current position in ap
  word* p = res;  // current position in result

  while (t--)
  { Karatsuba(tpr, q, bp, tmp, n/2);  // tpr := q[0..n-1] * bp[0..n-1]
    School_Add(p,2*n,tpr,2*n,p);      // add tpr to result
    q += n;
    p += n;
   }
     
  // use School_Mult to multiply rest of a with bp
  while (r--) Mult_Inner_Loop(p++,bp,bp+b_used, *q++);


  p = Copy_Vector(prod,res,p_used);  // copy result to pred
  while (*--p == 0) p_used--;        // and adjust p_used

  if (ap != a) delete[] ap;
  if (bp != b) delete[] bp;
  delete[] tmp;
  delete[] res;

  return p_used; 
}
  


static word Shift_Left(word *a, word *b, sz_t length, sz_t shift)
{ 
  // auxiliary function used in School_Div
  // a := (b << shift) and return carry   (0 < shift < 32)
  
  word* b_stop = b + length;

  if (shift == 0)
  { while (b < b_stop) *a++ = *b++;
    return 0;
   }

  int  r = WORD_LENGTH-shift;
  word carry = 0;
  while (b < b_stop)
  { word B = *b++;
    carry |= (B << shift);
    *a++ = carry;
    carry = (B >> r);
   }
  return carry; 
}



static sz_t School_Div(word* A, sz_t a_used, word* B, sz_t b_used,
                        word* Q, word* R, sz_t* R_used)
{
  word* a      = new word[a_used + 1];
  word* b      = new word[b_used];
  word* b_stop = b + b_used;

  sz_t quot_used = a_used - b_used + 1;

  // we first normalize A and B, i.e. ...

  sz_t shift = 0;
  word sh = B[b_used-1];
  while ((sh & 0x80000000) == 0)
   { sh <<= 1;
     shift++;
   }

  a[a_used] = Shift_Left(a, A, a_used, shift);
  Shift_Left(b, B, b_used, shift);

  // two often used values

  word b1 = b[b_used-1];               
  word b2 = (b_used > 1) ? b[b_used-2] : 0;

  /* division main loop */

  word* a_ptr = a + a_used;        // current position in a
  word* Q_ptr = Q + quot_used - 1; // current position in Q
 
  while (Q_ptr >= Q)
  {
    word q = MAX_WORD;

    if (*a_ptr != b1) 
    { word r = DivTwoWordsByOne(*a_ptr, *(a_ptr-1), b1, &q);
      word h11;
      word h10 = Multiply_Words(q, b2, &h11);
      word h21 = r;
      word h20 = *(a_ptr-2);

      while( h11 > h21 || (h11 == h21 && h10 > h20) ) // [h11,h10] > [h21,h20]
      { q--;
        if (h10 < b2) h11--;
        h10 -= b2;
        h21 += b1;
        if (h21 < b1) break;
       }
     }


     if (Div_Inner_Loop(a_ptr-b_used,b,b_stop,q)) // a -= b*q
     { // if carry add one b back to a and decrement q  (never executed ?)
       word* ap;
       word* bp;
       bool  carry = false;
       for(bp = b, ap = a_ptr-b_used; bp < b_stop; bp++, ap++) 
       { word aa = *ap;
         word bb = *bp;
         bb += aa;
         if (carry)
           { if (++bb > aa) carry = false; }
         else
             if (bb < aa) carry = true;
         *ap = bb;
        }
       if (carry) (*ap)++; 
       q--;
     }

    *Q_ptr-- = q;
     a_ptr--;
  }

  
  if (R)  // copy remainder to R[]
  { word* p = a + b_used;
    word* q = R + b_used;
    // copy and shift a to R[]
    if (shift) 
      { int  r = WORD_LENGTH-shift;
        word carry = 0;
        while (p > a)
        { word A = *--p;
          carry |= (A >> shift);
          *--q = carry;
          carry = (A << r);
         }
       }
    else
       while (p > a) *--q = *--p;

    for(q = R+b_used-1; b_used && *q==0; q--)  b_used--; 

    *R_used = b_used;
   }


  delete[] a;
  delete[] b;

  if (Q[quot_used-1] == 0) quot_used--;
  return quot_used;
}




static sz_t DivByOneWord(word *a, sz_t a_used, word B, word* quot, word* rem)
{
  word  r = 0;
  word* a_ptr = a + a_used;
  word* q_ptr = quot + a_used;

  while (a_ptr > a) r = DivTwoWordsByOne(r,*--a_ptr,B,--q_ptr);

  if (quot[a_used-1] == 0) a_used--;
  if (rem) *rem = r;
  return a_used;
}




//------------------------------------------------------------------------------
// friend operators  + - * / % == < > ...
//------------------------------------------------------------------------------



integer operator+(const integer & a, const integer & b) 
{
  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;

  sz_t sum_size;

  if (a_sign == ZERO) return b;
  if (b_sign == ZERO) return a;

  integer_rep* sum_ptr;

  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;
  sz_t a_size = a.PTR->size;
  sz_t b_size = b.PTR->size;

  word* av = a.PTR->vec;
  word* bv = b.PTR->vec;

  // we distinguish the cases of equal and unequal sign

  if (a_sign == b_sign)
  { if (a_used >= b_used)
     { sum_size = (a_used+1 <= a_size) ? a_size : 2*a_size;
       sum_ptr = new_integer_rep(sum_size);
       sum_ptr->used = School_Add(av, a_used, bv, b_used, sum_ptr->vec); 
      }
    else
     { sum_size = (b_used+1 <= b_size) ? b_size : 2*b_size;
       sum_ptr = new_integer_rep(sum_size);
       sum_ptr->used = School_Add(bv, b_used, av, a_used, sum_ptr->vec); 
      }

    sum_ptr->sign = a_sign;
    return sum_ptr;
   }


/* if |a| and |b| have different signs, we must subtract the smaller absolute
 * value from the greater one and set the sign appropriately
 */

  int rel = a_used - b_used;

  if (rel == 0)
  { word* p = av + a_used;
    word* q = bv + b_used;
    while (a_used && *--p == *--q) 
    { a_used--;
      b_used--;
     }
    if (a_used == 0) return 0;
    rel  = (*p > *q) ? 1 : -1;
   }

  if (rel > 0) 
    { // |a| > |b|
      sum_ptr = new_integer_rep(a_size);
      sum_ptr->used = School_Sub(av, a_used, bv, b_used, sum_ptr->vec);
      sum_ptr->sign = a_sign;
     }
  else
    { // |a| < |b|
      sum_ptr = new_integer_rep(b_size);
      sum_ptr->used = School_Sub(bv, b_used, av, a_used, sum_ptr->vec);
      sum_ptr->sign = b_sign;
     }

  return sum_ptr;
}




integer operator-(const integer & a, const integer & b)
{
  if (a.PTR == b.PTR) return 0;

  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;
  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;
  sz_t a_size = a.PTR->size;
  sz_t b_size = b.PTR->size;


  word* av = a.PTR->vec;
  word* bv = b.PTR->vec;

  sz_t diff_size;

  if (b_sign == ZERO) return a;
  if (a_sign == ZERO) return -b;

  integer_rep* diff_ptr;

  /* we distinguish the cases of equal and unequal signs */

  if (a_sign == b_sign)
  { 
    int rel = a_used - b_used;

    if (rel == 0)
    { word* p = av + a_used;
      word* q = bv + b_used;
      while (a_used && *--p == *--q) 
      { a_used--;
        b_used--;
       }
      if (a_used == 0) return 0;
      rel  = (*p > *q) ? 1 : -1;
     }
    
    if (rel > 0)
    { // |a| > |b| 
      diff_ptr = new_integer_rep(a_size);
      diff_ptr->used = School_Sub(av, a_used, bv, b_used, diff_ptr->vec);
      diff_ptr->sign = a_sign;  
      return diff_ptr;
     }
    else 
    {  // |b| > |a|
      diff_ptr = new_integer_rep(b_size);
      diff_ptr->used = School_Sub(bv, b_used, av, a_used, diff_ptr->vec);
      diff_ptr->sign = -a_sign;
      return diff_ptr;
    }

  }

/*
 * if |a| and |b| have different signs, we must carry out an addition and set
 * the sign appropriately
 */

  if (a_used >= b_used)
    { diff_size = (a_used+1 <= a_size) ? a_size : 2*a_size;
      diff_ptr  = new_integer_rep(diff_size);
      diff_ptr->used = School_Add(av, a_used, bv, b_used, diff_ptr->vec);
     }
  else
    { diff_size = (b_used+1 <= b_size) ? b_size : 2*b_size;
      diff_ptr  = new_integer_rep(diff_size);
      diff_ptr->used = School_Add(bv, b_used, av, a_used, diff_ptr->vec);
     }

  diff_ptr->sign = a_sign;

  return diff_ptr;
}




integer operator*(const integer& a, const integer& b) 
{
  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;

  if (a_sign == ZERO) return a;
  if (b_sign == ZERO) return b;

  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;

  integer_rep* prod_ptr;

  if (a_used == 1 && b_used == 1)
  { prod_ptr = new_integer_rep(2);
    word* p  = prod_ptr->vec;
    prod_ptr->sign = (a_sign == b_sign ? POSITIVE : NEGATIVE);
    *p = Multiply_Words(a.PTR->vec[0], b.PTR->vec[0], p+1);
    prod_ptr->used = (p[1]) ? 2 : 1;
    return prod_ptr;
   }

  int sz = next_power(a_used+b_used);

  prod_ptr = new_integer_rep(sz);
  prod_ptr->sign = (a_sign == b_sign ? POSITIVE : NEGATIVE);


  if (a_used >= b_used)
      prod_ptr->used = Kara_Mult(a.PTR->vec, a_used, b.PTR->vec, b_used,
                                 prod_ptr->vec);
  else
      prod_ptr->used = Kara_Mult(b.PTR->vec, b_used, a.PTR->vec, a_used,
                                 prod_ptr->vec);
  return prod_ptr;
}




integer operator/(const integer & a, const integer & b) 
{ 
  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;
  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;

  if (b_sign == ZERO) error_handler(1,"division by zero");

  if (a_sign == ZERO) return a;

  int rel = absolute_cmp(a.PTR->vec,a_used, b.PTR->vec, b_used);

  if (rel == 0) return (a_sign == b_sign) ? 1 : -1;  // |a| == |b|
  if (rel  < 0) return 0;                            // |a| <  |b|

  // |a| > |b|

  int sz = next_power(a_used - b_used + 1);

  integer_rep* quot_ptr = new_integer_rep(sz);

  quot_ptr->sign = (a_sign == b_sign) ? POSITIVE : NEGATIVE;

  if (b_used == 1) 
      quot_ptr->used = DivByOneWord(a.PTR->vec, a_used, b.PTR->vec[0],
                                    quot_ptr->vec,0);
  else 
      // |a| and |b| both have at least 2 digits
      quot_ptr->used = School_Div(a.PTR->vec, a_used, b.PTR->vec, b_used,
                                  quot_ptr->vec,0,0);

  return quot_ptr;
}




integer operator%(const integer & a, const integer & b) 
{ 
  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;
  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;

  if (b_sign == ZERO) error_handler(1,"mod by zero");

  if (a_sign == ZERO) return a;

  int rel = absolute_cmp(a.PTR->vec,a_used, b.PTR->vec, b_used);

  if (rel == 0) return 0;  // |a| == |b|
  if (rel  < 0) return a;  // |a| <  |b|

  // |a| > |b|

  int qsz = next_power(a_used - b_used + 1);
  int rsz = b.PTR->size;

  integer_rep* quot_ptr = new_integer_rep(qsz);
  integer_rep* rem_ptr = new_integer_rep(rsz);


  if (b_used == 1)
    { quot_ptr->used = DivByOneWord(a.PTR->vec, a_used, b.PTR->vec[0],
                                    quot_ptr->vec, rem_ptr->vec);
      rem_ptr->used = (rem_ptr->vec[0] == 0) ? 0 : 1;
     }
  else 
    { sz_t r_used;
      School_Div(a.PTR->vec, a_used, b.PTR->vec, b_used,
                 quot_ptr->vec,rem_ptr->vec,&r_used);
      rem_ptr->used = r_used;
     }

  delete_integer_rep(quot_ptr);

  if (rem_ptr->used == 0) 
     rem_ptr->sign = ZERO;
  else
     rem_ptr->sign = a_sign;

  return rem_ptr;
}




integer operator & (const integer & a, const integer & b)   // bitwise and
{
  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;

  if (a_sign == 0) return a;
  if (b_sign == 0) return b;

  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;

  integer_rep* and_ptr;

  word* ap = a.PTR->vec;
  word* bp = b.PTR->vec;
  word* a_stop = ap + a_used; 
  word* b_stop = bp + b_used; 

  int used;
  word* p;

  if (a_used <= b_used)
     { and_ptr = new_integer_rep(a.PTR->size);
       used = a_used;
       p = and_ptr->vec;
       while (ap < a_stop) *p++ = *ap++ & *bp++;
     }
  else
     { and_ptr = new_integer_rep(b.PTR->size);
       used = b_used;
       p = and_ptr->vec;
       while (bp < b_stop) *p++ = *ap++ & *bp++;
      }

  while (*--p == 0 && used > 0) used--;


  and_ptr->used = used;
  and_ptr->sign = (used > 0) ? a_sign : ZERO;


  return and_ptr;
}
 

integer operator | (const integer & a, const integer & b)   // bitwise or
{
  sz_t a_sign = a.PTR->sign;
  sz_t b_sign = b.PTR->sign;

  if (a_sign == 0) return a;
  if (b_sign == 0) return b;

  sz_t a_used = a.PTR->used;
  sz_t b_used = b.PTR->used;

  integer_rep* and_ptr;

  word* ap = a.PTR->vec;
  word* bp = b.PTR->vec;
  word* a_stop = ap + a_used; 
  word* b_stop = bp + b_used; 

  if (a_used >= b_used)
     { and_ptr = new_integer_rep(a.PTR->size);
       and_ptr->used = a_used;
       and_ptr->sign = a_sign;
       word* p = and_ptr->vec;
       while (bp < b_stop) *p++ = *ap++ | *bp++;
       while (ap < a_stop) *p++ = *ap++;
     }
  else
     { and_ptr = new_integer_rep(b.PTR->size);
       and_ptr->used = b_used;
       and_ptr->sign = a_sign;
       word* p = and_ptr->vec;
       while (ap < a_stop) *p++ = *ap++ | *bp++;
       while (bp < b_stop) *p++ = *bp++;
      }

  and_ptr->sign = a_sign;

  return and_ptr;
}
 


bool operator > (const integer & a, const integer & b)
{
  if (a.PTR == b.PTR) return false;

  int a_sign = a.PTR->sign;
  int b_sign = b.PTR->sign;

  if (a_sign > b_sign) return true;
  if (a_sign < b_sign) return false;

  /* the signs are equal */

  if (a_sign == ZERO) return false;

  if (a_sign == POSITIVE)
    return (absolute_cmp(a.PTR->vec,a.PTR->used,
                         b.PTR->vec,b.PTR->used) > 0);
  else
    return (absolute_cmp(a.PTR->vec,a.PTR->used,
                         b.PTR->vec,b.PTR->used) < 0);
}


bool operator < (const integer & a, const integer & b)
{
  if (a.PTR == b.PTR) return false;

  int a_sign = a.PTR->sign;
  int b_sign = b.PTR->sign;

  if (a_sign < b_sign) return true;
  if (a_sign > b_sign) return false;

  /* the signs are equal */

  if (a_sign == ZERO) return false;

  if (a_sign == POSITIVE)
    return (absolute_cmp(a.PTR->vec,a.PTR->used,
                         b.PTR->vec,b.PTR->used) < 0);
  else
    return (absolute_cmp(a.PTR->vec,a.PTR->used,
                         b.PTR->vec,b.PTR->used) > 0);
}


bool operator == (const integer & a, const integer & b)
{
  if (a.PTR == b.PTR) return true;

  int a_sign = a.PTR->sign;
  int b_sign = b.PTR->sign;

  if (a_sign != b_sign) return false;

  if (a_sign == ZERO) return true;

  return (absolute_cmp(a.PTR->vec,a.PTR->used,
                       b.PTR->vec,b.PTR->used) == 0);
}




integer gcd(const integer& a, const integer& b)
{ 
  // gcd is not efficient. See Knuth 2 for improvements. 

  int a_sign = a.PTR->sign;
  int b_sign = b.PTR->sign;
  
  if (a_sign == ZERO)
    if (b_sign == ZERO)
        return 1;
    else
        return abs(b);


  // a is non-zero

  if (b_sign == 0) return abs(a);
  

  // both a and b are non-zero

  integer u = abs(a);
  integer v = abs(b);

  if (u < v) v = v%u;
   
  while (sign(v) != 0)
  { integer tmp = u % v; 
    u = v;
    v = tmp;
   }

  return u;
}



//------------------------------------------------------------------------------
// member functions
//------------------------------------------------------------------------------

void integer::absolute() // *this = abs(*this), nur effizienter
{
  if (PTR->count == 1) PTR->sign = POSITIVE;
  else 
    *this = abs(*this); 
}

int integer::zeros() const
{ // gives the number of zeros at the end of the integer
  if (PTR->sign == ZERO) return 0;
  int k=0;
  while (!(PTR->vec[k])) k++;
  int len = k * WORD_LENGTH;
  word low = PTR->vec[k];
  if (! (low & 65535)) { low >>= 16; len += 16; }
  if (! (low & 255))  { low >>= 8; len += 8; }
  if (! (low & 15))  { low >>= 4; len += 4; }
  if (! (low & 3))  { low >>= 2; len += 2; }
  if (! (low & 1))  { low >>= 1; len += 1; }
  return len;
}

integer integer::div(const integer & b, integer& r) 
{ 
  // returns quotient and assings remainder to r

  sz_t a_sign = PTR->sign;
  sz_t b_sign = b.PTR->sign;
  sz_t a_used = PTR->used;
  sz_t b_used = b.PTR->used;

  if (b_sign == ZERO) error_handler(1,"division by zero");

  if (a_sign == ZERO) 
  { r = 0;
    return *this;
   }

  int rel = absolute_cmp(PTR->vec,a_used, b.PTR->vec, b_used);

  if (rel == 0) 
  { r = 0;
    return (a_sign == b_sign) ? 1 : -1; 
   }

  if (rel  < 0) 
  { r = *this;
    return 0;                     
   }

  // |a| > |b|

  int sz = next_power(a_used - b_used + 1);

  integer_rep* quot_ptr = new_integer_rep(sz);

  quot_ptr->sign = (a_sign == b_sign) ? POSITIVE : NEGATIVE;

  integer_rep* rem_ptr = new_integer_rep(b.PTR->size);

  sz_t r_used;

  if (b_used == 1)
    { quot_ptr->used = DivByOneWord(PTR->vec, a_used, b.PTR->vec[0],
                                    quot_ptr->vec, rem_ptr->vec);
      r_used = (rem_ptr->vec[0] == 0) ? 0 : 1;
     }
  else 
     School_Div(PTR->vec, a_used, b.PTR->vec, b_used,
                quot_ptr->vec,rem_ptr->vec,&r_used);

  rem_ptr->used = r_used;
  rem_ptr->sign = (r_used == 0) ? ZERO : a_sign;

  r = rem_ptr;

  return quot_ptr;
}



int integer::length()  const   // number of bits
{ if (PTR->sign == ZERO) return 0;
  int len = PTR->used - 1;
  word hi= PTR->vec[len];
  len *= WORD_LENGTH;
  while(hi)
  { hi >>= 1;
    len++;
   }
  return len;
 }
  

integer integer::operator-()  const
{ // unary minus
  if (PTR->sign == ZERO) return *this;
  integer_rep* p = copy_integer_rep(PTR);
  p->sign = -PTR->sign;
  return p;
}


integer integer::operator~()  const
{ // negation
  integer_rep* p = copy_integer_rep(PTR);
  word* wp = p->vec + p->used;
  while (--wp >= p->vec) *wp = ~*wp;
  return p;
}



bool integer::operator==(int l) const
{ int sig = PTR->sign;
  if (l==0) return sig == ZERO;

  if (PTR->used > 1) return false;

  if (l > 0 )
  { if (sig <= 0) return false;
    return PTR->vec[0] == word(l);
   }

  // l < 0
  if (sig >= 0) return false;
  return PTR->vec[0] == word(-l);
}


bool integer::operator<(int l) const
{ int sig = PTR->sign;
  if (l==0) return sig == NEGATIVE;

  if (l > 0 )
  { if (sig <= 0) return true;
    if (PTR->used > 1) return false;
    return PTR->vec[0] < word(l);
   }
  
  // l < 0
  if (sig >= 0) return false;
  if (PTR->used > 1) return true;
  return PTR->vec[0] > word(-l);
}


bool integer::operator>(int l) const
{ int sig = PTR->sign;
  if (l==0) return sig == POSITIVE;

  if (l < 0 )
  { if (sig >= 0) return true;
    if (PTR->used > 1) return false;
    return PTR->vec[0] < word(-l);
   }
  
  // l > 0
  if (sig <= 0) return false;
  if (PTR->used > 1) return true;
  return PTR->vec[0] > word(l);
}


double integer::todouble() const
{ 
  int i = PTR->used;
  if (i == 0) return 0;
  if (i > 32) return 2*MAXDOUBLE; //Infinity

  word*  v = PTR->vec;
  double d = 0;

  while (i > 0) d = pow32*d + v[--i];

  return (PTR->sign == NEGATIVE) ? -d : d;
}


integer integer::sqrt() const
{
  if (PTR->sign == NEGATIVE)
    error_handler(1, "negative argument in integer::sqrt");

  if (PTR->sign == ZERO) return *this;

  integer root = (*this) >> (length()/2);  // first approx.

  integer fix  = (root*root + *this)/(root << 1);

  while (fix != root)
  { fix  = (root*root + *this)/(root << 1);
    root = (fix*fix   + *this)/(fix << 1);
  }

  return root;  
}


integer integer::operator<<(long n)  const
{
  if (PTR->sign == ZERO || n == 0) return *this;

  int w_shift = int(n / WORD_LENGTH);
  int b_shift = int(n % WORD_LENGTH);
  int used = PTR->used + w_shift;
  int r = WORD_LENGTH - b_shift;

  if (b_shift && (PTR->vec[PTR->used-1] >> r)) used++;

  int sz = next_power(used);

  integer_rep* result;
  result = new_integer_rep(sz);
  result->used = used;
  result->sign = PTR->sign;

  word* q = PTR->vec;
  word* q_stop = q + PTR->used;
  word* p = result->vec;

  while (w_shift--) *p++ = 0;

  if (b_shift) 
    { word carry = 0;
      while (q < q_stop)
      { word B = *q++;
        carry |= (B << b_shift);
        *p++ = carry;
        carry = (B >> r);
       }
      if (carry) *p = carry;
     }
  else
     while (q < q_stop) *p++ = *q++;

  return result;
}



integer integer::operator>>(long n)  const
{
  if (n <= 0 || PTR->sign == ZERO) return *this;

  int sticks = int(n / WORD_LENGTH);
  int shifts = int(n % WORD_LENGTH);
  int used   = PTR->used - sticks;
 
  int sz = next_power(used);
 
  integer_rep* result;
 
  result = new_integer_rep(sz);
  result->sign = PTR->sign;
 
  word* p = result->vec + used;
  word* q = PTR->vec + PTR->used;
 
  if (shifts != 0)
    { word carry = 0;
      int r = WORD_LENGTH - shifts;
      while (p > result->vec)
      { word B = *--q;
        carry |= (B >> shifts);
        *--p = carry;
        carry = (B << r);
       }
     }
  else
     while (p > result->vec) *--p = *--q;
 
  if (result->vec[used-1] == 0) used--;
  if (used == 0) result->sign = ZERO;
 
  result->used = used;
 
  return result;
}
 


//------------------------------------------------------------------------------
// stream i/o
//------------------------------------------------------------------------------


void integer::hex_print(ostream& out)
{ if (PTR->sign == ZERO)
  { out << "0";
    return;
   }
  word* p = PTR->vec + PTR->used;
  out << string("%x",*--p);
  while (p > PTR->vec) out << string("%0x",*--p);
}



ostream & operator << (ostream & out, const integer & x) 
{
  if (x.PTR->sign == ZERO) 
  { out << "0";
    return out;
   }

  if (x.PTR->sign == NEGATIVE) out << "-";

  Print_Vector(x.PTR->vec,x.PTR->used,out);

  return out;
}



istream & operator >> (istream & in, integer & a) 
{
  bool negative = false;
  const int null = '0';
  char c;

  while (in.get(c) && isspace(c));

  if (c == '-') 
  { negative = true;
    while (in.get(c) && isspace(c));
   }

  if (isdigit(c)) 
  { a = c - '0';
    while (in.get(c) && isdigit(c)) a = 10*a + (c-null);
   }

  if (sign(a) != 0 && negative) a = -a;

  return in;

}



//------------------------------------------------------------------------------
// miscellaneous 
//------------------------------------------------------------------------------


integer_rep* copy_integer_rep(integer_rep* x)
{ integer_rep* result = new_integer_rep(x->size);
  result->sign = x->sign;
  result->used = x->used;
  Copy_Vector(result->vec,x->vec,x->size);
  return result;
}


integer abs(const integer & a)
{ if (a.PTR->sign >= 0) return a;
         integer_rep* ptr = copy_integer_rep(a.PTR);
         ptr->sign = POSITIVE;
         return ptr;
}


int log(const integer & a)
{
  if (a.PTR->sign != POSITIVE)
        error_handler(999,"domain error in log(integer)");

  int l  = a.PTR->used - 1;
  int lg = l * WORD_LENGTH;

  word hi = a.PTR->vec[l];

  while (hi >>= 1) lg++;

  return lg;
}


int integer::cmp(const integer & a, const integer & b)
{ int a_sign = a.PTR->sign;
  int b_sign = b.PTR->sign;

  if (a_sign < b_sign) return -1;
  if (a_sign > b_sign) return  1;

  // the signs are equal 

  if (a_sign == ZERO) return 0;

  if (a_sign == POSITIVE)
      return  absolute_cmp(a.PTR->vec,a.PTR->used,
                           b.PTR->vec,b.PTR->used);
  else
      return -absolute_cmp(a.PTR->vec,a.PTR->used,
                           b.PTR->vec,b.PTR->used);
}



integer integer::random(int n)  // return n-bit random integer
{ 
  sz_t w = n / WORD_LENGTH;
  sz_t r = n % WORD_LENGTH;
  if (r) w++;

  int sz = next_power(w);

  integer_rep* ptr = new_integer_rep(sz);
  ptr->sign = POSITIVE;
  ptr->used = w;

  for (int i = 0; i < w; i++) rand_int >> ptr->vec[i];

  if (r) ptr->vec[w-1] >>= (WORD_LENGTH-r);

  return ptr;
 }

