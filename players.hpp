#pragma once


#include "constants.hpp"
#include "tetrominos.hpp"
#include "terrain.hpp"


//renvoie si le terrain a pu être modifié
template<typename I>
bool isValid(const Tetrominos* P,const Terrain<I>* T)
{
  bool b=true;
  int xi,yi;
  for(int i=0;(i<16 && b);i++)
    {
      if (!P->fshape(i)) continue;
      xi=(i%4)+P->pos[0];
      yi=(i/4)+P->pos[1];
      b = ( (xi>=0) && (yi >=0) && (xi<T->sx) && (yi<T->sy) && (T->v[xi][yi]==0) );
    }
  return b;
}
template<typename I>
bool apply(const Tetrominos* P,Terrain<I>* T,int& nbl)
{
  if (!isValid(P,T)) return false; // ça bloque
  int x,y;
  // on insère le tetrominos dans la grille
  for(int i=0;i<16;i++)
    {
      if (!P->fshape(i)) continue;
      x=(i%4)+P->pos[0];
      y=(i/4)+P->pos[1];
      T->v[x][y]=(I)(P->type+2);
    }
  nbl=T->checkLines();
  return true;
}


class Joueur
{
 public:
  Grille* T;
  Tetrominos* P;
  Tetrominos* Pnext;
  int score;
  int level;

  Joueur();
  void init(int,int);
  ~Joueur();
  void update(int&);
  void getKey(int key);
  virtual void moveIA(){std::cout<<"oups"<<std::endl;};
  virtual void newTetromino();

  bool fisValid(){return isValid(P,T);}
  bool fapply(int& nbl){return apply(P,T,nbl);}
  bool move(int x,int y);
  bool turn(int);
  void fall();
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
  int sx,sy;
  int level;
  std::vector< Joueur* > vJ;
 public:
  int IAMillis(){return 250-10*level;}
  int fallMillis(){return 530-50*level;}

  Tetris(int w,int h,int n=1,int lv=1);
  ~Tetris();
  std::vector<float> winBounds();
  void gldisplay();
  void update();
  void IAupdate();
  inline void keyboard(int key){if (nbh>0) vJ[0]->getKey(key);}
};



// renvoie le meilleur score possible
float simulation(const GrilleIA& g,int type);
// renvoie le meilleur score possible et les instructions
float instructions(const Grille* pT,int type1,int type2,int& xmin,int& rmin);

// place la pièce
template<typename I>
void setPositionIA(Tetrominos* P, const Terrain<I>* T,int x,int s)
{
  P->pos[0]=x;
  P->pos[1]=T->sy-4;
  P->rot=s;
  while (isValid(P,T))
    P->move(0,-1);
  P->move(0,1);
}
