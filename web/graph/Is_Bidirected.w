\documentstyle[11pt,comments]{cweb}

\textwidth=6.5in
\textheight=9in
\oddsidemargin=0in
\topmargin=0in

\begin{document}

\markright{\fbox{\protect\tiny version of \today}}
\pagestyle{myheadings}

@* Testing Bidirectedness.

\noindent Let $G$ be a simple graph. We test whether $G$ is bidirected
and (if required) also establish the correspondence between edges and
their reversals. We first bucket--sort the edges according to their
target node, then according to their source node. The result is 
maintained in a list. Afterwards we sort the edges again, but first 
according to their source node and then according to their target node.
The result is maintained in a second list. Since |bucket sort| is 
stable,
in a bidirected graph the position of an edge in the first list
corresponds to the position of the reversal edge in the second
list. The following procedure 
checks this relation. 
As soon as the tested edges do not correspond, false is returned. 

@c
bool is_bidirected(const graph& G)     
{
 // computes for every edge e = (v,w) in G its reversal reversal[e] = (w,v)
 // in G ( nil if not present). Returns true if every edge has a
 // reversal and false otherwise.

  int n = G.max_i_node();
  int count = 0;

  edge e,r;

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

    if ((target(r) == source(e))&&(source(r) == target(e))) 
         { El1.pop();
           El.pop();
          }
    else
         return false;
  }
  return true;

}

@ The following procedure 
also establishes the correspondence between edges and their reversals.
For every edge |e = (v, w)| in $G$ it computes its reversal
|reversal[e] = (w,v)| if existing in $G$ (|nil| otherwise). If every
edge has a reversal it returns |true| and |false| otherwise.


@c

bool is_bidirected(const graph& G, edge_array<edge>& reversal)     
{
 // computes for every edge e = (v,w) in G its reversal reversal[e] = (w,v)
 // in G ( nil if not present). Returns true if every edge has a
 // reversal and false otherwise.

  int n = G.max_i_node();
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
  return (count == G.number_of_edges());

}
 
@
\end{document}

