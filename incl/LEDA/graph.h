/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_GRAPH_H
#define LEDA_GRAPH_H

#include <LEDA/basic.h>
#include <LEDA/list.h>

#include <LEDA/graph_objects.h>
#include <LEDA/graph_map.h>


//------------------------------------------------------------------------------
// graph: base class for all graphs
//------------------------------------------------------------------------------

/*{\Manpage {graph} {} {Graphs} }*/

class graph {

/*{\Mdefinition
An instance $G$ of the data type $graph$ consists of a list $V$  of nodes
and a list $E$  of edges ($node$ and $edge$ are item types). A pair of nodes 
$(v,w) \in V\times V$ is associated with every edge $e\in E$; $v$ is called 
the {\em source} of $e$ and $w$ is called the {\em target} of $e$. Two lists 
of edges are associated with every node $v$: the list
$out\_edges(v) = \{e \in E\ \abs\ source(e) = v\}$ of edges starting in $v$, 
and the list $in\_edges(v) = \{e \in E\ \abs\ target(e) = v\}$ of
edges ending in $v$. Distinct graphs have disjoint node and edge sets. 
A graph with empty node list is called {\em empty}.

A graph is either {\em directed} or {\em undirected}; the main difference
between directed and undirected edges is the definition of {\em adjacent}. 
Undirected graphs are the subject of section \ref{Undirected Graphs}. 
In a directed graph an edge is adjacent to its source and in an undirected 
graph it is adjacent to its source and target. In a directed graph a node $w$ 
is adjacent to a node $v$ if there is an edge $(v,w) \in E$; in an undirected 
graph $w$ is adjacent to $v$ if there is an edge $(v,w)$ or $(w,v)$ in the 
graph. The {\em adjacency list} of a node $v$ is the list of edges adjacent 
to $v$; more precisely, for directed graphs the adjacency list of $v$ is 
equal to $out\_edges(v)$ and for undirected graphs it is the concatenation
of $out\_edges(v)$ and $in\_edges(v)$. 

The value of a variable of type node is either the node of some graph, or the
special value nil (which is distinct from all nodes), or is undefined (before the first assignment to the variable). A corresponding statement is true for
the variables of type edge.}*/


friend class ugraph;
friend class planar_map;

//list<node> V;              /* list of all nodes */
obj_list V;

//list<edge> E;              /* list of all edges */
obj_list E;

int max_n_index;      // maximal node index 
int max_e_index;      // maximal edge index

bool   undirected;    // true iff graph is undirected


/* space: 2 lists + 4 words + "virtual" = 2*20 + 5*4 bytes = 60 bytes */

virtual void init_node_entry(GenPtr&)  {}
virtual void init_edge_entry(GenPtr&)  {}

virtual void copy_node_entry(GenPtr&) const {}
virtual void copy_edge_entry(GenPtr&) const {}

virtual void clear_node_entry(GenPtr&) const {}
virtual void clear_edge_entry(GenPtr&) const {}

virtual void read_node_entry(istream&, GenPtr&) {}
virtual void read_edge_entry(istream&, GenPtr&) {}

virtual void write_node_entry(ostream&, GenPtr&) const {}
virtual void write_edge_entry(ostream&, GenPtr&) const {}

virtual void print_node_entry(ostream&, GenPtr&) const {}
virtual void print_edge_entry(ostream&, GenPtr&) const {}

virtual char* node_type() const { return "void"; }
virtual char* edge_type() const { return "void"; }

protected:

graph* parent;           // for subgraphs


void copy_all_entries() const;
void clear_all_entries() const;

public:

virtual int cmp_node_entry(node, node) const { return 0; }
virtual int cmp_edge_entry(edge, edge) const { return 0; }

/*{\Mcreation G }*/

graph();
/*{\Mcreate creates an object $G$ of type $graph$ and initializes it to 
            the empty directed graph. }*/

graph(const graph&);
graph& operator=(const graph&); 

virtual void clear();

virtual ~graph(){ clear(); }

//subgraph constructors

graph(graph&, const list<node>&, const list<edge>&);
graph(graph&, const list<edge>&);

   graph* super()        const;

   int max_node_index()   const;
   int max_edge_index()   const;
   int adj_edges_select() const { return undirected ? 1 : 0; }
   int  space() const;


/*{\Moperations 1.9 4.7 }*/

/*{\Mtext       
\bigskip
{\bf a) Access operations} }*/

