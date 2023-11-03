#include <LEDA/matrix.h>

int main()

{
  int d = read_int("dimension = ");

  matrix       A(d,d);
  matrix       I(d,d);
  double       det;


  for(int i=0;i<d;i++)
    for(int j=0;j<d;j++)
      A(i,j) = double(rand_int(-1000,1000))/100;

  float T = used_time();


  std::cout << "A.inv():  ";
  std::cout.flush();
  I =  A.inv();
  std::cout << string("%5.2f sec\n",used_time(T));

  std::cout << "A.det():  ";
  std::cout.flush();
  det = A.det();
  std::cout << string("%5.2f sec \n",used_time(T));
  newline;

/*
  std::cout << "A*A.inv() = \n" << A*I << "\n";
  newline;
*/

  while (Yes("A.solve(v) ? "))
  {
    vector v(d), x(d), y(d);

    for(int i=0;i<d;i++) v[i] = double(rand_int(-1000,1000))/100;

    used_time(T);
    x = A.solve(v);
    std::cout << string("time for solve:  %5.2f \n",used_time(T));
    newline;

    std::cout << "v   = " << v   << "\n";
    std::cout << "A*x = " << A*x << "\n";
    newline;
  }

  return 0;

}
