/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  dlist.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DLIST_H
#define LEDA_DLIST_H

//------------------------------------------------------------------------------
//  doubly linked lists
//------------------------------------------------------------------------------

#include <LEDA/basic.h>

//------------------------------------------------------------------------------
// some declarations
//------------------------------------------------------------------------------

class dlist; 
class dlink;

typedef dlink* list_item;

//------------------------------------------------------------------------------
// class dlink (list items)
//------------------------------------------------------------------------------

class dlink {

  dlink* succ;
  dlink* pred;
  GenPtr e;

  dlink(GenPtr a, dlink* pre, dlink* suc)
  { 
    e = a;
    succ = suc;
    pred = pre;
  }

  LEDA_MEMORY(dlink)


  friend class dlist;
  friend dlink* succ_item(dlink* p) { return p->succ; }
  friend dlink* pred_item(dlink* p) { return p->pred; }

  //space: 3 words = 12 bytes
};




//------------------------------------------------------------------------------
// dlist: base class for all doubly linked lists
//------------------------------------------------------------------------------

class dlist {

   dlink* h;                     //head
   dlink* t;                     //tail
   dlink* iterator;              //iterator
   int count;                    //length of list

//space: 4 words + virtual =  5 words = 20 bytes

virtual int  cmp(GenPtr, GenPtr) const { return 0; }
virtual int  ord(GenPtr) const { return 0; }
virtual void app(GenPtr&) const {}
virtual void read_el(GenPtr&,istream&) const {}
virtual void print_el(GenPtr&,ostream&) const {}
virtual void clear_el(GenPtr&) const {}
virtual void copy_el(GenPtr&)  const {}
virtual int  int_type() const { return 0; }

void quick_sort(list_item*,list_item*);
void int_quick_sort(list_item*,list_item*);

void insertion_sort(dlink**,dlink**,dlink**);
void int_insertion_sort(dlink**,dlink**,dlink**);

void recompute_length() const;

public:

// access operations

   int  length() const { if (count < 0) recompute_length(); return count; }
   int  size()   const { return length(); }
   bool empty()  const { return h==nil; }

   dlink* first()               const { return h; }
   dlink* first_item()          const { return h; }
   dlink* last()                const { return t; }
   dlink* last_item()           const { return t; }
   dlink* next_item(dlink* p)   const { return p->succ; }
   dlink* succ(dlink* l)        const { return l->succ; }
   dlink* pred(dlink* l)        const { return l->pred; }
   dlink* cyclic_succ(dlink*)   const;
   dlink* cyclic_pred(dlink*)   const;
   dlink* succ(dlink* l, int i) const; 
   dlink* pred(dlink* l, int i) const;
   dlink* get_item(int = 0)     const; 

   dlink* max() const;
   dlink* min() const;
   dlink* search(GenPtr) const;

   int    rank(GenPtr) const;

   GenPtr contents(dlink* l) const { return l->e; }
   GenPtr head()             const { return h ? h->e : 0;}
   GenPtr tail()             const { return t ? t->e : 0;}


// update operations

protected:

   dlink* insert(GenPtr a, dlink* l, int dir=0);

   dlink* push(GenPtr a)   
   { if (count >= 0) count++;
     if (h) h = h->pred = new dlink(a,0,h); 
     else   h = t =  new dlink(a,0,0);
     return h;
    }
   
   dlink* append(GenPtr a)
   { if (count >= 0) count++;
     if (t) t = t->succ = new dlink(a,t,0);
     else   t = h = new dlink(a,0,0);
     return t; 
    } 
   
   void   assign(dlink* l, GenPtr a) { clear_el(l->e); l->e = a; }

   void   apply();
   void   sort();

public:

   GenPtr del(dlink* loc);
   GenPtr pop();
   GenPtr Pop();

   void   conc(dlist&,int dir=0);
   void   split(list_item,dlist&,dlist&,int dir=0);
   void   bucket_sort(int,int);
   void   permute();
   void   clear();

// iteration

   GenPtr loop_to_succ(GenPtr& x) const { return x = list_item(x)->succ; }
   GenPtr loop_to_pred(GenPtr& x) const { return x = list_item(x)->pred; }

#if defined(__ELSE_SCOPE_BUG__)
   GenPtr forall_loop_item;
   GenPtr& Forall_Loop_Item() const { return (GenPtr&)forall_loop_item; }
#endif



//  old iteration stuff

   void set_iterator(dlink* p) const { *(dlink**)&iterator = p; }
   void init_iterator()        const { set_iterator(nil); }
   void reset()                const { set_iterator(nil); }
 
   dlink* get_iterator()       const { return iterator; }
   dlink* move_iterator(int=0) const;

   bool   current_element(GenPtr&) const;
   bool   next_element(GenPtr&)    const;
   bool   prev_element(GenPtr&)    const;


// operators

   GenPtr&   entry(dlink* l)            { return l->e; }
   GenPtr    inf(dlink* l)        const { return l->e; }
   GenPtr&   operator[](dlink* l)       { return l->e; }
   GenPtr    operator[](dlink* l) const { return l->e; }

   dlist& operator=(const dlist&); 
   dlist  operator+(const dlist&); 


   void print(ostream&,string, char)       const;    
   void print(ostream& out,char space=' ') const { print(out,"",space);  }
   void print(string s, char space=' ')    const { print(cout,s,space);  }
   void print(char space=' ')              const { print(cout,"",space); }   


   void read(istream&,string, int);  

   void read(istream& in,int delim) { read(in,"",delim); }
   void read(istream& in)           { read(in,"",EOF); }

   void read(string s, int delim)   { read(cin,s,delim); }   
   void read(string s)              { read(cin,s,'\n'); }   

   void read(char delim)  { read(cin,"",delim);}  
   void read()            { read(cin,"",'\n');}  


// constructors & destructors

   dlist();    
   dlist(GenPtr a);
   dlist(const dlist&);

   virtual ~dlist()  { clear(); }

   int space()  const { return sizeof(dlist) + length() * sizeof(dlink); }
};



#if !defined(__TEMPLATE_FUNCTIONS__)
// default I/O and cmp functions

inline void Print(const dlist& L,ostream& out) { L.print(out); out << endl; }
inline void Read(dlist& L, istream& in)        { L.read(in,'\n'); }
inline int  compare(const dlist&,const dlist&) { return 0; }
#endif

#endif
