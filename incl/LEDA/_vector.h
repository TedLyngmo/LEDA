/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _vector.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_PAR_VECTOR_H
#define LEDA_PAR_VECTOR_H

//----------------------------------------------------------------------
//  parameterized vectors:  _vector<T>
//----------------------------------------------------------------------
//
// generic vectors with entries of type T
// T must support the following operators and functions:
//
// T::operator+, T::operator-, T::operator*, T::operator/
// T::operator+=, T::operator-=, T::operator*=, T::operator/=
// T::operator=, T::operator==
// T::operator<, T::operator>
// T::operator<<, T::operator>>
// sqrt(), acos()


#include <LEDA/basic.h>
#include <math.h>

template<class T> class _vector
{
  T* v;
  int d;

  void		check_dimensions(const _vector<T>&) const;

public:

  _vector();
  _vector(int);
  _vector(const T&, const T&);
  _vector(const T&, const T&, const T&);
  _vector(const _vector<T>&);

  ~_vector();


  T		length()				const;

  int		dim()					const;
  _vector<T>	norm()					const;

  T		angle(const _vector<T>&)			const;

  _vector<T>&	operator=(const _vector<T>&);

  T&		operator[](int);
  T		operator[](int)				const;

  _vector<T>	operator+(const _vector<T>&)		const;
  _vector<T>	operator-(const _vector<T>&)		const;

  _vector<T>	operator*(const T&)			const;
  _vector<T>	operator/(const T&)			const;

  T		operator*(const _vector<T>&)		const;

  int		operator==(const _vector<T>&)		const;
  int		operator!=(const _vector<T>&)		const;


  friend_vector<T>	operator-(const _vector<T>&);

  friend std::ostream&	operator<<(std::ostream&, const _vector<T>&);
  friend std::istream&	operator>>(std::istream&, _vector<T>&);

  friend int		compare(const _vector<T>&, const _vector<T>&);
};

template<class T>	void
_vector<T>::check_dimensions(const _vector<T>& p)			const
{ if (d != p.d) error_handler(1,"_vector: _vector arguments have different dimensions"); };

template<class T>
_vector<T>::_vector()
{ d = 0; v = nil; };

template<class T>
_vector<T>::_vector(int n)
{
  if (n < 0) error_handler(1,"_vector: negative dimension!");
  d = n;
  if (d > 0) {
    v = new T[d];
    T* pv = v + d;
    while (n--) { *--pv = 0; }
  }
  else { v = nil; }
};

template<class T>
_vector<T>::_vector(const T& x, const T& y)
{
  d = 2;
  v = new T[d];
  v[0] = x;
  v[1] = y;
};

template<class T>
_vector<T>::_vector(const T& x, const T& y, const T& z)
{
  d = 3;
  v = new T[d];
  v[0] = x;
  v[1] = y;
  v[2] = z;
};

template<class T>
_vector<T>::_vector(const _vector<T>& p)
{
  d = p.d;
  if (d > 0) {
    v = new T[d];
    int stop = d;
    T* pv = v + d;
    T* pp = p.v + d;
    while (stop--) *--pv = *--pp;
  }
  else v = nil;
};

template<class T>
_vector<T>::~_vector()
{ if (v) delete v; };

template<class T>	T
_vector<T>::length()						const
{ return sqrt((*this) * (*this)); };

			int
_vector<int>::length()						const
{ error_handler(1,"_vector<int>: length not implemented"); return 0; };

template<class T>	int
_vector<T>::dim()						const
{ return d; };

template<class T>	_vector<T>
_vector<T>::norm()						const
{ return (*this)/length(); };

			_vector<int>
_vector<int>::norm()						const
{
  error_handler(1,"_vector<int>: norm not implemented");
  _vector<int> v;
  return v;
};

template<class T>	T
_vector<T>::angle(const _vector<T>& p)				const
{
  T lv = length();
  T lp = p.length();
  if ((lv == 0) || (lp == 0)) {
    error_handler(1,"_vector: zero length _vector");
  }
  return acos(((*this) * p) / (lv * lp));
};


