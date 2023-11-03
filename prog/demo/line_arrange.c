#include <LEDA/subdivision.h>
#include <LEDA/plane_alg.h>
#include <LEDA/window.h>




void  show_face(subdivision<segment>& G, face f, window& W)
{ list<node> L = G.adj_nodes(f);
  list<point> P;
  node v;
  forall(v,L) P.append(G[v]);
  W.draw_filled_polygon(P,red);
}


main()
{

  window W;

  W.init(0,1000,0);

  W.message("            Arrangement of line segments                ");
  W.message("                                                        ");
  W.message("This program computes an arrangement of straight lines. ");
  W.message("Use the left button to insert a sequence of lines and ");
  W.message("terminate the input by clicking the right button. Now");
  W.message("you can input query points with the left button. For ");
  W.message("each point p the face of the arrangment containing p ");
  W.message("is computed and displayed. Terminate the program by  "); 
  W.message("pressing the right mouse key.                        ");
  W.message("                                                     ");
  W.message("Click any button to start.                           ");
  W.read_mouse();
  W.clear();



  list<segment> seglist;

  // build up a frame

  double x0 = W.xmin();
  double x1 = W.xmax();
  double y0 = W.ymin();
  double y1 = W.ymax();

  segment b(x0,y0,x1,y0);
  segment t(x0,y1,x1,y1);
  segment l(x0,y0,x0,y1);
  segment r(x1,y0,x1,y1);

  seglist.append(b);
  seglist.append(t);
  seglist.append(l);
  seglist.append(r);

  line L;

  while ( W >> L ) 
  {  W << L;
     if (L.vertical()) 
        seglist.append(segment(L.x_proj(y0),y0,L.x_proj(y1),y1));
     else 
        seglist.append(segment(x0,L.y_proj(x0),x1,L.y_proj(x1)));
   }

  W.message("Computing subdivision");
  newline;

  GRAPH<point,segment> G;

  SWEEP_SEGMENTS(seglist,G);


  // insert reverse edges

  edge e;
  list<edge> E = G.all_edges();
  forall(e,E) G.new_edge(target(e),source(e),G[e]);

  // sort edges counter-clockwise

  edge_array<double> angle(G);

  forall_edges(e,G)
  { segment s(G[source(e)],G[target(e)]);
    angle[e] = s.angle();
   }

  G.sort_edges(angle);

  subdivision<segment> S(G);

  W.clear(); 
  W.set_line_width(2);

  forall_edges(e,G)
  W.draw_segment(G[source(e)], G[target(e)],blue);


  // Locate points

  W.set_mode(xor_mode);

  point p;
  face  f=nil;

  W.message("Give query points !");

  while (W >> p)
  { if (f) show_face(S,f,W);
    f = S.locate_point(p);
    show_face(S,f,W);
   }



/*
  // show strips

  segment s;
  slist<segment> SL;

  while (W >> p)
  { forall(s,SL) W.draw_segment(s);
    SL.clear();
    SL = S.compute_strip(p);
    W.set_line_width(1);
    W.draw_vline(p.xcoord());
    W.set_line_width(5);
    forall(s,SL) W.draw_segment(s);
   }
*/;

  return 0;
}
