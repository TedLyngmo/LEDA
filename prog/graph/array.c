#define LEDA_CHECKING_OFF

#include <LEDA/graph.h>
#include <LEDA/map.h>
#include <LEDA/node_map.h>
#include <LEDA/h_array.h>
#include <LEDA/d_array.h>
#include <LEDA/dph_array.h>

int main()
{

   graph G;
   node v;
   int sum = 0;

   int n = read_int("n = ");

   random_graph(G,n,0);

   node_array<int>   A(G,0);
   map<node,int>     M(0,sizeof(node_struct));
   h_array<node,int> C(0,sizeof(node_struct));
   d_array<node,int> D(0);
   dph_array<node,int> E(0);

   node_map<int>     NM(G);


   float T = used_time();

   int count = 0;

   std::cout << "node_array: " <<flush;
   forall_nodes(v,G)
   { A[v] = count++;
     sum += A[v];
    }
    std::cout << string("%5.2f sec",used_time(T)) << std::endl;

   count = 0;
   std::cout << "map:        " <<flush;
   forall_nodes(v,G)
   { M[v] = count++;
     sum += M[v];
    }
    std::cout << string("%5.2f sec",used_time(T)) << std::endl;


   count = 0;
   std::cout << "node_map:   " <<flush;
   forall_nodes(v,G)
   { NM[v] = count++;
     sum += NM[v];
    }
    std::cout << string("%5.2f sec",used_time(T)) << std::endl;


   count = 0;
   std::cout << "h_array:    " <<flush;
   forall_nodes(v,G)
   { C[v] = count++;
     sum += C[v];
    }
   std::cout << string("%5.2f sec",used_time(T)) << std::endl;


   count = 0;
   std::cout << "d_array:    " <<flush;
   forall_nodes(v,G)
   { D[v] = count++;
     sum += D[v];
    }
   std::cout << string("%5.2f sec",used_time(T)) << std::endl;


   count = 0;
   std::cout << "dph_array:  " <<flush;
   forall_nodes(v,G)
   { E[v] = count++;
     sum += E[v];
    }
   std::cout << string("%5.2f sec",used_time(T)) << std::endl;


   newline;
   std::cout << sum << std::endl;

  return 0;
}
