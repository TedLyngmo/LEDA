#include <LEDA/window.h>
#include <LEDA/stream.h>

#include <cstdlib>


int main()
{
  window W;
  panel  P;

  string file_name = "points";
  bool file_output = true;
  int N = 10000;
  int max_coord = 1000;


  P.string_item("file",file_name);
  P.bool_item("file output",file_output);
  P.int_item("# points",N);
  P.int_item("max coord",max_coord,0,5000);
  P.open();

  file_ostream F(file_name);

  W.init(0,max_coord,0);

  random_source ran(0,max_coord);

  for(int i=0; i<N ; i++)
  { long x,y;
    ran >> x >> y;
    W.draw_pix(x,y);
    if (file_output) F << x << " " << y << "\n";
   }

  W.read_mouse();

  return 0;

}

