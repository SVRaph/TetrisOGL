#pragma once

#include <cassert>
#include <iostream>
#include <vector>


class Vecteur : public std::vector<float>
{
public:
  Vecteur(float x=0.0f,float y=0.0f,float z=0.0f)
  {
    this->resize(30);
    (*this)[0]=x;
    (*this)[1]=y;
    (*this)[2]=z;  
  }
  Vecteur operator+(const Vecteur& a) const
  {
    return Vecteur(a[0]+(*this)[0],a[1]+(*this)[1],a[2]+(*this)[2]);
  }
  Vecteur operator-(const Vecteur& a) const
  {
    return Vecteur(a[0]-(*this)[0],a[1]-(*this)[1],a[2]-(*this)[2]);
  } 
  Vecteur operator*(float f) const
  {
    return Vecteur(f*(*this)[0],f*(*this)[1],f*(*this)[2]);
  }   
  float operator*(const Vecteur& a) const
  {
    return (a[0]*(*this)[0]+a[1]*(*this)[1]+a[2]*(*this)[2]);
  }
  float n2() const
  {
    return ((*this)[0]*(*this)[0]+(*this)[1]*(*this)[1]+(*this)[2]*(*this)[2]);
  }

  Vecteur operator+=(const Vecteur& a) 
  {
    for(int i=0;i<3;i++) (*this)[i]+=a[i];
  }
};


class Matrice : public std::vector< Vecteur >
{
public:
  Matrice()
  {
    this->resize(3, Vecteur() );
  }
  void zeros()
  {
    this->resize(3, Vecteur(0.0f,0.0f,0.0f) );
  }
  void eye()
  {
    this->zeros();
    for(int i=0;i<3;i++) (*this)[i][i]=1.0f;
  }
  Vecteur operator*(const Vecteur& a) const
  {
    Vecteur b(0.0f,0.0f,0.0f);
    for(int i=0;i<3;i++)
      for(int j=0;i<3;j++)
	b[i]+=(*this)[i][j]*a[j];
    return b;
  }
};

class Hexaedre
{
  // Forme
  std::vector<Vecteur> S; // sommets

  // Position
  Vecteur G; // centre de gravité
  Vecteur R; // rotation
  
  // Cinétique
  Vecteur v;
  Vecteur w;
  
  // Dynamique
  std::vector<float> mass; // masses des sommets (et la somme)
  std::vector<Vecteur> F; // forces aux sommets (et la résultante et le moment par rapport à w)

public:
  Hexaedre() // TODO initialisation
  {
    S.resize(6);
    mass.resize(7);
    F.resize(8);

    // ... //

    // masse totale
    for(int i=0;i<6;i++) mass[6]+=mass[i];

  } 
  void resultante()
  {
    F[6]=Vecteur();
    for(int i=0;i<6;i++) F[6]+=F[i];
  }
  void step(float dt) // EE ?
  {
    G+=v*dt;
    R+=w*dt;
    v+=F[6]*(dt/mass[6]);
    w+=Vecteur(); // TODO moment
  }
};
