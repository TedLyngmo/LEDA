/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _leda.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/basic.h>
#include <cstdio>
#include <cstring>

int LEDA::loop_dummy = 0;

LEDA::LEDA()
{ init_list = getenv("LEDA_INIT"); }

LEDA::~LEDA()
{ if (init_list && strcmp(init_list,"statistics")==0)
  { if (version_string) printf("\n%s\n",version_string);
    print_statistics();
   }
 }

LEDA L_E_D_A;

