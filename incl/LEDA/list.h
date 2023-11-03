/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  list.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_LIST_H
#define LEDA_LIST_H

/*{\Manpage {list} {E} {Linear Lists}}*/

#include <LEDA/impl/dlist.h>

template<class E> 

class list : public dlist 
{

/*{\Mdefinition
An instance $L$ of the parameterized data type \name\ is a sequence of items
($list\_item$). Each item in $L$ contains an element of data type $E$, called
the element type of $L$. The number of items in $L$ is called the length of $L$.
If $L$ has length zero it is called the empty list. In the sequel $\<x\>$ is 
used to denote a list item containing the element $x$ and $L[i]$ is used to 
denote the contents of list item $i$ in $L$.}*/

int  (*cmp_ptr)(const E&, const E&); // pointer to user supplied cmp function 
int  (*ord_ptr)(const E&);           // pointer to user supplied ord function 
void (*app_ptr)(E&);                 // pointer to user supplied apply function 

int  int_type() const { return LEDA_INT_TYPE(E); }


int  cmp(GenPtr x, GenPtr y) const 
{ if (cmp_ptr) 
     return (*cmp_ptr)(LEDA_ACCESS(E,x),LEDA_ACCESS(E,y));
  else 
     return LEDA_COMPARE(E,x,y); 
}

int  ord(GenPtr x)  const { return (*ord_ptr)(LEDA_ACCESS(E,x)); }
void app(GenPtr& x) const { (*app_ptr)(LEDA_ACCESS(E,x)); }

void clear_el(GenPtr& x)     const { LEDA_CLEAR(E,x); }
void copy_el(GenPtr& x)      const { LEDA_COPY(E,x); }
void print_el(GenPtr& x,ostream& out) const { LEDA_PRINT(E,x,out);  }
void read_el(GenPtr& x,istream& in)   const { E X; Read(X,in); x = Copy(X); }


public:

/*{\Mcreation L }*/ 

list() { cmp_ptr = 0; }
/*{\Mcreate creates  an instance \var\ of type \name\ and initializes it to 
            the empty list.}*/

 list(E a) : dlist(Copy(a)) { cmp_ptr = 0; }

