#include <LEDA/basic.h>
#include <LEDA/list.h>

int ord(const int& x) { return x%1000; }

int main ()
{
  list<int>     L;
  list<int>     L1;
  list<double>   L2;

  int i;

  int N = read_int("Number of list entries: ");

  float T = used_time();

  std::cout << "allocating    ";
  std::cout.flush();
  for (i = 0; i < N; i++) L.append(rand_int(1,10000));
  std::cout << string("  %5.3f sec",used_time(T));
  newline;


  std::cout << "reversing     ";
  std::cout.flush();
  while (!L.empty()) L1.push(L.pop());
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "assignment    ";
  std::cout.flush();
  L = L1;
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "sorting(int)  ";
  std::cout.flush();
  L.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "sorting again ";
  std::cout.flush();
  L.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;


  std::cout << "iteration     ";
  std::cout.flush();
  forall(i,L) {}
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  forall(i,L) L2.append(double(i));

  std::cout << "sorting(double)";
  std::cout.flush();
  L2.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;


  std::cout << "sorting again ";
  std::cout.flush();
  L2.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;



  std::cout << "test sorting  ";
  std::cout.flush();
  forall(i,L)
  { double f = L2.pop();
    if (i != f)  std::cout << string("%d  != %f\n",i,f);
   }
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "bucket sort   ";
  std::cout.flush();
  L.bucket_sort(0,1000,ord);
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "clear         ";
  std::cout.flush();
  L.clear();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  newline;
  std::cout << "used memory = " << used_memory()/1024.0 << " kb\n";

  return 0;
}
