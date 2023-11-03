/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  param_types.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_PARAM_TYPE_H
#define LEDA_PARAM_TYPE_H

#include <istream>
#include <ostream>

//----------------------------------------------------------------------------
// Parameterized data types store copies of values of arbitrary types T
// in a single data field of type void* (GenPtr). There are two cases
// which have to be treated differently here.
//
// If T is a "big type" (sizeof(T) > sizeof(GenPtr)) we make a copy in the
// dynamic memory using the new operator and store a pointer to this copy.
// If T is a "small type" (sizeof(T) <= sizeof(GenPtr)) we avoid the
// overhead of an extra level of indirection by copying the value directly
// into the void* data field. This is done by using the "in-place" variant
// of the new operator. A good description of this feature can be found
// in  R.B. Murray: "C++ Strategies and Tactics" (chapter 9.5). We would
// like to thank all people contributing to this solution by sending us
// problem reports and helpful suggestions (see LEDA/ACKNOWLEDGEMENTS).
//
// Both cases are handled by the "Copy" function template defined in this file.
// Similarly, the two cases (small or big type) have to be treated differently,
// when creating ("Create"), destroying ("Clear"), and accessing ("Access")
// data of a parameterized type. Note that destruction of small types uses an
// an explicit destructor call which is not supported by some compilers.
//
// More details on the implementation of parameterized data types in LEDA
// will be given in the text book "LEDA: A Platform ..."  by K. Mehlhorn
// and S. Naeher (to appear in the begining of 1996)
//
//----------------------------------------------------------------------------



#if defined (__TEMPLATE_FUNCTIONS__)

// Most c++ implementations provide an in-place new operator in <new.h>.
// However, it is not always declared inline. So, for efficiency reasons,
// we define our own (inline) version. It has been suggested to overload
// the new operator using a third dummy argument to avoid conflicts with
// user defined variants, e.g  void* operator new(size_t, void*, int)
// Because we had not the time to test this with enough compilers and
// do not know if there is really a problem here we did not include it
// in the curent version. However, you can switch to this solution by
// turning on the AVOID_CONFLICTS_WITH_NEW flag.
//
// For compilers not supporting explicit destructor calls or in-place new
// we use the old method based on casting and assignment. Then 0 must be
// legal value on the left side of assignments for all one-word types.


#if defined(__INPLACE_NEW__)

inline void Avoid_Unused_Warning(GenPtr) {}

// work-around for a g++ bug:  cast the place operand to a T* pointer

#if defined(AVOID_CONFLICTS_WITH_NEW)
inline void* operator new(size_t, void* where, int) { return where; }
#define COPY_INTO_WORD(T,x,p) Avoid_Unused_Warning(new((T*)&p,0) T(x))
#define CONSTRUCT_WORD(T,p)   Avoid_Unused_Warning(new((T*)&p,0) T)
#else
//inline void* operator new(size_t, void* where) { return where; }
#define COPY_INTO_WORD(T,x,p) Avoid_Unused_Warning(new((T*)&p) T(x))
#define CONSTRUCT_WORD(T,p)   Avoid_Unused_Warning(new((T*)&p) T)
#endif

#else

#define COPY_INTO_WORD(T,x,p) *(T*)&p = x;
#define CONSTRUCT_WORD(T,p)   { T x; COPY_INTO_WORD(T,x,p); }

#endif


#if defined(__EXPLICIT_DESTRUCTION__)

#if !defined(__WATCOMC__)
#define DESTROY_WORD(T,p)     p->T::~T();
#else
#define DESTROY_WORD(T,p)     p->~T();
#endif

#else

inline void explicit_destruction(GenPtr) {}
#define DESTROY_WORD(T,p)  explicit_destruction((T*)p)

#endif


//----------------------------------------------------------------------------
// Function templates for Copy, Create, Clear, Convert, Access, etc. and
// default implementations of Print, Read, compare, and Hash. When using
// a compiler that does not support function templates (g++-2.5.8) these
// functions have to be created for class types by a call of the LEDA_TYPE_
// PARAMETER macro defined at end of this file. Most of the templates
// distinguish the two cases that the actual type has size greater than the
// size of a pointer or not, i.e., is a big or small type. Note that this
// does not cause any overhead at run time since the right case can be
// determined at compile time.
//----------------------------------------------------------------------------


template<class T>
inline GenPtr Copy(const T& x)
{ GenPtr p=0;
  if (sizeof(T) <= sizeof(GenPtr)) COPY_INTO_WORD(T,x,p);
  if (sizeof(T) >  sizeof(GenPtr)) p = new T(x);
  return p;
 }

