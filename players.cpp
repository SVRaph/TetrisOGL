#include "players.hpp"
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

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
      P = new Tetrominos(r,T->sx/2-2,T->sy-4);
    }
  int r=rand()%NBTETRO;
  Pnext = new Tetrominos(r,T->sx/2-2,T->sy-4);
}

void Joueur::update(int& nbl,bool& gover)
{
  nbl=0;
  gover=false;
  if (!move(0,-1))
    {
      gover=(P->pos[1]>=T->sy-4);
      fapply(nbl);
      newTetromino();
    }
}


bool Joueur::move(int x,int y)
{
  P->move(x,y);
  if (!fisValid()) 
    {
      P->move(-x,-y);
      return false;
    }
  return true;
}
bool Joueur::turn(int s)
{
  P->turn(s);
  if (!fisValid()) 
    {
      P->turn(-s);
      return false;
    }
  return true;
}
void Joueur::fall()
{
  while (move(0,-1)) {}
}


// --- Human --- //

void Human::getKey(int key,bool down)
{
  for(int i=0;i<4;i++)
    {
      if (key==keyconf[i])
	keypressed[i]=down;
    }
  this->command();
}
void Human::command()
{
  if (keypressed[0])
    move(+1,0);
  if (keypressed[1])
    move(-1,0);
  if (keypressed[2])
    {
      turn(+1);
      keypressed[2]=false;
    }
  if (keypressed[3]) 
    {
     fall();
     keypressed[3]=false;
    }
}

// --- IA --- //

void IA::newTetromino()
{
  Joueur::newTetromino();
  instructions(T,P->type,Pnext->type,xobj,robj);
}
void IA::command()
{
  if (P->rot != robj) 
    turn(+1);
  else if (P->pos[0] < xobj)
    move(+1,0);
  else if (P->pos[0] > xobj)
    move(-1,0);
  else if (level>5)
    fall();
}

// --- Tetris --- // 
Tetris::Tetris(int w,int h,int n1,int n2,int n3,int lv)
{
  gameover=false;
  sx=w;
  sy=h;
  level=lv;

  nbj=n1+n2+n3;
  nbh=n1;
  nbIA=n2;
  nbnet=n3;

  vJ.resize(nbj,NULL);
  int j=0;
  for(;j<nbh;j++)
    {
      vJ[j]= new Human;
      vJ[j]->init(w,h);
    }
  for(;j<nbh+nbIA;j++)
    {
      vJ[j]= new IA(level);
      vJ[j]->init(w,h);
    }
  for(;j<nbj;j++)
    {
      vJ[j]= new Joueur;
      vJ[j]->init(w,h);
    }
}
Tetris::~Tetris()
{
  for(int j=0;j<nbj;j++)
    {
      delete vJ[j];
    }
}
void Tetris::reinit(int w,int h,int n1,int n2,int n3,int lv)
{ 
  for(int j=0;j<nbj;j++)
    {
      delete vJ[j];
    }
  gameover=false;
  sx=w;
  sy=h;
  level=lv;

  nbj=n1+n2+n3;
  nbh=n1;
  nbIA=n2;
  nbnet=n3;

  std::cout<<"Reinitialisation : "<<nbj<<std::endl;
  vJ.resize(nbj,NULL);
  int j=0;
  for(;j<nbh;j++)
    {
      vJ[j]= new Human;
      vJ[j]->init(w,h);
    }
  for(;j<nbh+nbIA;j++)
    {
      vJ[j]= new IA(level);
      vJ[j]->init(w,h);
    }
  for(;j<nbj;j++)
    {
      vJ[j]= new Joueur;
      vJ[j]->init(w,h);
    }
}
  
void Tetris::update()
{
  if (ispaused) return;
  int nbl;
  bool disp_scores=false;
  bool bgame_over;
  for(int j=0;j<nbh+nbIA;j++)
    {
      vJ[j]->update(nbl,bgame_over);
      assert(nbl<5);
      if (nbl>0)
	{
	  vJ[j]->score+=SCORE[nbl];
	  disp_scores=true;
	  vJ[(j+1)%nbj]->T->addLines(4,nbl-1);
	}
      if(bgame_over)
	{
	  std::cout<<"Partie terminée"<<std::endl;
	  this->gameover=true;
	}
    }
  if (disp_scores)
    {
      std::cout<<"scores :";
      for(int j=0;j<nbj;j++)
	std::cout<<vJ[j]->score<<" ";
      std::cout<<std::endl;
    }
}
void Tetris::command(bool ia)
{
  if (ispaused) return;
  int ni=(ia?nbh:0);
  int nf=(ia?nbIA:nbh);
  for(int j=ni;j<ni+nf;j++)
    {
      vJ[j]->command();
    }
}



// ------------- //
//   Affichage
// ------------- //

std::vector<float> Tetris::winBounds()
{
  std::vector<float> v(4,0.0);
  v[1]=(float)(sx*nbj);
  v[3]=(float)sy;
  return v;
}

void Tetris::gldisplay()
{
  glPushMatrix();
  for(int j=0;j<nbj;j++)
    {
      if (j>0) glTranslatef((float)sx,0.0f,0.0f);
      if (j<nbh+nbIA) vJ[j]->P->gldisplay();
      vJ[j]->T->gldisplay();
    }
  glPopMatrix();
}


// ----------------------
//        IA
// ----------------------


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
	setPositionIA(&P,&G,x2,s2);

	modif=apply(&P,&G,nbl);
	if (!modif) continue; // ça bloque

	m=std::min(m,G.score());	
      }
  return m;
}

float instructions(const Grille* pT,int type1,int type2,int& xmin, int& rmin)
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
	setPositionIA(&P,&G,x1,s1);

	// on l'ajoute à la grille
	modif = apply(&P,&G,nbl);
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



// ----------------------
//       Network
// ----------------------

void Tetris::get_data(std::vector<uint32_t>& buf) const
{
  int n=nbh+nbIA;  // nombre de joueurs en local
  int len=1+n+n*sx*sy; // 2,score1...scoren,[T1 (en col)],...,[Tn]
  assert(len<=buf.size()); // si il y a trop de joueurs par exemple
  buf[0]=2;
  for(int j=0;j<n;j++)
    {
      //les scores
      buf[1+j]=vJ[j]->score;
      // les terrains
      for(int x=0;x<sx;x++)
	for(int y=0;y<sy;y++)
	  {
	    buf[1+n+y+(x+j*sx)*sy]=vJ[j]->T->v[x][y];
	  }
      // les pièces
      for(int i=0;i<16;i++)
	{
	  if (!vJ[j]->P->fshape(i)) continue;
	  int x=(i%4)+vJ[j]->P->pos[0];
	  int y=(i/4)+vJ[j]->P->pos[1];
	  buf[1+n+y+(x+j*sx)*sy]=(vJ[j]->P->type+2);
	}
    }
}
  
// RQ : pas de réalocation mémoire
void Tetris::set_data(const std::vector<uint32_t>& buf)
{
  int n=nbnet; // nbre de joueurs distants
  int len=1+n+n*sx*sy;
  assert(len<buf.size());
 
  for(int j=0;j<n;j++)
    {
      vJ[nbh+nbIA+j]->score=buf[1+j];
      for(int x=0;x<sx;x++)
	for(int y=0;y<sy;y++)
	  {
	    vJ[nbh+nbIA+j]->T->v[x][y]=buf[1+n+y+(x+j*sx)*sy];
	  }
    }
}
  
