#include <GL/glut.h>  // GLUT, include glu.h and gl.h

#include "tetrominos.h"

/*
( 
    /x
   / 
  /______> y
  |
  |
  |z
)

  |y
  |
  |
  |_____x
  
*/


Tetrominos::Tetrominos(int t,int px,int py)
{
  assert(t<NBTETRO);

  type=t;
  rot=0;
  ref=4*type+rot;
  pos.resize(3,0);
  pos[0]=px;
  pos[1]=py;
}

void Tetrominos::fall(Terrain* T)
{
  while (isValidTT(this,T))
    move(0,-1);
  move(0,1);
}

Terrain::Terrain(int largeur,int hauteur)
{
  size[0]=largeur; //x
  size[1]=hauteur; //y

  v.resize(size[0],std::vector<uchar>(size[1],0));

  // Les bords
  for(int xi=0;xi<size[0];xi++)
    {
      v[xi][0]=1;
    }
  for(int yi=0;yi<size[1];yi++)
    {
      v[0][yi]=1;
      v[size[0]-1][yi]=1;
    }
}


void Terrain::addTetromino(Tetrominos* t)
{
    for(int i=0;i<16;i++)
    {
      if (!t->fshape(i)) continue;
      int xi=(i%4)+t->pos[0];
      int yi=(i/4)+t->pos[1];
      v[xi][yi]=t->type+2;
    }
}

int Terrain::checkLines()
{
  int n=0;
  bool b;

  // pour chaque ligne
  for(int yi=size[1]-1;yi>0;yi--)
    {
      b=true;
      for(int xi=1;(b && xi<size[0]-1);xi++)
	  b=(v[xi][yi]!=0);
      // si ligne pleine on supprime en remontant
      if (b)
	{
	  n++;
	  for(int xj=1;xj<size[0]-1;xj++)
	    {
	      for(int yj=yi;yj<size[1]-1;yj++)
		v[xj][yj]=v[xj][yj+1];
	      v[xj][size[1]-1]=0;
	    }
	}
    }
  return n;
}


void Joueur::newTetromino()
{
  assert(T!=NULL);
  if (P!=NULL) delete P;
  int r=rand()%NBTETRO;
  P = new Tetrominos(r,T->size[0]/2-2,T->size[1]-4);
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
  P->move(0,-1);
  if (!isValid())
    {
      P->move(0,1);
      T->addTetromino(P);
      T->checkLines();
      newTetromino();
    }
}

bool isValidTT(Tetrominos* P,Terrain* T)
{
  bool b=true;
  int xi,yi;
  for(int i=0;(i<16 && b);i++)
    {
      if (!P->fshape(i)) continue;
      xi=(i%4)+P->pos[0];
      yi=(i/4)+P->pos[1];
      b = ( (xi>=0) && (yi >=0) && (xi<T->size[0]) && (yi<T->size[1]) && (T->v[xi][yi]==0) );
    }
  return b;
}


Joueur::Joueur()
{
  T=NULL;
  P=NULL;
  score=0;
}
void Joueur::init(int w,int h)
{
  T=new Terrain(w,h);
  newTetromino();
}
Joueur::~Joueur()
{
  delete T;
  delete P;
}
Tetris::Tetris(int w,int h,int n)
{
  fallMillis=500;
  nbj=n;
  nbh=n;
  vJ.resize(nbj);
  for(int j=0;j<n;j++)
    {
      vJ[j].init(w,h);
    }
}
void Tetris::update()
{
  for(int j=0;j<nbj;j++)
    {
      vJ[j].update();
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
void Terrain::gldisplay()
{
  for(int xi=0;xi<size[0];xi++)
    for(int yi=0;yi<size[1];yi++)
      {
	if (v[xi][yi]==0) continue;
	DrawCube3((float)xi,(float)yi,3*v[xi][yi]);
      }
}

void Tetrominos::gldisplay()
{
  for(int i=0;i<16;i++)
    {
      if (!fshape(i)) continue;
      int xi=(i%4)+pos[0];
      int yi=(i/4)+pos[1];
      
      DrawCube5(float(xi),float(yi),fcolor(0),fcolor(1),fcolor(2));
    }
}

void DrawCube5(float xPos,float yPos,uchar cR,uchar cG,uchar cB)
{
  // Dessine un cube
  // en 2D

  const float c=0.4f;// côté
 
  glPushMatrix();
  glTranslatef(xPos,yPos,0.0f);
  glColor3ub(cR,cG,cB); //couleur
  glBegin(GL_QUADS);

  glVertex2f(+c,+c);
  glVertex2f(+c,-c);
  glVertex2f(-c,-c);
  glVertex2f(-c,+c);

  glEnd();
  glPopMatrix();
}

void DrawCube3(float xPos,float yPos,int iC)
{
  DrawCube5(xPos,yPos,COLOR[iC+0],COLOR[iC+1],COLOR[iC+2]);
}
