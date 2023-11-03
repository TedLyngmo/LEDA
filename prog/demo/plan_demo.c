#include <cmath>
#include <LEDA/graph.h>
#include <LEDA/graph_alg.h>
#include <LEDA/window.h>
#include <LEDA/graph_edit.h>


void circular_embedding(GRAPH<point,int>& G, int x, int y, int radius)
{ int n = G.number_of_nodes();
  float ang = 0;
  node v;
  forall_nodes(v,G)
  { G[v] = point(x+radius*sin(ang),y+radius*cos(ang));
    ang += 6.283/n;
  }
}

void grid_embedding(GRAPH<point,int>& G, int c0, int c1)
{ int l = int(sqrt(G.number_of_nodes()) + 1);
  int d = (c1-c0)/l;
  int i = 0;
  node v;
  forall_nodes(v,G)
  { int row = i/l;
    int col = i%l;
    G[v] = point(c0+col*d ,c0+row*d);
    i++;
   }
}


void random_embedding(GRAPH<point,int>& G, int c0, int c1)
{ int n = G.number_of_nodes();
  random_source ran(c0,c1);
  node v;
  forall_nodes(v,G)
  { int x,y;
    ran >> x >> y;
    G[v] = point(x,y);
   }
}


void draw_graph(const GRAPH<point,int>& G, window& W, bool numbering=false)
{ node v;
  edge e;

  if (numbering)
     { int i = 0;
       forall_nodes(v,G) W.draw_int_node(G[v],i++,red);
      }
  else
     forall_nodes(v,G) W.draw_filled_node(G[v],red);

  forall_edges(e,G)
    W.draw_edge(G[source(e)],G[target(e)],blue);
}


void draw_graph(const GRAPH<point,int>& G, window& W,
                const node_array<double>& xc, const node_array<double>& yc,
                const node_array<double>& dx, const node_array<double>& dy)
{ node v;

  forall_nodes(v,G) W.draw_filled_node(xc[v]+dx[v],yc[v]+dy[v],red);

  edge e;
  forall_edges(e,G)
  { node v = source(e);
    node w = target(e);
    W.draw_edge(xc[v]+dx[v],yc[v]+dy[v],xc[w]+dx[w],yc[w]+dy[w],blue);
   }
}

void draw_graph(const GRAPH<point,int>& G, window& W,
                const node_array<double>& xc, const node_array<double>& yc)
{ node v;

  forall_nodes(v,G) W.draw_filled_node(xc[v],yc[v],red);

  edge e;
  forall_edges(e,G)
  { node v = source(e);
    node w = target(e);
    W.draw_edge(xc[v],yc[v],xc[w],yc[w],blue);
   }
}




void move_graph_x11(window& W, GRAPH<point,int>& G,
                    node_array<double>& xc,
                    node_array<double>& yc, int speed)
{
  node_array<double> dx(G);
  node_array<double> dy(G);
  int d = 400/speed;

  W.clear();

  W.set_mode(xor_mode);

  draw_graph(G,W);


  node v;
  forall_nodes(v,G)
  { dx[v] = (xc[v] - G[v].xcoord())/d;
    dy[v] = (yc[v] - G[v].ycoord())/d;
    xc[v] = G[v].xcoord();
    yc[v] = G[v].ycoord();
   }

  while(d--)
  { draw_graph(G,W,xc,yc,dx,dy);
    draw_graph(G,W,xc,yc);
    forall_nodes(v,G)
    { xc[v] += dx[v];
      yc[v] += dy[v];
     }
   }

  W.set_mode(src_mode);

}


void move_graph_os2(window& W, GRAPH<point,int>& G,
                    node_array<double>& xc,
                    node_array<double>& yc, int speed)
{
  node_array<double> dx(G);
  node_array<double> dy(G);
  int d = 200/speed;

  node v;
  forall_nodes(v,G)
  { dx[v] = (xc[v] - G[v].xcoord())/d;
    dy[v] = (yc[v] - G[v].ycoord())/d;
    xc[v] = G[v].xcoord();
    yc[v] = G[v].ycoord();
   }

  while(d--)
  { W.start_batch();
    W.clear();
    draw_graph(G,W,xc,yc,dx,dy);
    W.end_batch();
    forall_nodes(v,G)
    { xc[v] += dx[v];
      yc[v] += dy[v];
     }
   }

}

void move_graph(window& W, GRAPH<point,int>& G,
                node_array<double>& xc,
                node_array<double>& yc, int speed)
{

#if defined(__OS2__)
     move_graph_os2(W,G,xc,yc,speed);
#else
     move_graph_x11(W,G,xc,yc,speed);
#endif

 }


