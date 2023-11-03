#include <LEDA/graph_alg.h>


int main(int argc,char** argv)
{


node v;
int  i = 0;

GRAPH<int,int> G;

cmdline_graph(G,argc,argv);

node_array<int> xcoord(G), ycoord(G);


cout << "Planarity Test ... " << flush;

float T = used_time();

bool plan = PLANAR(G);

cout << string(" time = %.2f sec", used_time(T)) << std::endl;

if (plan)
    std::cout << "Graph is planar" << std::endl;
else
   { std::cout << "Graph is not planar." << std::endl;
     std::cout << "I compute a a Kuratowsky Subgraph ..." << std::endl;
     list<edge> L;
     edge e;

     PLANAR(G,L);
     forall(e,L)
     { G.print_edge(e);
       newline;
      }
     newline;

    }

  return 0;

}
