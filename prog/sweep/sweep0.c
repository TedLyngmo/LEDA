
/* This file has been automatically generated from "sweep.w"
   by CTANGLE (Version 3.1 [km2c]) */

#include <LEDA/sortseq.h>
#include <LEDA/graph.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/integer.h>
#include <LEDA/floatf.h>
#include <math.h>

typedef rat_point POINT;
typedef rat_segment SEGMENT;

extern int cmp_points_count;
extern int exact_cmp_points_count;


class MyPointRep;

static MyPointRep *first_myPoint = 0;


struct MyPointRep {
  POINT pt;
  floatf x;
  floatf y;
  floatf w;
  int count;
  MyPointRep *next;

   MyPointRep(const POINT & p) {
    pt = p;
    x = floatf(p.X());
    y = floatf(p.Y());
    w = floatf(p.W());
    count = 0;
    next = first_myPoint;
    first_myPoint = this;
  }
   LEDA_MEMORY(MyPointRep)
};

typedef MyPointRep *myPoint;


struct MySegmentRep {

  myPoint start;
  myPoint end;
  SEGMENT seg;

  floatf dx;
  floatf dy;

  node last_node;

   MySegmentRep(const SEGMENT & s) {
    start = new MyPointRep(s.start());
    end = new MyPointRep(s.end());
    seg = s;
    dx = floatf(s.dx());
    dy = floatf(s.dy());
    last_node = nil;
  }

   MySegmentRep(const myPoint & p) {	/* creates the zero-length segment
					 * $(p,p)$ */
    start = p;
    end = p;
    seg = SEGMENT(p->pt, p->pt);
    last_node = nil;
  }

  LEDA_MEMORY(MySegmentRep)
};

typedef MySegmentRep *mySegment;





static integer Infinity;
static myPoint p_sweep;
static bool use_filter;

int cmp_points_count = 0;
int exact_cmp_points_count = 0 ;

#if defined(STATISTICS)
int cmp_points_failed;
int cmp_segments_count;
int exact_cmp_segments_count;
#endif





/*
template <class int_type> inline
int cmp_points(const int_type& x1, const int_type& y1, const int_type& w1,
               const int_type& x2, const int_type& y2, const int_type& w2)
{
  int s = Sign(x1*w2-x2*w1);

  return (s != 0) ? s : Sign(y1*w2-y2*w1);
 }
*/

inline int cmp_points(const integer & x1, const integer & y1, const integer & w1,
   const integer & x2, const integer & y2, const integer & w2)
{
  cmp_points_count++;
  exact_cmp_points_count++;

  int s1 = sign(x1) * sign(w2);
  int s2 = sign(x2) * sign(w1);

  if (s1 > s2)
    return +1;
  if (s1 < s2)
    return -1;

  int l1 = x1.length() + w2.length();
  int l2 = x2.length() + w1.length();

  if (l1 > l2 + 1)
    return s1;
  if (l2 > l1 + 1)
    return -s1;

/*
  int s = cmp_products(x1,w2,x2,w1);
  return (s != 0) ? s : cmp_products(y1,w2,y2,w1);
*/


  int s = compare(x1 * w2, x2 * w1);
  return (s != 0) ? s : compare(y1 * w2, y2 * w1);

}


inline int cmp_points(const floatf & x1, const floatf & y1, const floatf & w1,
   const floatf & x2, const floatf & y2, const floatf & w2)
{
  cmp_points_count++;
  int s = Sign(x1 * w2 - x2 * w1);
  return (s != 0) ? s : Sign(y1 * w2 - y2 * w1);
}


inline int cmp_points(const double &x1, const double &y1, const double &w1,
   const double &x2, const double &y2, const double &w2)
{
  int s = Sign(x1 * w2 - x2 * w1);
  return (s != 0) ? s : Sign(y1 * w2 - y2 * w1);
}






