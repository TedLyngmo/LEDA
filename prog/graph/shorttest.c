#include <LEDA/stream.h>
#include <LEDA/ugraph.h>
#include <LEDA/node_pq.h>
#include <LEDA/b_node_pq.h>

#include <LEDA/node_slist.h>

int  dijkstra1(UGRAPH<int,int>& g, node source, node target)
{
  // use a linear list priority queue  (node_list)
  // and node_array<int> for dist values

  node_array<int> dist(g,MAXINT); // initialize labels
  node_list      labeled;        // candidate nodes
  node v;

  dist[source] = 0;

  labeled.push(source);

  while ( ! labeled.empty() )
  {
    // find node v with minimal dist[v] by linear search

    v = labeled.first();
    int dv = dist[v];
    node u = labeled.succ(v);
    while (u)
    { if (dist[u] < dv)  { v = u; dv = dist[v]; }
       u = labeled.succ(u);
     }


    if (v == target) break;

    edge e;

    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int  d = dist[v] + g[e];
      if (dist[w] > d)
      { if (dist[w] == MAXINT) labeled.append(w); // first time touched
	dist[w] = d;
       }
      }

    labeled.del(v);
  }

  return dist[v];
}

int  dijkstra2(UGRAPH<int,int>& g, node source, node target)
{
  // use a node priority queue (node_pq)
  // and node_array<int> for dist values

  node_array<int> dist(g,MAXINT); // initialize labels
  dist[source] = 0;

  node_pq<int> PQ(g);             // candidate nodes
  PQ.insert(source,0);

  while (! PQ.empty())
  {
    node v = PQ.del_min();
    int dv = dist[v];

    if (v == target) break;

    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int d = dv + g[e];
      if (d < dist[w])
      { if (dist[w] == MAXINT)
           PQ.insert(w,d);
        else
           PQ.decrease_inf(w,d);
        dist[w] = d;
       }
    }

  }

  return dist[target];
}




int dijkstra3(UGRAPH<int,int>& g, node s, node t)
{
  // use a bounded node priority queue (b_node_pq)
  // and a node_array<int> for dist values

  node_array<int> dist(g,MAXINT);

  b_node_pq<101> PQ(t);  // on empty queue del_min returns t

  node v;

  dist[s] = 0;
  PQ.insert(s,0);

  while ( (v = PQ.del_min()) != t )
  { int dv = dist[v];
    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int d = dv + g[e];
      if (d < dist[w])
      { if (dist[w] != MAXINT) PQ.del(w);
	dist[w] = d;
        PQ.insert(w,d);
       }
     }
   }

  return dist[t];
}


int dijkstra4(UGRAPH<int,int>& g, node s, node t)
{
  // use a bounded node priority queue (b_node_pq)
  // and node_array<int> for dist values

  node_array<int> dist(g,MAXINT);

  b_node_pq<101> PQ(t);  // on empty queue del_min returns t

  node v;

  dist[s] = 0;
  PQ.insert(s,0);

  while ( (v = PQ.del_min()) != t )
  { int dv = dist[v];
    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int d = dv + g[e];
      if (d < dist[w])
      { if (dist[w] != MAXINT) PQ.del(w);
	dist[w] = d;
        PQ.insert(w,d);
       }
     }
   }

  return dist[t];
}

int dijkstra5(UGRAPH<int,int>& g, node s, node t)
{
  // use a bounded node priority queue (b_node_pq)
  // and the node information of g for  dist values

  b_node_pq<101> PQ(t);  // on empty queue del_min returns t

  node v;
  forall_nodes(v,g) g[v] = MAXINT;

  g[s] = 0;
  PQ.insert(s,0);

  while ( (v = PQ.del_min()) != t )
  { int dv = g[v];
    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int d = dv + g[e];
      if (d < g[w])
      { if (g[w] != MAXINT) PQ.del(w);
	g[w] = d;
        PQ.insert(w,d);
       }
     }
   }

  return g[t];
}


int dijkstra6(UGRAPH<int,int>& g, node s, node t)
{
  // use a bounded node priority queue (b_node_pq)
  // and the node information of g for  dist values

  b_node_pq<101> PQ(t);  // on empty queue del_min returns t

  node v;
  forall_nodes(v,g) g[v] = MAXINT;

  g[s] = 0;
  PQ.insert(s,0);

  while ( (v = PQ.del_min()) != t )
  { int dv = g[v];
    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int d = dv + g[e];
      if (d < g[w])
      { if (g[w] != MAXINT) PQ.del(w);
	g[w] = d;
        PQ.insert(w,d);
       }
     }
   }

  return g[t];
}



int  moore1(UGRAPH<int,int>& g, node source, node target)
{
  // use a queue of candidate nodes (node_list)
  // and node_array<int> for dist values

  node_array<int> dist(g,MAXINT);   // initialize labels
  dist[source] = 0;

  node_list labeled;               // queue of candidate nodes
  labeled.append(source);

  while (! labeled.empty())
  { node v = labeled.pop();
    int dv = dist[v];

    if (dv >= dist[target]) continue;

    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int d = dv + g[e];
      if (d < dist[w])
      { if (!labeled(w)) labeled.append(w);
	dist[w] = d;
       }
     }

   }

  return dist[target];

}


