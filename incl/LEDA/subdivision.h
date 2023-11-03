/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  subdivision.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_SUBDIVISION_H
#define LEDA_SUBDIVISION_H

#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/planar_map.h>

class SubDivision : public planar_map
{
  face outer_face;

  void* strip_ptr;   //pointer to strip_list
  
public:
  
  SubDivision(const graph&);

 ~SubDivision();

  point  position(node v)    const { return LEDA_ACCESS(point,inf(v)); }
  
  face   locate_point(point) const;
  void   print_stripes() const;
  
};


//------------------------------------------------------------------------------
//
// subdivision: generic subdivisions with face entries of type "I"
//
//------------------------------------------------------------------------------

/*{\Manpage {subdivision} {I} {Planar Subdivisions}}*/

template <class I>

class subdivision : public SubDivision {

/*{\Mdefinition
An instance $S$ of the parameterized data type \name\ is a
subdivision of the two-dimensional plane, i.e., an embedded planar graph
with straight line edges (see also sections \ref{Planar Maps} and
\ref{Parameterized Planar Maps}). With each node
$v$ of $S$ is associated a point, called the position of $v$ and with each
face of $S$ is associated an information from data type $I$, called the
information type of $S$.}*/

void copy_face_entry(GenPtr& x)  const { LEDA_COPY(I,x); }
void clear_face_entry(GenPtr& x) const { LEDA_CLEAR(I,x);  }

public:

/*{\Mcreation S }*/
   subdivision(GRAPH<point,I>& G) : SubDivision(G)   {}

/*{\Mcreate 
creates an instance \var\ of type \name\ and initializes it to
the subdivision represented by the parameterized directed graph $G$.
The node entries of $G$ (of type point)  define the positions of the
corresponding nodes of \var. Every face $f$ of \var\ is assigned the
information of one of its bounding edges in $G$.\\
\precond $G$ represents
a planar subdivision, i.e., a straight line embedded planar map.}*/

  ~subdivision()     { clear(); }


/*{\Moperations 2 4.5}*/

point position(node v) const {return SubDivision::position(v);}
/*{\Mop       returns the position of node $v$.}*/

I  inf(face f) const {return LEDA_ACCESS(I,SubDivision::inf(f));}
/*{\Mop       returns the information of face $f$.}*/

face locate_point(point p) const { return SubDivision::locate_point(p);}
/*{\Mop       returns the face containing point $p$.}*/

point  operator[](node v)  const {return LEDA_ACCESS(point,SubDivision::inf(v));}
I  operator[](face f)  const {return LEDA_ACCESS(I,SubDivision::inf(f));}

void print_node(node v) const { cout << "[" << index(v) <<"] (";
                                Print(position(v),cout);
                                cout << ") ";}

};

/*{\Mimplementation
Planar subdivisions are implemented by parameterized planar maps and an
additional data structure for point location based on persistent search trees
\cite{DSST89}. Operations position and inf take constant time, a locate\_point 
operation takes time $O(\log^2 n)$. Here $n$ is the number of nodes. 
The space requirement and the initialization time is $O(n^2)$.}*/

#endif
