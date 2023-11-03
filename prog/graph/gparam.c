
#include <LEDA/graph.h>
#include <LEDA/integer.h>
#include <LEDA/real.h>
#include <LEDA/vector.h>

struct Simple1 {
   string str;
};

void Print(const Simple1& x, ostream& out) { out << x.str; }


struct Simple2 {
   integer i;
};

void Print(const Simple2& x, ostream& out) { out << x.i; }

struct Simple3 {
   real r;

Simple3(int i) { r = i; }
Simple3() { r = 0; }
};

void Print(const Simple3& x, ostream& out) { out << x.r; }



int main()
{
   GRAPH<Simple1,Simple2> G;

   test_graph(G);

   G.del_node(G.first_node());

   G.print();

   GRAPH<Simple1,Simple2> G1 = G;

   vector vec(1.1,2.2,3.3);
   node_array<Simple3> A(G1);
   edge_array<vector>  B(G1,vec);

   G1.print();

   int i = 0;
   node v;
   forall_nodes(v,G1) A[v] = i++;
   forall_nodes(v,G1) { Print(A[v],cout); cout << endl; }

 
   edge e;
   forall_edges(e,G1)  cout << B[e] << endl;

   return 0;
}

