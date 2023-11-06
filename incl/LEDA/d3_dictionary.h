/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  d3_dictionary.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_d3_dictionary_H
#define LEDA_d3_dictionary_H

#include <LEDA/impl/range_tree.h>
typedef rt_item dic3_item;



template<class type1, class type2, class type3, class itype>
class d3_dictionary : public range_tree {

  // redefine the virtual functions of class range_tree

  void rt_clear_key(GenPtr*& x) const {
    Clear(LEDA_ACCESS(type1,x[0])); Clear(LEDA_ACCESS(type2,x[1]));
    Clear(LEDA_ACCESS(type3,x[2]));
  }

  void rt_copy_key(GenPtr*& x) const {
    Copy(LEDA_ACCESS(type1,x[0])); Copy(LEDA_ACCESS(type2,x[1]));
    Copy(LEDA_ACCESS(type3,x[2]));
  }

  void rt_print_key(int d,GenPtr*& x) const {
    switch(d) {
      case 0: LEDA_PRINT(type1,x[0],cout);
      	      break;
      case 1: LEDA_PRINT(type2,x[1],cout);
      	      break;
      case 2: LEDA_PRINT(type3,x[2],cout);
      	      break;
    }
  }

  void rt_clear_inf(GenPtr& x) const { Clear(LEDA_ACCESS(itype,x));}
  void rt_copy_inf(GenPtr& x) const { Copy(LEDA_ACCESS(itype,x));}

  int rt_cmp(int d, GenPtr* p, GenPtr* q) const {
    switch(d) {
      case 0: return compare(LEDA_ACCESS(type1,p[0]),LEDA_ACCESS(type1,q[0]));
      case 1: return compare(LEDA_ACCESS(type2,p[1]),LEDA_ACCESS(type2,q[1]));
      case 2: return compare(LEDA_ACCESS(type3,p[2]),LEDA_ACCESS(type3,q[2]));
    }
  }

  range_tree* new_range_tree(int d, int level) {
    return new d3_dictionary<type1,type2,type3,itype>(level);
  }

  public:

    d3_dictionary( int level=0 ) : range_tree(3,level) {}
    ~d3_dictionary() { clear(); }

    itype inf(dic3_item x)    { return LEDA_ACCESS(itype,x->inf()); }
    type1 key1(dic3_item x)   { return LEDA_ACCESS(type1,x->key(0)); }
    type2 key2(dic3_item x)   { return LEDA_ACCESS(type2,x->key(1)); }
    type3 key3(dic3_item x)   { return LEDA_ACCESS(type3,x->key(2)); }


    void  change_inf(dic3_item x, itype i) {
      Clear(LEDA_ACCESS(itype,x->inf()));
      x->inf() = Copy(i);
    }

    dic3_item min_key1() { return range_tree::rt_min(0); }
    dic3_item max_key1() { return range_tree::rt_max(0); }
    dic3_item min_key2() { return range_tree::rt_min(1); }
    dic3_item max_key2() { return range_tree::rt_max(1); }
    dic3_item min_key3() { return range_tree::rt_min(2); }
    dic3_item max_key3() { return range_tree::rt_max(2); }

    dic3_item insert(type1 x,type2 y,type3 z,itype i)
    {
      rt_item p = new rt_elem(Copy(x),Copy(y),Copy(z),Copy(i));
      return range_tree::insert(p);
     }

    list<rt_item> range_search( type1 x0, type1 x1,
  			        type2 y0, type2 y1,
  			        type3 z0, type3 z1 )
    {
      rt_elem p(Convert(x0),Convert(y0),Convert(z0),0);
      rt_elem q(Convert(x1),Convert(y1),Convert(z1),0);
      return range_tree::query(&p,&q);
    }

    dic3_item lookup( type1 x, type2 y, type3 z ) {
      rt_elem p(Convert(x),Convert(y),Convert(z),0);
      return range_tree::lookup(&p);
    }

    void del(type1 x,type2 y, type3 z) {
      rt_elem p(Convert(x),Convert(y),Convert(z),0);
      range_tree::del(&p);
     }

    void del_item(dic3_item it) { range_tree::del(it); }
    list<dic3_item> all_items() { return range_tree::all_items(); }
};

// iteration macro
//
#define forall_dic3_items(x,T)  (T).init_iteration(); forall(x,(T).L )

#endif
