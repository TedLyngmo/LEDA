/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _string.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/basic.h>
#include <LEDA/string.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>


//------------------------------------------------------------------------------
// String
//------------------------------------------------------------------------------

string_rep::string_rep(const char* p) 
{ s = string::str_dup(p); }

string_rep::string_rep(const char* p, const char* q) 
{ s = string::str_cat(p,q); }

string_rep::string_rep(char c)   
{ char p[2];
  p[0] = c;
  p[1] = '\0';
  s=string::str_dup(p); 
 }

char* string::str_dup(const char* p)
{ if (p==nil) error_handler(1,"string::str_dup: nil argument");
  char* q = new char[strlen(p)+1];
  if (q==nil) error_handler(1,"string::str_dup: out of memory");
  strcpy(q,p);
  return q;
}

char* string::str_cat(const char* p1, const char* p2)
{ char* q = new char[strlen(p1)+strlen(p2)+1];
  if (q==nil) error_handler(1,"string::str_cat: out of memory");
  strcpy(q,p1);
  strcat(q,p2);
  return q;
 }

char* string::str_ncat(int argc, char** argv)
{ int l=0;
  int i;
  for(i=0;i<argc;i++)  l += (strlen(argv[i])+1); 
  char* q = new char[l+1];
  if (q==nil) error_handler(1,"string::str_cat: out of memory");
  q[0] = 0;
  for(i=0;i<argc;i++)
  { strcat(q,argv[i]);
    strcat(q," ");
   }
  return q;
 }

void string::read(istream& s, char delim)
{ char buf[512];
  char* q = buf+511;
  bool go = true;

  operator=(""); // clear string

  while (s && go)
  { char* p;
    for(p = buf; p < q && s.get(*p); p++)
    { if (*p == delim) 
      { if (delim != '\n') s.putback(*p);
        go = false;
        break;
       }
     } 
    *p = '\0';
    operator+=(buf);
   }

}


#define STRING_CONSTRUCTOR(type)                         \
string::string(const char* f, type x, ...)               \
{ char buf1[512];                                        \
  char buf2[512];                                        \
  char* q = buf1;                                        \
  int first = 1;                                         \
  /* copy f to buf1 doubling all %'s except the first */ \
  while(*f != '\0')                                      \
    if ((*q++ = *f++) == '%')                            \
      if (!first) *q++ = '%';                            \
      else first = 0;                                    \
  *q = '\0';                                             \
  /* insert first argument x*/                           \
  sprintf(buf2,buf1,x);                                  \
  /* insert the rest */                                  \
  va_list arg_list;                                      \
  va_start(arg_list,x);                                  \
  vsprintf(buf1,buf2,arg_list);                          \
  PTR = new string_rep(buf1);                            \
}
    
  
STRING_CONSTRUCTOR(char)
STRING_CONSTRUCTOR(unsigned char)
STRING_CONSTRUCTOR(short)
STRING_CONSTRUCTOR(unsigned short)
STRING_CONSTRUCTOR(int)
STRING_CONSTRUCTOR(unsigned int)
STRING_CONSTRUCTOR(long)
STRING_CONSTRUCTOR(unsigned long)
STRING_CONSTRUCTOR(float)
STRING_CONSTRUCTOR(double)
STRING_CONSTRUCTOR(void*)


int string::length() const  { return strlen(cstring()); }

char string::operator[](int i) const
{ if (i<0 || length() <= i) 
    error_handler(1,"string[]: index out of range");
  return ptr()->s[i];
}

char& string::operator[](int i)
{ if (i<0 || length() <= i) 
    error_handler(1,"string[]: index out of range");
  if (refs() > 1) *this = cstring();    // disconnect
  return ptr()->s[i];
}

string& string::operator+=(const string& x) 
{ *this = *this + x; 
  return *this; 
 }

//friends

string  operator+(const string& x ,const string& y)
{ return new string_rep(x.cstring(),y.cstring()); }

bool operator==(const string& x, const string& y) 
{ if (x.ptr() == y.ptr()) return true;
  return strcmp(x.cstring(),y.cstring())==0; 
 }

bool operator==(const string& x, const char* y) 
{ return strcmp(x.cstring(),y)==0; }

bool operator!=(const string& x, const string& y) 
{ if (x.ptr() == y.ptr()) return false;
  return strcmp(x.cstring(),y.cstring())!=0; 
 }

bool operator!=(const string& x, const char* y) 
{ return strcmp(x.cstring(),y)!=0; }

bool operator<(const string& x, const string& y)  
{ return strcmp(x.cstring(),y.cstring())<0; }

bool operator>(const string& x, const string& y)  
{ return strcmp(x.cstring(),y.cstring())>0; }

bool operator<=(const string& x, const string& y) 
{ if (x.ptr() == y.ptr()) return true;
  return strcmp(x.cstring(),y.cstring())<=0; 
 }

bool operator>=(const string& x, const string& y) 
{ if (x.ptr() == y.ptr()) return true;
  return strcmp(x.cstring(),y.cstring())>=0; 
 }



istream& operator>>(istream& in, string& x)
{ char buf[256];
  char* p = buf;

  //skip leading white space (but not eol if in = cin)
  while (in.get(*p) && isspace(*p) && (&in != &cin || *p != '\n')); 

  if (in && *p != '\n')
  { p++;
    while (in.get(*p) && !isspace(*p)) p++;
   }

  *p = '\0';
  x.operator=(buf);
  return in;
}


ostream& operator<<(ostream& out, const string& x) 
{ return out << x.cstring(); }


string string::sub(int i, int j) const
{
  if (j >= length()) j = length() - 1;

  if (i < 0) i = 0;

  int l = j-i+1;

  if (l <= 0)  return string("");

  char* q = new char[l+1];
  strncpy(q,cstring()+i,l);
  q[l] = '\0';

  string s(q);
  delete[] q;
  return s;
}

int string::pos(string s1, int i) const
{
  int l1 = s1.length();
  if (l1==0 || i >= length()) return -1;

  char c = s1[0];

  char* q = cstring() + i;
  while (*q)
    { while ( (*q) && (*q != c) ) q++;
      if ( strncmp(q,s1.cstring(),l1)==0 ) break ;
      if (*q) q++;
     }
  return (*q) ? int((q - cstring())/ sizeof(char)) :  -1;
}

string string::insert(string s, int i) const
{ return sub(0,i-1) + s + sub(i,length()-1); }

string string::insert(int i, string s) const
{ return sub(0,i-1) + s + sub(i,length()-1); }


string string::del(int i, int j) const
{ return sub(0,i-1) + sub(j+1,length()-1); }

string string::del(const string& s, int n) const
{ return replace(s,"",n); }



string string::replace(int i, int j, const string& s) const
{ return sub(0,i-1) + s + sub(j+1,length()-1); }

string string::replace(const string& s1, const string& s2, int n) const 
{ 
  // replace n-th (all if n=0) occurrence of s1 by s2 

  int i = 0;
  int match = 0;  

  int l1 = s1.length();

  string tmp;

  for(;;)
  { int j = pos(s1,i);
    if (j < 0 ) break;
    tmp += sub(i,j-1);

    if (n==0 || ++match == n)
       tmp += s2;
    else
       tmp += s1;

    i = j+l1;
   }

  tmp += sub(i,length()-1);

  return tmp;

 }


string string::format(string f) const
{ 
  char buf[512];
  sprintf(buf,~f,cstring());
  return buf;

 }


int string::cmp(const string& x, const string& y) { return strcmp(x,y); }


