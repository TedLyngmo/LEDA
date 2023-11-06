#include <LEDA/sortseq.h>
#include <LEDA/stream.h>


void print(sortseq<string,int>& S)
{ seq_item it;
  std::cout << S.size() << std::endl;
  forall_items(it,S) std::cout << S.key(it) << std::endl;
  newline;
}

void print1(sortseq<string,int> S)
{
  std::cout << S.size() << std::endl;
  while( !S.empty() )
  { seq_item it = S.min();
    std::cout << S.key(it) << std::endl;
    S.del_item(it);
   }
  newline;
}


int main()
{
  sortseq<string,int> S,S1,S2;

  file_istream input(read_string("file name: "));

  string s;

  while (input >> s)  S.insert(s,0);

  std::cout << S.size() << " strings.\n\n";


cout << "print: " << std::endl;
  print(S);

cout << "print1: " << std::endl;
  print1(S);

  string s1,s2;

  do { s1 = read_string("s1 = ");

       s2 = read_string("s2 = ");

       if (s1 > s2)
        { std::cerr << "illegal input: s1 > s2.\n";
          continue;
         }

       seq_item it1 = S.locate(s1);

/*
       std::cout << "SPLIT at " << S.key(it1) << "\n";
       newline;
*/

       seq_item it2 = S.locate(s2);

       while (it1!=it2)
         { std::cout << S.key(it1) << "\n";
           it1 = S.succ(it1);
          }

/*
       S.split(it1,S1,S2);  //now S is mepty

       print(S1);

       print(S2);

       S1.conc(S2);         //now S2 is empty

       S.conc(S1);          //now S1 is empty
*/


     } while (s1 != "");


  return 0;
}
