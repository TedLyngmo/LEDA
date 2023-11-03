#include <LEDA/integer.h>

/*
#if defined(__GNUG__)
#include <Integer.h>
typedef Integer integer;
#endif
*/

main()
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

   cout << "integer -> double:    " << flush;
   T = used_time();
   for(i=0; i<N; i++) { x = A.todouble(); }
   cout << string(" %4.2f",used_time(T)) << endl;

   cout << "double -> integer:    " << flush;
   T = used_time();
   for(i=0; i<N; i++) { S = x; }
   cout << string(" %4.2f",used_time(T)) << endl;
   newline;
*/

   cout << "Addition (integer):   " << flush;
   T = used_time();
   for(i=0; i<N; i++) { S = A+B; }
   cout << string(" %4.2f",used_time(T)) << endl;

   cout << "Subtraction (integer):" << flush;
   T = used_time();
   for(i=0; i<N; i++) { D = A-B; }
   cout << string(" %4.2f",used_time(T)) << endl;

   if (S+D-A != A) cout << "integer: error in addition/subtraction" << endl;


   cout << "Multiplication (integer):" << flush;
   T = used_time();
   for(i=0; i<N; i++) { P = A*B; }
   cout << string(" %4.2f",used_time(T)) << endl;

   cout <<"Division:" << endl;
   for(i=0; i<N; i++) { Q = A/B; }
   cout << string("integer: %4.2f",used_time(T)) << endl;


   cout <<"Modulo:" << endl;
   for(i=0; i<N; i++) { R = A%B; }
   cout << string("integer: %4.2f",used_time(T)) << endl;

   if (Q*B + R != A) 
     cout << "error in division/multiplication" << endl;
}