   int  outdeg(node v)    const;
/*{\Mop     returns the outdegree of node $v$, i.e., the number of 
            edges starting at $v$ ($\abs out\_edges(v)\abs$). }*/

   int  indeg(node v)     const;
/*{\Mop     returns the indegree of node $v$, i.e., the number of 
            edges ending at $v$ ($\abs in\_edges(v) \abs$). }*/


   int  degree(node v)    const;
/*{\Mop     returns the degree of node $v$, i.e., the number of 
            edges starting or ending at $v$. }*/

   node source(edge e)    const;
/*{\Mop     returns the source node of edge $e$.}*/

   node target(edge e)    const;
/*{\Mop     returns the target node of edge $e$.}*/

   node opposite(node v, edge e) const;
/*{\Mop     returns a node of edge $e$ different from $v$ (returns $v$ 
            if $e$ has source and target equal to $v$) . }*/


   int  number_of_nodes() const;
/*{\Mop     returns the number of nodes in $G$. }*/

   int  number_of_edges() const;
/*{\Mop     returns the number of edges in $G$. }*/


   list<edge> all_edges()     const;
/*{\Mop     returns the list $E$ of all edges of $G$. }*/

   list<node> all_nodes()     const;
/*{\Mop     returns the list $V$ of all nodes of $G$. }*/

   list<edge> adj_edges(node v) const;
/*{\Mop     returns the list of all edges adjacent to $v$. }*/

   list<edge> in_edges(node v) const;
/*{\Mop     returns the list of all edges ending at $v$. }*/

   list<node> adj_nodes(node v) const;
/*{\Mop     returns the list of all nodes adjacent to $v$. }*/

   node first_node()      const;
/*{\Mop       returns the first node in $V$. }*/

   node last_node()       const;
/*{\Mop       returns the last node in $V$. }*/

   node choose_node()     const;
/*{\Mop       returns a node of $G$ (nil if $G$ is empty). }*/

   node succ_node(node v) const;
/*{\Mop       returns the successor of node $v$ in $V$
	      (nil if it does not exist). }*/

   node pred_node(node v) const;
/*{\Mop       returns the predecessor of node $v$ in $V$
	      (nil if it does not exist). }*/


   edge first_edge()      const;
/*{\Mop       returns the first edge in $E$. }*/

   edge last_edge()       const;
/*{\Mop       returns the last edge in $E$. }*/

   edge choose_edge()     const;
/*{\Mop       returns an edge of $G$ (nil if $G$ has no edges). }*/

   edge succ_edge(edge e) const;
/*{\Mop       returns the successor of edge $e$ in $E$
	      (nil if it does not exist). }*/

   edge pred_edge(edge e) const;
/*{\Mop       returns the predecessor of edge $e$ in $E$
	      (nil if it does not exist). }*/



   edge first_adj_edge(node v) const;
/*{\Mop       returns the first edge in the adjacency list of $v$. }*/

   edge last_adj_edge(node v)  const;
/*{\Mop       returns the last edge in the adjacency list of $v$. }*/

   edge adj_succ(edge e)  const;
/*{\Mop       returns the successor of edge $e$ in the
              adjacency list of node $source(e)$
	      (nil if it does not exist). }*/

   edge adj_pred(edge e)  const;
/*{\Mop       returns the predecessor of edge $e$ in the
              adjacency list of node $source(e)$
	      (nil if it does not exist). }*/

   edge cyclic_adj_succ(edge e) const;
/*{\Mop       returns the cyclic successor of edge $e$ in the
              adjacency list of node $source(e)$. }*/

   edge cyclic_adj_pred(edge e) const;
/*{\Mop       returns the cyclic predecessor of edge $e$ in the
	      adjacency list of node $source(e)$. }*/


   edge first_in_edge(node v)  const;
/*{\Mop       returns the first edge of $in\_edges(v)$. }*/

   edge last_in_edge(node v)   const;
/*{\Mop       returns the last edge of $in\_edges(v)$. }*/

   edge in_succ(edge e)  const;
/*{\Mop       returns the successor of edge $e$ in $in\_edges(target(e))$ 
              (nil if it does not exist). }*/

   edge in_pred(edge e)  const;
/*{\Mop       returns the predecessor of edge $e$ in $in\_edges(target(e))$ 
              (nil if it does not exist). }*/

   edge cyclic_in_succ(edge e) const;
/*{\Mop       returns the cyclic successor of edge $e$ in 
              $in\_edges(target(e))$. }*/


