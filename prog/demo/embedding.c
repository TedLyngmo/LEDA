#include <LEDA/graph.h>
#include <LEDA/graph_alg.h>
#include <LEDA/window.h>
#include <LEDA/graph_edit.h>

main()
{
  window W;
  panel P;

  GRAPH<point,int> G;
  node v,w;
  edge e;

  int  N = 40;
  int  node_width   = 12;
  int  edge_width   =  1;
  bool random_input = true;

  P.bool_item("random input",random_input);
  P.int_item("# segments",N);
  P.int_item("node width",node_width,1,15);
  P.int_item("edge width",edge_width,1,8);


  for(;;)
  {
    P.open();
    W.set_node_width(node_width);
    W.set_line_width(edge_width);

    G.clear();

    if (random_input)
     { W.set_node_width(3);
       random_planar_graph(G,N);
      }
    else
       graph_edit(W,G,false);

  
    W.del_messages();
    W.message(string("PLANARITY TEST  N = %d",G.number_of_nodes()));
  
    if ( ! PLANAR(G,true) )
    { W.acknowledge("Graph is not planar");
      continue;
     }
  
  
     node_array<int> x(G),y(G);
  
  
     W.del_messages();
     W.message("STRAIGHT LINE EMBEDDING");
  
     STRAIGHT_LINE_EMBEDDING(G,x,y);
     
     int xmax=0;
  
     forall_nodes(w,G)  
       if (x[w]>xmax) xmax= x[w];
     
     
     W.init(-5,xmax+5,-5);
  
     forall_nodes(v,G) W.draw_node(x[v],2*y[v]);
     forall_edges(e,G)  
     { v = source(e);
       w = target(e);
       W.draw_edge(point(x[v],2*y[v]),point(x[w],2*y[w]),red);
      }
     
      W.read_mouse();

    }
     
  return 0;
 }
