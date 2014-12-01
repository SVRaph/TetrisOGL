#pragma once

#include "constants.hpp"


/*

  |y
  |
  |
  |_____x

On définit ici :
- une classe mère générique Terrain 
- 2 classes filles 
     - Grille représentant l'état d'une partie
     - GrilleIA destinée à être utilisé pour réaliser les expériences des IA. 


*/


template<typename T>
class Terrain
{
public:
  std::vector< std::vector<T> > v;
  int sx,sy;

  // return the number of lines deleted
  int checkLines();
 public:
  Terrain(int l=0,int h=0);
};


class Grille: public Terrain<uchar>
{
public:
  Grille(int l=0,int h=0): Terrain<uchar>(l,h){}
  void gldisplay();
  void addLines(int n0,int k=1);
};


class GrilleIA: public Terrain<bool>
{
  int nbre_trous() const;
  int puits() const;
  int h_pond() const;
  int prof_trous() const; 

public:
  GrilleIA(int l=0,int h=0): Terrain<bool>(l,h){}
  GrilleIA(const Grille* G=NULL): Terrain<bool>()
  {
    if (G != NULL)
      {
	sx=G->sx;
	sy=G->sy;
	v.resize(sx,std::vector<bool>(sy,false));
	for(int x=0;x<sx;x++)
	  for(int y=0;y<sy;y++)
	      if (G->v[x][y]!=0) 
		v[x][y]=true;		 
      }
  }
  float score() const;
};



template<typename T>
Terrain<T>::Terrain(int largeur,int hauteur)
{
  sx=largeur; //x
  sy=hauteur; //y
  v.resize(sx,std::vector<T>( sy,(T)0 ));

  // Les bords
  for(int xi=0;xi<sx;xi++)
    {
      v[xi][0]=(T)1;
    }
  for(int yi=0;yi<sy;yi++)
    {
      v[0][yi]=(T)1;
      v[sx-1][yi]=(T)1;
    }
}

template<typename T>
int Terrain<T>::checkLines()
{
  int n=0;
  bool b;

  // pour chaque ligne
  for(int yi=sx-1;yi>0;yi--)
    {
      b=true;
      for(int xi=1;(b && xi<sx-1);xi++)
	b=(v[xi][yi]!=(T)0);
      // si ligne pleine on supprime en remontant
      if (b)
	{
	  n++;
	  for(int xj=1;xj<sx-1;xj++)
	    {
	      for(int yj=yi;yj<sy-1;yj++)
		v[xj][yj]=v[xj][yj+1];
	      v[xj][sy-1]=(T)0;
	    }
	}
    }
  return n;
}
