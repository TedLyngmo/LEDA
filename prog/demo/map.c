#include <LEDA/basic.h>
#include <LEDA/slist.h>
#include <LEDA/plane.h>
#include <LEDA/window.h>
 
#include<math.h>
 
const int WORDS = 6;    // maximal size of placement vectors (in words)

const int N     = 96;   // maximal number of input points = 16 * WORDS

typedef unsigned long word;

class PLACEMENT
{
  word A[WORDS];

  word cache;      // cache of 16 last written entries
 
  public:

  PLACEMENT();
  PLACEMENT(const PLACEMENT&);

  word get_last_word() const { return cache;}

  word get_last_word(int i) const 
  { return (i>0) ? (cache << ((16-i)<<1)) : 0; }
  
  int read(int i) const 
  { return (A[i>>4] >> ((i&15)<<1)) & 3; }
 
  void write(int i, int pos) 
  { A[i>>4] |= (pos << ((i&15)<<1));
    cache <<=2;
    cache |= pos;
   } 
 
  LEDA_MEMORY(PLACEMENT)
 
};


PLACEMENT::PLACEMENT()
{ for(int i = 0; i < WORDS; i++) A[i] = 0; }

PLACEMENT::PLACEMENT(const PLACEMENT& P)
{ cache = P.cache;
  for(int i = 0; i < WORDS; i++) A[i] = P.A[i]; 
}
 
 
typedef PLACEMENT* placement;
 

static window W(600,650);
 
static color node_color1 = blue;
static color node_color2 = red;
static color node_color3 = green;
static color rect_color  = yellow;
 
void draw_rec(window& W, double x1, double y1, double x2, double y2)
{ if (!W.mono())
      W.draw_filled_rectangle(x1,y1,x2,y2,rect_color);
  W.draw_rectangle(x1,y1,x2,y2);
 }
 
 
void draw_config(window& W, const list<point>& L, PLACEMENT& P, double sig, 
                                                                int maxp = -1)
{
        /*--------*--------*
         |        |        |
         |   0    |    1   |
         |        |        |
         *--------p--------*
         |        |        |
         |   2    |    3   |
         |        |        |
         *--------*--------*/
 
  int   i=0;
  point p;
 
  if (maxp <0) maxp = L.length();
 
  W.clear();
 
  forall(p,L)
  {
    double x = p.xcoord();
    double y = p.ycoord();
 
    int  pos = P.read(i++);
 
    switch(pos)
    { case 0 : draw_rec(W,x,y,x-sig,y+sig);
               break;
 
      case 1 : draw_rec(W,x,y,x+sig,y+sig);
               break;
      
      case 2 : draw_rec(W,x,y,x-sig,y-sig);
               break; 
      
      case 3 : draw_rec(W,x,y,x+sig,y-sig);
               break;
    }

    if (i > maxp) break;
  }
 
  forall(p,L) W.draw_filled_node(p,node_color1);
 
}
 
 
 
 
int** D[N][4];   // D[N][4][N][4]
                 //  
                 // D[p][i][q][j] gives maximal possible sigma for square at 
                 // point p in position i and square at point q in position j
 
 
