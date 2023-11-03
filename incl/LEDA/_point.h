/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _point.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

// generic points, having coordinates of type T
// T must support the following operators and functions:
//
// T::operator+, T::operator-, T::operator*, T::operator/
// T::operator+=, T::operator-=, T::operator*=, T::operator/=
// T::operator=, T::operator==
// T::operator<, T::operator>
// T::operator<<, T::operator>>
// sqrt(), sin(), cos(), acos()

#ifndef LEDA_POINT_H
#define LEDA_POINT_H

#include "vector.h"
#include "ctype.h"

//----------------------------------------------------------------------
// points
//----------------------------------------------------------------------

template<class T> class point
{
  friend class segment;
  friend class line;
  friend class circle;

  T x;
  T y;

public:

  point();
  point(const T&, const T&);
  point(const vector<T>&);
  point(const point<T>&);
  ~point();

  point<T>&	operator=(const point<T>&);

//		operator vector<T>();

  T		xcoord()					const;
  T		ycoord()					const;

  T		angle(const point<T>&, const point<T>&)		const;
  T		angle()						const;

  T		distance(const point<T>&)			const;
  T		distance()					const;

  point<T>	translate(const T&, const T&)			const;
  point<T>	translate(const vector<T>&)			const;

  point<T>	rotate(point<T>, const T&)			const;
  point<T>	rotate(const T&)				const;

  point<T>	operator+(const vector<T>&)			const;
  point<T>	operator-(const vector<T>&)			const;

  friend point<T>	operator-(const point<T>&); // unary minus

  friend int		operator==(const point<T>&, const point<T>&);
  friend int		operator!=(const point<T>&, const point<T>&);

  friend std::ostream&	operator<<(std::ostream&, const point<T>&);
  friend std::istream&	operator>>(std::istream&, point<T>&);

  friend void		Print(const point<T>&, std::ostream&);
  friend void		Read(point<T>&,  std::istream&);
/*
  friend int		compare(const point<T>&, const point<T>&);
*/
};

template<class T>
point<T>::point()
{ x = 0; y = 0; };

template<class T>
point<T>::point(const T& px, const T& py)
{ x = px; y = py; };

template<class T>
point<T>::point(const vector<T>& v)
{
  if (v.dim() != 2)
    error_handler(1,"point: vector has dimension other than 2");
  x = v[0]; y = v[1];
};

template<class T>
point<T>::point(const point<T>& p)
{ x = p.x; y = p.y; };

template<class T>
point<T>::~point()
{};

template<class T>	point<T>&
point<T>::operator=(const point<T>& p)
{ x = p.x; y = p.y; return *this; };

template<class T>	T
point<T>::xcoord()						const
{ return x; };

template<class T>	T
point<T>::ycoord()						const
{ return y; };

template<class T>	T
point<T>::angle(const point<T>& q, const point<T>& r)		const
{
  T dx  = q.x - x,
    dy  = q.y - y,
    dxs = r.x - q.x,
    dys = r.y - q.y;

  T norm = sqrt((dx * dx + dy * dy) * (dxs * dxs + dys * dys)),
    cosphi = (dx * dxs + dy * dys) / norm;

  if (cosphi >=  1) return 0;
  if (cosphi <= -1) return M_PI;

  T phi = acos(cosphi);

  if (dx * dys > dy *dxs) return phi;
  else return -phi;
};

template<class T>	T
point<T>::angle()						const
{
  point<T> x_line(1,0);
  return -(x_line.angle(point<T>(0,0),*this));
};

template<class T>	T
point<T>::distance(const point<T>& p)				const
{
  T dx = p.x - x,
    dy = p.y - y;
  return sqrt(dx * dx + dy * dy);
};

template<class T>	T
point<T>::distance()						const
{ return distance(point<T>(0,0)); };

template<class T>	point<T>
point<T>::translate(const T& alpha, const T& d)			const
{
  T cosalpha = cos(alpha),
    sinalpha = sin(alpha);
  return point<T>(x + cosalpha * d, y + sinalpha * d);
};

template<class T>	point<T>
point<T>::translate(const vector<T>& v)				const
{ return point<T>(x + v[0], y + v[1]); };

template<class T>	point<T>
point<T>::rotate(point<T> center, const T& alpha)		const
{
  if (*this == center) return *this;

  T cosalpha = cos(alpha),
    sinalpha = sin(alpha),
    xdiff = x - center.x,
    ydiff = y - center.y;
  return point<T>(cosalpha * xdiff - sinalpha * ydiff + center.x,
                  sinalpha * xdiff + cosalpha * ydiff + center.y);
};

template<class T>	point<T>
point<T>::rotate(const T& alpha)				const
{ return rotate(point<T>(0,0), alpha); };

template<class T>	point<T>
point<T>::operator+(const vector<T>& v)				const
{ return translate(v); };

template<class T>	point<T>
point<T>::operator-(const vector<T>& v)				const
{ return translate(-v); };

template<class T>	point<T>
operator-(const point<T>& v)
{ return point<T>(-v.x,-v.y); };

template<class T>	int
operator==(const point<T>& p, const point<T>& q)
{ return ((p.x == q.x) && (p.y == q.y)); };

template<class T>	int
operator!=(const point<T>& p, const point<T>& q)
{ return !(p == q); };

template<class T>	ostream&
operator<<(std::ostream& out, const point<T>& p)
{
  out << "(" << p.x << "," << p.y << ")";
  return out;
};

template<class T>	istream&
operator>>(std::istream& in, point<T>& p)
{ // syntax: {(} x {,} y {)}

  T x,y;
  char c;

  do in.get(c); while (in && isspace(c));

  if (!in) return in;

  if (c != '(') in.putback(c);

  in >> x;

  do in.get(c); while (isspace(c));
  if (c != ',') in.putback(c);

  in >> y;

  do in.get(c); while (c == ' ');
  if (c != ')') in.putback(c);

  p = point<T>(x,y);
  return in;
};

template<class T>	void
Print(const point<T>& p, std::ostream& out)
{ out << p; };

template<class T>	void
Read(point<T>& p,  std::istream& in)
{ in >> p; };

/*
template<class T>	int
compare(const point<T>& a, const point<T>& b)
{
  int r = compare(a.x, b.x);
  if (r == 0) return compare(a.y, b.y);
  return r;
};
*/



#endif
