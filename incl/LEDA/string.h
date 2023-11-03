/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  string.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/basic.h>

#ifndef LEDA_STRING_H
#define LEDA_STRING_H

class string;
class string_rep : public handle_rep {

    friend class string;

      char*   s;

int dummy;

 string_rep(const char*);
 string_rep(const char*,const char*); //concat
 string_rep(char);

~string_rep() { delete[] s; }

friend string operator+(const string&, const string&);

};




/*{\Manpage {string} {} {Strings} }*/

class string  : public handle_base
{

/*{\Mdefinition
An instance $s$ of the data type $string$ is a sequence of characters
(type $char$).  The number of characters in the sequence is called the
length of $s$. A string of length zero is called the empty string.
Strings can be used wherever a \CC $char*$ string can be used.

$Strings$ differ from the \CC type $char*$ in several aspects: parameter
passing by value and assignment works properly (i.e., the value is passed
or assigned and not a pointer to the value) and $strings$ offer many additional
operations.
}*/

 friend class string_rep;
 friend class panel;

 static char* str_dup(const char*);
 static char* str_cat(const char*,const char*);
 static char* str_ncat(int, char**);

 string_rep*  ptr() const { return (string_rep*)PTR; }

 char** access_ptr() { return &(ptr()->s); }   // used by panel::string_item

 string(string_rep* rep) { PTR = rep; }  // for private use only

public:

/*{\Mcreation s }*/

string() { PTR = new string_rep(""); }
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized
            with the empty string.}*/


string(const char* p)       { PTR = new string_rep(p);}
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized
            with a copy of the \CC string $p$.}*/

// string(const char*, ...); // printf-like constructor
//
// That's what we want, but then (ARM page 326) a call string("xyz") is
// ambiguous. We first tried to use a dummy class "format_string" with
// a format_string(cosnt char*) constructor to resolve  the  ambiguity:
// string(format_string, ...);
// However, only g++ seems to be able to handle the case where the
// first argument is a class object (like format_string). For this reason
// we now provide a version of the string constructor for every possible
// second argument (this seems to work with all compilers).

 string(const char*, char, ...);
 string(const char*, unsigned char, ...);
 string(const char*, short,...);
 string(const char*, unsigned short, ...);
 string(const char*, int, ...);
 string(const char*, unsigned int, ...);
 string(const char*, long, ...);
 string(const char*, unsigned long, ...);
 string(const char*, float, ...);
 string(const char*, double, ...);
 string(const char*, void*, ...);

/* manual:
string(const char* format, ...);
*/
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized
            with the string produced by printf($format$,\dots). }*/

string(char c) { PTR = new string_rep(c);  }
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized
            with the one-character string ``$c$''.}*/


 string(int argc, char** argv) { PTR = new string_rep(str_ncat(argc,argv)); }


 string(const string& x) : handle_base(x)  {}
~string() {}
 string& operator=(const string& x) { handle_base::operator=(x); return *this; }

char*    operator~()   const { return str_dup(ptr()->s); }   // makes a copy !
char*    cstring()     const { return ptr()->s; }

operator const char*() const { return ptr()->s; }


/*{\Moperations 2. 4.7 }*/


int    length()          const;
/*{\Mop        returns the length of string $s$.}*/

char  operator[](int) const;
char& operator[](int i);
/*{\Marrop     returns the character at position $i$.\\
	       \precond{$0 \le i \le s$.length()$-$1.}}*/

string operator()(int i, int j)  const { return sub(i,j); }
/*{\Mfunop    returns the substring of $s$ starting at
	      position $\max(0,i)$ and ending at
              position $\min(j,s$.length()$-1)$.\\
              If $\min(j,s$.length()$-1) < \max(0,i)$
              then the empty string is returned.
	     }*/

string head(int i)               const { return sub(0,i-1); }
/*{\Mop       returns the first $i$ characters of $s$. }*/

string tail(int i)               const { return sub(length()-i,length()-1); }
/*{\Mop       returns the last $i$ characters of $s$. }*/

string sub(int,int)      const;



int pos(string s1, int i) const;
/*{\Mop       returns the minimum $j$ such that $j \ge i$ and $s_1$
              is a substring of $s$ starting at position $j$
	      (returns -1 if no such $j$ exists).}*/

int    pos(string s1) const { return pos(s1,0); }
/*{\Mop       returns pos$(s1,0)$.}*/

