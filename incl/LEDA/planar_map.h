/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  planar_map.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_PLANAR_MAP_H
#define LEDA_PLANAR_MAP_H

#include <LEDA/graph.h>

class face_struct;

typedef face_struct* face;

class face_struct {

friend class planar_map;

 edge      head;     // first edge of face
 list_item loc;      // location in F_list
 GenPtr    inf;      // user defined information

 planar_map*  g;     // face of (*g)


 face_struct(GenPtr x, planar_map* G)
 {
   inf = x ;
   head = nil;
   loc = nil;
   g = G;
  }


  LEDA_MEMORY(face_struct)

friend planar_map* graph_of(face f) { return f->g; }

};






extern int STRAIGHT_LINE_EMBEDDING(graph& G, node_array<int>& xcoord,
                                            node_array<int>& ycoord);

/*{\Manpage {planar_map} {} {Planar Maps} }*/

class planar_map : public graph {
/*{\Mdefinition
    An instance $M$ of the data type $planar\_map$ is the combinatorial
    embedding of a planar graph, i.e., $M$ is bidirected (for every edge
    $(v,w)$ of $M$ the reverse edge $(w,v)$ is also in $M$) and
    there is a planar embedding of $M$ such that for every node $v$ the
    ordering of the edges in the adjacency list of $v$ corresponds to the
    counter-clockwise ordering of these edges around $v$ in the embedding.

    Planar maps make use of the item type $face$ in addition to nodes and
    edges.
}*/

list<face>       F_list;

face  new_face(GenPtr i=0);
void  del_face(face f) { F_list.del(f->loc); delete f; }

face& FACE(edge e) { return (face&)(e->data[0]);  }

virtual void copy_face_entry(GenPtr&)  const {}
virtual void init_face_entry(GenPtr&)        {}
virtual void clear_face_entry(GenPtr&) const {}

/* inherited from graph:
virtual void copy_node_entry(GenPtr&)  const {}
virtual void init_node_entry(GenPtr&)        {}
virtual void clear_node_entry(GenPtr&) const {}
*/

virtual void print_face_entry(GenPtr&) const {}

protected:

GenPtr& entry(face f)         { return f->inf; }
GenPtr& entry(node v)         { return graph::entry(v); }

GenPtr  inf(face f)     const { return f->inf; }
GenPtr  inf(node v)     const { return graph::inf(v); }

void       clear();
void       init_entries();

public:

/*{\Mcreation M }*/

planar_map() {}

planar_map(const graph& G);
/*{\Mcreate creates an instance $M$ of type $planar\_map$ and initializes it to
    the planar map represented by the directed graph $G$.\\
    \precond $G$
    represents a bidirected planar map, i.e. for every edge $(v,w)$ in $G$
    the reverse edge $(w,v)$ is also in $G$ and there is a planar embedding
    of $G$ such that for every node $v$ the ordering of the edges in the
    adjacency list of $v$ corresponds to the counter-clockwise ordering of
    these edges around $v$ in the embedding.}*/

virtual ~planar_map() { clear(); }



/*{\Moperations 2 4.6}*/

const list<face>& all_faces()       const { return F_list; }
/*
list<face> all_faces();
*/
/*{\Mop      returns the list of all faces of \var.}*/


list<face> adj_faces(node v)   const;
/*{\Mop      returns the list of all faces of \var\ adjacent
	     to node $v$ in counter-clockwise order.}*/

face       adj_face(edge e)  const { return face(e->data[0]); }
/*{\Mop      returns the face of \var\ to the right of $e$.}*/


list<node> adj_nodes(face f)   const;
/*{\Mop      returns the list of all nodes of \var\ adjacent
             to face $f$ in clockwise order.}*/

list<node> adj_nodes(node v) const { return graph::adj_nodes(v); }

list<edge> adj_edges(face)   const;
/*{\Mop      returns the list of all edges of \var\ bounding
	     face $f$ in clockwise order.}*/

list<edge> adj_edges(node v) const { return graph::adj_edges(v); }

edge reverse(edge e)         const { return edge(e->rev); }
/*{\Mop      returns the reversal of edge $e$ in \var. }*/


edge first_face_edge(face f) const { return f->head; }
/*{\Mop      returns the first edge of face $f$ in \var. }*/

edge succ_face_edge(edge e)  const { return cyclic_adj_succ(reverse(e)); }
/*{\Mop      returns the successor edge of $e$ in face $M$.adj\_face($e$)
	     i.e., the next edge in clockwise order.}*/

edge pred_face_edge(edge e)  const { return reverse(cyclic_adj_pred(e)); }
/*{\Mop      returns the predecessor edge of $e$ in face $f$,
             i.e., the next edge in counter-clockwise order.}*/


edge next_face_edge(edge e)  const
{ e = succ_face_edge(e);
  return (e==adj_face(e)->head) ? nil : e;
}

face first_face() const { return F_list.head(); }

face next_face(face f) const
{ list_item it = F_list.succ(f->loc);
  return (it) ? F_list.contents(it) : nil;
}


edge    new_edge(edge e1, edge e2, GenPtr);
edge    new_edge(edge e1, edge e2) { return new_edge(e1,e2,0); }
/*{\Mopl    inserts the edge $e=(source(e_1),source(e_2))$
	    and its reversal into $M$ and returns $e$.\\
            \precond $e_1$ and $e_2$ are bounding the same face $F$.
	    The operation splits $F$ into two new faces. }*/

void    del_edge(edge,GenPtr);
void    del_edge(edge e) { del_edge(e,0); }
/*{\Mop     deletes the edge $e$ from \var. The two faces
	    adjacent to $e$ are united to one face.}*/


edge    split_edge(edge,GenPtr);
edge    split_edge(edge e) { return split_edge(e,0); }
/*{\Mop     splits edge $e=(v,w)$ and its reversal $r=(w,v)$
	    into edges $(v,u)$, $(u,w)$, $(w,u)$, and $(u,v)$.
	    Returns the edge $(u,w)$. }*/


node    new_node(const list<edge>&, GenPtr);
node    new_node(const list<edge>& el) { return new_node(el,0); }
/*{\Mop     splits the face bounded by the edges in $el$ by
	    inserting a new node $u$ and connecting it to all
	    source nodes of edges in $el$.\\ \precond
	    all edges in $el$ bound the same face.}*/

node    new_node(face, GenPtr);
node    new_node(face f) { return new_node(f,0); }
/*{\Mop     splits face $f$ into triangles by inserting a new
	    node $u$ and connecting it to all nodes of $f$.
	    Returns $u$.}*/


list<edge> triangulate();
/*{\Mop     triangulates all faces of \var\ by inserting new
	    edges. The list of inserted edges is returned.}*/

int     straight_line_embedding(node_array<int>& x, node_array<int>& y)
                              { return STRAIGHT_LINE_EMBEDDING(*this,x,y); }
/*{\Mopl    computes a straight line embedding for $M$ with
	    integer coordinates ($x[v]$, $y[v]$) in the
	    range $0\dots 2(n-1)$ for every node $v$ of $M$,
	    and returns the maximal used coordinate. }*/



};

