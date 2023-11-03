#include <LEDA/plane_alg.h>
#include <LEDA/point.h>
#include <LEDA/matrix.h>
#include <LEDA/map.h>


static int  flip_count = 0;
static int visited_edges = 0;

#define NEXT_FACE_EDGE(x)   G.cyclic_adj_pred(G[x])

void DELAUNAY_FLIPPING(GRAPH<point,edge>& G)
{
  edge_map<bool> marked(G,false);
  list<edge> L;
  edge e;

  forall_edges(e,G) 
    if ( !marked[e] )
    { L.append(e);
      marked[e] = true;
      marked[G[e]] = true;
     }

  while ( !L.empty() )
  { visited_edges++;

    edge e = L.pop();
    edge r = G[e];
    edge e_succ = G.cyclic_adj_succ(e);
    edge e_pred = G.cyclic_adj_pred(e);

    marked[e] = false;
    marked[r] = false;

    point a = G[source(e)];
    point b = G[target(e_pred)];
    point c = G[target(e)];
    point d = G[target(e_succ)];

    if ( right_turn(b,a,d) && right_turn(b,d,c) && incircle(a,b,c,d) )
    { flip_count++;

      G.del_edge(r);

      for(edge x = e_pred; x != e; x = NEXT_FACE_EDGE(x))
        if ( !marked[x] ) 
        { L.push(x); 
          marked[x] = marked[G[x]] = true; 
         }

      G.del_edge(e);

      e = G.new_edge(G[e_pred],target(e_succ),nil,before);
      r = G.new_edge(G[e_succ],target(e_pred),e);
      G[e] = r;

     }

   }
 }



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

GRAPH<point,edge> G;


float T = used_time();

G.clear();
flip_count = 0;
TRIANGULATE_POINTS(L,G);
DELAUNAY_FLIPPING(G);
cout << string("visited = %d flips = %d   time = %.2f",
                visited_edges, flip_count,used_time(T)) << endl;

return 0;
}
