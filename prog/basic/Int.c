#include <LEDA/integer.h>

/*
#if defined(__GNUG__)
#include <Integer.h>
typedef Integer integer;
#endif
*/

int main()
{
   int N  = read_int("iterations = ");
   int la = read_int("size of A  = ");
   int lb = read_int("size of B  = ");
   int i;

   int seed = rand_int(1000,1000000);

   rand_int.set_seed(seed);
   integer A = integer::random(32*la);
   integer B = integer::random(32*lb);
   integer D,P,Q,R,S;

   float T;

/*
   double x =123456789.123456789;

   std::cout << "integer -> double:    " << flush;
   T = used_time();
   for(i=0; i<N; i++) { x = A.todouble(); }
   std::cout << string(" %4.2f",used_time(T)) << std::endl;

   std::cout << "double -> integer:    " << flush;
   T = used_time();
   for(i=0; i<N; i++) { S = x; }
   std::cout << string(" %4.2f",used_time(T)) << std::endl;
   newline;
*/

   std::cout << "Addition (integer):   " << flush;
   T = used_time();
   for(i=0; i<N; i++) { S = A+B; }
   std::cout << string(" %4.2f",used_time(T)) << std::endl;

   std::cout << "Subtraction (integer):" << flush;
   T = used_time();
   for(i=0; i<N; i++) { D = A-B; }
   std::cout << string(" %4.2f",used_time(T)) << std::endl;

   if (S+D-A != A) std::cout << "integer: error in addition/subtraction" << std::endl;


   std::cout << "Multiplication (integer):" << flush;
   T = used_time();
   for(i=0; i<N; i++) { P = A*B; }
   std::cout << string(" %4.2f",used_time(T)) << std::endl;

   std::cout <<"Division:" << std::endl;
   for(i=0; i<N; i++) { Q = A/B; }
   std::cout << string("integer: %4.2f",used_time(T)) << std::endl;


   std::cout <<"Modulo:" << std::endl;
   for(i=0; i<N; i++) { R = A%B; }
   std::cout << string("integer: %4.2f",used_time(T)) << std::endl;

   if (Q*B + R != A)
     std::cout << "error in division/multiplication" << std::endl;
}


