/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _array.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/impl/gen_array.h>

#define SWAP(a,b) { GenPtr help = *a; *a = *b; *b = help; }

#define MIN_D 16 

void gen_array::read(istream& in, string s)
{ cout << s;
  for (int i = 0; in && i<sz; i++) read_el(v[i],in);
 }

void gen_array::print(ostream& out, string s, char space) const
{ out << s;
  for (int i=0; i<sz; i++)
  { out << string("%c",space);
    print_el(v[i],out); 
   }
  out.flush();
}

void gen_array::clear() 
{ GenPtr* p = v + sz;
  while (p > v) clear_entry(*--p);
}

void gen_array::init() 
{ GenPtr* p = v + sz;
  while (p > v) init_entry(*--p);
}

gen_array::gen_array()
{ Low = 0;
  High = -1;
  sz = 0;
  v = 0;
  last = 0;
}


gen_array::gen_array(int a, int b)
{ if (a>b) error_handler(1,"bad array size");
  Low = a;
  High = b;
  sz = b-a+1;
  v = new GenPtr[sz];  
  if (v==0) error_handler(99,"array: out of memory");
  last = v+sz-1;
 }

gen_array::gen_array(int n)
{ Low = 0;
  High = n-1;
  sz = n;
  v = new GenPtr[sz];
  if (v==0) error_handler(99,"array: out of memory");
  last = v+sz-1;
}

gen_array::gen_array(const gen_array& a)
{ sz = a.sz;
  Low = a.Low;
  High = a.High;
  v = new GenPtr[sz];
  if (v==0) error_handler(99,"array: out of memory");
  last = v+sz-1;
  GenPtr* vv = v + sz;
  GenPtr* av = a.v + sz;
  while (vv > v) 
  { *--vv = *--av;
    a.copy_entry(*vv);
   }
}

gen_array& gen_array::operator=(const gen_array& a)
{ if (this == &a) return *this;

  if (sz != a.sz)
  { clear();
    sz = a.sz;       
    delete v;
    v = new GenPtr[sz];
    if (v==0) error_handler(99,"array: out of memory");
    last = v+sz-1;
   }

  Low = a.Low;
  High = a.High;
  GenPtr* vv = v + sz;
  GenPtr* av = a.v + sz;
  while (vv > v) 
  { *--vv = *--av;
    copy_entry(*vv);
   }

  return *this;
}

void gen_array::permute(int l, int r)
{
  if (l<Low || l>High || r<l || r>High) 
         error_handler(2,"array::permute illegal range");
 
  l -= Low;
  r -= Low;

  GenPtr* x;
  GenPtr* y;
  GenPtr* stop = v+r+1;

  for(x=v+l;x!=stop;x++) 
  { y = v + rand_int(l,r);  
    SWAP(x,y);  
   }
}


void gen_array::sort(int l, int h) 
{
  GenPtr* left  = v+l-Low;
  GenPtr* right = v+h-Low;
  GenPtr* min_stop = left + MIN_D;

  if (min_stop > right) min_stop = right;

  if (int_type())
    { int_quick_sort(left,right);
      int_insertion_sort(left,right,min_stop);
     }
  else
     { quick_sort(left,right);
       insertion_sort(left,right,min_stop);
      }
 }


void gen_array::quick_sort(GenPtr* l, GenPtr* r)
{ 
  GenPtr* i = l+(r-l)/2; //rand_int(l,r);
  GenPtr* k;

  if (cmp(*i,*r) > 0) SWAP(i,r);
  SWAP(l,i);

  GenPtr  s = *l;

  i = l;
  k = r;

  for(;;)
  { while (cmp(*(++i),s)<0);
    while (cmp(*(--k),s)>0);
    if (i<k) SWAP(i,k) else break;
   }

  SWAP(l,k);

  if (k > l+MIN_D) quick_sort(l,k-1);
  if (r > k+MIN_D) quick_sort(k+1,r);
}



void gen_array::int_quick_sort(GenPtr* l, GenPtr* r)
{ 
  GenPtr* i = l+(r-l)/2; //rand_int(l,r)
  GenPtr* k;

  if (LEDA_ACCESS(int,*i) > LEDA_ACCESS(int,*r)) SWAP(i,r);
  SWAP(l,i);

  int  s = LEDA_ACCESS(int,*l);

  i = l;
  k = r;

  for(;;)
  { while (LEDA_ACCESS(int,*(++i)) < s);
    while (LEDA_ACCESS(int,*(--k)) > s);
    if (i<k) SWAP(i,k) else break;
   }

  SWAP(l,k);

  if (k > l+MIN_D) int_quick_sort(l,k-1);
  if (r > k+MIN_D) int_quick_sort(k+1,r);
}


