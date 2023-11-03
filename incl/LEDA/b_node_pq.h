/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  b_node_pq.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_B_NODE_PQ_H
#define LEDA_B_NODE_PQ_H


//------------------------------------------------------------------------------
// bounded node priority queue
//
// S. Naeher (1993)
//
// - insert into empty queue, bug fixed   (S.N. May 1995)
//------------------------------------------------------------------------------

/*{\Manpage {b_node_pq} {N} {Bounded Node Priority Queues} }*/

template <int N> class b_node_pq 
{
/*{\Mdefinition
An instance of the data type \name\ is a priority queue of nodes with
integer priorities with the restriction that the size of the minimal interval 
containing all priorities in the queue is bounded by $N$, the minimum priority 
is never decreasing, and every node is contained in at most one queue. 
When applied to the empty queue the del\_min - operation returns a special
default minimum node defined in the constructor of the queue. 
}*/

  node_list bucket[N+1];

  node_struct stop_item;
  node_list*  stop;
  node_list*  minptr;  // pointer to bucket of nodes with minimal dist value
  int         val0;    // current dist value of nodes in bucket[0]

  node nil_node;       // node to be returned by del_min if queue is empty


  // no copy operation

  b_node_pq(const b_node_pq&) {}

public:

/*{\Mcreation PQ }*/

b_node_pq()
{ stop = bucket+N; 
  minptr = stop; 
  val0 = 0; 
  stop->append(&stop_item);
  nil_node = nil;
 }
/*{\Mcreate   introduces a variable \var\ of type \name\ and initializes
              it with the empty queue with default minimum node $nil$. }*/

b_node_pq(node v)
{ stop = bucket+N; 
  minptr = stop; 
  val0 = 0; 
  stop->append(&stop_item);
  nil_node = v;
 }
/*{\Mcreate   introduces a variable \var\ of type \name\ and initializes
              it with the empty queue with default minimum node $v$. }*/



/*{\Moperations 1.3 4.5 }*/

node del_min()
{ while (minptr->empty()) minptr++;
  if (minptr == stop) 
  { val0 += N;
    minptr = bucket;
    while (minptr->empty()) minptr++;
   }
  return (minptr==stop) ? nil_node : minptr->pop();
}
/*{\Mop     removes the node with minimal priority from \var\ and returns
            it (the default minimum node if \var\ is empty). }*/ 


void insert(node w, int p) 
{ if (minptr == stop) 
    { // insert in empty queue (initialize val0 and min_ptr)
      val0 = p - p % N;
      p -= val0;
      minptr = bucket + p;
     }
  else
     if ((p-=val0) >= N) p -= N;
  bucket[p].push(w); 
}
/*{\Mop    adds node $w$ with priority $p$ to \var. }*/

void del(node w) { node_list::del_node(w); }
/*{\Mop    deletes node $w$ from \var. }*/


/*{\Mimplementation
Bounded node priority queues are implemented by cyclic arrays of
doubly linked node lists. }*/

};


/*{\Mexample
Using a $b\_node\_pq$ in Dijktra's shortest paths algorithm.
\begingroup
\ttbig
{\obeyspaces\gdef {\ }}
\ttverbatim

int dijkstra(const GRAPH<int,int>& g, node s, node t) 
{ node_array<int> dist(g,MAXINT);
  b_node_pq<100> PQ(t);  // on empty queue del_min returns t 
  dist[s] = 0;

  for (node v = s;  v != t; v = PQ.del_min() )
  { int dv = dist[v];
    edge e;
    forall_adj_edges(e,v) 
    { node w = g.opposite(v,e);
      int d = dv + g.inf(e);
      if (d < dist[w])
      { if (dist[w] != MAXINT) PQ.del(w);
        dist[w] = d;
        PQ.insert(w,d);
       }
     }
   }
  return dist[t];
}

\endgroup
}*/




#endif
