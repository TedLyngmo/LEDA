/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  matrix.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_MATRIX_H
#define LEDA_MATRIX_H

//------------------------------------------------------------------------------
//  matrices
//------------------------------------------------------------------------------

#include <LEDA/basic.h>
#include <LEDA/vector.h>


/*{\Manpage {matrix} {}  {Real-Valued Matrices} }*/

class matrix {

/*{\Mdefinition
An instance of the data type $matrix$ is a matrix of double variables.
}*/

  vector** v;
  int  d1;
  int  d2;

  void     flip_rows(int,int);
  void     check_dimensions(const matrix&) const;
  double&  elem(int i, int j) const { return v[i]->v[j]; }
  double** triang(const matrix&, int&) const;

public:

/*{\Mcreation M }*/

 matrix(int n=0, int m=0);

/*{\Mcreate creates an instance $M$ of type $matrix$, $M$ is initialized to
           the $n \times m$ - zero matrix. }*/

  matrix(const matrix&);
  matrix(const vector&);
  matrix(int,int,double**);

  matrix& operator=(const matrix&);

 ~matrix();

  LEDA_MEMORY(matrix)


/*{\Moperations 2 4 }*/

int     dim1()  const  {  return d1; }
/*{\Mop  returns $n$, the number of rows of $M$. }*/

int     dim2()  const  {  return d2; }
/*{\Mop  returns $m$, the number of columns of $M$. }*/


vector& row(int i) const;
/*{\Mop  returns the $i$-th row of $M$ (an $m$-vector).
	 \precond  $0 \le i \le n-1$. }*/

vector  col(int i) const;
/*{\Mop  returns the $i$-th column of $M$ (an $n$-vector).
	 \precond  $0 \le i \le m-1$. }*/

matrix  trans() const;
/*{\Mop  returns  $M^T$ ($m\times n$ - matrix). }*/


matrix  inv()   const;
/*{\Mop  returns the inverse matrix of $M$.\\
	 \precond  $M$.det() $\neq$ 0. }*/

double  det()   const;
/*{\Mop  returns the determinant of $M$.\\
	 \precond  $M$ is quadratic. }*/


matrix solve(const matrix&) const;
vector solve(const vector& b) const { return vector(solve(matrix(b))); }
/*{\Mop  returns vector $x$ with $M\cdot x = b$.\\
	 \precond $M$.dim1() = $M$.dim2() = $b$.dim()
	 and $M$.det() $\neq$ 0. }*/


operator vector() const;

vector& operator[](int i)    const { return row(i); }

double& operator()(int i, int j);
/*{\Mfunop returns $M_{i,j}$. \\
	  \precond $0\le i\le n-1$ and $0\le j\le m-1$.}*/

double  operator()(int,int) const;

int     operator==(const matrix&)    const;
int     operator!=(const matrix& x)  const { return !(*this == x); }

matrix operator+(const matrix& M1);
/*{\Mbinop Addition. \\
 	   \precond $M$.dim1() = $M1$.dim1() and $M$.dim2() = $M1$.dim2().}*/

matrix operator-(const matrix& M1);
/*{\Mbinop Subtraction. \\
	   \precond $M$.dim1() = $M1$.dim1() and
	   $M$.dim2() = $M1$.dim2().}*/

matrix operator-(); // unary

matrix& operator-=(const matrix&);
matrix& operator+=(const matrix&);

matrix operator*(const matrix& M1);
/*{\Mbinop Multiplication. \\
	   \precond $M$.dim2() = $M1$.dim1().}*/

vector operator*(const vector& vec) { return vector(*this * matrix(vec)); }
/*{\Mbinop  Multiplication with vector.\\
	    \precond $M$.dim2() = $vec$.dim(). }*/

matrix operator*(double x);
/*{\Mbinop Multiplication with double x.}*/


friend std::ostream& operator<<(std::ostream& O, const matrix& M);
/*{\Mbinopfunc  writes matrix $M$ row by row to the output stream $O$. }*/

friend std::istream& operator>>(std::istream& I, matrix& M);
/*{\Mbinopfunc  reads matrix $M$ row by row from the input stream $I$. }*/

/*{\Mimplementation
Data type $matrix$ is implemented by two-dimensional arrays of double numbers.
Operations det, solve, and inv take time $O(n^3)$, dim1, dim2, row, and col
take constant time, all other operations take time $O(nm)$.
The space requirement is $O(nm)$.}*/

};


inline void Print(const matrix& m,std::ostream& out)
{ out << m.dim1() << " ";
  out << m.dim2() << " ";
  out << m;
}

inline void Read(matrix& m,std::istream& in)
{ int d1, d2;
  in >> d1 >> d2;
  matrix x(d1,d2);
  in >> x;
  m = x;
}

inline int compare(const matrix&, const matrix&)
{ error_handler(1,"compare not defined for type `matrix`");
  return 0;
 }


LEDA_TYPE_PARAMETER(matrix)

#endif
