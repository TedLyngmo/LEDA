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

#include <ctime>

float used_time() { return  float(clock())/CLOCKS_PER_SEC; }

#endif


float used_time(float& T)
{ float t = T;
  T =  used_time();
  return  T-t;
}


void print_time()
{ std::cout << string(" time = %.2f sec",used_time()) << std::endl; }

void print_time(string s)
{ std::cout << s; print_time(); }



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
  { std::cout << s;
      std::cin >> answer;
    if (!std::cin)
      { std::cin.clear();
          std::cin.get(c);
        std::cout << string("read_int: illegal input \"%c\"\n",c);
        if (c != '\n') skip_line(std::cin);
       }
    else
       break;
   }

  skip_line(std::cin);

  return answer;
}

char read_char(string s)
{ char c;
    std::cout << s;
    std::cin.get(c);
  if (c != '\n') skip_line(std::cin);
  return c;
}

double read_real(string s)
{ double answer;
    std::cout << s;
    std::cin >> answer;
  skip_line(std::cin);
  return answer;
}

string read_line(std::istream& in)
{ string result;
  result.read_line(in);
  return result;
 }

string read_string(string s)
{ std::cout << s << std::flush;
  return read_line(std::cin);
 }

void skip_line(std::istream& s)
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

