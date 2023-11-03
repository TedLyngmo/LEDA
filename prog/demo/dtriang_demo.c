#include <LEDA/graph.h>
#include <LEDA/plane_alg.h>
#include <LEDA/window.h>

window W;

void draw_graph(const GRAPH<point,edge>& G)
{ edge_array<bool> drawn(G,false);
  node v;
  forall_nodes(v,G) W.draw_filled_node(G[v],blue);
  edge e;
  forall_edges(e,G) 
    if (!drawn[e])
    { W.draw_edge(G[source(e)],G[target(e)],blue);
      drawn[G[e]] = drawn[e] = true;
     }
 }

point center(point a, point b, point c)
{ line L1 = p_bisector(a,b);
  line L2 = p_bisector(a,c);
  point m;
  L1.intersection(L2,m);
  return m;
}


void draw_circle(point a, point b, point c)
{ line L1 = p_bisector(a,b);
  line L2 = p_bisector(a,c);
  point m;
  L1.intersection(L2,m);
  W.draw_disc(m,m.distance(a),violet);
}


#define next_face_edge(x)  G.cyclic_adj_pred(G[x])


void draw_voro(window& W, const GRAPH<point,edge>& G)
{ 
  point nil_point;
  edge_array<bool>  visited(G,false);
  edge_array<point> vnode(G,nil_point);

  edge e;
  forall_edges(e,G)
     if (identical(vnode[e],nil_point))
     { edge e1 = next_face_edge(e);
       edge e2 = next_face_edge(e1);
       point a = G[source(e)];
       point b = G[source(e1)];
       point c = G[source(e2)];
       vnode[e] = vnode[e1] = vnode[e2] = center(a,b,c);
     }

/*
  node v;
  forall_nodes(v,G) W.draw_point(G[v],blue);
*/

  forall_edges(e,G)
     if (!visited[e])
     { edge r = G[e];
       visited[r] = visited[e] = true;
       W.draw_segment(vnode[e], vnode[r],red);
     }
}

/*
void build_voro(window& W, const GRAPH<point,edge>& G)
{ 
  point nil_point;

  GRAPH<rat_point,rat_point> V;

  d_array<rat_point,node>  A;

  edge_array<point> vnode(G,nil_point);

  edge e;
  forall_edges(e,G)
     if (identical(vnode[e],nil_point))
     { edge e1 = next_face_edge(e);
       edge e2 = next_face_edge(e1);
       point a = G[source(e)];
       point b = G[source(e1)];
       point c = G[source(e2)];
       rat_point m = center(a,b,c);
       node v = V.new_node(m);
       vnode[e] = vnode[e1] = vnode[e2] = v;
     }

  forall_edges(e,G)
       V.new_edge(vnode[e], vnode[G[e]], G[source(e)]);
}
*/




void activate_edge(const GRAPH<point,edge>& G, edge e)
{ W.draw_edge(G[source(e)],G[target(e)],blue);
  W.draw_edge(G[source(e)],G[target(e)],red);
 }

void inactivate_edge(const GRAPH<point,edge>& G, edge e)
{ W.draw_edge(G[source(e)],G[target(e)],red);
  W.draw_edge(G[source(e)],G[target(e)],blue);
 }


static int  flip_count = 0;
static int  speed = 40;
static bool animate = false;

void animate_flip(const GRAPH<point,edge>& G, edge e, int n)
{ point a = G[source(e)];
  point b = G[target(G.cyclic_adj_pred(e))];
  point c = G[target(e)];
  point d = G[target(G.cyclic_adj_succ(e))];

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




edge locate_point(const GRAPH<point,edge>& G, point p)
{ edge e;
  forall_edges(e,G)
  { edge e1 = next_face_edge(e);
    point a = G[source(e)];
    point b = G[target(e)];
    point c = G[target(e1)];
    if ( left_turn(a,b,p) && left_turn(b,c,p) && left_turn(c,a,p) ) return e;
   }
  return nil;
}

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


  edge e;
  forall_edges(e,G) 
    if (It[e] == nil && flip_test(G,e)) 
    { It[G[e]] = It[e] = L.append(e);
      if (animate) activate_edge(G,e);
     }


  while ( !L.empty() )
  { flip_count++;

    edge e = L.pop();
    edge x = G.cyclic_adj_pred(e); 

    if (animate) animate_flip(G,e,8000/speed);

    // delete diagonal
    G.del_edge(G[e]);
    G.del_edge(e);

    // collect face edges of quadriliteral
    for(int i = 0; i < 4; i++) 
    { E[i] = x; 
      x = next_face_edge(x); 
     }

    // insert new diagonal
    e = G.new_edge(E[1],source(E[3]));
    G[e] = G.new_edge(E[3],source(E[1]),e);


    // test collected edges 
    for(int j=0; j<4; j++)
    { edge e = E[j];
      if (flip_test(G,e))
        { if (It[e] == nil) 
          It[G[e]] = It[e] = L.push(e); 
          if (animate) activate_edge(G,e);
         }
      else
          if (It[e] != nil)
          { L.del_item(It[e]);
            It[G[e]] = It[e] = nil;
            if (animate) inactivate_edge(G,e);
           }
     }

   }
 }


random_source& operator>>(random_source& R, point& p)
{ double x,y;
  R >> x >>y;
  p = point(1000*x,1000*y);
  return R;
}

main()
{
   int N = 1000;
   bool grid = false;

   panel P;

   P.int_item("speed",speed,1,100);
   P.bool_item("animate",animate);
   P.bool_item("grid",grid);
   P.int_item("#points",N,0,10000);
   P.open();


   list<point> L;
   random_source ran(0,100);

//window W;
W.init(-5,105,-5);
if (grid) W.set_grid_mode(4);
W.set_node_width(3);
W.set_mode(xor_mode);

int infinity = 200;

L.append(point(-infinity,-infinity));
L.append(point(-infinity,+infinity));
L.append(point(+infinity,-infinity));
L.append(point(+infinity,+infinity));

point p;
for(int i=0; i<N; i++) 
{ ran >> p;
  L.append(p);
  W.draw_point(p,blue);
 }

GRAPH<point,edge> G;

TRIANGULATE_POINTS(L,G);
DELAUNAY_FLIPPING(G);

if (!animate)
{ W.clear();
  draw_graph(G);
 }
W.read_mouse();

/*
draw_voro(W,G);
W.read_mouse();

W.clear();
draw_graph(G);
*/


for(;;)
{ int but = W.read_mouse(p);
  if (but == 1)
  { edge e = locate_point(G,p);
    if ( e != nil)
    { node v = G.new_node(p);
      W.draw_filled_node(p);
      for(int i=0; i<3; i++)
      { edge x = G.new_edge(e,v);
        G[x] = G.new_edge(v,source(e),x);
        e = next_face_edge(e);
        W.draw_edge(G[source(x)],p,blue);
       }
      animate = true;
      DELAUNAY_FLIPPING(G);
     }
   }

   if (but == 2) // display circle 
   { edge e = locate_point(G,p);
     int v;
     double a,b;
     if ( e != nil)
     { edge x = next_face_edge(e);
       draw_circle(G[source(e)],G[target(e)],G[target(x)]);
       while (W.read_event(v,a,b) != button_release_event);
       draw_circle(G[source(e)],G[target(e)],G[target(x)]);
      }
    }

   if (but == 3) break;
}

return 0;
}
