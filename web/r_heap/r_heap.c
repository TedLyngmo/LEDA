
/* This file has been automatically generated from "r_heap.w"
   by CTANGLE (Version 3.1 [km2c]) */




#include "r_heap.h"
#include <cmath>




r_heap::r_heap(int c)
{
  C = c;
  B = int (ceil(log(C) / log(2))) + 2;

  buckets = (r_heap_item *) new int[B];
  for (int i = 0; i < B; i++)
    buckets[i] = nil;

  bsize = new int[B];
  u = new int[B];

  bsize[0] = 1;
  bsize[B - 1] = MAXINT;
  for (i = 1; i < B - 1; i++)
    bsize[i] = 1 << (i - 1);

  u[B - 1] = MAXINT;		/* this value won't change throughout the
				 * computation the other u[] values will be
				 * initialized by |insert| */
  si = 0;
}



r_heap::r_heap(const r_heap & rh)
{
  copy_heap(rh);
}
r_heap & r_heap::operator = (const r_heap & rh) {
  if (this != &rh) {
    delete[] buckets;
    delete[] u;

    copy_heap(rh);
  }
}




inline void r_heap::add_item(r_heap_item it, int bnr)
{
  it->succ = buckets[bnr];
  if (buckets[bnr] != nil)
    buckets[bnr]->pred = it;
  it->pred = nil;
  it->bucket = bnr;
  buckets[bnr] = it;
}

inline void r_heap::rm_item(r_heap_item it)
{
  if (it->pred != nil)
    (it->pred)->succ = it->succ;
  else
    buckets[it->bucket] = it->succ;
  if (it->succ != nil)
    (it->succ)->pred = it->pred;
}



void r_heap::copy_heap(const r_heap & rh)
{
  C = rh.C;
  B = rh.B;
  si = rh.si;

  buckets = (r_heap_item *) new int[B];
  u = new int[B];
  bsize = new int[B];


  for (int i = 0; i < B; i++) {
    u[i] = rh.u[i];
    bsize[i] = rh.bsize[i];
  }

  r_heap_item item1, item2;
  for (i = 0; i < rh.B; i++) {
    if (rh.buckets[i] != nil) {
      item1 = rh.buckets[i];
      do {
	item2 = new r_heap_node(item1->key, item1->inf);
	add_item(item2, i);
	item1 = item1->succ;
      } while (item1 != nil);
    }
    else
      buckets[i] = nil;
  }
}



inline void r_heap::set_bucket_bounds(int min, int upto)
{

  u[0] = min;
  for (int i = 1; i < upto; i++) {
    u[i] = u[i - 1] + bsize[i];
    if (u[i] > u[upto])
      break;
  }
  for (; i < upto; i++)
    u[i] = u[upto];
}



inline int r_heap::findbucket(r_heap_item it, int start)
{
  if (int (it->key) == u[0])
    start = -1;
  else
    while (int (it->key) <= u[--start]);

/* now $u[start] < int(it->key) \le u[start+1]$ */

  return (start + 1);
}





r_heap_item r_heap::find_min(void) const
{
  if (si > 0)
    return buckets[0];
  else
    error_handler(1, "r_heap::find_min : Heap is empty!");
}



r_heap_item r_heap::insert(GenPtr k, GenPtr i)
{

  r_heap_item item = new r_heap_node(k, i);

  if (si > 0) {			/* We check whether the operation respects
				 * the r\_heap conditions */
    if (int (k) < u[0] || int (k) > u[0] + C) {
      string s("r_heap::insert: k= %d out of range [%d,%d]\n", int (k), u[0], u[0] + C);
      error_handler(1, s);
    }
    int i = findbucket(item, B - 1);
    add_item(item, i);
  }
  else {
    set_bucket_bounds((int) k, B - 1);
    buckets[0] = item;
    item->bucket = 0;
  }
  si++;

  return item;
}



void r_heap::del_min(void)
{
  if (si > 0) {
    r_heap_item it = buckets[0];
    del_item(it);
  }
  else
    error_handler(1, "r_heap::del_min : Heap is empty!");
}



void r_heap::decrease_key(r_heap_node * x, GenPtr k)
{
  if ((int (k) < int (x->key)) &&(int (k) >= u[0])) {
    x->key = k;
    if ((int (k) <= u[x->bucket - 1])) {
      rm_item(x);
      int i = findbucket(x, x->bucket);
      add_item(x, i);
    }
  }
  else {
    string s("r_heap::decrease_key: k= %d out of range [%d,%d]\n", int (k), u[0], int (x->key) - 1);
    error_handler(1, s);
  }
}



void r_heap::change_inf(r_heap_node * x, GenPtr i)
{
  x->inf = i;
}



void r_heap::clear(void)
{
  r_heap_item it;
  for (int i = 1; i < B; i++)
    while ((it = buckets[i]) != nil) {
      rm_item(it);
      delete it;
    }
}



void r_heap::del_item(r_heap_node * x)
{
  int buck = x->bucket;
  rm_item(x);
  delete x;

  if ((si > 1) && (buck == 0) && (buckets[0] == nil)) {
    r_heap_item item;



    int idx = 1;
    while (buckets[idx] == nil)
      idx++;

    item = buckets[idx];
    r_heap_item dummy = item->succ;

    while (dummy != nil) {
      if ((int) dummy->key < (int) item->key)
	item = dummy;
      dummy = dummy->succ;
    }
/* we have found the minimum */




    set_bucket_bounds(int (item->key), idx);
    rm_item(item);
    add_item(item, 0);

/* Redistribution */

    item = buckets[idx];
    r_heap_item next;

    while (item != nil) {
      next = item->succ;

/* we know that every item in bucket \#idx MUST be redistributed */

      rm_item(item);
      int i = findbucket(item, idx);
      add_item(item, i);

      item = next;
    }


  }
  si--;
}



int r_heap::size(void) const
{
  return si;
}

int r_heap::empty(void) const
{
  return (si == 0);
}



r_heap_item r_heap::first_item(void) const
{

  return buckets[0];		/* nil if heap is empty */
}

r_heap_item r_heap::next_item(r_heap_item p) const
{

  if (p->succ != nil)
    return p->succ;
  else {
    int next = p->bucket + 1;
    while ((next < B) && (buckets[next] == nil))
      next++;
    if (next == B)
      return nil;
    else
      return buckets[next];
  }
}



void r_heap::print_contents(std::ostream & os) const
{
  r_heap_item item;

  os << "--------------------------------------\n";
  os << "Si: " << si << "\n";
  os << "--------------------------------------\n";
  for (int i = 0; i < B; i++) {
    os << "--------------------------------------\n";
    os << "Bucket " << i << "\n";
    os << "Intervall: [";
    if (i > 0)
      os << u[i - 1] - 1;
    else
      os << u[i];
    os << "," << u[i] << "]\n";
    os << "--------------------------------------\n";
    item = buckets[i];
    while (item != nil) {
      os << "Key: " << (int) item->key << " Bucket: " << item->bucket;
      os << "\n";
      item = item->succ;
    }
  }
}
