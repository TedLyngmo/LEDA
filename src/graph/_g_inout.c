/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _g_inout.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#include <LEDA/graph.h>
#include <fstream>


//--------------------------------------------------------------------------
// graph i/o
//--------------------------------------------------------------------------


/*
static void put_int(filebuf& fb, int n)
{ char* A = (char*)&n;
  char* E = A+sizeof(int);
  while (A<E) fb.sputc(*(A++));
}

static int get_int(std::istream& from)
{ int n;
  char* A = (char*)&n;
  char* E = A+sizeof(int);
  while (A<E) from.get(*(A++));
  return n;
}
*/

void graph::write(string file_name) const
{ std::ofstream out(file_name);
  if (out.fail()) error_handler(1,"write: cannot open file");
  write(out);
}


void graph::write(std::ostream& out) const
{
  int* A = new int[max_n_index+2];

  // nodes get numbers from 1 to |V| (trouble with 0)

  int count = 1;

  out << "LEDA.GRAPH\n";
  out << node_type() << "\n";
  out << edge_type() << "\n";

  out << V.length() << "\n";

  node v;
  forall_nodes(v,*this)
  { write_node_entry(out,v->data[0]);
    out << std::endl;
    A[v->name] = count++;
   }

  out << E.length() << "\n";
  edge e;
  forall_edges(e,*this)
  { out << A[e->s->name] << " " << A[e->t->name] << " ";
    write_edge_entry(out,e->data[0]);
    out << std::endl;
   }
delete A;
}

int graph::read(string file_name)
{ std::ifstream in(file_name);
  if (in.fail())  return 1;
  return read(in);
}

int graph::read(std::istream& in)
{ int result = 0;

  clear();

  edge e;
  int n,i,v,w;

  string d_type,n_type,e_type;

  string this_n_type = node_type();
  string this_e_type = edge_type();

  while (in && d_type=="") in >> d_type;

  in >> n_type >> e_type >> n;

  if (d_type != "LEDA.GRAPH") return 3;

  read_line(in);

  node* A = new node[n+1];

  // since Type_Name currently does not work for user defined types
  // (produces the string "unknown") we allow "unknown" to match
  // any node_type

  if (this_n_type != "unknown" && n_type != this_n_type)
  { if (this_n_type != "void") result = 2;   // incompatible node types
    for (i=1; i<=n; i++)
    { A[i] = new_node();
      read_line(in);
     }
   }
  else
    for (i=1; i<=n; i++)
    { A[i] = new_node(0);
      read_node_entry(in,A[i]->data[0]);
     }

  in >> n;       // number of edges

  if (this_e_type != "unknown" && e_type != this_e_type)   // see above remark
  { if (this_e_type != "void") result = 2;   // incompatible edge types
    while (n--) { in >> v >> w;
                  e = new_edge(A[v],A[w]);
                  read_line(in);
                 }
   }
  else
   while (n--) { in >> v >> w;
                 e = new_edge(A[v],A[w],0);
                 read_edge_entry(in,e->data[0]);
                }

  delete A;
  return result;
}


void graph::print_node(node v,std::ostream& o) const
{ if (super() != 0)
     super()->print_node(node(graph::inf(v)),o);
  else
     { o << "[" << index(v) <<"]" ;
       print_node_entry(o,v->data[0]);
      }
}

void graph::print_edge(edge e,std::ostream& o) const
{ if (super() != 0)
     super()->print_edge(edge(graph::inf(e)),o);
  else
     { o <<   "[" << e->s->name << "]";
       o <<  ((undirected) ? "==" : "--");
       print_edge_entry(o,e->data[0]);
       o <<  ((undirected) ? "==" : "-->");
       o << "[" << e->t->name << "]";
      }
}


void graph::print(string s, std::ostream& out) const
{ node v;
  edge e;
  out << s << std::endl;
  forall_nodes(v,*this)
  { print_node(v,out);
    out << " : ";
    forall_adj_edges(e,v) print_edge(e,out);
    out << std::endl;
   }
  out << std::endl;
}

