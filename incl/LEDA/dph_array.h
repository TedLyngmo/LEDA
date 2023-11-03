/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  dph_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_DPHARRAY_H
#define LEDA_DPHARRAY_H

//------------------------------------------------------------------------------
// dph_array  
//------------------------------------------------------------------------------ 
#include <LEDA/basic.h> 
#include <LEDA/impl/slist.h> 
#include <LEDA/impl/dp_hash.h> 


template<class itype, class etype>

class dph_array : public dp_hash {

void clear_key(GenPtr& x)   const { LEDA_CLEAR(itype,x); }
void clear_inf(GenPtr& x)   const { LEDA_CLEAR(etype,x); }
void copy_key(GenPtr& x)    const { LEDA_COPY(itype,x);  }
void copy_inf(GenPtr& x)    const { LEDA_COPY(etype,x);  }

int  int_type() const { return 1; }

etype init;

SLIST def_list;
slist_item iterator;

public:

etype& operator[](itype y) 
{ stp i=lookup(Convert(y));
  if (i==nil) 
  { GenPtr p = Convert(init);
    i=insert(Convert(y),p);
    def_list.append(Convert(y));  
   }
  return LEDA_ACCESS(etype,info(i)); 
}

etype  operator[](itype y) const
{ stp i=lookup(Convert(y));
  if (i==nil) return init;
  else return LEDA_ACCESS(etype,info(i)); 
 }

bool defined(itype y) const { return (lookup(Convert(y)) != nil); }

slist_item first_item() const { return def_list.first(); }

void loop_to_succ(GenPtr& x) const 
{ x = def_list.succ(slist_item(x)); }

GenPtr forall_defined_test(GenPtr it, itype& x) const
{ if (it) x = LEDA_ACCESS(itype,def_list[slist_item(it)]);
  return it;
}



 dph_array() { }
 dph_array(int n,itype* I,etype* E): dp_hash(n,(GenPtr*)I,(GenPtr*)E) { }
 dph_array(etype i) { init=i; }

 dph_array(const dph_array<itype,etype>& A): dp_hash((dp_hash&)A) { init = A.init; }

virtual ~dph_array() { def_list.clear(); }
};


#endif
