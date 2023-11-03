#include <LEDA/dictionary.h>
#include <LEDA/impl/ch_hash.h>

inline int Hash(const string& s)  { return (s.length() > 0) ? s[0] : 0; }

main()
{
  dictionary<string,int> D;

/*
  _dictionary<string,int,ch_hash> D;
*/

  dic_item it;
  string s;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
   }

  forall_items(it,D) 
    cout <<  D.key(it) << " : " << D.inf(it) << "\n";


  return 0;
}

