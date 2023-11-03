#include <LEDA/basic.h>
#include <LEDA/integer.h>


integer fibonacci(integer n, char* int_name)
{
  integer a=1;
  integer b=1;
  integer c;

  integer i=1;

  float T = used_time();

  while(i<n) 
  { //cout << "Fib " <<i<< " = " << b << endl; // |b| is $Fib_i$
    c = a+b;
    a = b;
    b = c;
    i++;
   }

  integer fib = b;

  cout << string("%s: %6.3f sec",int_name,used_time(T)) << endl;


  while(i>0) 
  { //cout << "Fib " <<i<< " = " << b << endl;
    c = b-a;
    b = a;
    a = c;
    i--;
   }
  cout << string("%s: %6.3f sec",int_name,used_time(T)) << endl;
  newline;
  if (c != 0) cerr << string("ERROR in FIB: c = ") << c << endl;

  return fib;
}


main()
{
  integer n = read_int("n= ");

  integer fib  = fibonacci(n,"integer    ");

  cout << "fib(n) = " << fib << endl;

  return 0;
}
