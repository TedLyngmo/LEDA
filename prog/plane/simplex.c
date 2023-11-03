
#include <LEDA/simplex.h>



int main()
{
  std::cout << "Maximize               \n";
  std::cout << "                  |x1| \n";
  std::cout << "                  |x2| \n";
  std::cout << "                  |. | \n";
  std::cout << "(c1 c2 ...  cn) x |. | \n";
  std::cout << "                  |. | \n";
  std::cout << "                  |. | \n";
  std::cout << "                  |xn| \n";
  newline;

  std::cout << "constraints:\n";
  newline;

  std::cout << "     <-------  n  -------->                    \n";
  std::cout << "                                               \n";
  std::cout << "    |.......................|   |x1|       |b1|\n";
  std::cout << "    |.......................|   |x2|       |b2|\n";
  std::cout << "  i |.......         .......|   |. |  <=   |. |\n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "                                               \n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "  j |.......    A    .......| x |. |  >=   |. |\n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "                                               \n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "    |.......         .......|   |. |       |. |\n";
  std::cout << "  k |.......         .......|   |. |  ==   |. |\n";
  std::cout << "    |.......................|   |. |       |. |\n";
  std::cout << "    |.......................|   |xm|       |bm|\n";
  newline;
  newline;

  int n = read_int("number of variables n = ");
  newline;

  int i = read_int("number of <= relations i = ");
  int j = read_int("number of >= relations j = ");
  int k = read_int("number of == equations k = ");

  int m = i+j+k;

  matrix A(m,n);
  vector b(m);
  vector c(n);

  std::cout << string("(%d x %d) - Matrix A:\n",m,n);

  for (int z = 0 ; z<m ; z++ )
  { if (z < i)
       std::cout << string("<= row %d : ",z);
    else
       if (z < i+j)
          std::cout << string(">= row %d : ",z);
       else
          std::cout << string("== row %d : ",z);

   	for (int s = 0 ; s<n ; s++ ) cin >> A(z,s);
   }
  newline;


  std::cout << string("%d-vector b: ",m);
  cin >> b;
  newline;

  std::cout << string("%d-vector c: ",n);
  cin >> c;
  newline;


  list<matrix> S = SIMPLEX(A,i,j,k,b,c);


  if (!S.empty())
    S.print ();
  else
    std::cout << "Keine Loesung !";

}