 list(const list<E>& a) : dlist(a) { cmp_ptr = 0; }
virtual ~list() { clear(); }


/*{\Moperations 2 5 }*/

/*{\Mtext
\medskip
{\bf 3.1 Access Operations}
\medskip }*/

int length() const {return dlist::length();}
/*{\Mop      returns the length of $L$.}*/

int size() const {return dlist::size();}
/*{\Mop      returns $L$.length().}*/

bool empty() const {return dlist::empty();}
/*{\Mop      returns true if $L$ is empty, false otherwise.}*/

list_item first() const {return dlist::first();}
/*{\Mop      returns the first item of $L$.}*/

list_item last() const {return dlist::last();}
/*{\Mop      returns the last item of $L$.}*/

list_item item(int i) const {return dlist::get_item(i);}
/*{\Mop      returns the item at position $i$ (the first position is 0).
             \precond $i < L$.length().}*/

list_item succ(list_item it) const {return dlist::succ(it);}
/*{\Mop      returns the successor item of item $it$, nil if
	     $it=L$.last().\\
	     \precond $it$ is an item in $L$.}*/

list_item pred(list_item it) const {return dlist::pred(it);}
/*{\Mop      returns the predecessor item of item $it$, nil if
	     $it=L$.first().\\
	     \precond $it$ is an item in $L$.}*/

list_item cyclic_succ(list_item it) const {return dlist::cyclic_succ(it);}
/*{\Mop      returns the cyclic successor of item $it$, i.e.,
	     $L$.first() if $it = L$.last(), $L$.succ($it$) otherwise.}*/

list_item cyclic_pred(list_item it) const
{return dlist::cyclic_pred(it);}
/*{\Mop      returns the cyclic predecessor of item $it$, i.e,
             $L$.last() if $it = L$.first(), $L$.pred($it$) otherwise.}*/

list_item search(E x) const   
{ ((GenPtr&)cmp_ptr) = 0; return dlist::search(Convert(x)); }
/*{\Mop      returns the first item of $L$ that contains $x$,
	     nil if $x$ is not an element of $L$.\\
	     \precond{compare has to be defined for type $E$.} }*/

E  contents(list_item it) const { return LEDA_ACCESS(E,dlist::contents(it)); }
/*{\Mop      returns the contents $L[it]$ of item $it$.\\
	     \precond $it$ is an item in $L$.}*/

E  inf(list_item it)      const { return contents(it); }
/*{\Mop      returns $L$.contents($it$).}*/


E  head()                const { return LEDA_ACCESS(E,dlist::head()); }
/*{\Mop      returns the first element of $L$, i.e. the contents
	     of $L$.first().\\
	     \precond $L$ is not empty.}*/

E  tail()                const { return LEDA_ACCESS(E,dlist::tail()); }
/*{\Mop      returns the last element of $L$, i.e. the contents
	     of $L$.last().\\
	     \precond $L$ is not empty.}*/

int       rank(E x)   const   { return dlist::rank(Convert(x)); }
/*{\Mop      returns the rank of $x$ in $L$, i.e. its first
	     position in $L$ as an integer from [1\dots $|L|$]
	     (0 if $x$ is not in $L$). }*/



/*{\Mtext
\medskip
{\bf 3.2 Update Operations}
\medskip }*/

list_item push(E x)   { return dlist::push(Copy(x));}
/*{\Mop      adds a new item $\<x\>$ at the front of $L$ and 
	     returns it ( $L$.insert($x,L$.first$(),before$) ).}*/

list_item append(E x) { return dlist::append(Copy(x));}
/*{\Mop      appends a new item $\<x\>$ to $L$ and returns
             it ( $L$.insert($x,L$.last$(),after$) ).}*/

list_item insert(E x, list_item l, int dir=0)
                         { return dlist::insert(Copy(x),l,dir); }
/*{\Mopl     inserts a new item $\<x\>$ after (if $dir=after$)
	     or before (if $dir=before$) 
	     item $it$ into $L$ and
	     returns it
	     (here $after$ and $before$
	     are predefined $int$ constants).\\ 
	     \precond $it$ is an item in $L$.}*/


GenPtr forall_loop_test(GenPtr it, E& x) const
{ if (it) x = contents(list_item(it));
  return it;
 }


E  pop() { GenPtr x=dlist::pop(); 
              E   y=LEDA_ACCESS(E,x); 
              Clear(LEDA_ACCESS(E,x)); 
              return y; }
/*{\Mop      deletes the first item from $L$ and returns its
  	     contents.\\
	     \precond $L$ is not empty.}*/


E  Pop() { GenPtr x=dlist::Pop(); 
              E   y=LEDA_ACCESS(E,x); 
              Clear(LEDA_ACCESS(E,x)); 
              return y; }
/*{\Mop      deletes the last item from $L$ and returns its
	     contents.\\
	     \precond $L$ is not empty.}*/


E  del_item(list_item it) { GenPtr x=dlist::del(it);
                              E   y=LEDA_ACCESS(E,x);
                              Clear(LEDA_ACCESS(E,x));
                              return y; }
/*{\Mop      deletes the item $it$ from $L$ and returns its
	     contents $L[it]$.\\
	     \precond $it$ is an item in $L$.}*/


E  del(list_item a)           { return del_item(a); }

void  assign(list_item it, E x) { dlist::assign(it,Copy(x));}
/*{\Mop      makes $x$ the contents of item $it$.\\
	     \precond $it$ is an item in $L$.}*/


void  conc(list<E>& L1)              { dlist::conc((dlist&)L1); }
/*{\Mop      appends list $L_1$ to list $L$ and makes $L_1$ the
	     empty list.\\
	     \precond: $L \ne L_1$}*/


void  split(list_item it, list<E>& L1, list<E>& L2)
                                 { dlist::split(it,(dlist&)L1,(dlist&)L2);}
/*{\Mopl     splits $L$ at item $it$ into lists $L1$ and $L2$. More precisely,
	     if $it \ne nil$ and $ L = x_1,\dots,x_{k-1},it,x_{k+1},\dots,x_n$
             then $L1 = x_1,\dots,x_{k-1}$ and $L2 = it,x_{k+1},\dots,x_n$. If
             $it = nil$ then $L1$ is made empty and $L2$ a copy of $L$. Finally
             $L$ is made empty if it is not identical to $L1$ or $L2$.\\
	     \precond $it$ is an item of $L$ or $nil$.}*/


void  split(list_item it, list<E>& L1, list<E>& L2, int dir)
                                 { dlist::split(it,(dlist&)L1,(dlist&)L2,dir);}
/*{\Mopl     splits $L$ at item $it$ into lists $L1$ and $L2$. Item $it$ 
             becomes the first item of $L2$ if $dir==0$ and the last item
             of $L1$ otherwise.\\ \precond $it$ is an item of $L$.}*/


void  sort(int (*cmp)(const E&, const E&)) { cmp_ptr = cmp; dlist::sort(); }
/*{\Mopl      sorts the items of $L$ using the ordering defined
	     by the compare function $cmp : E\times E \longrightarrow int$,
	     with\\
 	     $$cmp(a,b)\ \ \cases{< 0,  &if $a < b$\cr
		                  = 0,  &if $a = b$\cr
		                  > 0,  &if $a > b$\cr}$$
	     More precisely, if $(in_1,\dots,in_n)$ and 
	     $(out_1,\dots,out_n)$ denote the values of $L$
	     before and after the call of sort, then
	     $cmp(L[out_j], L[out_{j+1}]) \le 0$ for
	     $1\le j<n$ and there is a
	     permutation
	     $\pi$ of $[1..n]$ such that $out_i = in_{\pi_i}$ for
	     $1 \le i \le n$
	     .}*/

void  sort()                            { cmp_ptr = 0; dlist::sort(); }
/*{\Mop      sorts the items of $L$ using the default ordering of type $E$,
             i.e., the linear order defined by function 
             $int$ compare$(const\ E\&,\ const\ E\&)$. }*/


list_item min()                 { cmp_ptr = 0; return dlist::min(); }
/*{\Mop      returns the item with the minimal contents with respect
             to the default linear order of type $E$. }*/

list_item min(int (*cmp)(const E&, const E&)) 
                                { cmp_ptr = cmp; return dlist::min(); }
/*{\Mopl      returns the item with the minimal contents with respect
             to the linear order defined by compare function $cmp$. }*/

list_item max()                 { cmp_ptr = 0; return dlist::max(); }
/*{\Mop      returns the item with the maximal contents with respect
             to the default linear order of type $E$. }*/

list_item max(int (*cmp)(const E&, const E&)) 
                                { cmp_ptr = cmp; return dlist::max(); }
/*{\Mopl      returns the item with the maximal contents with respect
             to the linear order defined by compare function $cmp$. }*/



void  apply(void (*f)(E& x))   { app_ptr = f; dlist::apply(); }
/*{\Mop      for all items $\<x\>$ in $L$ function $f$ is
	     called with argument $x$ (passed by reference).}*/


void  bucket_sort(int i, int j, int (*f)(const E&))
                                     { ord_ptr = f; dlist::bucket_sort(i,j); }
/*{\Mopl     sorts the items of $L$ using bucket sort,
	     where $f : E \longrightarrow int$ with $f(x) \in [i..j]$ for
	     all elements $x$ of $L$. The sort is stable,
	     i.e., if $f(x)=f(y)$ and $\<x\>$ is before $\<y\>$ in
	     $L$ then $\<x\>$ is before $\<y\>$ after the sort.}*/

void permute() {dlist::permute();}
/*{\Mop      the items of $L$ are randomly permuted.}*/

void clear() {dlist::clear();}
/*{\Mop      makes $L$ the empty list. }*/

/*{\Mtext
\bigskip
{\bf 3.3 Input and Output} }*/

void read(istream& I, char delim = '\n') { dlist::read(I,delim); }
/*{\Mopl     reads a sequence of objects of type $E$ terminated
	     by the delimiter $delim$ from the input stream $I$
	     using the overloaded $Read$ function 
	     (section \ref{Overloading}).
	     $L$ is made a list of appropriate length and the
	     sequence is stored in $L$.}*/

void read(char delim = '\n') { dlist::read(delim); }
/*{\Mop      calls $L$.read($cin$, $delim$) to read $L$ from
	     the standard input stream $cin$.}*/

void read(string s,char delim = '\n') {dlist::read(s,delim);}
/*{\Mopl     As above, but uses string $s$ as a prompt.}*/

void print(ostream& O, char space = ' ') const { dlist::print(O,space); }
/*{\Mopl     prints the contents of list $L$ to the output
	     stream $O$ using the overload $Print$ function
	     (cf.~section \ref{Overloading}) to print each element. The 
	     elements are separated by the character $space$.}*/

void print(char space = ' ') const { dlist::print(space); }
/*{\Mop      calls $L$.print($cout$, $space$) to print $L$ on
	     the standard output stream $cout$.}*/

void print(string s,char space = ' ') const { dlist::print(s,space); }
/*{\Mopl     As above, but uses string $s$ as a header.}*/


/*{\Mtext
\medskip
{\bf 3.4 Iterators }

Each list $L$ has a special item called the iterator of $L$. There 
are operations to read the current value or the contents of this iterator,
to move it (setting it to its successor or predecessor) and to test whether 
the end (head or tail) of the list is reached. If the iterator contains a 
$list\_item \neq nil$ we call this item the position of the iterator. 
Iterators are used to implement iteration statements on lists. }*/

void set_iterator(list_item it) const {dlist::set_iterator(it);}
/*{\Mop      assigns item $it$ to the iterator.\\
	     \precond $it$ is in $L$ or $it$ = nil. }*/

void init_iterator()  { dlist::init_iterator(); }
/*{\Mop      assigns nil to the iterator.}*/

list_item get_iterator() { return dlist::get_iterator(); }
/*{\Mop      returns the current value of the iterator.}*/

list_item move_iterator(int dir) const { return dlist::move_iterator(dir); }
/*{\Mopl     moves the iterator to its successor (predecessor)
	     if $dir=forward$ ($backward$) and to the first 
	     (last) item if the iterator is undefined (= nil), returns
	     the value of the iterator.}*/

bool current_element(E& x) const {GenPtr y; bool b=dlist::current_element(y);
                                     if (b) x = LEDA_ACCESS(E,y); return b; }
/*{\Mop      if the iterator is defined ($\neq$ nil) its contents is
             assigned to $x$ and true is returned else false
	     is returned.}*/


bool next_element(E& x)   const  {GenPtr y; bool b = dlist::next_element(y);
                                     if (b) x = LEDA_ACCESS(E,y); return b; }
/*{\Mop      calls $L$.move\_iterator($forward$) and then  
	     returns $L$.current\_element($x$). }*/

bool prev_element(E& x)   const  {GenPtr y; bool b = dlist::prev_element(y);
                                     if (b) x = LEDA_ACCESS(E,y); return b; }
/*{\Mop      calls $L$.move\_iterator($backward$) and then 
	     returns $L$.current\_element($x$). }*/


/*{\Mtext
\bigskip
{\bf 3.5 Operators} }*/

list<E>& operator=(const list<E>& L1) 
{ dlist::operator=(L1); return *this;}
/*
list<E>& operator=(const list& L1) 
*/
/*{\Mbinop   The assignment operator makes $L$ a copy of
	     list $L_1$. More precisely if $L_1$ is the sequence
	     of items $x_1, x_2, \dots , x_n$ then $L$ is made a
	     sequence of items $y_1, y_2, \dots , y_n$ with
	     $L[y_i] = L_1[x_i]$ for $1 \le i \le n$.}*/


#if !defined(__sgi)
list<E>  operator+(const list<E>& a) 
{ dlist L = *(dlist*)this + *(dlist*)&a; return *(list<E>*)&L; }
#endif

list_item operator+=(E x)   { return append(x); }
list_item operator[](int i) const   { return get_item(i); }

E&  operator[](list_item it) { return LEDA_ACCESS(E,dlist::entry(it)); }
/*{\Marrop   returns a reference to the contents of $it$.}*/

E   operator[](list_item p) const { return contents(p); }

/*
friend void Print(const list<E>& L, ostream& out) { L.print(out); }
friend void Read(list<E>& L, istream& in)         { L.read(in); }
*/

}; 


//------------------------------------------------------------------------------
// Iteration Macros
//------------------------------------------------------------------------------

#define forall_list_items(a,L) forall_items(a,L)

#define Forall_list_items(a,L) for( a=(L).last();  a ; a=(L).pred(a) )


/*{\Mtext
\bigskip
{\bf 3.6 Iterations Macros}

{\bf forall\_items}($it,L$)       
$\{$ ``the items of $L$ are successively assigned to $it$'' $\}$

{\bf forall}($x,L$)       
$\{$ ``the elements of $L$ are successively assigned to $x$'' $\}$ }*/



/*{\Mimplementation
The data type list is realized by doubly linked linear lists. All operations
take constant time except for the following operations: search and rank take 
linear time $O(n)$, item($i$) takes time $O(i)$, bucket\_sort takes time 
$O(n + j - i)$ and sort takes time $O(n\cdot c\cdot\log n$) where $c$ is the 
time complexity of the compare function. $n$ is always the current length of 
the list.}*/

#endif

