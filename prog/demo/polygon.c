#include <LEDA/plane.h>
#include <LEDA/window.h>

int main()

{ window W;

  int grid_mode = 0;

  panel p("polygon");

  p.text_item("This program demonstrates the intersection operation  ");
  p.text_item("for simple polygons (data type polygon). Use the left ");
  p.text_item("mouse button to define the vertex sequence of a simple");
  p.text_item("polygon P in clockwise order. Terminate the input with");
  p.text_item("the middle button. Now, for each next drawn polygon Q ");
  p.text_item("the intersection with P (list of polygons) is computed");
  p.text_item("and displayed. Terminate the program by clicking the  ");
  p.text_item("right button.");

  p.int_item("grid",grid_mode,0,50,10);

  p.button("continue");

  p.open();


  W.init(0,1000,0,grid_mode);

  polygon P;

  W >> P;
  W.draw_polygon(P,blue);

  W.message(string("AREA = %f",P.area()));

  W.set_mode(xor_mode);

  polygon Q;

  while (W >> Q)
  { W << Q;
    list<polygon> L = P.intersection(Q);
    polygon R;
    forall(R,L) W.draw_filled_polygon(R,red);
    W.read_mouse();
    W << Q;
    forall(R,L) W.draw_filled_polygon(R,red);
   }

  W.set_node_width(2);

  for(int i=0; i<1000; i++)
  { point p(rand_int(0,1000),rand_int(0,1000));
    if (P.inside(p)) W.draw_filled_node(p,red);
    if (P.outside(p)) W.draw_point(p,blue);
   }

  W.read_mouse();

 return 0;

}
