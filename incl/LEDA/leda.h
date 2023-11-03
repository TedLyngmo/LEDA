/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  leda.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_LEDA_H
#define LEDA_LEDA_H

struct LEDA {

 LEDA();
~LEDA();

char* init_list;
static const char* version_string;
static int loop_dummy;
};

extern LEDA L_E_D_A;

#endif
