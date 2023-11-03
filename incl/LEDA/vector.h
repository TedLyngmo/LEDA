/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  vector.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_VECTOR_H
#define LEDA_VECTOR_H

//------------------------------------------------------------------------------
//  vectors
//------------------------------------------------------------------------------

#include <LEDA/basic.h>


/*{\Manpage {vector} {} {Real-Valued Vectors}}*/

class vector
{

/*{\Mdefinition
An instance of the data type $vector$ is a vector of real variables.}*/

  friend class matrix;

  double* v;
  int d;

  void check_dimensions(const vector&) const;
 
public:

/*{\Mcreation v }*/

vector(); 
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the zero-dimensional vector.}*/

vector(int d); 
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the zero vector of dimension $d$.}*/ 

vector(double a, double b);
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the two-dimensional vector $(a,b)$.}*/

vector(double a, double b, double c);
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the three-dimensional vector $(a,b,c)$.}*/

  vector(const vector&);
 ~vector(); 
  vector& operator=(const vector&);


/*{\Moperations 2 4.5}*/

int    dim()    const { return d; }
/*{\Mop       returns the dimension of $v$.}*/ 

double length() const;
/*{\Mop      returns the Euclidean length of $v$.}*/
  
vector norm()   const { return *this/length(); }
  
double angle(const vector& w) const; 
/*{\Mop     returns the angle between $v$ and $w$.}*/

  
double& operator[](int i);
/*{\Marrop     returns $i$-th component of $v$.\\
	       \precond $0\le i \le v$.dim()$-$1.}*/
  
double  operator[](int) const;

vector& operator+=(const vector&);
vector& operator-=(const vector&);
  
vector  operator+(const vector& v1) const;
/*{\Mbinop     Addition.\\
	       \precond $v$.dim() = $v1$.dim().}*/

vector  operator-(const vector& v1) const;
/*{\Mbinop     Subtraction.\\
	       \precond $v$.dim() = $v1$.dim().}*/

double  operator*(const vector& v1) const;
/*{\Mbinop     Scalar multiplication.\\
               \precond $v$.dim() = $v1$.dim().}*/

vector  operator*(double r)        const;
/*{\Mbinop     Componentwise multiplication with double $r$.}*/

vector  operator-() const;
vector  operator/(double)        const;
  
bool     operator==(const vector& w) const;
/*{\Mbinop     Test for equality.}*/

bool     operator!=(const vector& w)  const { return !(*this == w); }
/*{\Mbinop     Test for inequality.}*/

  
/*
friend vector operator*(double f, const vector& v);
friend vector operator/(const vector& v, double f);
*/
  
friend ostream& operator<<(ostream& O, const vector& v);
/*{\Mbinopfunc  writes $v$ componentwise to the output stream $O$.}*/

friend istream& operator>>(istream& I, vector& v);
/*{\Mbinopfunc  reads $v$ componentwise from the input stream $I$.}*/

static int cmp(const vector&, const vector&);
 
LEDA_MEMORY(vector)

};

inline int compare(const vector& x, const vector& y)
{ return vector::cmp(x,y); }


inline void Print(const vector& v, ostream& out) 
{ out << v.dim() << " " << v; }

inline void Read(vector& v, istream& in)         
{ int d;
  in >> d;  
  vector x(d);
  in >> x;
  v = x;
}

LEDA_TYPE_PARAMETER(vector)

/*{\Mimplementation
Vectors are implemented by arrays of real numbers. All operations on a vector 
$v$ take time $O(v.dim())$, except for dim and $[\ ]$ which take constant
time. The space requirement is $O(v.dim())$.
}*/


#endif
