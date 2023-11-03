/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _g_matrix.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/graph.h>
#include <LEDA/node_matrix.h>


void Node_Matrix::init(const graph& G, int n, GenPtr x)
{ g = (graph*)&G;
  M.init(G,n,0);
  for(int i=0;i<M.size();i++)
  { graph_map* gm = new graph_map(&G,n);
    M.entry(i) = gm;
    for(int j=0;j<gm->size();j++)
    { copy_entry(x);
      gm->access(j) = x;
     }
   }
}

void Node_Matrix::init(const Node_Matrix& A)
{ g = A.g;
  int n = A.M.size();
  M.init(*g,n,0);
  for(int i=0;i<n;i++)
  { graph_map* gm = new graph_map(g,n);
    M.entry(i) = gm;
    graph_map* gma = A.M.inf(i);
    for(int j=0;j<n;j++)
    { GenPtr x = gma->read(j);
      copy_entry(x);
      gm->access(j) = x;
     }
   }
}


void Node_Matrix::clear_entries()
{ for(int i=0;i<M.size();i++)
  { graph_map* gm = M.entry(i);
    for(int j=0;j<gm->size();j++)
    { GenPtr x = gm->read(j);
      clear_entry(x);
     }
   }
}


Node_Matrix::~Node_Matrix()
{ for(int i=0; i<M.size();i++)
    delete M.entry(i);
 }


