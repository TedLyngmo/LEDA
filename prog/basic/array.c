#include <LEDA/array.h>
#include <LEDA/array2.h>


int cmp_inv(const string& x, const string& y)
{ return compare(y,x); }

int main()
{
  array<string> A(1,10);

  int i;
  int j;

  A.read("A(1,10) = ");

  A.permute();
  A.print("permuted:\n");
  newline;

  std::cout << "sort subarray [i..j]\n";
  i = read_int("i = ");
  j = read_int("j = ");

  A.sort(i,j);
  A.print("A =\n");
  newline;

  A.sort(cmp_inv);
  A.print("sorted decreasingly\n");
  newline;
  newline;


  array2<double> B(1,10,1,10);

  for(i=1;i<=10;i++)
      for(j=1;j<=10;j++) B(i,j) = 0.12345*i*j;


  for(i=1;i<=10;i++)
  { for(j=1;j<=10;j++) std::cout << string("%6.2f ",B(i,j));
    newline;
   }

 return 0;
 }

