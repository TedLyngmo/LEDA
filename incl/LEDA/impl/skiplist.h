/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  skiplist.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_SKIPLIST_H
#define LEDA_SKIPLIST_H

#include <LEDA/basic.h>

//------------------------------------------------------------------------------
// Pugh's skip lists
//------------------------------------------------------------------------------

class skiplist_node
{ 
  friend class skiplist;

  static unsigned long id_count;

  GenPtr key;
  GenPtr inf;
  int    level;
  unsigned long id;           // id number
  skiplist_node* pred;  
  skiplist_node* backward;  
  skiplist_node* forward[1];  // variable sized array of forward pointers 

  friend unsigned long ID_Number(skiplist_node* p) { return p->id; }

 };

typedef skiplist_node* skiplist_item;


class skiplist
{  
   int level;                  // maximum level
   skiplist_node*  header;     // pointer to header
   skiplist_item STOP;         // pointer to end 
   unsigned long randomBits;   // random bit source
   int randomsLeft;            // number of unused bit pairs in randomBits
   int count;                  // number of entries

   float prob;                 

   random_source ran;

   int  randomLevel();

virtual int cmp(GenPtr, GenPtr) const { return 0; }
virtual void copy_key(GenPtr&)  const {}
virtual void copy_inf(GenPtr&)  const {}
virtual void clear_key(GenPtr&) const {}
virtual void clear_inf(GenPtr&) const {}
virtual void print_key(GenPtr)  const {}
virtual void print_inf(GenPtr)  const {}

virtual int int_type()    const { return 0; }

skiplist_item search(GenPtr,int&) const;
void          remove_item(skiplist_item);
void          insert_item_at_item(skiplist_item,skiplist_item);

public:
 skiplist_item item(void* p) const { return skiplist_item(p); }

 skiplist(float prob = 0.25);
 skiplist(const skiplist&);
 skiplist& operator=(const skiplist&);
 virtual ~skiplist();

 GenPtr key(skiplist_item p) const;
 GenPtr inf(skiplist_item p) const;
 GenPtr& info(skiplist_item p) const;
 int     get_level(skiplist_item p) const;

 skiplist_item insert(GenPtr key, GenPtr inf);
 skiplist_item locate(GenPtr key) const;
 skiplist_item locate_succ(GenPtr key) const;
 skiplist_item locate_pred(GenPtr key) const;
 skiplist_item lookup(GenPtr key) const;
 skiplist_item min() const;
 skiplist_item max() const;
 void          reverse_items(skiplist_item,skiplist_item);
 void          del(GenPtr key);
 void          del1(GenPtr key);

 void          conc(skiplist&);
 void          split_at_item(skiplist_item,skiplist&,skiplist&);

 void          print();

 skiplist_item insert_at_item(skiplist_item p, GenPtr key, GenPtr inf);

 skiplist_item insert1(GenPtr key, GenPtr inf);


 void          change_inf(skiplist_item p, GenPtr inf);
 void          del_item(skiplist_item p);
 void          clear();
 int           size() const;
 int           empty() const;

 skiplist_item first_item() const;
 skiplist_item next_item(skiplist_item p) const;
 skiplist_item succ(skiplist_item p) const;
 skiplist_item pred(skiplist_item p) const;


 // piority queue operations

 skiplist_item find_min() const;
 void del_min();
 void decrease_key(skiplist_item p, GenPtr k);


};




inline GenPtr  skiplist::key(skiplist_item p) const { return p->key; }
inline GenPtr  skiplist::inf(skiplist_item p) const { return p->inf; }
inline GenPtr& skiplist::info(skiplist_item p) const { return p->inf; }

inline int     skiplist::get_level(skiplist_item p) const { return p->level; }


inline skiplist_item skiplist::first_item() const  
{ skiplist_item q = header->forward[0];
  return (q==STOP) ? 0 : q;
 }

inline skiplist_item skiplist::min() const  
{ skiplist_item q = header->forward[0];
  return (q==STOP) ? 0 : q;
 }

inline skiplist_item skiplist::max() const  
{ skiplist_item q = STOP->pred;
  return (q==header) ? 0 : q;
 }

inline skiplist_item skiplist::next_item(skiplist_item p) const 
{ skiplist_item q =  p->forward[0]; 
  return (q==STOP) ? 0 : q;
 }

inline skiplist_item skiplist::succ(skiplist_item p) const 
{ skiplist_item q =  p->forward[0]; 
  return (q==STOP) ? 0 : q;
 }

inline skiplist_item skiplist::pred(skiplist_item p) const 
{ skiplist_item q =  p->pred; 
  return (q==header) ? 0 : q;
 }

inline void skiplist::change_inf(skiplist_item p, GenPtr inf) 
{ clear_inf(p->inf); 
  copy_inf(inf);  
  p->inf = inf; 
 }

inline int  skiplist::size() const { return count; }
inline int  skiplist::empty() const { return count==0; }

//priority queue
inline skiplist_item skiplist::find_min() const { return min(); }

inline void skiplist::del_min() 
{ skiplist_item p = min(); if (p) del_item(p); }

inline void skiplist::decrease_key(skiplist_item p, GenPtr k) 
{ insert(k,p->inf); del_item(p);}


#if !defined(__TEMPLATE_FUNCTIONS__)
// dummy I/O and cmp functions

inline void Print(const skiplist&,ostream&) { }
inline void Read(skiplist&, istream&) { }
inline int  compare(const skiplist&,const skiplist&) { return 0; }
#endif

#endif
