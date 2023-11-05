\documentstyle[11pt,comments]{cweb}

\textwidth=6.5in
\textheight=9in
\oddsidemargin=0in
\topmargin=0in

\begin{document}

\markright{\fbox{\protect\tiny version of \today}}
\pagestyle{myheadings}

%\title{An Implementation of Randomized Search Trees}
%\author{Markus Paul}
%\date{\today}
%\maketitle

%\tableofcontents




@* Testing simplicity.

\noindent
Let $G$ be a graph. We test whether $G$ is simple, i.e. contains no
parallel edges. That is easy to do. We bucket-sort the edges twice:
once according to their source node and once according to their target
node. Since bucket sort is stable this will collect parallel edges.

@c

bool is_simple(graph& G)

{
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

@ \end{document}

