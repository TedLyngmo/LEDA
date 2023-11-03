\documentstyle[11pt,comments,a4]{cweb}

\voffset=-0.5cm
\textwidth		14cm
\oddsidemargin          0.4cm
\evensidemargin         1.4cm
\marginparwidth		1.9cm
\marginparsep		0.4cm
\marginparpush		0.4cm
\topmargin		0cm
\headsep		1.5cm
\textheight		21.5cm
\footskip		2.2cm

\input transfig
\input papermacros

\begin{document}

\renewcommand{\thefootnote}{\fnsymbol{footnote}}

\title{Implementation of a Sweep Line Algorithm for the\\
       Straight Line Segment Intersection Problem}

\author{Kurt Mehlhorn and Stefan N\"aher\\
       {\footnotesize  Max-Planck-Institut f\"ur Informatik,}\\[-0.7ex]
       {\footnotesize 66123 Saarbr\"ucken, Germany}}
 
        \date{} 
        \maketitle

@i typnamen

@s myPoint int
@s mySegment int


@* Abstract.

We describe a robust and efficient implementation of the Bentley-Ottmann 
sweep line algorithm \cite{Bentley-Ottmann} based on the LEDA library
of efficient data types and algorithms \cite{LEDA-Paper}. The program 
computes the planar graph $G$ induced by a set $S$ of straight line segments 
in the plane. The nodes of |G| are all endpoints and all proper intersection 
points of segments in |S|. The edges of |G| are the maximal relatively open 
subsegments of segments in |S| that contain no node of |G|. All edges are
directed from left to right or upwards.
The algorithm runs in time $O((n+s)\log n)$ where $n$ is the number of 
segments and $s$ is the number of vertices of the graph $G$. The implementation
uses exact arithmetic for the reliable realization of the geometric 
primitives and it uses floating point filters to reduce the overhead of
exact arithmetic.
\tableofcontents

@* Introduction.

Let $S$ be a set of $n$ straight-line segments in the plane and let $G(S)$
be the graph induced by $S$. The vertices of $G(S)$ are all endpoints of
segments in $S$ and all intersection points between segments in $S$. The edges 
of $G$ are the maximal relatively open and connected subsets of segments in
$S$ that contain no vertex of $G(S)$. Figure \ref{example} shows an example.
Note that the graph $G(S)$ contains no parallel edges even if $S$ contains segments that overlap.
\begin{figure}
\begin{center}
\input{figures/introfig.tex}
\caption{\label{example}A set $S$ of segments and the induced planar graph.}
\end{center}
\end{figure}

Bentley and Ottmann \cite{Bentley-Ottmann} have shown how to compute $G(S)$
in time $O((n+m)\log n)$ where $m$ is the number of pairs of intersecting
segments in $S$. The algorithm is based on the plane-sweep paradigm. We
refer the reader to \cite[section VIII.4]{Mehlhorn-book}
\cite[section 7.2.3]{Preparata-Shamos}, and 
\cite[section 35.2]{Cormen-Leiserson-Rivest}
for a discussion of the plane sweep paradigm.

In this paper we describe an implementation of the Bentley-Ottmann algorithm. 
More precisely, we define a procedure
\begin{center}
  |sweep_segments(list<rat_segment> &seglist,GRAPH<rat_point,rat_segment> &G,bool use_filter)|
\end{center}
that takes a list $seglist$ of |rat_segment|s (segments with rational coordinates)
and computes the graph $G$ induced by it. For each vertex $v$ of $G$ it also 
computes its position in the plane, a |rat_point| (a point with rational 
coordinates) and for each edge $e$ of $G$ it computes a |rat_segment| containing 
it. The algorithm is based on the LEDA platform for combinatorial and geometric 
computing \cite{LEDA-Paper,LEDA-Manual}. In LEDA a |rat_segment| is specified by 
its two endpoints (of type |rat_point|) and a |rat_point| is specified by its 
homogeneous coordinates $(X,Y,W)$ of type |integer|. The type |integer| is the type 
of arbitrary precision integers. 


If |use_filter| is set to |true| then there must be
integers $k$ and $l$ such that $5k+3l\leq 450$ and 
$^^7cX^^7c,^^7cY^^7c\leq 2^k$ and $1\leq ^^7cW^^7c\leq 2^l$ for all 
endpoints. In section \ref{float-restrictions} we will explain 
where this restriction on the input size comes from. Use of the filter increases
the speed of the program by up to a factor of 4.

We want to stress that the implementation makes no other assumptions
about the input, in particular, segments may have length zero, may overlap,
several segments may intersect in the same point, endpoints of segments may
lie in the interior of other segments, ... .

We have achieved this generality by following two principles.
\begin{itemize}
\item
Treat degeneracies as first class citizens and not as an afterthought
\cite{Burnikel-et-al}. In particular, we reformulated the plane-sweep
algorithm so that it can handle all geometric situations. The details will
be given in section \ref{Algorithm}. The reformulation makes the 
description of the algorithm shorter since we do not distinguish between
three kinds of events but have only one kind of event and it also makes the 
algorithm faster. The algorithm now runs in time $O((n+s)\log n)$ where
$s$ is the number of vertices of $G$. Note that $s\leq n+m$ and that $m$
can be as large as $s^2$. The only previous algorithm that could handle all
degeneracies is due to Myers \cite{Myers}. Its expected running time for
random segments is $O(n \log n + m)$ and its worst case running time is
$O((n + m) \log n)$.

\item
Evaluate all geometric tests exactly. We use arbitrary precision integer
arithmetic for all geometric computations. So all tests are computed exactly
and we do not have to worry about numerical precision. Of course, we have
to pay for the overhead of arbitrary precision integer arithmetic. In order
to keep the overhead low we followed the suggestion of Fortune and van Wyk
\cite{Fortune-vanWyk} and implemented a floating point filter, i.e., all tests
are first performed using floating point arithmetic and only if the result of
the floating point computation is inconclusive we perform the costly exact
computation. The floating point filter improves the running time by a factor
of up to 4 depending on the problem instance. The floating point filter
is responsible for the restrictions that we have to place on the input.
The restrictions guarantee that the floating point computations will not
overflow.
\end{itemize}

This paper is structured as follows. In section \ref{Algorithm} we describe the
(generalized) plan-sweep algorithm. Section \ref{Implementation}
and \ref{geometric primitives} give the details of the implementation: the 
former section describes the combinatorial part and the latter section describes
the geometric primitives. The floating point filter is also discussed there.
Section \ref{experiments} contains some experimental results.


@* The Algorithm. 
\label{Algorithm}

In the sweep-line paradigm a line is moved from left to right across the plane
and the output (here the graph $G(S)$) is constructed incrementally as it
evolves
behind the sweep line. One maintains two data structures to keep the
construction going: The so-called {\em Y-structure} contains the intersection
of
the sweep line with the scene (here the set $S$ of line segments) and the
so-called
{\em X-structure} contains the events where the sweep has to be
stopped in order to add to the output or to update the $X$- or $Y$-structure.
In the line segment intersection problem an event occurs when the sweep line
hits an endpoint of some segment or an intersection point. When an event
occurs, some nodes and edges are added to the graph $G(S)$, the $Y$-structure
is
updated, and maybe some more events are generated. When the input is in
general position (no three lines intersecting in a common point, no endpoint
lying on a segment, no two endpoints or intersections having the same
$x$-coordinate, no vertical lines, no overlapping segments) then at most one
event can occur for each position of the sweep line and there are three clearly
distiguishable types of events (left endpoint, right endpoint, intersection)
with easily describable associated actions, cf. 
\cite{Mehlhorn-book}(section VIII.4).
We want to place no restrictions on the input and therefore need to proceed
slightly differently. We now describe the required changes.

We define the sweep line by a point $p\_sweep = (x\_sweep, y\_sweep)$. Let
$\epsilon$ be a positive infinitesimal (readers not familiar with
infinitesimals may think of $\epsilon$ as an arbitrarily small positive real).
Consider the directed line $L$ consisting of a vertical upward ray ending in
point $(x\_sweep + \epsilon, y\_sweep + \epsilon)$ followed by a horizontal
segment ending in $(x\_sweep - \epsilon, y\_sweep + \epsilon)$ followed by a
vertical upward ray. We call $L$ the {\em sweep line}. Note that no endpoint
of any segment lies on $L$, that no two segments of $S$ intersect $L$ in the
same point except if the segments overlap, and that no non-vertical segment
of $S$ intersects the horizontal part of $L$. All three properties follow from
the fact that $\epsilon$ is arbitrarily small but positive. Figure \ref{the
sweep line} illustrates the definition of $L$ and the data structures used in
the algorithm: The $Y$-structure, the $X$-structure, and the graph |G|.

\begin{figure}
\begin{center}
\input{figures/sw2fig.tex}
\caption{\label{the sweep line}A scene of 9 segments. The segments $s_1$ and
$s_8$ overlap. The $Y$-structure contains segments $s_1$, $s_2$, $s_9$, $s_4$,
and $s_3$ and the $X$-structure contains points $a$, $b$, $c$, $d$, $e$, $f$,
$g$, $h$, and $i$. An item in the $X$-structure containing point
$p$
is denoted $xit_p$ and an item in the $Y$-structure containing segment $s_i$
is denoted $sit_i$. The vertices of the graph $G$ are shown as full circles.}
\end{center}
\end{figure}

The $Y$-structure contains all
segments intersecting the sweep line $L$ ordered as their intersections with
$L$ appear on the directed line $L$. For segments intersecting $L$ in the same
point (these segments necessarily have the same underlying line) only the
segment extending further to the right is stored in the $Y$-structure. 

In the example of Figure \ref{the sweep line} the sweep line intersects the
segments $s_8$, $s_1$, $s_2$, $s_9$, $s_4,$ and $s_3$. The segments $s_8$ and
$s_1$
intersect $L$ in the same point and $s_1$ extends further to the right. Thus
$s_1$ is stored in the $Y$-structure and $s_8$ is not. The $Y$-structure
therefore consists of five items, one each for segments $s_1$,$s_2$, $s_9$,
$s_4,$ and
$s_3$.

The $X$-structure is a sorted sequence.
It contains an item for each point in $St\cup E\cup I$, where $St$ is the set
of all start points of segments that lie to the right of $L$, $E$ is the set of
all endpoints of segments that intersect $L$, and $I$ is defined as follows.
Every point in $I$ is the intersection $s\cap s'$ of two segments $s$ and $s'$
adjacent in the $Y$-structure that lies to the right of the sweep line. The ordering of
the points in the $X$-structure is the lexicographic ordering, i.e.,
$(x,y)$ is before $(x',y')$ if either $x<x'$ or $x=x'$ and $y<y'$.

In the example of Figure \ref{the sweep line} we have $St = \{a,d,e\},
E = \{b,c,f,e,g,i,h\}$ and $I =\{f,a\}$. The $X$-structure 
therefore
contains 9 items, one each for points |a|, |b|, |c|, |d|, |e|, |f|, |g|, 
|h|, and |i|.

We next define the informations associated with the items of both structures.
These informations serve to link the items in the $X$-structure with the items
in the $Y$-structure and vice versa. In particular, any item in the
|X|-structure is a pair $<p,sit>$ where |p| is a point and |sit| is either
|nil| or an item in the |Y|-structure and any item in the |Y|-structure 
is a pair $<s,xit>$ where |s| is a segment and |xit| is either |nil|
 or an item in the |X|-structure. Let
$<p,sit>$ be any item in the $X$-structure. If $p\in I\cup E$ then $sit$ is an
item in the $Y$-structure such that the segment associated with $sit$ contains
$p$. If $p\in St\setminus (I\cup E)$ then $sit = nil$. Next consider an item
$<s,xit>$ in the
$Y$-structure and let $s'$ be the segment associated with the successor
item in the $Y$-structure. If $s\cap s'$ exists and lies to the right of the
sweep line then $xit$ is an item in the $X$-structure and $s\cap s'$ is the
point associated with that item. If $s\cap s'$ either does not exist or does
not lie to the right of $L$ then $xit = nil$.

In our example, the $Y$-structure contains the items $<s_1,xit_f>, <s_2,nil>$,
$<s_9,nil>, <s_4,xit_a>$ and $<s_3,nil>$ where $xit_a$ and $xit_f$ are the
items
of the $X$-structure with associated points $a$ and $f$ respectively. Let's
turn to the items of the $X$-structure next. All items except $<d,nil>$ point
back to the $Y$-structure. If $sit_i$ denotes the item $<s_i$,...$>$, $i\in%
\{1,2,9,4,3\}$, of the $
Y$-structure then the items of the $X$-structure are $<a,sit_4>,
<b,sit_4>$, $<c,sit_1>, <d,nil>, <e,sit_9>, <f,sit_1>, <g,sit_2>, <h,sit_3>$
and $<i,sit_1>$.

The graph $G$ is the part of $G(S)$ to the left of $L$. With each vertex of
$G$ we store its position and with each edge of $G$ we store a segment
containing it. 

There is one additional piece of information that we need to keep. For each
segment $s$ contained in the $Y$-structure we store the rightmost vertex of $G$
lying on $s$.

We can now give the details of the algorithm. Initially, $G$ and the
$Y$-structure are empty, and the $X$-structure contains the left endpoints of
all segments in $S$.

In order to process an event we proceed as follows. Let $<p,sit>$ be the first
item in the $X$-structure. We may assume inductively that all invariants hold
for $p\_sweep = (p.x,p.y-2\epsilon)$. Note that this is true initially, i.e.,
before the first event is removed from the $X$-structure. We now show how to
establish the invariants for $p = p\_sweep$. We proceed in seven steps.
\begin{enumerate}
\item
  We add a node $v$ at position $p$ to our graph $G$.
\item
  We determine all segments in the $Y$-structure containing the point $p$.
  These segments form a possibly empty subsequence of the $Y$-structure.
\item
  For each segment in the subsequence we add an edge to the graph $G$.
  Its right endpoint is
  $v$ and its other
  endpoint is the vertex stored with the segment $s$.
\item
  We delete all segments ending in $p$ from the $Y$-structure.
\item
  We reverse the order of the subsequence in the $Y$-structure. This amounts to
  moving the sweep line across the point $p$.
\item
  We add all segments starting in $p$ to the $Y$-structure and then associate
  the node $v$ with all segments in the $Y$-structure containing $v$. If a
  newly inserted segment is collinear to an already existing segment
  we make
  sure to only keep the segment extending further to the right.
\item
  We update the events associated with the items of the $Y$-structure. We
  remove the events associated with the predecessor of the subsequence and
  the last item of the subsequence and we generate new
  events for the predecessor of the first item and the last item after the
  reversal of the subsequence.
\end{enumerate}
This completes the description of how to process the event $<p,...>$. The
invariants now hold for $p\_sweep = p$ and hence also for
$p\_sweep = (p'.x,p'.y-2\epsilon)$ where $<p',...>$ is the new first element of
the $X$-structure.


@* The Implementation. 
\label{Implementation}

The implementation follows the algorithm closely. 
There are two main differences.

\begin{itemize}
\item
  We add two infinitely long horizontal segments with $y$-coordinate
  $+\infty$ and $-\infty$ respectively. They serve as sentinels and simplify
  many tests. 
\item
  We maintain all points twice: once by their exact
  homogeneous coordinates and once by floating point approximations to
  these coordinates. All tests are first performed using the floating point
  approximations and only if the floating point filter gives no conclusive
  result the test is performed using exact arithmetic. 
\end{itemize}


We use sorted sequences, graphs, rational points and segments, big integers, 
and a floating point filter from LEDA, and some functions of the C++ maths 
library. We have to include the corresponding header files.

@<include statements@> =
#include <LEDA/sortseq.h>
#include <LEDA/graph.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/integer.h>
#include <LEDA/floatf.h>
#include <cmath>

@
Let us briefly explain these types; for a detailed discussion we refer
the reader to the LEDA manual \cite{LEDA-Manual}. |integer| is the type of 
arbitrary precision integers and |floatf| is the type of floating point 
approximation to integers. The type |floatf| is defined in section \ref{floatf}. 
The types |rat_point| and |rat_segment| realize points and segments in the 
plane with rational coordinates. An |rat_point| is specified by its homogeneous
coordinates of type |integer|. If |p| is a |rat_point| then |p.X()|, |p.Y()|, and
|p.W()| return its homogeneous coordinates and if |X|, |Y|, and |W| are of
type |integer| and $W\neq 0$ then |rat_point(X,Y)| and |rat_point(X,Y,W)| create
the |rat_point| with homogeneous coordinates |(X,Y,1)| and |(X,Y,W)| 
respectively. A |rat_segment| is specified by its two endpoints; so if 
|p| and |q| are |rat_point|s then |rat_segment(p,q)| constructs the directed
|rat_segment| with startpoint |p| and endpoint |q|. If |s| is a |rat_segment| then
|s.start()| and |s.end()| return the start- and endpoint of |s| respectively 
and |s.dx()| and |s.dy()| return the normalized |x|- and |y|-difference
of the segment, i.e., if |(px,py,pw)| and |(qx,qy,qw)| are the
homogeneous coordinates of the start- and endpoint of |s| then
|s.dx()| returns $px\cdot qw - qx\cdot pw$ and |s.dy()| returns
$py\cdot qw - qy\cdot pw$. The slope of a segment |s| is given by
$s.dy()/s.dx()$, but be careful. The slope might be infinite (if the
segment is vertical) or undefined (if the segment has length zero).

@s K int
@s Inf int

For any types |K| and |Inf| the type |sortseq<K,Inf>| is the type of sorted
sequences of pairs in $K\times Inf$. The type |K| must be linearly ordered 
by a function |compare|, i.e., the function 
|int compare(const K&, const K&)| must be defined for the type |K|
and the relation $<$ on |K| defined by $k_1 < k_2$ iff 
$compare(k_1,k_2) < 0$ is a linear order on |K|. Any object of type
|sortseq<K,Inf>| is a sequence of items (type |seq_item|) each containing
a pair $K\times Inf$. We use |<k,i>| to denote an item containing the pair
$(k,i)$ and call |k| the key and |i| the information of the item. A sorted 
sequence $<k_1,i_1>$,$<k_2,i_2>$,\dots ,$<k_m,i_m>$ has the additional
property that the keys of the item form an increasing sequence, i.e.,
$k_l < k_{l+1}$ for $1\leq l<m$.

In our implementation the |X|-structure has type |sortseq<rat_point,seq_item>|
and the |Y|-structure has type |sortseq<rat_segment,seq_item>|. We will later 
define the appropriate compare functions. If |p| and |q| are points with
cartesian coordinates |px|, |py|, |qx|, and |qy| respectively then
\[ compare(p,q) = \left \{
                  \begin{array}{rl}
		  -1 & \mbox{ if }px<qx\mbox{ or }px=qx\mbox{ and }py<qy\\
		  0  & \mbox{ if }px=qx\mbox{ and }py=qy\\
		  +1 & \mbox{ otherwise, }
		  \end{array}
		  \right . 
\]
and if |s1| and |s2| are segments intersecting the sweep line |L| then
|compare(s1,s2) = -1| if $s1\cap L$ precedes $s2\cap L$ on |L|,
|compare(s1,s2) = 0| if $s1\cap L = s2\cap L$, and
|compare(s1,s2) = +1| if $s1\cap L$ comes after $s2\cap L$.

It is important to observe that the compare function for segments
changes as the sweep progresses. What does it mean then that the keys of the 
items in a sorted sequence form an increasing sequence? LEDA requires 
that whenever a lookup or insert operation is applied to a sorted
sequence the sequence must be sorted with respect to the current 
compare function. The other operations may be applied even if the 
sequence is not sorted. What operations are available?

Let |S| be a sorted sequence of type |sortseq<K,Inf>| and let |k| 
and |i| be of type |K| and |Inf| respectively. The operation 
|S.lookup(k)| returns the item $it = <k,.>$ in |S| with key |k| if there
is such an item and returns |nil| otherwise. If |S.lookup(k)==nil|
then |S.insert(k,i)| adds a new item |<k,i>| to |S| and returns
this item. If |S.lookup(k)==it| then |S.insert(k,i)| changes the
information in the item |it| to |i|. If $it=<k,i>$ is an item of |S|
then |S.key(it)| and |S.inf(it)| return |k| and |i| respectively and
|S.succ(it)| and |S.pred(it)| return successor and predecessor item
of |it| respectively; the latter operations return |nil| if these
items do not exist. The operation |S.min()| returns the first item
of |S|, |S.empty()| returns true if |S| is empty and false otherwise. 
Finally, if |it1| and |it2| are items of |S| with |it1| before |it2|
then |S.reverse_items(it1,it2)| reverses the subsequence of |S| 
starting at item |it1| and ending at item |it2|. The requirement for
this operation is that the sequence |S| is sorted with respect to
the current compare function after the operation.

The graph |G| to be constructed has type |GRAPH<rat_point,rat_segment>|, i.e.,
it is a directed graphs where a |rat_point| (|rat_segment|) is associated with
each node (edge) of the graph. If |p| is a |rat_point| then
|G.new_node(p)| adds a new node to |G|, associates |p| with the node,
and returns the new node. If |v| and |w| are nodes of |G| and |s|
is a |rat_segment| then |G.new_edge(v,w,s)| adds the edge |(v,w)| to |G|, 
associates |s| with the edge, and returns the new edge.




@* The Basic Program Structure.

Our program has the following structure.
@c
@<include statements@>@;
@<global types and declarations@>@;
@<geometric primitives@>@;

void sweep(list<rat_segment>& S, GRAPH<rat_point,rat_segment>& G, bool use_filter)
{
 @<local declarations @>@;
 @<initialization @>@;
 @<sweep @>@;
 @<clean-up @>@;
}


@ During the sweep we use two local types |myPoint| and |mySegment|. 
They are extensions of the LEDA types |rat_point| and |rat_segment| which we use 
to make the program more efficient. A |myPoint| consists of a
|rat_point| plus floating point approximations to the homogeneous coordinates of
the point. Tests on |myPoint|s, e.g. the compare function, are first evaluated 
using the floating point approximations and the exact test is only 
performed if the floating point filter gives insufficient information. 
The details will be described in section \ref{geometric primitives}.

A |mySegment| consists of two |myPoint|s $p$ and $q$, the underlying LEDA 
segment |seg|, floating point approximations for the expressions 
$dx = px*qw-qx*pw$ and $dy = py*qw-qy*pw$ which are often used in the program,
and the last node |last_node| of the output graph $G$ lying on the segment
(intially |nil|).

We make both types pointer types to avoid the overhead of copying. 
Note that the objects of both types have multiple occurrences, e.g., 
in |myPoint|s occur in |mySegment|s and also in the X-structure.

We also need to say something about memory management. Our program
allocates storage for |myPoint|s and |mySegment|s. LEDA's memory mangement
feature is used to allocate this storage in big chunks and thus to avoid the
overhead of frequent calls to |malloc|. To free the memory again we use
different strategies for points and segments. A segment (but not its 
constituent points) is deleted when it is removed from the Y-structure. 
All points are collected in a hand-crafted linear list and are deleted 
in section |@<clean-up@>| at the end of |sweep|.



@<global types and declarations@>=

class MyPointRep;

static MyPointRep* first_myPoint = 0;


struct MyPointRep {
  rat_point pt;
  floatf   x;
  floatf   y;
  floatf   w;
  int     count;
  MyPointRep* next;

MyPointRep(const rat_point& p)
{ pt = p;
  x = floatf(p.X());
  y = floatf(p.Y());
  w = floatf(p.W());
  count = 0;
  next = first_myPoint;
  first_myPoint = this;
}
LEDA_MEMORY(MyPointRep)

};

typedef MyPointRep* myPoint;


struct MySegmentRep {

  myPoint   start;
  myPoint   end;
  rat_segment seg;

  floatf dx;
  floatf dy;

  node last_node;
 
MySegmentRep(const rat_segment& s)
{ start = new MyPointRep(s.start());
  end   = new MyPointRep(s.end());
  seg   = s;
  dx = floatf(s.dx());
  dy = floatf(s.dy());
  last_node = nil;
}
 
MySegmentRep(const myPoint& p) // creates the zero-length segment $(p,p)$
{ start = p;
  end = p;
  seg = rat_segment(p->pt,p->pt);
  last_node = nil;
}

LEDA_MEMORY(MySegmentRep)
};

typedef MySegmentRep* mySegment;



@ The programs uses three global variables: 
|Infinity| is a big integer constant that is used as a safe approximation for 
$\infty$; it will be initialized to a power of two greater than the maximal 
absolute value of any input coordinate. |p_sweep| is a |myPoint| that defines 
the current sweep position. |use_filter| is a flag that indicates whether 
the floating point filter should be applied, i.e., whether floating point 
computations should be used before doing exact arithmetic.


@<global types and declarations@> +=

integer     Infinity;
myPoint p_sweep;
bool    use_filter;


#if defined(STATISTICS)
int cmp_points_count;
int cmp_points_failed;
int exact_cmp_points_count;
int cmp_segments_count;
int exact_cmp_segments_count;
#endif


@ In the local declarations section of function |sweep| we introduce 
the data types for the event queue (|X_structure|) and for the sweep 
line (|Y_structure|). For the X-structure we use a sorted sequence of 
points with the lexicographic ordering of their coordinates, and for 
the Y-structure a sorted sequence of segments with the linear order 
defined by the sequence intersections of the segments with the sweep line 
at its current position (|p_sweep|) from bottom to top.

The X-structure contains all so far 
known event points right and above of the current sweep line position 
(|p_sweep|), i.e. all start and end points of segments and intersections 
of segments being adjacent in the sweep line. The X-structure associates 
with each event point |p| an item of the Y-structure (|seq_item|) as 
information; if there are only starting segments at |p| the information is 
|nil|, otherwise the information is an item in the Y-structure containing one 
of the segments passing through or ending in |p|. 

Vice versa we associate with each 
segment |s| in the Y-structure that intersects its successor in some point |p| 
the corresponding item |<p,...>| in the X-structure as information.
We call this item the {\sl current intersection item} of |s|. If |s| does not 
intersect its successor its current intersection item is |nil|.
Furthermore, we maintain for every point |p| a counter |count| that gives
the number of all segments having |<p,...>| as current intersection item.
We will use this counter to decide whether a point can be removed from the 
X-structure. It can be removed if |count| is zero.
Finally, we store with each segment |s| the last created node
of the output graph |last_node| lying on |s|.

@<local declarations @> =

sortseq<myPoint,seq_item> X_structure;

sortseq<mySegment,seq_item> Y_structure;


@ We now come to the initialization of the data structures. 
We first compute a big integer |Infinity| that can be used as a safe 
approximation for $\infty$. We start the sweep at $(-\infty,-\infty)$. 
At its initial position the sweep line (i.e. the Y-structure)
contains two infinitely long horizontal segments (|lowersentinel| and 
|uppersentinel|) with $y$-coordinates $-\infty$ and $+\infty$ respectively, 
and the output graph |G| is empty. 

We create for each |rat_segment| in |S| the corresponding |mySegment| 
(reorienting if necessary) and insert its left endpoint together with the 
information |nil| into the $X$-structure. We use the fact that a sorted 
sequence contains at most one item for every key and that a second insert 
operation with the same key only changes the information of the item to make 
the left endpoints of all segments unique. This makes equality tests between 
endpoints of segments much cheaper, since now the |myPoint| pointers can be 
compared directly (using the |==| operator) instead of having to call an 
expensive compare function. 

Finally, we sort all segments according to their left endpoints into
a list |S_Sorted| by calling the LEDA list sorting operation
|S_Sorted.sort(cmp_seg)|. Here |cmp_seg| is a compare function
defined in section \ref{geometric primitives}.



@<initialization @> =

#if defined(STATISTICS)
cmp_points_count = 0;
cmp_points_failed = 0;
exact_cmp_points_count = 0;
cmp_segments_count = 0;
exact_cmp_segments_count = 0;
#endif

::use_filter = use_filter;

/* compute an upper bound |Infinity| for the input coordinates */

Infinity = 1;

rat_segment s;

forall(s,S)
  while ( abs(s.X1()) >= Infinity || abs(s.Y1()) >= Infinity ||
          abs(s.X2()) >= Infinity || abs(s.Y2()) >= Infinity ) 
    Infinity *= 2;

p_sweep =  new MyPointRep(rat_point(-Infinity,-Infinity));


mySegment uppersentinel = 
new MySegmentRep(rat_segment(-Infinity,Infinity,Infinity,Infinity));

mySegment lowersentinel = 
new MySegmentRep(rat_segment(-Infinity,-Infinity,Infinity,-Infinity));

Y_structure.insert(uppersentinel,nil);
Y_structure.insert(lowersentinel,nil);


G.clear();

list<mySegment> S_Sorted;

forall(s,S)
{ 
  /* |mySegment|s are always oriented from left to right or (if vertical)
     from bottom to top
   */
  if (s.X1() > s.X2() || (s.X1() == s.X2() && s.Y1() > s.Y2()) )
    s = rat_segment(s.end(),s.start());

  mySegment s1 =  new MySegmentRep(s);
  S_Sorted.append(s1);
  seq_item it  = X_structure.insert(s1->start, nil);
  s1->start = X_structure.key(it);
  s1->start->count++;
}



S_Sorted.sort(cmp_mySeg);


@ To clean everything up we need to remove the two sentinels and all |myPoint|s.


@<clean-up @>=

{
 delete(uppersentinel);
 delete(lowersentinel);
 myPoint p = first_myPoint;
 while (first_myPoint != nil)
 { p = first_myPoint->next;
   delete(first_myPoint);
   first_myPoint = p;
  }

#if defined(STATISTICS)
if (use_filter)
{ cout << string("compare points:  %6d / %4d (%5.2f%%)  (%5.2f%% failed)  ",
                 cmp_points_count, exact_cmp_points_count,
                 (100.0*exact_cmp_points_count)/cmp_points_count, 
                 (100.0*cmp_points_failed)/cmp_points_count); 
  newline;
  cout << string("compare segments: %6d / %4d  (%5.2f%%)",
                 cmp_segments_count, exact_cmp_segments_count, 
                 (100.0*exact_cmp_segments_count)/cmp_segments_count); 
  newline;

 }
#endif

}



@ We now come to the heart of procedure sweep: the processing of events. Let
|event = <p,sit>| be the first event in the X-structure and assume 
inductively that our data structure is correct for 
$p\_sweep = (p.x,p.y-2\epsilon)$. Our goal is to to change |p_sweep| to |p|, 
i.e., move the sweep line across the point |p|. We perform the following 
actions.

We first add a vertex |v| with position |p| to the output graph |G|.
Then, we handle all segments passing through or ending at |p_sweep|. 
Finally, we insert all segments starting
at |p_sweep| into the Y-structure, check for possible intersections
between pairs of segments now adjacent in the Y-structure, and update 
the  X-structure. After having processed the |event| we delete it from 
the X-structure.



@<sweep @> =

while( ! X_structure.empty() )
{ 
  seq_item event = X_structure.min();
  seq_item sit = X_structure.inf(event);
  myPoint p = X_structure.key(event);
  node v = G.new_node(p->pt); 

  p_sweep = p;

  @<handle passing and ending segments @>@;

  @<insert starting segments and compute new intersections @>@;

  X_structure.del_item(event);
}


@ \label{misuse of compare}
We first handle all segments passing through or ending in point |p|. 
How can we find them?

Recall that the current event is $<p,sit>$ and that $sit \not= nil$ iff 
$p \in I \cup E$. If $sit \not= nil$ then $p$ is contained in the segment
associated with |sit|. If $sit = nil$ then $p \in St \setminus (I \cup E)$.
In this case there is at most segment in the Y-structure containing $p$. 
We may determine this segment by looking up the zero-length segment 
|(p->pt,p->pt)| in the Y-structure. We explain in section 
\ref{explanation of misuse} why this works.

After the lookup we have either $sit = nil$ and then no segment in the 
Y-structure contains |p| or $sit \not= nil$ and then the segment associated 
with $sit$ contains $p$. In the latter case we determine all such segments 
and update the graph $G$ and the Y-structure.

We also declare two items |sit_pred| and |sit_succ| and initialize them to 
|nil|. If the $Y$-structure contains a segment containing |p| then |sit_pred| 
and |sit_succ| will be the set to the predecessor and successor item of the
subsequence of segments containing |p|, otherwise they stay |nil|.
 


@<handle passing and ending segments @> =

  seq_item sit_succ = nil;
  seq_item sit_pred = nil;

  if (sit == nil) 
  { MySegmentRep s(p); // create a zero length segment $s = (p,p)$
    sit = Y_structure.lookup(&s);
   }
               
  if( sit != nil)
  { @<find subsequence of ending or passing segments @>@;
    @<construct edges and delete ending segments @>@;
    @<reverse subsequence of segments passing through |p|@>@;
   }


@ Taking |sit| as an entry point into the Y-structure we determine all segments
incident to |p| from the left or from below. These segments form a subsequence 
of the Y-structure. Let |sit_first| and |sit_last| denote the first and the 
last item of the subsequence and let |sit_pred| be the predecessor of 
|sit_first| and |sit_succ| the successor of |sit_last|. Note that the 
information of all items in this subsequence is equal to the current
event item |event|, except for |sit_last| whose information is either |nil|
or a different item in the X-structure resulting from an intersection
with |sit_succ|. 

Note also that the identification of the subsequence of segments incident to 
$p$ takes constant time per element of the sequence. Moreover, the constant 
is small since the test whether $p$ is incident to a segment involves no 
geometric computation but only equality tests between items.

Note finally that the code is particularly simple due to our sentinel segments:
|sit_first| can never be the first item of the Y-structure and |sit_last|
can never be the last.


@<find subsequence of ending or passing segments @> =

     seq_item sit_last = sit;

     while (Y_structure.inf(sit_last) == event)
         sit_last = Y_structure.succ(sit_last);

     sit_succ = Y_structure.succ(sit_last);
 
     sit_pred = Y_structure.pred(sit);

     while (Y_structure.inf(sit_pred) == event)
	sit_pred = Y_structure.pred(sit_pred);

     seq_item sit_first = Y_structure.succ(sit_pred);




@ We can now add edges to the graph |G|. For each segment in the subsequence 
between |sit_first| and |sit_last| inclusive we construct an edge. Let |sit| be 
any such item and let |s| be the segment associated with |sit|. We construct an 
edge connecting |s -> last_node| and |v| and label it with the segment |s|.
We also either delete the item from the Y-structure (if the segment ends at
|p|) or change its information to |nil| (if the segment does not end at |p|) to
reflect the fact that no intersection event is now associated with the 
segment. In the former case we free the storage reserved for the segment.

At the end we have to update variables |sit_first| and |sit_last|
since the corresponding items may have been deleted.


@<construct edges and delete ending segments @> =
     seq_item i1 = sit_pred;
     seq_item i2 = sit_first;

     while (i2 != sit_succ)
     { 
       mySegment s = Y_structure.key(i2);

       G.new_edge(s->last_node,v,s->seg); 
       s->last_node = v;

       if (p == s->end)  // ending segment
          { Y_structure.del_item(i2);
	    delete s;
	   }
       else
          { //continuing segment
            if (i2 != sit_last) Y_structure.change_inf(i2,nil);
            i1 = i2;
           }

       i2 = Y_structure.succ(i1);
      }
 
      sit_first = Y_structure.succ(sit_pred);
      sit_last = Y_structure.pred(sit_succ);


@ All segments remaining in the subsequence pass through node |v| and moving 
the sweep line through |p_sweep| inverses the order of the segments in the 
subsequence. The subsequence is non-empty iff |sit_last != sit_pred|.
Reversing the subsequence destroys the adjacency of pairs 
(|sit_pred|,|sit_first|) and (|sit_last|,|sit_succ|) in the |Y|-structure 
and hence we have to set the current intersection event of |sit_pred| and 
|sit_last| (i.e. the associated information in the Y-structure) to |nil|,
decrement the corresponding counters, and delete the items from the 
X-structure if the counters are zero now. If the subsequence is empty we 
only need to change the intersection event associated of |sit_pred| to |nil|.
Finally, we reverse the subsequence by calling
|Y_structure.reverse_items(sit_first,sit_last)|.


@<reverse subsequence of segments passing through |p| @> =

seq_item xit = Y_structure.inf(sit_pred);

if (xit != nil)
{ if (--X_structure.key(xit)->count == 0) X_structure.del_item(xit);
  Y_structure.change_inf(sit_pred,nil);
 }

if (sit_last != sit_pred)
{ xit = Y_structure.inf(sit_last);
  if (xit != nil)
  { if (--X_structure.key(xit)->count == 0) X_structure.del_item(xit);
    Y_structure.change_inf(sit_last,nil);
   }
  Y_structure.reverse_items(sit_first,sit_last);
}

@ The last step in handling the event point |p| is to insert all segments 
starting at |p| into the |Y|-structure and to test the new pairs of adjacent
items (|sit_pred|,...) and (...,|sit_succ|) for possible intersections. If
there were no segments passing through or ending in |p| then the items 
|sit_succ| and |sit_pred| are still |nil|  and we have to compute them now.

We use the sorted list |S_Sorted| to find all segments to be 
inserted. As long as the first segment |seg| in |S_Sorted| starts at point |p| 
we remove it from the list, insert it into the |Y|-structure, add its right 
endpoint to the |X|-structure, and set |seg -> last_node| to |v|. If the 
segment has length zero we simply discard it and if |sit_succ| and |sit_pred| 
are still undefined (|nil|) then we use the first inserted segment to define 
them. In this case, we also have to change the possible current intersection
event of |sit_pred| to |nil| since it is no longer adjacent to |sit_succ|.
If all segments starting in |p| have length zero then |sit_succ| and 
|sit_pred| remain undefined. 

We need to say more clearly how to insert a segment |s| into the |Y|-structure.
If the |Y|-structure contains no segment with the same underlying line then we
simply add the segment. Otherwise, let $s'$ be the segment in the |Y|-structure
with the same underlying line. We replace |s| by $s'$ if $s'$ extends further 
to the right than $s$ and do nothing otherwise.

After having inserted all segments starting at |p|, we test whether |sit_succ| 
(|sit_pred|) intersects its predecessor (successor) in the |Y|-structure.

@<insert starting segments and compute new intersections@> =

while (!S_Sorted.empty() && p == S_Sorted.head()->start)
{ 
  mySegment Seg = S_Sorted.pop();

  /* first insert the right endpoint of |Seg| into the X-structure */

  seq_item end_it= X_structure.insert(Seg->end,nil);
  Seg->end = X_structure.key(end_it);
  Seg->end->count++;

  /* note that the following test uses the fact that two endpoints are equal 
     if an only if the corresponding pointer values (|myPoint|s) are equal
   */

  if (Seg->start == Seg->end)   // Seg has length  zero, nothing to do
  { delete Seg;
    continue;
   }

  sit = Y_structure.locate(Seg);

  if (compare(Seg, Y_structure.key(sit)) != 0) 
   { /* |Seg| is not collinear with the segment associated with |sit|. We
        simply insert |Seg| into the Y-structure 
      */
     sit = Y_structure.insert_at(sit,Seg,nil); 
     Seg->last_node = v;
    }
  else 
   { /* |Seg| is collinear with the segment associated with |sit|. If |Seg| is 
        longer then we use |Seg| and otherwise we do nothing.
       */

     mySegment Seg_old = Y_structure.key(sit);

     if (compare(Seg->end, Seg_old->end) > 0)
     { /* |Seg| extends further to the right or above 
          replace |Seg_old| by |Seg|.  */
       Seg_old->seg = Seg->seg;
       Seg_old->end = Seg->end;
      }

     delete Seg;   // not needed anymore
    }

   X_structure.change_inf(end_it,sit);

   if (sit_succ == nil)
   { sit_succ = Y_structure.succ(sit);
     sit_pred = Y_structure.pred(sit);

     /* |sit_pred| is no longer adjacent to |sit_succ| we have to
        change its current intersection event to |nil| and delete
        the corresponding item in the X-structure if necessary
      */

     seq_item xit = Y_structure.inf(sit_pred);
     if (xit != nil)
     { if (--X_structure.key(xit)->count == 0) X_structure.del_item(xit);
       Y_structure.change_inf(sit_pred,nil);
      }
    }
}

/* compute possible intersections */

if (sit_succ != nil)  // |v| is an isolated vertex otherwise
{
  compute_intersection(X_structure,Y_structure,sit_pred);

  sit = Y_structure.pred(sit_succ);

  if (sit != sit_pred) 
    compute_intersection(X_structure,Y_structure,sit);
 }




@* Geometric Primitives.
\label{geometric primitives}

It remains to define the geometric primitives used in the implementation. We
need four:

\begin{itemize}

\item
  a compare function for |myPoint|s which orders points according 
  to the lexicographic ordering of their coordinates. It defines the linear 
  order used in the X-structure.

\item
  a compare-function for |mySegment|s which given two segments intersecting the
  sweep line $L$ determines the order of the intersections on $L$. It defines 
  the linear order used in the Y-structure.

\item
  a second compare function |cmp_mySeg| for |mySegment|s which orders segments
  according to the order of their left endpoint. It is used to sort the list 
  |S_Sorted| in the beginning.

\item
  a function |compute_intersection| that decides whether two segments intersect
  and if so whether the intersection is to the right of the sweep line. If
  both tests are positive it also makes the required changes to the $X$- and
  $Y$-structure.

\end{itemize}


We define the compare functions for |myPoint|s and |mySegment|s in two steps.
We first write two function templates |cmp_points| and |cmp_segments|
for comparing points and segments given by their homogenous coordinates,
independently from the actual (numerical) type of the coordinates.
These templates can be used to compare points and segments with coordinates 
of any integer type |int_type| that supports addition, subtraction, 
multiplication, and a special sign function |Sign(x)| that returns
$+1$ if $x > 0$, $0$ if $x = 0$, $-1$ if $x < 0$, and a special integer 
value |NO_IDEA| if the test cannot be performed in a conclusive way.


We start with writing a compare function |cmp_points(x1,y1,w1,x2,y2,w2)|
that compares two points with homogeneous coordinates $(x_1,y_1,w_1)$ and 
$(x_2,y_2,w_2)$. A point $(x_1,y_1,w_1)$ precedes a point $(x_2,y_2,w_2)$ 
if the pair $(x_1/w_1,y_1/w_1)$ lexicographically precedes the pair 
$(x_2/w_2,y_2/w_2)$, i.e., if ($x_1w_2 > x_2w_1$) or ($x_1w_2 = x_2w_1$ and 
$y_1w_2 > y_2w_1$).


@<geometric primitives @> =

/*
template <class int_type> inline
int cmp_points(const int_type& x1, const int_type& y1, const int_type& w1,
               const int_type& x2, const int_type& y2, const int_type& w2)
{ 
  int s = Sign(x1*w2-x2*w1);

  return (s != 0) ? s : Sign(y1*w2-y2*w1);
 }
*/

inline int cmp_points(const integer& x1, const integer& y1, const integer& w1,
                      const integer& x2, const integer& y2, const integer& w2)
{ 
  int s1 = sign(x1) * sign(w2);
  int s2 = sign(x2) * sign(w1);

  if (s1 > s2) return +1;
  if (s1 < s2) return -1;

  int l1 = x1.length() + w2.length();
  int l2 = x2.length() + w1.length();

  if (l1 > l2+1) return   s1;
  if (l2 > l1+1) return  -s1;

/*
  int s = cmp_products(x1,w2,x2,w1);
  return (s != 0) ? s : cmp_products(y1,w2,y2,w1);
*/


  int s = compare(x1*w2,x2*w1);
  return (s != 0) ? s : compare(y1*w2,y2*w1);

 }


inline int cmp_points(const floatf& x1, const floatf& y1, const floatf& w1,
                      const floatf& x2, const floatf& y2, const floatf& w2)
{ 
  int s = Sign(x1*w2-x2*w1);
  return (s != 0) ? s : Sign(y1*w2-y2*w1);
 }


inline int cmp_points(const double& x1, const double& y1, const double& w1,
                      const double& x2, const double& y2, const double& w2)
{ 
  int s = Sign(x1*w2-x2*w1);
  return (s != 0) ? s : Sign(y1*w2-y2*w1);
 }



@ Next we write the compare function |cmp_segments| for segments.

A segment is stored as the pair of its endpoints and a point is stored
by its homogeneous coordinates, i.e., as a triple $(X,Y,W)$ of integers (type 
|integer|). The $x$- and $y$-coordinates of the point are $X/W$ and $Y/W$ 
respectively. 
|cmp_segments| takes the coordinates of two segments $s_1$ and $s_2$ 
intersecting the sweep line |L|.
We assume that both segments have non-zero length and treat the case that one of
them has zero length in the next section.
If both segments have the same underlying line then we return 0. 
So let us assume that the underlying lines are different. Let |l| denote the
vertical line through |p_sweep|. Only one of the segments can be vertical. If
$s_1$ is vertical and hence intersects |L| in $(x\_sweep,y\_sweep+\epsilon)$ 
then $s_1$ is before $s_2$ iff |p_sweep| is below $l\cap s2$. If $s_2$ is
vertical then $s_1$ is before $s_2$ iff $l\cap s1$ is not above |p_sweep|.
Assume now that neither $s_1$ nor $s_2$ is vertical. If $l\cap s1$ and 
$l\cap s2$ are distinct then $s_1$ is before $s_2$ iff $l\cap s1$ 
is below $l\cap s2$. If $l\cap s1$ and $l\cap s2$ are identical and not above
|p_sweep| then $s_1$ is before $s_2$ iff $s_1$ has the smaller slope. Finally,
if the intersections are identical and above |p_sweep| then $s_1$ is before
$s_2$ iff $s_1$ has the larger slope. 



@<geometric primitives @> +=


template <class int_type>
int cmp_segments( const int_type& px,  const int_type& py,  const int_type& pw,
                  const int_type& spx, const int_type& spy, const int_type& spw,
                  const int_type& sqx, const int_type& sqy, const int_type& sqw,
                  const int_type& rx,  const int_type& ry,  const int_type& rw,
                  const int_type& dx,  const int_type& dy,
                  const int_type& sdx, const int_type& sdy)
{
@;
  /*
    We first test whether the underlying lines are identical. The lines are
    identical if the three slopes $dy/dx$, $sdy/sdx$, and $mdy/mdx$ are equal
  */
@;

  
  int_type T1 = dy*sdx - sdy*dx;

  int sign1 = Sign(T1);


  if (sign1 == 0 || sign1 == NO_IDEA)
  { int_type mdx = sqx*pw - px*sqw;
    int_type mdy = sqy*pw - py*sqw;
    int sign2 = Sign(dy*mdx - mdy*dx);
    if (sign2 == 0 || sign2 == NO_IDEA)
    { int sign3 = Sign(sdy*mdx - mdy*sdx);
      if (sign3 == 0 || sign3 == NO_IDEA)
      return (sign1 == 0 && sign2 == 0 && sign3 == 0)  ? 0 : NO_IDEA;
     }
   }


@;
   /* The underlying lines are different; in particular, at most one of the
      lines is vertical. We first deal with the cases that one of the lines 
      is vertical. A segment $(p,q)$ is vertical iff $px*qw-qx*pw$ is equal 
      to zero. Since |dx| is an optimal floating point approximation
      of this integer value, a segment is vertical iff its |dx|-value is zero.
    */
@;
    if (dx == 0)
       { /* |dx| = 0, i.e., |s1| is vertical and |s2|  is not vertical; 
            $l \cap s2$ is above |p_sweep| iff 
            $(spy/spw + sdy/sdx(rx/rw - spx/spw) -ry/rw) > 0$; 
            in this case we return $-1$ 
          */

          int i = Sign((spy*sdx - spx*sdy)*rw + (sdy*rx - ry*sdx)*spw);
          if (i==NO_IDEA) return NO_IDEA;
          return (i <= 0) ? 1 : -1;
        }
@;
    if (sdx == 0)
       { /* $sdx = 0$, i.e., |s2| is vertical but |s1| is not vertical; 
            we return -1 if
            $l \cap s1$ is below or equal to |p_sweep| iff 
            $(py/pw + dy/dx(rx/rw - px/pw)-ry/rw) \leq 0$.
          */

         int i = Sign((py*dx - px*dy) * rw + (dy*rx - ry*dx) * pw);
         if (i==NO_IDEA) return NO_IDEA;
         return (i <= 0) ? -1 : 1;
       }
@;
  /* Neither |s1| nor |s2| is vertical. We compare $l \cap s1$ and $ l \cap s2$.
     We have $$ y(l \cap s1) -y(l \cap s2) = 
     \frac{py}{pw} +\frac{dy}{dx}(\frac{rx}{rw} -\frac{px}{pw}) -\frac{spy}{spw}
     - \frac{sdy}{sdx}(\frac{rx}{rw} - \frac{spx}{spw}). $$ 
     If the difference is non-zero then we return its sign. If the difference is
     zero then we return $-1$ iff the common intersection is not above 
     |p_sweep| and $s1$ has the smaller slope or the intersection is above 
     |p_sweep| and $s1$ has the larger slope.
  */
@;
  int_type T2 = sdx*spw*(py*dx*rw+dy*(rx*pw-px*rw)) 
                - dx*pw*(spy*sdx*rw+sdy*(rx*spw-spx*rw));

  int sign2 = Sign(T2);

  if (sign2 == NO_IDEA) return NO_IDEA;

  if (sign2 != 0) return sign2;
@;
  /* Now we know that the difference is zero, i.e., |s1| and |s2| 
     intersect in a point $I$. We compare slopes:\\
     |s1| has larger slope than |s2| iff |T1*dxs*dx > 0|; 
     note that orienting the lines from left to right makes all |dx| 
     values non-negative , i.e., 
     |s1| has larger slope than |s2| iff |sign(T1) = 1|
   */

  int_type T3 = (py*dx - px*dy) * rw + (dy*rx - ry*dx) * pw;

  /* The common intersection $I$ is above |p_sweep| iff $T3*rw*dx*pw > 0$.
     In this case we return |-sign(T1)| and |sign(T1)| otherwise.
     Note that all |dx| and |w| values are non-negative
     i.e., |sign(T3*rw*dx*pw) = sign(T3)|
   */

  int sign3 = Sign(T3);

  if (sign3 == NO_IDEA) return NO_IDEA;

  return (sign3 <= 0) ? sign1 : -sign1;

}


@ Finally, we define the compare functions for |myPoint|s and |mySegment|s
by first calling |cmp_points| and |cmp_segments| on the floating point
filter coordinates (of type |floatf|) of the corresponding points and segments.
In the case that these calls do not return a reliable result (i.e. return
|NO_IDEA|) we call them again with the exact coordinates (of type |integer|).


@<geometric primitives @> +=

inline int compare(const myPoint& a, const myPoint& b)
{ 
  /* floating point filter version for |myPoint|s */

#if defined(STATISTICS)
  cmp_points_count++;
#endif

 int c = NO_IDEA;

 /* if not explicitely turned off we first use floating point arithmetic */

 if (use_filter)
   c = cmp_points( a->x, a->y, a->w, b->x, b->y, b->w);

 /* if the floating point computation is not reliable, i.e., the result 
    is |NO_IDEA| we use exact arithmetic (|integer|) 
  */

 if (c == NO_IDEA) 
 {
   c = cmp_points(a->pt.X(),a->pt.Y(),a->pt.W(), b->pt.X(),b->pt.Y(),b->pt.W());

#if defined(STATISTICS)
  exact_cmp_points_count++;

  if (cmp_points(double(a->x),double(a->y),double(a->w),
                 double(b->x),double(b->y),double(b->w)) != c) 
     cmp_points_failed++;
#endif

  }

 return c;
}

 
inline int compare(const mySegment& s1,const mySegment& s2)
{ 
  int c = NO_IDEA;

  /* if not explicitely turned off we first try the 
     floating point computation 
   */

#if defined(STATISTICS)
cmp_segments_count++;
#endif

  if (use_filter)
      c =  cmp_segments(s1->start->x, s1->start->y, s1->start->w,
                        s2->start->x, s2->start->y, s2->start->w,
                        s2->end->x,   s2->end->y,   s2->end->w,
                        p_sweep->x,   p_sweep->y,   p_sweep->w,
                        s1->dx, s1->dy, s2->dx, s2->dy);
    

  /* If the result is not reliable  we call the exact compare 
     for the underlying |rat_segment|s.
   */

  if (c == NO_IDEA)
  { c = cmp_segments(s1->seg.X1(), s1->seg.Y1(), s1->seg.W1(),
                     s2->seg.X1(), s2->seg.Y1(), s2->seg.W1(), 
                     s2->seg.X2(), s2->seg.Y2(), s2->seg.W2(),
                     p_sweep->pt.X(), p_sweep->pt.Y(), p_sweep->pt.W(),
                     s1->seg.dx(),s1->seg.dy(), s2->seg.dx(),s2->seg.dy());
#if defined(STATISTICS)
    exact_cmp_segments_count++;
#endif
   }

  return c;
}


@ For the initialization of the list |S_Sorted| we need a second compare 
function |cmp_mySeg| for |mySegment|s. It simply compares the left endpoints.

@<geometric ...@>+=

int cmp_mySeg(const mySegment& s1, const mySegment& s2) 
{ 
  int c = NO_IDEA;

  if (use_filter) // floating point compare
     c = cmp_points(s1->start->x, s1->start->y, s1->start->w,
                    s2->start->x, s2->start->y, s2->start->w);
  
  if (c == NO_IDEA) // exact compare
     c = cmp_points(s1->seg.X1(),s1->seg.Y1(),s1->seg.W1(),
                    s2->seg.X1(),s2->seg.Y1(),s2->seg.W1());
  return c;
}



@ \label{explanation of misuse}
What does compare do when one of the segments, say |s1|, has both endpoints 
equal to |p_sweep| and the Y-structure contains 
at most one segment containing |p_sweep|. That's exactly 
the situation in section \ref{misuse of compare}. When |s2| 
contains |p_sweep| the underlying lines are found identical and 
compare returns 0. When |s2| does not contain 
|p_sweep| then the underlying lines are declared different. 
Also |s1| is declared vertical and
|s2| is not vertical since it would otherwise contain |p_sweep|. 
We now compare
$ l \cap s2$ and |p_sweep| and return the result. We conclude that the call
|Y_structure.locate(s)| where $s$ is the zero-length segment |(p->pt,p->pt)| 
in section \ref{misuse of compare} has the desired effect.




@ Finally, we define a function |compute_intersection| that takes an item
|sit0| of the Y-structure and determines whether the segment associated with
|sit0| intersects the segment associated with the successor of |sit0|
right or above of the sweep line. If so it updates the X- and the Y-structure.

The function first tests whether the underlying straight lines intersect right 
or above of the sweep line by comparing their slopes. This test is performed
with floating point arithmetic and repeated with exact arithmetic if the
floating point test gives no reliable result.  If the segments do intersect
right or above of the sweep line it is checked whether the point of 
intersection lies on both segments, i.e., is not larger than the endpoints of 
the segments.


@<geometric ...@>+=

void compute_intersection(sortseq<myPoint,seq_item>& X_structure,
                          sortseq<mySegment,seq_item>& Y_structure, 
                          seq_item sit0)
{ 
  seq_item  sit1 = Y_structure.succ(sit0);
  mySegment seg0 = Y_structure.key(sit0);
  mySegment seg1 = Y_structure.key(sit1);

  /* |seg1| is the successor of |seg0| in the Y-structure, hence, 
     the underlying lines intersect right or above of the sweep line 
     iff the slope of |seg0| is larger than the slope of |seg1|. 
   */

  if (use_filter)
  { int i = Sign(seg0->dy * seg1->dx - seg1->dy * seg0->dx);
    if (i == -1 || i == 0) return; /* $slope(s0)\leq slope(s1)$ */
   }


  rat_segment s0 = seg0->seg;
  rat_segment s1 = seg1->seg;

  integer w = s0.dy()*s1.dx() - s1.dy()*s0.dx();

  if (sign(w) > 0)   // $slope(s0) > slope(s1)$
  {
    integer c1 = s0.X2()*s0.Y1() - s0.X1()*s0.Y2();
    integer c2 = s1.X2()*s1.Y1() - s1.X1()*s1.Y2();

    /* The underlying lines intersect in a point right or above of the 
       sweep line. We still have to test whether it lies on both segments.
     */

    integer x = c2*s0.dx()-c1*s1.dx();


    integer d0 = x*s0.W2() - s0.X2()*w;
    if (sign(d0) > 0) return;

    if (x*s1.W2() > s1.X2()*w) return;

    integer y = c2*s0.dy()-c1*s1.dy();
    if (sign(d0)==0 && y*s0.W2() > s0.Y2()*w) return;

    myPoint Q = new MyPointRep(rat_point(x,y,w));
    seq_item xit = X_structure.insert(Q,sit0);
    X_structure.key(xit)->count++;
    Y_structure.change_inf(sit0,xit);

  }
}
 



@* A Floating Point Filter. 
\label{floatf}

The type |floatf| provides a clean and efficient way to approximately
compute with large integers. Consider an expression |E| with integer
operands and operators $+,-$, and $*$, and suppose that we want
to determine the sign of |E|. In general, the integer arithmetic
provided by our machines does not suffice to evaluate |E| since intermediate
results might overflow. Resorting to arbitrary precision integer
arithmetic is a costly process. An alternative is to evaluate the 
expression using floating point arithmetic, i.e., to convert the
operands to doubles and to use floating-point addition, subtraction,
and multiplication. Of course, only an approximation $\tilde{E}$ of
the true value |E|\footnote{We misuse notation and use |E| to denote the
expression and its value.} is computed. However, $\tilde{E}$ might still be
able to tell us something about the sign of |E|. If $\tilde{E}$ is far
away from zero (the forward error analysis carried out in the next
section gives a precise meaning to "far away") then the 
signs of $\tilde{E}$ and |E| 
agree and if $\tilde{E}$ is zero then we may be able to conclude under
certain circimstances that |E| is zero. Again, forward error analysis
can be used to say what `certain circumstances' are. The type |floatf|
encapsulates this kind of approximate integer arithmetic. Any
integer (= object of type |integer|) can be converted to a |floatf|,
|floatf|s can be added, subtracted, multiplied, and their sign
can be computed: for any |floatf| $x$ the function |Sign(x)| returns 
either the sign of $x$ ($-1$ if $x < 0$, $0$ if $x = 0$, and $+1$ 
if $x > 0$) or the special value |NO_IDEA|.
If |x| approximates |X|, i.e., |X| is the integer value abtained by an 
exact computation, then |Sign(x) != NO_IDEA| implies that |Sign(x)| 
is actually the sign of $X$ if
|Sign(x) = NO_IDEA| then no claim is made about the sign of |X|.\\
\smallskip
\underline{Declaration}\\
\bigskip
|floatf x| declares |x| as a variable of type |floatf|\\
\smallskip
\underline{Operations}\\
\parbox{8.5cm}{|floatf floatf(integer i)|} 
\parbox[t]{6.5cm}{converts |i| to a float}\\
\parbox{8.5cm}{|floatf +(floatf a, floatf b)|}
\parbox[t]{6.5cm}{Addition}\\
\parbox{8.5cm}{|floatf -(floatf a, floatf b)|}
\parbox[t]{6.5cm}{Subtraction}\\
\parbox{8.5cm}{|floatf *(floatf a, floatf b)|}
\parbox[t]{6.5cm}{Multiplication}
\parbox{8.5cm}{|{-1,0,+1,NO_IDEA} Sign(floatf x)|}
\parbox[t]{6.5cm}{as described above}\\

A |floatf| is represented by a double (its value) and an error bound.
An operation on |floatf|s performs the corresponding operation on
the values and also computes the error bound for the result. For
this reason the cost of a |floatf| operation is about four times
the cost of the corresponding operation on
doubles. Rules \ref{first rule} to \ref{sixth rule} below are used
to compute the error bounds.

@ Floating Point Numbers.

A {\em floating point number} is a number of the form
\[ a = m\cdot 2^e,\]
where $e\in [e_{min}.. e_{max}]$ is
the {\em exponent}, and $m$ is the {\em mantissa}. The mantissa is a rational
number of the form
\[m = v\cdot \sum_{1\leq i\leq l} m_i\cdot 2^{-i},\]
where $v\in \{ -1,1\}$ is the sign, $l$ is the {\em mantissa length}, and the
$m_i$'s are the {\em digits} of the mantissa. We have $m_i\in \{0,1\}$ and 
either $m = 0$ or $m_1 = 1$. One also says that the mantissa is
{\em normalized}. We also assume that $l\leq e_{\max}$. The latter 
assumption implies that all integers whose absolute value is less than
$2^{l+1}$ can be presented as floating point numbers.

Let $\cal{F}$ denote the set of all floating point numbers; the set $\cal{F}$ 
clearly depends on the exponent range $[e_{min} .. e_{max}]$
and the mantissa length $l$. In our examples, we use $l = 53$,
$e_{min} = -1022$, and $e_{max} = 1023$.
This choice corresponds to the  IEEE double precision floating point standard.

The absolute values of all non-zero floating point numbers lie between 
$min\_abs = 2^{e_{\min}-1}$ and $max\_abs = (1-2^{-l}) 2^{e_{\max}}$. 
Call a real number 
{\em representable} if it is either 0 or its absolute value lies in the
interval $[min\_abs, max\_abs]$ and let the size |size(a)| of a real
number |a| be the smallest power of two larger than its absolute 
value,i.e.,
\[ size(a) = \left \{
             \begin{array}{rl}
	     0 & \mbox{ if } a=0\\
	     2^{\lceil \log\abs{a} \rceil} & \mbox{ if }a\neq 0
	     \end{array}
	     \right.
\]

Then $size(a)/2\leq \abs{a}\leq size(a)$. If a number |a| is 
representable then there is a floating point number $fl(a)\in \cal{F}$
such that
\begin{eqnarray} \label{fl}
\abs{a - fl(a)}\leq 1/2\cdot 2^{-l}\cdot size(a)\leq 2^{-l}\abs{a}
\end{eqnarray}
The number $fl(a)$ can be obtained from $a$ by rounding in the $l$-th most
significant position. We call $fl(a)$ a {\em floating point approximation} of
$a$ and use $eps$ to denote $2^{-l}$. Note that $fl(a)$ is a floating point
number that is closest to $a$.
The set $\cal{F}$ of floating point numbers is not closed under the arithmetic 
operations addition, subtraction, and multiplication, i.e., if $f_1$ and 
$f_2$ are floating point numbers and $op$ is any one of the arithmetic 
operations then $f_1 op f_2$ does not necessarily belong to $\cal{F}$. The
machine implementation of the operation (we use $\oplus, \ominus$, and
$\odot$ to denote the implementation of $+, -$, and $\cdot$ respectively) 
therefore returns a floating point approximation of the exact result, 
i.e., a number $\tilde{f}$ such that
\begin{eqnarray} \label{op}
\abs{\tilde{f} - f_1 op f_2}\leq 1/2\cdot eps\cdot size(f_1op f_2) 
\leq eps\cdot \abs{f_1 op f_2}
\end{eqnarray}
or equivalently
\begin{eqnarray*} \tilde{f} = (1+\epsilon)\cdot(f_1 op f_2)\end{eqnarray*}
for some $\epsilon$ with $\abs{\epsilon}\leq eps$.

Note that if $f_1 op f_2$ is a floating point number then $\tilde{f} = f_1 op f_2$ 
since the computed result is the floating point number closest to the exact
result. In particular, if $f_1$ and $f_2$ are both powers of 2 then
$f_1\odot f_2 = f_1\cdot f_2$ and if $f_1 = f_2$ and $f_1$ is a
power of 2 then $f_1\oplus f_2 = f_1 + f_2 = 2\cdot f_1$. The floating point
operations $\oplus$ and $\odot$ also satisfy a monotonicity property, namely,
if $f_1\leq g_1$ and $f_2\leq g_2$ then $f_1\oplus f_2 \leq g_1\oplus g_2$ and
$f_1\odot f_2\leq g_1\odot g_2$. For the second inequality we also need
$f_1\geq 0$ and $f_2\geq 0$.

We next turn to expression evaluation. Let $E$ be an arithmetic expression,
e.g., $E = a\cdot b - c\cdot d$. When we evaluate $E$ using floating point 
arithmetic we obtain $\tilde{E} = a\odot b\ominus c\odot d$. What is the relationship
between the exact value
$E$ and the computed value $\tilde{E}$?
We have
\begin{eqnarray*}
a\odot b&=&(1+\epsilon_1)\cdot a\cdot b\\
c\odot d&=&(1+\epsilon_2)\cdot c\cdot d\\
a\odot b\ominus c\odot d&=&(1+\epsilon_3)(a\odot b - c\odot d)
\end{eqnarray*}
for some constants $\epsilon_1$, $\epsilon_2$, $\epsilon_3$ with 
$\abs{\epsilon_i}\leq eps$ and hence
\begin{eqnarray*}
\tilde{E}&=&(a\cdot b\cdot (1+\epsilon_1) - c\cdot d\cdot (1+\epsilon_2))(1+\epsilon_3)\\
&=&E+E\cdot \epsilon_3 + (a\cdot b\cdot \epsilon_1 - c\cdot d\cdot \epsilon_2)(1+\epsilon_3).
\end{eqnarray*}
The error
\[\tilde{E} - E = E\cdot \epsilon_3 + a\cdot b\cdot \epsilon_1 - 
                  c\cdot d\cdot \epsilon_2 +(a\cdot b\cdot \epsilon_1 - 
                  c\cdot d\cdot \epsilon_2)\cdot \epsilon_3\]
is therefore essentially the sum of the errors introduced by considering the 
three operations individually. In addition, there is a `high-order' term.

One can drop the higher-order term by expressing the error in terms of upper
bounds on the inputs of an expression rather than the actual input values
themselves. This leads to weaker but more managable error bounds. 


@ A Floating Point Filter.

Throughout this section |E| denotes an expression involving real operands
and the arithmetic operations addition, subtraction, and multiplication. 
We use |E| to also denote the value of the expression. 
We assume that the operands
and more generally the values of all subexpressions to be representable
and use $\tilde(E)$ to denote the value of the expression when
evaluated with floating point arithmetic, i.e., first all operands
are replaced by their floating point approximations and then all 
operations by their floating point counterparts. Our goal is to derive an
easily computed bound for the difference between the computed
value $\tilde{E}$ and the exact value |E|.

To this end we define for every expression |E| its {\em measure} $mes(E)$
and its {\em index} $ind(E)$. The 
measure $mes(E)$ is a power of two which bounds the size of $E$
and $\tilde{E}$ from above, i.e.,
\begin{eqnarray} \label{M}
size(E)\leq mes(E) \mbox{ and } size(\tilde{E})\leq mes(E)
\end{eqnarray} 
The index  $ind(E)$ bounds the difference of the computed result $\tilde{E}$
and the exact value $E$ as a multiple of $eps\cdot mes(E)$, i.e.,
\begin{eqnarray} \label{I}
\abs{\tilde{E} - E} \leq ind(E)\cdot eps \cdot mes(E).
\end{eqnarray}
The crucial observation is now that both quantities are easily computed 
inductively. Here are the rules:

If |E = a|, where |a| is a representable real number, then
\begin{equation} mes(E) = size(a) \label{first rule}
\end{equation}
and 
\begin{equation} ind(E) = \left \{
                 \begin{array}{rl}
		 0 & \mbox{ if }a\in \cal{F}\\
		 1/2 & \mbox{ otherwise }
		 \end{array} \right.
		 \label{second rule} \end{equation}
if $E = E_1 \pm E_2$ then
\begin{eqnarray}
mes(E)&=&2\cdot \max(mes(E_1),mes(E_2)) \label{third rule}
\end{eqnarray}
and
\begin{eqnarray}
ind(E)&=&(1 + ind(E_1) + ind(E_2))/2, \label{fourth rule}
\end{eqnarray}
and if $E = E_1\cdot E_2$ then
\begin{eqnarray}
mes(E)&=&mes(E_1)\cdot mes(E_2) \label{fifth rule}
\end{eqnarray}
and
\begin{eqnarray}
ind(E)&=&1/2 + ind(E_1) + ind(E_2). \label{sixth rule}
\end{eqnarray}

\begin{lemma}
Let |E| be an expression with real operands and assume that the
values of all subexpressions of |E| are representable. Then rules 
(\ref{first rule}) to (\ref{sixth rule}) compute quantities $mes(E)$
and $ind(E)$ satisfying (\ref{M}) and (\ref{I}).
\end{lemma}
\begin{proof}:
If |E = a|, where |a| is a real, then $\tilde{E} = fl(a)$.
Clearly, $\abs{a}\leq size(a)$ and $\abs{fl(a)}\leq size(a)$ (note 
however that $\abs{a}\leq size(fl(a))$ does not always hold). This
establishes (\ref{M}). Inequality (\ref{fl}) implies (\ref{I}).

If $E = E_1op E_2$ then let
$\tilde{E}$, $\tilde{E}_1$, and $\tilde{E}_2$ be the computed values for
$E$, $E_1$, and $E_2$. Then $\abs{E_i}\leq mes(E_i)$, $\abs{\tilde{E}_i}\leq 
size(\tilde{E}_i)\leq mes(E_i)$, 
and $\abs{\tilde{E}_i - E_i}\leq ind(E_i)\cdot eps\cdot mes(E_i)$. Use 
$err(E_i)$ to abreviate $\tilde{E}_i - E_i$.

Assume first that $E = E_1 + E_2$; the argument for subtraction is analogous.
Clearly, $size(E) = size(E_1 + E_2)\leq 2\cdot \max(size(E_1),size(E_2))
\leq 2\cdot (mes(E_1),mes(E_2)) = mes(E)$ and
$size(\tilde{E}) = size(\tilde{E}_1\oplus \tilde{E}_2)\leq
size(\max(size(\tilde{E}_1),size(\tilde{E}_2))\oplus 
\max(size(\tilde{E}_1),size(\tilde{E}_2)))\leq
size(2\cdot \max(mes(E_1),mes(E_2))) \leq size(mes(E)) = mes(E)$
where the first inequality follows from the monotonicity of $\oplus$ 
and the second inequality follows from the fact that equal powers of
two are added exactly. Similar reasoning shows that 
$size(\tilde{E}_1+\tilde{E}_2)\leq mes(E)$. We turn to the error bound
next. We have
\[
\begin{array}{ll}
\abs{\tilde{E} - E}
&= \abs{\tilde{E}_1\oplus \tilde{E}_2 -(\tilde{E}_1+\tilde{E}_2)+\tilde{E}_1 + 
       \tilde{E}_2 - E_1 - E_2}\\
& \leq 1/2\cdot eps\cdot size(\tilde{E}_1+\tilde{E}_2) + err(E_1) + err(E_2)\\
& \leq (1/2\cdot mes(E) + ind(E_1)\cdot mes(E_1) + ind(E_2)\cdot 
       mes(E_2))\cdot eps\\
& = (1+ind(E_1) + ind(E_2))/2 \cdot mes(E)\cdot eps,
\end{array}
\]
where the first inequality follows from (\ref{op}).

Assume next that $E = E_1\cdot E_2$. We leave it to the reader to show
that $size(E)\leq mes(E)$, $size(\tilde{E}) \leq mes(E)$,
and $size(\tilde{E}_1\cdot \tilde{E}_2)\leq mes(E)$. For the error
bound we obtain
\[
\begin{array}{ll}
\abs{\tilde{E}-E}
&= \abs{\tilde{E}_1\odot \tilde{E}_2 - \tilde{E}_1\cdot \tilde{E}_2 + 
   (E_1 + err(E_1))\cdot (E_2 + err(E_2))- E_1\cdot E_2}\\
&\leq 1/2\cdot eps\cdot size(\tilde{E}_1\cdot \tilde{E}_2)
   + err(E_1)\cdot \abs{E_2} + \abs{\tilde{E}_1}\cdot err(E_2)\\
&\leq (1/2\cdot mes(E) + ind(E_1)\cdot mes(E_1)\cdot size(E_2)
   + size(E_1)\cdot ind(E_2)\cdot mes(E_2)\cdot eps \\
&= (1/2 + ind(E_1) + ind(E_2))\cdot eps\cdot mes(E)
\end{array}
\]
This completes the proof of the lemma.
\end{proof}

How are we going to use the error estimates? Assume that |E| is an expression
involving \underline{integral} operands and operators $+,-$, and
$*$. Assume further that we want to know the sign of |E|. We evaluate
|E| using floating point arithmetic, i.e., we convert the operands into
floating point numbers and then use the floating point operations
$\oplus$, $\ominus$, and $\odot$, and we also compute the quantities
$mes(E)$ and $ind(E)$ using rules (\ref{first rule}) to (\ref{sixth rule}).
Let $\tilde{E}$ be the computed floating point approximation for |E|.
Note that $\tilde{E}$ is an integer. We have:\\
If $\abs{\tilde{E}} > ind(E)\cdot mes(E)\cdot eps$ then
$sign(\tilde{E}) = sign(E)$, i.e., the sign of $\tilde{E}$ is reliable. This
follows immediately from 
$\abs{\tilde{E} - E}\leq ind(E)\cdot mes(E)\cdot eps$.\\
If $\abs{\tilde{E}}\leq ind(E)\cdot mes(E)\cdot eps < 1$ then 
$\tilde{E} = E = 0$. This follows from
$\abs{\tilde{E} - E}\leq ind(E)\cdot mes(E)\cdot eps$ and the fact 
that |E| and $\tilde{E}$ are integers.\\
If $\abs{\tilde{E}} \leq ind(E)\cdot mes(E)\cdot eps$ and
$ind(E)\cdot mes(E)\cdot eps\geq 1$ then the signs of |E| and $\tilde{E}$
may be different.


@ Implementation.

The filter is realized by a data type (class) |floatf| in C++. |floatf|,
can be used in the same way as the built-in numerical type |double|.
The only exception is that the result of some tests may be unreliable.
In particular there is a function |Sign(floatf x)| that tries to compute the 
sign of $x$. Possible results are |-1|, |O|, |+1| and |NO_IDEA|. The last 
value indicates that the sign of $x$ could not be computed.

Each instance of type |floatf| has three data members: |num|, $mes$, and $ind$.
|num| is the floating-point approximation of the integer $x$ to be represented, 
$mes = mes(x)$ and $ind = ind(x)$ are the bounds defined in the previous 
section. We define initialization and operations $+$, $-$, $*$ following the 
rules given in Lemma~1. A minor complication arises from tha fact that
rules (8) and (10) are evaluated using single precision 
floating point arithmetic. We compensate the error in these calculations
by multiplying with the correction factor $1 + 8\cdot 2^{-23}$. 
The |Sign| operation is realized by a 
number of simple comparisons.
The details of the implementation are given below.



@(floatf.h@>=

#include <LEDA/basic.h>
#include <LEDA/integer.h>
#include <cmath>

const double eps0 = ldexp(1,-53);  // machine $\epsilon = 2^{-53}$
const float correction = 1 + ldexp(1,-20);  

const int NO_IDEA = 2;


class floatf {

  double num;
  double mes;
  float  ind;

  floatf(double d, double m, float i) { num = d; mes = m; ind = i; }

public:

floatf() { num = 0; mes = 0; ind = 0; }

floatf(integer i)
{ /* rules (\ref{first rule}) and (\ref{second rule}).
     Note that |ldexp(1,x)| is $2^x$ and that |Ilog(i)| is $\lfloor \log\abs{i}
     \rfloor$ for $i \not= 0$ and $-1$ for $i =0$. Thus |Ilog(i-1) + 1|$ = 
     \lceil \log i \rceil$ for $ i > 0$. 
   */
 
  if (i == 0) 
     { num = 0; mes = 0; ind = 0; }
  else
     { num = i.todouble(); 
       mes = (i > 0) ? ldexp(1,log(i)+1) : ldexp(1,log(-i)+1); 
       ind = (mes <= 53) ? 0 : 0.5;
      }
}

operator double() const { return num; } 

friend floatf operator+(const floatf& a, const floatf& b)
{ //rules (\ref{third rule}) and (\ref{fourth rule}) 
  return floatf(a.num + b.num, 2*((a.mes > b.mes) ? a.mes : b.mes),
                             correction* (a.ind + b.ind + 1)/2);
 }

friend floatf operator-(const floatf& a, const floatf& b)
{ //rules (\ref{third rule}) and (\ref{fourth rule})
  return floatf(a.num - b.num, 2*((a.mes > b.mes) ? a.mes : b.mes),
                            correction*  (a.ind + b.ind+ 1)/2);
 }

friend floatf operator*(const floatf& a, const floatf& b)
{ //tules (\ref{fifth rule}) and (\ref{sixth rule})
  return floatf(a.num * b.num, a.mes * b.mes, 
                 correction* (a.ind + b.ind + 0.5));
 }


friend int Sign(const floatf& f)
{ double eps = f.ind * f.mes * eps0;
  if (f.num >  eps) return +1;
  if (f.num < -eps) return -1;
  if (eps < 1) return 0;
  return NO_IDEA;
 }
 

};

@ \label{float-restrictions}
Note that the quantities $mes$ and $ind$ are computed using double- and 
single-precision floating point arithmetic. The implementation is 
therefore correct as long as $mes\leq 2^{1024}$ and $ind$ is a 
binary number with at most 24 bits.

%The procedure |sweep| takes a list of |rat_segment|s and computes the graph 
%induced by them. Recall that a |rat_segment| is specified by a pair of 
%|rat_point|s and that a |rat_point| is given by its homogeneous coordinates 
%$(X,Y,W)$ of type |integer|. The input must be such that there are integer 
%constants $k$ and $l$ with $5k + 3l <= 450$, $\mid X \mid, \mid Y\mid < 2^k$, 
%and $1 \le \mid W\mid \le 2^l$ for all segment endpoints.



@* Experiments and Efficiency.
\label{experiments}


We performed tests on three kinds of test data: 
difficult inputs, random inputs and  hand-crafted examples.

\begin{itemize}
\item
Difficult inputs:
Let $size$ be a random $k$-bit integer and
let $y = 2size/n$. We intersect $n$ segments where the $i$-th segment 
has endpoints $(size + rx1$, $2 \cdot size  -i \cdot y + ry1)$ and 
$(3size + rx2$, $2size + i \cdot y + ry2)$ where $rx1$, $rx2$, $ry1$, $ry2$ 
are random integers in $[-s,s]$ for some small integer $s$. 

\item 
Random inputs: 
We generated $n$ (between 1 and several hundred) segments
with random coordinates between $-size$ and $size$ for some parameter $size$. 
For $size$ we used large values to test the correctness and efficiency of the
floating point filter and the long integer arithmetic and small values to
test our claim that we can handle all degeneracies (a set of 100 segments whose
endpoints have integer coordinates between -3 and +3 is highly degenerate).


\item Hand-crafted examples: We handcrafted some examples that exhibited a 
lot of degeneracies. We also asked students to break the code and offered 
DM 100.- for the first counter-example.

\end{itemize}



Table \ref{difficult} gives the result for the difficult inputs with 
$s=3$, $k=10,15,20,\cdots,100$, and $n=100$. It lists the number 
of intersection and the running times
with and without the floating point filter. Furthermore it
gives  the percentage of the comparisons of points in the X-structure 
that were left undecided by the floating point filter (failure
rate) and  the percentage of tests where the floating point computation 
would have decided incorrectly (error rate).

\begin{table}
\begin{tabular}{rccccc}
$k$ & $|V|$ & without filter &  with filter & failures & errors  \\
\hline
 10 &  1323 &  0.78 &  0.37  &   0.05\% &  0.00\% \\
 15 &  1323 &  0.68 &  0.40  &   0.04\% &  0.00\% \\
 20 &  1314 &  0.70 &  0.38  &   0.12\% &  0.05\% \\
 25 &  1325 &  0.77 &  0.42  &   0.08\% &  0.05\% \\
 30 &  1318 &  0.75 &  0.45  &   1.48\% &  0.73\% \\
 35 &  1321 &  1.03 &  0.52  &   0.94\% &  0.39\% \\
 40 &  1312 &  1.27 &  0.55  &   2.17\% &  1.02\% \\
 45 &  1325 &  1.27 &  0.77  &  20.98\% &  2.19\% \\
 50 &  1323 &  1.45 &  1.23  &  73.33\% & 19.98\% \\
 55 &  1325 &  1.90 &  1.68  &  86.35\% & 44.74\% \\
 60 &  1325 &  1.33 &  1.52  &  83.52\% & 58.07\% \\
 65 &  1321 &  2.20 &  1.90  &  89.66\% & 65.21\% \\
 70 &  1323 &  2.07 &  1.87  &  86.88\% & 63.41\% \\
 75 &  1316 &  2.57 &  2.30  &  89.93\% & 67.89\% \\
 80 &  1323 &  2.62 &  2.08  &  89.90\% & 67.33\% \\
 85 &  1321 &  2.63 &  2.98  &  83.11\% & 62.74\% \\
 90 &  1313 &  2.78 &  2.48  &  90.96\% & 67.88\% \\
 95 &  1319 &  3.03 &  3.07  &  81.74\% & 62.33\% \\
100 &  1323 &  3.12 &  3.13  &  89.19\% & 67.05\% \\
\end{tabular}
\caption{The difficult example with 100 segments.}\label{difficult}
\end{table}

Comparing the $x$-coordinates of two intersection points is tantamount 
to testing the sign of a fifth degree homogeneous
polynomial in the coordinates of the segment endpoints. These coordinates
are of the form $size + rx$, $3size + rx$, $2size + i\cdot y + ry$, $2size -i
\cdot y + ry$.
Thus, if we write the polynomial in terms of the perturbations $rx$ and
$ry$ we essentially obtain a term of order $size^5$ independent of the
perturbations (this term is actually zero but has maximal size in
intermediate results) plus a term or order $size^4$ times a linear
function in the perturbations plus a term of order $size^3$ times a
quadratic function in the perturbations $\cdots$ . Since the input
coordinates are not equal to |size| but vary between $size$ and $3size$
the various terms are actually spread out over some orders of magnitude.

In the floating point filter analysis the error bound $\delta$ is about
$3size^5 \cdot 2^{-53}$. We conclude that the floating point filter
becomes worthless for $k$ about 50 and that we are starting to loose
the linear terms for $k$ about 40. For smaller $k$ the filter fails
only in those rare cases where the second order term must decide. This is
well reflected in Table \ref{difficult}. 


\begin{table}
\begin{tabular}{rccccc}
$k$ & $|V|$ & without filter & with filter & failures & errors \\ 
\hline
  10& 1298  & 0.57  & 0.35   &  0.00\%  &  0.00\% \\
  30& 1373  & 0.87  & 0.53   &  0.00\%  &  0.00\% \\
  50& 1526  & 1.78  & 0.78   &  0.00\%  &  0.00\% \\
  70& 1298  & 2.17  & 0.70   &  0.00\%  &  0.00\% \\
  90& 1417  & 2.05  & 0.83   &  0.00\%  &  0.00\% \\
 110& 1144  & 2.08  & 0.77   &  0.00\%  &  0.00\% \\
 130& 1222  & 2.82  & 1.25   &  0.00\%  &  0.00\% \\
 150& 1434  & 4.72  & 1.37   &  0.00\%  &  0.00\% \\
 170& 1189  & 3.65  & 1.25   &  0.00\%  &  0.00\% \\
 190& 1463  & 6.20  & 1.63   &  0.00\%  &  0.00\% \\
 210& 1401  & 7.63  & 8.12   & 54.66\%  & 34.06\% \\
 230& 1270  &10.58  & 8.48   & 51.53\%  & 29.26\% \\
 250& 1608  &14.05  & 9.72   & 56.75\%  & 33.74\% \\
 270& 1226  & 9.03  & 6.68   & 46.28\%  & 29.02\% \\
 290& 1576  &14.55  &12.27   & 49.03\%  & 28.17\% \\
 310& 1260  &15.12  &11.10   & 46.25\%  & 28.21\% \\
 330& 1280  &12.93  &11.35   & 48.15\%  & 29.52\% \\
 350& 1250  &13.25  &16.80   & 87.45\%  & 42.02\% \\
 370& 1343  &24.97  &21.37   & 86.42\%  & 46.91\% \\
 390& 1420  &20.55  &20.50   & 87.96\%  & 46.30\% \\
 400& 1391  &22.38  &23.30   & 89.26\%  & 47.10\% \\
\end{tabular}
\caption{100 random segments, coordinates are random $k$-bit integers.}
\label{random}
\end{table}

Table \ref{random} gives the results for 100 segments whose endpoints have 
random $k$ bit coordinates. The experiments inidicate that the floating point filter reduces the
running time of the program by a factor of 2. 
 
More experiments with different floating
point filters are described in \cite{IFIP94}.

@* Conclusion.

We have given an implementation of the Bentley-Ottmann
plane sweep algorithm for line segment intersection. The implementation is
complete and reliable in the sense that it will work for all input instances.
It is asympotically more efficient than previous algorithms for the same task;
its running time  depends on the number of vertices in the intersection
graph and not on the number of pairs of intersecting segments. It also
achieves a low constant factor in its running time by means of a floating
point filter.

The use of LEDA makes the implementation  of the combinatorial part of the
algorithm quite elegant. In the geometric part we have not achieved the same
level of elegance yet. We are currently exploring whether the
floating point filter can be completely hidden from the user as suggested in
\cite{Yap-Dube:Exact-Computation}.




\newpage

\begin{thebibliography}{10}


\bibitem{Bentley-Ottmann} 
J.L.~Bentley and T.A.~Ottmann. 
\newblock Algorithms for reporting and counting geometric intersections.
\newblock {\em IEEE Trans. Comput.}, C-28:643--647, 1979.



\bibitem{Burnikel-et-al}
C.~Burnikel, K.~Mehlhorn, and S.~Schirra.
\newblock On degeneracy in geometric computations.
\newblock In {\em Proc.\ of the 5th ACM-SIAM Symp.\ on Discrete Algorithms},
  pp. 16--23, 1994.


\bibitem{Cormen-Leiserson-Rivest}
T.H. Cormen and C.E. Leiserson and R.L. Rivest.
\newblock Introduction to Algorithms.
\newblock MIT Press/McGraw-Hill Book Company, 1990.



\bibitem{Fortune-vanWyk}
S.~Fortune and C.~van Wyk.
\newblock Efficient exact arithmetic for computational geometry.
\newblock In {\em Proc.\ of the 9th ACM Symp.\ on Computational Geometry},
  pp. 163--172, 1993.


\bibitem{Mehlhorn-book}
K.~Mehlhorn.
\newblock Data Structures and Efficient Algorithms.
\newblock Springer Publishing Company, 1984.

\bibitem{Myers}
E.~Myers.
\newblock{An $O(E \log E + I)$ expected time algorithm for the planar
segment intersection problem.}
\newblock {\em SIAM J. Comput.}, pp. 625 --636, 1985.

\bibitem{LEDA-Paper}
K.~Mehlhorn and S.~N\"aher.
\newblock {LEDA}: A library of efficient data types and algorithms.
\newblock In {\em LNCS}, 379:88--106, 1989.

\bibitem{IFIP94}
K.~Mehlhorn and S.~N\"aher.
\newblock The Implementation of Geometric Algorithms.
\newblock 13th World Computer Congress IFIP 94, Elsevier Science B.V., 
Vol. 1, pp. 223--231, 1994.

\bibitem{LEDA-Manual}
S.~N\"aher.
\newblock LEDA Manual.
\newblock Technical Report No. MPI--I--93--109.
\newblock Max-Planck-Institut f\"ur Informatik, 1993.

\bibitem{Preparata-Shamos}
F. Preparata and M.I. Shamos.
\newblock Computational Geometry: An Introduction.
\newblock Springer Publishing Company, 1985

\bibitem{Yap-Dube:Exact-Computation}
Ch. Yap and Th. Dub\'{e}.
\newblock The exact computation paradigm.
\newblock In {\em Computing in Euclidian Geometry}, World Scientific
Press, 1994. (To appear, 2nd edition).


\end{thebibliography}


\end{document}