   edge cyclic_in_pred(edge e) const;
/*{\Mop       returns the cyclic predecessor of edge $e$ in 
              $in\_edges(target(e))$. }*/


protected:

   GenPtr& entry(node v);
   GenPtr& entry(edge e);
   GenPtr  inf(node v) const;
   GenPtr  inf(edge e) const;

   node new_node(GenPtr);
   edge new_edge(node, node, GenPtr);
   edge new_edge(edge, node, GenPtr, int dir=0);
   edge new_edge(edge, edge, GenPtr, int dir1=0, int dir2=0);

   void assign(node v,GenPtr x);
   void assign(edge e,GenPtr x);

public:


/*{\Mtext       
\bigskip
{\bf b) Update operations} }*/


   node new_node();
/*{\Mop        adds a new node to $G$ and returns it. }*/


   edge new_edge(node v, node w) 
   { GenPtr x; init_edge_entry(x);
     return new_edge(v,w,x);}
/*{\Mop         adds a new edge $(v,w)$ to \var\ by appending it to 
                $out\_edges(v)$ and to $in\_edges(w)$, and returns it.}*/


   edge new_edge(edge e, node w, int dir=after) 
   { GenPtr x; init_edge_entry(x);
     return new_edge(e,w,x,dir); }
/*{\Mopl       adds a new edge $(source(e), w)$ to \var\ by inserting it
               before ($dir$ = $before$) or after ($dir$ = $after$) edge $e$ into 
               $out\_edges(source(e))$ and appending it to $in\_edges(w)$,
               and returns it. Here $before$ and $after$ are predefined
               integer constants. }*/


   edge new_edge(edge e1, edge e2, int d1=after, int d2=after) 
   { GenPtr x; init_edge_entry(x);
     return new_edge(e1,e2,x,d1,d2); }
/*{\Mopl       adds a new edge $(source(e1), target(e2))$ to \var\ by 
               inserting it before (if $d1$ = $before$) or after (if $d1$ = $after$) 
               edge $e1$ into $out\_edges(source(e1))$ and before 
               (if $d2$ = $before$) or after (if $d2$ = $after$) edge $e2$ into
               $in\_edges(target(e2))$, and returns it. }*/ 



   void hide_edge(edge e);
/*{\Mop        removes edge $e$ from $out\_edges(source(e))$ and 
               from $in\_edges(target(e))$, but leaves it in the
               list of all edges $E$. }*/


   void restore_edge(edge e);
/*{\Mop        re-inserts $e$ into $out\_edges(source(e))$
               and into $in\_edges(target(e))$.\\
	       \precond $e$ must have been
               removed by a call of hide\_edge before. }*/


   void del_node(node v);
/*{\Mop       deletes node $v$ and all edges starting or ending at $v$
	      from $G$. }*/

   void del_edge(edge e);
/*{\Mop       deletes the edge $e$ from $G$. }*/

   void del_all_nodes(); 
/*{\Mop       deletes all nodes from $G$.  }*/

   void del_all_edges(); 
/*{\Mop       deletes all edges from $G$.  }*/

   edge rev_edge(edge e);
/*{\Mop       reverses the edge $e = (v,w)$ by removing it from $G$ 
              and inserting the edge $(w,v)$ into $G$; returns the 
              reversed edge. }*/

graph& rev();
/*{\Mop        all edges in $G$ are reversed. }*/

void sort_nodes(int (*cmp)(const node&, const node&));
/*{\Mopl      the nodes of $G$ are sorted according to the
	      ordering defined by the comparing function
	      $cmp$. Subsequent executions of forall\_nodes
	      step through the nodes in this order.
	      (cf.~TOPSORT1 in section \ref{Graph and network algorithms}). }*/


void sort_edges(int (*cmp)(const edge&, const edge&));
/*{\Mopl      the edges of $G$ and all out\_edges lists (but not the in\_edges lists)
              are sorted according to the
	      ordering defined by the comparing function
	      $cmp$. Subsequent executions of forall\_edges
	      step through the edges in this order.
              (cf.~TOPSORT1 in section \ref{Graph and network algorithms}). }*/

void sort_nodes(const graph_map& A);
/* manual:
void sort_nodes(const node_array<T>& A);
*/
/*{\Mopl      the nodes of $G$ are sorted according to the entries of
              node\_array $A$ (cf. section \ref{Node Arrays})
	      \precond $T$ must be linearly ordered. }*/

void sort_edges(const graph_map& A);
/* manual:
void sort_edges(const edge_array<T>& A);
*/
/*{\Mopl      the edges of $G$ are sorted according to the entries of 
              edge\_array $A$ (cf. section \ref{Edge Arrays})
	      \precond $T$ must be linearly ordered. }*/

void sort_edges();
void sort_nodes();




