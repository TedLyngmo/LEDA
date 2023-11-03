/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _g_misc.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/graph.h>
#include <LEDA/ugraph.h>




node_array<int>* num_ptr;
  
int epe_source_num(const edge& e) { return (*num_ptr)[source(e)]; }
int epe_target_num(const edge& e) { return (*num_ptr)[target(e)]; }


bool Is_Simple(graph& G)  
{ 
  // return true iff G is simple, i.e, has no parallel edges
 
  list<edge>el= G.all_edges();
  node v;
  
  edge e;
  int n= 0;
  
  node_array<int> num(G);
  forall_nodes(v,G) num[v]= n++;
  
  num_ptr= &num;
  
  el.bucket_sort(0,n-1,&epe_source_num);
  el.bucket_sort(0,n-1,&epe_target_num);
  
  int i= -1;
  int j= -1;
  
  forall(e,el)
  { if(j==num[source(e)]&&i==num[target(e)])
    return false;
    else
    { j= num[source(e)];
      i= num[target(e)];
    }
  }
  return true;
  
}
  
  
void Delete_Loops(graph& G)
{ list<edge> loops;
  edge e;
  forall_edges(e,G)
    if (source(e) == target(e)) loops.append(e);
  forall(e,loops) G.del_edge(e);
 }

void Make_Simple(graph& G)
{ 
  //use bucket sort to find and eliminate parallel edges
  
  list<edge> el = G.all_edges();
  node v;
  edge e;
  int  n = 0;

  node_array<int> num(G);
  forall_nodes(v,G) num[v] = n++;
  
  num_ptr = &num;

  el.bucket_sort(0,n-1,&epe_source_num);
  el.bucket_sort(0,n-1,&epe_target_num);
  
  int i = -1;
  int j = -1; 
  forall(e,el)  
    { if (j==num[source(e)] && i==num[target(e)]) 
        G.del_edge(e);
      else 
        { j=num[source(e)];
          i=num[target(e)];
         }
     }
  
 }




static int edge_ord1(const edge& e) { return index(source(e)); }
static int edge_ord2(const edge& e) { return index(target(e)); }

bool Is_Bidirected(const graph& G, edge_array<edge>& reversal)     
{
 // computes for every edge e = (v,w) in G its reversal reversal[e] = (w,v)
 // in G ( nil if not present). Returns true if every edge has a
 // reversal and false otherwise.

  int n = G.max_node_index();
  int count = 0;

  edge e,r;

  forall_edges(e,G) reversal[e] = 0;

  list<edge> El = G.all_edges();
  El.bucket_sort(0,n,&edge_ord2);
  El.bucket_sort(0,n,&edge_ord1);
  
  list<edge> El1 = G.all_edges();
  El1.bucket_sort(0,n,&edge_ord1);
  El1.bucket_sort(0,n,&edge_ord2);


  // merge El and El1 to find corresponding edges

  while (! El.empty() && ! El1.empty())
  { e = El.head();
    r = El1.head();

    if (target(r) == source(e))
      if (source(r) == target(e))
         { reversal[e] = r;
           El1.pop();
           El.pop();
           count++;
          }
      else
         if (index(source(r)) < index(target(e)))
             El1.pop();
         else
             El.pop();

    else
      if (index(target(r)) < index(source(e)))
          El1.pop();
      else
          El.pop();

    }

  return (count == G.number_of_edges()) ? true : false;

}




#if defined(COMMENT)

static void MB_DFS(graph & G, node v, int &dfs_count, list<edge>& L,
                                      node_array<bool>& reached,
                                      node_array<int>&  dfsnum, 
                                      node_array<int>&  lowpt,
                                      node_array<node>& parent)
{
  node w;
  edge e;

  dfsnum[v] = dfs_count++;
  lowpt[v] = dfsnum[v];
  reached[v] = true;

  if (!G.first_adj_edge(v))
    return;			/* no children */

  node u = target(G.first_adj_edge(v));		/* first child */

  forall_adj_edges(e, v) 
  { w = target(e);
    if (!reached[w])	// e is a tree edge
    { parent[w] = v;

      MB_DFS(G, w, dfs_count, L, reached, dfsnum, lowpt, parent);

      if (lowpt[w] == dfsnum[v]) 	
      { // |v| is an articulation point. We
        // now add an edge. If |w| is the
	// first child and |v| has a parent
	// then we connect |w| and
	// |parent[v]|, if |w| is a first
	// child and |v| has no parent then
	// we do nothing. If |w| is not the
	// first child then we connect |w| to 
	// the first child. The net effect
	// of all of this is to link all
	// children of an articulation point
	// to the first child and the first
	// child to the parent (if it
	// exists)
	if (w == u && parent[v]) 
        { L.append(G.new_edge(w, parent[v]));
	  L.append(G.new_edge(parent[v], w));
	 }

	if (w != u)
	{ L.append(G.new_edge(u, w));
	  L.append(G.new_edge(w, u));
	 }
       }

      lowpt[v] = Min(lowpt[v], lowpt[w]);
    } // tree edge
    else
      lowpt[v] = Min(lowpt[v], dfsnum[w]);	// non tree edge

  }

}


list<edge> Make_Biconnected(graph & G)
{
  node_array <bool>  reached(G, false);
  list<edge> L;

  node u = G.first_node();

  node v;

  forall_nodes(v, G) 
    if (!reached[v]) // explore the connected component with root v 
    { DFS(G, v, reached);
      if (u != v)   // link v's component to the first component
      { L.append(G.new_edge(u, v));
	L.append(G.new_edge(v, u));
       }
     }

// G is now connected. We next make it biconnected.

  reached.init(G,false);

  node_array<int>  dfsnum(G);
  node_array<int>  lowpt(G);
  node_array<node> parent(G,nil);
  int dfs_count = 0;

  MB_DFS(G,G.first_node(), dfs_count, L,reached, dfsnum, lowpt, parent);

  return L;
}				
#endif