void gen_array::insertion_sort(GenPtr* l, GenPtr* r, GenPtr* min_stop)
{
  GenPtr* min=l;
  GenPtr* run;
  GenPtr* p;
  GenPtr* q;

  for (run = l+1; run <= min_stop; run++)
      if (cmp(*run,*min) < 0) min = run;

  SWAP(min,l);

  if (r == l+1) return;

  for(run=l+2; run <= r; run++)
  { for (min = run-1; cmp(*run,*min) < 0; min--);
    min++;
    if (run != min) 
    { GenPtr save = *run;
      for(p=run, q = run-1; p > min; p--,q--) *p = *q;
      *min = save;
     }
   }
}



void gen_array::int_insertion_sort(GenPtr* l, GenPtr* r, GenPtr* min_stop)
{
  GenPtr* min=l;
  GenPtr* run;
  GenPtr* p;
  GenPtr* q;

  for (run = l+1; run <= min_stop; run++)
      if (LEDA_ACCESS(int,*run) <  LEDA_ACCESS(int,*min)) min = run;

  SWAP(min,l);

  if (r == l+1) return;

  for(run=l+2; run <= r; run++)
  { for (min = run-1; LEDA_ACCESS(int,*run) < LEDA_ACCESS(int,*min); min--);
    min++;
    if (run != min) 
    { GenPtr save = *run;
      for(p=run, q = run-1; p > min; p--,q--) *p = *q;
      *min = save;
     }
   }
}




int gen_array::binary_search(GenPtr x)
{ int l = 0;
  int r = sz-1;

  if (int_type())
    { int x_i = LEDA_ACCESS(int,x);
      while (l<r)
      { int m = (l+r)/2;
        int m_i = LEDA_ACCESS(int,elem(m));
        if (x_i == m_i) { l = m; break; }
        l = (x_i > m_i) ? m+1 : m-1;
       }
     }
  else
     while (l<r)
     { int m = (l+r)/2;
       int c = cmp(x,elem(m));
       if (c == 0) { l = m; break; }
       l = (c > 0) ? m+1 : m-1;
      }

  return  (cmp(elem(l),x)==0) ? (l+Low) : (Low-1);
}



void gen_array2::init(int a, int b, int c, int d)
{ int i,j;
  for (i=a;i<=b;i++) 
      for (j=c; j<=d; j++) init_entry(row(i)->entry(j));
}

gen_array2::gen_array2(int a, int b, int c, int d) : A(a,b) 
{ Low1  = a;
  High1 = b;
  Low2  = c;
  High2 = d;
  while (b>=a) A.entry(b--) = (GenPtr) new gen_array(c,d); 
}

gen_array2::gen_array2(int a, int b) : A(a) 
{ Low1  = 0;
  High1 = a-1;
  Low2  = 0;
  High2 = b-1;
  while (a>0) A.entry(--a) = (GenPtr) new gen_array(b); 
}

void gen_array2::clear()
{ int i,j;
  for (i=Low1;i<=High1;i++) 
  for (j=Low2;j<=High2;j++) 
  clear_entry(row(i)->entry(j));
}

gen_array2::~gen_array2()
{ for (int i=Low1;i<=High1;i++) delete (gen_array*)A.entry(i); }



void gen_array2::copy_row(gen_array* from, gen_array* to) const
{ GenPtr* p = to->v + to->sz;
  GenPtr* q = from->v + from->sz;
  while (q > from->v) 
  { *--p = *--q;
    copy_entry(*p);
   }
}


gen_array2::gen_array2(const gen_array2& a) : A(a.Low1,a.High1)
{ 
  Low1 = a.Low1;
  High1 = a.High1;
  Low2 = a.Low2;
  High2 = a.High2;

  for(int i=Low1; i<=High1; i++)
  { gen_array* p =  new gen_array(Low2,High2); 
    if (p==0) error_handler(99,"array2: out of memory");
    a.copy_row((gen_array*)a.A.inf(i),p);
    A.entry(i) = p;
   }
}

gen_array2& gen_array2::operator=(const gen_array2& a)
{ 
  clear();

  Low1 = a.Low1;
  High1 = a.High1;
  Low2 = a.Low2;
  High2 = a.High2;

  for(int i=Low1; i<=High1; i++)
  { gen_array* p =  new gen_array(Low2,High2); 
    if (p==0) error_handler(99,"array2: out of memory");
    a.copy_row((gen_array*)a.A.inf(i),p);
    A.entry(i) = p;
   }

  return *this;
}
