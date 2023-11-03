#include <LEDA/d_array.h>


int main()
{
  d_array<string,int> N(0);

  string s;

  while (cin >> s) N[s]++;

  forall_defined (s,N) std::cout << s << " " << N[s] << "\n";

  return 0;
}

