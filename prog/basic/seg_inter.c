#include <LEDA/integer.h>


bool intersection(integer s1x1, integer s1y1, integer s1x2, integer s1y2,
                  integer s2x1, integer s2y1, integer s2x2, integer s2y2,
                  integer& X, integer& Y, integer& W)

{ 
  // decides whether |s| and |this| segment intersect and, if so, 
  // returns the intersection in |I|. It is assumed that both segments 
  // have non-zero length 

  integer dx1 = s1x2 - s1x1;
  integer dy1 = s1y2 - s1y1;

  integer dx2 = s2x2 - s2x1;
  integer dy2 = s2y2 - s2y1;

  W = dy1*dx2 - dy2*dx1;

  if (W == 0) return false;   // $slope(s) = slope(this)$

  integer c1 =   s1x2*s1y1 -  s1x1*s1y2;
  integer c2 =   s2x2*s2y1 -  s2x1*s2y2;


  /* The underlying lines intersect in a point $p = (x,y,w)$.
     We still have to test whether $p$ lies on both segments.
     $p$ lies on $s$ ($this$)if its x-coordinate $x$ compares 
     diffently with the x-coordinates of the two endpoints of $s$ ($this).
   */

  X = c2*dx1-c1*dx2;

  if (sign(X-s1x1*W) == sign(X-s1x2*W) || 
      sign(X-s2x1*W) == sign(X-s2x2*W )) return false;

  Y = c2*dy1-c1*dy2;

  return true;

}


main()
{


   int n = read_int("n = ");
   int i,j;

   long* x1 = new long[n];
   long* x2 = new long[n];
   long* y1 = new long[n];
   long* y2 = new long[n];

   rand_int.set_seed(12345*n);

   for(i = 0; i < n; i++)
   { x1[i] = rand_int();
     x2[i] = rand_int();
     y1[i] = rand_int();
     y2[i] = rand_int();
    }

  float T = used_time();

  int s = 0;
  for(i=0; i<n; i++)
  for(j=i+1; j<n; j++)
  { integer x,y,w;
    if(intersection(integer(x1[i]),integer(x2[i]),integer(y1[i]),integer(y2[i]),
                    integer(x1[j]),integer(x2[j]),integer(y1[j]),integer(y2[j]),
                    x,y,w)) s++;
   }
   cout << string("integer:    s = %3d    time = %5.2f",s,used_time(T)) << endl;
   return 0;
}
