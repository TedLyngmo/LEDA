/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  hash.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_HASH_H
#define LEDA_HASH_H

//------------------------------------------------------------------------------
// hash dictionary (based on hashing with chaining)
//------------------------------------------------------------------------------

#include <LEDA/impl/ch_hash.h>

typedef ch_hash_item hash_item;


template<class ktype, class itype> 

class hash : public ch_hash {

int (*hash_ptr)(const ktype&);

int  int_type()              const { return LEDA_INT_TYPE(ktype); }
int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(ktype,x,y); }
int  hash_fct(GenPtr x)   const 
{ return (hash_ptr==0) ? LEDA_HASH(ktype,x):(*hash_ptr)(LEDA_ACCESS(ktype,x)); }
void clear_key(GenPtr& x) const { LEDA_CLEAR(ktype,x); }
void clear_inf(GenPtr& x) const { LEDA_CLEAR(itype,x); }
void copy_key(GenPtr& x)  const { LEDA_COPY(ktype,x); }
void copy_inf(GenPtr& x)  const { LEDA_COPY(itype,x); }
void print_key(GenPtr x)  const { LEDA_PRINT(ktype,x,cout); }
void print_inf(GenPtr x)  const { LEDA_PRINT(itype,x,cout); }

public:

hash_item lookup(ktype y)  const { return ch_hash::lookup(Convert(y)); }
int       defined(ktype x) const { return (lookup(x)) ? false : true; }
void      change_inf(hash_item it, itype i)
                               { ch_hash::change_inf(it,Convert(i)); }
hash_item insert(ktype y,itype x)
                               { return ch_hash::insert(Convert(y),Convert(x));}
void   del(ktype y)            { ch_hash::del(Convert(y)); } 
void   del_item(hash_item it)    { del(key(it)); } 
ktype  key(hash_item it)   const { return LEDA_ACCESS(ktype,ch_hash::key(it)); }
itype  inf(hash_item it)   const { return LEDA_ACCESS(itype,ch_hash::inf(it)); }

hash()                       { hash_ptr=0; }
hash(int (*f)(const ktype&)) { hash_ptr=f; }

hash(int s)  : ch_hash(s) { hash_ptr=0;}
hash(int s, int (*f)(const ktype&)) : ch_hash(s) { hash_ptr=f;}

~hash() { clear(); }

} ;


#endif
