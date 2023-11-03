#include <LEDA/plane_alg.h>

template <class Point>
struct ch_edge {

Point   source;
Point   target;
ch_edge* succ;
ch_edge* pred;
ch_edge* link;
bool     outside;

ch_edge(const Point& a, const Point& b) : source(a), target(b)
{ outside = true; }

~ch_edge() {}

};

template <class Point>
list<Point>  C_HULL1(list<Point>& L)
{
  // move the four extreme points to the front of |L|


  list_item it;

  list_item xmax_it = L.first();
  list_item xmin_it = L.first();
  list_item ymax_it = L.first();
  list_item ymin_it = L.first();

  forall_items(it,L)
  { if (L[it].xcoord() > L[xmax_it].xcoord()) xmax_it = it;
    if (L[it].xcoord() < L[xmin_it].xcoord()) xmin_it = it;
    if (L[it].ycoord() > L[ymax_it].ycoord()) ymax_it = it;
    if (L[it].ycoord() > L[ymin_it].ycoord()) ymin_it = it;
   }

  L.push(L[xmax_it]); L.del(xmax_it);
  L.push(L[xmin_it]); L.del(xmin_it);
  L.push(L[ymax_it]); L.del(ymax_it);
  L.push(L[ymin_it]); L.del(ymin_it);


  it = L.first();

  Point A = L[it];
  it = L.succ(it);

  Point B = L[it];
  it = L.succ(it);

  ch_edge<Point>* T1 = new ch_edge<Point>(A,B);
  ch_edge<Point>* T2 = new ch_edge<Point>(B,A);

  ch_edge<Point>* last_edge = T2;
  T2->link = T1;
  T1->link = nil;

  T1->succ = T2;
  T1->pred = T2;
  T2->succ = T1;
  T2->pred = T1;

  // scan remaining points


  while (it != nil)
  { Point P = L[it];
    it = L.succ(it);

    ch_edge<Point>* p = (right_turn(A,B,P)) ?  T1 : T2;

    while ( ! p->outside) //(p->succ->pred != p)
    { ch_edge<Point>* r0 = p->pred;
      if (right_turn(r0->source,r0->target,P)) p = r0;
      else { ch_edge<Point>* r1 = p->succ;
             if (right_turn(r1->source,r1->target,P)) p = r1;
             else { p =  nil; break; }
            }
     }

    if (p==nil) continue;  // P lies inside convex hull


    // compute "upper" tangent (p,high->source)

    ch_edge<Point>* high = p->succ;
    while (right_turn(high->source,high->target,P)) high = high->succ;


    // compute "lower" tangent (p,low->target)

    ch_edge<Point>* low = p->pred;
    while (right_turn(low->source,low->target,P)) low = low->pred;


    p = low->succ;  // p = successor of low edge

    // insert two edges between "low" and "high"

    ch_edge<Point>* e_l = new ch_edge<Point>(low->target,P);
    ch_edge<Point>* e_h = new ch_edge<Point>(P,high->source);

    e_h->link = e_l;
    e_l->link = last_edge;
    last_edge = e_h;

    e_h->succ = high;
    e_l->pred = low;
    high->pred = e_l->succ = e_h;
    low->succ  = e_h->pred = e_l;


    // mark edges between low and high as "inside" and define refinements

    while (p != high)
    { ch_edge<Point>* q = p->succ;
      p->pred = e_l;
      p->succ = e_h;
      p->outside = false;
      p = q;
     }

   }


  // return list of vertices

  list<Point> CH;

  CH.append(last_edge->source);
  for(ch_edge<Point>* p = last_edge->succ; p != last_edge; p = p->succ)
     CH.append(p->source);


 // clean up

  while (last_edge)
  { ch_edge<Point>* p = last_edge;
    last_edge = last_edge->link;
    delete p;
   }

  return CH;

}




int main()
{
  int N = read_int("N = ");
  int k = read_int("k = ");

  list<point>      L1;
  list<rat_point>  L2;

  rand_int.set_seed(N*N);

  for(int i = 0; i<N; i++)
  { integer x = integer::random(k);
    integer y = integer::random(k);
    L1.append(point(x.todouble(),y.todouble()));
    L2.append(rat_point(x,y,1));
   }

 { std::cout << "C_HULL1(point)      " << flush;
   float T = used_time();
   list<point> C = C_HULL1(L1);
   std::cout << string("|C| = %d   time = %.2f",C.length(),used_time(T)) << std::endl;
  }

 { std::cout << "CONVEX_HULL(point) " << flush;
   float T = used_time();
   list<point> C = CONVEX_HULL(L1);
   std::cout << string("|C| = %d   time = %.2f",C.length(),used_time(T)) << std::endl;
  }


 { std::cout << "C_HULL1(rat_point)  " << flush;
   float T = used_time();
   list<rat_point> C = C_HULL1(L2);
   std::cout << string("|C| = %d   time = %.2f",C.length(),used_time(T)) << std::endl;
  }

  return 0;
}