   list<edge> insert_reverse_edges();
/*{\Mop       for every edge $(v,w)$ in $G$ the reverse edge $(w,v)$ 
              is inserted into $G$. Returns the list of all inserted 
              edges.}*/


   void make_undirected() const { *(bool*)&undirected = true; }
/*{\Mop       make  $G$  undirected. }*/

   void make_directed() const  { *(bool*)&undirected = false; }
/*{\Mop       make  $G$  directed. }*/

   bool is_directed() const { return !undirected; }
/*{\Mop       returns true if $G$  directed. }*/

   bool is_undirected() const { return undirected; }
/*{\Mop       returns true if $G$  undirected. }*/


/*
virtual void clear();
*/
/*{\Mop       makes $G$ the empty graph. }*/

/*{\Mtext
\bigskip
{\bf c) Iterators}

With the adjacency list of every node $v$ a list iterator, 
called the adjacency iterator of $v$, is associated (cf. section \ref{Linear Lists}). There 
are operations to initialize, move, and read these iterators.  }*/


void init_adj_iterator(node v)        const;
/*{\Mop      sets the adjacency iterator of $v$ to undefined. }*/

bool next_adj_edge(edge& e,node v)    const;
/*{\Mopl     moves the adjacency iterator of $v$ forward by one edge
             (to the first item of the adjacency list of $v$ if it was undefined) 
             and returns $G$.current\_adj\_edge($e,v$).}*/

bool current_adj_edge(edge& e, node v) const;
/*{\Mopl     if the adjacency iterator of $v$ is defined then the
             corresponding edge is assigned to $e$ and true is returned, otherwise,
             false is returned.}*/

bool next_adj_node(node& w, node v)    const;
/*{\Mopl     if $G$.next\_adj\_edge($e$, $v$) = true then 
	     $target(e)$ is assigned to $w$ and true is returned else false is returned.}*/

bool current_adj_node(node& w, node v) const;
/*{\Mopl     if $G$.current\_adj\_edge($e$, $v$) = true then
	     $target(e)$ is assigned to $w$ and true is returned, else false is returned.}*/
 
 void reset() const;
/*{\Mop      sets all iterators in $G$ to undefined. }*/


void init_node_iterator() const {}
void init_edge_iterator() const {}

/*{\Mtext
\bigskip
{\bf d) I/O Operations} }*/

void write(ostream& O = cout) const;
/*{\Mopl      writes $G$ to the output stream $O$.}*/

void write(string s) const;
/*{\Mop       writes $G$ to the file with name $s$.}*/


int  read(istream& I = cin);
/*{\Mopl      reads a graph from the input stream $I$ and assigns
              it to \var. }*/

int  read(string s);
/*{\Mop       reads a graph from the file with name $s$ and assigns
              it to \var. }*/


void print_node(node v, ostream& O = cout)  const;
/*{\Mopl      prints node $v$ on the output stream $O$. }*/


virtual void print_edge(edge e, ostream& O = cout) const;
/*{\Mopl      prints  edge $e$ on the output stream $O$. If \var\
              is directed $e$ is represented by an arrow 
              pointing from source to target. If \var\ is undirected
              $e$ is printed as an undirected line segment. }*/


void print(string s, ostream& O = cout) const;
/*{\Mopl      pretty-prints $G$ with header line $s$ on the output 
              stream $O$. }*/

void print(ostream& O) const { print("",O); }
/*{\Mop       pretty-prints $G$ on the output stream $O$. }*/

void print() const { print("");   }
/*{\Mop       pretty-prints $G$ on the standard ouput stream $cout$. }*/


};

inline int  graph::outdeg(node v) const { return v->adj_edges[0].length(); }
inline int  graph::indeg(node v)  const { return v->adj_edges[1].length(); }
inline int  graph::degree(node v) const { return outdeg(v) + indeg(v); }

inline node graph::source(edge e)    const   { return e->s; }
inline node graph::target(edge e)    const   { return e->t; }
inline node graph::opposite(node v,edge e) const {return (v==e->s)?e->t:e->s;}

