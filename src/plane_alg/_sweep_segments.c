/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _sweep_segments.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/graph.h>
#include <LEDA/sortseq.h>
#include <LEDA/p_queue.h>
#include <LEDA/map.h>
#include <LEDA/hash.h>

#include <cmath>


// global variables

static POINT p_sweep;
static int   N;

// geometric primitives

inline int pair(const SEGMENT& p, const SEGMENT& q)
{ return  ID_Number(p) * N + ID_Number(q); }

inline int compare(const SEGMENT& s1, const SEGMENT& s2)
{
  // Precondition: |p_sweep| is identical to the left endpoint of
  // either |s1| or |s2|. This is true since comparisons are only
  // executed when inserting or looking up new segments.

  if ( identical(p_sweep,s1.start()) )
  { int s = orientation(s2,p_sweep);
    return (s != 0) ?  s : cmp_slopes(s1,s2);
   }

  if ( identical(p_sweep,s2.start()) )
  { int s = orientation(s1,p_sweep);
    return (s != 0) ? -s : cmp_slopes(s1,s2);
   }

  error_handler(1,"internal error in sweep");
  return 0; // never reached
}


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

  if (orientation(s0,s1.end()) <= 0 && orientation(s1,s0.end()) >=0 )
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


void SWEEP_SEGMENTS(const list<SEGMENT>& S, GRAPH<POINT,SEGMENT>& G)
{
  // we use two sorted sequences ...

  sortseq <POINT,seq_item>   X_structure;
  sortseq <SEGMENT,seq_item> Y_structure;

  // two maps ...

  map<SEGMENT,node>    last_node(nil);
  map<SEGMENT,SEGMENT> original;

  hash<int,seq_item>   inter_dic;


  // and a priority queue of segments ordered by their left endpoints

  p_queue<POINT,SEGMENT> seg_queue;

  /* INITIALIZATION
     - clear the output graph.
     - compute an upper bound |Infinity| for the input coordinates
     - make copies of the input segments such that all segments are
       oriented from left to right or from bottom to top.
     - insert all endpoints of the new segments into the X-structure
     - exploit the fact that insert operations into the X-structure
       leave previously inserted points unchanged to achieve that
       any pair of endpoints $p$ and $q$ with |p == q| are identical
     - use a map to associate with every segment its original
     - for every created segment $(p,q)$ insert the pair $(p,(p,q))$
       into priority queue |seg_queue|
   */

  G.clear();

  numtype Infinity = 1;

  SEGMENT s;
  forall(s,S)
  {
    while (fabs(s.xcoord1())>=Infinity || fabs(s.ycoord1())>=Infinity ||
           fabs(s.xcoord2())>=Infinity || fabs(s.ycoord2())>=Infinity)
      Infinity *= 2;

    seq_item it1 = X_structure.insert(s.start(), nil);
    seq_item it2 = X_structure.insert(s.end(), nil);

    if (it1 == it2) continue;  // ignore zero-length segments

    POINT p = X_structure.key(it1);
    POINT q = X_structure.key(it2);

    SEGMENT s1(p,q);
    if (compare(p,q) > 0) s1 = SEGMENT(q,p);
    original[s1] = s;
    seg_queue.insert(s1.start(),s1);
  }

  // insert a lower and an upper sentinel segment to avoid special
  // cases when traversing the Y-structure

  SEGMENT lower_sentinel(-Infinity, -Infinity, Infinity, -Infinity);
  SEGMENT upper_sentinel(-Infinity,  Infinity, Infinity,  Infinity);

  // the sweep begins at the lower left corner

  p_sweep = lower_sentinel.start();
  Y_structure.insert(upper_sentinel,nil);
  Y_structure.insert(lower_sentinel,nil);


  // insert a stopper into |seg_queue| and initialize |next_seg| with
  // the first segment in  the queue

  POINT   p_stop(Infinity,Infinity);
  SEGMENT s_stop(p_stop,p_stop);
  seg_queue.insert(p_stop,s_stop);
  SEGMENT next_seg = seg_queue.inf(seg_queue.find_min());

  N = ID_Number(s_stop);

  // Main Loop

  while (!X_structure.empty())
  {
    // move |p_sweep| to next event point and insert a new node
    // into the output graph G

    seq_item event = X_structure.min();
    p_sweep = X_structure.key(event);
    node v = G.new_node(p_sweep);

    // If there is a non-nil item |sit| associated with |event|,
    // |key(sit)| is either an ending or passing segment.
    // We use |sit| as an entry point to compute the bundle of
    // segments ending at or passing through |p_sweep|.
    // In particular, we compute the first (sit_first)| and last
    // (|sit_last|) item of this bundle and the successor (|sit_succ)|)
    // and predecessor (|sit_pred|) items.

    seq_item sit = X_structure.inf(event);

    if (sit == nil)
    { // here we do not know any segments ending or passing through
      // |p_sweep|. However, |p_sweep| could come to lie on a segment
      // inserted before. To check this we look up the zero length
      // segment |(p_sweep,p_sweep)|.
      sit = Y_structure.lookup(SEGMENT(p_sweep,p_sweep));
     }

    seq_item sit_succ  = nil;
    seq_item sit_pred  = nil;
    seq_item sit_first = nil;
    seq_item sit_last  = nil;

    // A value of |nil| for |sit_succ| and |sit_pred| after the
    // following computation indicates that there are no segments
    // ending at or passing through |p_sweep|

    if (sit != nil)  // key(sit) is an ending or passing segment
    {
      // first walk up until |sit_succ|

      while (Y_structure.inf(sit) == event) sit = Y_structure.succ(sit);
      sit_succ = Y_structure.succ(sit);

      seq_item xit = Y_structure.inf(sit);
      if (xit != nil)
      { SEGMENT s1 = Y_structure.key(sit);
        SEGMENT s2 = Y_structure.key(sit_succ);
        inter_dic.insert(pair(s1,s2),xit);
       }

      // walk down until |sit_pred|, construct edges for all segments
      // in the bundle, assign information |nil| to continuing segments,
      // and delete ending segments from the Y_structure

      do { SEGMENT s = Y_structure.key(sit);
           SEGMENT orig = original[s];
           if (s.source() == orig.source())
              G.new_edge(last_node[s], v, orig);
           else
              G.new_edge(v,last_node[s], orig);
   	   last_node[s] = v;
           if ( identical(p_sweep,s.end()) )  // ending segment
   	     { seq_item it = Y_structure.pred(sit);
               Y_structure.del_item(sit);
               sit = it;
              }
           else                               // passing segment
             { Y_structure.change_inf(sit,nil);
               sit = Y_structure.pred(sit);
              }
          } while (Y_structure.inf(sit) == event);

       sit_pred  = sit;
       sit_first = Y_structure.succ(sit_pred);

       // reverse the bundle of continuing segments (if existing)

       if (sit_first != sit_succ)
       { sit_last = Y_structure.pred(sit_succ);
         Y_structure.reverse_items(sit_first,sit_last);
        }
    }

    // insert all segments starting at |p_sweep|

    while ( identical(p_sweep,next_seg.start()) )
    {
      // insert |next_seg| into the Y-structure and associate the
      // corresponding item with the right endpoint of |next_seg| in
      // the X-structure (already present)

      sit = Y_structure.locate(next_seg);

      SEGMENT seg0 = Y_structure.key(sit);

      if (compare(next_seg, seg0) != 0)
         { // |next_seg| is not collinear with seg0, insert it

           sit = Y_structure.insert_at(sit, next_seg, nil);
           X_structure.insert(next_seg.end(),sit);
           last_node[next_seg] = v;

           if (sit_succ == nil)
           { // there are only starting segments, compute |sit_succ|
             // and |sit_pred| using the first inserted segment
     	     sit_succ = Y_structure.succ(sit);
     	     sit_pred = Y_structure.pred(sit);
             sit_first = sit_succ;
            }
         }
      else
        { // |next_seg| and |seg0| are collinear; if |next_seg| is
          // longer insert |(seg0.end(),next_seg.end())| into |seg_queue|
          POINT p = seg0.end();
          POINT q = next_seg.end();
     	  if (compare(p,q) < 0) seg_queue.insert(p,SEGMENT(p,q));
         }

      // delete minimum and assign new minimum to |next_seg|

      seg_queue.del_min();
      next_seg = seg_queue.inf(seg_queue.find_min());
     }

    // if |sit_pred| still has the value |nil|, there were no ending,
    // passing or starting segments, i.e., |p_sweep| is an isolated
    // point. In this case we are done, otherwise we delete the event
    // associated with |sit_pred| from the X-structure and compute
    // possible intersections between new neighbors.

    if (sit_pred != nil)
    {
      // |sit_pred| is no longer adjacent to its former successor we
      // change its intersection event to |nil|

      seq_item xit = Y_structure.inf(sit_pred);
      if (xit != nil)
      { SEGMENT s1 = Y_structure.key(sit_pred);
        SEGMENT s2 = Y_structure.key(sit_first);
        inter_dic.insert(pair(s1,s2),xit);
        Y_structure.change_inf(sit_pred, nil);
       }

      // compute possible intersections between |sit_pred| and its
      // successor and |sit_succ| and its predecessor

      compute_intersection(X_structure, Y_structure, inter_dic, sit_pred);
      sit = Y_structure.pred(sit_succ);
      if (sit != sit_pred)
 	compute_intersection(X_structure, Y_structure, inter_dic, sit);
     }

    // finally we delete the current event from the X-structure
    X_structure.del_item(event);
  }

}


void SWEEP_SEGMENTS(const list<SEGMENT>& S, list<POINT>& L)
{
  GRAPH<POINT,SEGMENT> G;
  SWEEP_SEGMENTS(S,G);
  L.clear();

  node v;
  forall_nodes(v,G)
   if (G.degree(v) > 1) L.append(G[v]);
}