#define forall_face_edges(e,F)\
for(e=graph_of(F)->first_face_edge(F); e; e=graph_of(F)->next_face_edge(e))

#define forall_faces(f,G)\
for(f=G.first_face(); f; f=G.next_face(f))

#define forall_adj_faces(f,v)\
if (0);else \
for(edge forall_af_e=graph_of(v)->first_adj_edge(v);\
forall_af_e && (f = ((planar_map*)graph_of(v))->adj_face(forall_af_e));\
forall_af_e=graph_of(v)->adj_succ(forall_af_e))


/*{\Mtext
\bigskip
{\bf Iteration}

{\bf forall\_faces}($f,M$)
$\{$ ``the faces of $M$ are successively assigned to $f$" $\}$

{\bf forall\_face\_edges}($e,f$) \\
\phantom{{\bf forall\_faces}($f,M$)}
$\{$ ``the edges of face $f$ are successively assigned to $e$" $\}$

{\bf forall\_adj\_faces}($f,v$) \\
\phantom{{\bf forall\_faces}($f,M$)}
$\{$ ``the faces adjacent to node $v$ are successively assigned to $f$" $\}$ }*/

/*{\Mimplementation
Planar maps are implemented by parameterized directed graphs.
All operations take constant time, except for new\_edge and del\_edge
which take time $O(f)$ where $f$ is the number of edges in the created
faces and triangulate and straight\_line\_embedding which take time $O(n)$
where $n$ is the current size (number of edges) of the planar map. }*/


//------------------------------------------------------------------------------
// PLANAR_MAP: generic planar map
//------------------------------------------------------------------------------

/*{\Mtext
\newpage
}*/

/*{\Manpage {PLANAR_MAP} {vtype,ftype} {Parameterized Planar Maps}}*/

template <class vtype, class ftype>

