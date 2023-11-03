#include "LEDA/d2_dictionary.h"




int main()
{

  d2_dictionary<double,double,string> S;

  list<dic2_item> L;
  dic2_item       it;
  char            ch;
  double          x,y;



  while (ch != 'q')
  {
    std::cout << "(r/s/f/i/d/D/l/q): ";
    cin  >> ch;

    float T  = used_time();

    switch(ch) {

    case 'i': { std::cout << "insert point: ";
                cin >> x >> y;
                S.insert(x,y,string("x = %f",x));
                break;
               }

    case 'r': { int n = read_int("#random points = ");
                while (n--)
                { double x = rand_int(1,1000)/100.0;
                  double y = rand_int(1,1000)/100.0;
                  S.insert(x,y,string("x = %f",x));
                 }
                std::cout << string("time: %6.2f\n",used_time(T));
                break;
               }

    case 's': { double a=read_real("x0=");
                double b=read_real("x1=");
                double c=read_real("y0=");
                double d=read_real("y1=");

                L = S.range_search(a,b,c,d);

                forall(it,L) std::cout << S.key1(it) << " " << S.key2(it) << "\n";
                newline;

                std::cout << string("time: %6.2f\n",used_time(T));
                break;
              }


    case 'd': { std::cout << "delete point: ";
	        cin >> x >> y;
                S.del(x,y);
                break;
               }

    case 'D': { double a=read_real("x0=");
                double b=read_real("x1=");
                double c=read_real("y0=");
                double d=read_real("y1=");

                L = S.range_search(a,b,c,d);

                forall(it,L)
                { x = S.key1(it);
                  y = S.key2(it);
                  std::cout << "delete: " << x  << " " << y <<"\n";
                  S.del(x,y);
                 }
                newline;

                std::cout << string("time: %6.2f\n",used_time(T));
                break;
              }


    case 'f': { std::cout << "find point: ";
	        cin >> x >> y;
                if (S.lookup(x,y) != nil) std::cout << "yes";
                else std::cout << "no";
                newline;
                break;
               }


    case 'l': { L = S.all_items();
                forall(it,L)
                  std::cout << S.key1(it) << " " << S.key2(it) << "\n";
                newline;
                break;
               }


    } //switch

  } //for

  return 0;
}
