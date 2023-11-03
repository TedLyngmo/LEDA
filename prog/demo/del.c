#include <LEDA/plane_alg.h>
#include <LEDA/window.h>

main()
{
  window W;

  list<point> L;

  point p;
  while (W >> p) 
  { W << p;
    L.append(p);
   }

  GRAPH<point,edge> G;

  DELAUNAY_TRIANG(L,G);

  G.print();

  edge e;
  forall_edges(e,G)
  { node v = source(e);
    node w = target(e);
    W.draw_segment(G[v],G[w],blue);
   }

  W.read_mouse();

}