int  moore2(UGRAPH<int,int>& g, node source, node target)
{
  // use a double ended queue of candidate nodes (node_list)
  // and a node_array<int> for dist values

  node_array<int> dist(g,MAXINT); // initialize labels
  dist[source] = 0;

  node_list labeled;             // deque of candidate nodes
  labeled.append(source);

  while (! labeled.empty())
  {
    node v = labeled.pop();
    int dv = dist[v];

    if (dv > dist[target]) continue;

    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int  d = dv + g[e];
      if (d < dist[w])
      { if ( ! labeled(w) )
        { if (dist[w] == MAXINT)
	       labeled.append(w);
	    else
	       labeled.push(w);
	   }
	  dist[w] = d;
       }
     }

  }

  return dist[target];
}


int  moore3(UGRAPH<int,int>& g, node source, node target)
{
  // use a double ended queue of candidate nodes (node_list)
  // and the node information of g for  dist values


  node v;
  forall_nodes(v,g) g[v] = MAXINT;  // initialize labels

  g[source] = 0;

  node_list labeled;             // deque of candidate nodes
  labeled.append(source);

  while (! labeled.empty())
  {
    node v = labeled.pop();
    int dv = g[v];

    if (dv > g[target]) continue;

    edge e;

    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int  d = dv + g[e];
      if (d < g[w])
      { if ( ! labeled(w) )
        { if (g[w] == MAXINT)
	       labeled.append(w);
	    else
	       labeled.push(w);
	   }
	  g[w] = d;
       }
     }

  }

  return g[target];
}


int  moore4(UGRAPH<int,int>& g, node source, node target)
{
  // use a double ended queue of candidate nodes (node_slist)
  // and the node information of g for  dist values


  node v;
  forall_nodes(v,g) g[v] = MAXINT;  // initialize labels

  g[source] = 0;

  node_slist labeled(g);            // deque of candidate nodes
  labeled.append(source);

  while (! labeled.empty())
  {
    node v = labeled.pop();
    int dv = g[v];

    if (dv > g[target]) continue;

    edge e;
    forall_adj_edges(e,v)
    { node w = g.opposite(v,e);
      int  d = dv + g[e];
      if (d < g[w])
      { if (g[w] == MAXINT) labeled.append(w);
	else if (!labeled(w)) labeled.push(w);
	g[w] = d;
       }
     }

  }

  return g[target];
}



GRAPH<int,int> make_bidirected(GRAPH<int,int>& G)
{
  edge last = G.last_edge();

  for(edge e = G.first_edge(); e != last; e = G.succ_edge(e))
  G.new_edge(target(e),source(e),G[e]);
  return G;
}


int main (int argc, char** argv)
{

  GRAPH<int,int>   G0;

  int sourcename;
  int targetname;
  int len;

  string filename = "grid100";

  if (argc > 1) filename = argv[1];

  // read names of source and target from file <filename>

  file_istream  infile (filename);

  if ( ! (infile >> sourcename >> targetname) )
  { std::cerr << "Cannot read file " << filename << std::endl;
    exit(1);
   }

  std::cout << "Source node: " << sourcename << std::endl;
  std::cout << "Target node: " << targetname << std::endl;
  newline;

  // read graph from file <filename>.graph

  float t0 = used_time();

  if (G0.read(filename + ".graph") != 0)
  { std::cerr << "Cannot read graph from file " << filename << ".graph" << std::endl;
    exit(1);
   }

  std::cout << string("Time for reading:  %5.2f",used_time(t0)) << std::endl;
  newline;


  UGRAPH<int,int>  g = G0;
  GRAPH<int,int> G = make_bidirected(G0);

  std::cout << string("Time for making undirected:  %5.2f",used_time(t0)) << std::endl;
  newline;

  // search for source and target nodes

  node source = nil;
  node target = nil;

  node v;
  forall_nodes(v,g)
  { if (g[v] == sourcename) source = v;
    if (g[v] == targetname) target = v;
   }

  node sourceG = nil;
  node targetG = nil;

  forall_nodes(v,G)
  { if (G[v] == sourcename) sourceG = v;
    if (G[v] == targetname) targetG = v;
   }



  t0 = used_time();

  if (g.number_of_edges() < 20000)
  { len = dijkstra1(g, source, target);
    std::cout <<string("Time for dijkstra1: %5.3f pathlength: %d",used_time(t0),len);
    newline;
   }

  len = dijkstra2(g, source, target);
  std::cout <<string("Time for dijkstra2: %5.3f pathlength: %d",used_time(t0),len);
  newline;

  len = dijkstra3(g, source, target);
  std::cout <<string("Time for dijkstra3: %5.3f pathlength: %d",used_time(t0),len);
  newline;

  len = dijkstra4(g, source, target);
  std::cout <<string("Time for dijkstra4: %5.3f pathlength: %d",used_time(t0),len);
  newline;

  len = dijkstra5(g, source, target);
  std::cout <<string("Time for dijkstra5: %5.3f pathlength: %d",used_time(t0),len);
  newline;

  len = dijkstra6(g, source, target);
  std::cout <<string("Time for dijkstra6: %5.3f pathlength: %d",used_time(t0),len);
  newline;


/*
  len = moore1(g, source, target);
  std::cout <<string("Time for moore1:    %5.3f pathlength: %d",used_time(t0),len);
  newline;
*/

  len = moore2(g, source, target);
  std::cout <<string("Time for moore2:    %5.3f pathlength: %d",used_time(t0),len);
  newline;

  len = moore3(g, source, target);
  std::cout <<string("Time for moore3:    %5.3f pathlength: %d",used_time(t0),len);
  newline;

  len = moore4(g, source, target);
  std::cout <<string("Time for moore4:    %5.3f pathlength: %d",used_time(t0),len);
  newline;

  return 0;
}
