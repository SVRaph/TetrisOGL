
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "smoothsort.hh"

const int RND = 10000;

typedef std::vector<int> vect;

void randomize_vector(vect& v)
{
  int N=v.size();
  for(int i=0;i<N;i++)
    {
      v[i]=rand()%RND;
    }
}
void noise_vector(vect& v,int n=20)
{
  int N=v.size();
  for(int k=0;k<n;k++)
    {
      int i=rand()%N;
      v[i]=rand()%RND;
    }
}


// Tri croissant de v
void mergeSort(vect& v)
{
  static vect u;

  int N=v.size();
  u.resize(N);

  // sélectionne les valeurs non triées, l'algo ne garantit pas que iu soit minimal
  // iv: le sous-vecteur v[0:iv] est trié, comme iv<=i on n'écrase aucun donnée
  // iu: le sous-vecteur u[0:iu] est rempli de manière quelconque
  int iv=0;
  int iu=-1;
  int i=1;

  // Pour éviter les cas où le premier élément est grand
  if(v[0]>v[1])
    {
      u[++iu]=v[0];
      v[0]=v[1];
      i++;
    }
  for(;i<N-2;i++)
    {
      // heuristique sur le v[i] non modifié, v[i]>=v[iv] ne suffit pas
      if ( (v[i]>=v[iv]) && (v[i]<=v[i+1] || v[i+1]<v[iv]) && (v[i]<=v[i+2] || v[i+2]<v[iv]) )
	v[++iv]=v[i];
      else
	u[++iu]=v[i];
    }
  u[++iu]=v[N-2];
  u[++iu]=v[N-1];
  /*
    for(int j=0;j<iv+1;j++) std::cout<<v[j]<<" ";
    std::cout<<std::endl<<std::endl;
    for(int j=0;j<iu+1;j++) std::cout<<u[j]<<" ";
    std::cout<<std::endl<<std::endl;
  */

  // on trie u[0:iu], par construction v l'est déjà
  //if (iu>40) std::cout<<"Longueur du vecteur à trier : "<<iu+1<<" "<<v[0]<<std::endl;
  std::sort(u.begin(),u.begin()+iu+1);
  

  // on effectue le merge en descendant
  for(i=N-1;i>=0;i--)
    v[i]=(iv>=0 && (iu<0 || v[iv]>=u[iu]))?v[iv--]:u[iu--];
}

void shakerSort(vect& v)
{
  // on préfère le shaker au bubble pour des listes quasi-triées
  int N=v.size();
  bool isSorted=false;
  int swap;
  int k=0;

  while (!isSorted)
    {
      k++;
      isSorted=true;
      for(int i=0;i<N-1;i++)
	{
	  if (v[i]>v[i+1])
	    {
	      swap=v[i+1];
	      v[i+1]=v[i];
	      v[i]=swap;
	      isSorted=false;
	    }
	}
      for(int i=N-1;i>0;i--)
	{
	  if (v[i]<v[i-1])
	    {
	      swap=v[i-1];
	      v[i-1]=v[i];
	      v[i]=swap;
	      isSorted=false;
	    }
	}
    }
  std::cout<<"Shaker sort (nb iterations): "<<k<<std::endl; 
}
void shakerSort2(vect& v)
{
  // on préfère le shaker au bubble pour des listes quasi-triées
  int N=v.size();
  int swap;

  // imax et imin (inclus) délimitent la zone à trier
  // itmp est l'indice de la dernière permutation rencontrée
  int imax=N-1;
  int imin=0;
  int itmp=imin;

  while (imax>imin)
    {
      for(int i=imin;i<imax;i++)
	{
	  if (v[i]>v[i+1])
	    {
	      swap=v[i+1];
	      v[i+1]=v[i];
	      v[i]=swap;
	      itmp=i;
	    }
	}
      imax=itmp;
      for(int i=imax;i>imin;i--)
	{
	  if (v[i]<v[i-1])
	    {
	      swap=v[i-1];
	      v[i-1]=v[i];
	      v[i]=swap;
	      itmp=i;
	    }
	}
      imin=itmp;
    }
}

void print_vector(vect& v)
{
  std::cout<<"u: ";
  for(int i=0;i<v.size();i++)
    std::cout<<v[i]<<" ";
  std::cout<<std::endl;
  //std::cout<<std::is_sorted(v.begin(),v.end())<<std::endl; // c++11 only
}

int main()
{
  srand (time(NULL));
  int N=100000;
  int n=20;

  // Initialisation

  vect u(N);
  randomize_vector(u);
  std::sort(u.begin(),u.end());

  // Tests
  /*
  noise_vector(u,n);
  mergeSort(u);

  noise_vector(u,n);
  shakerSort(u);

  return 0;
  */

  // Benchmark

  int K=100;
  clock_t begin_time,end_time;

  // c++-quicksort
  begin_time = clock();
  for(int k=0;k<K;k++)
    {
      noise_vector(u,n);
      std::sort(u.begin(),u.end());
    }
  end_time = clock();
  std::cout << "std::sort  : "<<float( end_time - begin_time ) /  CLOCKS_PER_SEC << std::endl;

  // mergesort
  begin_time = clock();
  for(int k=0;k<K;k++)
    {
      noise_vector(u,n);
      mergeSort(u);
    }
  end_time = clock();
  std::cout << "mergeSort  : "<<float( end_time - begin_time ) /  CLOCKS_PER_SEC << std::endl;


  //shakerSort
  begin_time = clock();
  for(int k=0;k<K;k++)
    {
      noise_vector(u,n);
      shakerSort2(u);
    }
  end_time = clock();
  std::cout << "shakerSort : "<<float( end_time - begin_time ) /  CLOCKS_PER_SEC << std::endl;

  //smoothSort
  begin_time = clock();
  for(int k=0;k<K;k++)
    {
      noise_vector(u,n);
      Smoothsort(u.begin(),u.end());
    }
  end_time = clock();
  std::cout << "smoothSort : "<<float( end_time - begin_time ) /  CLOCKS_PER_SEC << std::endl;

  return 0;
}
