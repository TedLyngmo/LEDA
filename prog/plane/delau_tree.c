#include <LEDA/impl/delaunay_tree.h>
#include <LEDA/plane.h>


const double R = 1000;  // "length of inifinite rays"

void infi_pt(double x1, double y1, double x2, double y2, double *x, double* y)
/* retourne le point a l'infini dans la direction x2 y2 depuis x1 y1*/
{ vector v(x2,y2);
  v = v.norm();
  *x = x1 + R * v[0];
  *y = y1 + R * v[1];
}

list<segment> seg_list;

void draw_triang_seg(double x1, double y1, double x2, double y2)
{ seg_list.append(segment(x1,y1,x2,y2));  }


random_source& operator>>(random_source& R, point& p)
{ double x,y;
  R >> x >>y;
  p = point(x,y);
  return R;
}

main()
{
   int N = read_int("N = ");

   list<point> L;
   random_source ran(0,1000);

   ran.set_seed(12345*N);

   for(int i=0; i<N; i++) 
   { point p;
     ran >> p;
     L.append(p);
    }

  DELAUNAY_TREE<int> DT;

  float T = used_time();

  point p;
  forall(p,L) DT.insert(p,0);
  cout << string("insert time = %.2f",used_time(T)) << endl;

  DT.trace_triang_edges( draw_triang_seg);
  cout << string("trace time  = %.2f",used_time(T)) << endl;

}

