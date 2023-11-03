/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  node_partition.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_NODE_PARTITION_H
#define LEDA_NODE_PARTITION_H


//------------------------------------------------------------------------------
// node partitions 
//------------------------------------------------------------------------------

#include <LEDA/graph.h>

/*{\Manpage {node_partition} {} {Node Partitions}}*/

#include <LEDA/partition.h>

class node_partition : private partition
{
/*{\Mdefinition
An instance $P$ of the data type $node\_partition$ is a partition of the nodes
of a graph $G$.}*/

public:

void init(const graph& G);

/*{\Mcreation P }*/

 node_partition(const graph& G) { init(G); }
/*{\Mcreate creates a \name\ \var\ containing for every node $v$ in $G$ a 
            block $\{v\}$.}*/

~node_partition()               {}   

/*{\Moperations 1.2 4.5}*/

int  same_block(node v, node w)   
{ return partition::same_block(partition_item(v->data[2]),
                               partition_item(w->data[2])); }

/*{\Mopl      returns true if $v$ and $w$ belong to the
              same block of \var, false otherwise.}*/


void union_blocks(node v, node w) 
{ partition::union_blocks(partition_item(v->data[2]), 
                          partition_item(w->data[2])); }
/*{\Mopl      unites the blocks of \var\ containing nodes
	      $v$ and $w$.}*/


void make_rep(node v) 
{ partition::set_inf(partition_item(v->data[2]),v); }

node find(node v) 
{ return node(partition::inf(partition::find(partition_item(v->data[2])))); }
/*{\Mop       returns a canonical representative node of 
	      the block that contains node $v$.}*/

node operator()(node v) { return find(v); }
/*{\Mfunop    returns \var.find($v$). }*/


};

/*{\Mimplementation
A node partition for a graph $G$ is implemented by a combination of a 
partition $P$ and a node array of $partition\_item$ associating with 
each node in $G$ a partition item in $P$. Initialization takes linear time,
union\_blocks takes time $O(1)$ (worst-case), and same\_block and find take 
time $O(\alpha(n))$ (amortized).  The space requirement is $O(n)$, where $n$ 
is the number of nodes of $G$.}*/

#endif
