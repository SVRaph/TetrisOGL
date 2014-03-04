#include "tetrominos.hpp"

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
  pos.resize(2,0);
  pos[0]=px;
  pos[1]=py;
}


// ------------- //
//   Affichage
// ------------- //


void Tetrominos::gldisplay()
{
  int xi,yi;
  for(int i=0;i<16;i++)
    {
      if (!fshape(i)) continue;
      xi=(i%4)+pos[0];
      yi=(i/4)+pos[1];
      
      DrawCube5((float)xi,(float)yi,fcolor(0),fcolor(1),fcolor(2));
    }
}
