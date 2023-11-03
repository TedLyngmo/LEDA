/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_list.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_NODE_LIST_H
#define LEDA_NODE_LIST_H

//------------------------------------------------------------------------------
// node_list
//------------------------------------------------------------------------------

#include <LEDA/graph.h>

/*{\Manpage {node\_list} {} {Lists of Nodes} }*/


class node_list : public c_obj_list {

/*{\Mdefinition
An instance of the data type \name\ is a doubly linked list of nodes. It
is implemented more efficiently than the general list type $list\<node\>$ 
(\ref{Linear Lists}). However, it can only be used with the 
restriction that every node is contained in at most one \name.  }*/


// private copy constructor (node_list's cannot be copied)

  node_list(const node_list& ) {}

public:

/*{\Mcreation L }*/

  node_list() {};
/*{\Mcreate    introduces a variable \var\ of type \name\ and initializes
               it with the empty list. }*/


/*{\Moperations 2 4.5 }*/

  static void del_node(node v) { aux_link(v)->del_item(); }

  void append(node v) { c_obj_list::append(aux_link(v)); }
/*{\Mop   appends $v$ to list \var. }*/

  void push(node v)   { c_obj_list::push(aux_link(v)); }
/*{\Mop   adds $v$ at the front of \var. }*/

  void insert(node v, node w) 
                      { c_obj_list::insert(aux_link(v),aux_link(w)); }
/*{\Mop   inserts $v$ after $w$ into \var.\\
          \precond $w \in L$. }*/


  node pop()          { return node(aux_link(c_obj_list::pop())); }
/*{\Mop   deletes the first node from \var\ and returns it.\\
          \precond \var\ is not empty. }*/

  void del(node v)    { c_obj_list::del(aux_link(v)); }
/*{\Mop   deletes $v$ from \var.\\
          \precond $v \in L$. }*/

  bool member(node v)  const { return aux_link(v)->succ_link != nil; }
/*{\Mop    returns true if $v \in L$ and false otherwise. }*/

  bool operator()(node v) const { return member(v); }
/*{\Mfunop    returns true if $v \in L$ and false otherwise. }*/


  node head() const { return node(aux_link(c_obj_list::first())); }
  node first()const { return node(aux_link(c_obj_list::first())); }
/*{\Mop    returns the first node in \var\ (nil if \var\ is empty).}*/

  node tail() const { return node(aux_link(c_obj_list::last()));  }
  node last() const { return node(aux_link(c_obj_list::last()));  }
/*{\Mop    returns the last node in \var\ (nil if \var\ is empty).}*/


  node succ(node v) const
  { return node(aux_link(c_obj_list::succ(aux_link(v)))); }
/*{\Mop    returns the successor of $v$ in \var.\\ 
           \precond $v \in L$. }*/

  node pred(node v) const
  { return node(aux_link(c_obj_list::pred(aux_link(v)))); }
/*{\Mop    returns the predecessor of $v$ in \var.\\ 
           \precond $v \in L$. }*/

  node cyclic_succ(node v) const
  { return node(aux_link(c_obj_list::cyclic_succ(aux_link(v)))); }
/*{\Mop    returns the cyclic successor of $v$ in \var.\\ 
           \precond $v \in L$. }*/

  node cyclic_pred(node v) const
  { return node(aux_link(c_obj_list::cyclic_pred(aux_link(v)))); }
/*{\Mop    returns the cyclic predecessor of $v$ in \var.\\ 
           \precond $v \in L$. }*/

// iteration

  node first_item() const { return first(); }
  node last_item()  const { return last(); }

  void loop_to_succ(GenPtr& x) const { x = succ(node(x)); }
  void loop_to_pred(GenPtr& x) const { x = pred(node(x)); }

  GenPtr forall_loop_test(GenPtr x, node& v) const { return v = node(x); }

#if defined(__ELSE_SCOPE_BUG__)
  GenPtr forall_loop_item;
  GenPtr& Forall_Loop_Item() const { return (GenPtr&)forall_loop_item; }
#endif

};

/*{\Mtext
\bigskip
{\bf forall}($x,L$)
$\{$ ``the elements of $L$ are successively assigned to $x$'' $\}$ }*/


#endif