template < class int_type >
int cmp_segments(const int_type & px, const int_type & py, const int_type & pw,
   const int_type & spx, const int_type & spy, const int_type & spw,
   const int_type & sqx, const int_type & sqy, const int_type & sqw,
   const int_type & rx, const int_type & ry, const int_type & rw,
   const int_type & dx, const int_type & dy,
   const int_type & sdx, const int_type & sdy)
{

/*
    We first test whether the underlying lines are identical. The lines are
    identical if the three slopes $dy/dx$, $sdy/sdx$, and $mdy/mdx$ are equal
  */



  int_type T1 = dy * sdx - sdy * dx;

  int sign1 = Sign(T1);


  if (sign1 == 0 || sign1 == NO_IDEA) {
    int_type mdx = sqx * pw - px * sqw;
    int_type mdy = sqy * pw - py * sqw;
    int sign2 = Sign(dy * mdx - mdy * dx);
    if (sign2 == 0 || sign2 == NO_IDEA) {
      int sign3 = Sign(sdy * mdx - mdy * sdx);
      if (sign3 == 0 || sign3 == NO_IDEA)
	return (sign1 == 0 && sign2 == 0 && sign3 == 0) ? 0 : NO_IDEA;
    }
  }



/* The underlying lines are different; in particular, at most one of the
      lines is vertical. We first deal with the cases that one of the lines
      is vertical. A segment $(p,q)$ is vertical iff $px*qw-qx*pw$ is equal
      to zero. Since |dx| is an optimal floating point approximation
      of this integer value, a segment is vertical iff its |dx|-value is zero.
    */

  if (dx == 0) {		/* |dx| = 0, i.e., |s1| is vertical and |s2|
				 * is not vertical; $l \cap s2$ is above
				 * |p_sweep| iff $(spy/spw + sdy/sdx(rx/rw -
				 * spx/spw) -ry/rw) > 0$; in this case we
				 * return $-1$ */

    int i = Sign((spy * sdx - spx * sdy) * rw + (sdy * rx - ry * sdx) * spw);
    if (i == NO_IDEA)
      return NO_IDEA;
    return (i <= 0) ? 1 : -1;
  }

  if (sdx == 0) {		/* $sdx = 0$, i.e., |s2| is vertical but |s1|
				 * is not vertical; we return -1 if $l \cap
				 * s1$ is below or equal to |p_sweep| iff
				 * $(py/pw + dy/dx(rx/rw - px/pw)-ry/rw) \leq
				 * 0$. */

    int i = Sign((py * dx - px * dy) * rw + (dy * rx - ry * dx) * pw);
    if (i == NO_IDEA)
      return NO_IDEA;
    return (i <= 0) ? -1 : 1;
  }

/* Neither |s1| nor |s2| is vertical. We compare $l \cap s1$ and $ l \cap s2$.
     We have $$ y(l \cap s1) -y(l \cap s2) =
     \frac{py}{pw} +\frac{dy}{dx}(\frac{rx}{rw} -\frac{px}{pw}) -\frac{spy}{spw}
     - \frac{sdy}{sdx}(\frac{rx}{rw} - \frac{spx}{spw}). $$
     If the difference is non-zero then we return its sign. If the difference is
     zero then we return $-1$ iff the common intersection is not above
     |p_sweep| and $s1$ has the smaller slope or the intersection is above
     |p_sweep| and $s1$ has the larger slope.
  */

  int_type T2 = sdx * spw * (py * dx * rw + dy * (rx * pw - px * rw))
  - dx * pw * (spy * sdx * rw + sdy * (rx * spw - spx * rw));

  int sign2 = Sign(T2);

  if (sign2 == NO_IDEA)
    return NO_IDEA;

  if (sign2 != 0)
    return sign2;

/* Now we know that the difference is zero, i.e., |s1| and |s2|
     intersect in a point $I$. We compare slopes:\\
     |s1| has larger slope than |s2| iff |T1*dxs*dx > 0|;
     note that orienting the lines from left to right makes all |dx|
     values non-negative , i.e.,
     |s1| has larger slope than |s2| iff |sign(T1) = 1|
   */

  int_type T3 = (py * dx - px * dy) * rw + (dy * rx - ry * dx) * pw;

/* The common intersection $I$ is above |p_sweep| iff $T3*rw*dx*pw > 0$.
     In this case we return |-sign(T1)| and |sign(T1)| otherwise.
     Note that all |dx| and |w| values are non-negative
     i.e., |sign(T3*rw*dx*pw) = sign(T3)|
   */

  int sign3 = Sign(T3);

  if (sign3 == NO_IDEA)
    return NO_IDEA;

  return (sign3 <= 0) ? sign1 : -sign1;

}




