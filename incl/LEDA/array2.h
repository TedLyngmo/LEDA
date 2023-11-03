/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  array2.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_ARRAY2_H
#define LEDA_ARRAY2_H

//--------------------------------------------------------------------------
// 2 dimensional arrays                                                   
//--------------------------------------------------------------------------

#include <LEDA/array.h>

/*{\Manpage {array2} {E} {Two Dimensional Arrays} }*/


template<class E> 

class array2 : private gen_array2 {

/*{\Mdefinition
An instance $A$ of the parameterized data type \name\ is a mapping from a 
set of pairs $I = [a..b] \times [c..d]$, called the index set of $A$, to the 
set of variables of data type $E$, called the element type of $A$, for two 
fixed intervals of integers $[a..b]$ and $[c..d]$.  $A(i,j)$ is called the 
element at position $(i,j)$.
}*/

void clear_entry(GenPtr& x) const { LEDA_CLEAR(E,x);  }
void copy_entry(GenPtr& x)  const { LEDA_COPY(E,x);   }
void init_entry(GenPtr& x)  const { LEDA_CREATE(E,x); }

public:

/*{\Mcreation  A }*/

array2(int a, int b, int c, int d) :gen_array2(a,b,c,d) { init(a,b,c,d);}

/*{\Mcreate      creates an instance \var\ of type \name\ with index set 
                 $[a..b]\times [c..d]$.  }*/

array2(int n, int m)             :gen_array2(n,m)     { init(0,n-1,0,m-1);}
/*{\Mcreate      creates an instance \var\ of type \name\ with index set 
                 $[0..n-1]\times [0..m-1]$.  }*/


~array2() { clear(); }



/*{\Moperations 1.5 5 }*/


E& operator()(int i, int j)  { return LEDA_ACCESS(E,row(i)->entry(j)); }

/*{\Mfunop      returns $A(i,j)$.\\
        	\precond $a\le i\le b$ and $c\le j\le d$.}*/


/*
E  operator()(int i, int j) const { return LEDA_ACCESS(E,row(i)->entry(j)); }
*/

int low1() const { return gen_array2::low1();}
/*{\Mop         returns $a$. }*/

int high1() const {return gen_array2::high1();}
/*{\Mop         returns $b$. }*/

int low2() const {return gen_array2::low2();}
/*{\Mop         returns $c$. }*/

int high2() const {return gen_array2::high2();}
/*{\Mop         returns $d$. }*/

};

/*{\Mimplementation
Two dimensional arrays are implemented by \CC vectors. All operations
take time $O(1)$, the space requirement is $O(|I|* sizeof(E))$.
}*/


#endif

