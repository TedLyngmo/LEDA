#include <LEDA/dictionary.h>

int main()
{
 dictionary<int,int> D;
 dic_item it;


 forall_items(it,D)
      if (D.key(it) == 17) break;


}