void initialize_matrix(const list<point>& p_list)
{
  list_item it1, it2;
 
  int n = p_list.length();
 
  int i,j,p1,p2;
 
  for(i=0;i < n; i++)
     for(p1=0;p1 < 4; p1++)
     { int** ptr = new int*[n];
       D[i][p1] = ptr;
       for(j=0; j<n; j++) ptr[j] = new int[4];
       }
 
 
  i = 0;
  forall_items(it1,p_list)
  { 
    point p = p_list[it1];
 
    j = 0;
    forall_items(it2,p_list)
     { 
       if (it1==it2) break;
 
       point q = p_list[it2];
 
 
       int xrel  = int(p.xcoord() - q.xcoord());
       int yrel  = int(p.ycoord() - q.ycoord());
       int xdist = int(fabs(xrel));
       int ydist = int(fabs(yrel));
       int max_xy = Max(xdist,ydist);
 
       int d_00  = max_xy;
       int d_01  = (xrel>0) ? Max(xdist/2,ydist) : MAXINT;
       int d_01r = (xrel<0) ? Max(xdist/2,ydist) : MAXINT;
       int d_02  = (yrel<0) ? Max(xdist,ydist/2) : MAXINT;
       int d_02r = (yrel>0) ? Max(xdist,ydist/2) : MAXINT;
       int d_03  = (xrel>0 && yrel<0) ? max_xy/2 : MAXINT;
       int d_03r = (xrel<0 && yrel>0) ? max_xy/2 : MAXINT;
       int d_12  = (xrel<0 && yrel<0) ? max_xy/2 : MAXINT;
       int d_12r = (xrel>0 && yrel>0) ? max_xy/2 : MAXINT;
 
 
       for(p1=0; p1<4;p1++)
         for(p2=0; p2<4;p2++)
          switch(p1 - p2)
          {
            case  0 : D[i][p1][j][p2] = d_00;
                      break;
     
            case -1 : D[i][p1][j][p2] = (p1==1) ? d_12 : d_01;
                      break;
     
            case  1 : D[i][p1][j][p2] = (p1==2) ? d_12r : d_01r;
                      break;
     
            case -2 : D[i][p1][j][p2] = d_02;
                      break;
     
            case  2 : D[i][p1][j][p2] = d_02r;
                      break;
     
            case -3 : D[i][p1][j][p2] = d_03;
                      break;
     
            case  3 : D[i][p1][j][p2] = d_03r;
                      break;
          }
    
      j++;
     }
 
    i++;
 
   }
 
}
 
 
 

bool find_placement(list<point>& L, int sigma, PLACEMENT& P)
{ 
  // determines whether a placement is possible and returns it in P 
  // P is unchanged if there is no placment possible 
  
  slist<placement> T,Tnew;
 
  int n = L.length();
 
  placement v;
 
  register int   p,q,k,i;
  register word  buf,plw;
  register int** dist_to_p;
 
  point a;
 
  float X[N];
  float Y[N];
 
  p = 0;
 
  forall(a,L) 
  { X[p] = a.xcoord();
    Y[p] = a.ycoord();
    p++;
   }
 
  placement Pnew = new PLACEMENT;

 // we perform a sweep the strip consists of points q .. p-1 
 
 
 // initialization for the sweep; the strip is empty; the first point to 
 // enter is point 0; T consists of a single placement which is undefined 
 // for all points 
 
 
 p = 0; // next point to enter the strip
 q = 0; // next point to leave the strip
 
 T.append(Pnew);        
 
 // The sweep: 
 // we proceed as long as there is still a point to be processed and 
 // the set T of legal placements is not empty 

 while (p < L.length() && !T.empty())
 { 
   // we decide whether p enters the strip or q leaves the strip 
 
   if (p == 0 || X[p]-X[q] < 2*sigma)
     { 
       // p enters the strip
       // we combine the four possible placements of p 
       // with all placements in T

       if (p-q > 16) error_handler(1,"too many points in strip");

       W.draw_filled_node(X[p],Y[p],node_color3);

       for(i=0; i<4; i++)
       { 
         dist_to_p = D[p][i];
 
         forall(v,T)
         { // check whether v[q...p-1] can be extended by placement i for p 
 
           for(k = p-1, buf = v->get_last_word(); k >= q; k--, buf >>= 2)
             if (dist_to_p[k][buf&3] < sigma) break;
 
           if (k < q)                
           { // position i for p is compatible with placement vector v
             Pnew = new PLACEMENT(*v);
             Pnew->write(p,i);
             Tnew.append(Pnew); 
  
             if (W.get_button() != 0) 
             { color save = rect_color;
               if (!W.mono()) rect_color = violet;
               draw_config(W,L,*Pnew,sigma,p);
               rect_color = save;
               for(int i=q; i<=p; i++)
                  W.draw_filled_node(X[i],Y[i],node_color2);
               W.draw_filled_node(X[p],Y[p],node_color3);
               W.read_mouse();
               W.clear();
               for(i=0; i<q; i++)
                  W.draw_filled_node(X[i],Y[i],node_color1);
               for(i=q; i<=p; i++)
                  W.draw_filled_node(X[i],Y[i],node_color2);
               for(i=p+1; i<n; i++)
                  W.draw_filled_node(X[i],Y[i],node_color1);
              }
  
            }
 
          }

        }

       W.draw_filled_node(X[p],Y[p],node_color2);
 
       p++;
 
       forall(v,T) delete v;
 
      } 
    else 
     { // q leaves the strip

       W.draw_filled_node(X[q],Y[q],node_color1);

       q++;
 
       // remove placement vectors identical in [q+1 ... p] from T
 
       plw = T.head()->get_last_word(p-q);
 
       Tnew.append(T.pop());
 
       forall(v,T)
       { buf = v->get_last_word(p-q);
         if (buf == plw)
           delete v;
         else 
           { plw = buf;
             Tnew.append(v);
            }
        }
      }
 
    T.clear();
    T.conc(Tnew); // clears Tnew

    W.draw_text(-80,-60,string("k = %2d   |T| = %5d   ",p-q+1,T.length()));
 
 } // end of sweep 

 while (q < p)
 { W.draw_filled_node(X[q],Y[q],node_color1);
   q++;
  }

 
 if (T.empty()) 
     return false;
 else 
    { P = *(T.head());
      forall(v,T) delete v;
      return true;
     }
 
} // end of find_placement 
 
 
 
