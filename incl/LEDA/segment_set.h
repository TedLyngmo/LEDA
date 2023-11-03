/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  segment_set.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_SEGMENT_SET_H
#define LEDA_SEGMENT_SET_H

#include <LEDA/impl/seg_tree.h>
#include <LEDA/line.h>

typedef seg_tree_item seg_item;

typedef list<seg_item> list_seg_item_;


//------------------------------------------------------------------------------
// SegmentSet: a dictionary for line segments  with a fixed orientation
//------------------------------------------------------------------------------

class SegmentSet : public segment_tree<double,double,GenPtr> {

double alpha;           // orientation given by an angle

public:

segment  key(seg_item);

seg_item insert(segment, GenPtr);
seg_item lookup(segment);
void     del(segment);

list<seg_item>  intersection(segment);
list<seg_item>  intersection(line);

void clear() { clear_tree(); }

 SegmentSet(double a=0)  { alpha =a; }
~SegmentSet()  {}
};

#define forall_seg_items(i,S) forall_seg_tree_items(i,S)


//------------------------------------------------------------------------------
// class segment_set: generic SegmentSet
//------------------------------------------------------------------------------


/*{\Manpage {segment_set} {I} {Sets of Parallel Segments}}*/

template<class I>

class segment_set : public SegmentSet {

/*{\Mdefinition
    An instance $S$ of the parameterized data type \name\ is a
    collection of items ($seg\_item$). Every item in $S$ contains as key a
    line segment with a fixed direction $\alpha$ (see data type segment) and
    an information from data type $I$, called the information type of $S$.
    $\alpha$ is called the orientation of $S$. We use $\<s,i\>$ to denote the
    item with segment $s$ and information $i$. For each segment $s$ there is
    at most one item $\<s,i\> \in S$.}*/

public:

/*{\Mcreation S }*/

segment_set(double a) : SegmentSet(a) {}
/*{\Mcreate creates an empty instance \var\ of type \name\ with orientation
            $a$. }*/

segment_set() : SegmentSet(0) {}
/*{\Mcreate creates an empty instance \var\ of type \name\ with orientation
            zero, i.e., horizontal segments.}*/

~segment_set()  {}


/*{\Moperations 2.7 4.5}*/

segment key(seg_item it) {return SegmentSet::key(it);}
/*{\Mop   returns the segment of item $it$.\\
	  \precond $it$ is an item in \var.}*/

I inf(seg_item it)  { return LEDA_ACCESS(I,SegmentSet::inf(it));  }
/*{\Mop   returns the information of item $it$.\\
	  \precond $it$ is an item in \var.}*/

seg_item insert(segment s, I i)   { return SegmentSet::insert(s,Copy(i));}
/*{\Mop   associates the information $i$ with segment
	  $s$. If there is an item $\<s,j\>$ in \var\
	  then $j$ is replaced by $i$, else a new item
	  $\<s,i\>$ is added to $S$. In both cases the
	  item is returned.}*/

seg_item lookup(segment s) {return SegmentSet::lookup(s);}
/*{\Mop   returns the item with segment $s$ (nil if no
	  such item exists in \var).}*/

list<seg_item> intersection(segment q) {return SegmentSet::intersection(q);}
/*{\Mop   returns all items $\<s,i\>\ \in\ S$ with
	  $s \cap q \neq \emptyset$.\\
	  \precond $q$ is
	  orthogonal to the segments in \var.}*/

list<seg_item> intersection(line l) {return SegmentSet::intersection(l);}
/*{\Mop   returns all items $\<s,i\>\ \in\ S$ with
	  $s \cap l \neq \emptyset$. \precond $l$ is
	  orthogonal to the segments in \var.}*/

void del(segment s) {SegmentSet::del(s);}
/*{\Mop   deletes the item with segment $s$ from \var.}*/

void del_item(seg_item it) {SegmentSet::del_item(it);}
/*{\Mop   removes item $it$ from \var.\\
	  \precond $it$ is an item in \var.}*/

void  change_inf(seg_item it, I i) { SegmentSet::change_inf(it,Copy(i)); }
/*{\Mopl  makes $i$ the information of item $it$.\\
	  \precond $it$ is an item in \var.}*/

void clear() { SegmentSet::clear(); }
/*{\Mop   makes \var\ the empty segment\_set.}*/

bool empty() {return SegmentSet::empty();}
/*{\Mop   returns true iff \var\ is empty.}*/

int size() {return SegmentSet::size();}
/*{\Mop   returns the size of \var.}*/

};

/*{\Mimplementation
Segment sets are implemented by dynamic segment trees based on BB[$\alpha$]
trees (\cite{Wi85,Lu78}) trees. Operations key, inf, change\_inf, empty, and
size take time $O(1)$, insert, lookup, del, and del\_item take time
$O(\log^2 n)$ and an intersection operation takes time $O(k + \log^2 n)$,
where $k$ is the size of the returned list. Here $n$ is the current size of
the set. The space requirement is $O(n\log n)$.}*/


#endif
