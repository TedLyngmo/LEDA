/* dieses Programm belegt einen Fehler in der integer division */

#include <iostream.h>
#include <math.h>
#include <LEDA/bigfloat.h>

void binout (ostream &,integer);

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

  cout << "a:           " << a << "\n";
  cout << "b:           " << b << "\n";
  /* wir haben hier die bigfloat Konvertierung vorgenommen, um vergleichbare
     Ausgaben zu erreichen */
  cout << "a_dbl:       " << bigfloat(a_dbl).tointeger() << "\n";
  cout << "b_dbl:       " << b_dbl << "\n";
  cout << "a/b:         " << (a/b) << "\n";
  cout << "a_dbl/b_dbl: " << bigfloat(a_dbl/b_dbl).tointeger() << "\n";

  cout << "das ganze in Binaerstellen:\n";
  cout << "a/b=         ";
  binout(cout,bigfloat(a/b).get_significant());
  cout << "\n";
  cout << "a_dbl/b_dbl= ";
  binout(cout,bigfloat(a_dbl/b_dbl).get_significant());
  cout << "\n";

  
}
 