string insert(string s1, int i =0)     const;
string insert(int i, string s1)       const;
/*{\Mop       returns $s(0,i-1)$ + $s_1$ + $s(i,s$.length()$-1)$.}*/

string replace(const string& s1, const string& s2, int i=1) const;
/*{\Mopl      returns the string created from $s$ by replacing
	      the $i$-th occurrence of $s_1$ in $s$ by $s_2$. }*/

string replace(int i, int j , const string& s1) const;
/*{\Mopl     returns the string created from $s$ by replacing
	      $s(i,j)$ by $s_1$.\\ \precond{$i \leq j$.} }*/

string replace(int i, const string& s1) const { return replace(i,i,s1);  }
/*{\Mopl      returns the string created from $s$ by replacing
	      $s[i]$ by $s_1$. }*/

string replace_all(const string& s1, const string& s2) const
{ return replace(s1,s2,0); }
/*{\Mopl      returns the string created from $s$ by replacing
	      all occurrences of $s_1$ in $s$ by $s_2$.\\
              \precond{The occurrences of $s_1$ in $s$
               do not overlap (it's hard to say what the function returns
               if the precondition is violated.).} }*/

string del(const string& s1 , int i=1) const;
/*{\Mopl       returns $s$.replace($s_1,"",i$). }*/

string del(int i , int j) const;
/*{\Mop       returns $s$.replace($i,j,""$). }*/

string del(int i) const  { return del(i,i); }
/*{\Mop       returns $s$.replace($i,""$). }*/

string del_all(const string& s1) const  { return del(s1,0); }
/*{\Mop       returns $s$.replace\_all($s_1,""$). }*/


void   read(std::istream& I, char delim = ' ');
/*{\Mopl      reads characters from input stream $I$ into $s$
              until the first occurrence of character $delim$.}*/

void   read(char delim = ' ')           { read(std::cin,delim); }
/*{\Mop       read($cin$,$delim$). }*/

void   read_line(std::istream& I)    { read(I,'\n'); }
/*{\Mop       read($I$,'$\backslash$n'). }*/

void read_line() {read_line(std::cin);}
/*{\Mop       read\_line($cin$). }*/

string format(string) const;

friend string  operator+(const string& x ,const string& y);
/*{\Mbinopfunc     returns the concatenation of $x$ and $y$.}*/

string& operator+=(const string& x);
/*{\Mbinop     appends $x$  to $s$ and returns a reference to $s$.}*/

friend bool operator==(const string& x, const string& y);
/*{\Mbinopfunc     true iff $x$ and $y$ are equal.}*/

friend bool operator==(const string& x, const char* y);
friend bool operator!=(const string& x, const string& y);
/*{\Mbinopfunc     true iff $x$ and $y$ are not equal.}*/

friend bool operator!=(const string& x, const char* y);
friend bool operator< (const string& x, const string& y);
/*{\Mbinopfunc     true iff $x$ is lexicographically smaller than $y$.}*/

friend bool operator> (const string& x, const string& y);
/*{\Mbinopfunc     true iff $x$ is lexicographically greater than $y$.}*/

friend bool operator<=(const string& x, const string& y);
/*{\Mbinopfunc     returns $(x < y)\ ||\ (x == y)$.}*/

friend bool operator>=(const string& x, const string& y);
/*{\Mbinopfunc     returns $(x > y)\ ||\ (x == y)$.}*/

friend std::istream& operator>>(std::istream& I, string& s);
/*{\Mbinopfunc     read($I$,' '). }*/

friend std::ostream& operator<<(std::ostream& O, const string& s) ;
/*{\Mbinopfunc     writes string $s$ to the output stream $O$. }*/

static int cmp(const string& x, const string& y);

};

inline void Print(const string& x, std::ostream& out)      { out << x; }
inline void Read(string& x, std::istream& in)              { in  >> x; }

inline int  compare(const string& x, const string& y)
{ return string::cmp(x,y); }

inline const char* Type_Name(const string*) { return "string"; }


/*{\Mimplementation
Strings are implemented by \CC character vectors. All operations involving
the search for a pattern $s1$ in a string $s$ take time $O(s.lenght() * s1.length())$, $[\ ]$ takes constant time and all other operations on a
string $s$ take time $O(s.length())$.}*/


#endif
