/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  plane_graph.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_PLANE_GRAPH_H
#define LEDA_PLANE_GRAPH_H

#include <LEDA/point.h>
#include <LEDA/graph.h>


typedef GRAPH<point,int>    graph_point_int_dummy;
typedef GRAPH<point,point>  graph_point_point_dummy;
typedef GRAPH<point,edge>   graph_point_edge_dummy;

#endif
