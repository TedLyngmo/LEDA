/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _g_sort.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/



#include <LEDA/graph.h>

//--------------------------------------------------------------------------
// sorting
//--------------------------------------------------------------------------

static const graph_map*  NA;

static graph* GGG;

static int array_cmp_nodes(const node& x, const node& y)
{ return NA->cmp_entry(NA->array_read(x),NA->array_read(y)); }

static int array_cmp_edges(const edge& x, const edge& y)
{ return NA->cmp_entry(NA->array_read(x),NA->array_read(y)); }

static int graph_cmp_nodes(const node& x, const node& y)
{ return GGG->cmp_node_entry(x,y); }

static int graph_cmp_edges(const edge& x, const edge& y)
{ return GGG->cmp_edge_entry(x,y); }

static int (*CMP_NODES)(const node&, const node&);
static int (*CMP_EDGES)(const edge&, const edge&);


static int CMP_ADJ_LINKS(obj_link* p, obj_link* q)
{ edge e1 = edge(adj_link1(p));
  edge e2 = edge(adj_link1(q));
  return CMP_EDGES(e1,e2);
 }

/*
static int CMP_ADJ_LINKS1(obj_link* p, obj_link* q)
{ edge e1 = edge(adj_link2(p));
  edge e2 = edge(adj_link2(q));
  return CMP_EDGES(e1,e2);
 }
*/

static int CMP_EDGE_LINKS(obj_link* p, obj_link* q)
{ edge e1 = edge(edge_link(p));
  edge e2 = edge(edge_link(q));
  return CMP_EDGES(e1,e2);
 }

static int CMP_NODE_LINKS(obj_link* p, obj_link* q)
{ node u = node(node_link(p));
  node v = node(node_link(q));
  return CMP_NODES(u,v);
 }



void graph::sort_nodes(int (*f)(const node&, const node&))
{ CMP_NODES = f;
  V.sort(CMP_NODE_LINKS);
 }

void graph::sort_edges(int (*f)(const edge&, const edge&))
{ CMP_EDGES = f;
  E.sort(CMP_EDGE_LINKS);
  node v;
  forall_nodes(v,*this)
  { v->adj_edges[0].sort(CMP_ADJ_LINKS);
    //v->adj_edges[1].sort(CMP_ADJ_LINKS1);
   }
 }

void graph::sort_nodes(const graph_map& A)
{ NA = &A;
  sort_nodes(array_cmp_nodes);
 }

void graph::sort_edges(const graph_map& A)
{ NA = &A;
  sort_edges(array_cmp_edges);
 }


void graph::sort_nodes()
{ GGG = this;
  sort_nodes(graph_cmp_nodes);
 }

void graph::sort_edges()
{ GGG = this;
  sort_edges(graph_cmp_edges);
 }

