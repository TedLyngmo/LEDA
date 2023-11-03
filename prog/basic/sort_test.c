#include <LEDA/basic.h>
#include <LEDA/list.h>
#include <LEDA/array.h>



main ()
{
  int i;
  int N = read_int("N = ");

  float T = used_time();

  std::cout << "building arrays      ";
  std::cout.flush();

  array<int>    Ai(N);
  array<float>  Af(N);
  array<double> Ad(N);

  for (i = 0; i < N; i++) Ad[i] = Af[i] = Ai[i] = rand_int(1,1000000);

  std::cout << string("  %5.3f sec",used_time(T));
  newline;


  std::cout << "building lists       ";
  std::cout.flush();

  list<int>     Li;
  list<float>   Lf;
  list<double>  Ld;

  for (i = 0; i < N; i++)
  { Li.append(Ai[i]);
    Lf.append(Af[i]);
    Ld.append(Ad[i]);
  }
  std::cout << string("  %5.3f sec",used_time(T));
  newline;
  newline;

  std::cout << "sorting array<int>   ";
  std::cout.flush();
  Ai.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;


  std::cout << "sorting again        ";
  std::cout.flush();
  Ai.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;


  std::cout << "sorting array<float> ";
  std::cout.flush();
  Af.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "sorting array<double>";
  std::cout.flush();
  Ad.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;
  newline;


  std::cout << "sorting list<int>    ";
  std::cout.flush();
  Li.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "sorting again        ";
  std::cout.flush();
  Li.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "sorting list<float>  ";
  std::cout.flush();
  Lf.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  std::cout << "sorting list<double> ";
  std::cout.flush();
  Ld.sort();
  std::cout << string("  %5.3f sec",used_time(T));
  newline;
  newline;

  std::cout << "test sorting         ";
  std::cout.flush();

  for(i=0;i<N;i++)
  { int    x = Li.pop();
    float  y = Lf.pop();
    double z = Ld.pop();
    if (x != y || y != z || z != Ai[i] || Ai[i] != Af[i] || Af[i] != Ad[i])
      std::cout << "ERROR\n";
   }
  std::cout << string("  %5.3f sec",used_time(T));
  newline;

  return 0;

}


