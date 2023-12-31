/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _embedding.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


/*******************************************************************************
*                                                                              *
*  EMBEDDING   (straight line embedding)                                       *
*                                                                              *
*  K. Mehlhorn (1989)                                                          *
*                                                                              *
*******************************************************************************/

#include <LEDA/graph_alg.h>


const int A = -2; 
const int B = -1; 

static node_array<list_item>  Classloc;
static node_array<int>  ord, labelled, Class;
static node_array<node> first, second, last;
static edge_array<edge> reversal;

void label_node(graph& G, list<node>& L, int& count,
                list<node>& Al,list<node>& Bl,list<node>*& Il,
                node v, node c)

{ // labels the node v; c is the special node which is to be labelled
  // last; the details are described in lemma 10
  edge e;
  L.append(v);
  ord[v]=count++; 
  labelled[v]=1;

  forall_adj_edges(e,v)
  { edge e1 = reversal[e];
    node tt = target(e);
    int i;

    if (labelled[tt] && !labelled[target(G.cyclic_adj_succ(e))])
      { first[v]=tt; 
        second[v]=target(G.cyclic_adj_pred(e));
       }

    if (labelled[tt] && !labelled[target(G.cyclic_adj_pred(e))]) last[v]=tt;

    if (!labelled[tt] && (tt != c))
      { if (Class[tt] == A) 
          { Al.del(Classloc[tt]); 
            Classloc[tt] = Bl.push(tt);
            Class[tt]=B;
           }  
        else 
          { if (Class[tt] == B) 
               { Bl.del(Classloc[tt]);
                 i = 2-labelled[target(G.cyclic_adj_succ(e1))]
                      -labelled[target(G.cyclic_adj_pred(e1))];
                }
            else 
               { i=Class[tt];
                 Il[i].del(Classloc[tt]);
                 i = i+1-labelled[target(G.cyclic_adj_succ(e1))]
                        -labelled[target(G.cyclic_adj_pred(e1))];
                }

             Class[tt]=i;
             Classloc[tt]=Il[i].push(tt);

           }//end else case

      }//end if

  }//end 

}//end of label_node


void compute_labelling(graph& G,list<node>& L, list<node>& Pi)
{ /* computes the ordering of lemma 10 in List L ,the sequence pi
     in List Pi, the function L^-1(v) in Array ord, and the functions
     first,second,last of lemma 11 in the corresponding Arrays
   */
  node v,a,b,c;

  /* zuerst berechne ich die drei Knoten,die am Rand des aeusseren 
     Gebiets liegen sollen
   */

  a=G.first_node();
  list<edge> temp = G.adj_edges(a);
  b = target(temp.pop());
  c = target(temp.pop());


/*
  node_array<int> labelled(G,0);
*/

  labelled.init(G,0);

  int count = 0;
  
  list<node> Al ;

/*
  node_array<int> Class(G); 
  node_array<list_item> Classloc(G);
*/

  Class.init(G); 
  Classloc.init(G);

  forall_nodes(v,G) { Classloc[v] = Al.push(v);Class[v]=A;}

  list<node> Bl;
  list<node>* Il = new list<node>[G.number_of_nodes()];
  
  label_node(G,L,count,Al,Bl,Il,a,c);
  label_node(G,L,count,Al,Bl,Il,b,c);

  while (v=Il[1].pop()) label_node(G,L,count,Al,Bl,Il,v,c);

  label_node(G,L,count,Al,Bl,Il,c,c);

   //nun berechne ich noch first second und last des Knoten c 
  first[c]=a;
  last[c]=b;

  edge e;
  forall_adj_edges(e,c) if (target(e)==a) second[c]=target(G.cyclic_adj_pred(e));
  
 //nun die Folge Pi
  node_array<list_item> Piloc(G);
  Piloc[a] = Pi.push(a);
  Piloc[b] = Pi.append(b);
  forall(v,L) if (v != a && v != b) Piloc[v] = Pi.insert(v,Piloc[second[v]],-1);

}//end of compute_labelling 

void move_to_the_right(list<node>& Pi, node v, node w, 
                       node_array<int>& ord, node_array<int>& x)

{ //increases the x-coordinate of all nodes which follow w in List Pi
  //and precede v in List L,i.e., have a smaller ord value than v
  int seen_w = 0;
  node z;
  forall(z,Pi)
  { if (z==w) seen_w=1;
    if (seen_w && (ord[z]<ord[v])) x[z]=x[z]+1;
  }
}


int STRAIGHT_LINE_EMBEDDING(graph& G,node_array<int>& x, node_array<int>& y)
{
 // computes a straight-line embedding of the planar map G into
 // the 2n by n grid. The coordinates of the nodes are returned
 // in the Arrays x and y. Returns the maximal coordinate.

list<node> L;
list<node> Pi;
list<edge> TL;

node v;
edge e;
int maxcoord=0;

/*
node_array<int>  ord(G);
node_array<node> first(G), second(G), last(G);
*/

ord.init(G);
first.init(G);
second.init(G);
last.init(G);


TL = TRIANGULATE_PLANAR_MAP(G);

/*
edge_array<edge> reversal(G);
*/

reversal.init(G);
compute_correspondence(G,reversal);

compute_labelling(G,L,Pi);


//I now embed the first three nodes

v = L.pop();
x[v]=y[v]=0;

v=L.pop();
x[v]=2;y[v]=0;

v=L.pop();
x[v]=y[v]=1;

//I now embed the remaining nodes

while (v=L.pop())
 { // I first move the nodes depending on second[v] by one unit
   // and the the nodes depending on last[v] by another unit to the 
   // right

   move_to_the_right(Pi,v,second[v],ord,x);
   move_to_the_right(Pi,v,last[v],ord,x);

   // I now embed v at the intersection of the line with slope +1
   // through first[v] and the line with slope -1 through last[v]

   int x_first_v = x[first[v]];
   int x_last_v =  x[last[v]];
   int y_first_v = y[first[v]];
   int y_last_v =  y[last[v]];

   x[v]=(y_last_v - y_first_v + x_first_v + x_last_v)/2;
   y[v]=(x_last_v - x_first_v + y_first_v + y_last_v)/2;
 }

// delete triangulation edges

forall(e,TL) G.del_edge(e);

forall_nodes(v,G) maxcoord = Max(maxcoord,Max(x[v],y[v]));
return maxcoord;

}



int STRAIGHT_LINE_EMBEDDING(graph& G,node_array<double>& x, node_array<double>& y)
{ node v;
  node_array<int> x0(G), y0(G);

  int result = STRAIGHT_LINE_EMBEDDING(G,x0,y0);

  forall_nodes(v,G) { x[v] = x0[v];
                      y[v] = y0[v]; }
  return result;
}
