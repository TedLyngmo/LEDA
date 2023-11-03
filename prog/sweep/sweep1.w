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
is based on the basic geometric types |rat_point| and |rat_segment| of LEDA.
These types represent two-dimensional points and segments with rational
coordinates using exact arithmetic for the realization of all geometric 
primitives. The overhead of the exact arithmetic is reduced by using floating 
point filters (cf. \cite{Fortune-vanWyk, LEDA-Geometry}).

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
More precisely, we define a function
\begin{center}
  |sweep_segments(const list<rat_segment>& L,GRAPH<rat_point,rat_segment>& G)|
\end{center}
that takes a list $L$ of |rat_segment|s (segments with rational coordinates)
and computes the graph $G$ induced by it. For each vertex $v$ of $G$ it also 
computes its position in the plane, a |rat_point| (a point with rational 
coordinates) and for each edge $e$ of $G$ it computes one of the |rat_segment|s
containing it. The algorithm is based on the LEDA platform for combinatorial 
and geometric computing \cite{LEDA-Paper,LEDA-Manual}. 

In LEDA a |rat_segment| is specified by its two endpoints (of type |rat_point|)
and a |rat_point| is specified by its homogeneous coordinates $(X,Y,W)$ of 
type |integer|. The type |integer| is the LEDA type of arbitrary precision 
integers.  Types |rat_point| and |rat_segment| 
-->
--> schreib was ueber rat_point, rat_segment, etc
--> orientation, ...
-->


We want to stress that the implementation makes no assumptions
about the input, in particular, segments may have length zero, may be
vertical or may overlap, several segments may intersect in the same point, 
endpoints of segments may lie in the interior of other segments, \dots .

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
Evaluate all geometric tests exactly. This is achieved by using the LEDA
basic objects |rat_point| and |rat_segment|. In the implementation
of these types, LEDA uses arbitrary precision integer arithmetic for all 
geometric computations. So all tests are computed exactly and we do not have 
to worry about numerical precision. Of course, there is an overhead of 
arbitrary precision integer arithmetic. In order to keep the overhead small
LEDA uses a floating point filter 
following the suggestion of Fortune and van Wyk \cite{Fortune-vanWyk}, i.e., 
all tests are first performed using floating point arithmetic and only if the 
result of the floating point computation is inconclusive the costly exact
computation is performed (cf. \cite{LEDA-Geometry} for details). 
\end{itemize}

This paper is structured as follows. In section \ref{Algorithm} we describe the
(generalized) plane-sweep algorithm. Section \ref{Implementation}
and \ref{geometric primitives} give the details of the implementation: the 
former section describes the combinatorial part and the latter section describes
the geometric primitives. Section \ref{experiments} contains some experimental 
results.


@* The Algorithm. 
\label{Algorithm}

In the sweep-line paradigm a vertical line is moved from left to right across 
the plane and the output (here the graph $G(S)$) is constructed incrementally as
it evolves behind the sweep line. One maintains two data structures to keep the
construction going: The so-called {\em Y-structure} contains the intersection
of the sweep line with the scene (here the set $S$ of line segments) and the
so-called {\em X-structure} contains the events where the sweep has to be
stopped in order to add to the output or to update the $X$- or $Y$-structure.
In the line segment intersection problem an event occurs when the sweep line
hits an endpoint of some segment or an intersection point. When an event
occurs, some nodes and edges are added to the graph $G(S)$, the $Y$-structure
is updated, and maybe some more events are generated. When the input is in
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
containing it. For each segment $s$ contained in the $Y$-structure we store 
the rightmost vertex of $G$ lying on $s$.


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
  Its right endpoint is $v$ and its other endpoint is the vertex stored 
  with the segment $s$.
\item
  We delete all segments ending in $p$ from the $Y$-structure.
\item
  We reverse the order of the subsequence in the $Y$-structure. This amounts to
  moving the sweep line across the point $p$.
\item
  We add all segments starting in $p$ to the $Y$-structure and then associate
  the node $v$ with all segments in the $Y$-structure containing $v$.

--> ACHTUNG HIER HAT SICH WAS GEAENDERT
-->
--> If a
--> newly inserted segment is collinear to an already existing segment
--> we make sure to only keep the segment extending further to the right.
-->
-->

