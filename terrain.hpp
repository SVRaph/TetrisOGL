#pragma once

#include "constants.h"
#include "tetrominos.h"

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
  int checkLines();// return the number of lines deleted
 public:
  Terrain(int,int);
  
  //renvoie si ça a réussi, met à jour le terrain si possible
  bool isValid(const Tetrominos& P) const;
  bool apply(const Tetrominos& P,int& nbl);
};


class Grille: public Terrain<uchar>
{
public:
  void gldisplay();
};


class GrilleIA: public Terrain<bool>
{
  int nbre_trous() const;
  int puits() const;
  int h_pond() const;
  int prof_trous() const; 

public:
  GrilleIA(const Grille* G=NULL);
  float score() const;
  void setPositionIA(Tetrominos& P,int x,int s) const;  
};


// renvoie le meilleur score possible
float simulation(const GrilleIA& g,int type);
// renvoie le meilleur score possible et les instructions
float instructions(const Terrain* pT,int type1,int type2,int& xmin,int& rmin);
