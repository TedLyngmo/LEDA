/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  p_dictionary.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_P_DICTIONARY_H
#define LEDA_P_DICTIONARY_H

#include <LEDA/impl/pers_tree.h>


typedef pers_tree_node* p_dic_item;


template <class K,class I>

class PERS_DIC: public pers_rb_tree, public handle_rep {

void copy_key(GenPtr& x)   { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)   { LEDA_COPY(I,x); }
void clear_key(GenPtr& x)  { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x)  { LEDA_CLEAR(I,x); }
void print_key(GenPtr x)   { LEDA_PRINT(K,x,cout); }
void print_inf(GenPtr x)   { LEDA_PRINT(I,x,cout); }

int  cmp_keys(GenPtr x, GenPtr y) { return LEDA_COMPARE(K,x,y); }

Version V;

public:

 PERS_DIC() { init_tree(); V = v_list->vl.first(); }
 PERS_DIC(V_LIST* vl,Version v) { v_list=vl; V=v;  }
 void CLEAR() { if (--v_list->count==0) del_tree(); }
~PERS_DIC() { CLEAR(); }

PERS_DIC(const PERS_DIC<K,I>& D)
{ v_list = D.v_list; v_list->count++; V = D.V; count = D.count; }

PERS_DIC<K,I>& operator=(PERS_DIC<K,I>& D)
{ CLEAR(); v_list = D.v_list; v_list->count++; V = D.V; count = D.count;
  return *this; }

K  key(p_dic_item p) { return LEDA_ACCESS(K,pers_rb_tree::key(p)); }
I inf(p_dic_item p)  { return LEDA_ACCESS(I,pers_rb_tree::inf(p)); }

p_dic_item locate(K k) { return pers_rb_tree::locate(Convert(k),V); }
p_dic_item locate_pred(K k) { return pers_rb_tree::locate_pred(Convert(k),V); }
p_dic_item lookup(K k) { return pers_rb_tree::lookup(Convert(k),V); }

PERS_DIC<K,I>  insert(K k, I i)
{ return PERS_DIC<K,I>(v_list,pers_rb_tree::insert(Convert(k),Convert(i),V)); }

PERS_DIC<K,I>  del(K k)
{ return PERS_DIC<K,I>(v_list,pers_rb_tree::del(Convert(k),V)); }

PERS_DIC<K,I>  change_inf(p_dic_item p, I i)
{ return PERS_DIC<K,I>(v_list,pers_rb_tree::change_inf(p,Convert(i),V)); }

p_dic_item min()          { return pers_rb_tree::min(V); }
p_dic_item max()          { return pers_rb_tree::max(V); }
p_dic_item succ(p_dic_item p)  { return pers_rb_tree::succ(p,V); }
p_dic_item pred(p_dic_item p)  { return pers_rb_tree::pred(p,V); }
int   size()         { return pers_rb_tree::size(V); }
void  print()        { pers_rb_tree::print(V); }
void  draw(DRAW_NODE_FCT f, DRAW_EDGE_FCT g, double x0, double x1, double y, double dy)  { pers_rb_tree::draw(f,g,V,x0,x1,y,dy); }
double get_version() { return ver_num(V); }

};



/*{\Manpage {p_dictionary} {K,I} {Persistent Dictionaries}}*/

template <class K, class I>


class p_dictionary : public handle_base {
/*{\Mdefinition

An instance $D$ of the parameterized data type \name\ is a set
of items (type $p\_dic\_item$). Every item in $D$ contains a key from the
linearly ordered data type $K$, called the key type of $D$, and an information
from data type $I$, called the information type  of $D$. The number of items in
$D$ is called the size of $D$. A dictionary of size zero is called empty.
We use $\<k,i\>$ to denote an item with key $k$ and information
$i$ ($i$ is said to be the information associated with key $k$).  For each
$k \in K$ there is at most one item $\<k,i\> \in D$.

The difference between dictionaries (cf. section \ref{Dictionaries}) and 
persistent dictionaries lies in the fact that update operations performed
on a persistent dictionary $D$ do not change $D$ but create and return a
new dictionary $D\'$. For example, $D$.del($k$) returns the dictionary $D\'$
containing all items $it$ of $D$ with key($it$) $\ne$ $k$. Also, an assignment
$ D1 = D2 $ does not assign a copy of $D2$ (with new items) to $D1$ but the
value of $D2$ itself.
}*/

PERS_DIC<K,I>* ptr() const { return (PERS_DIC<K,I>*) PTR; }

public:
/*{\Mcreation D }*/

