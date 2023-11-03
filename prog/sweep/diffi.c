#include <LEDA/graph.h>
#include <LEDA/window.h>
#include <LEDA/integer.h>
#include <LEDA/rat_point.h>
#include <LEDA/rat_segment.h>
#include <LEDA/point.h>
#include <LEDA/segment.h>

extern void sweep0(list<rat_segment>&,GRAPH<rat_point,rat_segment>&,bool=true);
extern void sweep(const list<rat_segment>&,GRAPH<rat_point,rat_segment>&);
extern void sweep(const list<segment>&,GRAPH<point,segment>&);

extern int cmp_points_count;
extern int exact_cmp_points_count;

main()
{ 
  list<rat_segment> seglist;
  GRAPH<rat_point,rat_segment>   G;

  list<segment> seglist1;
  GRAPH<point,segment> G1;

  integer size;
  int k = 0;

  int N = read_int("N = ");
  int s = read_int("s = ");

  for (size = 1024, k=10; k <= 100; k+=5, size <<=5)
  {
   size +=  rand_int(1,23);
  
   seglist.clear(); 
   G.clear();
   G1.clear();
   
   integer y = size;
   integer d = 2*size/(N-1);
   
   for(int i=0; i < N; i++)
   { int rx1 = rand_int(-s,s);
     int ry1 = rand_int(-s,s);
     int rx2 = rand_int(-s,s);
     int ry2 = rand_int(-s,s);
   
     rat_point p(size+rx1,2*size+y+ry1,1);
     rat_point q(3*size+rx2,2*size-y+ry2,1);
     seglist.append(rat_segment(p,q));
     seglist1.append(segment(p.xcoord(),p.ycoord(),q.xcoord(),q.ycoord()));
     y += d;
    }
   

    cmp_points_count = 0;
    exact_cmp_points_count = 0;
    cout << string("sweep0: k = %2d ",k) << flush;
    float T = used_time();
    sweep0(seglist,G);
    float t = used_time(T);
    cout << string("|V|= %4d ",G.number_of_nodes());
    cout << string("|E|= %4d ",G.number_of_edges());
    cout << string("time = %6.2f sec  ",t);
    cout << string("%.2f %%",float(exact_cmp_points_count)/cmp_points_count);
    newline;

    rat_point::cmp_count = 0;
    rat_point::exact_cmp_count = 0;
    cout << string("sweep1: k = %2d ",k) << flush;
    T = used_time();
    sweep(seglist,G);
    t = used_time(T);
    cout << string("|V|= %4d ",G.number_of_nodes());
    cout << string("|E|= %4d ",G.number_of_edges());
    cout << string("time = %6.2f sec  ",t);
    cout << string("%.2f %%",float(exact_cmp_points_count)/cmp_points_count);
    newline;

/*
    cout << string("sweep2: k = %2d ",k) << flush;
    T = used_time();
    sweep(seglist1,G1);
    t = used_time(T);
    cout << string("|V|= %4d ",G1.number_of_nodes());
    cout << string("|E|= %4d ",G1.number_of_edges());
    cout << string("time = %6.2f sec  ",t);
    newline;


    cout << string("LEDA  : k = %2d ",k) << flush;
    T = used_time();
    SWEEP_SEGMENTS(seglist1,G2);
    t = used_time(T);
    cout << string("|V|= %4d ",G2.number_of_nodes());
    cout << string("|E|= %4d ",G2.number_of_edges());
    cout << string("time = %6.2f sec  ",t);
    newline;
*/
    newline;

   }
  
  return 0;
}

