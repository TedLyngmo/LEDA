/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  simplex.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_SIMPLEX_H
#define LEDA_SIMPLEX_H

#include <LEDA/matrix.h>
#include <LEDA/list.h>


list<matrix> SIMPLEX(matrix A, int i, int j, int k, vector b, vector c);


#endif