 p_dictionary()      { PTR = new PERS_DIC<K,I>; }
/*{\Mcreate creates an instance \var\ of type \name\ and initializes \var\ to an
    empty persistent dictionary.}*/

 p_dictionary(PERS_DIC<K,I>* p) { PTR = (PERS_DIC<K,I>*)p; }
 p_dictionary(const p_dictionary<K,I>& p) : handle_base(p) {}
~p_dictionary()     {}

 p_dictionary<K,I>& operator=(const p_dictionary<K,I>& p)
 { handle_base::operator=(p); return *this; }


/*{\Moperations 3.8 4}*/

K key(p_dic_item it)     { return ptr()->key(it); }
/*{\Mop        returns the key of item $it$.\\
               \precond $it$ $\in$ \var.}*/

I inf(p_dic_item it)     { return ptr()->inf(it); }
/*{\Mop        returns the information of item $it$.\\
	       \precond $it$ $\in$ \var.}*/


p_dic_item locate(K k)      { return ptr()->locate(k); }
p_dic_item locate_pred(K k) { return ptr()->locate_pred(k); }
p_dic_item lookup(K k)      { return ptr()->lookup(k); }
/*{\Mop        returns the item with key $k$ (nil if no such
	       item exists in \var).}*/


p_dictionary<K,I> del(K k)
{ return new PERS_DIC<K,I>
                                       (ptr()->del(k)); }
/*{\Mop        returns $\{x \in \var\ |\ key(x) \ne k\}$.}*/

p_dictionary<K,I> del_item(p_dic_item it);
/*{\Mopl       returns $\{x \in \var\ |\ x \ne it\}$.}*/

p_dictionary<K,I> insert(K k, I i)
{ return new PERS_DIC<K,I>
                                       (ptr()->insert(k,i)); }
/*{\Mop        returns \var.del($k$) $\cup$ $\{\<k,i\>\}$.}*/

p_dictionary<K,I> change_inf(p_dic_item it, I i)
{ return new PERS_DIC<K,I>
                                       (ptr()->change_inf(it,i)); }
/*{\Mopl       returns \var.del\_item($it$) $\cup$
               $\{\<k,i\>\}$, where $k = key(it)$.\\
	       \precond $it$ $\in$ \var.}*/

p_dic_item min()         { return ptr()->min();     }
p_dic_item max()         { return ptr()->max();     }

p_dic_item succ(p_dic_item p) { return ptr()->succ(p);   }
p_dic_item succ(K k)    { return ptr()->locate(k); }
p_dic_item pred(p_dic_item p) { return ptr()->pred(p);   }
p_dic_item pred(K k)    { return ptr()->locate_pred(k); }

p_dic_item first_item()       { return ptr()->min();     }
p_dic_item next_item(p_dic_item p) { return ptr()->succ(p);   }


int   size()        { return ptr()->size();    }
/*{\Mop        returns the size of \var.}*/

bool   empty()       { return ptr()->size()==0; }
/*{\Mop        returns true if \var\ is empty, false otherwise.}*/


void print()       { ptr()->print(); }

void draw(DRAW_NODE_FCT f,DRAW_EDGE_FCT g,double x0,double x1,double y,double dy)  { ptr()->draw(f,g,x0,x1,y,dy); }

}; 

/*{\Mimplementation
Persistent dictionaries are implemented by leaf oriented 
persistent red black trees.
Operations insert, lookup, del\_item, del take time $O(\log^2 n)$, key, inf, 
empty, size and change\_inf take time $O(1)$. The space requirement is
$O(1)$ for each update operation.}*/
 
#endif

