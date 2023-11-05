/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _graph.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/graph.h>

// constructors, destructor, operator=, ...

graph::graph() 
{ max_n_index = -1;
  max_e_index = -1; 
  parent = 0; 
  undirected = false;
 }


graph& graph::operator=(const graph& G)
{ 
  if (&G == this) return *this;

  clear();

  node* node_vec = new node[G.max_n_index+1];
  edge* edge_vec = new edge[G.max_e_index+1];

  node v;
  forall_nodes(v,G) 
     node_vec[v->name] = new_node(v->data[0]);

  edge e;
  forall_nodes(v,G) 
    forall_out_edges(e,v) 
       edge_vec[e->name] = new_edge(node_vec[e->s->name],
                                    node_vec[e->t->name], e->data[0]);

  parent = G.parent;
  undirected = G.undirected;

  delete node_vec;
  delete edge_vec;

  return *this;
}


void graph::copy_all_entries() const
{ node v;
  forall_nodes(v,*this) copy_node_entry(v->data[0]);
  edge e;
  forall_edges(e,*this) copy_edge_entry(e->data[0]);
}

void graph::clear_all_entries() const
{ node v;
  forall_nodes(v,*this) clear_node_entry(v->data[0]);
  edge e;
  forall_edges(e,*this) clear_edge_entry(e->data[0]);
}


graph::graph(const graph& G)  
{ 
  node* node_vec = new node[G.max_n_index+1];
  edge* edge_vec = new edge[G.max_e_index+1];

  max_n_index = max_e_index = -1;

  node v;
  forall_nodes(v,G) 
     node_vec[v->name] = new_node(v->data[0]);

  edge e;
  forall_nodes(v,G) 
    forall_out_edges(e,v) 
       edge_vec[e->name] = new_edge(node_vec[e->s->name],
                                    node_vec[e->t->name], e->data[0]);

  parent = G.parent;
  undirected = G.undirected;

  delete node_vec;
  delete edge_vec;
}


// subgraph constructors  (do not work for undirected graphs)

graph::graph(graph& G, const list<node>& nl, const list<edge>& el)
{ // construct subgraph (nl,el) of graph G

  parent = &G;
  node v,w;
  edge e;

  node* N = new node[G.max_n_index+1];

  forall(v,nl)
   { if (graph_of(v) != parent) 
      error_handler(1,"graph: illegal node in subgraph constructor");
     N[v->name] = new_node((GenPtr)v);
    }

  forall(e,el)
   { v = source(e);
     w = target(e);
     if ( graph_of(e)!= parent || N[v->name]==0 || N[w->name]==0 ) 
      error_handler(1,"graph: illegal edge in subgraph constructor");
     new_edge(N[v->name],N[w->name],(GenPtr)e);
    }

  undirected = G.undirected;

  delete N;

 }

graph::graph(graph& G, const list<edge>& el)
{ /* construct subgraph of graph G with edge set el  */

  node  v,w;
  edge  e;
  node* N = new node[G.max_n_index+1];

  forall_nodes(v,G) N[v->name] = 0;

  parent = &G;

  forall(e,el)
   { v = source(e);
     w = target(e);
     if (N[v->name] == 0) N[v->name] = new_node((GenPtr)v);
     if (N[w->name] == 0) N[w->name] = new_node((GenPtr)w);
     if ( graph_of(e) != parent )
      error_handler(1,"graph: illegal edge in subgraph constructor");
     new_edge(N[v->name],N[w->name],(GenPtr)e);
    }

  undirected = G.undirected;

  delete N;

 }


// destructor

void graph::clear()
{ 
  while ( ! E.empty() ) 
    delete edge(edge_link(E.pop()));

  while ( ! V.empty() ) 
  { node v = node(node_link(V.pop()));
    v->adj_edges[0].clear();
    v->adj_edges[1].clear();
    delete v;
   }

  max_n_index = max_e_index = -1;
}

list<node> graph::all_nodes() const
{ list<node> result;
  node v;
  forall_nodes(v,*this) result.append(v);
  return result;
}

list<edge> graph::all_edges() const
{ list<edge> result;
  edge e;
  forall_edges(e,*this) result.append(e);
  return result;
}

list<edge> graph::adj_edges(node v) const
{ list<edge> result;
  edge e;
  forall_adj_edges(e,v) result.append(e);
  return result;
}

list<edge> graph::in_edges(node v) const
{ list<edge> result;
  edge e;
  forall_in_edges(e,v) result.append(e);
  return result;
}



list<node> graph::adj_nodes(node v) const
{ list<node> result;
  edge e;
  forall_adj_edges(e,v) result.append(opposite(v,e));
  return result;
}



void init_node_data(const graph& G,int i, GenPtr x)
{ node v;
  forall_nodes(v,G) v->data[i] = x;
 }
