/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  misc.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MISC_H
#define LEDA_MISC_H

/*{\Manpage {misc} {} {Some Useful Functions}  }*/

/*{\Mtext
The following functions and macros are defined in \<LEDA/basic.h\>.
}*/


int    read_int();
int    read_int(string s);
/*{\Mfunc  prints $s$ and reads an integer from $cin$.}*/


double read_real();
double read_real(string s);
/*{\Mfunc  prints $s$ and reads a real number from $cin$.}*/


void skip_line(istream& =cin);
string read_line(istream& =cin);

string read_string();
string read_string(string s);
/*{\Mfunc  prints $s$ and reads a line from $cin$.}*/

char   read_char();
char   read_char(string s);
/*{\Mfunc  prints $s$ and reads a character from $cin$.}*/


int   Yes();
int   Yes(string s);
/*{\Mfunc  returns (read\_char($s$) == `y').}*/

float used_time();
/*{\Mfunc  returns the currently used cpu time in seconds.}*/

float used_time(float& T);
/*{\Mfunc  returns the cpu time used by the program from time
           $T$ up to this moment and assigns the current
           time to $T$.}*/

void  print_time(string s);
void  print_time();

void  wait(float sec);  
/*{\Mfunc suspends execution for $sec$ seconds.}*/



/*{\Mtext
\func void      print\_statistics() { }  
                           {prints a summary of the currently used memory.}
}*/



typedef int (*LEDA_SIG_PF) (...);
extern LEDA_SIG_PF catch_interrupts(LEDA_SIG_PF handler = nil);



// maximal and minimal values for some numerical types

inline int    Max_Value(int& x)     { return x =  MAXINT;   }
inline int    Min_Value(int& x)     { return x = -MAXINT;   }
inline float  Max_Value(float& x)   { return x =  MAXFLOAT; }
inline float  Min_Value(float& x)   { return x = -MAXFLOAT; }
inline double Max_Value(double& x)  { return x =  MAXDOUBLE;}
inline double Min_Value(double& x)  { return x = -MAXDOUBLE;}


#if defined(__TEMPLATE_FUNCTIONS__) &&  (__SUNPRO_CC != 0x400)

template <class T> inline 
T Max(const T& a, const T& b) { return (a>b) ? a : b; }
/*{\Mfunc     returns the maximum of $a$ and $b$. } */

template <class T> inline 
T Min(const T& a, const T& b) { return (a<b) ? a : b; }
/*{\Mfunc     returns the minimum of $a$ and $b$. } */

#else
#define Max(a,b)        (((a)>(b)) ? (a) : (b))
#define Min(a,b)        (((a)>(b)) ? (b) : (a))
#endif

#define newline         cout << endl


#endif