inline graph* graph::super()       const   { return parent; }
inline int graph::max_node_index() const   { return max_n_index; }
inline int graph::max_edge_index() const   { return max_e_index; }

inline int  graph::number_of_nodes() const   { return V.length(); }
inline int  graph::number_of_edges() const   { return E.length(); }

inline GenPtr& graph::entry(node v)        { return v->data[0]; }
inline GenPtr& graph::entry(edge e)        { return e->data[0]; }
inline void graph::assign(node v,GenPtr x) { v->data[0] = x; }
inline void graph::assign(edge e,GenPtr x) { e->data[0] = x; }

inline GenPtr  graph::inf(node v) const { return v->data[0]; }
inline GenPtr  graph::inf(edge e) const { return e->data[0]; }


inline node graph::first_node()   const { return node(node_link(V.first())); }
inline node graph::last_node()    const { return node(node_link(V.last())); }
inline node graph::choose_node()  const { return first_node(); }

inline node graph::succ_node(node v)  const  
{ return node(node_link(V.succ(node_link(v)))); }

inline node graph::pred_node(node v)  const  
{ return node(node_link(E.pred(node_link(v)))); }


inline edge graph::first_edge()   const { return edge(edge_link(E.first())); }
inline edge graph::last_edge()    const { return edge(edge_link(E.last())); }
inline edge graph::choose_edge()  const { return first_edge(); }

inline edge graph::succ_edge(edge e)  const  
{ return edge(edge_link(E.succ(edge_link(e)))); }

inline edge graph::pred_edge(edge e)  const  
{ return edge(edge_link(E.pred(edge_link(e)))); }



inline edge graph::first_adj_edge(node v) const
{ edge e = First_Adj_Edge(v,0);
  if (undirected && e == Leda_Nil_Edge(0)) 
  { e = First_Adj_Edge(v,1);
    if (e == Leda_Nil_Edge(1)) e = nil;
   }
  return e ;
 }

inline edge graph::last_adj_edge(node v) const
{ int index = (undirected) ? 1 : 0;
  edge e = Last_Adj_Edge(v,index);
  if (undirected && e == Leda_Nil_Edge(index)) 
  { e = Last_Adj_Edge(v,1-index);
    if (e == Leda_Nil_Edge(1-index)) e = nil;
   }
  return e;
 }

inline edge graph::adj_succ(edge e) const
{ return edge(adj_link1(adj_link1(e)->succ_item())); }

inline edge graph::adj_pred(edge e) const
{ return edge(adj_link1(adj_link1(e)->pred_item())); }

inline edge graph::cyclic_adj_succ(edge e) const 
{ edge e1 = adj_succ(e);
  return e1 ? e1 : first_adj_edge(e->s); }

inline edge graph::cyclic_adj_pred(edge e) const 
{ edge e1 = adj_pred(e);
  return e1 ? e1 : last_adj_edge(e->s); }



inline edge graph::first_in_edge(node v) const
{ return edge(adj_link2(v->adj_edges[1].first())); }

inline edge graph::last_in_edge(node v)  const
{ return edge(adj_link2(v->adj_edges[1].last())); }

inline edge graph::in_succ(edge e)  const
{ return edge(adj_link2(adj_link2(e)->succ_item())); }

inline edge graph::in_pred(edge e)  const
{ return edge(adj_link2(adj_link2(e)->pred_item())); }

inline edge graph::cyclic_in_succ(edge e) const 
{ edge e1 = in_succ(e);
  return e1 ? e1 : first_in_edge(e->t); }

inline edge graph::cyclic_in_pred(edge e) const 
{ edge e1 = in_pred(e);
  return e1 ? e1 : last_in_edge(e->t); }



inline void graph::init_adj_iterator(node v) const 
{ v->adj_iterator = nil; }

inline bool  graph::current_adj_edge(edge& e,node v) const 
{ return (e = v->adj_iterator) != nil;}

inline bool  graph::next_adj_edge(edge& e,node v) const 
{ if (v->adj_iterator) 
      e = v->adj_iterator = adj_succ(v->adj_iterator);
  else
      e = v->adj_iterator = first_adj_edge(v);
  return  (e) ? true : false;
 }

inline bool graph::next_adj_node(node& w,node v)  const
{ edge e;
  if (next_adj_edge(e,v))
  { //w = (v==e->s) ? e->t : e->s;  // ugraph
    w = e->t;
    return true; 
   }
  else return false;
 }
   
