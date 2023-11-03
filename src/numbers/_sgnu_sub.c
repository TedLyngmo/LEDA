/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _sgnu_sub.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#define MAX_WORD     0xFFFFFFFF

typedef unsigned long  word;
typedef unsigned int sz_t;



extern sz_t School_Sub(word *a, sz_t a_used, word *b, sz_t b_used, word* diff)
{
  /* compute diff = a - b    (a > b) */

  word* d_stop = diff + a_used;
  int n = b_used / 16;
  int r = b_used % 16;

  word  aa;
  word  bb;

#define SUB_LOOP_BODY {\
aa = *a++;\
bb = *b++;\
asm volatile ("subxcc  %2,%1,%0" : "=r"(aa) : "r"(bb), "0"(aa) );\
*diff++ = aa; }



  asm volatile ("addcc  %%g0,%%g0,%0" : "=r"(bb) : );

  switch (r)
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
     case  0: asm volatile ("addx  %%g0,%%g0,%0" : "=r"(bb) : );
    }


  while (n--)
  { asm volatile ("addcc 0xffffffff,%1,%0" : "=r"(bb) : "0"(bb) );
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY;
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY;
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY;
    SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY; SUB_LOOP_BODY;
    asm volatile ("addx  %%g0,%%g0,%0" : "=r"(bb) : );
   }

  if (diff != a) /* Copy_Vector(diff,a,a_used-b_used); */
      for(b=diff; b != d_stop; b++) *b = *a++;

  if (bb)
    while (--*diff == MAX_WORD) diff++;

  while (*--d_stop == 0) a_used--;

  return a_used;
}


