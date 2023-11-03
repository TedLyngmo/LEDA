/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  gen_array.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_GEN_ARRAY_H
#define LEDA_GEN_ARRAY_H

//------------------------------------------------------------------------------
// arrays
//------------------------------------------------------------------------------

#include <LEDA/basic.h>


class gen_array {

friend class gen_array2;

protected:
	GenPtr* v;
	GenPtr* last;
	int sz;
        int Low;
        int High;

virtual int  cmp(GenPtr, GenPtr)  const { return 0; }
virtual void print_el(GenPtr&,std::ostream&) const {}
virtual void read_el(GenPtr& ,std::istream&) const {}
virtual void clear_entry(GenPtr&) const {}
virtual void copy_entry(GenPtr&)  const {}
virtual void init_entry(GenPtr&)  const {}
virtual int  int_type() const { return 0; }

  void quick_sort(GenPtr*,GenPtr*);
  void int_quick_sort(GenPtr*,GenPtr*);
  void insertion_sort(GenPtr*,GenPtr*,GenPtr*);
  void int_insertion_sort(GenPtr*,GenPtr*,GenPtr*);

protected:

  int  binary_search(GenPtr);
  void sort(int,int);
  void init();
  void clear();

public:


         gen_array();
         gen_array(int);
         gen_array(int, int);
         gen_array(const gen_array&);
virtual ~gen_array() { if (v) delete[] v; }

   gen_array& operator=(const gen_array&);

   int     size() const     { return sz; }
   int     low()  const     { return Low; }
   int     high() const     { return High; }

   GenPtr& elem(int i)       { return v[i]; }
   GenPtr  elem(int i) const { return v[i]; }

   GenPtr& entry(int i)
   { if (i<Low || i>High)
     error_handler(2,"array::entry index out of range");
     return v[i-Low];
    }

   GenPtr  inf(int i) const
   { if (i<Low || i>High)
     error_handler(2,"array::inf index out of range");
     return v[i-Low];
    }

   void permute(int,int);
   void permute()  { permute(Low,High); }

   void print(std::ostream&,string, char space) const;
   void print(std::ostream& out,char space=' ') const { print(out,"",space);  }
   void print(string s, char space=' ')    const { print(std::cout,s,space);  }
   void print(char space=' ')              const { print(std::cout,"",space); }

   void read(std::istream&,string);
   void read(std::istream& in)      { read(in,"");  }
   void read(string s )        { read(std::cin,s);  }
   void read()                 { read(std::cin,""); }

// Iteration

   GenPtr first_item() { return v; }
   GenPtr last_item()  { return last; }

   void loop_to_succ(GenPtr& p) { p = (p == last) ? 0 : (GenPtr*)p + 1;}
   void loop_to_pred(GenPtr& p) { p = (p == v)    ? 0 : (GenPtr*)p - 1;}

#if defined(__ELSE_SCOPE_BUG__)
   GenPtr* forall_loop_item;
   GenPtr& Forall_Loop_Item() const { return *(GenPtr*)&forall_loop_item; }
#endif

};


/*------------------------------------------------------------------------*/
/* 2 dimensional arrays                                                   */
/*------------------------------------------------------------------------*/


class gen_array2 {
gen_array A;
int Low1, Low2, High1, High2;
virtual void clear_entry(GenPtr& x) const { x = 0; }
virtual void copy_entry(GenPtr& x)  const { x = 0; }
virtual void init_entry(GenPtr& x)  const { x = 0; }

protected:
void clear();
gen_array* row(int i) const { return (gen_array*)A.inf(i); }

void copy_row(gen_array*, gen_array*) const;

public:
void init(int,int,int,int);
int low1()  const { return Low1; }
int low2()  const { return Low2; }
int high1() const { return High1; }
int high2() const { return High2; }
gen_array2(int,int,int,int);
gen_array2(int,int);
virtual ~gen_array2();

gen_array2(const gen_array2& a);

gen_array2& operator=(const gen_array2& a);

};



#if !defined(__TEMPLATE_FUNCTIONS__)
// default I/O and cmp functions

inline void Print(const gen_array& A, std::ostream& out) { A.print(out); }
inline void Read(gen_array& A, std::istream& in) { A.read(in); }
inline int compare(const gen_array&,const gen_array&) { return 0; }
#endif

#endif
