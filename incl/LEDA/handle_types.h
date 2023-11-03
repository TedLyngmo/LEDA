/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  handle_types.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_HANDLE_H
#define LEDA_HANDLE_H

//------------------------------------------------------------------------------
// handle_base/rep: base classes for handle types string, point, segment,...
//------------------------------------------------------------------------------


class handle_rep  {

friend class handle_base;

protected:

 int  count;

 handle_rep()  { count = 1; }
 virtual ~handle_rep()  {}

public:

 LEDA_MEMORY(handle_rep)

};



class handle_base {

protected:

handle_rep* PTR;

public:

 handle_base() {}
 handle_base(const handle_base& x) { PTR = x.PTR;  PTR->count++; }
~handle_base() { if (PTR && --PTR->count == 0)  delete PTR;}

handle_base& operator=(const handle_base& x)
{ x.PTR->count++;
  if (PTR && --PTR->count == 0)  delete PTR; // left side may be 0
  PTR = x.PTR;
  return *this;
 }

int    refs()  const { return PTR->count; }


friend unsigned long ID_Number(const handle_base& x)
{ return (unsigned long)x.PTR; }

friend bool identical(const handle_base& x, const handle_base& y)
{ return x.PTR == y.PTR; }


#if !defined(__EXPLICIT_DESTRUCTION__)
void  clear() { if (PTR && --PTR->count == 0) delete PTR; }
friend void explicit_destruction(handle_base* p) {  p->clear(); }
#endif


#if !defined(__TEMPLATE_FUNCTIONS__)

// without function templates we define  Copy,Convert,Clear, ..
// functions valid for all handle types derived from handle_base

GenPtr copy() const { PTR->count++; return PTR; }
GenPtr conv() const { return PTR; }
void   clear() { if (PTR && --PTR->count == 0) delete PTR; }

friend inline GenPtr Copy(const handle_base&);
friend inline GenPtr Convert(const handle_base&);
friend inline void   Clear(handle_base&);
friend inline int    compare(const handle_base&, const handle_base&);
friend inline int    Hash(const handle_base&);
friend inline void   Print(const handle_base&, std::ostream&);
friend inline void   Read(handle_base&, std::istream&);

#endif


};




#if !defined(__TEMPLATE_FUNCTIONS__)

inline GenPtr Copy(const handle_base& x)    { return x.copy(); }
inline GenPtr Convert(const handle_base& x) { return x.conv(); }
inline void   Clear(handle_base& x)         { x.clear(); }

inline int compare(const handle_base& x, const handle_base& y)
{ return compare(x.PTR, y.PTR); }

inline int  Hash(const handle_base& x)               { return *(int*)x.PTR; }
inline void Print(const handle_base& x, std::ostream& os) { Print(x,os); }
inline void Read(handle_base& x, std::istream& is)        { Read(x,is); }

#endif


#define LEDA_HANDLE_TYPE(T)  /* historical, not used  anymore */


#endif


