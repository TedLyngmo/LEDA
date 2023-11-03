/* dieses Programm belegt einen Fehler in der integer division */

#include <iostream>
#include <cmath>
#include <LEDA/bigfloat.h>

void binout (std::ostream &,integer);

int main (void)
{
  integer a=integer(26118786) << 32;
  a=a + 3762119088;
  integer b=17;

  double a_dbl=26118786;
  a_dbl=a_dbl * pow(2,32);
  a_dbl=a_dbl + 3762119088;
  double b_dbl=17;

  char *out;
//  ieee_flags("set","direction","tozero",&out);

  std::cout << "a:           " << a << "\n";
  std::cout << "b:           " << b << "\n";
  /* wir haben hier die bigfloat Konvertierung vorgenommen, um vergleichbare
     Ausgaben zu erreichen */
  std::cout << "a_dbl:       " << bigfloat(a_dbl).tointeger() << "\n";
  std::cout << "b_dbl:       " << b_dbl << "\n";
  std::cout << "a/b:         " << (a/b) << "\n";
  std::cout << "a_dbl/b_dbl: " << bigfloat(a_dbl/b_dbl).tointeger() << "\n";

  std::cout << "das ganze in Binaerstellen:\n";
  std::cout << "a/b=         ";
  binout(std::cout,bigfloat(a/b).get_significant());
  std::cout << "\n";
  std::cout << "a_dbl/b_dbl= ";
  binout(std::cout,bigfloat(a_dbl/b_dbl).get_significant());
  std::cout << "\n";


}

