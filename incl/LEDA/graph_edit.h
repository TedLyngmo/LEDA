/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph_edit.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/window.h>
#include <LEDA/plane_graph.h>

/*{\Manpage {graph_edit} {} {A Simple Graph Editor} }*/

void graph_edit(window& W,GRAPH<point,int>& G,bool dir=true,bool redraw=true);
/*{\Mfunc     starts a simple graph editor in window $W$ allowing to display
              and manipulate graph $G$. If the boolean flag $dir$ is $true$
              $G$ is displayed as a directed graph, otherwise, it is displayed
              undirected. }*/