inline bool graph::current_adj_node(node& w,node v)  const
{ edge e;
  if (current_adj_edge(e,v))
  { //w = (v==e->s) ? e->t : e->s;  // ugraph
    w = e->t;
    return true; 
   }
  else return false;
}
   

//------------------------------------------------------------------------------
// Iteration   (macros)
//------------------------------------------------------------------------------

/*{\Mtext            
\bigskip
{\bf e) Iteration} }*/


#define forall_nodes(v,g) for (v=(g).first_node(); v; v=(g).succ_node(v) )
/*{\Mtext            
\bigskip
{\bf forall\_nodes}($v,G$)\\ 
$\{$ ``the nodes of $G$ are successively assigned to $v$" $\}$ }*/

#define forall_edges(e,g) for (e=(g).first_edge(); e; e=(g).succ_edge(e) )
/*{\Mtext            
\bigskip
{\bf forall\_edges}($e,G$)\\
$\{$ ``the edges of $G$ are successively assigned to $e$" $\}$ }*/

#define Forall_nodes(v,g) for (v=(g).last_node(); v; v=(g).pred_node(v) )
/*{\Mtext            
\bigskip
{\bf Forall\_nodes}($v,G$)\\
$\{$ ``the nodes of $G$ are successively assigned to $v$ in reverse order" $\}$ }*/

#define Forall_edges(e,g) for (e=(g).last_edge(); e; e=(g).pred_edge(e) )
/*{\Mtext            
\bigskip
{\bf Forall\_edges}($e,G$)\\ 
$\{$ ``the edges of $G$ are successively assigned to $e$ in reverse order" $\}$ }*/


#define forall_out_edges(e,v)\
for(e = First_Adj_Edge(v,0); e != Leda_Nil_Edge(0); e = Succ_Adj_Edge(e,0))
/*{\Mtext
\bigskip
{\bf forall\_out\_edges}($e,w$)\\
$\{$ ``the edges of $out\_edges(w)$ are successively assigned to $e$" $\}$ }*/

#define forall_in_edges(e,v)\
for(e = First_Adj_Edge(v,1); e != Leda_Nil_Edge(1); e = Succ_Adj_Edge(e,1))
/*{\Mtext
\bigskip
{\bf forall\_in\_edges}($e,w$)\\
$\{$ ``the edges of $in\_edges(w)$ are successively assigned to $e$" $\}$ }*/


#define ADJ_BREAK { adj_loop_index = -1; break; }

#define forall_inout_edges(e,v)\
if (0); else for(int adj_loop_index = 1; adj_loop_index>=0; adj_loop_index--)\
for(e = First_Adj_Edge(v,adj_loop_index);\
e != Leda_Nil_Edge(adj_loop_index);\
e = Succ_Adj_Edge(e,adj_loop_index))

/*{\Mtext
\bigskip
{\bf forall\_inout\_edges}($e,w$)\\
$\{$ ``the edges of $out\_edges(w)$ and $in\_edges(w)$ are successively assigned to $e$" $\}$ }*/


#define forall_adj_edges(e,v)\
if (0); else for(int adj_loop_index = graph_of(v)->adj_edges_select(); adj_loop_index>=0; adj_loop_index--)\
for(e = First_Adj_Edge(v,adj_loop_index);\
e != Leda_Nil_Edge(adj_loop_index);\
e = Succ_Adj_Edge(e,adj_loop_index))

/*{\Mtext
\bigskip
{\bf forall\_adj\_edges}($e,w$)\\
$\{$ ``the edges adjacent to node $w$ are successively assigned to $e$" $\}$ }*/


#define forall_adj_nodes(u,v)\
if (0); else for(int adj_loop_index = graph_of(v)->adj_edges_select(); adj_loop_index>=0; adj_loop_index--)\
for(edge adj_loop_e = First_Adj_Edge(v,adj_loop_index);\
(adj_loop_e!=Leda_Nil_Edge(adj_loop_index))&&((u=opposite(v,adj_loop_e))||1);\
adj_loop_e = Succ_Adj_Edge(adj_loop_e,adj_loop_index))

/*{\Mtext
\bigskip
{\bf forall\_adj\_nodes}($v,w$)\\
$\{$ ``the nodes adjacent to node $w$ are successively assigned to v" $\}$ }*/


