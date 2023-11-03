/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _error.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/basic.h>

LEDA_PEH p_error_handler = default_error_handler;

void default_error_handler(int i, const char* s)
{ 
  cout << endl;

  if (i==0) 
   { cout << "(LEDA WARNING) " << s << endl;
     cout.flush();
    }
  else 
   { cout << "(LEDA ERROR) " << s << endl;
     cout.flush();
     abort();
    }
}

LEDA_PEH set_error_handler(LEDA_PEH handler)
{ LEDA_PEH old = error_handler;
  p_error_handler = handler;
  return old;
}

