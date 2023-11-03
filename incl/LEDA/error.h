/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  error.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_ERROR_H
#define LEDA_ERROR_H


/*{\Mtext
\section{Error Handling} \label{Error Handling}

LEDA tests the preconditions of many (not all!) operations. Preconditions
are never tested, if the test takes more than constant time. If the test
of a precondition fails an error handling routine is called. It takes 
an integer error number $i$ and a $char*$ error message string $s$ as 
arguments. It writes $s$ to the diagnostic output (cerr) and terminates 
the program abnormally if $i \ne 0$.
Users can provide their own error handling function $handler$ by calling 
\smallskip
set\_error\_handler($handler$).
\smallskip 
After this function call $handler$ is used instead of the default error 
handler. $handler$ must be a function of type  $void\ \ handler(int,\ char*)$. 
The parameters are replaced by the error number and the error message
respectively. 
}*/


//------------------------------------------------------------------------------
// Error Handling
//------------------------------------------------------------------------------

typedef void (*LEDA_PEH)(int,const char*);   // Pointer to Error Handler

extern LEDA_PEH  p_error_handler;
extern LEDA_PEH  set_error_handler(LEDA_PEH);
extern void default_error_handler(int,const char*);
inline void error_handler(int i, const char* s)  { p_error_handler(i,s); }


#endif
