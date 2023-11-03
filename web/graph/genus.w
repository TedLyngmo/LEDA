\documentstyle[11pt,comments]{cweb}

\textwidth=6.5in
\textheight=9in
\oddsidemargin=0in
\topmargin=0in

\input /LEDA/SRC/man/MANUAL.mac

\begin{document}

@ stack:

@c

#ifndef LEDA_STACK_H
#define LEDA_STACK_H

#include <LEDA/basic.h>
#include <LEDA/impl/slist.h>


/*{\manual\\

\section {Stacks (stack) } \label {Stacks}

\definition
\decl stack E

An instance $S$ of the parameterized data type \name\ is 
a sequence of elements of data type $E$, called the element 
type of $S$. Insertions or deletions of elements take place only at one end of 
the sequence, called the top of $S$. The size of $S$ is the length of the 
sequence, a stack of size zero is called the empty stack.
}*/



template<class E>

class _CLASSTYPE stack : private SLIST
{
  void copy_el(GenPtr& x)  const { x=Copy(ACCESS(E,x)); }
  void clear_el(GenPtr& x) const { Clear(ACCESS_REF(E,x)); }

public:


/*{\manual\\

\creation
\create S { }   

creates an instance \var\ of type \name. \var\ is initialized with the empty 
stack.

\operations 2 4

}*/

  stack() {}
  stack(const stack<E>& S) : SLIST(S) {}
 ~stack() { clear(); }
  stack<E>& operator=(const stack<E>& S) 
                     { return (stack<E>&)SLIST::operator=(S); }



  E top()   const { return ACCESS(E,SLIST::head());}

/*{\manual\\
\op E       top   { }  
                       {returns the top element of \var.\\
                        \precond $S$ is not empty.}
}*/




void push(E x)  { SLIST::push(Copy(x)); }

/*{\manual\\
\op void push  {E\ x}  
                       {adds $x$ as new top element to \var.}
}*/



E pop()         { E x=top(); SLIST::pop(); return x; }

/*{\manual\\
\op E    pop   { }  
                       {deletes and returns the top element of \var.\\
                       \precond $S$ is not empty.}
}*/



int  size()  const { return SLIST::length(); }

/*{\manual\\
\op int  size  { }  
                       {returns the size of \var.}
}*/


int  empty() const { return SLIST::empty(); }

/*{\manual\\
\op bool empty { }  
                       {returns true if \var\ is empty, false otherwise.}
}*/


void clear()       { SLIST::clear(); }

/*{\manual\\
\op void clear { }  
                       {makes \var\ the empty stack.}
}*/

};

#endif


@

\end {document}
