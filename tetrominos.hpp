#pragma once

#include "constants.hpp"

/*

  |y
  |
  |
  |_____x

*/


class Tetrominos
{
public:
  std::vector<int> pos;
  int type,rot;
public:
  Tetrominos(int t=0,int px=0,int py=0)
  {
    assert(t<NBTETRO);
    type=t;
    rot=0;
    pos.resize(2,0);
    pos[0]=px;
    pos[1]=py;
  }

  inline bool  fshape(int i)const {return VSHAPE[16*fref()+i];}
  inline uchar fcolor(int i=0)const {return COLOR[3*(type+2)+i];}
  inline int fref()const {return 4*type+rot;}
  inline void move(int x,int y){pos[0]+=x;pos[1]+=y;}
  inline void turn(int r){rot=(rot+r)%4;}

  void gldisplay()
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
};


/*
( 
    /x
   / 
  /______> y
  |
  |
  |z
)
*/
