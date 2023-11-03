/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _planar_map.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/planar_map.h>

extern bool PLANAR(graph&, bool=false);

list<edge> planar_map::adj_edges(face f) const
{ list<edge> result(f->head);
  edge e1 = succ_face_edge(f->head);
  while (e1!=f->head)
  { result.append(e1);
    e1 = succ_face_edge(e1);
   }
  return result;
 }

list<node> planar_map::adj_nodes(face f) const
{ list<node> result(source(f->head));
  edge e1 = succ_face_edge(f->head);
  while (e1!=f->head)
  { result.append(source(e1));
    e1 = succ_face_edge(e1);
   }
  return result;
 }

list<face> planar_map::adj_faces(node v) const
{ list<face> result;
  edge e;
  forall_out_edges(e,v) result.append(adj_face(e));
  return result;
 }

face  planar_map::new_face(GenPtr i) 
{ 
  face f=new face_struct(i,this);
  f->loc=F_list.append(f);
  return f;
 }


edge planar_map::split_edge(edge e, GenPtr node_inf)
{ 
  /* splits edge e and its reversal by inserting a new node u (node_inf) 

              e                          e           rr
        ----------->                --------->   --------->
     (v)            (w)   ====>  (v)          (u)          (w)
        <-----------                <---------   <---------
              r                          er          r

     returns edge rr
  */


  edge r = e->rev;

  node v = source(e);
  node w = target(e);

  node u = graph::new_node();

  copy_node_entry(node_inf);

  u->data[0] = node_inf;

  // remove e and r from corresponding in-lists
  w->adj_edges[1].del(adj_link2(e));
  v->adj_edges[1].del(adj_link2(r));

  // insert e and r in in-list of u
  e->t = u;
  r->t = u;
  u->adj_edges[1].append(adj_link2(e));
  u->adj_edges[1].append(adj_link2(r));


  // create reverse edges rr and re
  edge rr = graph::new_edge(u,w);
  edge er = graph::new_edge(u,v);

  FACE(rr) = FACE(e);
  FACE(er) = FACE(r);

  r->rev  = rr;
  rr->rev = r;

  e->rev  = er;
  er->rev = e;

  return rr;

}


edge planar_map::new_edge(edge e1, edge e2, GenPtr face_i)
{ 
 /* cout << "NEW_EDGE:\n";
    print_edge(e1); cout << " F = " << int(adj_face(e1)) << "\n";
    print_edge(e2); cout << " F = " << int(adj_face(e2)) << "\n";
    newline;
  */

  if (adj_face(e1) != adj_face(e2)) 
    error_handler(1,"planar_map::new_edge: new edge must split a face."); 

  face F = adj_face(e1);
  face f = new_face(face_i);

  edge y = graph::new_edge(e1,source(e2),before);
  F->head = y;
  FACE(y) = F;
  
  edge x = graph::new_edge(e2,source(e1),before);
  f->head = x;
  FACE(x) = f;

  x->rev = y;
  y->rev = x;

  for (edge e = succ_face_edge(x); e != x; e = succ_face_edge(e)) 
     FACE(e) = f;

  return y;
}


node planar_map::new_node(const list<edge>& el, GenPtr node_inf)
{
  if (el.length() < 2)
      error_handler(1,"planar_map::new_node(el,i):  el.length() < 2."); 

  list_item it = el.first();

  edge e0 = el[it];

  it = el.succ(it);

  face f = adj_face(e0);

  edge e;
  forall(e,el)
  { if (adj_face(e) != f)
      error_handler(1,"planar_map::new_node: edges bound different faces."); 
   }

  e = el[it];

  it = el.succ(it);

  GenPtr face_inf = f->inf;

  copy_face_entry(face_inf);
  copy_node_entry(node_inf);

  edge e1 = split_edge(new_edge(e0,e,face_inf),node_inf);

  node u = source(e1);

  while(it)
  { copy_face_entry(face_inf);
    e1 = new_edge(e1,el[it],face_inf);
    it = el.succ(it);
   }

  return u;
}

node planar_map::new_node(face f, GenPtr node_inf)
{ return new_node(adj_edges(f),node_inf);
 }


