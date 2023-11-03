/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _ch_map.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/impl/ch_map.h>

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

#define NIL 0xFFFFFFFF

ch_map_elem ch_map::STOP;


ch_map::ch_map(int sz, int n) 
{ 
  shift = 0;
  while (sz >>= 1) shift++;

  if (n < 512)
     init_table(512); 
  else
   { int ts = 1;
     while (ts < n) ts <<= 1;
     init_table(ts);
    }
}


GenPtr ch_map::lookup(unsigned long x) const
{ ch_map_item p = HASH(x);
  STOP.k = x;
  while (p->k != x) p = p->succ;
  return (p == &STOP) ? nil : p;
}


GenPtr ch_map::access(unsigned long x) const
{ ch_map_item p = HASH(x);
  GenPtr y;
  STOP.k = x;
  while (p->k != x) p = p->succ;
  if (p == &STOP) init_inf(y);
  else y = p->i;
  return y;
}



GenPtr& ch_map::access1(ch_map_item p, unsigned long x)
{ 
  STOP.k = x;
  ch_map_item q = p->succ; 
  while (q->k != x) q = q->succ;
  if (q != &STOP) return q->i;


  // index x not present, insert it

  if (free == table_end)   // table full: rehash
  { rehash();
    p = HASH(x);
   }

  if (p->k == nil)
  { p->k = x;
    init_inf(p->i);
    return p->i;
   }

  q = free++;
  q->k = x;
  init_inf(q->i);
  q->succ = p->succ;
  p->succ = q;
  return q->i;
}



void ch_map::clear_entries() 
{ for(ch_map_item p = table; p < free; p++)
    if (p->k != NIL) clear_inf(p->i);
 }


void ch_map::init_table(int T)
{ 
  table_size = T;
  table_size_1 = T-1;
  table = new ch_map_elem[2*T];
  free = table + T;
  table_end = free + T/2;

  for (ch_map_item p=table; p < free; p++) 
  { p->k = NIL;
    p->succ = &STOP;
   }
}


#define INSERT(x,y)\
{ ch_map_item q = HASH(x);\
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


void ch_map::rehash()
{ 
  ch_map_item old_table = table;
  ch_map_item old_table_mid = table+table_size;
  ch_map_item old_table_end = table_end;
  
  //init_table(2*table_size);
  init_table(4*table_size);

  ch_map_item p;
  for(p = old_table; p < old_table_mid; p++)
  { unsigned long x = p->k;
    if (x != NIL)
    { ch_map_item q = HASH(x);
      q->k = x;
      q->i = p->i;
     }
   }

  while (p < old_table_end)
  { unsigned long x = p->k;
    INSERT(x,p->i);
    p++;
   }

  delete[] old_table;
}


void ch_map::start_iteration()
{ iterator = table;
  while (iterator < table_end && iterator->k == NIL) iterator++;
 }


bool ch_map::next_index(unsigned long& p)
{ if (iterator == table_end) return false;
  p = iterator->k;
  iterator++;
  while (iterator < table_end && iterator->k == NIL) iterator++;
  return true;
 }


ch_map::ch_map(const ch_map& D)
{ 
  init_table(D.table_size);

  for(ch_map_item p = D.table; p < D.table_end; p++) 
  { if (p->k != NIL)
    { INSERT(p->k,p->i);
      D.copy_inf(p->i);
     }
   }
}


ch_map& ch_map::operator=(const ch_map& D)
{ 
  clear_entries();
  delete[] table;

  init_table(D.table_size);

  for(ch_map_item p = D.table; p < D.table_end; p++) 
  { if (p->k != NIL)
    { INSERT(p->k,p->i);
      copy_inf(p->i);
     }
   }
  return *this;
}


void ch_map::print()
{ cout << "shift = " << shift << endl;
  for (int i=0; i<table_size; i++)
  { ch_map_item p = table + i;
    if (p->k != NIL)
    { int l = 0;
      while(p != &STOP)
      { l++; 
        p = p->succ;
       }
      cout << string("L(%d) = %d",i,l) << endl;
     }
   }
}
  
