/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph_objects.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_GRAPH_OBJECTS_H
#define LEDA_GRAPH_OBJECTS_H


#include <LEDA/impl/olist.h>


class node_struct;
typedef node_struct* node;

class edge_struct;
typedef edge_struct* edge;

class adj_link_struct1 : public obj_link {};
typedef adj_link_struct1* adj_link1;

class adj_link_struct2 : public obj_link {};
typedef adj_link_struct2* adj_link2;

class node_link_struct : public obj_link {};
typedef node_link_struct* node_link;

class edge_link_struct : public obj_link {};
typedef edge_link_struct* edge_link;

class aux_link_struct : public obj_link {};
typedef aux_link_struct* aux_link;

class graph;
class ugraph;
class planar_map;
class node_list;
inline graph* graph_of(node);
inline graph* graph_of(edge);
inline int    indeg(node);
inline int    outdeg(node);
inline int    degree(node);
inline int    index(node);

inline edge   First_Adj_Edge(node,int);
inline edge   Last_Adj_Edge(node,int);

void init_node_data(const graph&,int,GenPtr);

//------------------------------------------------------------------------------
// class node_struct: internal representation of nodes
//------------------------------------------------------------------------------

class node_struct : public aux_link_struct,  // used for node_list
                    public node_link_struct  // chaining all nodes
{

   friend class graph;
   friend class ugraph;
   friend class planar_map;
   friend class node_list;
   //friend template<int n> class b_node_pq;
   //friend template<class I> class node_pq;


   graph*    g;             // pointer to graph of node
   int       name;          // internal name (index)
   obj_list  adj_edges[2];  // lists of adjacent and incoming edges
   edge      adj_iterator;  //

public:

   GenPtr data[3];      // data[0]: GRAPH
                        // data[1]: node_pq
                        // data[2]: node_partition


   node_struct(GenPtr i=0)
   { data[0] = i; name = -1; g = nil; adj_iterator = nil; }

LEDA_MEMORY(node_struct)

friend inline graph* graph_of(node);
friend inline graph* graph_of(edge);
friend inline int    indeg(node);
friend inline int    outdeg(node);
friend inline int    degree(node);
friend inline int    index(node);

friend inline edge   First_Adj_Edge(node,int);
friend inline edge   Last_Adj_Edge(node,int);

friend void init_node_data(const graph&,int,GenPtr);

};



//------------------------------------------------------------------------------
// class edge_struct: internal representation of edges
//------------------------------------------------------------------------------

class graph;
class ugraph;
class planar_map;
inline graph* graph_of(edge);
inline node   source(edge);
inline node   opposite(node,edge);
inline node   target(edge);
inline int    index(edge);

class edge_struct : public adj_link_struct1,  // chaining of adjacent out-edges
                    public adj_link_struct2,  // chaining of adjacent in-edges
                    public edge_link_struct   // chaining of all edges
{
   friend class graph;
   friend class ugraph;
   friend class planar_map;

   int  name;          // internal name (index)
   node s;             // source node
   node t;             // target node
   edge rev;           // space for reverse edge (used by planar_map)

   GenPtr data[1];     // space for data (data[0] used by GRAPH)

   edge_struct(node v, node w, GenPtr i=0)
   { data[0] = i;
     name = -1;
     s = v;
     t = w;
   }

public:

LEDA_MEMORY(edge_struct)

friend inline graph* graph_of(edge);
friend inline node   source(edge);
friend inline node   opposite(node,edge);
friend inline node   target(edge);
friend inline int    index(edge);

};


inline int    outdeg(node v) { return v->adj_edges[0].length(); }
inline int    indeg(node v)  { return v->adj_edges[1].length(); }
inline int    degree(node v) { return indeg(v) + outdeg(v); }

inline int    index(node v)    { return v->name;    }
inline graph* graph_of(node v) { return v->g; }

inline graph* graph_of(edge e) { return e->s->g;   }
inline node   source(edge e)   { return e->s;      }
inline node   opposite(node v, edge e) { return (v==e->s) ? e->t : e->s; }

inline node   target(edge e)   { return e->t;      }
inline int    index(edge e)    { return e->name;    }


// parameterized access of adjacent edges (portable code?)
// outgoing (i=0) or incoming (i=1) edges

inline edge First_Adj_Edge(node v, int i)
{ GenPtr p = v->adj_edges[i].first() - i;
  return edge(p); }

inline edge Last_Adj_Edge(node v, int i)
{ GenPtr p = v->adj_edges[i].last() - i;
  return edge(p); }

inline edge Succ_Adj_Edge(edge e, int i)
{ GenPtr p = ((obj_link*)(((obj_link*)GenPtr(e))+i))->succ_item() - i;
  return edge(p); }

inline edge Pred_Adj_Edge(edge e, int i)
{ GenPtr p = ((obj_link*)(((obj_link*)GenPtr(e))+i))->pred_item() - i;
  return edge(p); }

inline edge Leda_Nil_Edge(int i)
{ GenPtr p = (obj_link*)0 - i;
  return edge(p); }


#endif
