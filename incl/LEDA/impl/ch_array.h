/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  ch_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_CH_HASHING3_H
#define LEDA_CH_HASHING3_H

//------------------------------------------------------------------------------
// Hashing Array with Chaining
//
// S. Naeher  (1994)
//
//------------------------------------------------------------------------------

#include <LEDA/basic.h>
 

//------------------------------------------------------------------------------
// class ch_array_elem  
//------------------------------------------------------------------------------

class ch_array_elem 
{
  friend class ch_array;

  ch_array_elem* succ;
  GenPtr k;
  GenPtr i;

};

typedef ch_array_elem*  ch_array_item;


//--------------------------------------------------------------------
// class ch_array
//--------------------------------------------------------------------


class ch_array 
{

   static ch_array_elem STOP;

   ch_array_elem* table;
   ch_array_elem* table_end;
   ch_array_elem* free;

   ch_array_elem* iterator;

   int table_size;           
   int table_size_1;           
   int shift;

   virtual int  hash_fct(GenPtr)    const { return 0; }
   virtual void clear_inf(GenPtr&)  const { }
   virtual void copy_inf(GenPtr&)   const { }
   virtual void init_inf(GenPtr&)   const { }

   void init_table(int);
   void rehash();
   void destroy();


   public:

   GenPtr& access(GenPtr);
   GenPtr  access(GenPtr) const;
   GenPtr  lookup(GenPtr) const;

   void print();

   ch_array_item first_item() const;
   ch_array_item next_item(ch_array_item) const;

   ch_array& operator=(const ch_array&);
   ch_array(const ch_array&);
   ch_array(int n=1024); 

   virtual ~ch_array() { destroy(); }

   GenPtr key(ch_array_item p) const { return p->k; }
   GenPtr inf(ch_array_item p) const { return p->i; }

};


#endif