int main()
{

panel P("LEDA Planarity Test Demo");

P.text_item("");
P.text_item("This demo illustrates planarity testing and straight-line");
P.text_item("embedding. You have two ways to construct a graph: either");
P.text_item("interactively by using the LEDA graph editor or by calling");
P.text_item("one of two graph generators. The first generator constructs");
P.text_item("a random graph with a certain number of nodes and edges and");
P.text_item("the second constructs a planar graph with a certain number");
P.text_item("of nodes by intersecting random lines in the unit square");
P.text_item("");
P.text_item("The graph is displayed and then tested for planarity. If it");
P.text_item("is planar a straight-line drawing is produced, otherwise, a");
P.text_item("Kuratowski subgraph is highlighted.");
P.button("continue");

P.open();

window W;

GRAPH<point,int>G;
node v,w;
edge e;

int n = 30;
int m = 40;
int init_embed = 2;
int final_embed = 0;
int speed = 0;


panel P1("PLANARITY TEST");

P1.text_item("The first slider asks for the number n of nodes and the second");
P1.text_item("slider asks for the number m of edges. If you select the random");
P1.text_item("button then a graph with n nodes and m edges is constructed, if");
P1.text_item("you select the planar button then a set of random line segments");
P1.text_item("is chosen and intersected to yield a planar graph with about n");
P1.text_item("n nodes, and if you select the edit button the graph editor is");
P1.text_item("called.");
P1.text_item(" ");

P1.int_item("number of nodes",n,0,200);
P1.int_item("number of edges",m,0,200);
P1.choice_item("initial embedding",init_embed,"random","grid","circular");
P1.choice_item("final embedding",final_embed,"fary","schnyder");
P1.int_item("animation speed",speed,0,20);

P1.button("random");
P1.button("planar");
P1.button("triang");
P1.button("edit");
P1.button("quit");

for(;;)
{
  int inp = P1.open(W);

  if (inp == 4) break;   // quit button pressed

  W.init(-50,950,-50);
  W.set_node_width(4);

  G.clear();

  switch(inp) {

  case 0: { random_graph(G,n,m);
            eliminate_parallel_edges(G);
            list<edge> Del;
            forall_edges(e,G)
               if (source(e)==target(e)) Del.append(e);
            forall(e,Del) G.del_edge(e);
            break;
           }

  case 1:  random_planar_graph(G,n);
           break;

  case 2:  triangulated_planar_graph(G,n);
           break;

  case 3:  W.set_node_width(10);
           graph_edit(W,G,false);
           break;

   }

   if (inp != 3)
   { if (init_embed == 0) random_embedding(G,0,850);
     if (init_embed == 1) grid_embedding(G,0,850);
     if (init_embed == 2) circular_embedding(G,450,400,350);
     draw_graph(G,W);
    }



  if (PLANAR(G,false))
  {
    if(G.number_of_nodes()<4)
        W.message("That's an insult: Every graph with |V| <= 4 is planar");
    else
      {
        W.message("G is planar. I compute a straight-line embedding ...");

        // first make graph bi-directed and bi-connected

        edge_array<edge> rev(G);
        list<edge> single;
        edge e;

        // collect all "singles" and give each of them a "partner"

        Is_Bidirected(G,rev);

        forall_edges(e,G)
             if (rev[e] == nil)  single.append(e);

        forall(e,single)
             rev[e] = G.new_edge(target(e),source(e));

        list<edge> bi_edges = make_biconnected_graph(G);

        PLANAR(G,true);

        node_array<int> xcoord(G);
        node_array<int> ycoord(G);

        float fx = 900.0/G.number_of_nodes();
        float fy = fx;

        if (final_embed == 0)
          { STRAIGHT_LINE_EMBEDDING(G,xcoord,ycoord);
            fx /= 2;
           }
        else
            STRAIGHT_LINE_EMBEDDING2(G,xcoord,ycoord);

        forall(e,single) G.del_edge(rev[e]);
        forall(e,bi_edges) G.del_edge(e);

        node_array<double> xc(G);
        node_array<double> yc(G);


         forall_nodes(v,G)
         { xc[v] = fx*xcoord[v];
           yc[v] = fy*ycoord[v];
          }

        if (speed > 0) // animate
          { W.message("click any button");
            W.read_mouse();
            move_graph(W,G,xc,yc,speed);
           }
         else
          { forall_nodes(v,G) G[v] = point(xc[v],yc[v]);
            W.clear();
            draw_graph(G,W);
           }
      }
   }
  else
    {
      W.message("Graph is not planar. I compute the Kuratowski subgraph ...");

      list<edge> L;

      PLANAR(G,L,false);


      // display Kuratowski subdivision with edge set L

      edge_array<bool> marked(G,false);
      node_array<int>  side(G,0);

      edge e;
      forall(e,L) marked[e] = true;

      list<edge> el = G.all_edges();

      forall(e,el)
        if (!marked[e])
          { //W.draw_edge(G[source(e)],G[target(e)],yellow);
            G.del_edge(e);
           }

      int lw = W.set_line_width(3);

      forall_edges(e,G) W.draw_edge(G[source(e)],G[target(e)]);

      node v;
      forall_nodes(v,G) if (G.degree(v) == 3) break;

      if (G.degree(v) == 3)
        forall_inout_edges(e,v)
        { marked[e] = false;
          node w = G.opposite(v,e);
          while (G.degree(w) == 2)
          { edge x,y;
            forall_inout_edges(x,w)
               if (marked[x]) y=x;
             marked[y] = false;
             w = G.opposite(w,y);
           }
          side[w] = 1;
         }


      int i = 1;
      int j = 4;

      forall_nodes(v,G)
      {
        if (G.degree(v)==2) W.draw_filled_node(G[v],black);

        if (G.degree(v) > 2)
        { int nw = W.set_node_width(10);
          if (side[v]==0)
             W.draw_int_node(G[v],i++,green);
          else
             if (W.mono())
                W.draw_int_node(G[v],j++,black);
             else
                W.draw_int_node(G[v],j++,violet);

          W.set_node_width(nw);
         }
      }
      W.set_line_width(lw);
   }

   W.set_show_coordinates(false);
   W.set_frame_label("click any button to continue");
   W.read_mouse(); // wait for a click
   W.reset_frame_label();
   W.set_show_coordinates(true);

 } //  Main Loop

return 0;

}
