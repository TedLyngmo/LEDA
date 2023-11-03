/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _node_part.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/node_partition.h>

void node_partition::init(const graph& G) 
{ node v;
  forall_nodes(v,G) v->data[2] = partition::make_block(v);
 }


