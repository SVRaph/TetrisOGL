#include "terrain.hpp"
#include <cstdlib>


void Grille::gldisplay()
{
  for(int xi=0;xi<sx;xi++)
    for(int yi=0;yi<sy;yi++)
      {
	if (v[xi][yi]==0) continue;
	DrawCube3((float)xi,(float)yi,3*v[xi][yi]);
      }
}

void Grille::addLines(int n0,int k)
{
  // n0 = nombre de trous par ligne (en moy)
  // k  = nombre de lignes
  for(int x=1;x<sx-1;x++)
    {
      for(int y=sy-1;y>k;y--)
	v[x][y]=v[x][y-k];
      for(int y=k;y>0;y--)
	{
	  int r0=rand()%(sx-2);
	  int r1=rand()%NBTETRO;
	  if (r0<n0)
	    v[x][1]=0;
	  else
	    v[x][1]=r1;
	}
    }
}


// ----------------------
//        IA
// ----------------------


int GrilleIA::nbre_trous() const
{
  int nb=0;
  bool ok=false;
  for(int x=1;x<sx;x++)
    {
      ok=false;
      for (int y=sy;y>1;y--)
	{
	  if (!ok &&  v[x][y]) ok=true;
	  if ( ok && !v[x][y]) nb++;
	}
    }
  return nb;
}
int GrilleIA::puits() const
{
  int nb=0;
  //formes particulières de la surface associées aux tetrominos du même nom
  bool z=false,s=false,o=false;
  int* hs=new int[sx];
  for (int x=0;x<sx;x++)
    {
      for (int y=sy;y>0;y--)
	{
	  if (v[x][y])
	    {
	      hs[x]=y;
	      s=s||(y-1==hs[x-1]);
	      z=z||(y+0==hs[x-1]);
	      o=o||(y+1==hs[x-1]);
	      break;
	    }
	}
    }
  for (int x=1;x<sx-1;x++)
    {
      int m=std::min(hs[x-1]-hs[x],hs[x+1]-hs[x]);
      if (m>2) nb+=m;
    }
  delete[] hs;
  return (nb+3-s-z-o);
}
int GrilleIA::h_pond() const
{
  int h=0;
  for (int x=1;x<sx-1;x++)
    for (int y=1;y<sy;y++)
      if (v[x][y]) h += y;
  return h;
}
int GrilleIA::prof_trous() const
{
  int nb=0;
  bool ok=false;
  int prof=0;
  for (int x=1;x<sx-1;x++)
    {
      ok=false;
      prof=0;
      for (int y=sy;y>1;y--)
	{
	  if (!ok && v[x][y]) ok=true;
	  if ( ok ) prof++;
	  if ( ok && !v[x][y]) nb+=prof;
	}
    }
  return nb;
}

float GrilleIA::score() const
{
  return calcul_poly(STRAT,(float)puits(),(float)nbre_trous(),(float)h_pond(),(float)prof_trous());
}
