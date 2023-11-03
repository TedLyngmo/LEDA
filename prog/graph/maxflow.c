#include <LEDA/graph.h>
#include <LEDA/graph_alg.h>



int main(int argc, char** argv)
{

graph G;

cmdline_graph(G,argc,argv);

edge_array<int>  cap(G,0);
edge_array<int>  cost(G,0);
edge_array<int>  flow(G,0);
edge_array<double> cap1(G,0);
edge_array<double> cost1(G,0);
edge_array<double> flow1(G,0);

edge e;
forall_edges(e,G) cap1[e] = cap[e] = rand_int(1,1000);
forall_edges(e,G) cost1[e] = cost[e] = rand_int(-1000,1000);

node s = G.first_node();
node t = G.last_node();

float T = used_time();

cout << "MAX_FLOW<int>             " << flush;
int f = MAX_FLOW(G,s,t,cap,flow) ;
cout << string("time: %6.2f sec  f = %d",used_time(T),f) << std::endl;

cout << "MAX_FLOW<double>          " << flush;
double f1 = MAX_FLOW(G,s,t,cap1,flow1);
cout << string("time: %6.2f sec  f = %.2f",used_time(T),f1) << std::endl;

cout << "MIN_COST_MAX_FLOW<int>    " << flush;
f = MIN_COST_MAX_FLOW(G,s,t,cap,cost,flow) ;
int c = 0;
forall_edges(e,G) c += cost[e]*flow[e];
cout << string("time: %6.2f sec  f = %d    c = %d",used_time(T),f,c) << std::endl;

return 0;

}
