#pragma once

#include "constants.h"
#include <cstdlib>
#include <ctime>

class Terrain;
class Tetrominos;

class Tetrominos
{
 public:
  std::vector<int> pos;
  int type,rot;
  int ref; // 4*type+rot
 public:
  Tetrominos(int type=0,int px=0,int py=0);
  inline bool  fshape(int i)const {return VSHAPE[16*ref+i];}
  inline uchar fcolor(int i)const {return COLOR[3*(type+2)+i];}
  void gldisplay();
  inline void move(int x,int y){pos[0]+=x;pos[1]+=y;}
  inline void moveto(int x,int y){pos[0]=x;pos[1]=y;}
  inline void turn(int r){rot=(rot+r)%4;ref=4*type+rot;}
  void fall(Terrain* T);
  friend bool isValidTT(Tetrominos*, Terrain*);
  friend class Joueur;
};

class Terrain
{
  std::vector< std::vector<uchar> > v;

 public:
  int size[2];
 public:
  Terrain(int,int);
  void gldisplay();
  void addTetromino(Tetrominos*);
  int checkLines();// return the number of lines deleted
  friend bool isValidTT(Tetrominos*, Terrain*);
  friend class Joueur;
};

class Joueur
{
 public:
  Terrain* T; // TODO : terrain partagé...
  Tetrominos* P;
  int score;
  Joueur(int,int);
  ~Joueur();
  inline bool isValid(){return isValidTT(P,T);}
  void update();
  void newTetromino();
  void getKey(int key);
};

class Tetris
{
  int nbj;
  int jkeyboard;
  std::vector< Joueur > vJ;

 public:
  Tetris(int w,int h,int n=1);
  std::vector<float> winBounds();
  void gldisplay();
  void update();
  inline void getKey(int key){vJ[jkeyboard].getKey(key);}
};


void DrawCube5(float,float,uchar,uchar,uchar);
void DrawCube3(float,float,int);
