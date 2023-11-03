#include <LEDA/point_set.h>




int main()
{

  point_set<string> S;

  list<ps_item> L;
  ps_item       it=nil;
  point         p;
  char          ch;



  while (ch != 'q')
  {
    std::cout << "(r/s/f/n/i/d/D/l/q): ";
    cin  >> ch;

    float T  = used_time();

    switch(ch) {

    case 'i':  { std::cout << "insert point: ";
                 cin >> p;
                 S.insert(p,string("x = %f",p.xcoord()));
                 break;
                }

     case 'n': { std::cout << "nearest neighbor: ";
	         cin >> p;
                 it = S.nearest_neighbor(p);
                 if (it!=nil) std::cout << S.key(it);
                 else std::cout << "Empty point set.\n";
                 newline;
                 break;
               }


    case 'r' : { int n = read_int("#random points = ");
                 while (n--)
                 { double x = rand_int(1,1000)/100.0;
                   double y = rand_int(1,1000)/100.0;
                   S.insert(point(x,y),string("x = %f",x));
                  }
                 std::cout << string("time: %6.2f\n",used_time(T));
                 break;
                }

    case 's': { double a=read_real("x0=");
                double b=read_real("x1=");
                double c=read_real("y0=");
                double d=read_real("y1=");

                L = S.range_search(a,b,c,d);

                forall(it,L) std::cout << S.key(it) << " " << S.inf(it) << "\n";
                newline;

                std::cout << string("time: %6.2f\n",used_time(T));
                break;
              }


    case 'd': { std::cout << "delete point: ";
	        cin >> p;
                S.del(p);
                break;
               }

    case 'D': { double a=read_real("x0=");
                double b=read_real("x1=");
                double c=read_real("y0=");
                double d=read_real("y1=");

                L = S.range_search(a,b,c,d);

                forall(it,L)
                { point p = S.key(it);
                  std::cout << "delete: " << p <<"\n";
                  S.del(p);
                 }
                newline;

                std::cout << string("time: %6.2f\n",used_time(T));
                break;
              }


    case 'f': { std::cout << "find point: ";
	        cin >> p;
                if (S.lookup(p) != nil) std::cout << "yes";
                else std::cout << "no";
                newline;
                break;
               }


    case 'l': { L = S.all_items();
                forall(it,L)
                  std::cout << S.key(it) << " " << S.inf(it) << "\n";
                newline;
                break;
               }


    } //switch

  } //for

  return 0;
}
