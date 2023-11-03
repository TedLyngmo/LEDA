/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  seg_tree.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_SEGMENT_TREE_H
#define LEDA_SEGMENT_TREE_H

// ------------------------------------------------------------------
//
// full dynamic Segment Trees
//
// Michael Wenzel     (1990)
//
// Implementation follows
// Kurt Mehlhorn: Data Structures and Algorithms, Vol. 3
//
// ------------------------------------------------------------------

#include <LEDA/impl/bb1_tree.h>
#include <LEDA/list.h>

// ------------------------------------------------------------------
// declarations and definitions
// -----------------------------------------------------------------


class h_segment;
typedef h_segment* h_segment_p;

class seg_node_tree;
typedef seg_node_tree* seg_node_list;


typedef bb1_node* seg_tree_item;
typedef list<seg_tree_item> list_seg_tree_item_;

//------------------------------------------------------------------
// class h_segment
//------------------------------------------------------------------

class h_segment {

  GenPtr _x0;
  GenPtr _x1;
  GenPtr _y;
  GenPtr _inf;

 public:

 GenPtr& x0()    { return _x0;   }
 GenPtr& x1()    { return _x1;   }
 GenPtr& y()     { return _y;    }
 GenPtr& info()  { return _inf;  }

 h_segment()
{
   _x0 = _x1 = _y = _inf = 0;
 }

 h_segment(GenPtr x0, GenPtr x1, GenPtr y, GenPtr i=0)
{
   _x0  = x0;
   _x1  = x1;
   _y   = y;
   _inf = i;
 }

 LEDA_MEMORY(h_segment)

 friend std::ostream& operator<<(ostream&, h_segment&);
 friend class Segment_Tree;
 friend class seg_node_tree;

};


/*------------------------------------------------------------------
   class seg_node_tree = Dictionary(seg_tree_item , void* )
-------------------------------------------------------------------*/

class seg_node_tree : public bb1_tree {

public:

Segment_Tree* father;

int cmp(GenPtr x, GenPtr y) const;

list<seg_tree_item> query(GenPtr, GenPtr);
list<seg_tree_item> all_items();

int            defined(h_segment_p y)    { return bb1_tree::member(Convert(y)); }
seg_tree_item  lookup(h_segment_p y)     { return bb1_tree::lookup(Convert(y)); }
seg_tree_item  locate(h_segment_p y)     { return bb1_tree::locate(Convert(y)); }
seg_tree_item  ord(int y)                { return bb1_tree::ord(int(y)); }
seg_tree_item  insert(h_segment_p y, GenPtr i=0 )
                                 { return bb1_tree::insert(Convert(y),i); }
void del(h_segment_p y)          { delete bb1_tree::del(Convert(y)); }
void del_item(seg_tree_item it)  { del(key(it)); }

h_segment_p& key(seg_tree_item it)
	     { if (!it) error_handler(1,"seg_tree_item gleich nil");
               return (h_segment_p&)it->ke  ; }
GenPtr&   info(seg_tree_item it)              { return key(it)->info(); }
void         change_inf(seg_tree_item it, GenPtr i) { key(it)->info() = i; }

seg_node_tree(Segment_Tree* p)   {father = p;}
virtual ~seg_node_tree()  {}

friend class Segment_Tree;

} ;


#define forall_seg_tree_items(a,b) for ((b).init_iterator(); a=(b).move_iterator(); )



//------------------------------------------------------------------
// class segment_tree
//------------------------------------------------------------------

class Segment_Tree  : public bb1_tree {


virtual  h_segment_p new_y_h_segment(GenPtr y)
{ std::cout << "error: virtual new_y_h_segmentn"; y=0; return 0; }

virtual int cmp_dim1(GenPtr,GenPtr) {return 0; }
virtual int cmp_dim2(GenPtr,GenPtr) {return 0; }

virtual void clear_dim1(GenPtr&) {}
virtual void clear_dim2(GenPtr&) {}
virtual void clear_info(GenPtr&) {}
virtual void copy_dim1(GenPtr&)  {}
virtual void copy_dim2(GenPtr&)  {}
virtual void copy_info(GenPtr&)  {}

int seg_cmp(h_segment_p p, h_segment_p q);

  void lrot(bb1_item , bb1_item);
  void rrot(bb1_item , bb1_item);
  void ldrot(bb1_item , bb1_item);
  void rdrot(bb1_item , bb1_item);

  //void change_inf(bb1_item it, seg_node_list i)   { info(it) = i; }

  GenPtr& key(bb1_item it)
       { if (!it) error_handler(1,"bb1_item in segment_tree gleich nil");
	 return it->ke; }
  seg_node_list& info(bb1_item it)    { return (seg_node_list&)(bb1_tree::info(it)); }

  int start_coord(bb1_item& x,seg_tree_item& i)
      { return (!cmp(key(x),x0(i))); }
  int end_coord(bb1_item& x,seg_tree_item& i)
      { return (!cmp(key(x),x1(i))); }

  int empty(bb1_item);
  void clear(bb1_item& );
  void print(bb1_item , string);

  protected:

  seg_node_tree r;                // tree with all segments


  int cmp_dummy(int a, int b, int c);


  public :

  int cmp(GenPtr, GenPtr)  const
  { std::cout << "error: Segment_Tree::cmpn"; return 0; }

