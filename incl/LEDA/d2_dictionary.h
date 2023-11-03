/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  d2_dictionary.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_d2_dictionary_H
#define LEDA_d2_dictionary_H

#include <LEDA/impl/range_tree.h>

typedef rt_item dic2_item;


/*{\Manpage {d2_dictionary} {K1,K2,I} {Two-Dimensional Dictionaries} }*/ 

template<class K1, class K2, class I>
class d2_dictionary : public range_tree {

/*{\Mdefinition
An instance $D$ of the parameterized data type \name\ is a
collection of items ($dic2\_item$). Every item in $D$ contains a key from the
linearly ordered data type $K1$, a key from the linearly ordered data type $K2$,
and an information from data type $I$. $K1$ and $K2$ are called the key types
of $D$, and $I$ is called the information type of $D$. The number
of items in $D$ is called the size of $D$. A two-dimensional dictionary of size
zero is said to be  empty. We use $\<k_1,k_2,i\>$ to denote the item with first
key $k_1$, second key $k_2$, and information $i$. For each pair
$(k_1,k_2) \in K1 \times K2$ there is at most one item $\<k_1,k_2,i\> \in D$.
Additionally to the normal dictionary operations, the data type $d2\_dictionary$
supports rectangular range queries on $K1\times K2$.}*/

  // redefine the virtual functions of class range_tree
 
  void rt_clear_key(GenPtr*& x) const { 
    LEDA_CLEAR(K1,x[0]); LEDA_CLEAR(K2,x[1]); 
  }

  void rt_copy_key(GenPtr*& x) const { 
    LEDA_COPY(K1,x[0]); LEDA_COPY(K2,x[1]); 
  }

  void rt_print_key(int d,GenPtr*& x) const { 
    if( d==0 ) LEDA_PRINT(K1,x[0],cout);
    else       LEDA_PRINT(K2,x[1],cout);
  }
  
  void rt_clear_inf(GenPtr& x) const { LEDA_CLEAR(I,x);}
  void rt_copy_inf(GenPtr& x)  const { LEDA_COPY(I,x);}
  
  int rt_cmp(int d,GenPtr* x,GenPtr* y) const { 
    if( d==0 ) return LEDA_COMPARE(K1,x[0],y[0]);
    else       return LEDA_COMPARE(K2,x[1],y[1]);
  }
  
  range_tree* new_range_tree(int /*dim*/, int l ) { 
    return new d2_dictionary<K1,K2,I>(l); 
  }

  public:
  
/*{\Mcreation D }*/  

d2_dictionary(int l=0) : range_tree(2,l) {}

/*
d2_dictionary();
*/

/*{\Mcreate creates an instance \var\ of type \name\ and initializes \var\ to 
            the empty dictionary.}*/

~d2_dictionary() { clear(); }



/*{\Moperations 2.8 4.5 }*/
 
    K1 key1(dic2_item it)   { return LEDA_ACCESS(K1,it->key(0));  }
/*{\Mop    returns the first key of item $it$.\\
	   \precond $it$ is an item in \var.}*/

    K2 key2(dic2_item it)   { return LEDA_ACCESS(K2,it->key(1));  }
/*{\Mop  returns the second key of item $it$.\\
	 \precond $it$ is an item in \var.}*/

    I inf(dic2_item it)    { return LEDA_ACCESS(I,it->inf());}
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in \var.}*/
    
    dic2_item min_key1() { return range_tree::rt_min(0); }
/*{\Mop   returns the item with minimal first key.}*/

    dic2_item min_key2() { return range_tree::rt_min(1); }
/*{\Mop   returns the item with minimal second key.}*/

    dic2_item max_key1() { return range_tree::rt_max(0); }
/*{\Mop    returns the item with maximal first key.}*/

    dic2_item max_key2() { return range_tree::rt_max(1); }
/*{\Mop   returns the item with maximal second key.}*/
    
    dic2_item insert(K1 x, K2 y, I i) { 
      dic2_item p = new rt_elem(Copy(x),Copy(y),Copy(i));
      return range_tree::insert(p);
     }
/*{\Mopl  associates the information $i$ with the keys $x$
	  and $y$. If there is an item \<$x,y,j$\> in 
	  \var\ then $j$ is replaced by $i$, else a new item 
	  \<$x,y,i$\> is added to $D$. In both cases the
	  item is returned.}*/

    dic2_item lookup(K1 x, K2 y) 
    { rt_elem p(Convert(x), Convert(y),0);
      return range_tree::lookup(&p);
    }
/*{\Mopl   returns the item with keys $x$ and $y$ 
          (nil if no such item exists in \var).}*/
    
list<dic2_item> range_search(K1 x0, K1 x1, K2 y0, K2 y1)  
{  rt_elem p(Convert(x0),Convert(y0),0); rt_elem q(Convert(x1), Convert(y1), 0);
   return range_tree::query(&p,&q); }
/*{\Mopl    returns the list of all items \<$k_1,k_2,i$\> in 
	    \var\ with $x_0\le k_1 \le x_1$ and $y_0\le k_2 \le y_1$.}*/
    
list<dic2_item> all_items() { return range_tree::all_items(); }
/*{\Mop     returns the list of all items of \var.}*/

void del(K1 x, K2 y)  
    { rt_elem p(Convert(x),Convert(y),0);
      range_tree::del(&p);
    }
/*{\Mop       deletes the item with keys $x$ and $y$ 
              from \var.}*/
    
void del_item(dic2_item it) { range_tree::del(it); }
/*{\Mop       removes item $it$ from \var.\\
	     \precond $it$ is an item in \var.}*/

void  change_inf(dic2_item it, I i) {
    LEDA_CLEAR(I,it->inf());
        it->inf() = Copy(i);
    }
/*{\Mopl      makes $i$ the information of item $it$.\\
	     \precond $it$ is an item in \var.}*/

void clear(){range_tree::clear();}
/*{\Mop    makes \var\ the empty d2\_dictionary.}*/

bool empty(){return range_tree::empty();}
/*{\Mop   returns true if \var\ is empty, false otherwise.}*/

int size(){return range_tree::size();}
/*{\Mop    returns the size of \var.}*/


};


// iteration macro
// 
#define forall_dic2_items(x,T)  (T).init_iteration(); forall(x,(T).L )

/*{\Mimplementation
Two-dimensional dictionaries are implemented by dynamic two-dimensional range
trees \cite{Wi85,Lu78} based on BB[$\alpha$] trees. Operations insert, lookup, 
del\_item, del take time $O(\log^2 n)$,  range\_search takes time 
$O(k + \log^2 n)$, where $k$ is the size of the returned list, key, inf, 
empty, size, change\_inf take time $O(1)$, and clear takes time $O(n\log n)$.
Here $n$ is the current size of the dictionary. The space requirement is 
$O(n\log n)$.}*/

#endif
