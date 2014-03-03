#include "players.hpp"


Joueur::Joueur()
{
  T=NULL;
  P=NULL;
  Pnext=NULL;
  score=0;
}
void Joueur::init(int w,int h)
{
  T=new Grille(w,h);
  newTetromino();
}
Joueur::~Joueur()
{
  delete T;
  delete P;
  delete Pnext;
}



void Joueur::newTetromino()
{
  assert(T!=NULL);
  if (P!=NULL) delete P;
  if (Pnext != NULL) 
    {
      P=Pnext;
    }
  else
    {
      int r=rand()%NBTETRO;
      P = new Tetrominos(r,T->size[0]/2-2,T->size[1]-4);
    }
  int r=rand()%NBTETRO;
  Pnext = new Tetrominos(r,T->size[0]/2-2,T->size[1]-4);
}



void Joueur::getKey(int key)
{
  switch (key) {
  case GLUT_KEY_RIGHT:
    P->move(+1,0);
    if (!isValid()) P->move(-1,0);
    break;
  case GLUT_KEY_LEFT: 
    P->move(-1,0);
    if (!isValid()) P->move(+1,0);
    break;
  case GLUT_KEY_UP:  
    P->turn(1);
    if (!isValid()) P->turn(-1);
    break;
  case GLUT_KEY_DOWN: 
    P->fall(T);
    break;
  }
}


void Joueur::update()
{
  int nbl=0;
  bool b;
  P->move(0,-1);
  if (!isValid())
    {
      P->move(0,1);
      b = T->apply(P,nbl);
      if (!b) std::cout<<"ERREUR Joueur::update()"<<std::endl;
      newTetromino();
    }
}


void IA::newTetromino()
{
  Joueur::newTetromino();
  std::cout<<"IA"<<std::endl;
  //instructions(T,P->type,Pnext->type,xobj,robj);
}

void IA::moveIA()
{
  if (P->rot != robj) 
    P->turn(+1);
  else if (P->pos[0] < xobj)
    P->move(+1,0);
  else if (P->pos[0] > xobj)
    P->move(-1,0);
}


Tetris::Tetris(int w,int h,int n)
{
  level=1;
  nbj=n;
  nbh=std::max(1,n);
  nbIA=nbj-nbh;
  vJ.resize(nbj,NULL);
  for(int j=0;j<nbh;j++)
    {
      vJ[j]= new Joueur;
      vJ[j].init(w,h);
    }
  for(int j=nbh;j<nbh+nbIA;j++)
    {
      vJ[j]= new IA;
      vJ[j].init(w,h);
    }
}
Tetris::~Tetris()
{
  for(int j=0;j<nbh+nbIA;j++)
    {
      delete vJ[j];
    }
}
void Tetris::update()
{
  for(int j=0;j<nbj;j++)
    {
      vJ[j].update();
    }
}
void Tetris::IAupdate()
{
  for(int j=nbh;j<nbh+nbIA;j++)
    {
      vJ[j].moveIA();
    }
}



// ------------- //
//   Affichage
// ------------- //

std::vector<float> Tetris::winBounds()
{
  std::vector<float> v(4,0.0);
  v[1]=(float)vJ[0].T->size[0]*nbj;
  v[3]=(float)vJ[0].T->size[1]*nbj;
  return v;
}

void Tetris::gldisplay()
{
  for(int j=0;j<nbj;j++)
    {
      vJ[j].P->gldisplay();
      vJ[j].T->gldisplay();
    }
}