class PLANAR_MAP : public planar_map {

/*{\Mdefinition
A parameterized planar map $M$ is a planar map whose nodes and faces
contain additional (user defined) data. Every node  contains
an element of a data type $vtype$, called the node type of $M$ and every
face contains an element of a data type $ftype$ called the face type of
$M$. All operations of the data type $planar\_map$ are also defined for
instances of any parameterized planar\_map type. For parameterized
planar maps there are additional operations to access or update
the node and face entries.}*/


void init_node_entry(GenPtr& x)  { LEDA_CREATE(vtype,x); }
void init_face_entry(GenPtr& x)  { LEDA_CREATE(ftype,x); }

void copy_node_entry(GenPtr& x)  const { LEDA_COPY(vtype,x); }
void copy_face_entry(GenPtr& x)  const { LEDA_COPY(ftype,x); }

void clear_node_entry(GenPtr& x) const { LEDA_CLEAR(vtype,x); }
void clear_face_entry(GenPtr& x) const { LEDA_CLEAR(ftype,x); }

void print_node_entry(std::ostream& o, GenPtr& x)  const
{ o << "("; LEDA_PRINT(vtype,x,o); o << ")"; }

void print_edge_entry(std::ostream& o, GenPtr& x)  const
{ o << "(" << x << ")"; }

public:

/*{\Mcreation M }*/

   PLANAR_MAP() {}

   PLANAR_MAP(const GRAPH<vtype,ftype>& G) : planar_map((graph&)G)   {}

/*{\Mcreate creates an instance \var\ of type \name\ and initializes it to
the planar map represented by the parameterized directed graph $G$.
The node entries of $G$ are copied into the corresponding nodes of $M$
and every face $f$ of $M$ is assigned the information of one of its
bounding edges in $G$.\\
\precond $G$ represents a planar map. }*/

  ~PLANAR_MAP() { clear(); }



/*{\Moperations 1.5 4.5 }*/

   vtype  inf(node v) const   { return LEDA_ACCESS(vtype,planar_map::inf(v)); }
/*{\Mop      returns the information of node $v$.}*/

   ftype  inf(face f) const   { return LEDA_ACCESS(ftype,planar_map::inf(f)); }
/*{\Mop      returns the information of face $f$.}*/

   vtype& entry(node v) { return LEDA_ACCESS(vtype,planar_map::entry(v)); }
   vtype& operator[] (node v)    { return entry(v); }
/*{\Marrop   returns a reference to the information of node $v$.}*/

   ftype& entry(face f) { return LEDA_ACCESS(ftype,planar_map::entry(f)); }
   ftype& operator[] (face f)    { return entry(f); }
/*{\Marrop   returns a reference to the information of face $f$.}*/


   void   assign(node v, vtype x) { entry(v) = x; }
/*{\Mop      makes $x$ the information of node $v$.}*/

   void   assign(face f, ftype x) { entry(f) = x; }
/*{\Mop      makes $x$ the information of face $f$.}*/


   edge   new_edge(edge e1, edge e2)
                          { return planar_map::new_edge(e1,e2,0); }
   edge   new_edge(edge e1, edge e2, ftype y)
                          { return planar_map::new_edge(e1,e2,Convert(y)); }
/*{\Mopl     inserts the edge $e=(source(e_1),source(e_2))$
	     and its reversal edge $e'$ into $M$.\\
	     \precond
	     $e_1$ and $e_2$ are bounding the same face $F$.
	     The operation splits $F$ into two new faces $f$,
	     adjacent to edge $e$ and $f'$, adjacent to edge
	     $e'$ with inf($f$) = inf ($F$) and inf($f'$) = $y$.}*/


   edge   split_edge(edge e, vtype x)
                          { return planar_map::split_edge(e,Convert(x)); }
/*{\Mopl    splits edge $e=(v,w)$ and its reversal $r=(w,v)$
	    into edges $(v,u)$, $(u,w)$, $(w,u)$, and $(u,v)$.
            Assigns information $x$ to the created node $u$ and
	    returns the edge $(u,w)$. }*/

   node   new_node(list<edge>& el, vtype x)
                          { return planar_map::new_node(el,Convert(x)); }
/*{\Mopl    splits the face bounded by the edges in $el$ by
	    inserting a new node $u$ and connecting it to all
	    source nodes of edges in $el$. Assigns information $x$
            to $u$ and returns $u$.\\
	    \precond all edges in $el$ bound
            the same face.}*/

   node   new_node(face f, vtype x)
                          { return planar_map::new_node(f,Convert(x)); }
/*{\Mopl      splits face $f$ into triangles by inserting a new
              node $u$ with information $x$ and connecting it
	      to all nodes of $f$. Returns $u$.}*/

   void print_node(node v) const
   { std::cout << "["; Print(inf(v),std::cout); std::cout << "]";}

};

/*{\Mimplementation
Parameterized planar maps are derived from planar maps. All additional
operations for manipulating the node and edge contents take constant time.}*/



#endif
