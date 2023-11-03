#include <LEDA/graph.h>
#include <LEDA/window.h>
#include <LEDA/rat_point1.h>
#include <LEDA/rat_segment1.h>

extern void sweep1(const list<rat_segment1>&, GRAPH<rat_point1,rat_segment1>&);

int main()
{
  window W;
  W.set_node_width(3);

  int size = 5;
  int  N = 100;

  panel P("sweep demo");
  P.int_item("grid size",size,1,40);
  P.int_item("# segments", N, 1,2000);
  P.button("continue");
  P.button("repeat");
  P.button("quit");

  bool repeat = false;

  for(;;)
  {
    if ( !repeat && P.open(0,0) == 1) repeat = true;;

    W.init(-0.25*size,1.25*size,-0.25*size);

    list<rat_segment1> seglist;
    GRAPH<rat_point1,rat_segment1>   G;

    for (int i=0; i<N; i++)
    { int x1 = rand_int(0,size);
      int y1 = rand_int(0,size);
      int x2 = rand_int(0,size);
      int y2 = rand_int(0,size);
      seglist.append(rat_segment1(x1,y1,x2,y2));
     }


    rat_segment1 s;
    int c = 0;
    forall(s,seglist)
    { if (++c == 16) c = 1;
      W.draw_segment(s.xcoord1(),s.ycoord1(), s.xcoord2(),s.ycoord2(),color(c));
     }

    sweep1(seglist,G);

    std::cout << G.number_of_nodes() << std::endl;

    node v;
    forall_nodes(v,G)
       W.draw_filled_node(G[v].xcoord(),G[v].ycoord());

  }

 return 0;

}
