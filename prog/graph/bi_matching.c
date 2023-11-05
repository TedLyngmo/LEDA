#include <LEDA/graph.h>
#include <LEDA/graph_alg.h>


main()
{

GRAPH<int,int> G;
list<node> A,B;
edge e;

test_bigraph(G,A,B);

edge_array<int>  weight1(G);


forall_edges(e,G) G[e] = weight1[e] =  rand_int(0,1000);


if (Yes("show graph? ")) G.print();

float T = used_time();

cout << "MAX_CARD_BIPARTITE MATCHING            ";
cout.flush();
list<edge> M0 = MAX_CARD_BIPARTITE_MATCHING(G,A,B);
cout << string("%5.2f sec    |M| = %d\n",used_time(T), M0.length());

/*
if (Yes("output ? "))  
   { newline;
     forall(e,M0) { G.print_edge(e); newline; }
     newline;
    }
*/

cout << "MAX_CARD_MATCHING                      ";
cout.flush();
list<edge> M1 = MAX_CARD_MATCHING(G);
cout << string("%5.2f sec    |M| = %d\n",used_time(T), M1.length());

/*
if (Yes("output ? "))  
   { newline;
     forall(e,M1) { G.print_edge(e); newline; }
     newline;
    }
*/

newline;

int w = 0;

cout << "MAX_WEIGHT_BIPARTITE_MATCHING <int>    ";
cout.flush();
list<edge> M2 = MAX_WEIGHT_BIPARTITE_MATCHING(G,A,B,weight1);
forall(e,M2)  w += weight1[e];
cout << string("%5.2f sec   W = %d\n",used_time(T),w);

double W = 0;

edge_array<double> weight2(G);

forall_edges(e,G) weight2[e] =  weight1[e]/1000.0; 

cout << "MAX_WEIGHT_BIPARTITE_MATCHING <double> ";
cout.flush();
list<edge> M3 = MAX_WEIGHT_BIPARTITE_MATCHING(G,A,B,weight2);
forall(e,M3)  W += weight2[e];
cout << string("%5.2f sec   W = %.2f\n",used_time(T),W);

return 0;
}