main()
{ 
  // In the first implementation we deal only with points 
  // whose coordinates are integers in the range 1 .. 999;
  // the algorithms consists of two steps: 
  // In the first step we generate the problem and in the second step we 
  // determine the optimal sigma by binary search 
  // generation of the problem; we ask the user for the number of points
  // then generate the appropriate number of random points 
 
 
  W.init(-100,1100,-100);
  W.set_node_width(5);
  W.set_text_mode(opaque);
 
  if (W.mono()) 
  { node_color1 = white;
    node_color2 = black;
   }
 
  int n = 50;
  int input = 0;
  int grid_width = 0;
 
  list<point> L;
 
  panel P;
 
  P.text_item("                                      ");
  P.text_item("        A Plane Sweep Algorithm       ");
  P.text_item("                 for a                ");
  P.text_item("       Geometric Packing Problem      ");
  P.text_item("                                      ");
  P.text_item("                  ...                 ");
  P.text_item("                  ...                 ");
  P.text_item("                                      ");
  P.int_item("points", n,1,80);
  P.int_item("grid", grid_width, 0,40,10);
 
  P.button("random");
  P.button("mouse");
  P.button("quit");
 
 
  for(;;)
  {
    int but = P.open(0,0);
 
    W.clear();
    W.set_grid_mode(grid_width);
    L.clear();
 
    switch(but)  {
 
    case 0: { for(int i=0; i<n; i++) 
              { double x =  rand_int(1,999);
                double y =  rand_int(1,999);
                L.append(point(x,y));
                W.draw_filled_node(x,y,node_color1);
               }
               break;
             }
 
     case 1: { point p;
               while (W >> p) 
               { L.append(p);
                 W.draw_filled_node(p,node_color1);
                }
               break;
              }
 
 
     case 2: { exit(0);
               break;
              }
             
    }
 
    n = L.length();
 
    W.set_frame_label(string("%d points",n));
 
 
 
     // sort points  and initialize distance matrix
   
     L.sort();
 
     initialize_matrix(L);
   
     PLACEMENT PL;
 
     int low  =   1;
     int high = 999;
 
     find_placement(L,low,PL);
 
     // binary search
   
     while (high > low+1)
     { 
       // Invariant:  a) PL is placement for "sigma = low"
       //             b) no placement possible for "sigma = high"
   
       int mid = (high + low)/2;
   
       W.del_message();
       W.message(string("%3d <= sigma < %3d      %3d ??",low, high, mid));
   
       if (find_placement(L,mid,PL))
          { draw_config(W,L,PL,mid);
            low = mid;
           }
        else 
            high = mid;
      }
   
     // end of the binary search 
     // low is the optimal side length, PL the corresponding placement 
   
     W.del_messages();
     W.message(string("sigma = %d",low));
   }
                
  return 0;
}
