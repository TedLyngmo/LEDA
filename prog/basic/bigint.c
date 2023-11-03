#include <LEDA/list.h>
#include <LEDA/integer.h>


int ord(const integer& x) { return x.tolong()%1000; }

int main ()
{
  list<integer>     L;
  list<integer>     L1;

  integer i;

  i = i + 0;

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
