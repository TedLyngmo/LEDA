#include <LEDA/vector.h>
#include <LEDA/matrix.h>
#include <LEDA/list.h>



int main()

{
  int d = read_int("dimension = ");

  matrix       A(d,d);
  vector       b(d);
  vector       x(d);

  list<vector> B;  // List of b-vectors
  list<vector> X;  // List of x-vectors

  std::cout << string("give (%d x %d) matrix A:",d,d) << std::endl;

  cin >> A;

  std::cout << "A = \n";
  std::cout << A << "\n";
  newline;

  std::cout << "A.trans() = \n";
  std::cout << A.trans() << "\n";
  newline;


  std::cout << "A.inv() = \n";
  std::cout << A.inv() << "\n";
  newline;

  std::cout << "A*A.inv() = \n";
  std::cout << A*A.inv() << "\n";
  newline;


  std::cout << "We solve the system of linear equations A*x = b.\n";
  std::cout << "Give a list of vectors b (terminated by ctrl-d):\n";

  while (cin >> b) B.append(b);
  newline;

  B.print("b's = ",'\n');
  newline;
  newline;

  forall(b,B)  X.append(A.solve(b));

  X.print("x's = ",'\n');
  newline;
  newline;


  std::cout << "A*x's = \n";
  forall(x,X) std::cout << A*x << "\n";
  newline;

  return 0;
}
