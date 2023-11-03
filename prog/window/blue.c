#include <fstream>
#include <LEDA/list.h>
#include <LEDA/window.h>


int main(int argc, char** argv)
{

    std::ifstream colors("/usr/lib/X11/rgb.txt");

   int r,g,b;
   char c;
   string s;

   string pat = "blue";

   if (argc > 1) pat = argv[1];

   list<string> L;

   while (colors >> r >> g >> b >> c)
   { colors.putback(c);
     s = read_line(colors);
     if (s.pos(pat) != -1) L.append(s);
    }

   int n = L.length();

   window W(500,500);

   W.init(0,n,0);
   W.set_show_coordinates(false);

   int i = 0;
   forall(s, L)
   { std::cout << s << std::endl;
     W.draw_filled_rectangle(i,0,i+1,n,color(~s));
     //W.draw_rectangle(i,0,i+1,n,black);
     i++;
    }
    std::cout << i << std::endl;

    W.read_mouse();

  return 0;
}

