#pragma once

// File Vector_2D.h
class CGLObject;

#ifndef __Vector_2D__
#define __Vector_2D__
#include <math.h>

#define PI     3.1415926 
//#define KFR    57.29578 
//#define RFK    1/KFR 
class CGLObject;
class PlaneArea;
class Plane;
//class CSlab;

#define GeomTreshold 0.00001f

class CMyFile;
class Matrix;
class Vector_2D : public CObject
{ 
public:
double x,y;
Vector_2D * m_pBgn;
Vector_2D * m_pFin;



CPoint * m_pPix;

Vector_2D (){}



Vector_2D(double v) { x = y = v;  m_tikTick = -1; m_limTick = 16; m_pPix = NULL; };


~Vector_2D(void){m_pBgn = m_pFin = NULL;}
_int16 m_d;


Vector_2D ( const Vector_2D& v ){ x = v.x; y = v.y;	m_tikTick = -1; m_limTick = 16;m_pPix = NULL;};


//Vector_2D ( double vx, double vy ):
//    color(RGB(0,0,0)) 
//	{ x = vx; y = vy; };

Vector_2D(double vx, double vy) { x = vx; y = vy; m_tikTick = -1; m_limTick = 16; m_pPix = NULL;};



//Vector_2D& operator =  ( const Vector_2D& v ) { x = v.x; y = v.y; m_pBgn = v.m_pBgn; m_pFin = v.m_pFin; return  *this; };
Vector_2D& operator =  ( const Vector_2D& v ) { x = v.x; y = v.y;  return  *this; };
Vector_2D& operator =  ( double f ) { x = y =  f; return  *this;  };
Vector_2D& operator - () const;
Vector_2D& operator -= ( const Vector_2D& );
Vector_2D& operator += ( const Vector_2D& );
Vector_2D& operator *= ( const Vector_2D& );
Vector_2D& operator *= ( double );
Vector_2D& operator /= ( double );
friend Vector_2D operator + ( const Vector_2D&,  const Vector_2D& ); 
friend Vector_2D operator - ( const Vector_2D&,  const Vector_2D& ); 
friend double operator * ( const Vector_2D&,  const Vector_2D& ); 
friend Vector_2D operator * ( double,  const Vector_2D& ); 
friend Vector_2D operator * ( const Vector_2D&, double  ); 
friend Vector_2D operator / ( const Vector_2D&, double  ); 
friend Vector_2D operator / ( const Vector_2D&, const Vector_2D&  );
friend Vector_2D operator & ( const Vector_2D& u, const Vector_2D& v  ){ return (u.x + v.x, u.y + v.y);};
friend double operator ^ ( const Vector_2D& u, const Vector_2D& v  ){return (u.x*v.y - u.y*v.x);}; 

friend double operator | ( const Vector_2D &u, const Vector_2D &v  ){ return (u.x * v.x + u.y * v.y );};
double operator ! () { return ( double ) sqrt ( x*x + y*y );}; 
double& operator [] ( int n ) { return * ( &x + n );}; 
int operator < ( double v ) { return x < v && y < v; }; 
int operator > ( double v ) { return x > v && y > v; }; 
int operator == ( const Vector_2D& u) { return u.x == x  && u.y == y ;}; 
int operator != ( const Vector_2D& u) { return u.x != x  || u.y != y;}; 



//COLORREF color;
//Vector_2D Vector_2D::NewAxisVector_2D ( Vector_2D sdvig, Matrix matrix );
BOOL ifPointBetween( Vector_2D v, Vector_2D u );
void SaveFile( CFile * pFile );
void OpenFile( CFile * pFile );
//Vector_2D Vector_2D::GetTrueVector_2D(Vector_2D vIni);
void TestVector_2DBox(CString strText);
CString strVector_2D(void);



CObList m_planeAreaList;
CObList m_pPointList;
int m_pntStyle;
CStringArray m_strPropArray;
//CString m_strProp;
void SetProperty(CString strProp);
BOOL bHasProperty(CString strProp);
void RemoveProperty(CString strProp);
void RemoveAllProperties(void);
void CopyProperty(Vector_2D * pSrc);
double dist(Vector_2D v2){return !(v2 - (* this));};  
void CheckAppendPlaneArea(PlaneArea * pPln);
void CheckAppendPoint(Vector_2D ** pV);
void RemovePlaneArea(PlaneArea * pPln);
int GetThumb(Vector_2D * pV);
CString strProperty(void);

CString m_iString;
int m_tikTick;
int m_limTick;

int GetGLNumberOf();
int GetGLNumberGlob(void);
double DistToPlane(Plane * plane);
Vector_2D ProjToPlane(Plane * plane);
CString strVector(void);

};

////////////////////  Implementation ///////////////////////////////
inline Vector_2D Normalize ( Vector_2D& v ) { double vv = !v;  return vv > GeomTreshold ? v /vv : Vector_2D(0); };     

inline Vector_2D  operator + ( const Vector_2D& u,  const Vector_2D& v )
{
  return Vector_2D( u.x + v.x, u.y + v.y );
}

inline Vector_2D  operator - ( const Vector_2D& u,  const Vector_2D& v )
{
  return Vector_2D( u.x - v.x, u.y - v.y );
}

//inline Vector_2D  operator =  ( const Vector_2D& u,  const Vector_2D& v )
//{
//  return Vector_2D( u.x = v.x, u.y = v.y, u.z = v.z );
//}

inline double  operator *  ( const Vector_2D& u,  const Vector_2D& v )
{
  return  u.x * v.x + u.y * v.y;
}

inline Vector_2D  operator *  ( const Vector_2D& u,  double f )
{
  return Vector_2D( u.x * f, u.y * f);
}

inline Vector_2D  operator *  ( double f, const Vector_2D& u )
{
  return Vector_2D( f * u.x , f * u.y  );
}

inline Vector_2D  operator /  ( const Vector_2D& u,  double f )
{
  return Vector_2D( u.x / f, u.y / f );
}




inline Vector_2D  operator /  ( const Vector_2D& u,  const Vector_2D& v )
{
  return Vector_2D( u.x / v.x, u.y / v.y);
}

inline Vector_2D&    Vector_2D::operator += ( const Vector_2D& v )
{
  x += v.x;
  y += v.y;
   return * this;
}

inline Vector_2D&    Vector_2D::operator -= ( const Vector_2D& v )
{
  x -= v.x;
  y -= v.y;
  return * this;
}

inline Vector_2D&    Vector_2D::operator *= ( double  v )
{
  x *= v;
  y *= v;
  return * this;
}

inline Vector_2D&    Vector_2D::operator *= ( const Vector_2D& v )
{
  x *= v.x;
  y *= v.y;
  return * this;
}

inline Vector_2D&    Vector_2D::operator /= ( double  v )
{
  x /= v;
  y /= v;
  return * this;
}
     
//////////////////////////// Functions /////////////////////////////
Vector_2D RndVector_2D();
Vector_2D& Clip ( Vector_2D& );   


#endif

