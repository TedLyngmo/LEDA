#include <LEDA/planar_map.h>
#include <LEDA/graph_alg.h>

void print_face(const PLANAR_MAP<int,int>& M, face f)
{ cout << string("F%d: ",M.inf(f));
  list<node> L1 = M.adj_nodes(f);
  node v;
  forall(v,L1) M.print_node(v);
  newline;
 }


main()

{  GRAPH<int,int> G;
   node v;
   edge e;
   face f;

   test_graph(G);

   edge_array<edge> rev(G);
   Is_Bidirected(G,rev);

   list<edge> L;
   forall_edges(e,G) if (rev[e] == nil) L.append(e);

   forall(e,L) G.new_edge(target(e),source(e));

   if (!PLANAR(G,true))
   { cerr << "G is not planar!\n";
     exit(1);
    }

   int count = 0;
   forall_nodes(v,G) G[v] = count++;
   count = 0;
   forall_edges(e,G) G[e] = count++;


   PLANAR_MAP<int,int>  M(G);

   count = 0;
   forall_faces(f,M) M[f] = count++;

   newline;
   forall_faces(f,M) print_face(M,f);

   newline;
   cout << "faces adjacent to nodes" << endl;

   forall_nodes(v,M)
   { M.print_node(v);
     cout << " : ";
     forall_adj_faces(f,v) cout << string("F%d ",M[f]);
     newline;
    }

/*
   M.triangulate();
   forall_faces(f,M) print_face(M,f);
   newline;
*/


   newline;
   while( Yes("split faces ? ") )
   { list<face> FL= M.all_faces();
      int i = 0;
      forall(f,FL)  M.new_node(f,--i);
   
      forall_faces(f,M) print_face(M,f);
      newline;
   }
   
   
   list<edge> E;
   edge_array<bool> marked(M,false);

   forall_edges(e,M)
    if (!marked[e])
    { E.append(e);
      marked[M.reverse(e)] = true;
     } 


   forall(e,E)
   { Yes("del_edge ");
     M.del_edge(e);
     forall_faces(f,M) print_face(M,f);
     newline;

  }

}
