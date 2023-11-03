/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  ch_map.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_CH_MAP_H
#define LEDA_CH_MAP_H

//------------------------------------------------------------------------------
// Hashing Map with Chaining & Table Doubling
//
// S. Naeher  (1994)
//
//------------------------------------------------------------------------------

#include <LEDA/basic.h>
 

//------------------------------------------------------------------------------
// class ch_map_elem  
//------------------------------------------------------------------------------

class ch_map_elem 
{
  friend class ch_map;

  unsigned long    k;
  GenPtr           i;
  ch_map_elem*  succ;
};

typedef ch_map_elem*  ch_map_item;


//--------------------------------------------------------------------
// class ch_map
//--------------------------------------------------------------------



class ch_map 
{

   static ch_map_elem STOP;

   ch_map_elem* table;
   ch_map_elem* table_end;
   ch_map_elem* free;
   ch_map_elem* iterator;

   int table_size;           
   int table_size_1;           
   int shift;

   virtual void clear_inf(GenPtr&)  const { }
   virtual void copy_inf(GenPtr&)   const { }
   virtual void init_inf(GenPtr&)   const { }


/*
   ch_map_elem*  HASH(unsigned long x)  const
   { return table + ((x>>shift) & table_size_1);  }
*/
   ch_map_elem*  HASH(unsigned long x)  const
   { return table + (x & table_size_1);  }

   void init_table(int);
   void rehash();
   GenPtr& access1(ch_map_item, unsigned long);

   public:

   void clear_entries();

   GenPtr& access(unsigned long);
   GenPtr  access(unsigned long) const;
   GenPtr  lookup(unsigned long) const;

   void print();

   void start_iteration();
   bool next_index(unsigned long&);

   ch_map_item first_item() const { return 0; }
   ch_map_item next_item(ch_map_item) const { return 0; }

   ch_map& operator=(const ch_map&);
   ch_map(const ch_map&);
   ch_map(int s = 0, int n=1024); 

   virtual ~ch_map() { delete[] table; }

};



inline GenPtr& ch_map::access(unsigned long x)
{ 
  ch_map_item p = HASH(x);
  if (p->k == x) 
    return p->i; 
  else
    if (p->k == 0xFFFFFFFF) // NIL
       { p->k = x;
         init_inf(p->i);
         return p->i;
        }
     else
       return access1(p,x);
 }

#endif
