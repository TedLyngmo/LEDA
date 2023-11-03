/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  r_heap.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


/* This file has been automatically generated from "r_heap.w"
   by CTANGLE (Version 3.1 [km2c]) */




#include <LEDA/basic.h>



class r_heap_node {
  friend class r_heap;

  GenPtr key;			/* key */
  GenPtr inf;			/* information */
  int bucket;			/* number of bucket containing the node */
  r_heap_node *succ, *pred;	/* pointers for list maintenance */

 public:

  r_heap_node(GenPtr k, GenPtr i):
   key(k), inf(i), bucket(0), succ(nil),
   pred(nil) {
  }

   LEDA_MEMORY(r_heap_node)
};

typedef r_heap_node *r_heap_item;




class r_heap {
/* data kept in an |r_heap| */
  int C;			/* maximal difference between two keys in the
				 * heap */
  r_heap_item *buckets;		/* buckets of the |r_heap| */
  int *u;			/* upper bounds on the key intervals
				 * corresponding to the buckets */
  int B;			/* number of buckets */
  int si;			/* current number of elements stored in the
				 * heap */
  int *bsize;			/* table used to (re-)initialize the array
				 * |u| or part of it */

/* private functions that facilitate the descriptions of the |r_heap| operations */

  inline void set_bucket_bounds(int min, int upto);
  inline int findbucket(r_heap_item, int);
  void copy_heap(const r_heap &);
  inline void add_item(r_heap_item, int);
  inline void rm_item(r_heap_item);

/* non-public functions concerned with the use of |r_heap| within LEDA */

  virtual void print_key(GenPtr) const {
  }
  virtual void print_inf(GenPtr) const {
  }
  virtual void clear_key(GenPtr &) const {
  }
  virtual void clear_inf(GenPtr &) const {
  }
  virtual void copy_key(GenPtr &) const {
  }
  virtual void copy_inf(GenPtr &) const {
  }
  virtual int int_type() const {
    return 0;
  }

 protected:
   r_heap_node * item(void *p) const {
    return (r_heap_node *) p;
  }

 public:
   r_heap(int C);
/* the maximal difference between two keys in the heap needs to be provided upon initialization */

  r_heap() {
    error_handler(1, "r_heap: must initialize with int C>=0");
  }
  r_heap(const r_heap &);
  r_heap & operator = (const r_heap &);
  virtual ~ r_heap() {
    clear();
  }

  r_heap_item find_min() const;

  r_heap_item insert(GenPtr k, GenPtr i);	/* precondition: |k >=
						 * key(find_min())| */

  void del_item(r_heap_node * x);

  void del_min();

  void decrease_key(r_heap_node * x, GenPtr k);	/* precond.:
						 * |key(find_min())<=k<key(x)|
						 *  */

  void change_inf(r_heap_node * x, GenPtr i);

  GenPtr key(r_heap_node * x) const {
    return x->key;
  }
  GenPtr inf(r_heap_node * x) const {
    return x->inf;
  }

  void clear();
  int size() const;
  int empty() const;

/* functions that are used by the LEDA iteration macros */

  r_heap_item first_item() const;
  r_heap_item next_item(r_heap_item p) const;

  void print_contents(std::ostream & chk = std::cout) const;
};

/* dummy I/O and cmp functions */

inline void Print(const r_heap &, std::ostream &)
{
}
inline void Read(r_heap &, std::istream &)
{
}
inline int compare(const r_heap &, const r_heap &)
{
  return 0;
}
