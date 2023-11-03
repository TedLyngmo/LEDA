#include <LEDA/graph.h>
#include <LEDA/window.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/plane_alg.h>

int main()
{
  window W;
  W.set_node_width(3);

  int size = 5;
  int  N = 200;

  panel P("sweep demo");
  P.int_item("grid size",size,1,40);
  P.int_item("# segments", N, 1,2000);
  P.button("continue");
  P.button("repeat");
  P.button("quit");

  bool repeat = false;

  for(;;)
  {
    if ( !repeat )
    { int but = P.open(0,0);
      if (but == 1) repeat = true;
      if (but == 2) break;
     }

    W.init(-0.25*size,1.25*size,-0.25*size);

    list<rat_segment> seglist;
    GRAPH<rat_point,rat_segment>   G;

    for (int i=0; i<N; i++)
    { int x1 = rand_int(0,size);
      int y1 = rand_int(0,size);
      int x2 = rand_int(0,size);
      int y2 = rand_int(0,size);
      seglist.append(rat_segment(x1,y1,x2,y2));
     }


    int j = 0;
    rat_segment s;
    forall(s,seglist)
    { j++;
      W.del_messages();
      W.message(string("%4d",j));
      color c = color(1 + (j % 15));
      W.draw_segment(s.xcoord1(),s.ycoord1(), s.xcoord2(),s.ycoord2(),c);
     }

    SWEEP_SEGMENTS(seglist,G);

    W.del_messages();
    W.message(string("|V| = %d",G.number_of_nodes()));

    node v;
    forall_nodes(v,G)
       W.draw_filled_node(G[v].xcoord(),G[v].ycoord());

  }

 return 0;

}