\item
  We update the events associated with the items of the $Y$-structure. We
  remove the events associated with the predecessor of the subsequence and
  the last item of the subsequence and we test whether segments becoming
  neighbors in the $Y$-structure intersect and generate new events if necessary.

\end{enumerate}

This completes the description of how to process the event $<p,...>$. The
invariants now hold for $p\_sweep = p$ and hence also for
$p\_sweep = (p'.x,p'.y-2\epsilon)$ where $<p',...>$ is the new first element of
the $X$-structure.


@* The Implementation. 
\label{Implementation}

The implementation follows the algorithm closely. 
We use sorted sequences, graphs, points and segments with rational
coordinates, and integers of arbitrary precision from LEDA.

-->
--> maps
--> p_queues
-->

We have to include the corresponding header files.

@<include statements@> =
#include <LEDA/sortseq.h>
#include <LEDA/graph.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/integer.h>
#include <LEDA/p_queue.h>
#include <LEDA/map.h>
#include <math.h>

@
Let us briefly explain these types; for a detailed discussion we refer
the reader to the LEDA manual \cite{LEDA-Manual}. |integer| is the type of 
arbitrary precision integers.
The types |rat_point| and |rat_segment| realize points and segments in the 
plane with rational coordinates. A |rat_point| is specified by its homogeneous
coordinates of type |integer|. If |p| is a |rat_point| then |p.X()|, |p.Y()|, 
and |p.W()| return its homogeneous coordinates. If |x|, |y|, and |w| are of
type |integer| with $w\neq 0$ then |rat_point(x,y)| and |rat_point(x,y,w)| 
create the |rat_point| with homogeneous coordinates |(x,y,1)| and |(x,y,w)| 
respectively. A |rat_segment| is specified by its two endpoints; so if 
|p| and |q| are |rat_point|s then |rat_segment(p,q)| is the directed
segment with startpoint |p| and endpoint |q|. If |s| is a |rat_segment| then
|s.start()| and |s.end()| return the start- and endpoint of |s| respectively.


%---> orientation
%---> left turn
%---> right turn
%---> compare
%---> intersection


@s K int
@s I int

For any types |K| and |I| the type |sortseq<K,I>| is the type of sorted
sequences of pairs in $K\times I$. The type |K| must be linearly ordered 
by a function |compare|, i.e., the function 
|int compare(const K&, const K&)| must be defined for the type |K|
and the relation $<$ on |K| defined by $k_1 < k_2$ iff 
$compare(k_1,k_2) < 0$ is a linear order on |K|. Any object of type
|sortseq<K,I>| is a sequence of items (type |seq_item|) each containing
a pair $K\times I$. We use |<k,i>| to denote an item containing the pair
$(k,i)$ and call |k| the key and |i| the information of the item. A sorted 
sequence $<k_1,i_1>$,$<k_2,i_2>$,\dots ,$<k_m,i_m>$ has the additional
property that the keys of the item form an increasing sequence, i.e.,
$k_l < k_{l+1}$ for $1\leq l<m$.

