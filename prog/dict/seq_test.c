#include <LEDA/_sortseq.h>

#include <LEDA/impl/skiplist.h>
#include <LEDA/impl/ab_tree.h>
#include <LEDA/impl/bin_tree.h>


void seq_test(sortseq<int,int>& D, int N, int* A, const char* name)
{ int i;
  float T0 = used_time();
  float T  = T0;

  std::cout << string("%-10s",name);
  std::cout.flush();

  for(i=0; i<N; i++)  D.insert(A[i],0);
  std::cout << string("%10.2f",used_time(T));
  std::cout.flush();

  for(i=0; i<N; i++)  D.lookup(A[i]);
  std::cout << string("%10.2f",used_time(T));
  std::cout.flush();

  for(i=0; i<N; i++)  D.del(A[i]);
  std::cout << string("%10.2f",used_time(T));

  std::cout << string("%10.2f",used_time(T0));
  newline;
}


int main()
{

  sortseq<int,int>           RS_SEQ;

 _sortseq<int,int,skiplist> SKIP_SEQ;
 _sortseq<int,int,ab_tree>  AB_SEQ;
 _sortseq<int,int,bin_tree> BIN_SEQ;


  int     N = read_int("# keys = ");
  int* RAND = new int[N];

  for(int i=0; i<N; i++) RAND[i] = rand_int(0,1000000);

  newline;
  std::cout << "               insert    lookup    delete     total";
  newline;
  newline;

  seq_test(RS_SEQ,N,RAND,"rs_tree");
  seq_test(SKIP_SEQ,N,RAND,"skiplist");
  seq_test(AB_SEQ,N,RAND,"ab_tree");
  seq_test(BIN_SEQ,N,RAND,"bin_tree");

  return 0;
}
