#include <LEDA/window.h>
#include <LEDA/plane_alg.h>
#include <LEDA/point.h>
#include <LEDA/map.h>

window W;

void draw_circle(window& W, point a, point b, point c)
{ line L1 = p_bisector(a,b);
  line L2 = p_bisector(a,c);
  point m;
  L1.intersection(L2,m);
  W.draw_disc(m,m.distance(a),violet);
}

static int  flip_count = 0;
static int  visited_edges = 0;
static int  speed = 40;
static bool animate = false;

void animate_flip(point a, point b, point c, point d, int n)
{
  W.del_messages();
  W.message(string(" %2d flips",flip_count++));

  segment s1(a,b);
  segment s2(c,d);
  double l1 = s1.length()/n;
  double l2 = s2.length()/n;
  double a1 = s1.angle();
  double a2 = s2.angle();

  while (n--)
  { point a_new = a.translate(a1,l1);
    point c_new = c.translate(a2,l2);
    W.draw_edge(a_new,c_new,red);
    W.draw_edge(a,c,red);
    a = a_new;
    c = c_new;
   }
  W.draw_edge(a,c,red);
  W.draw_edge(a,c,blue);

}




void flipping(GRAPH<point,edge> &G)
{
  edge_map<bool> marked(G,false);
  list<edge> L;
  edge e;

  forall_edges(e,G)  marked[e] = false;

  forall_edges(e,G)
    if ( !marked[e] )
    { L.append(e);
      marked[e] = true;
      marked[G[e]] = true;
      if (animate) W.draw_edge(G[source(e)],G[target(e)],red);
     }


  while ( !L.empty() )
  { visited_edges++;
    edge e = L.pop();
    marked[e] = false;
    marked[G[e]] = false;

    edge e1 =  G.cyclic_adj_pred(e);
    edge e2 =  G.cyclic_adj_pred(G[e1]);
    edge e3 =  G.cyclic_adj_pred(G[e]);
    edge e4 =  G.cyclic_adj_pred(G[e3]);

    point a = G[source(e1)];
    point b = G[source(e2)];
    point c = G[source(e3)];
    point d = G[source(e4)];

    if ( orientation(b,a,d) > 0 && orientation(b,d,c) > 0
         && incircle(a,b,c,d) > 0)
    {
      if (animate) animate_flip(a,b,c,d,8000/speed);

      G.del_edge(G[e]);
      G.del_edge(e);
      e = G.new_edge(e2,source(e4),nil);
      G[e] = G.new_edge(e4,source(e2),e);
      marked[e] = false;
      marked[G[e]] = false;

      if ( !marked[e1])
      { L.append(e1);
        marked[e1] = marked[G[e1]] = true;
        if (animate)
        { W.draw_edge(a,b,blue);
          W.draw_edge(a,b,red);
         }
       }

      if ( !marked[e2])
      { L.append(e2);
        marked[e2] = marked[G[e2]] = true;
        if (animate)
        { W.draw_edge(b,c,blue);
          W.draw_edge(b,c,red);
         }
       }

      if ( !marked[e3])
      { L.append(e3);
        marked[e3] = marked[G[e3]] = true;
        if (animate)
        { W.draw_edge(c,d,blue);
          W.draw_edge(c,d,red);
         }
       }

      if ( !marked[e4])
      { L.append(e4);
        marked[e4] = marked[G[e4]] = true;
        if (animate)
        { W.draw_edge(d,a,blue);
          W.draw_edge(d,a,red);
         }
       }
     }
     else
     if (animate)
     { W.draw_edge(a,c,red);
       W.draw_edge(a,c,blue);
      }
   }

}



random_source& operator>>(random_source& R, point& p)
{ double x,y;
  R >> x >>y;
  p = point(100*x,100*y);
  return R;
}

int main()
{
   int N = 100;
   bool grid = false;

   panel P;

   P.int_item("speed",speed,1,100);
   P.bool_item("animate",animate);
   P.bool_item("grid",grid);
   P.int_item("#points",N,0,500);
   P.open();


   list<point> L;
   random_source ran(0,1000);

//window W;
W.init(-5,105,-5);
if (grid) W.set_grid_mode(2);
W.set_node_width(2);

L.append(point(0,0));
L.append(point(100,0));
L.append(point(50,100));

point p;
for(int i=0; i<N; i++)
{ ran >> p;
  L.append(p);
 }

forall(p,L) W.draw_filled_node(p);

GRAPH<point,edge> G;

TRIANGULATE_POINTS(L,G);

W.set_mode(xor_mode);
flipping(G);
W.set_mode(src_mode);

W.set_flush(false);
edge e;
forall_edges(e,G) W.draw_edge(G[source(e)],G[target(e)],blue);
W.flush();

W.set_mode(xor_mode);

while (W >> p)
{
  forall_edges(e,G)
  { edge e1 = e;
    edge e2 = G.cyclic_adj_pred(G[e1]);
    edge e3 = G.cyclic_adj_pred(G[e2]);

    point a = G[source(e1)];
    point b = G[source(e2)];
    point c = G[source(e3)];

    if ( left_turn(a,b,p) && left_turn(b,c,p) && left_turn(c,a,p) )
    {
      W.draw_filled_node(p);
      //W.draw_edge(p,a,red);
      //W.draw_edge(p,b,red);
      //W.draw_edge(p,c,red);

      node v = G.new_node(p);

      edge x = G.new_edge(e1,v,nil);
      G[x] = G.new_edge(v,source(e1),x);
      x = G.new_edge(e2,v,nil);
      G[x] = G.new_edge(v,source(e2),x);
      x = G.new_edge(e3,v,nil);
      G[x] = G.new_edge(v,source(e3),x);
      break;
    }
  }

  W.clear();
  node v;
  forall_nodes(v,G) W.draw_filled_node(G[v]);

  animate = true;
  flipping(G);
}

return 0;
}
