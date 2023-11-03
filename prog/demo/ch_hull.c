#include <LEDA/plane.h>
#include <LEDA/window.h>

window W;

list<point> C_HULL(list<point> L)
{ 
  if (L.length() < 3) return L;

  list<point> CH;
  list_item last;
  point p;

  L.sort();  // sort points lexicographically


  // initialize convex hull with first two points

  p = L.pop();
  CH.append(p);
  while (L.head() == p) L.pop();
  last = CH.append(L.pop());


  // scan remaining points

  forall(p,L)
  {
    if (p == CH[last]) continue;  // multiple point

    // compute upper tangent (p,high)

    list_item high = last;
    list_item it = CH.cyclic_pred(high);

    while (left_turn(CH[it],CH[high],p))
    { high = it;
      it = CH.cyclic_pred(high);
     }


    // compute lower tangent (p,low)

    list_item low = last;
    it = CH.cyclic_succ(low);

    while (right_turn(CH[it],CH[low],p))
    { low = it;
      it = CH.cyclic_succ(low);
     }


    // remove all points between high and low

    if (high != low)
    { it = CH.cyclic_succ(high);

      while (it != low)
      { CH.del(it);
        it = CH.cyclic_succ(high);
       }
     }

    // insert new point after "high"

    last = CH.insert(p,high);

   }

  return CH;
}
     


main()
{
  //window W;
  W.init(-100,100,-100);
  W.set_node_width(3);

  int N = 500;

  panel P("convex hull");

  P.int_item("# points",N,1,2000);
  int b1 = P.button("mouse");
  int b2 = P.button("random");
  int b3 = P.button("quit");

  for(;;)
  { 
    list<point> L;
    point p,q;

    int but = P.open(0,0);

    W.clear();

    if (but == b1)
      while (W >> p)
      { W.draw_filled_node(p,blue2);
        L.append(p);
       }

    if (but == b2)
      for(int i = 0; i<N; i++) 
      { point p(rand_int(-90,90),rand_int(-90,90));
        W.draw_filled_node(p,blue2);
        L.append(p);
       }

    if (but == b3) break;
  
    list<point> C = C_HULL(L);

    W.set_mode(xor_mode);
    W.draw_filled_polygon(C,yellow);
    W.set_mode(src_mode);
  
  }
   
 return 0;
}

