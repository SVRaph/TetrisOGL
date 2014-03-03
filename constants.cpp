#include "constants.h"


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

