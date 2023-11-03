#include <LEDA/impl/delaunay_tree.h>
#include <LEDA/plane.h>
#include <LEDA/window.h>


window* Wp;

const double R = 1000;  // "length of inifinite rays"

int ymax;

int segment_to_points(segment s, list<point>& out, double dist)
{ 
  int n = int(s.length()/dist) + 2;

  double dx = (s.xcoord2() - s.xcoord1())/n;
  double dy = (s.ycoord2() - s.ycoord1())/n;
  double x  = s.xcoord1();
  double y  = s.ycoord1();

  out.append(s.start());

  for(int i = 0; i<n; i++)
  { x += dx + double(rand_int(-100,100))/1000000;
    y += dy + double(rand_int(-100,100))/1000000;
    out.append(point(x,y));
   }

  return n;

}

int circle_to_points(circle C, list<point>& out, double dist)
{ 
  point c = C.center();
  double r = C.radius();
  int n = int(6.283 * r/dist);

  double alpha = 0;
  double d = 6.283/n;

  for(int i = 0; i<n; i++)
  { out.append(c.translate(alpha,r+double(rand_int(-100,100))/1000000));
    alpha += d;
   }

  out.permute();

 return n;

}



static color site_color   = black;
static color voro_color   = blue;
static color triang_color = red;
static color hull_color   = violet;


void draw_vor_site(double x, double y)
{ Wp->draw_point(x,y,site_color); }

void draw_vor_seg(double x1, double y1, double x2, double y2,double,double)
{ Wp->draw_segment(x1,y1,x2,y2,voro_color); }


void draw_triang_seg(double x1, double y1, double x2, double y2)
{ Wp->draw_segment(x1,y1,x2,y2,triang_color); 
 }

void infi_pt(double x1, double y1, double x2, double y2, double *x, double* y)
/* retourne le point a l'infini dans la direction x2 y2 depuis x1 y1*/
{ vector v(x2,y2);
  v = v.norm();
  *x = x1 + R * v[0];
  *y = y1 + R * v[1];
}



DELAUNAY_TREE<int> DT;

bool vor = false;
bool triang = false;
list<segment> CH;
int display = 0;


void insert_point(point p)
{ Wp->draw_point(p,site_color);
  DT.insert(p,0);
 }

void insert_segment(segment s)
{ list<point> pl;
  segment_to_points(s,pl,10/Wp->scale());
  point p;
  forall(p,pl) insert_point(p);
}

void insert_circle(circle c)
{ list<point> pl;
  circle_to_points(c,pl,10/Wp->scale());
  point p;
  forall(p,pl) insert_point(p);
}
            


void draw(int display)
{ segment s;
  switch(display) {

  case 0: DT.trace_voronoi_edges(draw_vor_seg,infi_pt);
          break;

  case 1: DT.trace_triang_edges(draw_triang_seg);
          break;

  case 2: { list<DT_item> L;
            list<point> P;
            DT.convex_hull(L);
            DT_item it;
            forall(it,L) P.append(DT.key(it)); 
            Wp->draw_filled_polygon(P,hull_color);
            break;
           }
  }
}



void redraw()
{ DT.trace_voronoi_sites(draw_vor_site);
  draw(display);
}


void interactive(window& W)
{
  W.set_redraw(redraw);
  W.set_mode(xor_mode);
  W.set_node_width(4);


  int N = 500;
  int grid_width = 0;
  int line_width = 0;

  panel SP("SETTINGS");
  SP.choice_item("display",display,"voronoi","delaunay","ch-hull");
  SP.int_item("rand sites",N,1,1000);
  SP.int_item("grid width", grid_width,0,40);
  SP.int_item("line width", line_width,1,4);
  SP.color_item("sites ", site_color);
  SP.color_item("voro  ", voro_color);
  SP.color_item("triang", triang_color);
  SP.color_item("hull",   hull_color);
  SP.button("continue");


  panel P("DYNAMIC VORONOI DIAGRAMS");
  P.text_item("based on Delaunay Trees (by O. Devillers)");
  P.button("point");
  P.button("segment");
  P.button("circle");
  P.button("random");
  P.button("delete");
  P.button("neighbor");
  P.button("settings");
  P.button("clear");
  P.button("quit");

  if (window::screen_width() >= 800) P.display(0,0);

for(;;)
{
  int but;

  if (window::screen_width() < 800)
      but = P.open(0,0);
  else
      but = P.read();

  if (but == 8) break;

  //draw(display); // delete previous drawing

  W.clear();
  DT.trace_voronoi_sites(draw_vor_site);

  switch (but) {
         
    case 0: { point p;
              while (W >> p)  insert_point(p);
              break;
             }
            
    case 1: { segment s;
              while (W >> s) insert_segment(s);
              break;
             }
        
    case 2: { circle c;
              while (W >> c) insert_circle(c);
              break;
             }

    case 3: { // random points
              for(int i=0; i<N; i++)
                 insert_point(point(rand_int(10,500),rand_int(10,ymax)));
              break;
             }
   

    case 4: { // delete point
              point p;
              while (W >> p)
              { DT_item it = DT.neighbor(p); 
                if (it) 
                { W.draw_point(DT.key(it),site_color);
                  DT.del_item(it); 
                 }
               }
              break;
             }

    case 5: { // nearest neighbor
              DT_item near_it = nil;
              point p;
              while (W >> p)
              { if (near_it) W.draw_filled_node(DT.key(near_it),red);
                near_it = DT.neighbor(p); 
                if (near_it) W.draw_filled_node(DT.key(near_it),red);
              }
              if (near_it) W.draw_filled_node(DT.key(near_it),red);
              break;
             }

    case 6: { // settings
              SP.open();
              W.set_grid_mode(grid_width);
              W.set_line_width(line_width);
              W.set_mode(xor_mode);
              break;
             }

    case 7: { // clear
              DT.clear();
              W.clear();
              CH.clear();
             }

   }  // switch

  draw(display); // draw picture

  if (but < 5  && window::screen_width() < 800)
  { W.set_frame_label("click any button to continue");
    W.read_mouse(); // wait for a click
    W.reset_frame_label();
   }

 } // for(;;)

}



void demo(int N, int sec)
{ int i;

  while(Wp->get_button() == 0)
  {
    DT.clear();
    Wp->clear();
    Wp->message(string("%d sites",N));
    Wp->flush();

    for (i=0; i<N ; i++)
    { point p(rand_int(10,500),rand_int(10,ymax));
      DT.insert(p,i);
      *Wp << p;
     }


    //DT.trace_voronoi_sites( draw_vor_site);

    DT.trace_voronoi_edges( draw_vor_seg,infi_pt);
    Wp->flush();
    wait(sec);

    Wp->clear();

    DT.trace_triang_edges( draw_triang_seg);
    Wp->flush();
    wait(sec);
  }

}


/*
#include <LEDA/stream.h>
*/


main(int argc, char** argv)
{
  int N = (argc > 1) ? atoi(argv[1]) : 0;

  window W;

  W.set_flush(false);

  W.init(0,512,0);

  Wp = &W;

  ymax = int(W.ymax()-10);

  if (N==0) 
     interactive(W);
  else
     demo(N,2);

 return 0;
}
