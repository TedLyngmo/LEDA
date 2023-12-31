#define LEDA_CHECKING_OFF

#include <LEDA/graph.h>
#include <LEDA/map.h>
#include <LEDA/node_map.h>
#include <LEDA/h_array.h>
#include <LEDA/d_array.h>
#include <LEDA/dph_array.h>

main()
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

   cout << "node_array: " <<flush;
   forall_nodes(v,G) 
   { A[v] = count++;
     sum += A[v];
    }
    cout << string("%5.2f sec",used_time(T)) << endl;

   count = 0;
   cout << "map:        " <<flush;
   forall_nodes(v,G) 
   { M[v] = count++;
     sum += M[v];
    }
    cout << string("%5.2f sec",used_time(T)) << endl;


   count = 0;
   cout << "node_map:   " <<flush;
   forall_nodes(v,G) 
   { NM[v] = count++;
     sum += NM[v];
    }
    cout << string("%5.2f sec",used_time(T)) << endl;


   count = 0;
   cout << "h_array:    " <<flush;
   forall_nodes(v,G) 
   { C[v] = count++;
     sum += C[v];
    }
   cout << string("%5.2f sec",used_time(T)) << endl;


   count = 0;
   cout << "d_array:    " <<flush;
   forall_nodes(v,G) 
   { D[v] = count++;
     sum += D[v];
    }
   cout << string("%5.2f sec",used_time(T)) << endl;


   count = 0;
   cout << "dph_array:  " <<flush;
   forall_nodes(v,G) 
   { E[v] = count++;
     sum += E[v];
    }
   cout << string("%5.2f sec",used_time(T)) << endl;


   newline;
   cout << sum << endl;

  return 0;
}