In our implementation the |X|-structure has type |sortseq<rat_point,seq_item>|
the |Y|-structure has type |sortseq<rat_segment,seq_item>|. We use LEDA's
default linear order for |rat_point| which is the lexicographic ordering
of the cartesian coordinates. LEDA has already defined a |compare| function
realizing this linear order. If |p| and |q| are points with cartesian 
coordinates |px|, |py|, |qx|, and |qy| respectively then
\[ compare(p,q) = \left \{
                  \begin{array}{rl}
		  -1 & \mbox{ if }px<qx\mbox{ or }px=qx\mbox{ and }py<qy\\
		  0  & \mbox{ if }px=qx\mbox{ and }py=qy\\
		  +1 & \mbox{ otherwise, }
		  \end{array}
		  \right . 
\]

We will later define a linear order for |rat_segment| by writing
an appropriate compare function.
If |s1| and |s2| are segments intersecting the sweep line |L| then
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

Let |S| be a sorted sequence of type |sortseq<K,I>| and let |k| 
and |i| be of type |K| and |I| respectively. The operation 
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


%---> map
%---> hash



@* The Basic Program Structure.

Our program has the following structure.
@c
@<include statements@>@;
@<geometric primitives@>@;

void sweep(list<rat_segment>& S, GRAPH<rat_point,rat_segment>& G)
{
 @<local declarations @>@;
 @<initialization @>@;
 @<sweep @>@;
}




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

Finally, we store with each segment |s| the last created node
of the output graph |last_node| lying on |s|.

@<local declarations @> =

sortseq<rat_point,seq_item> X_structure;

sortseq<rat_segment,seq_item> Y_structure;


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
  p_sweep = X_structure.key(event);
  node v = G.new_node(p_sweep); 

  @<handle passing and ending segments @>@;

  @<insert starting segments @>@;

  @<compute new intersections @>@;

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
 


Taking |sit| as an entry point into the Y-structure we determine all segments
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



We can now add edges to the graph |G|. For each segment in the subsequence 
between |sit_first| and |sit_last| inclusive we construct an edge. Let |sit| be 
any such item and let |s| be the segment associated with |sit|. We construct an 
edge connecting |s -> last_node| and |v| and label it with the segment |s|.
We also either delete the item from the Y-structure (if the segment ends at
|p|) or change its information to |nil| (if the segment does not end at |p|) to
reflect the fact that no intersection event is now associated with the 
segment. In the former case we free the storage reserved for the segment.

At the end we have to update variables |sit_first| and |sit_last|
since the corresponding items may have been deleted.



All segments remaining in the subsequence pass through node |v| and moving 
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


@<handle passing and ending segments @> =





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


@<insert starting segments @> =




@ After having inserted all segments starting at |p|, we test whether
|sit_succ| (|sit_pred|) intersects its predecessor (successor) in the 
|Y|-structure.

@<compute new intersections @> =



@* Geometric Primitives.
\label{geometric primitives}

It remains to define the geometric primitives used in the implementation. We
need three:

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
  a function |compute_intersection| that decides whether two segments intersect
  and if so whether the intersection is to the right of the sweep line. If
  both tests are positive it also makes the required changes to the $X$- and
  $Y$-structure.

\end{itemize}



@ Next we write the compare function for segments.


@<geometric primitives @> =

// geometric primitives

static POINT p_sweep;
static int   N;

inline int pair(const SEGMENT& p, const SEGMENT& q)
{ return  ID_Number(p) * N + ID_Number(q); }

inline int compare(const SEGMENT& s1, const SEGMENT& s2)
{ 
  // Precondition: |p_sweep| is identical to the left endpoint of
  // either |s1| or |s2|. This is true since comparisons are only 
  // executed when inserting or looking up new segments.

  if ( identical(p_sweep,s1.start()) )
  { int s = orientation(s2,p_sweep);
    return (s != 0) ? -s : cmp_slopes(s1,s2);
   }

  if ( identical(p_sweep,s2.start()) )
  { int s = orientation(s1,p_sweep);
    return (s != 0) ? s : cmp_slopes(s1,s2);
   }

  error_handler(1,"internal error in sweep");
  return 0; // never reached
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

static void compute_intersection(sortseq<POINT,seq_item>& X_structure,
                                 sortseq<SEGMENT,seq_item>& Y_structure,
                                 hash<int,seq_item>& inter_dic,
                                 seq_item sit0)
{
  // Given an item |sit0| in the Y-structure compute the point of 
  // intersection with its successor and (if existing) insert it into 
  // the event queue and do all necessary updates.

  seq_item sit1 = Y_structure.succ(sit0);
  SEGMENT  s0   = Y_structure.key(sit0);
  SEGMENT  s1   = Y_structure.key(sit1);


  // |s1| is the successor of |s0| in the Y-structure, hence,
  // |s0| and |s1| intersect right or above of the sweep line
  // iff |(s0.start(),s0.end(),s1.end()| is not a left turn and 
  // |(s1.start(),s1.end(),s0.end()| is not a right turn.
  // In this case we intersect the underlying lines
  
  if (orientation(s0,s1.end()) >= 0 && orientation(s1,s0.end()) <=0 )
  { hash_item it = inter_dic.lookup(pair(s0,s1));
    if (it != nil)
       { Y_structure.change_inf(sit0,inter_dic.inf(it));
         inter_dic.del_item(it);
        }
    else
       { POINT    q;
         s0.intersection_of_lines(s1,q);
         Y_structure.change_inf(sit0, X_structure.insert(q,sit0));
        }
  }
}




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
point filter. The use of LEDA makes the implementation of the algorithm short 
and elegant.




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


