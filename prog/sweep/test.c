#include <math.h>
#include <LEDA/graph.h>
#include <LEDA/integer.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>

#include <LEDA/point.h>
#include <LEDA/segment.h>

extern int cmp_points_count;
extern int exact_cmp_points_count;
extern void sweep0(list<rat_segment>&,GRAPH<rat_point,rat_segment>&,bool=true);

extern void sweep(const list<rat_segment>&, GRAPH<rat_point,rat_segment>&);
extern void sweep(const list<segment>&, GRAPH<point,segment>&);

extern void sweep1(const list<rat_segment>&, GRAPH<rat_point,rat_segment>&);
extern void sweep1(const list<segment>&, GRAPH<point,segment>&);


#define TEST_SWEEP(sweep,seglist,G,cmp_count,exact_cmp_count) {\
cmp_count = 0;\
exact_cmp_count = 0;\
cout << string("%-10s",STRINGIZE(sweep));\
cout << string("k = %2d ",k) << flush;\
float T = used_time();\
sweep(seglist,G);\
float t = used_time(T);\
cout << string("|V|= %4d ",G.number_of_nodes());\
cout << string("|E|= %4d ",G.number_of_edges());\
cout << string("time = %6.2f sec  ",t);\
cout << string("%.2f %%",100*float(exact_cmp_count)/cmp_count);\
newline; }

main()
{ 
  int N  = read_int("N  = ");
  int k  = read_int("k  = ");
  int l = 0;

  rand_int.set_seed(1234567*N);

  //for (; k <= 300; k+=10)
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
      segment s1(point(x1,y1,w1),point(x2,y2,w2));
      seglist1.append(s1);
     }
  

TEST_SWEEP(sweep0,seglist, G, cmp_points_count,exact_cmp_points_count)
TEST_SWEEP(sweep,seglist,G,rat_point::cmp_count,rat_point::exact_cmp_count)
TEST_SWEEP(sweep,seglist1,G1,point::cmp_count,point::exact_cmp_count)
TEST_SWEEP(sweep1,seglist,G,rat_point::cmp_count,rat_point::exact_cmp_count)
TEST_SWEEP(sweep1,seglist1,G1,point::cmp_count,point::exact_cmp_count)
point::use_filter = false;
segment::use_filter = false;
TEST_SWEEP(sweep1,seglist1,G1,point::cmp_count,point::exact_cmp_count)
newline;

   }
  
  return 0;
}

