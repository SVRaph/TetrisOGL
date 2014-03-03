#include "terrain.hpp"


template<typename T>
Terrain::Terrain(int largeur,int hauteur)
{
  sx=largeur; //x
  sy=hauteur; //y
  v.resize(sx,std::vector<T>( sy,(T)0 ));

  // Les bords
  for(int xi=0;xi<sx;xi++)
    {
      v[xi][0]=(T)1;
    }
  for(int yi=0;yi<sy;yi++)
    {
      v[0][yi]=(T)1;
      v[sx-1][yi]=(T)1;
    }
}

template<typename T>
int Terrain::checkLines()
{
  int n=0;
  bool b;

  // pour chaque ligne
  for(int yi=sx-1;yi>0;yi--)
    {
      b=true;
      for(int xi=1;(b && xi<sx-1);xi++)
	b=(v[xi][yi]!=(T)0);
      // si ligne pleine on supprime en remontant
      if (b)
	{
	  n++;
	  for(int xj=1;xj<sx-1;xj++)
	    {
	      for(int yj=yi;yj<sy-1;yj++)
		v[xj][yj]=v[xj][yj+1];
	      v[xj][sy-1]=(T)0;
	    }
	}
    }
  return n;
}


//renvoie true ssi c'est ok
template<typename T>
bool Terrain::isValid(const Tetrominos& P) const
{
  bool b=true; // can add
  int x,y;
  for(int i=0;(i<16 && b);i++)
    {
      if (!P.fshape(i)) continue;
      x=(i%4)+P.pos[0];
      y=(i/4)+P.pos[1];
      b = ( (x>=0) && (y >=0) && (x<sx) && (y<sy) && (v[x][y]==(T)0) );
    }
  return b;
}
/*
template<typename I>
bool isValidPT(const Tetrominos* P,const Terrain<I>* T)
{
  bool b=true;
  int xi,yi;
  for(int i=0;(i<16 && b);i++)
    {
      if (!P->fshape(i)) continue;
      xi=(i%4)+P->pos[0];
      yi=(i/4)+P->pos[1];
      b = ( (xi>=0) && (yi >=0) && (xi<T->sx) && (yi<T->sy) && (T->v[xi][yi]==(I)0) );
    }
  return b;
}
*/


//renvoie si le terrain a pu être modifié
template<typename T>
bool Terrain::apply(const Tetrominos& P,int& nbl)
{
  if (!isValid(P)) return false; // ça bloque
  int x,y;
  // on insère le tetrominos dans la grille
  for(int i=0;i<16;i++)
    {
      if (!P.fshape(i)) continue;
      x=(i%4)+P.pos[0];
      y=(i/4)+P.pos[1];
      v[x][y]=(T)(P.type+2);
    }
  nbl=checkLines();
  return true;
}


void Grille::gldisplay()
{
  for(int xi=0;xi<size[0];xi++)
    for(int yi=0;yi<size[1];yi++)
      {
	if (v[xi][yi]==0) continue;
	DrawCube3((float)xi,(float)yi,3*v[xi][yi]);
      }
}



// ----------------------
//        IA
// ----------------------



GrilleIA::GrilleIA(const Grille* T)
{
  if (T==NULL)
    {
      sx=0;
      sy=0;
    }
  else
    {
      sx=T->sx;
      sy=T->sy;
    }
  v.resize(sx, std::vector<bool>(sy,false));
  for(int x=0;x<sx;x++)
    for(int y=0;y<sy;y++)
      if (T->v[x][y]!=0) v[x][y]=true;
}


int GrilleIA::nbre_trous() const
{
  int nb=0;
  bool ok=false;
  for(int x=1;x<sx;x++)
    {
      ok=false;
      for (int y=sy;y>1;y--)
	{
	  if (!ok &&  v[x][y]) ok=true;
	  if ( ok && !v[x][y]) nb++;
	}
    }
  return nb;
}
int GrilleIA::puits() const
{
  int nb=0;
  //formes particulières de la surface associées aux tetrominos du même nom
  bool z=false,s=false,o=false;
  int* hs=new int[sx];
  for (int x=0;x<sx;x++)
    {
      for (int y=sy;y>0;y--)
	{
	  if (v[x][y])
	    {
	      hs[x]=y;
	      s=s||(y-1==hs[x-1]);
	      z=z||(y+0==hs[x-1]);
	      o=o||(y+1==hs[x-1]);
	      break;
	    }
	}
    }
  for (int x=1;x<sx-1;x++)
    {
      int m=std::min(hs[x-1]-hs[x],hs[x+1]-hs[x]);
      if (m>2) nb+=m;
    }
  delete[] hs;
  return (nb+3-s-z-o);
}
int GrilleIA::h_pond() const
{
  int h=0;
  for (int x=1;x<sx-1;x++)
    for (int y=1;y<sy;y++)
      if (v[x][y]) h += y;
  return h;
}
int GrilleIA::prof_trous() const
{
  int nb=0;
  bool ok=false;
  int prof=0;
  for (int x=1;x<sx-1;x++)
    {
      ok=false;
      prof=0;
      for (int y=sy;y>1;y--)
	{
	  if (!ok && v[x][y]) ok=true;
	  if ( ok ) prof++;
	  if ( ok && !v[x][y]) nb+=prof;
	}
    }
  return nb;
}



float GrilleIA::score() const
{
  return calcul_poly(STRAT,(float)puits(),(float)nbre_trous(),(float)h_pond(),(float)prof_trous());
}

float simulation(const GrilleIA& Gref, int type)
{
  float m=INF;
  int nbl;
  bool modif=false;
  GrilleIA G(Gref);
  Tetrominos P(type); 

  for(int s2=0;s2<4;s2++)
    for(int x2=0;x2<Gref.sx+1;x2++)
      {
	if (modif) G=Gref;

	// on place la piece
	G.setPositionIA(P,x2,s2);

	modif=G.apply(P,nbl);
	if (!modif) continue; // ça bloque

	m=std::min(m,G.score());	
      }
  return m;
}
float instructions(const Terrain* pT,int type1,int type2,int& xmin, int& rmin)
{
  GrilleIA Gref(pT);
  GrilleIA G(Gref);
  Tetrominos P(type1);

  float min,m;
  min=INF;
  int nbl=0;
  bool modif=false;

  for(int s1=0;s1<4;s1++)
    for(int x1=0;x1<Gref.sx+1;x1++)
      {
	if (modif) G=Gref;

	// on place la piece
	G.setPositionIA(P,x1,s1);

	// on l'ajoute à la grille
	modif = G.apply(P,nbl);
	if (!modif) continue; // ça bloque
	
	m=simulation(G,type2);
	
	if (m<min) 
	  {
	    min=m;
	    xmin=x1;
	    rmin=s1;
	  }
      }
  return min;
}


void setPositionIA(const GrilleIA& G,Tetrominos& P,int x,int s)
{
  P.pos[0]=x;
  P.pos[1]=sy-4;
  P.rot=s;
  while(isValid(P))
    P.move(0,-1);
  P.move(0,1);
}