void planar_map::del_edge(edge x, GenPtr face_i)
{
  edge y  = reverse(x);
  face F1 = adj_face(x);
  face F2 = adj_face(y);

  edge e = succ_face_edge(y);

  F1->inf = face_i;

  if (F1!=F2)
  { edge e = succ_face_edge(y);
    F1->head = e;
    while (e!=y)
    { FACE(e) = F1;
      e = succ_face_edge(e);
     }

    del_face(F2);
   }
  else
  { e = succ_face_edge(e);

    if (e!=y) // no isolated edge
      F1->head = e;   
    else 
      del_face(F1);

   }

  graph::del_edge(x);
  graph::del_edge(y);

}


void planar_map::clear() 
{ face f;
  forall(f,F_list) 
  { clear_face_entry(f->inf);
    delete f;
   }
  F_list.clear();
  graph::clear();
 }


static int edge_ord1(const edge& e) { return index(source(e)); }
static int edge_ord2(const edge& e) { return index(target(e)); }

planar_map::planar_map(const graph& G) : graph(G)
{ 

  // input: planar embedded graph represented by a bidirected directed graph G 
  // i.e., for each node v of G the adjacent edges are sorted counter-clockwise.
  // computes planar map representation (faces!)
  // For each face F the information of one of its edges is copied into F


  int n = max_node_index();

  // test whether graph is bidirected and compute for every edge e = (v,w) 
  // its reversal e->rev = (w,v) 

  list<edge> E1 = all_edges();
  E1.bucket_sort(0,n,&edge_ord2);
  E1.bucket_sort(0,n,&edge_ord1);
  
  list<edge> E2 = all_edges();
  E2.bucket_sort(0,n,&edge_ord1);
  E2.bucket_sort(0,n,&edge_ord2);

  list_item it1 = E1.first();
  list_item it2 = E2.first();

  while (it1 != nil)
  { edge e = E1[it1];
    edge r = E2[it2];

    it1 = E1.succ(it1); 
    it2 = E2.succ(it2); 

    if (target(r) == source(e) && source(r) == target(e))
        e->rev = r;
    else
        error_handler(999,"planar_map: graph is not bidirected");
   }


  // test planarity
  
    if (!PLANAR(*this)) 
       error_handler(1,"planar_map: Graph is not planar."); 
  

  // compute faces

  edge_array<bool> marked(*this);

  edge e;
  forall_edges(e,*this)
    if (!marked[e])
    { marked[e] = true;
      face f = new_face(e->data[0]);  // copy entry of first edge into face
      G.copy_edge_entry(f->inf);
      e->data[0] =  f;
      f->head = e;
      edge e1 = succ_face_edge(e);
      while (e1 != e)
      { e1->data[0] = f;
        marked[e1] = true;
        e1 = succ_face_edge(e1);
       }
     } 

} 


list<edge> planar_map::triangulate()
{
  node v,w;
  edge e,e1,e2,e3;

  list<edge> L;

  node_array<bool> marked(*this,false);
 
  forall_nodes(v,*this)
  {
    list<edge> El = adj_edges(v);
 
    forall(e,El) marked[target(e)] = true;

    forall(e,El)
    { 
      e1 = e;
      e2 = succ_face_edge(e1);
      e3 = succ_face_edge(e2);

      face F = FACE(e1);

      while (target(e3) != v)
      { 

        // e1,e2 and e3 are the first three edges in a clockwise 
        // traversal of a face incident to v and t(e3) is not equal
        // to v.

        if ( !marked[target(e2)] )
        { 
          // we mark w and add the edge {v,w} inside F, i.e., after
          // dart e1 at v and after dart e3 at w.

          marked[target(e2)] = true;
  
          e1 = new_edge(e1,e3,F->inf);
          e2 = e3;
          e3 = succ_face_edge(e2);

          L.append(e1);
        }
        else
        { // we add the edge {source(e2),target(e3)} inside F, i.e.,
          // after dart e2 at source(e2) and before dart 
          // reversal_of[e3] at target(e3).

          e3 = succ_face_edge(e3); 

          e2 = new_edge(e2,e3,F->inf);

          L.append(e2);

        }
      } //end of while

    } //end of stepping through incident faces

   forall_adj_nodes(w,v) marked[w] = false;

  } // end of stepping through nodes

 return L;

}
