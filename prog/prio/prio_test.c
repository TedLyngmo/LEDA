#include <LEDA/_prio.h>

#include <LEDA/impl/m_heap.h>
#include <LEDA/impl/k_heap.h>
#include <LEDA/impl/p_heap.h>
#include <LEDA/impl/list_pq.h>

typedef float FLOAT;


void prio_test(priority_queue<int,int>& D, int N, int* A, char* name)
{
  pq_item* I = new pq_item[N];

  std::cout << string("%-12s",name);
  std::cout.flush();

  float T;
  float T0 = T = used_time();

  int i;
  for(i=0; i<N; i++)  I[i] = D.insert(i,A[i]);
  std::cout << string("%10.2f",used_time(T));
  std::cout.flush();

  for(i=0; i<N; i++)  D.decrease_inf(I[i],A[i]/2);
  std::cout << string("%14.2f",used_time(T));
  std::cout.flush();

  int old_min = -MAXINT;

  for(i=0; i<N; i++)
  { if (D.inf(D.find_min()) < old_min) std::cout << "error in del_min\n";
    old_min = D.inf(D.find_min());
    D.del_min();
   }

  std::cout << string("%10.2f",used_time(T));

  std::cout << string("%10.2f",used_time(T0));

  if (!D.empty()) std::cout << " NOT EMPTY !!\n";

  newline;

  delete I;
}


void prio_test(priority_queue<FLOAT,FLOAT>& D, int N, FLOAT* A, char* name)
{
  pq_item* I = new pq_item[N];

  std::cout << string("%-12s",name);
  std::cout.flush();

  float T;
  float T0 = T = used_time();


  int i;
  for(i=0; i<N; i++)  I[i] = D.insert(i,A[i]);
  std::cout << string("%10.2f",used_time(T));
  std::cout.flush();

  for(i=0; i<N; i++)  D.decrease_inf(I[i],A[i]/2);
  std::cout << string("%14.2f",used_time(T));
  std::cout.flush();

  FLOAT old_min = -MAXINT;

  for(i=0; i<N; i++)
  { if (D.inf(D.find_min()) < old_min) std::cout << "error in del_min\n";
    old_min = D.inf(D.find_min());
    D.del_min();
   }

  std::cout << string("%10.2f",used_time(T));

  std::cout << string("%10.2f",used_time(T0));

  if (!D.empty()) std::cout << " NOT EMPTY !!\n";

  newline;

  delete I;
}



int main()
{ priority_queue<int,int>            Q;
  priority_queue<FLOAT,FLOAT>        Qf;

  int    N     = read_int("# keys = ");
  int*   Int   = new int[N];
  FLOAT* Float = new FLOAT[N];

  random_source ran(0,1000000);

  for(int i=0; i<N; i++) Float[i] = Int[i] = ran();

  _priority_queue<int,int,m_heap>    m_q(N);
  _priority_queue<int,int,k_heap>    k_q(N);
  _priority_queue<int,int,p_heap>    p_q;
  _priority_queue<int,int,list_pq>   l_q;

  _priority_queue<FLOAT,FLOAT,m_heap>    m_qf(N);
  _priority_queue<FLOAT,FLOAT,k_heap>    k_qf(N);
  _priority_queue<FLOAT,FLOAT,p_heap>    p_qf;
  _priority_queue<FLOAT,FLOAT,list_pq>   l_qf;


  newline;
  std::cout << "                insert   decrease_inf  del_min    total\n";
  newline;

  prio_test(Q,N,Int,"prio");
  //prio_test(m_q,N,Int,"m_heap");
  prio_test(k_q,N,Int,"k_heap");
  prio_test(p_q,N,Int,"p_heap");
  //prio_test(l_q,N,Int,"list_pq");
  newline;


  prio_test(Qf,N,Float,"prio");
  //prio_test(m_qf,N,Float,"m_heap");
  prio_test(k_qf,N,Float,"k_heap");
  prio_test(p_qf,N,Float,"p_heap");
  //prio_test(l_qf,N,Float,"list_pq");
  newline;

  return 0;
}
