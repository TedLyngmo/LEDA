/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  slist.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_IMPL_SLIST_H
#define LEDA_IMPL_SLIST_H

//------------------------------------------------------------------------------
// simply linked lists
//------------------------------------------------------------------------------


#include <LEDA/basic.h>


class SLIST;
class slink;

typedef slink* slist_item;

//------------------------------------------------------------------------------
// class slink
//------------------------------------------------------------------------------

class slink {

  friend class SLIST;

  slink* succ;
  GenPtr e;

  slink(GenPtr a, slink* suc) { e = a; succ = suc; }

  LEDA_MEMORY(slink)

};


//------------------------------------------------------------------------------
// SLIST: base class for all simply linked Lists
//------------------------------------------------------------------------------

class SLIST {

   slink* h;                     //head
   slink* t;                     //tail
   slink* iterator;              //iterator;
   int    count;                 //length of List

virtual void clear_el(GenPtr&) const {}
virtual void copy_el(GenPtr&)  const {}
virtual int  int_type() const { return 0; }

public:

   int space()  const { return sizeof(SLIST) + count * sizeof(slink); }
   int length() const { return count; }
   bool empty()  const { return (count==0);}

   slink* insert(GenPtr, slink*);


   slink* push(GenPtr a)
   { count++;
     h = new slink(a,h);
     if (t==0) t = h;
     return h;
   }

   slink* append(GenPtr a)
   { count++;
     if (t) t = t->succ = new slink(a,0);
     else   t = h = new slink(a,0);
     return t;
   }

   slink* first()               const { return h; }
   slink* first_item()          const { return h; }
   slink* last()                const { return t; }
   slink* last_item()           const { return t; }
   slink* next_item(slink* p)   const { return p->succ; }
   slink* succ(slink* l)        const { return l->succ; }
   slink* cyclic_succ(slink*)   const;


// iteration

   GenPtr loop_to_succ(GenPtr& x) const { return x = slist_item(x)->succ; }

#if defined(__ELSE_SCOPE_BUG__)
   GenPtr forall_loop_item;
   GenPtr& Forall_Loop_Item() const { return (GenPtr&)forall_loop_item; }
#endif

// old iteration stuff

   void   set_iterator(slink* p)   const { *(slink**)&iterator = p; }
   slink* get_iterator()  const { return iterator; }
   void   init_iterator() const { set_iterator(0); }

   slink* move_iterator() const
   { set_iterator( iterator ? iterator->succ : h);
     return iterator;
   }

   void conc(SLIST&);

   GenPtr head()   const { return h ? h->e : 0;}
   GenPtr tail()   const { return t ? t->e : 0;}
   GenPtr pop();

   void del_succ(slink* p)
   { slink* q = p->succ;
     if (q == t) t = p;
     p->succ = q->succ;
     delete q;
     count--;
   }

   bool current_element(GenPtr& x)  const
   { if (!iterator) return false;
     else { x = iterator->e;
            return true; }
    }

   bool next_element(GenPtr& x) const
  { move_iterator();
    return current_element(x);
   }

   GenPtr  contents(slink* l)  const  { return l->e; }
   GenPtr& entry(slink* l)            { return l->e; }
   GenPtr& operator[](slink* l)       { return l->e; }
   GenPtr  operator[](slink* l) const { return l->e; }

   void clear();

   SLIST();
   SLIST(GenPtr a);
   SLIST& operator=(const SLIST&);
   SLIST(const SLIST&);
   virtual ~SLIST()     { clear(); }
};


#if !defined(__TEMPLATE_FUNCTIONS__)
// dummy I/O and cmp functions

inline void Print(const SLIST&,std::ostream&) { }
inline void Read(SLIST&, std::istream&) { }
inline int  compare(const SLIST&,const SLIST&) { return 0; }
#endif

#endif
