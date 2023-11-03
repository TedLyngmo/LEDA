
#include <LEDA/plane_alg.h>
#include <LEDA/point.h>
#include <LEDA/matrix.h>


bool incircle(const point& a, const point& b, const point& c, const point& d)
{
   double ax = a.xcoord();
   double ay = a.ycoord();

   double bx = b.xcoord() - ax;
   double by = b.ycoord() - ay;
   double bw = bx*bx + by*by;

   double cx = c.xcoord() - ax;
   double cy = c.ycoord() - ay;
   double cw = cx*cx + cy*cy;

   double dx = d.xcoord() - ax;
   double dy = d.ycoord() - ay;
   double dw = dx*dx + dy*dy;

   //return (by*cw-cy*bw) * (bx*dw-dx*bw) > (bx*cw-cx*bw) * (by*dw-dy*bw);

   return ((by*cx-bx*cy)*dw + (cy*bw-by*cw)*dx  + (bx*cw-cx*bw)*dy)  > 0;
 }



static int flip_count = 0;

inline bool flip_test(const GRAPH<point,edge>& G, edge e)
{ point a = G[source(e)];
  point b = G[target(G.cyclic_adj_pred(e))];
  point c = G[target(e)];
  point d = G[target(G.cyclic_adj_succ(e))];
  return right_turn(b,a,d) && right_turn(b,d,c) && incircle(a,b,c,d);
 }



void DELAUNAY_FLIPPING(GRAPH<point,edge>& G)
{
  edge_map<list_item> It(G,nil);
  list<edge> L;
  edge E[4];

#define next_face_edge(x)  G.cyclic_adj_pred(G[x])

  edge e;
  forall_edges(e,G)
    if (It[e] == nil && flip_test(G,e)) It[G[e]] = It[e] = L.append(e);

  while ( !L.empty() )
  { flip_count++;

    edge e = L.pop();
    edge x = G.cyclic_adj_pred(e);

    // delete diagonal
    G.del_edge(G[e]);
    G.del_edge(e);

    // collect face edges of quadriliteral
    for(int i = 0; i < 4; i++)
    { E[i] = x;
      x = next_face_edge(x);
     }

    // insert new diagonal
    e = G.new_edge(E[1],source(E[3]),nil);
    G[e] = G.new_edge(E[3],source(E[1]),e);


    // test collected edges
    for(int j=0; j<4; j++)
    { edge e = E[j];
      if (flip_test(G,e))
        { if (It[e] == nil)
          It[G[e]] = It[e] = L.push(e);
         }
      else
          if (It[e] != nil)
          { L.del_item(It[e]);
            It[G[e]] = It[e] = nil;
           }
     }

   }
 }


random_source& operator>>(random_source& R, point& p)
{ double x,y;
  R >> x >>y;
  p = point(x,y);
  return R;
}

int main()
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

   GRAPH<point,edge> G;

   float T = used_time();

   flip_count = 0;
   TRIANGULATE_POINTS(L,G);
   DELAUNAY_FLIPPING(G);
   std::cout << string("flips = %d   time = %.2f", flip_count,used_time(T)) << std::endl;

   return 0;
}
