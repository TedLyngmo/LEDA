/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  iteration.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_ITERATION_H
#define LEDA_ITERATION_H

//------------------------------------------------------------------------------
//   forall_items(it,...)
//------------------------------------------------------------------------------

#define forall_items(x,S)\
for(x = (S).first_item(); x; x = (S).next_item(x) )


//------------------------------------------------------------------------------
//   forall(x,...)
//   Forall(x,...)
//   forall_defined(x,...)
//------------------------------------------------------------------------------

// As long as the new scoping rules are not supported we avoid multiple 
// declarations of the forall-loop variable by placing it in the else 
// part of a dummy if-statement. However, even this does not work with some 
// compilers (ztc, HP C++).
  
#if defined(__NEW_SCOPE_RULES__)

#define forall(x,S)\
for(GenPtr forall_loop_item = (S).first_item();\
(S).forall_loop_test(forall_loop_item,x);\
(S).loop_to_succ(forall_loop_item))

#define Forall(x,S)\
for(GenPtr forall_loop_item = (S).last_item();\
(S).forall_loop_test(forall_loop_item,x);\
(S).loop_to_pred(forall_loop_item))

#define forall_defined(x,S)\
for (GenPtr forall_loop_item = (S).first_item();\
(S).forall_defined_test(forall_loop_item,x);\
(S).loop_to_succ(forall_loop_item))

#else

#if !defined(__ELSE_SCOPE_BUG__)

#define forall(x,S)\
if (0); else \
for(GenPtr forall_loop_item = (S).first_item();\
(S).forall_loop_test(forall_loop_item,x);\
(S).loop_to_succ(forall_loop_item))

#define Forall(x,S)\
if (0); else \
for(GenPtr forall_loop_item = (S).last_item();\
(S).forall_loop_test(forall_loop_item,x);\
(S).loop_to_pred(forall_loop_item))

#define forall_defined(x,S)\
if (0); else \
for (GenPtr forall_loop_item = (S).first_item();\
(S).forall_defined_test(forall_loop_item,x);\
(S).loop_to_succ(forall_loop_item))

#else

#define forall(x,S)\
for((S).Forall_Loop_Item()=(S).first_item();\
(S).forall_loop_test((S).Forall_Loop_Item(),x);\
(S).loop_to_succ((S).Forall_Loop_Item()))

#define Forall(x,S)\
for((S).Forall_Loop_Item()=(S).last_item();\
(S).forall_loop_test((S).Forall_Loop_Item(),x);\
(S).loop_to_pred((S).Forall_Loop_Item()))

#define forall_defined(x,S)\
for((S).Forall_Loop_Item()=(S).first_item();\
(S).forall_defined_test((S).Forall_Loop_Item(),x);\
(S).loop_to_succ((S).Forall_Loop_Item()))

#endif

#endif


#endif
