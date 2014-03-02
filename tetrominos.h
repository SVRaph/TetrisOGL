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
 public:
  Tetrominos(int type=0,int px=0,int py=0);
  inline bool  fshape(int i)const {return VSHAPE[16*fref()+i];}
  inline uchar fcolor(int i=0)const {return COLOR[3*(type+2)+i];}
  inline int fref()const {return 4*type+rot;}
  void gldisplay();
  inline void move(int x,int y){pos[0]+=x;pos[1]+=y;}
  inline void turn(int r){rot=(rot+r)%4;}
  void fall(const Terrain* T);

  friend bool isValidPT(const Tetrominos*, const Terrain*);
  friend class Joueur;
};

class Terrain
{
public:
  std::vector< std::vector<uchar> > v;

 public:
  int size[2];
 public:
  Terrain(int,int);
  void gldisplay();
  void addTetromino(Tetrominos*);
  int checkLines();// return the number of lines deleted

  friend bool isValidPT(const Tetrominos*, const Terrain*);
  friend class Joueur;
};

class Joueur
{
 public:
  Terrain* T;
  Tetrominos* P;
  Tetrominos* Pnext;
  int score;
  Joueur();
  void init(int,int);
  ~Joueur();
  inline bool isValid(){return isValidPT(P,T);}
  void update();
  virtual void newTetromino();
  virtual void moveIA(){};
  void getKey(int key);
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
  std::vector< Joueur > vJ;
 public:
  int IAMillis(){return 210-10*level;}
  int fallMillis(){return 530-50*level;}
  Tetris(int w,int h,int n);
  std::vector<float> winBounds();
  void gldisplay();
  void update();
  void IAupdate();
  inline void keyboard(int key){if (nbh>0) vJ[0].getKey(key);}
};


void DrawCube5(float,float,uchar,uchar,uchar);
void DrawCube3(float,float,int);
