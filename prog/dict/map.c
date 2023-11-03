#include <LEDA/map.h>
#include <LEDA/h_array.h>
#include <LEDA/d_array.h>
#include <LEDA/dph_array.h>

main()
{ 
  int N0[10000];
  int i;
  for(i=0; i<1000; i++) N0[i] = 0;

  map<int,int>       M(0);
  h_array<int,int>   N1(0);
  d_array<int,int>   N2(0);
  dph_array<int,int> N3(0);

  int n = read_int("n = ");

  int* A = new int[n];

  rand_int.set_seed(1234*n);
  for(i=0; i<n; i++)  A[i] = rand_int(1,9999);

  float T = used_time();
  for(i=0; i<n; i++)  N0[A[i]]++;
  cout << string("c++ array: %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  M[A[i]]++;
  cout << string("map:       %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  N1[A[i]]++;
  cout << string("h_array:   %.2f sec",used_time(T)) << endl; 

  for(i=0; i<n; i++)  N2[A[i]]++;
  cout << string("d_array:   %.2f sec",used_time(T)) << endl; 


  for(i=0; i<n; i++)  N3[A[i]]++;
  cout << string("dph_array: %.2f sec",used_time(T)) << endl; 


  for(i=1; i<1000; i++)
    if (M[i]  != N0[i] || N1[i] != N0[i] || N2[i] != N0[i] || N3[i] != N0[i]) 
      cout << "ERROR" << endl;

  return 0;
}

