#include <LEDA/basic.h>
#include <LEDA/integer.h>


integer fibonacci(integer n, const char* int_name)
{
  integer a=1;
  integer b=1;
  integer c;

  integer i=1;

  float T = used_time();

  while(i<n)
  { //cout << "Fib " <<i<< " = " << b << std::endl; // |b| is $Fib_i$
    c = a+b;
    a = b;
    b = c;
    i++;
   }

  integer fib = b;

  std::cout << string("%s: %6.3f sec",int_name,used_time(T)) << std::endl;


  while(i>0)
  { //cout << "Fib " <<i<< " = " << b << std::endl;
    c = b-a;
    b = a;
    a = c;
    i--;
   }
  std::cout << string("%s: %6.3f sec",int_name,used_time(T)) << std::endl;
  newline;
  if (c != 0) std::cerr << string("ERROR in FIB: c = ") << c << std::endl;

  return fib;
}


int main()
{
  integer n = read_int("n= ");

  integer fib  = fibonacci(n,"integer    ");

  std::cout << "fib(n) = " << fib << std::endl;

  return 0;
}
