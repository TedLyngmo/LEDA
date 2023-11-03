\documentstyle[11pt,comments,a4]{cweb}

\voffset=-0.5cm
\textwidth		15cm
\textheight		23cm
\oddsidemargin          0.3cm
\evensidemargin         0.3cm
\topmargin	       -0.5cm
\sloppy

\parindent 0cm
\parskip 8pt plus 1pt minus 1pt

\def\abs {|}

%\input figures/transfig

\begin{document}


\renewcommand{\thefootnote}{\fnsymbol{footnote}}

\title{A Robust and Efficient Implementation of a Sweep Line Algorithm
       for the Straight Line Segment Intersection Problem}

\author{Kurt Mehlhorn\\
       {\footnotesize  Max-Planck-Institut f\"ur Informatik,}\\
       {\footnotesize 66123 Saarbr\"ucken, Germany}\\[1cm]
       Stefan N\"aher\\
       {\footnotesize Fachbereich Mathematik und Informatik}\\
       {\footnotesize Martin-Luther-Universit\"at Halle-Wittenberg}\\
       {\footnotesize 06120 Halle (Saale), Germany}}
 
        \date{} 
        \maketitle



@* Abstract.
We describe a robust and efficient implementation of the Bentley-Ottmann 
sweep line algorithm \cite{Bentley-Ottmann} based on the LEDA platform 
of combinatorial and geometric computing \cite{LEDA-Manual,LEDA-CACM}. The program 
computes the planar graph $G$ induced by a set $S$ of straight line segments 
in the plane. The nodes of |G| are all endpoints and all proper intersection 
points of segments in |S|. The edges of |G| are the maximal relatively open 
subsegments of segments in |S| that contain no node of |G|. All edges are
directed from left to right or upwards.
The algorithm runs in time $O((n+s)\log n)$ where $n$ is the number of 
segments and $s$ is the number of vertices of the graph $G$. The implementation
makes use of the basic geometric types |rat_point| and |rat_segment| of LEDA.
These types realize two-dimensional points and segments with rational
coordinates; they use exact arithmetic for the realization of all geometric 
primitives. The overhead of exact arithmetic is reduced by means of a floating 
point filter (cf. \cite{Fortune-vanWyk,sweep-94,LEDA-Geometry}).
A first version of this program appeared already in \cite{sweep-94}. The
current version is considerably more elegant. 

@i ../leda_types.w

%\tableofcontents

@* Introduction.

Let $S$ be a set of $n$ straight-line segments in the plane and let $G(S)$
be the graph induced by $S$. The vertices of $G(S)$ are all endpoints of
segments in $S$ and all proper intersection points between segments in $S$.
The edges of $G$ are the maximal relatively open and connected subsets of 
segments in $S$ that contain no vertex of $G(S)$. Figure \ref{example} shows 
an example. Note that the graph $G(S)$ contains no parallel edges even if $S$ 
contains segments that overlap.

\begin{figure}
\begin{center}
%\input{figures/introfig.tex}
\begin{picture}(0,0)%
\special{psfile=figures/introfig.ps}%
\end{picture}%
\setlength{\unitlength}{0.012500in}%
\begin{picture}(380,220)(20,605)
\end{picture}

\caption{\label{example}A set $S$ of segments and the induced planar graph.}
\end{center}
\end{figure}

Bentley and Ottmann \cite{Bentley-Ottmann} have shown how to compute $G(S)$
in time $O((n+k)\log n)$ where $k$ is the number of pairs of intersecting
segments in $S$. The algorithm is based on the plane-sweep paradigm. We
refer the reader to \cite[section VIII.4]{Mehlhorn-book},
\cite[section 7.2.3]{Preparata-Shamos}, and 
\cite[section 35.2]{Cormen-Leiserson-Rivest}
for a discussion of the plane sweep paradigm.

In this paper we describe an implementation of the Bentley-Ottmann algorithm. 
More precisely, we define a function
\begin{center}
|sweep_segments(list<rat_segment> L, GRAPH<rat_point,rat_segment>& G)|
\end{center}
that takes a list $L$ of |rat_segment|s, i.e., segments whose vertices have rational coordinates,
and computes the graph $G$ induced by it. For each vertex $v$ of $G$ it also 
computes its position in the plane, a |rat_point| ($=$ a point with rational 
coordinates), and for each edge $e$ of $G$ it computes one of the |rat_segment|s
containing it. The algorithm is based on the LEDA platform for combinatorial 
and geometric computing \cite{LEDA-CACM,LEDA-Manual}. 

We want to stress that the implementation makes no assumptions
about the input, in particular, segments may have length zero, may be
vertical or may overlap, several segments may intersect in the same point, 
endpoints of segments may lie in the interior of other segments, \dots .

We have achieved this generality by following two principles.
\begin{itemize}
\item
We treat degeneracies as first class citizens and not as an afterthought
\cite{Burnikel-et-al}. In particular, we reformulated the plane-sweep
algorithm so that it can handle all geometric situations. The details will
be given in section \ref{Algorithm}. The reformulation makes the 
description of the algorithm shorter since we do not distinguish between
three kinds of events but have only one kind of event and it also makes the 
algorithm faster. The algorithm now runs in time $O((n+s)\log n)$ where
$s$ is the number of vertices of $G$. Note that $s\leq n+k$ and that $k$
can be as large as $s^2$. The only previous algorithm that could handle all
degeneracies is due to Myers \cite{Myers}. Its expected running time for
random segments is $O(n \log n + k)$ and its worst case running time is
$O((n + k) \log n)$.

\item
We evaluate all geometric tests exactly. This is achieved by using the LEDA
types |rat_point| and |rat_segment|. 
These types use arbitrary precision integer arithmetic for all 
geometric computations. So all tests are computed exactly. 
Of course, there is an overhead of 
arbitrary precision integer arithmetic. In order to keep the overhead small
LEDA uses a floating point filter 
following the suggestion of Fortune and van Wyk \cite{Fortune-vanWyk}, i.e., 
all tests are first performed using floating point arithmetic and only if the 
result of the floating point computation is inconclusive the costly exact
computation is performed (cf. \cite{LEDA-Geometry} for details). 
\end{itemize}

We presented a first version of this program already in \cite{sweep-94}.
The current version is more elegant and also slightly more efficient.
The increase in elegance is mainly due to two facts. Firstly, we
incorporated the floating point filter into the types |rat_point| and
|rat_segment|. It is therefore no longer visible in this program. 
Secondly, LEDA now offers the type map that gives an easy way to 
associate additional attributes with segments (and other objects). In 
the original version of the program we had to define a structure
|my_segment| that included fields for these additional attributes.

This paper is structured as follows. In section \ref{Algorithm} we describe the
generalized plane-sweep algorithm. Sections \ref{Implementation}
to \ref{compute intersection} give the details of the implementation. 
Section \ref{experiments} contains some experimental 
results.


@* The Algorithm.\label{Algorithm}

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
$x$-coordinate, no vertical lines, no overlapping segments, \dots) then at most one
event can occur for each position of the sweep line and there are three clearly
distiguishable types of events (left endpoint, right endpoint, intersection)
with easily describable associated actions, cf. 
\cite[section VII.8]{Mehlhorn-book}.
We want to place no restrictions on the input and therefore need to proceed
slightly differently. We now describe the required changes.

We define the sweep line by a point $p\_sweep = (x\_sweep, y\_sweep)$. Let
$\epsilon$ be a positive infinitesimal (readers not familiar with
infinitesimals may think of $\epsilon$ as an arbitrarily small positive real).
Consider the directed line $L$ consisting of a vertical upward ray ending in
point $(x\_sweep + \epsilon^2, y\_sweep + \epsilon)$ followed by a horizontal
segment ending in $(x\_sweep - \epsilon^2, y\_sweep + \epsilon)$ followed by a
vertical upward ray. We call $L$ the {\em sweep line}. Note that no endpoint
of any segment lies on $L$, that no two segments of $S$ intersect $L$ in the
same point except if the segments overlap, and that no non-vertical segment
of $S$ intersects the horizontal part of $L$. All three properties follow from
the fact that $\epsilon$ is arbitrarily small but positive. Figure \ref{the
sweep line} illustrates the definition of $L$ and the main data structures used in
the algorithm: The $Y$-structure, the $X$-structure, and the graph |G|.

\begin{figure}
\begin{center}
%\input{figures/sw2fig.tex}
\begin{picture}(0,0)%
\special{psfile=figures/sw2fig.ps}%
\end{picture}%
\setlength{\unitlength}{0.012500in}%
\begin{picture}(360,554)(100,280)
\end{picture}

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
$L$ appear on the directed line $L$, however, for segments intersecting $L$ in the same
point (these segments necessarily have the same underlying line) only one of the
segments is stored in the $Y$-structure. 

The $X$-structure contains all endpoints that are to the right of the
sweep line and also some intersection points between segments in the
$Y$-structure. More precisely, for each pair of segments adjacent in the
$Y$-structure their intersection point is contained in the $X$-structure
(if it exists and is to the right of the sweep line). The $X$-structure
may contain other intersection points. The graph $G$ contains the part of 
$G(S)$ that is to the left of the sweep line. 

Initially, the $Y$-structure and the graph $G$ are empty and the
$X$-structure contains all endpoints of all input segments. The events
in the $X$-structure are then processed in left to right order. Events
with the same $x$-coordinate are processed in bottom to top order.

Assume that we need to process an event at point $p$ and that the
$X$-structure and $Y$-structure reflect the situation for 
$p\_sweep = (p.x,p.y-2\epsilon)$.
Note that this is true initially, i.e.,
before the first event is removed from the $X$-structure. We now show how to
establish the invariants for $p\_sweep = p$. We proceed in seven steps.
\begin{enumerate}
\item
  We add a node $v$ at position $p$ to our graph $G$.
\item
  We determine all segments in the $Y$-structure containing the point $p$.
  These segments form a possibly empty subsequence of the $Y$-structure.
\item
  For each segment in the subsequence we add an edge to the graph $G$.
\item
  We delete all segments ending in $p$ from the $Y$-structure.
\item
  We reverse the order of the subsequence in the $Y$-structure. This amounts to
  moving the sweep line across the point $p$.
\item
  We insert all segments starting in $p$ into the $Y$-structure. 
  If a segment $s$ to be inserted is collinear to an already existing segment 
  $s0$ and extends further to the right then it is replaced by a new segment
  starting at the right endpoint of $s0$ and ending at the right endpoint of 
  $s$.

\item
  We generate events for  the segments in the $Y$-structure that become
  adjacent by the actions above and insert them into the $X$-structure.
\end{enumerate}

This completes the description of how to process the event $p$. The
invariants now hold for $p\_sweep = p$ and hence also for
$p\_sweep = (p'.x,p'.y-2\epsilon)$ where $p'$ is the new first element of
the $X$-structure.


@* The Implementation. \label{Implementation}

The implementation follows the algorithm closely. Our program has the 
following structure.

@c

@<include statements@>@;

@<geometric primitives@>@;

void sweep(const list<rat_segment>& S, GRAPH<rat_point,rat_segment>& G)
{
 @<local declarations @>@;
 @<initialization @>@;
 @<sweep @>@;
}


@
We use sorted sequences, maps, priority queues, graphs, points and segments 
with rational coordinates, and integers of arbitrary precision from LEDA
and have to include the corresponding header files.
\vspace{5mm}

@<include statements@> =
#include <LEDA/sortseq.h>
#include <LEDA/p_queue.h>
#include <LEDA/map.h>
#include <LEDA/graph.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/integer.h>
#include <cmath>

@
Let us briefly explain these types; for a detailed discussion we refer
the reader to the LEDA manual \cite{LEDA-Manual}. We also discuss the
use of these types in our program.

The type |integer| realizes arbitrary precision integers.

The types |rat_point| and |rat_segment| realize points and segments in the 
plane with rational coordinates. A |rat_point| is specified by its homogeneous
coordinates of type |integer|. If |p| is a |rat_point| then |p.X()|, |p.Y()|, 
and |p.W()| return its homogeneous coordinates. If |x|, |y|, and |w| are of
type |integer| with $w\neq 0$ then |rat_point(x,y)| and |rat_point(x,y,w)| 
create the |rat_point| with homogeneous coordinates |(x,y,1)| and |(x,y,w)| 
respectively. Two points are equal (operator |==|) if they agree in
their cartesian coordinates. A |rat_segment| is specified by its two endpoints; so if 
|p| and |q| are |rat_point|s then |rat_segment(p,q)| is the directed
segment with source |p| and target |q|. If |s| is a |rat_segment| then
|s.source()| and |s.target()| return the source and target of |s| respectively.
LEDA provides a set of basic geometric primitives on |rat_points| and
|rat_segments| (see \cite{LEDA-Manual} and \cite{LEDA-Geometry} for details). 
In the sweep program we use the following primitives:

\begin{itemize}
\item |int compare(rat_point p, rat_point q)|\\
defines the default linear order on |rat_point|s: it is the lexicographic
ordering of the cartesian coordinates of the points. 

\item |int orientation(rat_point p, rat_point q, rat_point r)|\\
computes the orientation of points |p|, |q|, and |r| in the plane
i.e., the sign of the determinant

    \[ \left\abs \begin{array}{ccc} p_x & p_y & p_w\\
                                 q_x & q_y & q_w\\
                                 r_x & r_y & r_w
              \end{array} \right\abs \] 

                           
\item |int orientation(rat_segment s, rat_point p)|\\
computes |orientation(s.source(),s.target(),p)|.

\item |int cmp_slopes(rat_segment s1, rat_segment s2)|\\
compares the slopes of |s1| and |s2|. If one of the segments is degenerate,
i.e., has length zero, the result is zero. Otherwise, the result is the sign
of |slope(s1) - slope(s2)|.

\item |bool intersection_of_lines(rat_segment s1, rat_segment s2, rat_point& p)|\\
returns |false| if segments |s1| and |s2| are parallel or one
of them is degenerate. Otherwise, it computes the point of intersection of the 
two straight lines supporting the segments, assigns it to the third 
parameter |q|, and returns |true|.

\end{itemize}


Our program maintains its own set of segments which we call
{\em internal segments} or simply segments. They are directed from
left to right; vertical segments are directed upwards. We have a
map  |<rat_segment, rat_segment>| original that associates an input
segment with every internal segment. More precisely, we have:

\begin{itemize}
\item For each internal segment $s$ the segment original $[s]$ is an
input segment; $s$ is contained in original $[s]$ and the endpoints 
of $s$ are endpoints of some input segments.

\item For each input segment of non-zero length there is an internal
segment with the same endpoints.
\end{itemize}

Note that the internal segments together with the zero length input
segments define exactly the same graph as the input segments. This
follows since every non-zero length input segment has an equivalent
internal segment and since the additional internal segments are
contained in input segments and introduce no new endpoints. We use
the additional internal segments to handle collinear segments in an
elegant way.

The type |sortseq<K,I>| realizes sorted sequences of pairs in $|K| \times |I|$;  
|K| is called the key type and |I| is called the information type of the
sequence. |K| must be linearly ordered, i.e., the function 
|int compare(const K&, const K&)| must be defined for the type |K|
and the relation $<$ on |K| defined by $k_1 < k_2$ iff 
$compare(k_1,k_2) < 0$ is a linear order on |K|. An object of type
|sortseq<K,I>| is a sequence of items (type |seq_item|) each containing
a pair in $K\times I$. We use |<k,i>| to denote an item containing the pair
$(k,i)$ and call |k| the key and |i| the information of the item. The keys in a sorted 
sequence $<k_1,i_1>$,$<k_2,i_2>$,\dots ,$<k_m,i_m>$ 
form an increasing sequence, i.e.,
$k_l < k_{l+1}$ for $1\leq l<m$.

Let |S| be a sorted sequence of type |sortseq<K,I>| and let |k| 
and |i| be of type |K| and |I| respectively. The operation 
|S.lookup(k)| returns the item $it = <k,.>$ in |S| with key |k| if there
is such an item and returns |nil| otherwise. If |S.lookup(k)==nil|
then |S.insert(k,i)| adds a new item |<k,i>| to |S| and returns
this item. If |S.lookup(k)==it| then |S.insert(k,i)| changes the
information in the item |it| to |i|. If $it=<k,i>$ is an item of |S|
then |S.key(it)| and |S.inf(it)| return |k| and |i| respectively and
|S.succ(it)| and |S.pred(it)| return the successor and predecessor item
of |it| respectively; the latter operations return |nil| if these
items do not exist. The operation |S.min()| returns the first item
of |S|, |S.empty()| returns true if |S| is empty and false otherwise. 
Finally, if |it1| and |it2| are items of |S| with |it1| before |it2|
then |S.reverse_items(it1,it2)| reverses the subsequence of |S| 
starting at item |it1| and ending at item |it2|. The requirement for
this operation is that the sequence |S| is sorted with respect to
the current compare function after the operation.

In our implementation the |X|-structure has type |sortseq<rat_point,seq_item>|
and the |Y|-structure has type |sortseq<rat_segment,seq_item>|. The
|Y|-structure has essentially one item for each segment intersecting
the sweep line, however, for collinear segments only one is stored in
the |Y|-structure. The information field in the |Y|-structure is used
for  cross-links with the |X|-structure. We will come back to this
below. The |Y|-structure is ordered according to the intersections
of the segments with the directed sweep line |L|. In section
\ref{linear order on segments} we will define a compare function 
for segments that realizes this
linear order. 

It is important to observe that the compare function for segments
changes as the sweep progresses. What does it mean then that the keys of the 
items in a sorted sequence form an increasing sequence? LEDA requires 
that whenever a lookup or insert operation is applied to a sorted
sequence the sequence must be sorted with respect to the current 
compare function. The other operations may be applied even if the 
sequence is not sorted.

In the example of Figure \ref{the sweep line} the sweep line intersects the
segments $s_8$, $s_1$, $s_2$, $s_9$, $s_4,$ and $s_3$. The segments $s_8$ and
$s_1$ intersect $L$ in the same point and hence only one of them is present in 
the |Y|-structure, say $s_1$. The $Y$-structure
therefore consists of five items, one each for segments $s_1$,$s_2$, $s_9$,
$s_4,$ and $s_3$.

The |X|-structure contains an item for each endpoint of an input segment
that is to the right of the sweep line and for each intersection point
between segments that are adjacent in the |Y|-structure and that intersect 
to the right of the sweep line. It may also contain intersection
points between segments that are not adjacent in the
|Y|-structure.\footnote{Our |X|-structure may contain
intersection points between segments that
are non-adjacent in the |Y|-structure and that are not endpoint of
any segment. Following Brown \cite{Brown} one may remove these events
from the |X|-structure. This guarantees that the size
of the |X|-structure stays linear but complicates the code.
Since the size of the |X|-structure
is always bounded by the size of the output graph we do not follow
Brown's suggestions.} The points in the |X|-structure are ordered 
according to the lexicographic ordering of their cartesian coordinates.
As mentioned above this is the default order on |rat_point|s.

In the example of Figure \ref{the sweep line} the |X|-structures contains
items for the endpoints $b,c,d,e,g,h,i$ and for intersections $a$ and $f$. 
Here, $a$ ($f$) is the intersection of adjacent segments $s_4$ and $s_3$ 
($s_1$ and $s_2$).

The informations associated with the items of both structures serve
as cross-links between the two structures: the information associated
with an item in either structure is either nil or an item in the other
structure. The precise definition follows: Consider first an item
$<s,xit>$ in the |Y|-structure and let $s'$ be the segment 
associated with the successor item in the |Y|-strucure. If $s \cap s'$
exists and lies to the right of the sweep line then |xit| is an item
in the |X|-structure with key $s \cap s'$. If $s \cap s'$ either does
not exist or does not lie to the right of |L| then $xit=nil$.

Consider next an item $<p,sit>$ in the |X|-structure. If $sit \neq nil$
then $sit$ is an item in the |Y|-structure and the segment
associated with it 
contains $p$. Moreover, if there is a pair of adjacent segments in the
|Y|-structure that intersect in $p$ then $sit \neq nil$. We may
$sit \neq nil$ even if there is no pair of adjacent segments
intersecting in $p$.   

In our example, the $Y$-structure contains the items $<s_1,xit_f>, <s_2,nil>, 
<s_9,nil>, <s_4,xit_a>$ and $<s_3,nil>$ where $xit_a$ and $xit_f$ are the 
items of the $X$-structure with associated points $a$ and $f$ respectively. 
Let's turn to the items of the $X$-structure next. All items except $<d,nil>$ 
point back to the $Y$-structure. If $sit_i$ denotes the item $<s_i$,...$>$, 
$i\in \{1,2,9,4,3\}$, of the $Y$-structure then the items of the $X$-structure
are $<a,sit_4>, <b,sit_4>, <c,sit_1>, <d,nil>, <e,sit_9>, <f,sit_1>, <g,sit_2>,
<h,sit_3>$ and $<i,sit_1>$.


The graph |G| to be constructed has type |GRAPH<rat_point,rat_segment>|, i.e.,
it is a directed graph where a |rat_point|, respectively |rat_segment|, is associated with
each node, respectively edge, of the graph.
The graph $G$ is the part of $G(S)$ that is left of the sweep line. The
point associated with  a vertex defines its position in the plane and
the segment associated with an edge is an input segment containing the
edge. We use two operations to extend the graph $G$.
If |p| is a |rat_point| then
|G.new_node(p)| adds a new node to |G|, associates |p| with the node,
and returns the new node. If |v| and |w| are nodes of |G| and |s|
is a |rat_segment| then |G.new_edge(v,w,s)| adds the edge |(v,w)| to |G|, 
associates |s| with the edge, and returns the new edge.
In order to facilitate the addition of edges we maintain a
map |<rat_segment,vertex> last_node|:. It gives for each segment
in the |Y|-structure the rightmost vertex lying on the segment.

Finally, we have a priority queue |seg_queue| of type 
|p_queue<rat_point,rat_segment>|. 
The LEDA data type |p_queue<P,I>| realizes priority queues with priority type 
|P| and information type |I|. |P| must be linearly ordered. Similar by 
sorted sequences and dictionaries priority queue is an item-based data type. Every item 
(of type |pq_item|) stores a pair $(p,i)$ from $P\times I$, $p$ is called the
priority and $i$ is called the information of the item.  LEDA offers the
usual operations on priority queues (|insert|, |delete_min|, |find_min|). 
The priority queue |seg_queue| contains  all internal segments that are ahead
of the sweep line. The segments are ordered according to their left
endpoint. In particular, the first segment in |seg_queue| is always the
segment that is encountered next by the sweep line.

In the local declarations section of function |sweep| we introduce
the variables of the data types mentioned above.
\vspace{5mm}

@<local declarations @> =
  sortseq<rat_point,seq_item>    X_structure;
  sortseq<rat_segment,seq_item>  Y_structure;
  map<rat_segment,node>          last_node(nil);
  map<rat_segment,rat_segment>       original;
  p_queue<rat_point,rat_segment>     seg_queue;


@ Next we come to the initialization of the data structures. We clear the 
graph $G$, we compute an integer Infinity that is larger than the absolute 
value of the coordinates of all endpoints and that plays the role of $\infty$
in our program, we insert the endpoints of all input segments into the
|X|-structure, and we create for each input segment of non-zero length
an internal segment with the same endpoints, insert this segment into
|seg_queue| and link the input segment to it (through map original),
we create two sentinel segments at $- \infty$ and $+ \infty $ respectively 
and insert them into the |Y|-structure, we put the sweep line at its
initial position by setting |p_sweep| to $( - \infty, - \infty)$, and
we add a stopper point with coordinates $(+ \infty, + \infty)$ to
|seg_queue|. The sentinels avoid special cases and thus simplify the
code. Finally, we introduce a variable |next_seg| that always  contains
the first segment in |seg_queue|.

For the sake of efficiency we do slightly more than just said. Note
that the equality test between points is a costly operation. It involves
four multiplications and two subtractions between integers. We therefore
replace equal points, i.e., points that have the same cartesian 
coordinates, by identical points, i.e., points that have the same
representation in memory, whenever possible. More precisely, when we
process an input segment $s$ we insert its endpoints in the
|X|-structure. If an endpoint already exists in the
|X|-structure this does not modify the
|X|-structure. We may however extract the point from the |X|-structure
equal to the endpoint and use this point instead of the endpoint when
constructing the internal segment. This has the effect of replacing
all equal points by the same point. Thus the expensive equality test
$(operator \equiv)$ can be replaced by the cheap test for identity
(predicate |identical|) in the sequel.\footnote{Some authors have the
|X|-structure contain only the target points of segments that
intersect the sweep line and not all target points as we do. This
reduces the size of the |X|-structure. We have decided to put all
endpoints into the |X|-structure because this makes the identification
of equal endpoints particularly simple.}
\vspace{5mm}

@<initialization @> =

  G.clear();

  integer Infinity = 1;

  rat_segment s;
  forall(s,S) 
  {
    while (fabs(s.xcoord1())>=Infinity || fabs(s.ycoord1())>=Infinity ||
           fabs(s.xcoord2())>=Infinity || fabs(s.ycoord2())>=Infinity)
      Infinity *= 2;

    seq_item it1 = X_structure.insert(s.source(), nil);
    seq_item it2 = X_structure.insert(s.target(), nil);

    if (it1 == it2) continue;  // ignore zero-length segments

    rat_point p = X_structure.key(it1);
    rat_point q = X_structure.key(it2);

    rat_segment s1 = (compare(p,q) < 0) ? rat_segment(p,q) : rat_segment(q,p);
    original[s1] = s;
    seg_queue.insert(s1.source(),s1);
  }


  rat_segment lower_sentinel(-Infinity, -Infinity, Infinity, -Infinity);
  rat_segment upper_sentinel(-Infinity,  Infinity, Infinity,  Infinity);

  p_sweep = lower_sentinel.source();
  Y_structure.insert(upper_sentinel,nil);
  Y_structure.insert(lower_sentinel,nil);

  rat_point pstop(Infinity,Infinity);
  seg_queue.insert(pstop,rat_segment(pstop,pstop));
  rat_segment next_seg = seg_queue.inf(seg_queue.find_min());





@ We now come to the heart of procedure sweep: processing events. Let
|event = <p,sit>| be the first event in the X-structure and assume 
inductively that our data structure is correct for 
$p\_sweep = (p.x,p.y-2\epsilon)$. Our goal is to change |p_sweep| to |p|,
i.e., to move the sweep line across point |p|. 
As long as the X-structure is not empty we perform the following actions.

We first extract the next event point |p_sweep| from the X-structure.
Then, we handle all segments passing through or ending at |p_sweep|. 
Finally, we insert all segments starting at |p_sweep| into the Y-structure, 
check for possible intersections between pairs of segments now adjacent in 
the Y-structure, and update the  X-structure.  
\vspace{5mm}

@<sweep @> =

  while (!X_structure.empty()) 
  { @<extract next event from the X-structure@>@;
    @<handle passing and ending segments @>@;
    @<insert starting segments @>@;
    @<compute new intersections and update X-structure @>@;
   }



@ Extracting the next event is easily done by assigning the minimal key in
the |X|-structure to |p_sweep|, adding a vertex |v| with position |p_sweep| 
to the output graph |G|, and deleting the minimal item from the |X|-structure.
\vspace{5mm}

@<extract next event from the X-structure@> = 

   seq_item event = X_structure.min();
   p_sweep = X_structure.key(event);
   node v = G.new_node(p_sweep);
   X_structure.del_item(event);
    


@ \label{misuse of compare}
Now we handle all segments passing through or ending in point |p|. 
How can we find them?

Recall that the current event is |<p_sweep,sit>|.
Assume first that $sit = nil$. Then there is  no pair of adjacent
segments in the |Y|-structure that intersect in $p\_sweep$ and hence 
there is at most one segment in the |Y|-structure that contains
$p\_sweep$\footnote{This can be seen as follows: Otherwise there
must be a pair of segments that are non-adjacent in the
|Y|-structure intersecting in $p\_sweep$. Consider
any segment in the |Y|-structure between the two. Either its target
point precedes $p\_sweep$ or the segment intersects one of the segments
in the pair. In either case there would be an event
preceding $p\_sweep$.}. 
We can determine this segment  and decide whether there is one by looking
up the zero-length segment $(p\_sweep, p\_sweep)$ in the |Y|-structure.
We explain in section \ref{compare zero-length segments} that our compare 
function for segments works also for zero-length segments. If there is a 
segment containing $p\_sweep$ then we  set $sit$ to the item containing this segment.

Assume next  that $sit \not= nil$ originally. Then we know that the
segment associated with $sit$ contains $p\_sweep$.
Now we have either $sit = nil$ and then no segment in the $Y$-structure 
contains |p_sweep| or $sit \not= nil$ and then the segment 
associated with $sit$ contains $p\_sweep$. In the latter case,
taking |sit| as an entry point into the Y-structure, we determine all segments
incident to |p_sweep| from the left or from below. Note that the corresponding 
items form a subsequence of the $Y$-structure. We also compute the predecessor
(|set_pred|), successor (|sit_succ|), first (|sit_first|) and last (|sit_last|)
item of this {\em bundle} of items. Note further that all items in the bundle 
have information equal to the current event item |event|, except for the last 
item whose information is either |nil| or a different item in the X-structure 
resulting from an intersection with |sit_succ|. 


Starting at |sit| we first walk up until an item is reached whose information 
is distinct from |event|;
|sit_succ| is the successor of this item. Then we walk down to
|sit_pred| which is the first item reached whose information is distinct 
from |event|). During the downward walk we also start to update the data
structures. For every segment |s| in the bundle we add an edge to |G|
connecting |last_node[s]| and |v| and label it with |s|. Also, if the
segment ends at |p_sweep| then we delete it from the |Y|-structure and 
if the segment continues through |p_sweep| then we change the intersection
information associated with it to |nil| and set |last_node| to |v|.

The identification of the subsequence of segments incident to $p\_sweep$ takes 
constant time per element of the sequence. Moreover, the constant 
is small since the test whether $p$ is incident to a segment involves no 
geometric computation but only identity tests between items. The code is 
particularly simple due to our sentinel segments: |sit| can never be the 
first or last item of the Y-structure.


All segments remaining in the bundle pass through node |v| and moving 
the sweep line through |p_sweep| inverses the order of these segments in the 
$Y$-structure. The bundle is non-empty iff the successor of 
item |sit_pred| is not equal to |sit_succ|. In this case we reverse the
bundle by call |Y_structure.reverse_items(sit_first,sit_last)|, where
|sit_first| and |sit_last| are the first and last item of the bundle
respectively. 
\vspace{5mm}

@<handle passing and ending segments @> =

    seq_item sit = X_structure.inf(event);

    if (sit == nil)
      sit = Y_structure.lookup(rat_segment(p_sweep,p_sweep));

    seq_item sit_succ  = nil;
    seq_item sit_pred  = nil;

    if (sit != nil)  
    { 
      /* walk up */
      while (Y_structure.inf(sit) == event) sit = Y_structure.succ(sit);
      sit_succ = Y_structure.succ(sit);

      /* walk down */
      do { rat_segment s = Y_structure.key(sit);
           G.new_edge(last_node[s], v, original[s]);
           if ( identical(p_sweep,s.target()) )
   	     { //ending segment
               seq_item it = Y_structure.pred(sit);
               Y_structure.del_item(sit);
               sit = it; 
              }
           else
             { //passing segment
                Y_structure.change_inf(sit,nil);
   	        last_node[s] = v;
               sit = Y_structure.pred(sit); 
              }
          } while (Y_structure.inf(sit) == event);

       sit_pred = sit;

       /* reverse the bundle of continuing segments (if any) */
       seq_item sit_first = Y_structure.succ(sit_pred);
       if (sit_first != sit_succ)
       { seq_item sit_last = Y_structure.pred(sit_succ);
         Y_structure.reverse_items(sit_first,sit_last);
        }
    }






@ The last step in handling the event point |p_sweep| is to insert all segments
starting at |p_sweep| into the |Y|-structure and to test the new pairs of 
adjacent items (|sit_pred|,...) and (...,|sit_succ|) for possible 
intersections. If there were no segments passing through or ending in |p_sweep|
then the items |sit_succ| and |sit_pred| still have the value |nil| and we 
have to compute them now.

We use the priority queue |seg_queue| to find the segments to be inserted. As 
long as the first segment in |seg_queue| starts at |p_sweep|, 
i.e. |next_seg.source()| is identical to |p_sweep|,
we remove it from the queue and locate it in the |Y|-structure. 
Then we distinguish cases.

If the |Y|-structure contains no segment collinear to |s| then we 
insert |next_seg| into the |Y|-structure; this will add an item
to the |Y|-structure. We associate this item with the right endpoint of
|next_seg| in the |X|-structure; note that the point is already
there but it does not have its link to the |Y|-structure yet. We also
set |last_node[s]| to |v| and if |sit_succ| and |sit_pred| are still
undefined we set them to the successor and predecessor of the new
item respectively.

If there is a segment, say |seg0|, in the |Y|-structure collinear
to |next_seg| then we clip |next_seg| by this segment and add the clipped
segment to |seg_queue| (if it has non-zero length. We do nothing if the
clipped segment has length zero. The reader may wonder why we do not
associate (the item corresponding to) |seg0| with the target point
of |next_seg| in the latter case. Well, our invariant does not require
it because the |Y|-structure does not change. However, there would be
no harm in doing it.
\vspace{5mm}

@<insert starting segments @> =

    while ( identical(p_sweep,next_seg.source()) )
    {
      sit = Y_structure.locate(next_seg);
      rat_segment seg0 = Y_structure.key(sit);

      if (compare(next_seg, seg0) != 0) 
         { // |next_seg| and |seg0| are not collinear
           sit = Y_structure.insert_at(sit, next_seg, nil);
           X_structure.insert(next_seg.target(),sit);
           last_node[next_seg] = v;
           if (sit_succ == nil)  
           { // compute |sit_succ| and |sit_pred| using the first segment
     	     sit_succ = Y_structure.succ(sit);
     	     sit_pred = Y_structure.pred(sit);
            }
          }
      else 
         { // |next_seg| and |seg0| are collinear
           rat_point p = seg0.target();
           rat_point q = next_seg.target();
     	   if (compare(p,q) < 0) { rat_segment
				       clipped_segment
				       = rat_segment (p,q);
                                 seg_queue.insert(p,clipped_segment);
				 original[clipped_segment]
				       = original[next_seg];}
          }

      seg_queue.del_min();
      next_seg = seg_queue.inf(seg_queue.find_min());
     }





@ \label{compute new intersections}
If |sit_pred| still has value |nil| then
|p_sweep| 
is an isolated  point and we are done. Otherwise we compute possible 
intersections between |sit_succ| and its predecessor and between |sit_pred|
and its successor and update the |X|-structure.
\vspace{5mm}

@<compute new intersections and update X-structure @> =

    if (sit_pred != nil) 
    { compute_intersection(X_structure, Y_structure, sit_pred);
      sit = Y_structure.pred(sit_succ);
      if (sit != sit_pred)
 	compute_intersection(X_structure, Y_structure, sit);
     }




@* Geometric Primitives.
\label{geometric primitives}

It remains to define the geometric primitives used in the implementation. We
need two:

\begin{itemize}

\item
  a compare-function for segments which given two segments intersecting the
  sweep line $L$ determines the order of the intersections on $L$. It defines 
  the linear order used in the Y-structure.

\item
  a function |compute_intersection| that decides whether two segments intersect
  and if so whether the intersection is to the right of the sweep line. If
  both tests are positive it also makes the required changes to the $X$- and
  $Y$-structure.

\end{itemize}



@ \label{linear order on segments}
The compare function for segments is surprisingly simple to write. Recall
that every segment in the |Y|-structure intersects the sweep line and
that we want the segments to be ordered according to their intersections
with the sweep line. Also recall that the |Y|-structure is realized
as a sorted sequence. In a sorted sequence comparisons between keys
are only made during insertions and lookups and then one of the keys
involved in the comparison is an argument of the operation\footnote{This
fact is not documented in the LEDA manual and therefore it is maybe unwise
to exploit it. We do it anyway because the resulting code is so elegant.}.
We conclude that compare is only called for |s1| and |s2| where one of the
segments has its source point equal to |p_sweep|. Assume |s2| has. If
|p_sweep| lies on |s1| (orientation |(s1, p_sweep) = 0)| then we only
have to compare the slopes, and if |p_sweep| does not lie on |s1| then
the orientation test is also the outcome of compare.
\vspace{5mm}

@<geometric primitives @> =

static rat_point p_sweep;

inline int compare(const rat_segment& s1, const rat_segment& s2)
{ 
  // Precondition:
  // |p_sweep| is identical to the left endpoint of either |s1| or |s2|. 

  if ( identical(p_sweep,s1.source()) )
  { int s = orientation(s2,p_sweep);
    return (s != 0) ? -s : cmp_slopes(s1,s2);
   }

  if ( identical(p_sweep,s2.source()) )
  { int s = orientation(s1,p_sweep);
    return (s != 0) ? s : cmp_slopes(s1,s2);
   }

  error_handler(1,"internal error in sweep"); // never reached
  return 0;
}


@ \label{compare zero-length segments}
What does compare do when one of the segments, say |s1|, has both endpoints 
equal to |p_sweep| and the Y-structure contains at most one segment 
containing |p_sweep|? That's exactly the situation in section 
\ref{misuse of compare}. When |s2| contains |p_sweep| the result of
both |orientation(s2,p_sweep)| and |cmp_slopes(s1,s2)| is zero, and
|compare| will return zero. The same holds if the roles of |s1| and |s2|
exchanged.
We conclude that the call |Y_structure.locate(s)| where $s$ is the zero-length
segment |(p_sweep,p_sweep)| in section \ref{misuse of compare} has the 
desired effect.




@ \label{compute intersection}
Finally, we define a function |compute_intersection| that takes an item
|sit0| of the Y-structure and determines whether the segment associated with
|sit0| intersects the segment associated with the successor item |sit1| of 
|sit0| right of the sweep line. If so, it updates the X- and the 
Y-structure. Let |s0| and |s1| be the segments associated with |sit0| and 
|sit1| respectively  and let $\ell_0$ and $\ell_1$ be the supporting lines of 
|s0| and |s1|, respectively. 

We know that so intersects the sweep line |L| before |s1|. Thus |s0|  and
|s1| intersect right of the sweep line 
if the right endpoint
of |s1| lies below of or on $\ell_0$  (|orientation(s0,s1.target()) >= 0|) and
the right endpoint of |s0| lies above of or on $\ell_1$ 
(|orientation(s1,s0.target()) <= 0|).

If the segments do intersect we compute the point of intersection, call it |q|, by
a call of |s0.intersection_of_lines(s1,q)|, insert a new pair (|q,sit0|)
into the X-structure and associate this pair with |sit0| in the Y-structure.
\vspace{5mm}

@<geometric ...@>+=

static void compute_intersection0(sortseq<rat_point,seq_item>& X_structure,
                                 sortseq<rat_segment,seq_item>& Y_structure,
                                 seq_item sit0)

{ seq_item sit1 = Y_structure.succ(sit0);
  rat_segment  s0   = Y_structure.key(sit0);
  rat_segment  s1   = Y_structure.key(sit1);
  seq_item q_it = nil;
  rat_point q;

  if (orientation(s0,s1.target()) >= 0 && orientation(s1,s0.target()) <=0 )
  { s0.intersection_of_lines(s1,q);
    q_it = X_structure.insert(q,sit0);
   }

  Y_structure.change_inf(sit0,q_it);
}



@* Improvements.
\label{Improvements}

In general, segments become adjacent in the |Y|-structure more than once.
In this case the program does a lot of unnecessary intersection tests.
In particular, points of intersection already present in the |X|-structure
may be recomputed several times. This is expensive for two reasons.
\begin {enumerate}
\item 
    intersections are always computed exactly
\item 
    the insertion of an intersection event into the |X|-structure
    that is already present compares to equal points with large homogeneous 
    coordinates. In this case, the floating point filter of type |rat_point|
    fails and again exact arithmetic has to be used.
\end {enumerate}

A solution to this problem is to maintain a dictionary storing
for every pair of intersecting segments the item in the |X|-structure
corresponding containing the intersection point. 

For this purpose we extend LEDA's id-numbering of type |rat_segment| 
(function |ID_number|) to a numbering $id$ of all pairs of segments by 
$id(s0,s1)$ =  |ID_number(s0) * max_seg_id + ID_number(s1)|, where |max_seg_id|
is the maximal id-number of all segments used in the sweep. Then we can use a
map |map<int,seq_item> intersection| to associate with every pair of 
intersecting segments |(s0,s1)| the item |intersection[id(s0,s1)]|
in the |X|-structure containing the corresponding point of intersection.

We add a global variable |max_seg_id| to the geometric primitives and
modify the |compute_intersection| function.

We first check whether the two segments already have an intersection
event in the |X|-structure by a lookup in map |intersection|. 


@<geometric ...@>+=

static int max_seg_id;

static void compute_intersection(sortseq<rat_point,seq_item>& X_structure,
                                 sortseq<rat_segment,seq_item>& Y_structure,
                                 seq_item sit0)
{
  static map<int,seq_item> inter_dic;

  seq_item   sit1 = Y_structure.succ(sit0);
  rat_segment  s0 = Y_structure.key(sit0);
  rat_segment  s1 = Y_structure.key(sit1);
  seq_item   q_it = nil;
  rat_point  q;

  if (orientation(s0,s1.end()) >= 0 && orientation(s1,s0.end()) <=0 )
  {
    int id =  ID_Number(s0) * max_seg_id + ID_Number(s1);

    if (inter_dic.defined(id))
         q_it = inter_dic[id];
    else
       { s0.intersection_of_lines(s1,q);
         q_it = X_structure.insert(q,sit0);
         inter_dic[id] = q_it;
        }

    Y_structure.change_inf(sit0,q_it);
  }
}



@ Finally, we must not forget to compute the maximal id-number |max_seg_id|
of all segments in the initialization section.

@<initialization @>+=

  // compute maximal id number of segments (global variable)

  max_seg_id = 0;
  pq_item it;
  forall_items(it,seg_queue)
  { int id = ID_Number(seg_queue.inf(it));
    if (id > max_seg_id) max_seg_id = id;
   }


@* Experiments and Efficiency.
\label{experiments}


We performed tests on three kinds of test data, namely random, difficult, and highly 
degenerate inputs, and with three different implementations of points and 
segments (floating point, long integer arithmetic with floating
point filter, long integer arithmetic without floating point filter). 

Note that it is very easy to change our sweep program to use
different implementations of the basic geometric objects. To switch to
floating point arithmetic just replace types |rat_point| and |rat_segment|
by |point| and |segment|, respectively. The types |point| and |segment|
support the same set of basic geometric primitives implemented, however,
by double floating point arithmetic. The floating point
filter in |rat_point| and |rat_segment| can be switched off 
by setting the flags |rat_point::use_filter| and
|rat_segment::use_filter| to false. We now describe the three kinds of
test data.


\begin{itemize}

\item Random inputs:\\
We generated $n$ (between 1 and several hundred) segments
with random coordinates between $-size$ and $size$ for some parameter $size$. 
For $size$ we used large values to test the correctness and efficiency of the
floating point filter and the long integer arithmetic.

\item Difficult inputs:\\
Let $size$ be a random $k$-bit integer and
let $y = 2size/n$. We intersected $n$ segments where the $i$-th segment 
has endpoints $(size + rx1$, $2 \cdot size  -i \cdot y + ry1)$ and 
$(3size + rx2$, $2size + i \cdot y + ry2)$ where $rx1$, $rx2$, $ry1$, $ry2$ 
are random integers in $[-s,s]$ for some small integer $s$. 

\item Highly degenerate inputs:\\
We generated a large number of segment with random coordinates in a small 
grid to test our claim that we can handle all degeneracies (a set of 100 
segments whose endpoints have integer coordinates between -3 and +3 is highly 
degenerate).

\end{itemize}



Table \ref{random} gives the number of nodes and edges of the
output graph and the running time for 200 segments whose endpoints have 
random $k$ bit coordinates. The experiments inidicate that the floating point 
filter reduces the running time of the program by a factor between 2 and 3. 
 


\begin{table}
\begin{tabular}{rccccc}
$k$ & $|V|$ & $|E|$ &integer &filter &double\\ 
\hline
  10&  4825&  9052&    1.68&    0.91&    0.00 \\
   20&  5539&  10478&    2.40&    1.21&    0.00 \\
   30&  5732&  10864&    2.58&    1.23&    0.45 \\
   40&  5120&  9640&    3.14&    1.22&    0.43 \\
   50&  4182&  7764&    2.76&    1.03&    0.37 \\
   60&  5409&  10218&    3.95&    1.34&    0.45 \\
   70&  4627&  8654&    4.11&    1.32&    0.38 \\
   80&  5195&  9790&    4.97&    1.52&    0.44 \\
   90&  5332&  10064&    6.29&    1.65&    0.48 \\
  100&  4530&  8460&    5.77&    1.58&    0.38 \\
  110&  5021&  9442&    6.92&    1.79&    0.47 \\
  120&  5439&  10278&    8.70&    1.99&    0.47 \\
  130&  5013&  9426&    8.15&    1.95&    0.45 \\
  140&  4600&  8600&    8.33&    1.98&    0.40 \\
  150&  4574&  8548&    9.29&    2.02&    0.40 \\
  160&  5020&  9440&   11.16&    2.23&    0.47 \\
  170&  4758&  8916&   11.99&    2.49&    0.00 \\
  180&  4703&  8806&   12.67&    2.54&    0.40 \\
  190&  4836&  9072&   13.34&    2.60&    0.42 \\
  200&  5253&  9906&   18.08&    3.32&    0.46 \\
  210&  4788&  8976&   17.40&   14.00&    0.45 \\
  220&  6118&  11636&   24.24&   21.27&    0.54 \\
  230&  5086&  9572&   22.62&   18.64&    0.43 \\
  240&  5038&  9476&   21.63&   17.70&    0.36 \\
  250&  5401&  10202&   27.51&   22.70&    0.47 \\
\end{tabular}
\caption{200 random segments, coordinates are random $k$-bit integers.}
\label{random}
\end{table}



Table \ref{difficult} gives the results for the difficult inputs with 
$s=3$, $k=10,15,20,\cdots,100$, and $n=100$.

\begin{table}
\begin{tabular}{rccccc}
$k$ & $|V|$ & $|E|$ &integer &filter &double\\ 
\hline
  10&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  20&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  30&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  40&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  50&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  60&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  70&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  80&    0  &    0  & 0.00   & 0.00  &  0.00  \\
  90&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 100&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 110&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 120&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 130&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 140&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 150&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 160&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 170&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 180&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 190&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 200&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 210&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 220&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 230&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 240&    0  &    0  & 0.00   & 0.00  &  0.00  \\
 250&    0  &    0  & 0.00   & 0.00  &  0.00  \\
\end{tabular}
\caption{The difficult example with 100 segments.}
\label{difficult}
\end{table}

More experiments and a detailed analysis of the floating point filter used
in the implementation of the LEDA data types |rat_point| and |rat_segment| 
are described in \cite{sweep-94,IFIP94,LEDA-Geometry}.


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

\bibitem{Brown}
Brown.
\newblock
\newblock

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

\bibitem{sweep-94}
K.~Mehlhorn and S.~N\"aher.
\newblock An Implementation of a Sweep Line Algorithm for the Straight Line 
Segment Intersection Problem
\newblock Technical Report No. MPI--I--94--160.
\newblock Max-Planck-Institut f\"ur Informatik, 1994.

\bibitem{IFIP94}
K.~Mehlhorn and S.~N\"aher.
\newblock The Implementation of Geometric Algorithms.
\newblock 13th World Computer Congress IFIP 94, Elsevier Science B.V., 
Vol. 1, pp. 223--231, 1994.

\bibitem{LEDA-CACM}
K.~Mehlhorn and S.~N\"aher.
\newblock LEDA: A library of efficient data types and algorithms.
\newblock {\em CACM} Vol. 38, No. 1, pp. 96--102, 1995.

\bibitem{LEDA-Manual}
S.~N\"aher.
\newblock LEDA Manual.
\newblock Technical Report No. MPI--I--93--109.
\newblock Max-Planck-Institut f\"ur Informatik, 1993.

\bibitem{LEDA-Geometry}
S.~N\"aher.
\newblock Basic Geometric Objects and Algorithms in LEDA
\newblock in preparation

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

