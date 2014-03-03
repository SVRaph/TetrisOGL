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
  pos.resize(3,0);
  pos[0]=px;
  pos[1]=py;
}

void Tetrominos::fall(const Terrain* T)
{
  while (isValidPT(this,T))
    move(0,-1);
  move(0,1);
}



// ------------- //
//   Affichage
// ------------- //


void Tetrominos::gldisplay()
{
  for(int i=0;i<16;i++)
    {
      if (!fshape(i)) continue;
      int xi=(i%4)+pos[0];
      int yi=(i/4)+pos[1];
      
      DrawCube5((float)xi,(float)yi,fcolor(0),fcolor(1),fcolor(2));
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

