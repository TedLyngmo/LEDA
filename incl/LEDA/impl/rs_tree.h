/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  rs_tree.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_RS_TREE_H
#define LEDA_RS_TREE_H

//------------------------------------------------------------------------------
//
// rs_tree:
//
//           randomized search trees (derived from class "bin_tree")
//
// Stefan N"aher (1993)
//
//------------------------------------------------------------------------------


#include <LEDA/basic.h>
#include <LEDA/impl/bin_tree.h>


typedef bin_tree_node* rs_tree_item;


//-----------------------------------------------------------------
// class rs_tree
//-----------------------------------------------------------------

class rs_tree : public bin_tree
{
  int root_balance() { return rand_int.get(); }
  int node_balance() { return rand_int.get(); }
  int leaf_balance() { return 0; }

  void insert_rebal(rs_tree_item);
  void del_rebal(rs_tree_item, rs_tree_item);


public:

 rs_tree() {}
~rs_tree() {}
 rs_tree(const rs_tree& T) : bin_tree(T) {}
 rs_tree& operator=(const rs_tree& T) { bin_tree::operator=(T); return *this; }

};

#endif


