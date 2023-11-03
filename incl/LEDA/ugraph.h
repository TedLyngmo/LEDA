/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  ugraph.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_UGRAPH_H
#define LEDA_UGRAPH_H

#include <LEDA/graph.h>


//-----------------------------------------------------------------------------
// ugraph: base class for all undirected graphs
//-----------------------------------------------------------------------------


/*{\Manpage {ugraph} {} {Undirected Graphs}}*/

class ugraph : public graph {

/*{\Mdefinition
An instance $G$ of the data type $ugraph$ is an undirected graph as defined
in section \ref{Graphs}. }*/


protected:

edge new_edge(node v,node w, GenPtr x) {return graph::new_edge(v,w,x);}

public:

/*{\Mcreation U }*/

ugraph()  { undirected = true; }
/*{\Mcreate creates an instance \var\ of type \name\ and initializes it to
            the empty undirected graph. }*/


ugraph(const graph& a)  : graph(a) { undirected = true; }
ugraph(const ugraph& a) : graph(a) { undirected = true; }

~ugraph() { /* ~graph does the job */ }

//subgraph constructors
ugraph(ugraph&, const list<node>&, const list<edge>&);
ugraph(ugraph&, const list<edge>&);

ugraph& operator=(const ugraph& a)
{ graph::operator=(a); undirected = true; return *this; }

ugraph& operator=(const graph& a)
{ graph::operator=(a); undirected= true; return *this; }


/*{\Moperations 2 4.5 }*/

/*{\Mtext
see section \ref{Graphs}.
}*/

edge new_edge(node v, node w)
{ GenPtr x; init_edge_entry(x);
  return graph::new_edge(v,w,x);
 }

edge adj_succ(edge e,node v) const
{ edge r = (v==e->s) ? graph::adj_succ(e) : graph::in_succ(e);
  if (r==nil && v==e->s) r = graph::first_in_edge(v);
  return r;
 }
/*{\op        returns the successor of edge $e$ in the
	      adjacency list of $v$. }*/


edge adj_pred(edge e,node v) const
{ edge r = (v==e->s) ? graph::adj_succ(e) : graph::in_succ(e);
  if (r==nil && v==e->t) r = graph::last_adj_edge(v);
  return r;
 }
/*{\op        returns the predecessor of edge $e$ in the
	      adjacency list of $v$. }*/


edge cyclic_adj_succ(edge e,node v)  const
{ edge r = (v==e->s) ? graph::adj_succ(e) : graph::in_succ(e);
  if (r==nil)
     r = (v==e->s) ? graph::first_in_edge(v) : graph::first_adj_edge(v);
  return r;
 }
/*{\opl      returns the cyclic successor of edge $e$ in the
	      adjacency list of $v$. }*/


edge cyclic_adj_pred(edge e,node v) const
{ edge r = (v==e->s) ? graph::adj_pred(e) : graph::in_pred(e);
  if (r==nil)
     r = (v==e->s) ? graph::last_in_edge(v) : graph::last_adj_edge(v);
  return r;
 }
/*{\opl       returns the cyclic predecessor of edge $e$ in the
	      adjacency list of $v$. }*/

};

/*{\Mimplementation
see section \ref{Graphs}.
}*/



//------------------------------------------------------------------------------
// UGRAPH: generic ugraphs
//------------------------------------------------------------------------------

/*{\Manpage {UGRAPH} {vtype,etype} {Parameterized Ugraphs}}*/

template<class vtype, class etype>

