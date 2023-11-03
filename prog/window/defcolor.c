#include <LEDA/window.h>


int r = 10;
int g = 20;
int b = 30;

panel* p;

void slider_red(int x)
{ r = x;
  p->set_palette(blue2,r,g,b);
 }

void slider_green(int x)
{ g = x;
  p->set_palette(blue2,r,g,b);
 }

void slider_blue(int x)
{ b = x;
  p->set_palette(blue2,r,g,b);
 }


main()
{

   panel P("define back ground color");
   p = &P;
   P.int_item("red  ",r,0,63,slider_red);
   P.int_item("green",g,0,63,slider_green);
   P.int_item("blue ",b,0,63,slider_blue);
   P.button("quit");

   P.set_palette(blue2,r,g,b);
   P.display();

   //P.draw_filled_circle(50,30,25,blue2);

   while (P.read() == -1)  // no button pressed
       P.set_palette(blue2,r,g,b);

   return 0;
}
