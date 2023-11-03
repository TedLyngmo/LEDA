#include <LEDA/d_array.h>



void print(d_array<string,string> A)  // by value
{ string s;
  forall_defined(s,A) std::cout << s << "  " << A[s] << std::endl;
  newline;
 }

int main()
{
  d_array<string,string> trans, D;

  trans["hello"]  = "hallo";
  trans["world"]  = "Welt";
  trans["book"]   = "Buch";
  trans["coffee"] = "Kaffee";
  trans["hello"]  = "xyz";


  D = trans;

  trans.undefine("hello");
  trans.undefine("world");

  print(D);

  print(trans);

  return 0;
}
