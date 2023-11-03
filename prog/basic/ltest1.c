#include <LEDA/basic.h>
#include <LEDA/list.h>

typedef int* int_ptr;


int main ()
{
  list<int_ptr>     L;
  list<int_ptr>     L1;

  int i;
  int_ptr p;

  int N = read_int("Number of list entries: ");

  float T = used_time();

  std::cout << "allocating    ";
  std::cout.flush();
  for (i = 0; i < N; i++) L.append((int_ptr)rand_int(1,10000));
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

  std::cout << "iteration     ";
  std::cout.flush();
  forall(p,L) {}
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "clear         ";
  std::cout.flush();
  L.clear();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  return 0;
}
