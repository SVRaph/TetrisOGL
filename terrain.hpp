#pragma once

#include "constants.h"

const float INF=10000.0f;
//rien, puits, nbre_trous, hauteur_pond, prof_trous
const float STRAT[5]={0.0f,3.32657f,4.95295f,0.597845f,0.12753f};


inline float calcul_poly(const float s[5],float x1,float x2,float x3,float x4){ return (s[1]*x1+s[2]*x2+s[3]*x3+s[4]*x4);}


/*

  |y
  |
  |
  |_____x
  
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
};


class GrilleIA: public Terrain<bool>
{
  int nbre_trous() const;
  int puits() const;
  int h_pond() const;
  int prof_trous() const; 

public:
  GrilleIA(int l=0,int h=0): Terrain<bool>(l,h){}
  GrilleIA(const Grille* G=NULL): Terrain<bool>(){}
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
