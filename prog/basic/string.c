#include <LEDA/basic.h>
#include <LEDA/stream.h>

int main()
{

  file_istream IN(read_string("input from file: "));

  string dummy = "string :";
  string dumm1 = "repace " + dummy;
         dummy = dumm1 + "replace";

  string s1 = read_string(dumm1);
  string s2 = read_string("by      string :");
  string s3 = read_char  ("delete  char   :");

  int i=0;
  while (IN)
  { string s;
    s.read_line(IN);
    s = s.replace_all(s1,s2);
    s = s.del_all(s3);
    std::cout << string("[%2d] ",i++) << s;
    newline;
   }
  newline;

  return 0;
}
