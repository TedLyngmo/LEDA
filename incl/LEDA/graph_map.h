/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  graph_map.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_GRAPH_MAP_H
#define LEDA_GRAPH_MAP_H

#include <LEDA/basic.h>


//------------------------------------------------------------------------------
// graph_map:  base type for node/edge arrays and node/edge maps
//------------------------------------------------------------------------------


class graph_map {

friend class graph;

virtual void init_entry(GenPtr&)  const {}
virtual void clear_entry(GenPtr&) const {}
virtual void copy_entry(GenPtr&)  const {}

GenPtr* table;
int table_size;
const graph*  g;

int  next_power(int) const;
void resize_table(int sz);


protected:

void init_table(GenPtr*, GenPtr*);
void init_table() { init_table(table,table+table_size); }
void clear_table();

public:
virtual int  cmp_entry(GenPtr,GenPtr) const  { return 0; }

int     size() const { return table_size; }
GenPtr& access(int i)     { return table[i]; }
GenPtr  read(int i) const { return table[i]; }


GenPtr& array_access(node v)
{
#if ! defined(LEDA_CHECKING_OFF)
if (index(v) >= table_size || graph_of(v) != g)
   error_handler(999,"node_array: illegal node");
#endif
  return table[index(v)];
 }

GenPtr& array_access(edge e)
{
#if ! defined(LEDA_CHECKING_OFF)
if (index(e) >= table_size || graph_of(e) != g)
   error_handler(999,"edge_array: illegal edge");
#endif
  return table[index(e)];
 }

GenPtr array_read(node v) const
{
#if ! defined(LEDA_CHECKING_OFF)
if (index(v) >= table_size || graph_of(v) != g)
   error_handler(999,"node_array: illegal node");
#endif
  return table[index(v)];
 }

GenPtr array_read(edge e) const
{
#if ! defined(LEDA_CHECKING_OFF)
if (index(e) >= table_size || graph_of(e) != g)
   error_handler(999,"edge_array: illegal edge");
#endif
  return table[index(e)];
 }


GenPtr& map_access(node v)
{
#if ! defined(LEDA_CHECKING_OFF)
if (graph_of(v) != g) error_handler(999,"node_map: illegal node");
#endif
  int i = index(v);
  if (i >= table_size) resize_table(next_power(i+1));
  return table[i];
}

GenPtr& map_access(edge e)
{
#if ! defined(LEDA_CHECKING_OFF)
if (graph_of(e) != g) error_handler(999,"edge_map: illegal edge");
#endif
  int i = index(e);
  if (i >= table_size) resize_table(next_power(i+1));
  return table[i];
}


GenPtr map_read(node v) const
{
#if ! defined(LEDA_CHECKING_OFF)
if (graph_of(v) != g) error_handler(999,"node_map: illegal node");
#endif
  int i = index(v);
  if (i < table_size)
     return table[i];
  else
    { GenPtr x;
      init_entry(x);
      return x;
     }
}

GenPtr map_read(edge e) const
{
#if ! defined(LEDA_CHECKING_OFF)
if (graph_of(e) != g) error_handler(999,"edge_map: illegal edge");
#endif
  int i = index(e);
  if (i < table_size)
     return table[i];
  else
    { GenPtr x;
      init_entry(x);
      return x;
     }
}


void init(const graph* G, int sz);



graph_map(const graph* G, int sz);
graph_map(const graph_map&);

graph_map& operator=(const graph_map&);

virtual ~graph_map() { delete[] table; }


};



#endif