template <class T>
inline GenPtr Create(const T*)
{ GenPtr p=0;
  if (sizeof(T) <= sizeof(GenPtr)) CONSTRUCT_WORD(T,p);
  if (sizeof(T) >  sizeof(GenPtr)) p = new T;
  return p;
 }

template <class T>
inline void Clear(T& x)
{ T* p = &x;
  if (sizeof(T) <= sizeof(GenPtr)) DESTROY_WORD(T,p);
  if (sizeof(T) >  sizeof(GenPtr)) delete p;
}


template <class T>
inline GenPtr Convert(const T& x)
{ GenPtr p=0;
  if (sizeof(T) == sizeof(GenPtr)) p = *(GenPtr*)&x;
  if (sizeof(T) <  sizeof(GenPtr)) *(T*)&p = x;
  if (sizeof(T) >  sizeof(GenPtr)) p = (GenPtr)&x;
  return p;
 }

template <class T>
inline T& Access(const T*, const GenPtr& p)
{ if (sizeof(T) <= sizeof(GenPtr))
     return *(T*)&p;
  else
     return *(T*)p;
 }

template <class T>
inline int   Int_Type(const T*)  { return 0; }

template <class T>
inline const char* Type_Name(const T*) { return "unknown"; }


//---------------------------------------------------------------------------
// default definitions for Print and Read
//---------------------------------------------------------------------------

template <class T>
inline void Print(const T& x, std::ostream& out)
{ if (sizeof(T) <= sizeof(GenPtr)) out << *(GenPtr*)&x;
  if (sizeof(T) >  sizeof(GenPtr)) out << "(object at " << GenPtr(&x) << ")";
 }

template <class T>
inline void Read(T&, std::istream&) { error_handler(1,"Read undefined"); }


//---------------------------------------------------------------------------
// default definitions for compare and hash
//---------------------------------------------------------------------------

template <class T>
inline int compare(const T& x, const T& y)
{ if (sizeof(T) > sizeof(GenPtr)) error_handler(1,"compare undefined");
  return *(long*)&x - *(long*)&y;
 }

template <class T>
inline int Hash(const T& x)
{ if (sizeof(T) > sizeof(GenPtr)) error_handler(1,"Hash undefined");
  return *(int*)&x;
 }


#else

//---------------------------------------------------------------------------
// Without function templates we have to provide functions for all
// pointer types (GenPtr versions).
//---------------------------------------------------------------------------

inline GenPtr  Copy(const GenPtr& x)                 { return x; }
inline GenPtr  Create(GenPtr)                        { return 0; }
inline void    Clear(GenPtr)                         {}
inline GenPtr  Convert(GenPtr x)                     { return x; }
inline GenPtr& Access(const GenPtr, const GenPtr& p) { return *(GenPtr*)&p; }

inline int   Int_Type(GenPtr)   { return 0; }
inline const char* Type_Name(GenPtr)  { return "unknown"; }

inline int compare(const GenPtr& x, const GenPtr& y) { return long(x)-long(y);}
inline int Hash(const GenPtr& x)  { return *(int*)&x; }
inline void Print(const GenPtr& x, std::ostream& out) { out << x; }
inline void Read(GenPtr, std::istream&) { }

#endif


//----------------------------------------------------------------------------
// Specializations for some built-in types (to be extended as needed) for
// compilers not supporting function templates at all or having particular
// problems with function templates (watcomc, lucid)
//----------------------------------------------------------------------------

#if !defined(__TEMPLATE_FUNCTIONS__) || defined(__WATCOMC__) || defined(__lucid) || defined(__GNUC__)

inline GenPtr Copy(const char& x)  { GenPtr p; *(char*)&p = x;  return p; }
inline GenPtr Copy(const short& x) { GenPtr p; *(short*)&p = x; return p; }
inline GenPtr Copy(const int& x)   { GenPtr p; *(int*)&p = x;   return p; }
inline GenPtr Copy(const long& x)  { GenPtr p; *(long*)&p = x;  return p; }
inline GenPtr Copy(const float& x) { GenPtr p; *(float*)&p = x; return p; }

inline GenPtr Convert(const char& x)  { return Copy(x); }
inline GenPtr Convert(const short& x) { return Copy(x); }
inline GenPtr Convert(const int& x)   { return Copy(x); }
inline GenPtr Convert(const long& x)  { return Copy(x); }
inline GenPtr Convert(const float& x) { return Copy(x); }

