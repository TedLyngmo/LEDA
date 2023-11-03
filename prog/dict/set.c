#include <LEDA/set.h>


//void print(set<string> S, string s)

void print(const set<string>& S, string s)
{ std::cout << s;
  forall(s,S) std::cout << s << " ";
  newline;
 }

int main()
{

set<string> S, S1;


string s;

while (cin >> s)
 { if (s == "stop") break;
   S.insert(s);
  }

S1 = S;

print(S, "S = ");
print(S1,"S1 = ");

while (cin >> s)
   if (S1.member(s))
   { std::cout << "delete " << s << "\n";
     S1.del(s);
    }
   else std::cout << s << "  not in S1 \n";


print(S, "S = ");
print(S1,"S1 = ");


 return 0;

}