  GenPtr x0(seg_tree_item x)    { return (r.key(x))->_x0;  }
  GenPtr x1(seg_tree_item x)    { return (r.key(x))->_x1;  }
  GenPtr y(seg_tree_item x)     { return (r.key(x))->_y;   }
  GenPtr& inf(seg_tree_item x)  { return r.info(x);        }

  GenPtr& x0(h_segment_p x)   { return x->_x0; }
  GenPtr& x1(h_segment_p x)   { return x->_x1; }
  GenPtr& y(h_segment_p x)    { return x->_y; }
  GenPtr& inf(h_segment_p x)  { return x->_inf; }

  void change_inf(seg_tree_item x, GenPtr i)  { r.info(x) = i; }

  seg_tree_item insert(GenPtr, GenPtr, GenPtr, GenPtr i=0 );

  void del(GenPtr, GenPtr, GenPtr);
  void del_item(seg_tree_item it) { del(x0(it),x1(it),y(it)) ; }


  seg_tree_item lookup(GenPtr, GenPtr, GenPtr );
  int member(GenPtr x0, GenPtr x1, GenPtr y) { return (lookup(x0,x1,y)!=0 ) ; }

  list<seg_tree_item> query(GenPtr, GenPtr, GenPtr );
  list<seg_tree_item> x_infinity_query(GenPtr, GenPtr );
  list<seg_tree_item> y_infinity_query(GenPtr );
  list<seg_tree_item> all_items();

  void clear_tree();

   Segment_Tree();
   virtual ~Segment_Tree();

  int size()                   { return r.size();   }
  int empty()                  { return (r.size()==0) ; }

  seg_tree_item y_min()        { return r.min();    }
  seg_tree_item y_max()        { return r.max();    }

  void init_iterator()            { r.init_iterator(); }
  seg_tree_item move_iterator()   { return r.move_iterator(); }

  void print_tree()               { print(root,"");    }


  friend class seg_node_tree;

};


//------------------------------------------------------------------
// typed segment_tree
//------------------------------------------------------------------



template <class  type1, class type2, class itype>

class segment_tree : public Segment_Tree {

h_segment_p new_y_h_segment(GenPtr y)
{ type1 x1;
  type2 x2;
  GenPtr p = Copy(x1);
  GenPtr q = Copy(x2);
  return new h_segment(p,q,y);
 }

int cmp_dim1(GenPtr x,GenPtr y) { return LEDA_COMPARE(type1,x,y); }
int cmp_dim2(GenPtr x,GenPtr y) { return LEDA_COMPARE(type2,x,y); }

void clear_dim1(GenPtr& x)     { LEDA_CLEAR(type1,x); }
void clear_dim2(GenPtr& x)     { LEDA_CLEAR(type2,x); }
void clear_info(GenPtr& x)     { LEDA_CLEAR(itype,x); }

void copy_dim1(GenPtr& x)     { LEDA_COPY(type1,x); }
void copy_dim2(GenPtr& x)     { LEDA_COPY(type2,x); }
void copy_info(GenPtr& x)     { LEDA_COPY(itype,x); }

int cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(type1,x,y);}

public:

type1  x0(seg_tree_item it)  { return LEDA_ACCESS(type1,Segment_Tree::x0(it)); }
type1  x1(seg_tree_item it)  { return LEDA_ACCESS(type1,Segment_Tree::x1(it)); }
type2   y(seg_tree_item it)  { return LEDA_ACCESS(type2,Segment_Tree::y(it));  }
itype inf(seg_tree_item it)  { return LEDA_ACCESS(itype,Segment_Tree::inf(it));}

seg_tree_item insert(type1 x0, type1 x1, type2 y, itype i)
{ return Segment_Tree::insert(Convert(x0),Convert(x1),Convert(y),Convert(i)); }

void del(type1 x0, type1 x1, type2 y)
{ Segment_Tree::del(Convert(x0),Convert(x1),Convert(y)); }

seg_tree_item lookup(type1 x0, type1 x1, type2 y)
{ return Segment_Tree::lookup(Convert(x0),Convert(x1),Convert(y)); }

int member(type1 x0, type1 x1, type2 y)
{ return Segment_Tree::member(Convert(x0),Convert(x1),Convert(y)); }

list<seg_tree_item> query(type1 x,type2 y0,type2 y1)
{ return Segment_Tree::query(Convert(x),Convert(y0),Convert(y1)); }

list<seg_tree_item> x_infinity_query(type2 y0,type2 y1)
{ return Segment_Tree::x_infinity_query(Convert(y0),Convert(y1)); }

list<seg_tree_item> y_infinity_query(type1 x)
{ return Segment_Tree::y_infinity_query(Convert(x)); }


segment_tree()  {}
~segment_tree()
 { seg_tree_item z;
  forall_seg_tree_items(z,r)
  { type1 t1 = x0(z); Clear(t1);
          t1 = x1(z); Clear(t1);
    type2 t2 = y(z);  Clear(t2);
    itype i  = inf(z); Clear(i);
    delete r.key(z); }
 }

} ;


//------------------------------------------------------------------
// Iterator
//------------------------------------------------------------------

#define forall_seg_tree_items(a,b) for ((b).init_iterator(); a=(b).move_iterator(); )


#endif
