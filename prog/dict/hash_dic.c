#include <LEDA/hash.h>


int Hash(const string& x) { return (x.length() > 0) ? x[0] : 0; }

int hash_last(const string& x) 
{ int l = x.length();
  return (l > 0) ? x[l-1] : 0; }

main()
{
  hash<string,int> D;
  hash<string,int> D1(hash_last);

  hash_item it;
  string s;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
  }

  forall_items(it,D) cout <<  D.inf(it) << " " << D.key(it) << "\n";

  return 0;
}