/*{\Mimplementation
Graphs are implemented by doubly linked adjacency lists. Most operations 
take constant time, except for all\_nodes, all\_edges, del\_all\_nodes,
del\_all\_edges, clear, write, and read which take time $O(n+m)$, where 
$n$ is the current number of nodes and $m$ is the current number of edges. 
The space requirement is $O(n+m)$.}*/



//------------------------------------------------------------------------------
// GRAPH<vtype,etype> : parameterized directed graphs
//------------------------------------------------------------------------------


/*{\Manpage {GRAPH} {vtype,etype} {Parameterized Graphs}}*/

template<class vtype, class etype> 

class GRAPH : public graph {

/*{\Mdefinition 
A parameterized graph $G$ is a graph whose nodes and edges contain additional
(user defined) data. Every node contains an element of a data type $vtype$, 
called the node type of $G$ and every edge contains an element of a data type 
$etype$ called the edge type of $G$. We use $\<v,w,y\>$ to denote an edge 
$(v,w)$ with information $y$ and $\<x\>$ to denote a node with information $x$.

All operations defined on instances of the data type $graph$ are also defined on
instances of any parameterized graph type \name. For 
parameterized graphs there are additional operations to access or update the 
information associated with its nodes and edges.  
Instances of a parameterized graph type can be used wherever an instance 
of the data type $graph$ can be used, e.g., in assignments and as 
arguments to functions with formal parameters of type $graph\&$. 
If a function $f(graph\&\ G)$ is called with an argument $Q$ of type 
\name\ then inside $f$ only the basic graph structure of $Q$ (the
adjacency lists) can be accessed. The node and edge entries are hidden.
This allows the design of generic graph algorithms, i.e., algorithms accepting
instances of any parametrized graph type as argument.}*/


void copy_node_entry(GenPtr& x) const  { LEDA_COPY(vtype,x); }
void copy_edge_entry(GenPtr& x) const  { LEDA_COPY(etype,x); }

void clear_node_entry(GenPtr& x) const { LEDA_CLEAR(vtype,x); }
void clear_edge_entry(GenPtr& x) const { LEDA_CLEAR(etype,x); }

void write_node_entry(ostream& o, GenPtr& x) const { LEDA_PRINT(vtype,x,o); }
void write_edge_entry(ostream& o, GenPtr& x) const { LEDA_PRINT(etype,x,o); }

void read_node_entry(istream& i, GenPtr& x) { vtype X; Read(X,i); x=Copy(X); }
void read_edge_entry(istream& i, GenPtr& x) { etype Y; Read(Y,i); x=Copy(Y); }

void init_node_entry(GenPtr& x) { LEDA_CREATE(vtype,x); }
void init_edge_entry(GenPtr& x) { LEDA_CREATE(etype,x); }

void print_node_entry(ostream& o, GenPtr& x)  const
     { o << "("; LEDA_PRINT(vtype,x,o); o << ")"; }
void print_edge_entry(ostream& o, GenPtr& x)  const
     { o << "("; LEDA_PRINT(etype,x,o); o << ")"; }

char* node_type()  const { return LEDA_TYPE_NAME(vtype); }
char* edge_type()  const { return LEDA_TYPE_NAME(etype); }

public:

int cmp_node_entry(node x, node y) const { return compare(inf(x),inf(y)); }
int cmp_edge_entry(edge x, edge y) const { return compare(inf(x),inf(y)); }


/*{\Mcreation G }*/

GRAPH()  {}

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it to 
            the empty graph. }*/


GRAPH<vtype,etype>& operator=(const GRAPH<vtype,etype>& a)
{ clear_all_entries();graph::operator=(a);copy_all_entries();return *this; }

GRAPH(const GRAPH<vtype,etype>& a) : graph(a) { a.copy_all_entries(); } 

// subgraphs
GRAPH(GRAPH<vtype,etype>& a, const list<node>& b, const list<edge>& c) 
: graph(a,b,c) {}
GRAPH(GRAPH<vtype,etype>& a, const list<edge>& c) : graph(a,c) {}

virtual ~GRAPH()   { if (parent==0) clear_all_entries(); }



/*{\Moperations 1.5 4.5 }*/

vtype  inf(node v)    const   { return LEDA_ACCESS(vtype,graph::inf(v)); }
/*{\Mop    returns the information of node $v$.}*/

etype  inf(edge e)    const   { return LEDA_ACCESS(etype,graph::inf(e)); }
/*{\Mop    returns the information of edge $e$.}*/

void   assign(node v, vtype x) { operator[](v) = x; }
/*{\Mop    makes $x$ the information of node $v$.}*/

