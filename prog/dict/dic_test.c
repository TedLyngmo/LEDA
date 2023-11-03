
#include <LEDA/_dictionary.h>

#include <LEDA/impl/ch_hash.h>
#include <LEDA/impl/dp_hash.h>
#include <LEDA/impl/avl_tree.h>
#include <LEDA/impl/bin_tree.h>
#include <LEDA/impl/rs_tree.h>
#include <LEDA/impl/rb_tree.h>
#include <LEDA/impl/skiplist.h>
#include <LEDA/impl/ab_tree.h>
#include <LEDA/impl/bb_tree.h>



void dic_test(dictionary<int,int>& D, int N, int* A, const char* name)
{
  std::cout << string("%-12s",name);
  std::cout.flush();

  float T;
  float T0 = T = used_time();

  int i;
  for(i=0; i<N; i++)  D.insert(A[i],0);
  std::cout << string("%10.2f",used_time(T));
  std::cout.flush();

  bool ok = true;
  for(i=0; i<N; i++)
  { dic_item it = D.lookup(A[i]);
    if (it == nil || D.key(it) != A[i])  ok = false;
   }

  if (!ok) error_handler(0,"errors in lookups");

  std::cout << string("%10.2f",used_time(T));
  std::cout.flush();

  for(i=0; i<N; i++)  D.del(A[i]);
  std::cout << string("%10.2f",used_time(T));

  std::cout << string("%10.2f",used_time(T0));

  if (!D.empty())
  {  std::cout << " NOT EMPTY ";
     std::cout << "size = " << D.size() << std::endl;
   }


  newline;

  memory_clear();

}


void dic_test(dictionary<float,float>& D, int N, float* A, const char* name)
{
  std::cout << string("%-12s",name);
  std::cout.flush();

  D.clear();

  float T;
  float T0 = T = used_time();


  int i;
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

  memory_clear();
}



int main()
{

  _dictionary<int,int,ch_hash> CHH_DIC;
  _dictionary<int,int,dp_hash> DPH_DIC;

  _dictionary<int,int,avl_tree> AVL_DIC;
  _dictionary<int,int,bin_tree> BIN_DIC;
  _dictionary<int,int,rb_tree>  RB_DIC;
  _dictionary<int,int,rs_tree>  RS_DIC;
  _dictionary<int,int,skiplist> SK_DIC;
  _dictionary<int,int,bb_tree>  BB_DIC;
  _dictionary<int,int,ab_tree>  AB_DIC;

  _dictionary<float,float,avl_tree> AVL_DIC1;
  _dictionary<float,float,bin_tree> BIN_DIC1;
  _dictionary<float,float,rb_tree>  RB_DIC1;
  _dictionary<float,float,rs_tree>  RS_DIC1;
  _dictionary<float,float,skiplist> SK_DIC1;
  _dictionary<float,float,bb_tree>  BB_DIC1;
  _dictionary<float,float,ab_tree>  AB_DIC1;


  int    N     = read_int("# keys = ");
  int*   Int   = new int[N];
  int*   Int1  = new int[N];
  float* Float = new float[N];

  int i;
  for(i=0; i<N; i++) Float[i] = Int[i] = rand_int(0,10000000);

  for(i=0; i<N; i++) Int1[i] = i;


  newline;
  std::cout << "                insert    lookup    delete     total\n";
  newline;



  dic_test(CHH_DIC,N,Int,"ch_hash");
//dic_test(DPH_DIC,N,Int,"dp_hash");
  newline;
  dic_test(SK_DIC,N,Int,"skiplist");
  dic_test(RS_DIC,N,Int,"rs_tree");
  dic_test(BIN_DIC,N,Int,"bin_tree");
  dic_test(RB_DIC,N,Int,"rb_tree");
  dic_test(AVL_DIC,N,Int,"avl_tree");
  dic_test(BB_DIC,N,Int,"bb_tree");
  dic_test(AB_DIC,N,Int,"ab_tree");
  newline;

//dic_test(DPH_DIC,N,Int1,"dp_hash");
  newline;
  dic_test(SK_DIC,N,Int1,"skiplist");
  dic_test(RS_DIC,N,Int1,"rs_tree");
  //dic_test(BIN_DIC,N,Int1,"bin_tree");
  dic_test(RB_DIC,N,Int1,"rb_tree");
  dic_test(AVL_DIC,N,Int1,"avl_tree");
  dic_test(BB_DIC,N,Int1,"bb_tree");
  dic_test(AB_DIC,N,Int1,"ab_tree");
  newline;


  dic_test(RB_DIC1,N,Float,"rb_tree");
  dic_test(AVL_DIC1,N,Float,"avl_tree");
  dic_test(SK_DIC1,N,Float,"skiplist");
  dic_test(RS_DIC1,N,Float,"rs_tree");
  dic_test(BIN_DIC1,N,Float,"bin_tree");
  dic_test(BB_DIC1,N,Float,"bb_tree");
  dic_test(AB_DIC1,N,Float,"ab_tree");
  newline;

  return 0;
}
