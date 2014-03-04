#pragma once

#include "constants.hpp"

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
  inline void move(int x,int y){pos[0]+=x;pos[1]+=y;}
  inline void turn(int r){rot=(rot+r)%4;}

  void gldisplay();
};

