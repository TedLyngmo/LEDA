/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _basic.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/basic.h>

#if defined(unix)

#include <unistd.h>
#include <sys/times.h>
#include <sys/param.h>

#if !defined(HZ)
#define HZ 60
#endif

float used_time()
{ tms x;
  times(&x);
  return  float(x.tms_utime)/HZ;
}

#else

#include <time.h>

float used_time() { return  float(clock())/CLOCKS_PER_SEC; }

#endif


float used_time(float& T)
{ float t = T;
  T =  used_time();
  return  T-t;
}


void print_time() 
{ cout << string(" time = %.2f sec",used_time()) << endl; }

void print_time(string s) 
{ cout << s; print_time(); }



void wait(float sec) 
{ 
#if defined(unix)
    sleep(int(sec+0.5));
#endif
  //usleep(int(1000000*sec));
}
  
//------------------------------------------------------------------------------
// Input/Ouput
//------------------------------------------------------------------------------


int Yes(string s)
{ char answer = read_char(s);
  return ((answer == 'y') || (answer == 'Y'));
}

int read_int(string s)
{ int answer;
  char c;

  for(;;)
  { cout << s;
    cin >> answer;
    if (!cin) 
      { cin.clear();
        cin.get(c);
        cout << string("read_int: illegal input \"%c\"\n",c);
        if (c != '\n') skip_line(cin);
       }
    else  
       break;
   }

  skip_line(cin);

  return answer;
}

char read_char(string s)
{ char c;
  cout << s;
  cin.get(c);
  if (c != '\n') skip_line(cin);
  return c;
}

double read_real(string s)
{ double answer;
  cout << s;
  cin >> answer;
  skip_line(cin);
  return answer;
}

string read_line(istream& in)
{ string result;
  result.read_line(in); 
  return result;
 }

string read_string(string s)
{ cout << s << flush;
  return read_line(cin); 
 }

void skip_line(istream& s)
{ char c;
  while(s.get(c) &&  c != '\n');
 }


int     Yes()              { return Yes(""); }
int     read_int()         { return read_int(""); }
char    read_char()        { return read_char(""); }
double  read_real()        { return read_real(""); }
string  read_string()      { return read_string(""); } 


LEDA_SIG_PF catch_interrupts(LEDA_SIG_PF handler) 
{ error_handler(0,"catch_interrupts not implemented.");
  return handler; 
 }

