/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _sgnu_add.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#define MAX_WORD     0xFFFFFFFF

typedef unsigned long  word;
typedef unsigned int sz_t;

extern sz_t School_Add(word *a, sz_t a_used, word *b, sz_t b_used, word* sum)
{
  /* compute sum = a + b   (a_used >= b_used) */

  int n = b_used / 16;
  word* a_stop = a + a_used;
  word* s_last = sum + a_used;
  word aa;
  word bb;  /* is used for b and to save carry (asm) */

#define ADD_LOOP_BODY {\
aa = *a++; bb = *b++;\
asm volatile ("addxcc  %2, %1,  %0" : "=r"(aa)    : "0"(aa), "r"(bb) );\
*sum++ = aa; }

asm volatile ("addcc  %%g0,%%g0,%0" : "=r"(bb) :  ); 

  switch (b_used%16) {
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
      case  0: asm volatile ("addx  %%g0,%%g0,%0" : "=r"(bb) :  ); 
     }


  while (n--)
  { asm volatile ("addcc 0xffffffff,%1,%0" : "=r"(bb) : "0"(bb) );
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; 
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; 
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; 
    ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY; ADD_LOOP_BODY;
    asm volatile ("addx  %%g0,%%g0,%0" : "=r"(bb) : );
   }

  if (sum != a) 
  { /* Copy_Vector(sum,a,a_used-b_used); */
    for(b =sum; b < s_last; b++) *b = *a++;
    *s_last = 0;
   }

  if (bb) /* propagate carry */
  { while (++*sum == 0) sum++;
    if (sum == s_last) a_used++;
   }

  return a_used;
}

