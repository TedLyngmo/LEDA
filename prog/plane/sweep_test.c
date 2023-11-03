#include <math.h>
#include <LEDA/graph.h>
#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/plane_alg.h>

#define TEST_SWEEP(seglist,G,cmp_count,exact_cmp_count) {\
cmp_count = 0;\
exact_cmp_count = 0;\
cout << string("k = %2d  ",k) << flush;\
float T = used_time();\
SWEEP_SEGMENTS(seglist,G);\
float t = used_time(T);\
cout << string("|V|= %4d ",G.number_of_nodes());\
cout << string("|E|= %4d ",G.number_of_edges());\
cout << string("time = %6.2f sec  ",t);\
cout << string("%6d / %6d   (%.2f %%)",exact_cmp_count,cmp_count,\
                                       100*float(exact_cmp_count)/cmp_count);\
newline; }

main()
{ 
  int N  = read_int("N  = ");
  int k  = read_int("k  = ");
  int l = 0;

  rand_int.set_seed(1234567*N);

  for (; k <= 300; k+=10)
  {
    integer size = (1<<k)/2;
    list<rat_segment> seglist;
    list<segment> seglist1;

    GRAPH<rat_point,rat_segment> G;
    GRAPH<point,segment> G1;

    for(int i=0; i < N; i++)
    { integer x1 = integer::random(k)-size;
      integer y1 = integer::random(k)-size;
      integer w1 = 1;
      integer x2 = integer::random(k)-size;
      integer y2 = integer::random(k)-size;
      integer w2 = 1;
      rat_segment s(rat_point(x1,y1,w1),rat_point(x2,y2,w2));
      seglist.append(s);
      segment s1(x1.todouble(),y1.todouble(),x2.todouble(),y2.todouble());
      seglist1.append(s1);
     }
  

   int point_cmp_count;
   int point_exact_cmp_count;

   rat_point::use_filter = 1;
   rat_segment::use_filter = true;
   TEST_SWEEP(seglist,G,rat_point::cmp_count,rat_point::exact_cmp_count)

   TEST_SWEEP(seglist1,G1,point_cmp_count,point_exact_cmp_count)

   rat_point::use_filter = 0;
   rat_segment::use_filter = false;
   TEST_SWEEP(seglist,G,rat_point::cmp_count,rat_point::exact_cmp_count)

   newline;

   }
  
  return 0;
}

