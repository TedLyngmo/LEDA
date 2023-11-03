/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _g_map.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#include <LEDA/graph.h>

int graph_map::next_power(int s) const
{ if (s==0) return 0;
  int p = 1;
  while (p < s) p <<= 1;
  return p;
}

void graph_map::init_table(GenPtr* start, GenPtr* stop)
{  for(GenPtr* q=start; q < stop; q++) init_entry(*q); }

void graph_map::clear_table()
{ GenPtr* stop = table+table_size;
  for(GenPtr* q=table; q < stop; q++) clear_entry(*q); 
 }

void graph_map::resize_table(int sz)
{ 
  GenPtr* old_table = table;
  GenPtr* old_stop  = table + table_size;
  
  table_size = sz;
  table = new GenPtr[sz];

  GenPtr* p = old_table; 
  GenPtr* q = table;
  while (p < old_stop) *q++ = *p++;

  init_table(q,table+sz);

  if (old_table != old_stop) delete[] old_table;
}


void graph_map::init(const graph* G, int sz) 
{ clear_table();
  if (table_size > 0) delete[] table;
  table = 0;
  g = G;
  table_size = next_power(sz);
  if (table_size > 0) table = new GenPtr[table_size];
}

graph_map::graph_map(const graph* G, int sz) 
{ g = G;
  table = 0;
  table_size = next_power(sz);
  if (table_size > 0) table = new GenPtr[table_size];
}


graph_map::graph_map(const graph_map& M)
{ g = M.g;
  table = 0;
  table_size = M.table_size;
  if (table_size > 0)
  { table = new GenPtr[table_size];
    GenPtr* p = table;
    GenPtr* stop = M.table+M.table_size;
    for(GenPtr* q=M.table; q < stop; q++) 
    { *p = *q;
      M.copy_entry(*p); 
      p++;
     }
   }
}


graph_map& graph_map::operator=(const graph_map& M)
{ if (&M == this) return *this;
  clear_table();
  if (table_size > 0) delete[] table;
  table = 0;
  g = M.g;
  table_size = M.table_size;
  if (table_size > 0)
  { table = new GenPtr[table_size];
    GenPtr* p = table;
    GenPtr* stop = M.table+M.table_size;
    for(GenPtr* q=M.table; q < stop; q++) 
    { *p = *q;
      copy_entry(*p); 
      p++;
     }
   }
  return *this;
}

