#include <LEDA/plane.h>
#include <LEDA/window.h>


enum { POINT=0, SEGMENT=1, LINE=2, CIRCLE=3, POLY=4 };

main()
{
  window W;

  line_style lstyle = solid;
  int        lwidth = 1;
  color      col    = black;
  bool       fill   = false;
  int        shape =  POINT;

  panel P("DRAW");

  P.bool_item  ("fill",fill);
  P.color_item ("color",col);
  P.lstyle_item("lstyle",lstyle);
  P.int_item   ("lwidth",lwidth,1,10);

  P.choice_item("shape",shape,"point","segment","line","circle","poly");

  P.button("clear");
  P.button("quit");

  P.display(0,0);


  for(;;)
  { 
    window* wp;
    double x;
    double y;
    read_mouse(wp,x,y);

    if (wp == &W || wp == (window*)&P) put_back_event();

    if (wp == &P)
      { int k = P.read();
        W.set_line_width(lwidth);
        W.set_line_style(lstyle);
  
        if (k == 0) W.clear();
        if (k == 1) break;
       }
    else
      switch(shape) {
  
        case POINT: 
              { point p;
                W >> p;  
                W.draw_point(p,col);
                break;
               }
  
        case SEGMENT: 
              { segment s;
                W >> s;
                W.draw_segment(s,col);
                break;
               }
  
        case LINE: 
              { line l;
                W >> l;  W.draw_line(l,col);
                break;
               }
  
        case CIRCLE: 
              { circle c;
                W >> c;  
                if (fill) 
                   W.draw_disc(c,col);
                else
                   W.draw_circle(c,col);
                break;
               }
  
        case POLY: 
              { polygon P;
                W >> P;  
                if (fill) 
                   W.draw_filled_polygon(P,col);
                else 
                   W.draw_polygon(P,col);
                break;
               }
      }
  }

  return 0;
}
