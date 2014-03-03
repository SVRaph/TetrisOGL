#pragma once


#include "constants.h"
#include "tetrominos.hpp"
#include "terrain.hpp"


class Joueur
{
 public:
  Grille* T;
  Tetrominos* P;
  Tetrominos* Pnext;
  int score;

  Joueur();
  void init(int,int);
  ~Joueur();
  inline bool isValid(){return T->isValid(*P);}
  void update();
  void getKey(int key);
  virtual void moveIA(){};
  virtual void newTetromino();
};

class IA : public Joueur
{
  int xobj;
  int robj;
public:
  virtual void moveIA();
  virtual void newTetromino();
};


class Tetris
{
  int nbj,nbh,nbIA;
  int level;
  std::vector< Joueur* > vJ;
 public:
  int IAMillis(){return 210-10*level;}
  int fallMillis(){return 530-50*level;}

  Tetris(int w,int h,int n);
  ~Tetris();
  std::vector<float> winBounds();
  void gldisplay();
  void update();
  void IAupdate();
  inline void keyboard(int key){if (nbh>0) vJ[0].getKey(key);}
};

