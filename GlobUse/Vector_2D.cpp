//Vector_2D.cpp - class Vector_2D representing routine geometry procedures in 2D space
//© Petrov V. MSUN 2017
// File Vector_2D.cpp
#include "stdafx.h"            
#include <math.h>
#include <stdlib.h>


#include "Vector_2D.h"



//static TCHAR str[255];
static TCHAR strk[255];
//CString strFormat(CString frmt, ...);



Vector_2D RndVector_2D()
{
Vector_2D v ( rand() - 0.5*RAND_MAX, rand() - 0.5*RAND_MAX);
return Normalize(v);
}

Vector_2D& Clip ( Vector_2D& v)
{
if ( v.x <0.)
     v.x = 0.;
else
   if ( v.x >1.)  
      v.x = 1.;

if ( v.y <0.)
     v.y = 0.;
else
   if ( v.y >1.)  
      v.y = 1.;


return v;

}
///////////////////////////////////////////////////////////////////
int PtInPoly( CPoint a, LPPOINT  p, int n );
int PtInRealPoly( Vector_2D a, Vector_2D * p, int n );
BOOL Vector_2D::ifPointBetween( Vector_2D v, Vector_2D u )
{
 
 if ( x < min ( v.x,u.x) - GeomTreshold )
    return FALSE;
 if ( x > max ( v.x,u.x) + GeomTreshold )
    return FALSE;
 
 if ( y < min ( v.y,u.y) - GeomTreshold )
    return FALSE;
 if ( y > max ( v.y,u.y) + GeomTreshold )
    return FALSE;

 
//AfxMessageBox( _T("I'm here, ifPointBetween"), MB_OK);

 Vector_2D ss = ((* this) - v);
  if( ! ss < GeomTreshold )
       return TRUE;


 Vector_2D ss1 = ((* this) - u);
  if( ! ss1 < GeomTreshold )
       return TRUE;


 ss = Normalize(ss);
 
 Vector_2D dd = u - v;
 if ( !dd < GeomTreshold )
    return FALSE;
 
 dd = Normalize(dd);
 
//_stprintf_s( str, _T("!( ss^dd )=%g"), !( ss^dd ));
//AfxMessageBox(str, MB_OK); 
 return ( fabs( ss^dd ) < GeomTreshold);      
          
}
//////////////////////////////////////////////////////////////////////
void Vector_2D::SaveFile( CFile  * pFile )
{
	pFile->Write( &x, sizeof(double));
	pFile->Write( &y, sizeof(double));
}
///////////////////////////////////////////////////////////////
void Vector_2D::OpenFile( CFile  * pFile )
{
	pFile->Read( &x, sizeof(double));
	pFile->Read( &y, sizeof(double));
}
///////////////////////////////////////////////////////////////
void Vector_2D::TestVector_2DBox(CString strText)
{
	CString wString;
//	wString.Format(_T("x = %6.2f  y = %6.2f z = %6.2f\n"), x, y, z);
	wString.Format(_T("x = %6.2f  y = %6.2f\n"), x, y);
//	TestMessageBox(strText + wString);
}
/////////////////////////////////////////////////////////////////
//CString Vector_2D::strVector_2D(void)
//{
////	return 	strFormat(_T("( %7.2f, %7.2f, %7.2f)"), x,y,z);
//	return 	strFormat(_T("( %7.3f, %7.3f)"), x,y);
////	return 	strFormat(_T("( %7.6f, %7.6f, %7.6f)"), x,y,z);
////	return 	strFormat(_T("( %g, %g, %g)"), x,y,z);
//
//}
/////////////////////////////////////////////////////////////////////
void Vector_2D::SetProperty(CString strProp)
{
//	m_strProp = strProp;
	if(!bHasProperty(strProp))
		m_strPropArray.Add(strProp);
}
///////////////////////////////////////////////////////////////////
BOOL Vector_2D::bHasProperty(CString strProp)
{
	for(int i = 0; i < m_strPropArray.GetSize(); i++)
		if(m_strPropArray.GetAt(i) == strProp)
			return TRUE;
	return FALSE;
}
//////////////////////////////////////////////////////////////////
void Vector_2D::RemoveProperty(CString strProp)
{
//	m_strProp = _T("");
	for(int i = 0; i < m_strPropArray.GetSize(); i++)
		if(m_strPropArray.GetAt(i) == strProp)
		{
			m_strPropArray.RemoveAt(i);
			break;
		}
}
///////////////////////////////////////////////////////////////////////
CString Vector_2D::strProperty(void)
{
CString wString;
	for(int i = 0; i < m_strPropArray.GetSize(); i++)
		wString += m_strPropArray[i];
return wString;
}
///////////////////////////////////////////////////////////////////////
void Vector_2D::CopyProperty(Vector_2D * pSrc)
{
	m_strPropArray.RemoveAll();
	for (int i = 0; i < pSrc->m_strPropArray.GetSize(); i++)
		m_strPropArray.Add(pSrc->m_strPropArray.GetAt(i));

}
//////////////////////////////////////////////////////////////////

void Vector_2D::RemoveAllProperties(void)
{
	m_strPropArray.RemoveAll();
}
///////////////////////////////////////////////////////////////////////
void Vector_2D::CheckAppendPoint(Vector_2D ** pV)
{
    if ((** pV) == NULL)
		return;

	if(m_pPointList.GetCount())
	{
	  POSITION pos = m_pPointList.Find(* pV);
      if(pos != NULL)
	     return;
	}//if(m_pPointList.GetCount())


	for (POSITION pos = m_pPointList.GetHeadPosition(); pos != NULL;)
	{
		Vector_2D * ptr = (Vector_2D *)m_pPointList.GetNext(pos);
		if(ptr == (*pV))
			return;
		if(ptr->dist(**pV)<GeomTreshold)
		{
          delete pV;
		  (*pV) = ptr;
		  return;
		}//if(!((* ptr) - (* pV))<GeomTreshold)
	}//for (POSITION pos = m_pPointList.GetHeadPosition(); pos != NULL;)

	m_pPointList.AddTail(*pV);


}
//////////////////////////////////////////////////////////////////////
CString strFormat(CString frmt, ...);

CString Vector_2D::strVector(void)
{
	return 	strFormat(_T("( %7.3f, %7.3f)"), x, y);

}
/////////////////////////////////////////////////////////////////////
