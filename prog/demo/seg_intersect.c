#include <LEDA/plane_alg.h>
#include <LEDA/window.h>


main()
{ 
  window W(700,800);
 
  int input;
  int grid_width = 0;
  int line_width = 1;
  int node_width = 3;
  int N = 100;

  panel P("SEGMENT_INTERSECTION");

  P.int_item("SEGMENTS", N);
  P.choice_item("INPUT", input,"mouse","random");
  P.int_item("GRID",grid_width,0,40,10);
  P.int_item("line width",line_width,1,5);
  P.int_item("node width",node_width,1,5);


for(;;)
{
  P.open(0,0);

  W.init(-1200,1200,-1200, grid_width);

  W.set_node_width(node_width);
  W.set_line_width(line_width);

  list<segment> seglist;

  if (input) 
   { 
     double ymax = W.ymax()-4*20/W.scale()-100;

     int xmin = int(W.xmin())+100;
     int xmax = int(W.xmax())-100;

     for(int i = 0; i < N; i++)
     { double x1 = rand_int(xmin,-100);
       double y1 = rand_int(-1000,int(ymax));
       double x2 = rand_int(100,xmax);
       double y2 = rand_int(-1000,int(ymax));
       segment s(x1,y1,x2,y2);
       W << s;
       seglist.append(s);
      }
    }
  else // mouse input
    { segment s;

      while (W >> s)
      { W << s;
        seglist.append(s);
       }

     }


  GRAPH<point,segment> SUB;
  node v;

  W.message("Computing Arrangement");
  float T = used_time();

  SWEEP_SEGMENTS(seglist,SUB);

  W.message(string("# = %d   T = %6.2f sec",
                   SUB.number_of_nodes(),
                   used_time(T))
            );

  forall_nodes(v,SUB) W.draw_filled_node(SUB[v]);

} // for(;;)

  return 0;

}