void   assign(edge e, etype x) { operator[](e) = x; }
/*{\Mop    makes $x$ the information of edge $e$.}*/
                
node   new_node(vtype x)        { return graph::new_node(Copy(x)); }
/*{\Mop    adds a new node $\<x\>$ to $G$ and returns it.}*/

node   new_node()               { return graph::new_node(); }
/*{\Mop    adds a new node $\<vdef\>$ to $G$ and returns it. Here,
           $vdef$ is the default value of type $vtype$.}*/


edge   new_edge(node v, node w, etype a) {return graph::new_edge(v,w,Copy(a)); }
/*{\Mopl   adds a new edge $\<v,w,a\>$ to $G$ by appending 
           it to the adjacency list of $v$ and the in\_edges list of 
           $w$ and returns it. }*/

edge   new_edge(node v, node w) { return graph::new_edge(v,w); }
/*{\Mopl   adds a new edge $\<v,w,edef\>$ to $G$ by
	   appending it to the adjacency list of $v$ and
           the in\_edges list of $w$ and returns it. Here, $edef$ 
           is the default value of type $etype$.}*/


edge   new_edge(edge e, node w, etype a)
                                { return graph::new_edge(e,w,Copy(a),0); }
/*{\Mopl   adds a new edge $\<source(e),w,a\>$ to $G$ by appending it 
           to the adjacency list of $source(e)$ and the in-list of 
           $w$ and returns it. }*/


edge   new_edge(edge e, node w) { return graph::new_edge(e,w); }
/*{\Mopl   adds a new edge $\<source(e),w,edef\>$ to $G$ by appending 
           it to the adjacency list of $source(e)$ and the in\_edges list of 
           $w$ and returns it. Here, $edef$ is the default value of 
           type $etype$.}*/


edge   new_edge(edge e, node w, etype a, int dir)
                                { return graph::new_edge(e,w,Copy(a),dir); }
/*{\Mopl   adds a new edge $\<source(e), w, a\>$ to $G$
	   by inserting it after ($dir$ = $after$) or before ($dir$
	   = $before$) edge $e$ into the adjacency list of
	   $source(e)$ and appending it to the in\_edges list of $w$.
           Returns the new edge.}*/


/* inherited from base graph
void sort_nodes();  
*/
/*{\Mop    the nodes of $G$ are sorted according to their
           contents.\\
           \precond $vtype$ is linearly ordered.}*/


/* inherited from base graph
void sort_edges();
*/
/*{\Mop    the edges of $G$ are sorted according to their
	   contents.\\ 
           \precond $etype$ is linearly ordered.}*/

/* inherited from base graph
void write(string fname) const;
*/
/*{\Mop    writes $G$ to the file with name $fname$. The
           output functions $Print(vtype,ostream)$ and
           $Print(etype,ostream)$ (cf. section 1.6) must
           be defined.}*/

/* inherited from base graph
int read(string fname);
*/
/*{\Mop       reads $G$ from the file with name $fname$. The 
	   input functions $Read(vtype,istream)$ and 
	   $Read(etype,istream)$ (cf.~section 1.6) must
	   be defined. Returns error code\\
	   1 \quad if file $fname$ does not exist\\
	   2 \quad if graph is not of type \name\\
	   3 \quad if file $fname$ does not contain a graph\\
	   0 \quad otherwise.}*/

void   clear()                  { clear_all_entries(); graph::clear(); }

/*{\Mtext
\bigskip
{\bf Operators} }*/

vtype& operator[] (node v)    { return LEDA_ACCESS(vtype,entry(v)); }
/*{\Marrop      returns a reference to $G$.inf($v$).}*/

etype& operator[] (edge e)    { return LEDA_ACCESS(etype,entry(e)); }
/*{\Marrop      returns a reference to $G$.inf($e$).}*/

vtype  operator[] (node v) const   { return LEDA_ACCESS(vtype,graph::inf(v)); }
etype  operator[] (edge e) const   { return LEDA_ACCESS(etype,graph::inf(e)); }

};

/*{\Mimplementation
Parameterized graphs are derived from directed graphs. All additional 
operations for manipulating the node and edge entries take constant
time.}*/


#include <LEDA/node_array.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_list.h>
#include <LEDA/node_map.h>
#include <LEDA/edge_map.h>
#include <LEDA/graph_gen.h>
#include <LEDA/graph_misc.h>


#endif