class UGRAPH : public ugraph {

/*{\Mdefinition
A parameterized undirected graph $G$ is an undirected graph whose nodes and
contain additional (user defined) data (cf. \ref{Parameterized Graphs}). Every
node contains an element of a data type $vtype$, called the node type of $G$
and every edge contains an element of a data type $etype$ called the edge type
of $G$. }*/


char* node_type()  const { return LEDA_TYPE_NAME(vtype); }
char* edge_type()  const { return LEDA_TYPE_NAME(etype); }

void copy_node_entry(GenPtr& x) const  { LEDA_COPY(vtype,x); }
void copy_edge_entry(GenPtr& x) const  { LEDA_COPY(etype,x); }

void clear_node_entry(GenPtr& x) const { LEDA_CLEAR(vtype,x); }
void clear_edge_entry(GenPtr& x) const { LEDA_CLEAR(etype,x); }

void write_node_entry(std::ostream& o, GenPtr& x) const
{ LEDA_PRINT(vtype,x,o); o << std::endl;}
void write_edge_entry(std::ostream& o, GenPtr& x) const { LEDA_PRINT(etype,x,o);}

void read_node_entry(std::istream& i, GenPtr& x) { vtype X; Read(X,i); x=Copy(X); }
void read_edge_entry(std::istream& i, GenPtr& x) { etype Y; Read(Y,i); x=Copy(Y); }

void init_node_entry(GenPtr& x) { LEDA_CREATE(vtype,x); }
void init_edge_entry(GenPtr& x) { LEDA_CREATE(etype,x); }

void print_node_entry(std::ostream& o, GenPtr& x)  const
     { o << "("; LEDA_PRINT(vtype,x,o); o << ")"; }
void print_edge_entry(std::ostream& o, GenPtr& x)  const
     { o << "("; LEDA_PRINT(etype,x,o); o << ")"; }



public:

/*{\creation G }*/

UGRAPH() {}
/*{\Mcreate creates an instance \var\ of type \name\ and initializes it to the
    empty undirected graph.}*/

UGRAPH(const UGRAPH<vtype,etype>& a): ugraph(*(ugraph*)&a) {copy_all_entries();}
UGRAPH(const graph& a) : ugraph(a) { copy_all_entries(); }
~UGRAPH() { if (parent==0) clear_all_entries(); }

UGRAPH<vtype,etype>& operator=(const UGRAPH<vtype,etype>& a)
{ clear_all_entries();
  ugraph::operator=(*(ugraph*)&a);
  copy_all_entries();
  return *this;}

UGRAPH<vtype,etype>& operator=(const graph& a)
{ clear_all_entries();
  ugraph::operator=(a);
  copy_all_entries();
  return *this;
 }


/*{\Moperations 2 4.5}*/


/*{\Mtext
see section \ref{Parameterized Graphs}.
}*/

int cmp_node_entry(node x, node y) const { return compare(inf(x),inf(y)); }
int cmp_edge_entry(edge x, edge y) const { return compare(inf(x),inf(y)); }

vtype  inf(node v)         const { return LEDA_ACCESS(vtype,ugraph::inf(v)); }
/*{\Xop      returns the information of node $v$ }*/

etype  inf(edge e)         const { return LEDA_ACCESS(etype,ugraph::inf(e)); }
/*{\Xop    returns the information of edge $e$ }*/

vtype& operator[] (node v)       { return LEDA_ACCESS(vtype,entry(v)); }
vtype  operator[] (node v) const { return LEDA_ACCESS(vtype,ugraph::inf(v)); }
etype& operator[] (edge e)       { return LEDA_ACCESS(etype,entry(e)); }
etype  operator[] (edge e) const { return LEDA_ACCESS(etype,ugraph::inf(e)); }

void   assign(node v,vtype x) { operator[](v) = x; }
/*{\Xop    makes $x$ the information of node $v$ }*/

void   assign(edge e,etype x) { operator[](e) = x; }
/*{\Xop    makes $x$ the information of edge $e$ }*/


node new_node(vtype a) { return graph::new_node(Copy(a)); }
/*{\Xop    adds a new node $\<x\>$ to $G$ and returns it}*/

edge new_edge(node v, node w) { return ugraph::new_edge(v,w); }
/*{\Xop    inserts the undirected edge $\<\{v,w\},edef\>$ into
	   $G$ by appending it to the adjacency lists of
	   both $v$ and $w$ and returns it. Here, $edef$
           is the default value of type $etype$.}*/


edge new_edge(node v, node w, etype a) { return ugraph::new_edge(v,w,Copy(a)); }
/*{\Xopl   inserts the undirected edge $\<\{v,w\},x\>$ into
	   $G$ by appending it to the adjacency lists of
	   both $v$ and $w$ and returns it }*/

/*{\Xopl   NOT  SUPPORTED IN CURRENT RELEASE !!!!
           $after,\ rel\_pos\ dir2=after$)
           inserts the undirected edge $<\{v,w\},x>$ after
	   (if $dir1=after$) or before (if $dir1=before$)
	   the edge $e1$ into the adjacency list of $v$ and
	   after (if $dir2=after$) or before (if $dir2=$
	   $before$) the edge $e2$ into the adjacency list
	   of $w$ and returns it. }*/


void clear() { clear_all_entries(); ugraph::clear(); }

};


/*{\Mimplementation
see section \ref{Parameterized Graphs}.
.}*/


extern void complete_ugraph(ugraph&,int);
extern void random_ugraph(ugraph&,int,int);
extern void test_ugraph(ugraph&);

#ifndef __ZTC__
inline void complete_graph(ugraph& U,int n)     { complete_ugraph(U,n); }
inline void random_graph(ugraph& U,int n,int m) { random_ugraph(U,n,m); }
inline void test_graph(ugraph& U)               { test_ugraph(U); }
#endif


#endif
