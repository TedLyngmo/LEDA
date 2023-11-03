#include <LEDA/subdivision.h>
#include <LEDA/segment.h>
#include <LEDA/plane_alg.h>
#include <LEDA/window.h>




void   show_face(subdivision<segment>& G, face f, window& W)
{ list<node> L = G.adj_nodes(f);
  list<point> P;
  node v;
  forall(v,L) P.append(G[v]);
  W.draw_filled_polygon(P,orange);
}


main()
{

  window W;

  W.init(0,1000,0);

  W.set_node_width(3);
  W.set_line_width(2);

  panel P("Arrangement of line segments");

  P.text_item("This program computes the subdivision defined by the "); 
  P.text_item("arrangement of a list of straight line segments. Use ");
  P.text_item("the left button to insert a sequence of segments and ");
  P.text_item("terminate the input by clicking the right button. Now");
  P.text_item("you can input query points with the left button. For ");
  P.text_item("each point p the face of the arrangment containing p ");
  P.text_item("is computed and displayed. Terminate the program by  "); 
  P.text_item("pressing the right mouse key.                        ");
  P.text_item("                                                     ");

  P.button("continue");

  P.open();



  list<segment> seglist;
  segment s;

  while ( W >> s ) 
  { W << s;
    seglist.append(s);
   }

  GRAPH<point,segment> G;


  cout << "Computing subdivision.\n";


  SWEEP_SEGMENTS(seglist,G);

  W.clear(); 

  // Draw Subdivision

  edge e;
  forall_edges(e,G)
  W.draw_segment(G[source(e)], G[target(e)]);



  cout << "Constructing search structure\n";


  // insert reverse edges

  list<edge> L = G.all_edges();
  forall(e,L) G.new_edge(target(e),source(e),G[e]);


  // sort edges counter-clockwise

  edge_array<double> angle(G);

  forall_edges(e,G)
  { segment s(G[source(e)],G[target(e)]);
    angle[e] = s.angle();
   }

  G.sort_edges(angle);


  // Create Subdivision

  subdivision<segment> S(G);



  W.message("Give query points!");




  // Locate points

  W.set_mode(xor_mode);

  point p;
  face  f=nil;

  while (W >> p)
  { if (f) show_face(S,f,W);
    f = S.locate_point(p);
    show_face(S,f,W);
   }


  return 0;
}
