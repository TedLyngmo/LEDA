/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  bool.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#if !defined(__BUILTIN_BOOL__)

/*{\Manpage {bool} {} {Boolean Values} }*/

/*{\Mdefinition
An instance of the data type $bool$ has either the value $true$ or $false$. 
The usual \CC logical operators \&\& (and), $||$ (or), ! (negation) are 
defined for $bool$. }*/

typedef char bool;

enum {false=0, true=1};

#endif