inline GenPtr Create(const char*)  { return 0; }
inline GenPtr Create(const short*) { return 0; }
inline GenPtr Create(const int*)   { return 0; }
inline GenPtr Create(const long*)  { return 0; }
inline GenPtr Create(const float*) { return 0; }

inline void   Clear(char&)  {}
inline void   Clear(short&) {}
inline void   Clear(int&)   {}
inline void   Clear(long&)  {}
inline void   Clear(float&) {}

inline char&  Access(const char*, const GenPtr& p) { return *(char*)&p;  }
inline short& Access(const short*,const GenPtr& p) { return *(short*)&p; }
inline int&   Access(const int*,  const GenPtr& p) { return *(int*)&p;   }
inline long&  Access(const long*, const GenPtr& p) { return *(long*)&p;  }
inline float& Access(const float*,const GenPtr& p) { return *(float*)&p; }

#endif



//----------------------------------------------------------------------------
// A "special" specialization for "double" using the LEDA memory management
// if sizeof(double) > sizeof(GenPtr).
//----------------------------------------------------------------------------

inline GenPtr Copy(const double& x)
{ GenPtr p;
  if (sizeof(double) > sizeof(GenPtr))
    { p = allocate_bytes(sizeof(double));
      *(double*)p = x; }
  else
      *(double*)&p = x;
  return p;
 }

inline GenPtr Create(const double*)
{ GenPtr p;
  if (sizeof(double) > sizeof(GenPtr))
    { p = allocate_bytes(sizeof(double));
      *(double*)p = 0; }
  else
      *(double*)&p = 0;
  return p;
 }

inline void Clear(double& x)
{ if (sizeof(double) > sizeof(GenPtr)) deallocate_bytes(&x,sizeof(double)); }

inline GenPtr Convert(const double& x)
{ return (sizeof(double) <= sizeof(GenPtr)) ? *(GenPtr*)&x : GenPtr(&x); }

inline double& Access(const double*, const GenPtr& p)
{ return (sizeof(double) <= sizeof(GenPtr)) ? *(double*)&p : *(double*)p; }




//----------------------------------------------------------------------------
// integer type or not ?
//----------------------------------------------------------------------------

inline int Int_Type(const int*)  { return 1; }
inline int Int_Type(const long*) { return sizeof(long) == sizeof(int); }


//----------------------------------------------------------------------------
// id numbers
//----------------------------------------------------------------------------

inline unsigned long ID_Number(GenPtr x) { return (unsigned long)x; }
inline unsigned long ID_Number(char   x) { return (unsigned long)x; }
inline unsigned long ID_Number(short  x) { return (unsigned long)x; }
inline unsigned long ID_Number(int    x) { return (unsigned long)x; }
inline unsigned long ID_Number(long   x) { return (unsigned long)x; }


//----------------------------------------------------------------------------
// type names
//----------------------------------------------------------------------------

inline const char* Type_Name(const char*  ) { return "char";   }
inline const char* Type_Name(const short* ) { return "short";  }
inline const char* Type_Name(const int*   ) { return "int";    }
inline const char* Type_Name(const long*  ) { return "long";   }
inline const char* Type_Name(const float* ) { return "float";  }
inline const char* Type_Name(const double*) { return "double"; }



//----------------------------------------------------------------------------
// pre-defined linear orders, I/O, and hash functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// compare
//----------------------------------------------------------------------------

inline int compare(const char& x, const char& y)
{ return int(x)-int(y); }

inline int compare(const int& x, const int& y)
{ if (x < y)  return -1;
  else if (x > y)  return  1;
  else return 0;
}

inline int compare(const long& x, const long& y)
{ if (x < y)  return -1;
  else if (x > y)  return  1;
  else return 0;
}

inline int compare(const double& x, const double& y)
{ if (x < y)  return -1;
  else if (x > y)  return  1;
  else return 0;
}

inline int compare(const float& x, const float& y)
{ if (x < y)  return -1;
  else if (x > y)  return  1;
  else return 0;
}

#if defined(__BUILTIN_BOOL__)
inline int compare(const bool& x, const bool& y) {return char(x)-char(y);}
#endif


//----------------------------------------------------------------------------
// hashing
//----------------------------------------------------------------------------

inline int Hash(const char&   x) { return int(x); }
inline int Hash(const short&  x) { return int(x); }
inline int Hash(const int&    x) { return int(x); }
inline int Hash(const long&   x) { return int(x); }
inline int Hash(const float&  x) { return int(x); }
inline int Hash(const double& x) { return int(x); }
#if defined(__BUILTIN_BOOL__)
inline int Hash(const bool& x) { return int(x); }
#endif