inline int compare(const myPoint & a, const myPoint & b)
{
/* floating point filter version for |myPoint|s */

  int c = NO_IDEA;

/* if not explicitely turned off we first use floating point arithmetic */

  if (use_filter)
    c = cmp_points(a->x, a->y, a->w, b->x, b->y, b->w);

/* if the floating point computation is not reliable, i.e., the result
    is |NO_IDEA| we use exact arithmetic (|integer|)
  */

  if (c == NO_IDEA) {
    c = cmp_points(a->pt.X(), a->pt.Y(), a->pt.W(), b->pt.X(), b->pt.Y(), b->pt.W());


#if defined(STATISTICS)
    if (cmp_points(double (a->x), double (a->y), double (a->w),
	double (b->x), double (b->y), double (b->w)) !=c)
      cmp_points_failed++;
#endif

  }

  return c;
}


inline int compare(const mySegment & s1, const mySegment & s2)
{
  int c = NO_IDEA;

/* if not explicitely turned off we first try the
     floating point computation
   */

#if defined(STATISTICS)
  cmp_segments_count++;
#endif

  if (use_filter)
    c = cmp_segments(s1->start->x, s1->start->y, s1->start->w,
      s2->start->x, s2->start->y, s2->start->w,
      s2->end->x, s2->end->y, s2->end->w,
      p_sweep->x, p_sweep->y, p_sweep->w,
      s1->dx, s1->dy, s2->dx, s2->dy);


/* If the result is not reliable  we call the exact compare
     for the underlying |SEGMENT|s.
   */

  if (c == NO_IDEA) {
    c = cmp_segments(s1->seg.X1(), s1->seg.Y1(), s1->seg.W1(),
      s2->seg.X1(), s2->seg.Y1(), s2->seg.W1(),
      s2->seg.X2(), s2->seg.Y2(), s2->seg.W2(),
      p_sweep->pt.X(), p_sweep->pt.Y(), p_sweep->pt.W(),
      s1->seg.dx(), s1->seg.dy(), s2->seg.dx(), s2->seg.dy());
#if defined(STATISTICS)
    exact_cmp_segments_count++;
#endif
  }

  return c;
}




int cmp_mySeg(const mySegment & s1, const mySegment & s2)
{
  int c = NO_IDEA;

  if (use_filter)		/* floating point compare */
    c = cmp_points(s1->start->x, s1->start->y, s1->start->w,
      s2->start->x, s2->start->y, s2->start->w);

  if (c == NO_IDEA)		/* exact compare */
    c = cmp_points(s1->seg.X1(), s1->seg.Y1(), s1->seg.W1(),
      s2->seg.X1(), s2->seg.Y1(), s2->seg.W1());
  return c;
}





void compute_intersection(sortseq <myPoint, seq_item >&X_structure,
   sortseq <mySegment, seq_item >&Y_structure,
   seq_item sit0)
{
  seq_item sit1 = Y_structure.succ(sit0);
  mySegment seg0 = Y_structure.key(sit0);
  mySegment seg1 = Y_structure.key(sit1);

/* |seg1| is the successor of |seg0| in the Y-structure, hence,
     the underlying lines intersect right or above of the sweep line
     iff the slope of |seg0| is larger than the slope of |seg1|.
   */

  if (use_filter) {
    int i = Sign(seg0->dy * seg1->dx - seg1->dy * seg0->dx);
    if (i == -1 || i == 0)
      return;			/* $slope(s0)\leq slope(s1)$ */
  }


  SEGMENT s0 = seg0->seg;
  SEGMENT s1 = seg1->seg;

  integer w = s0.dy() * s1.dx() - s1.dy() * s0.dx();

  if (sign(w) > 0) {		/* $slope(s0) > slope(s1)$ */
    integer c1 = s0.X2() * s0.Y1() - s0.X1() * s0.Y2();
    integer c2 = s1.X2() * s1.Y1() - s1.X1() * s1.Y2();

/* The underlying lines intersect in a point right or above of the
       sweep line. We still have to test whether it lies on both segments.
     */

    integer x = c2 * s0.dx() - c1 * s1.dx();


    integer d0 = x * s0.W2() - s0.X2() * w;
    if (sign(d0) > 0)
      return;

    if (x * s1.W2() > s1.X2() * w)
      return;

    integer y = c2 * s0.dy() - c1 * s1.dy();
    if (sign(d0) == 0 && y * s0.W2() > s0.Y2() * w)
      return;

    myPoint Q = new MyPointRep(POINT(x, y, w));
    seq_item xit = X_structure.insert(Q, sit0);
    X_structure.key(xit)->count++;
    Y_structure.change_inf(sit0, xit);

  }
}