int _vector<int>::angle(const _vector<int>&)				const
{ error_handler(1,"_vector<int>: angle not implemented"); return 0; };


template<class T>	_vector<T>&
_vector<T>::operator=(const _vector<T>& p)
{
  int n = p.d;
  if (d != n) {
    delete v;
    d = n;
    v = new T[d];
  }
  T* pv = v + d;
  T* pp = p.v + d;
  while (n--) { *--pv = *--pp; }
  return (*this);
};

template<class T>	T&
_vector<T>::operator[](int i)
{
  if ((i < 0) || (i >= d)) {
    error_handler(1,"_vector: index out of range");
  }
  return v[i];
};

template<class T>	T
_vector<T>::operator[](int i)					const
{
  if ((i < 0) || (i >= d)) {
    error_handler(1,"_vector: index out of range");
  }
  return v[i];
};

template<class T>	_vector<T>
_vector<T>::operator+(const _vector<T>& p)			const
{
  check_dimensions(p);
  int n = d;
  _vector<T> result(*this);
  T* pr = result.v + d;
  T* pp = p.v + d;
  while (n--) { *--pr += *--pp; }
  return result;
};

template<class T>	_vector<T>
_vector<T>::operator-(const _vector<T>& q)			const
{
  check_dimensions(q);
  _vector<T> result(*this);
  int n = result.d;
  T* pr = result.v + n;
  T* pq = q.v + q.d;
  while (n--) { *--pr -= *--pq; }
  return result;
};

template<class T>	_vector<T>
_vector<T>::operator*(const T& s)				const
{
  int n = d;
  _vector<T> result(*this);
  T* pr = result.v + d;
  while (n--) { *--pr *= s; }
  return result;
};

template<class T>	_vector<T>
operator*(const T& s, const _vector<T>& p)
{ return p*s; };

template<class T>	_vector<T>
_vector<T>::operator/(const T& s)				const
{
  if (s == 0) error_handler(1,"_vector: division by 0");
  int n = d;
  _vector<T> result(*this);
  T* pr = result.v + d;
  while (n--) { *--pr /= s; }
  return result;
};

template<class T>	T
_vector<T>::operator*(const _vector<T>& p)			const
{
  check_dimensions(p);
  int n = d;
  T result = 0;
  T* pv = v + d;
  T* pp = p.v + d;
  while (n--) result += ((*--pv) * (*--pp));
  return result;
};

template<class T>	int
_vector<T>::operator==(const _vector<T>& p)			const
{
  if (p.d != d) return false;
  int i = d;
  T* pv = v + d;
  T* pp = p.v + d;
  while ((i) && ((*--pv) == (*--pp))) i--;
  return (!i);
};

template<class T>	int
_vector<T>::operator!=(const _vector<T>& w)			const
{ return !(*this == w); };

template<class T>	_vector<T>
operator-(const _vector<T>& p)
{
  _vector<T> zerovec(p.d); // initialized with zeros
  return zerovec - p;
};

template<class T>	ostream&
operator<<(std::ostream& out, const _vector<T>& p)
{
  int i;
  for (i = 0; i < p.d; i++) {
    out << p.v[i] << " ";
  }
  return out;
}

template<class T>	istream&
operator>>(std::istream& in, _vector<T>& p)
{ int i=0;
  while (i < p.d && in >> p.v[i++]);
  return in;
}

template<class T>	int
compare(const _vector<T>& v1, const _vector<T>& v2)
{ v1.check_dimensions(v2);
  T* pv1 = v1.v;
  T* pv2 = v2.v;
  T* stopv1 = v1.v + v1.d;
  while ((pv1 < stopv1) && ((*pv1) == (*pv2))) { *pv1++; *pv2++; };
  if (pv1 == stopv1) return 0;
  else return (*pv1 < *pv2) ? -1 : 1;
}

template<class T>	void
Print(const _vector<T>& v, std::ostream& out)
{ out << v; };

template<class T>	void
Read(_vector<T>& v, std::istream& in)
{ in >> v;  };

#endif
