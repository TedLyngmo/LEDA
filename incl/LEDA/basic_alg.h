/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  basic_alg.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_BASIC_ALG_H
#define LEDA_BASIC_ALG_H

#include <LEDA/basic.h>

extern int    SELECT(int* l,int* r,int k); //returns k-biggest element in [l,r]
extern double SELECT(double*,double*,int);

#endif

