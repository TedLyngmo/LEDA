/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  rb_tree.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_RB_TREE_H
#define LEDA_RB_TREE_H

//------------------------------------------------------------------------------
//
// rb_tree:  red black trees (leaf oriented & doubly linked)
//
//           derived from class "bin_tree"
//
// Stefan N"aher (1993)
//
//------------------------------------------------------------------------------



#include <LEDA/basic.h>
#include <LEDA/impl/bin_tree.h>


typedef bin_tree_node* rb_tree_item;


// ----------------------------------------------------------------
// class rb_tree
// ----------------------------------------------------------------

class rb_tree : public bin_tree
{
  enum color  { red = 0, black = 1 };

  int root_balance() { return black; }
  int node_balance() { return red; }
  int leaf_balance() { return black; }

  void insert_rebal(rb_tree_item);
  void del_rebal(rb_tree_item, rb_tree_item);

public:

  rb_tree() {}
 ~rb_tree() {}

  rb_tree(const rb_tree& T) : bin_tree(T) {}

  rb_tree& operator=(const rb_tree& T)
  { bin_tree::operator=(T); return *this; }

};

#endif
