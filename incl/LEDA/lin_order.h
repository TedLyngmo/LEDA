/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  lin_order.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_LIN_ORDER_H
#define LEDA_LIN_ORDER_H


//------------------------------------------------------------------------------
// Defining Linear Orders
//------------------------------------------------------------------------------

#define DEFINE_LINEAR_ORDER(type,cmp,new_type)\
struct new_type : public type\
{ new_type(type s)            : type(s) {}\
  new_type(const new_type& s) : type(s) {}\
  new_type() {}\
 ~new_type() {}\
};\
inline int compare(const new_type& x, const new_type& y) { return cmp(x,y); }


// INT<cmp>: int with user defined linear order cmp

typedef int (*CMP_INT_TYPE)(const int&, const int&);

template<CMP_INT_TYPE cmp> 
class INT {
  int p;
public:
  INT(const int i=0) { p = i;}
  operator int()     { return p; }
  friend int compare(const INT<cmp>& x, const INT<cmp>& y)
  { return cmp(x.p,y.p);}
};

#endif
