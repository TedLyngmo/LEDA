#include <LEDA/d_array.h>


int main()
{
  d_array<double,float> N(0);

  double s;

  while (cin >> s) N[s]++;

  forall_defined (s,N) std::cout << s << " " << N[s] << "\n";

  return 0;
}