//----------------------------------------------------------------------------
// output
//----------------------------------------------------------------------------

inline void Print(const char& x, std::ostream& out  ) { out << x; }
inline void Print(const short& x, std::ostream& out ) { out << x; }
inline void Print(const int& x, std::ostream& out   ) { out << x; }
inline void Print(const long& x, std::ostream& out  ) { out << x; }
inline void Print(const float& x, std::ostream& out ) { out << x; }
inline void Print(const double& x, std::ostream& out) { out << x; }
#if defined(__BUILTIN_BOOL__)
inline void  Print(const bool& x, std::ostream& out) { out << (x ? "true":"false"); }
#endif


//----------------------------------------------------------------------------
// input
//----------------------------------------------------------------------------

inline void Read(char& x,  std::istream& in) { in >> x; }
inline void Read(short& x, std::istream& in) { in >> x; }
inline void Read(int&  x,  std::istream& in) { in >> x; }
inline void Read(long& x,  std::istream& in) { in >> x; }
inline void Read(float& x, std::istream& in) { in >> x; }
inline void Read(double& x,std::istream& in) { in >> x; }
#if defined(__BUILTIN_BOOL__)
inline void  Read(bool&,  std::istream&) {}
#endif



//----------------------------------------------------------------------------
// some useful macros
// (used in data type templates)
//----------------------------------------------------------------------------

#if defined(__TEMPLATE_FUNCTIONS__) && !defined(__WATCOMC__)

// this should work with all compilers, however ...

#define LEDA_ACCESS(T,p)     Access((const T*)0,p)
#define LEDA_COPY(T,x)       x=Copy(LEDA_ACCESS(T,x))
#define LEDA_CREATE(T,x)     x=Create((const T*)0)
#define LEDA_CLEAR(T,x)      Clear(LEDA_ACCESS(T,x))
#define LEDA_INT_TYPE(T)     Int_Type((const T*)0)
#define LEDA_TYPE_NAME(T)    Type_Name((const T*)0)

#else

#if !defined(__TEMPLATE_FUNCTIONS__)

#define LEDA_ACCESS(T,x)   (T&)(Access((T*)0,x))
#define LEDA_COPY(T,x)     x=Copy(LEDA_ACCESS(T,x))
#define LEDA_CREATE(T,x)   { T y; x=Copy(y); }
#define LEDA_CLEAR(T,x)    Clear(LEDA_ACCESS(T,x))
#define LEDA_INT_TYPE(T)   Int_Type((T*)0)
#define LEDA_TYPE_NAME(T)  Type_Name((T*)0)

#else

// __WATCOMC__
#define LEDA_ACCESS(T,p)   Access((const T*)0,(const GenPtr&)p)
#define LEDA_COPY(T,x)     x=Copy((const T&)LEDA_ACCESS(T,x))
#define LEDA_CREATE(T,x)   x=Create((const T*)0)
#define LEDA_CLEAR(T,x)    Clear(LEDA_ACCESS(T,x))
#define LEDA_INT_TYPE(T)   Int_Type((const T*)0)
#define LEDA_TYPE_NAME(T)  Type_Name((const T*)0)

#endif

#endif


#if defined(__ATTCFRONT__)
#undef  LEDA_CREATE
#define LEDA_CREATE(T,x)   { T y; x=Copy(y); }
#endif


#define LEDA_COMPARE(T,x,y)  compare(LEDA_ACCESS(T,x),LEDA_ACCESS(T,y))
#define LEDA_HASH(T,x)       Hash(LEDA_ACCESS(T,x))
#define LEDA_PRINT(T,x,out)  Print(LEDA_ACCESS(T,x),(std::ostream&)out)
#define LEDA_READ(T,x,in)    Read(LEDA_ACCESS(T,x),(std::istream&)in)



//----------------------------------------------------------------------------
// LEDA_TYPE_PARAMETER MACRO
//
// has to be called for any "big type" T before using T as an actual type
// argument in parameterized data types if function templates cannot be used.
//----------------------------------------------------------------------------

#define LEDA_TYPE_PARAMETER(T)\
inline GenPtr Copy(const T& x)     { return new T(x); }\
inline GenPtr Create(const T*)     { return new T; }\
inline void   Clear(T& x)          { delete &x; }\
inline GenPtr Convert(const T& x)  { return (GenPtr)&x; }\
inline T&     Access(const T*, const GenPtr& p)  { return *(T*)p; }\
inline char*  Type_Name(const T*)  { return STRINGIZE(T); }



#endif
