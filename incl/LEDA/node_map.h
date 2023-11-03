/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_map.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_NODE_MAP_H
#define LEDA_NODE_MAP_H

//------------------------------------------------------------------------------
// node maps
//------------------------------------------------------------------------------


/*{\Manpage {node_map} {E} {Node Maps} }*/

#include <LEDA/graph.h>

#define node_data node_map

template <class E>
class node_map : public graph_map {

/*{\Mdefinition
An instance of the data type \name\ is a map for the nodes of a graph
$G$, i.e., equivalent to $map\<node,E\>$ (cf. \ref{Maps}). It can be
used as a dynamic variant of the data type $node\_array$
(cf. \ref{Node Arrays}). }*/


  E def;

int  cmp_entry(GenPtr x, GenPtr y) const { return LEDA_COMPARE(E,x,y); }

void clear_entry(GenPtr& x) const { LEDA_CLEAR(E,x); }
void copy_entry(GenPtr& x)  const { LEDA_COPY(E,x);  }
void init_entry(GenPtr& x)  const { x = Copy(def); }


  
public:

/*{\Mcreation M }*/

node_map() : graph_map(nil,0) {}
/*{\Mcreate  introduces a variable \var\ of type \name\ and initializes
             it to the map with empty domain. }*/

node_map(const graph& G) : graph_map(&G,G.max_node_index()+1)
{ init_table(); }
/*{\Mcreate  introduces a variable \var\ of type \name\ and initializes
             it with a mapping $m$ from the set of all nodes of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized by a call of the default constructor 
             of type $E$. }*/

node_map(const graph& G, E x) : graph_map(&G,G.max_node_index()+1)
{ def = x; init_table(); }
/*{\Mcreate  introduces a variable \var\ of type \name\ and initializes
             it with a mapping $m$ from the set of all nodes of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized with a copy of $x$. }*/

~node_map() {}

/*{\Moperations 1.3 4.3 }*/


void init()  { graph_map::init(nil,1); }
/*{\Mop      makes \var\ a node map with empty domain. }*/

void init(const graph& G)      
{ graph_map::init(&G,G.max_node_index()+1); 
  init_table(); }
/*{\Mop       makes \var\ to a mapping $m$ from the set of all nodes of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized by a call of the default 
              constructor of type $E$. }*/

void init(const graph& G, E x) 
{ graph_map::init(&G,G.max_node_index()+1); 
  def = x;
  init_table(); }
/*{\Mop       makes \var\ to a mapping $m$ from the set of all nodes of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized with a copy of $x$. }*/

E  operator()(node v) const {  return LEDA_ACCESS(E,graph_map::map_read(v)); }
E& operator[](node v) {  return LEDA_ACCESS(E,graph_map::map_access(v)); }
/*{\Marrop    returns the variable $M(v)$. }*/

};

/*{\Mimplementation
Node maps are implemented by an efficient hashing method based on the 
internal numbering of the nodes. An access operation takes expected 
time $O(1)$. 
}*/


#endif
