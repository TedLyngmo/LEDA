/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  plane_alg.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_PLANE_ALG_H
#define LEDA_PLANE_ALG_H

#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/plane_graph.h>

/*{\Manpage {plane_alg} {} {Plane Algorithms} }*/

/*{\Mtext
\bigskip
$\bullet$ {\bf Triangulations}
}*/

void TRIANGULATE_POINTS(list<point> L, GRAPH<point,edge>& G);

void TRIANGULATE_POINTS(list<rat_point> L, GRAPH<rat_point,edge>& G);


void DELAUNAY_TRIANG(const list<point>& L, GRAPH<point,edge>& T);
void DELAUNAY_TRIANG(const list<rat_point>& L, GRAPH<rat_point,edge>& T);

void DELAUNAY_TRIANG(const list<point>& L, graph& T, node_array<point>& pos,
                                                     edge_array<edge>&  rev);

void DELAUNAY_TRIANG(const list<point>& L, planar_map& T,
                                                  node_array<point>& pos);


/*{\Mtext
\bigskip
$\bullet$ {\bf Line segment intersection}
}*/


void SWEEP_SEGMENTS(const list<segment>&, list<point>&);
void SWEEP_SEGMENTS(const list<segment>&, GRAPH<point,segment>&);

void SWEEP_SEGMENTS(const list<rat_segment>&, list<rat_point>&);
void SWEEP_SEGMENTS(const list<rat_segment>&, GRAPH<rat_point,rat_segment>&);


/*{\Mtext
\bigskip
$void$\quad SWEEP\_SEGMENTS($list\<segment\>\ L,\ GRAPH\<point,segment\>\&\ G$);

\bigskip
SWEEP\_SEGMENTS takes a list of segments $L$ as input and computes
the planar graph $G$ induced by the set of straight line segments
in $L$. The nodes of $G$ are all endpoints and all proper intersection
points of segments in $L$. The edges of $G$ are the maximal relatively open
subsegments of segments in $L$ that contain no node of $G$. All edges are
directed from left to right or upwards. The algorithm (\cite{BO79}) runs in
time $O((n+s)\log n)$ where $n$ is the number of segments and $s$ is the
number of vertices of the graph $G$.  }*/


inline void SEGMENT_INTERSECTION(const list<segment>& L, list<point>& P)
{ SWEEP_SEGMENTS(L,P); }


/*{\Mtext
\bigskip
$void$\quad SEGMENT\_INTERSECTION($list\<segment\>\ L,\ list\<point\>\&\ P$);

\bigskip
SEGMENT\_INTERSECTION takes a list of segments $L$ as input and computes
the list of intersection points between all segments in $L$.

The algorithm (\cite{BO79}) has running time $O((n+k)\log n)$,
where $n$ is the number of segments and $k$ is the number of intersections.
}*/



/*{\Mtext
\bigskip
$\bullet$ {\bf Convex Hulls}
}*/



list<point>     CONVEX_HULL(const list<point>&);

/*{\Mtext
\bigskip
$list\<point\>$\quad CONVEX\_HULL($list\<point\>\ L$);

\bigskip
CONVEX\_HULL takes as argument a list of points and returns the polygon
representing the convex hull of $L$. It is based on a randomized incremental
algorithm.

Running time: $O(n\log n)$ (with high probability), where $n$ is the number
of points.
}*/


list<rat_point> CONVEX_HULL(const list<rat_point>&);

/*{\Mtext
\bigskip
$list\<point\>$\quad CONVEX\_HULL($list\<point\>\ L$);
$list\<rat\_point\>$ CONVEX\_HULL($list\<rat\_point\>\ L$);

\bigskip
CONVEX\_HULL takes as argument a list of (rational) points and returns the
polygon representing the convex hull of $L$.

Running time: $O(n\log n)$ (with high probability), where $n$ is the number
of points.
}*/


/*{\Mtext
\bigskip
$\bullet$ {\bf Voronoi Diagrams}
}*/



void VORONOI(list<point>& sites, double R, GRAPH<point,point>& VD);

/*{\Mtext
\bigskip
$void$\quad VORONOI$(list\<point\>\&\ sites,\ double\ R,\ GRAPH\<point,point\>\&
\ G)$

\bigskip
VORONOI takes as input a list of points $sites$ and a real number
$R$. It computes a directed graph $G$ representing the planar subdivision
defined by the Voronoi-diagram of $sites$ where all ``infinite" edges have
length $R$. For each node $v$ $G$.inf($v$) is the corresponding Voronoi
vertex ($point$) and for each edge $e$  $G$.inf($e$) is the site ($point$)
whose Voronoi region is bounded by $e$.

The algorithm (\cite{De92}) has running time $O(n\log n)$ (with high probability
),
where $n$ is the number of sites.
}*/

#endif