void sweep0(list <SEGMENT > &S, GRAPH <POINT, SEGMENT > &G, bool use_filter)
{


  sortseq <myPoint, seq_item >X_structure;

  sortseq <mySegment, seq_item >Y_structure;



cmp_points_count = 0;
exact_cmp_points_count = 0;


#if defined(STATISTICS)
  cmp_points_failed = 0;
  cmp_segments_count = 0;
  exact_cmp_segments_count = 0;
#endif

  ::use_filter = use_filter;

/* compute an upper bound |Infinity| for the input coordinates */

  Infinity = 1;

  SEGMENT s;

  forall (s, S)
    while (abs(s.X1()) >= Infinity || abs(s.Y1()) >= Infinity ||
      abs(s.X2()) >= Infinity || abs(s.Y2()) >= Infinity)
      Infinity *= 2;

  p_sweep = new MyPointRep(POINT(-Infinity, -Infinity));


  mySegment uppersentinel =
  new MySegmentRep(SEGMENT(-Infinity, Infinity, Infinity, Infinity));

  mySegment lowersentinel =
  new MySegmentRep(SEGMENT(-Infinity, -Infinity, Infinity, -Infinity));

  Y_structure.insert(uppersentinel, nil);
  Y_structure.insert(lowersentinel, nil);


  G.clear();

  list <mySegment > S_Sorted;

  forall (s, S) {
/* |mySegment|s are always oriented from left to right or (if vertical)
     from bottom to top
   */

    if (compare(s.start(),s.end()) > 0) 
      s = SEGMENT(s.end(), s.start());

    mySegment s1 = new MySegmentRep(s);
    S_Sorted.append(s1);
    seq_item it = X_structure.insert(s1->start, nil);
    s1->start = X_structure.key(it);
    s1->start->count++;
  }



  S_Sorted.sort(cmp_mySeg);





  while (!X_structure.empty()) {
    seq_item event = X_structure.min();
    seq_item sit = X_structure.inf(event);
    myPoint p = X_structure.key(event);
    node v = G.new_node(p->pt);

    p_sweep = p;



    seq_item sit_succ = nil;
    seq_item sit_pred = nil;

    if (sit == nil) {
      MySegmentRep s(p);	/* create a zero length segment $s = (p,p)$ */
      sit = Y_structure.lookup(&s);
    }

    if (sit != nil) {

      seq_item sit_last = sit;

      while (Y_structure.inf(sit_last) == event)
	sit_last = Y_structure.succ(sit_last);

      sit_succ = Y_structure.succ(sit_last);

      sit_pred = Y_structure.pred(sit);

      while (Y_structure.inf(sit_pred) == event)
	sit_pred = Y_structure.pred(sit_pred);

      seq_item sit_first = Y_structure.succ(sit_pred);






      seq_item i1 = sit_pred;
      seq_item i2 = sit_first;

      while (i2 != sit_succ) {
	mySegment s = Y_structure.key(i2);

	G.new_edge(s->last_node, v, s->seg);
	s->last_node = v;

	if (p == s->end) {	/* ending segment */
	  Y_structure.del_item(i2);
	  delete s;
	}
	else {			/* continuing segment */
	  if (i2 != sit_last)
	    Y_structure.change_inf(i2, nil);
	  i1 = i2;
	}

	i2 = Y_structure.succ(i1);
      }

      sit_first = Y_structure.succ(sit_pred);
      sit_last = Y_structure.pred(sit_succ);





      seq_item xit = Y_structure.inf(sit_pred);

      if (xit != nil) {
	if (--X_structure.key(xit)->count == 0)
	  X_structure.del_item(xit);
	Y_structure.change_inf(sit_pred, nil);
      }

      if (sit_last != sit_pred) {
	xit = Y_structure.inf(sit_last);
	if (xit != nil) {
	  if (--X_structure.key(xit)->count == 0)
	    X_structure.del_item(xit);
	  Y_structure.change_inf(sit_last, nil);
	}
	Y_structure.reverse_items(sit_first, sit_last);
      }


    }






    while (!S_Sorted.empty() && p == S_Sorted.head()->start) {
      mySegment Seg = S_Sorted.pop();

/* first insert the right endpoint of |Seg| into the X-structure */

      seq_item end_it = X_structure.insert(Seg->end, nil);
      Seg->end = X_structure.key(end_it);
      Seg->end->count++;

/* note that the following test uses the fact that two endpoints are equal
     if an only if the corresponding pointer values (|myPoint|s) are equal
   */

      if (Seg->start == Seg->end) {	/* Seg has length  zero, nothing to
					 * do */
	delete Seg;
	continue;
      }

      sit = Y_structure.locate(Seg);

      if (compare(Seg, Y_structure.key(sit)) != 0) {	/* |Seg| is not
							 * collinear with the
							 * segment associated
							 * with |sit|. We simply
							 * insert |Seg| into the
							 * Y-structure */
	sit = Y_structure.insert_at(sit, Seg, nil);
	Seg->last_node = v;
      }
      else {			/* |Seg| is collinear with the segment
				 * associated with |sit|. If |Seg| is longer
				 * then we use |Seg| and otherwise we do
				 * nothing. */

	mySegment Seg_old = Y_structure.key(sit);

	if (compare(Seg->end, Seg_old->end) > 0) {	/* |Seg| extends further
							 * to the right or above
							 * replace |Seg_old| by
							 * |Seg|.  */
	  Seg_old->seg = Seg->seg;
	  Seg_old->end = Seg->end;
	}

	delete Seg;		/* not needed anymore */
      }

      X_structure.change_inf(end_it, sit);

      if (sit_succ == nil) {
	sit_succ = Y_structure.succ(sit);
	sit_pred = Y_structure.pred(sit);

/* |sit_pred| is no longer adjacent to |sit_succ| we have to
        change its current intersection event to |nil| and delete
        the corresponding item in the X-structure if necessary
      */

	seq_item xit = Y_structure.inf(sit_pred);
	if (xit != nil) {
	  if (--X_structure.key(xit)->count == 0)
	    X_structure.del_item(xit);
	  Y_structure.change_inf(sit_pred, nil);
	}
      }
    }

/* compute possible intersections */

    if (sit_succ != nil) {	/* |v| is an isolated vertex otherwise */
      compute_intersection(X_structure, Y_structure, sit_pred);

      sit = Y_structure.pred(sit_succ);

      if (sit != sit_pred)
	compute_intersection(X_structure, Y_structure, sit);
    }






    X_structure.del_item(event);
  }





  {
    delete(uppersentinel);
    delete(lowersentinel);
    myPoint p = first_myPoint;
    while (first_myPoint != nil) {
      p = first_myPoint->next;
      delete(first_myPoint);
      first_myPoint = p;
    }

#if defined(STATISTICS)
    if (use_filter) {
      cout << string ("compare points:  %6d / %4d (%5.2f%%)  (%5.2f%% failed)  ",
	 cmp_points_count, exact_cmp_points_count,
	 (100.0 * exact_cmp_points_count) / cmp_points_count,
	 (100.0 * cmp_points_failed) / cmp_points_count);
      newline;
      cout << string ("compare segments: %6d / %4d  (%5.2f%%)",
	 cmp_segments_count, exact_cmp_segments_count,
	 (100.0 * exact_cmp_segments_count) / cmp_segments_count);
      newline;

    }
#endif

  }




}
