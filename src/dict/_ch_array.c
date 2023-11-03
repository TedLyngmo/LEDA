/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _ch_array.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#include <LEDA/impl/ch_array.h>

//------------------------------------------------------------------------------
//
//  hashing array with chaining and table doubling
//
//  only integer/pointer keys
//  no delete operation
//
//  S. Naeher (1994)
//
//------------------------------------------------------------------------------


ch_array_elem ch_array::STOP;

#define NIL GenPtr(this)

/* #define HASH(x) (table + (int(x) & table_size_1)) */

#define HASH(x) (table + (hash_fct(x) & table_size_1))


ch_array::ch_array(int n)
{ int ts = 1;
  while (ts < n) ts <<= 1;
  init_table(ts);
}


GenPtr ch_array::lookup(GenPtr x) const
{ ch_array_item p = HASH(x);
  STOP.k = x;
  while (p->k != x) p = p->succ;
  return (p == &STOP) ? nil : p;
}


GenPtr ch_array::access(GenPtr x) const
{ ch_array_item p = HASH(x);
  STOP.k = x;
  while (p->k != x) p = p->succ;
  if (p == &STOP) init_inf(x);
  else x = p->i;
  return x;
}


GenPtr& ch_array::access(GenPtr x)
{
  //if (x == NIL) error_handler(1,"internal error in ch_array");

  ch_array_item p = HASH(x);

  if (p->k == x) return p->i;

  if (p->k == NIL)
  { p->k = x;
    init_inf(p->i);
    return p->i;
   }

  STOP.k = x;
  ch_array_item q = p->succ;
  while (q->k != x) q = q->succ;
  if (q != &STOP) return q->i;


  // index x not present, insert it

  if (free == table_end)   // table full: rehash
  { rehash();
    p = HASH(x);
   }

  q = free++;
  q->k = x;
  init_inf(q->i);
  q->succ = p->succ;
  p->succ = q;

  return q->i;
}



void ch_array::destroy()
{ for(ch_array_item p = table; p < table_end; p++)
    if (p->k != NIL) clear_inf(p->i);
  delete[] table;
 }


void ch_array::init_table(int T)
{
  table_size = T;
  table_size_1 = T-1;

  table = new ch_array_elem[2*T];

  free      = table + T;
  table_end = free  + T;

  ch_array_item p=table;

  while (p<free)
  { p->k = NIL;
    p->succ = &STOP;
    p++;
   }

  while (p<table_end)
  { p->k = NIL;
    p++;
   }


}


#define INSERT(x,y)\
{ ch_array_item q = HASH(x);\
  if (q->k == NIL)\
    { q->k = x;\
      q->i = y;\
     }\
  else\
   { free->k = x;\
     free->i = y;\
     free->succ = q->succ;\
     q->succ = free++;\
   }\
}


void ch_array::rehash()
{
  ch_array_item old_table = table;
  ch_array_item old_table_end = table_end;

  init_table(2*table_size);

  for(ch_array_item p = old_table; p < old_table_end; p++)
      if (p->k != NIL) INSERT(p->k,p->i);

  delete[] old_table;
}


ch_array_item ch_array::first_item() const
{ ch_array_item p = table;
  while (p < table_end && p->k == NIL) p++;
  return (p < table_end) ? p : 0;
 }


ch_array_item ch_array::next_item(ch_array_item p) const
{ p++;
  while (p < table_end && p->k == NIL) p++;
  return (p < table_end) ? p : 0;
 }


ch_array::ch_array(const ch_array& D)
{
  init_table(D.table_size);

  for(ch_array_item p = D.table; p < D.table_end; p++)
  { if (p->k != GenPtr(&D))
    { INSERT(p->k,p->i);
      D.copy_inf(p->i);
     }
   }
}


ch_array& ch_array::operator=(const ch_array& D)
{
  destroy();
  init_table(D.table_size);

  for(ch_array_item p = D.table; p < D.table_end; p++)
  { if (p->k != GenPtr(&D))
    { INSERT(p->k,p->i);
      copy_inf(p->i);
     }
   }
  return *this;
}


void ch_array::print()
{ for (int i=0; i<table_size; i++)
  { ch_array_item p = table + i;
    if (p->k != NIL)
    { int l = 0;
      while(p != &STOP)
      { l++;
        p = p->succ;
       }
      std::cout << string("L(%d) = %d",i,l) << std::endl;
     }
   }
}

