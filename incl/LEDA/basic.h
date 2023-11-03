/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  basic.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_BASIC_H
#define LEDA_BASIC_H

//------------------------------------------------------------------------------
// include iostreams, stdlib, values, etc.
//------------------------------------------------------------------------------

#include <iostream>
#include <cstdlib>

#if defined(unix) || defined(_AIX) || defined(__BORLANDC__)
#include <values.h>
#else
#define MAXINT          (int(0x7FFFFFFF))
#define MAXFLOAT	(float(3.37E+38))
#define MAXDOUBLE       (double(1.797693E+308))
#endif

// deccxx's definition of MAXINT seems to be broken
#if defined(__DECCXX)
#undef MAXINT
#define MAXINT  int(~(unsigned(1)<<(8*sizeof(int)-1)))
#endif


//------------------------------------------------------------------------------
// include basic LEDA header files
//------------------------------------------------------------------------------

#include <LEDA/system.h>
#include <LEDA/global.h>
#include <LEDA/bool.h>
#include <LEDA/leda.h>
#include <LEDA/error.h>
#include <LEDA/memory.h>
#include <LEDA/param_types.h>
#include <LEDA/handle_types.h>
#include <LEDA/string.h>
#include <LEDA/random.h>
#include <LEDA/iteration.h>
#include <LEDA/lin_order.h>
#include <LEDA/misc.h>


#endif
