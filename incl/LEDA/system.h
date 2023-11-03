/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  system.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_SYSTEM_H
#define LEDA_SYSTEM_H

//----------------------------------------------------------------------------
// system/compiler dependent definitions
//----------------------------------------------------------------------------

// In the first section of this file some flags are defined indicating
// that certain c++ features are supported or not by particular compilers.
// If your compiler does not appear you may have to add it.
//
// __BUILTIN_BOOL__         : built-in boolean
// __NEW_SCOPE_RULES__      : new scope rules
// __ELSE_SCOPE_BUG__       : else-part does not begin a new scope
// __TEMPLATE_FUNCTIONS__   : function templates
// __EXPLICIT_DESTRUCTION__ : explicit destructor call
// __INPLACE_NEW__          : in-place new operator


//----------------------------------------------------------------------------
// define __BUILTIN_BOOL__ for compilers supporting type bool
// currently only g++ >= 2.6
//----------------------------------------------------------------------------

#if (__GNUC__ > 2 || __GNUC_MINOR__ >= 6)
#define __BUILTIN_BOOL__
#endif


//----------------------------------------------------------------------------
// define __NEW_SCOPE_RULES__ for compilers supporting  the new scope rules
// currently only g++ >= 2.7
//----------------------------------------------------------------------------

#if (__GNUC__ > 2 || __GNUC_MINOR__ >= 7)
#define __NEW_SCOPE_RULES__
#endif


//----------------------------------------------------------------------------
// define __ELSE_SCOPE_BUG__ for compilers not starting a new scope for the
// else-part of an if-statement
//----------------------------------------------------------------------------

#if defined(__ZTC__) && (__ZTC__ <= 0x310)
#define __ELSE_SCOPE_BUG__
#endif

#if defined(sgi) &&  ! defined(__GNUC__) && _COMPILER_VERSION < 602
#define __ELSE_SCOPE_BUG__
#endif

#if defined(hpux) &&  ! defined(__GNUC__)
#define __ELSE_SCOPE_BUG__
#endif


//----------------------------------------------------------------------------
// define  __TEMPLATE_FUNCTIONS__ except for compilers  having problems
// with function templates  (g++ <= 2.5, ztc <= 3.1, bcc <= 4.1, DEC C++, ...)
//----------------------------------------------------------------------------

#define __TEMPLATE_FUNCTIONS__

#if (__GNUC__ == 2 && __GNUC_MINOR__ <= 5)
#undef __TEMPLATE_FUNCTIONS__
#endif

#if defined(__BORLANDC__) && (__BORLANDC__ <= 0x410)
#undef __TEMPLATE_FUNCTIONS__
#endif

#if defined(__ZTC__) && (__ZTC__ <= 0x310)
#undef __TEMPLATE_FUNCTIONS__
#endif

#if defined(__DECCXX)
#undef __TEMPLATE_FUNCTIONS__
#endif



//----------------------------------------------------------------------------
// define __EXPLICIT_DESTRUCTION__ except for compilers not supporting
// this feature (AT&T cfront, borland, sgi, aix)
//----------------------------------------------------------------------------

#define __EXPLICIT_DESTRUCTION__

#if defined(__ATTCFRONT__)
#undef __EXPLICIT_DESTRUCTION__
#endif

#if defined(__BORLANDC__) && (__BORLANDC__ <= 0x410)
#undef __EXPLICIT_DESTRUCTION__
#endif

#if defined(sgi) && ! defined(__GNUC__) && _COMPILER_VERSION < 602
#undef __EXPLICIT_DESTRUCTION__
#endif

#if defined(_AIX) && ! defined(__GNUC__)
#undef __EXPLICIT_DESTRUCTION__
#endif


//----------------------------------------------------------------------------
// define  __INPLACE_NEW__ except for compilers not supporting this feature
//----------------------------------------------------------------------------

#define __INPLACE_NEW__

#if defined(__BORLANDC__) && (__BORLANDC__ <= 0x410)
#undef __INPLACE_NEW__
#endif

#if defined(__ZTC__) && (__ZTC__ <= 0x310)
#undef __INPLACE_NEW__
#endif


//------------------------------------------------------------------------------
//  Other Flags
//------------------------------------------------------------------------------

#if defined(__SVR4) || defined(__SOLARIS_23) || defined(__SOLARIS_24)
#if !defined(__svr4__)
#define __svr4__
#endif
#endif

#if defined(_AIX)
#define unix
#endif


#if defined(__STDC__)
#define STRINGIZE(x) #x
#else
#define STRINGIZE(x) "x"
#endif


#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#if defined(random)
#undef random
#endif


#endif
