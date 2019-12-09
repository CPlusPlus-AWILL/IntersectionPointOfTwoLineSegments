//Polygon_2D.cpp - class Polygon representing the Object List of 2D Vectors
//© Petrov V. MSUN 2017

#include "stdafx.h"

#include <sys\timeb.h>


#include "Vector_2D.h"
#include "Polygon_2D.h"
#include "..\Segment2D\Segment2D.h"
#include "..\Segment2D\ChildView.h"




#define INFIN_MY 10e10 
#pragma warning(disable : 4996)

extern CChildView * m_pView;
extern CObList m_polygonList;
void DrawMyScene(void);
extern double m_Scale;

extern int m_radarRad;

struct _timeb m_timeInit;
extern int m_runTimer;

static WCHAR strT[32767];
static WCHAR strA[32767];

extern int m_echoTime;
extern BOOL m_bBlackStroke;

 extern BOOL bTestPrint;
static int	mx,my;											// General Loops (Used For Seeking)

void TestMessageBox(CString tString);
CString strFormat(CString frmt, ...);



Polygon_2D::Polygon_2D(void)
	:R2_style(R2_COPYPEN)
	,fillStyle(HS_SOLID)
	,penWidth(1)
	,penStyle(PS_SOLID)
	,m_curPoint(-1)
	,m_pBitmap(NULL)
	,coefX(1)
	,coefY(1)
	,angle(0)
	,fgColor(RGB(0,0,255))
	,bgColor(RGB(255,255,255))
	,prior(1)
	,turnPoint(Vector_2D(0,0))
	,m_bClosed(FALSE)
	,m_bVisible(TRUE)
	,m_polyName(_T("polyName"))
//	,m_pInitArea(NULL)
	,rot(0)
	,vel(0)
{
}


Polygon_2D::~Polygon_2D(void)
{
}
/////////////////////////////////////////////////////////////////////
BOOL Polygon_2D::AppendNewPoint(Vector_2D * pV)
{

	POSITION pos = m_PointList.Find(pV);
	if(pos != NULL)
		return FALSE;
	Vector_2D * pT = GetPointOfVector( * pV);
	BOOL bRslt = pT == NULL;
	if(bRslt)
		m_PointList.AddTail(pV);

 //   if(m_pSetValuesStroke != NULL)
//		if(m_pSetValuesStroke->m_pRealDraw != NULL)
//			m_pSetValuesStroke->m_pRealDraw->AddPoint(pV);


	return bRslt;
}
//////////////////////////////////////////////////////
BOOL Polygon_2D::AddHeadPoint(Vector_2D * pV)
{

	POSITION pos = m_PointList.Find(pV);
	if(pos != NULL)
		return FALSE;
	Vector_2D * pT = GetPointOfVector( * pV);
	BOOL bRslt = pT == NULL;
	if(bRslt)
		m_PointList.AddHead(pV);

   

	return bRslt;
}
//////////////////////////////////////////////////////
Vector_2D * Polygon_2D::GetPointOfVector(Vector_2D v)
{
	for ( POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
	{
	  Vector_2D * pV = (Vector_2D *) m_PointList.GetNext(pos);
	  if(pV->dist(v)<GeomTreshold)
		  return pV;
	}//for ( POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)


return NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::DeleteContents(BOOL bPoint)
{
//TestMessageBox(_T("Polygon_2D::DeleteContents Start\n") + strPlaneArea());


	while( m_PlaneList.GetCount())
	{
		Polygon_2D *  pPl = (Polygon_2D *)m_PlaneList.GetHead();
		m_PlaneList.RemoveHead();
		pPl->DeleteContents(bPoint);
		delete pPl;
	}//while( m_PlaneList.GetCount())

//TestMessageBox(_T("Polygon_2D::DeleteContents 1\n") + strPlaneArea());

	if(bPoint)
	while(m_PointList.GetCount())
	{
		Vector_2D * pV = (Vector_2D *)m_PointList.GetHead();

//TestMessageBox(_T("Polygon_2D::DeleteContents 1.1\n") + strPlaneArea());

		m_PointList.RemoveHead();
		if(pV != NULL)
		{
//		  delete pV;
		  pV = NULL;
		}
	}

//TestMessageBox(_T("Polygon_2D::DeleteContents 2\n") + strPlaneArea());
		
		m_PointList.RemoveAll();
	m_pixArray.RemoveAll();

}
///////////////////////////////////////////////////////////
Vector_2D * Polygon_2D::GetNextPoint(Vector_2D * pV, BOOL bAll)
{
	POSITION pos = m_PointList.Find(pV);

	pV = GetPointOfVector(* pV);
	if(pV == NULL)
		return NULL;

	pos = m_PointList.Find(pV);

	Vector_2D * pVn = NULL;
	do
	{
	m_PointList.GetNext(pos);
	   if( pos != NULL)
	  	 pVn = (Vector_2D *) m_PointList.GetNext(pos);
	   else
		   if(bAll)
	  	      pVn = (Vector_2D *) m_PointList.GetHead();
	}while(pVn == pV);

	return pVn;

}
/////////////////////////////////////////////////////////////////
Vector_2D * Polygon_2D::GetPrevPoint(Vector_2D * pV, BOOL bAll)
{
	POSITION pos = m_PointList.Find(pV);

	if(pos == NULL)
		pV = GetPointOfVector(* pV);
	if(pV == NULL)
		return NULL;

	pos = m_PointList.Find(pV);

	Vector_2D * pVn = NULL;
	do
	{
	m_PointList.GetPrev(pos);
	   if( pos != NULL)
	  	 pVn = (Vector_2D *) m_PointList.GetPrev(pos);
	   else
		 if(bAll)
	  		pVn = (Vector_2D *) m_PointList.GetTail();
	}
	while(pVn == pV);

	   return pVn;

}
/////////////////////////////////////////////////////////////////

BOOL Polygon_2D::IsPointInside(Vector_2D * pT)
{
	//Vector_2D pT0(37.00, 6.25);
	//		 if(dist2(pT, &pT0)<0.01)
	//	     TestMessageBox(_T("Start Polygon_2D::IsPointInside\n"
	//		 + strFormat(_T("x = %g  y = %g\n"), pT->x, pT->y ));

//	BOOL bY = FALSE;

		 Vector_2D dMin;
		 dMin.x = dMin.y = INFIN_MY; 
		 Vector_2D dMax;
		 dMax.x = dMax.y = -INFIN_MY;
		 Vector_2D d0;
		 d0.x = d0.y = 0;

		 GetMinMaxPoint(d0, &dMin, &dMax);

		 if(pT->x > dMax.x)
			 return FALSE;
		 if(pT->x < dMin.x) 
			 return FALSE;
 		 if(pT->y > dMax.y)
			 return FALSE;
		 if(pT->y < dMin.y)
			 return FALSE;

//		 if(IsPointInBound(pT) >= 0)
//			 return TRUE;
	int numLeft = 0;
//		 Polygon_2D sCross;
	double xPrev = INFIN_MY;

	 for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;) 
	 {
	 Vector_2D * p = (Vector_2D * )m_PointList.GetNext(pos);
	 if(p->dist(*pT) < GeomTreshold)
		 return TRUE;
	 Vector_2D * pn = GetNextPoint(p,TRUE);
	 if(pn->dist(*pT) < GeomTreshold)
		 return TRUE;
	    
		if (fabs(pT->y - p->y ) < GeomTreshold && fabs(pT->y - pn->y)  < GeomTreshold)
		{
			if (p->x < pT->x && pn->x > pT->x || p->x >= pT->x && pn->x < pT->x) 
				return TRUE;
            
		Vector_2D * pnn = GetNextPoint(pn,TRUE);
		Vector_2D * pp = GetPrevPoint(p,TRUE);

		if((*pnn - *pn)*(*p - *pp) <0.)
			continue;

			if (p->x < pT->x || pn->x < pT->x)
			{
				numLeft++;

//	ShowImage(SetImage(&Segment_2D( p, pn), &Segment_2D( pT, p))); 
//	 TestMessageBox(_T("1 Polygon_2D::IsPointInside\n"
//			 + pT->strVector()
//			 + _T("\n") + p->strVector()
//			 + _T("\n") + pn->strVector()
//			 + strFormat(_T("\nnumLeft = %d\n"), numLeft)
//			 );
			}


		}//while (fabs(pT->y - pn->y)  < GeomTreshold)
		else
		{
		if(((pn->y > pT->y )&& (p->y > pT->y))||((pn->y < pT->y) && (p->y < pT->y)))
				 continue;
		     double dy = pn->y - p->y;
			 double xx = p->x + (pT->y - p->y)*(pn->x - p->x)/dy;
			 if(fabs(xx - pT->x)<GeomTreshold)
				 return TRUE;

			
			 if (xx < pT->x )
			 if (fabs(xx - xPrev) > GeomTreshold )
			 {
   				 numLeft++;
//	ShowImage(SetImage(&Segment_2D( p, pn), &Segment_2D( pT, &Vector_2D(xx, pT->y)))); 
//	 TestMessageBox(_T("2 Polygon_2D::IsPointInside\n"
//			 + pT->strVector()
//			 + _T("\n") + p->strVector()
//			 + _T("\n") + pn->strVector()
//			 + strFormat(_T("\nnumLeft = %d\n"), numLeft)
//			 );
			 }
			 
			 xPrev = xx;

		}//if (fabs(pn->y - p->y ) >= GeomTreshold)
	 }//for (int i = 0; i< m_PointList.GetCount(); i++)


//	 TestMessageBox(_T("fin Polygon_2D::IsPointInside\n"
//			 + pT->strVector()
//			 + strFormat(_T("\nnumLeft = %d\n"), numLeft));
	 return numLeft%2;
}
//////////////////////////////////////////////////////////////////////
BOOL Polygon_2D::ReadFile( CFile * pFile )
{

//TestMessageBox(_T("Polygon_2D::ReadFile\n") + (CString)strokeName);
CString strokeName;	
	
 pFile->Read(&strokeName,	sizeof(strokeName));
 pFile->Read(& fgColor,		sizeof( COLORREF));
 pFile->Read(& bgColor,		sizeof( COLORREF));
 pFile->Read(& turnPoint.x, sizeof(double));
 pFile->Read(& turnPoint.y, sizeof(double));
 pFile->Read(& angle,		sizeof( double));
 pFile->Read(& coefX,		sizeof( double));
 pFile->Read(& coefY,		sizeof( double));
 pFile->Read(& R2_style,	sizeof( __int16)); 
 pFile->Read(& fillStyle,	sizeof( __int16));
 pFile->Read(& prior,		sizeof( __int16));
 pFile->Read(& penWidth,	sizeof( __int16));
 pFile->Read(& penStyle,	sizeof( __int16));
 pFile->Read(& typeNumber,  sizeof( __int16));
 pFile->Read(& identNumber, sizeof( __int16));


 turnPoint.y = - turnPoint.y;

// penStyle = PS_SOLID;
//   penWidth = 1;
//  fillStyle = HS_SOLID;   
//  prior = Priority();
// R2_style = R2_COPYPEN;
 
//TestMessageBox(_T("Polygon_2D::ReadFile\n") + (CString)strokeName);
 
 if ( !_tcsicmp(strokeName, _T("Dbl_X_Axis")))
    typeNumber = 11;
 
 if ( !_tcsicmp(strokeName, _T("Main_X_Axis")))
    typeNumber = 11;

 if ( !_tcsicmp(strokeName, _T("Twin_X_Axis")))
    typeNumber = 11;

 
 short numPoints;
 pFile->Read(&numPoints, sizeof(__int16));
m_PointList.RemoveAll();
//m_PointList.SetSize(numPoints);
//_stprintf_s(str, _T("numPoints = %d"), numPoints);
//AfxMessageBox(str,MB_OK);

 
 
  for ( UINT k =0; k < (UINT)numPoints; k++) 
    {
     Vector_2D * ptr = new Vector_2D;
	 ptr->OpenFile(pFile);
	 ptr->y = - ptr->y;
     m_PointList.AddTail(ptr);
    }
//     pFile->Read(tempDword, sizeof(DWORD)*numPoints, 1, rc);
//   for ( UINT j=0; j<numPoints; j++)
//       m_PointList.Add(tempDword[j]);

 short numStrokes;
 pFile->Read(&numStrokes, sizeof( __int16)); 



	for (UINT i = 0; i < (UINT)numStrokes ; i++ )
	{  
	   Polygon_2D * pStroke   = new Polygon_2D;
	   pStroke->ReadFile ( pFile );
       m_PlaneList.AddTail( pStroke );
	} 

return TRUE;                
}
///////////////////////////////////////////////////////////////////
int Polygon_2D::IsPointInBound(Vector_2D * pv)
{
	int nn = GetNumberOfPoint(pv);
	if(nn >=0)
		return nn;

	int i = 0;
	 for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; i++) 
	 {
	 Vector_2D * pt = (Vector_2D * )m_PointList.GetNext(pos);
	  Vector_2D * ptN = GetNextPoint(pt, TRUE);
	  Vector_2D v0(pt->x, pt->y); 
	  Vector_2D v1(ptN->x, ptN->y); 
	  Vector_2D vt(pv->x, pv->y);
	  Vector_2D u = v1 - v0;
	  Vector_2D u1 = vt - v0;
	  Vector_2D e = Normalize(u);
	  Vector_2D e1 = Normalize(u1);
	  if(fabs(e^e1) >= GeomTreshold)
		  continue;
	  if((e * e1) < 0)
		  continue;
	  if(!u1 > !u)
		  continue;
//TestMessageBox(_T("Polygon_2D::IsPointInBound\n") + strFormat(_T("i = %d\n"), i)
//	+ v0.strVector()
//	+ _T("\n") + v1.strVector()
//	+ _T("\n") + vt.strVector());
	  return i;
	}//for(int i = 0; i < m_PointList.GetCount(); i++)
	return -1;
}
//////////////////////////////////////////////////////////////////
int Polygon_2D::GetNumberOfPoint(Vector_2D * pVt)
{
int nCount = 0;
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; nCount++)
	{
		Vector_2D * pV = (Vector_2D *)m_PointList.GetNext(pos);
		if(pV == pVt)
			return nCount;
		if(pV->dist(* pVt) < GeomTreshold)
			return nCount;
	}//for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
	return -1;
}
//////////////////////////////////////////////////////////////////
void  Polygon_2D::GetMinMaxPoint( Vector_2D tPoint, Vector_2D * pMin,  Vector_2D * pMax)
{
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * ptr = (Vector_2D *)m_PointList.GetNext(pos);
        Vector_2D rpt;
		rpt.x = ptr->x + tPoint.x;
		rpt.y = ptr->y + tPoint.y;

//TestMessageBox(_T("Polygon_2D::GetMinMaxPoint\n") + strFormat(_T(" x = %g y = %g\n"), rpt.x, rpt.y));

		if( rpt.x < pMin->x)
          pMin->x = rpt.x;
		if( rpt.y < pMin->y)
          pMin->y = rpt.y;

		if( rpt.x >pMax->x)
          pMax->x = rpt.x;
		if( rpt.y > pMax->y)
          pMax->y = rpt.y;
	}//for(int i = 0; i < m_PointList.GetCount(); i++)

    for (POSITION pos = m_PlaneList.GetHeadPosition();pos != NULL;)
	{    
		Polygon_2D * pStroke = (Polygon_2D * )m_PlaneList.GetNext(pos);

		Vector_2D myCenter = tPoint ;
         myCenter.x = tPoint.x  +  pStroke->turnPoint.x ; 
         myCenter.y = tPoint.y  +  pStroke->turnPoint.y ;
        pStroke->GetMinMaxPoint( myCenter, pMin, pMax);
	} 
}
////////////////////////////////////////////////////////////////////////////

Vector_2D Polygon_2D::GetPoint(int idx)
{

if(!m_PointList.GetCount())
  return NULL;

POSITION pos = m_PointList.FindIndex(idx);
return *(Vector_2D *) m_PointList.GetAt(pos);

}
///////////////////////////////////////////////////////

void Polygon_2D::DeletePoint(int nmbr)
{
if(nmbr <0 )
  return;
if(nmbr >=m_PointList.GetCount())
   return;

POSITION pos = m_PointList.FindIndex(nmbr);
Vector_2D * ptr = (Vector_2D * )m_PointList.GetAt(pos);
m_PointList.RemoveAt(pos);


delete ptr; 
}
/////////////////////////////////////////////////////////////////////////////
void Polygon_2D::RemovePoint(Vector_2D * pnt)
{
if(pnt == NULL )
  return;

POSITION pos = m_PointList.Find(pnt);
if(pos == NULL)
	return;
m_PointList.RemoveAt(pos);

}
/////////////////////////////////////////////////////////////////////////////
double Polygon_2D::GetTrueAngle(void)
{
	if(m_pParentPolygon == NULL)
		return 0;
	else
	     return angle + m_pParentPolygon->GetTrueAngle();
}
/////////////////////////////////////////////////////////////////////////
void Polygon_2D::MakeDoubleStroke(Polygon_2D * source)
{      

    DeleteContents(TRUE);
	if(source == NULL)
		return;

//	if(!source->m_PointList.GetCount())
//		return;
	for(POSITION pos = source->m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * ptr = (Vector_2D *)source->m_PointList.GetNext(pos);
	   AppendNewPoint(ptr);
      }
turnPoint = source->turnPoint ;
//_tcscpy_s( strokeName, source->strokeName);
//strokeName[19] = source->strokeName [19];
bgColor = source->bgColor;
fgColor = source->fgColor;
angle = source->angle;
coefX = source->coefX;
coefY = source->coefY;
R2_style = source->R2_style;
fillStyle = source->fillStyle; 
prior = source->prior; 
penWidth = source->penWidth; 
penStyle = source->penStyle; 
typeNumber = source->typeNumber;
identNumber = source->identNumber;

m_curPoint = -1;
//_stprintf_s(str, _T("%s"), strokeName );
//AfxMessageBox( str, MB_OK );

//_stprintf_s(str, _T("%s"),strokeName); 
//AfxMessageBox(str,MB_OK);                     
    	for ( POSITION pos = source->m_PlaneList.GetHeadPosition();pos != NULL;)
	{   
        Polygon_2D * pStroke = (Polygon_2D * )source->m_PlaneList.GetNext(pos);
 	    Polygon_2D * newStroke = new Polygon_2D();
        newStroke->MakeDoubleStroke( pStroke );
        m_PlaneList.AddTail(newStroke);
	} 
 
    
    
}     
///////////////////////////////////////////////////////////////

void Polygon_2D::ResetPointArray(void)
{
   while(m_PointList.GetCount())
   {
          Vector_2D * ptr = (Vector_2D *)m_PointList.GetHead();
		  m_PointList.RemoveHead();
		  delete ptr;

   }//while(m_PointList.GetCount())
}
////////////////////////////////////////////////////////////////////////////////
Vector_2D * Polygon_2D::GetPointOfNumber(int nmbr)
{
if(nmbr <0 )
  return NULL;
if(nmbr >=m_PointList.GetCount())
   return NULL;
POSITION pos = m_PointList.FindIndex(nmbr);
return (Vector_2D * )m_PointList.GetAt(pos);
}
/////////////////////////////////////////////////////////////////////////////
void Polygon_2D::Sdvig(double x, double y)
{      
	turnPoint.x += x;
	turnPoint.y += y;
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * point = (Vector_2D *)m_PointList.GetNext(pos);
	     point->x += x;
	     point->y += y;
	   }
	for (POSITION pos = m_PlaneList.GetHeadPosition();pos != NULL;)
	{   
		Polygon_2D * pStroke = (Polygon_2D * )m_PlaneList.GetNext(pos);
	     pStroke->turnPoint.x -= x;
	     pStroke->turnPoint.y -= y;
	} 
   /**  
	for (POSITION pos = m_pSetValuesStroke->m_setValuesList.GetHeadPosition();pos != NULL;)
	{   
		CSetValues * ptr = (CSetValues * )m_pSetValuesStroke->m_setValuesList.GetNext(pos);
		ptr->m_pRealStroke->turnPoint.x += x;
	    ptr->m_pRealStroke->turnPoint.y += y;
	} 
    /**/ 
}     
//////////////////////////////////////////////////////////////
int Polygon_2D::GetNumber(Vector_2D * pnt)
{   
	int i = 0;
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; i++ )
	{
		Vector_2D * pt = (Vector_2D *)m_PointList.GetNext(pos);
	  if(pt == pnt)
	       return (i);
	       
      }  

   return -1;
}

/////////////////////////////////////////////////////////////////////////////
Polygon_2D * Polygon_2D::isPointTouchStroke(CPoint point)
{

	for(int i = 0; i< m_pixArray.GetSize(); i++)
	{
		CPoint *ptr = (CPoint * )m_pixArray[i];

	
        if(fabs((double)ptr->x - point.x)<2  && fabs((double)ptr->y - point.y)<2)
			return this;
	   CPoint *ptn = (CPoint * )m_pixArray[(i +1)% m_pixArray.GetSize()];
       if(point.y < min(ptr->y, ptn->y)-1)
		   continue;
        if(point.y > max(ptr->y, ptn->y)+1)
		   continue;

       if(point.x < min(ptr->x, ptn->x)-1)
		   continue;
        if(point.x > max(ptr->x, ptn->x)+1)
		   continue;
		
		int dx = ptn->x - ptr->x;
		int dy = ptn->y - ptr->y;

		if(abs(dx) > abs(dy))
		{
			int yy = ptr->y + (point.x - ptr->x)* dy/dx;
			
			if(abs(yy - point.y)<2)
				return this;
		}
		else
		{
			int xx = ptr->x + (point.y - ptr->y)* dx/dy;
			
			if(abs(xx - point.x)<2)
				return this;
		}
	}//for(int i = 0; i< m_pixArray.GetSize(); i++)

for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * ptr = (Polygon_2D *)m_PlaneList.GetNext(pos);
		Polygon_2D * pts = ptr->isPointTouchStroke(point);
		if(pts != NULL)
		{
		   return pts;
		}
	}//for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
return NULL;
}
/////////////////////////////////////////////////////////////////////////////
Vector_2D * Polygon_2D::FindPointInStrokeOfPix(CPoint point)
{

	for(int  k =0; k < (int)m_pixArray.GetSize(); k++)
    {
     CPoint * ptr = (CPoint * )m_pixArray[k];
     if(abs(point.x - ptr->x) <= 2 && abs(point.y - ptr->y) <= 2)
		 return GetPointOfNumber(k);
    }//for ( UINT k =0; k < numPoints; k++)

	return NULL;
}
////////////////////////////////////////////////////////////////////////
int Polygon_2D::GetCount ()
{     

int counter = 0;


     for (POSITION psn = m_PlaneList.GetHeadPosition();psn != NULL;) 
       {
         Polygon_2D * pStroke = ( Polygon_2D * ) m_PlaneList.GetNext(psn);
         //pStroke->vsl = vsl; 
         counter++;
         counter += pStroke->GetCount(); 

       }

  return counter;
} 
//////////////////////////////////////////////////////////////////////
CRect Polygon_2D::GetRect(CRect rect, Vector_2D parentCenter,
         double parentAngle, double parentCoefX, double parentCoefY,
         Vector_2D scl)
{   

//_stprintf_s(str, _T("Polygon_2D::GetRect: %s"),strokeName);
//AfxMessageBox(str, MB_OK);   

    
	 Vector_2D myCenter = parentCenter;
    double myAngle = parentAngle + angle;
    double csn = cos( myAngle );
    double sns = sin( myAngle );
    double myCoefX = coefX * parentCoefX;
    double myCoefY = coefY * parentCoefY;

//    double myCoefX = coefX ;
//    double myCoefY = coefY ;
  
    CRect myRect(30000,30000,-30000,-30000);
    
    
 	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
     Vector_2D * pt = (Vector_2D *)m_PointList.GetNext(pos);

         int xx = (int) ((myCenter.x  +  myCoefX * pt->x * csn + myCoefY * pt->y * sns )/scl.x); 
         int yy = (int) (( myCenter.y  + myCoefX * pt->x * sns - myCoefY * pt->y * csn )/scl.y);



         if ( myRect.left > xx )
             myRect.left = xx;
//         else
           if ( myRect.right < xx )
              myRect.right = xx;

         if ( myRect.top > yy )
             myRect.top = yy;
//         else
           if ( myRect.bottom < yy )
              myRect.bottom = yy;

        }
        
//CClientDC dc(vsl);
//dc.Rectangle(myRect);  
//DrawSolidStroke ( &dc, scl, Priority(), myCenter, myAngle, myCoefX, myCoefY);
//ReleaseDC(&dc);
//_stprintf_s(str, _T("Stroke: %s"), strokeName);
//AfxMessageBox( str, MB_OK);

        rect.UnionRect( & rect, &myRect );

    	for (POSITION pos = m_PlaneList.GetHeadPosition();pos != NULL;)
	{    
	   
		Polygon_2D * pStroke = (Polygon_2D * )m_PlaneList.GetNext(pos);
         //pStroke->vsl = vsl;
		 Vector_2D pt = pStroke->turnPoint; 
         double xx = (parentCenter.x  +  myCoefX * pt.x * csn + myCoefY * pt.y * sns ); 
         double yy = (parentCenter.y  -myCoefX * pt.x * sns + myCoefY * pt.y * csn );

		Vector_2D m_Center;
		m_Center.x = xx;
		m_Center.y = yy;

		
        CRect newRect = pStroke->GetRect( rect,m_Center, myAngle, myCoefX, myCoefY,scl ); 

        rect.UnionRect( &rect, &newRect ); 
	} 

//CClientDC dc(vsl);
//dc.Rectangle(rect);
//ReleaseDC(&dc);
//_stprintf_s(str, _T("%s"),strokeName);
//AfxMessageBox(str, MB_OK);   

 return rect;
}


////////////////////////////////////////////////////////////////////
//CRect Polygon_2D::GetRect( CPoint parentCenter,
//         double parentAngle, double parentCoefX, double parentCoefY)
//{   
//    
//	 CPoint myCenter= parentCenter;
//    double myAngle = parentAngle + angle;
////    double csn = cos( myAngle );
//    double sns = sin( myAngle );
//    double myCoefX = coefX * parentCoefX;
////    double myCoefY = coefY * parentCoefY;
//  
//    
//    CRect myRect(30000,30000,-30000,-30000);
///**    
//	for (int i=0; i < m_PointList.GetCount(); i++)
//        {
//   
//         CPoint pt = GetPoint(i);
//         int xx = myCenter.x  + (int) ( myCoefX * pt.x * csn + myCoefY * pt.y * sns ); 
//         int yy = myCenter.y  + (int) ( -myCoefX * pt.x * sns + myCoefY * pt.y * csn );
//         if ( myRect.left > xx )
//             myRect.left = xx;
//         else
//           if ( myRect.right < xx )
//              myRect.right = xx;
//
//         if ( myRect.top > yy )
//             myRect.top = yy;
//         else
//           if ( myRect.bottom < yy )
//              myRect.bottom = yy;
//
//        }
//**/        
// return myRect;
//}


////////////////////////////////////////////////////////////////////
void Polygon_2D::LeftToRight()
{

angle = - angle; 
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
     Vector_2D * ptr = (Vector_2D *)m_PointList.GetNext(pos);
      ptr->x = - ptr->x; 
        }       
    	for (POSITION pos = m_PlaneList.GetHeadPosition();pos != NULL;)
	{    
	   
		Polygon_2D * pStroke = (Polygon_2D * )m_PlaneList.GetNext(pos);  
		pStroke->turnPoint.x = - pStroke->turnPoint.x;
        pStroke->LeftToRight();
     }

}              
/////////////////////////////////////////////////////////////////
void Polygon_2D::TopToBottom()
{
angle = - angle; 
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
     Vector_2D * ptr = (Vector_2D *)m_PointList.GetNext(pos);
         ptr->y = - ptr->y; 
        }       
    	for (POSITION pos = m_PlaneList.GetHeadPosition();pos != NULL;)
	{    
	   
		Polygon_2D * pStroke = (Polygon_2D * )m_PlaneList.GetNext(pos);  
		pStroke->turnPoint.y = - pStroke->turnPoint.y;
        pStroke->TopToBottom();
     }

}              
/////////////////////////////////////////////////////////////////
Polygon_2D  * Polygon_2D::GetStroke ( int idx, int * counter )
{     


   if ((* counter) == idx ) 
      {
        return this;
      }
         (* counter)++;     

     for (POSITION psn = m_PlaneList.GetHeadPosition();psn != NULL;) 
       {

         Polygon_2D * pStroke = ( Polygon_2D * ) m_PlaneList.GetNext(psn);
         Polygon_2D * ppStroke;
         if(( ppStroke = ( pStroke->GetStroke ( idx, counter)))!=NULL)
            return ppStroke;
            
       }

  return FALSE;
} 
//////////////////////////////////////////////////////////////////////
Vector_2D * Polygon_2D::GetFirstPoint(void)
{
	if(!m_PointList.GetCount())
		return NULL;
	return(Vector_2D *)m_PointList.GetHead();

}
///////////////////////////////////////////////////////////////////////////////////
Vector_2D * Polygon_2D::GetLastPoint(void)
{
	if(!m_PointList.GetCount())
		return NULL;
	return(Vector_2D *)m_PointList.GetTail();

}
///////////////////////////////////////////////////////////////////////////////////

BOOL Polygon_2D::bHasProperty(CString strProp)
{
	for(int i = 0; i < m_PropArray.GetSize(); i++)
		if(m_PropArray.GetAt(i) == strProp)
		{
//	 TestMessageBox(_T("PlaneArea::bHasProperty\n") + strPlaneArea() );

			return TRUE;
		}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////
BOOL Polygon_2D::IsNeighbour(Polygon_2D * pPl, Vector_2D  ** pV1, Vector_2D  ** pV2)
{
//	TestMessageBox(_T("Polygon_2D::IsNeighbour\n") + strPolygon_2D() + _T("\n") + pPl->strPolygon_2D());
   for(POSITION pos = m_PointList.GetHeadPosition(); pos !=NULL;)
   {  
	  	Vector_2D * ptr = (Vector_2D *) m_PointList.GetNext(pos);
		if(pPl->bHasPoint(ptr))
		   if(bHasSlab(ptr, pPl, pV2))
	    {

			* pV1 = ptr;
			return TRUE;
		}
	}//for(POSITION pos = m_PointList.GetHeadPosition(); pos !=NULL;)

	return FALSE;
}
////////////////////////////////////////////////////////////
BOOL Polygon_2D::bHasPoint(Vector_2D * pV)
{
	for(POSITION pos = m_PointList.GetHeadPosition(); pos !=NULL;)
    {  
	  	Vector_2D * ptr = (Vector_2D *) m_PointList.GetNext(pos);
		if(ptr == pV)
			return TRUE;
	}//for(POSITION pos = m_PointList.GetHeadPosition(); pos !=NULL;)
return FALSE;
}
///////////////////////////////////////////////////////////////
BOOL Polygon_2D::bHasSlab(Vector_2D  * pV, Polygon_2D * pPl, Vector_2D  ** pV2)
{
//	TestMessageBox(_T("Polygon_2D::bHasSlab\n") + pV->strVector() + _T("\n") + pPl->strPolygon_2D());
	Vector_2D  * pVn = GetNextPoint(pV,TRUE);
	if(pVn == pPl->GetNextPoint(pV,TRUE))
	{
		*pV2 = pVn;
		return TRUE;
	}
	if(pVn == pPl->GetPrevPoint(pV,TRUE))
	{
		*pV2 = pVn;
		return TRUE;
	}

	pVn = GetPrevPoint(pV,TRUE);
	if(pVn == pPl->GetNextPoint(pV,TRUE))
	{
		*pV2 = pVn;
		return TRUE;
	}
	if(pVn == pPl->GetPrevPoint(pV,TRUE))
	{
		*pV2 = pVn;
		return TRUE;
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////
void Polygon_2D::RemoveDeleted(void)
{

	int nCount = 0;
int dCount = 0;

	for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;nCount++)
	{
		POSITION posPrev = pos;
		Polygon_2D * pPl = (Polygon_2D *)m_PlaneList.GetNext(pos);
//if(!((nCount+1)%100))
//{
//if(bTestPrint)
//	ShowImage(pPl->SetImage());
//TestMessageBox(_T("PlaneObject::RemoveDeleted\n") + strFormat(_T("dCount = %d\nnPlanes = %d\n"
//	,dCount, m_PlaneList.GetCount())
//	+pPl->strPlaneArea());
//}
		if(pPl->bHasProperty(_T("ToRemove")))
		{
			dCount++; 
			m_PlaneList.RemoveAt(posPrev);
		}
	}
//bTestPrint = TRUE;
//TestMessageBox(_T("fin PlaneObject::RemoveDeleted\n") + strFormat(_T("dCount = %d\nnPlanes = %d\n"
//	,dCount, m_PlaneList.GetCount()));

}
/////////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::EquPoints(void)
{
	for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pPl = (Polygon_2D *)m_PlaneList.GetNext(pos);
		for(POSITION psn = pos; psn != NULL;)
		{
			Polygon_2D * pPn = (Polygon_2D *)m_PlaneList.GetNext(psn);
			pPl->EquPoints(pPn);
		}//for(POSITION psn = pos; psn != NULL;)
	}
}
/////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::EquPoints(Polygon_2D * pPn)
{
   for(POSITION pos = m_PointList.GetHeadPosition(); pos !=NULL;)
   {  
	  	Vector_2D * ptr = (Vector_2D *) m_PointList.GetNext(pos);
     for(POSITION psn = pPn->m_PointList.GetHeadPosition(); psn !=NULL;)
     {  
		 POSITION psnPrev = psn;
	  	Vector_2D * ptn = (Vector_2D *) pPn->m_PointList.GetNext(psn);
		if(ptn == ptr)
			continue;
		if(ptn->dist(* ptr)<GeomTreshold)
	    	pPn->m_PointList.SetAt(psnPrev, ptr);
	 }//for(POSITION psn = pPn->m_PointList.GetHeadPosition(); psn !=NULL;)
	}//for(POSITION pos = m_PointList.GetHeadPosition(); pos !=NULL;)

}
//////////////////////////////////////////////////////////////////////////////
BOOL bClearSlot( int * dtb, int m, int n, int w, int h)
{
//TestMessageBox(_T("bClearSlot\n") + strFormat(_T("m = %d  n = %d\ndata = %d"), m,n,dtb[m,n]));

	  if(dtb[n*h + m])
		return FALSE;

//	   int nCount = 0;
		for(int i = m-1;i <= m+1; i++)
           for(int j = n -1; j <= n+1; j++)
		   {
			   if(i == m && j == n)
				   continue;
			   if(i < 0 || i >= w)
				   continue;
	            if(j < 0 || j >= h)
					continue;
				if(dtb[j * w + i] != 0)
				{
		
							if (j < n+1)
					for(int k = m-1; k <= m+1; k++)
						if(dtb[(n+1) * w + k] != 0)
							return FALSE;
	
					if (i < m+1)
					for(int k = n-1; k <= n+1; k++)
						if(dtb[k * w + m+1] != 0)
							return FALSE;
					
		

				}//if(dtb[j * w + i] != 0)
		   }
		   return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL OneStepRand(int * dtb,  int width, int height)
{
	int		dir;														// Will Hold Current Direction
	BOOL done=TRUE;															// Set done To True
	for (int x=0; x<width; x+=2)										// Loop Through All The Rooms
	{
		for (int y=0; y<height; y+=2)									// On X And Y Axis
		{
			if (!dtb[x+width*y])							// If Current Texture Pixel (Room) Is Blank
				done=FALSE;												// We Have To Set done To False (Not Finished Yet)
		}
	}

	if (done)															// If done Is True Then There Were No Unvisited Rooms
	{
      return TRUE;
	}

	// Check To Make Sure We Are Not Trapped (Nowhere Else To Move)
	if ((mx>width-4  || dtb[mx+2+width*my]) && (mx<2 || dtb[mx-2+width*my]) &&
		(my>height-4 || dtb[mx+width*(my+2)]) && (my<2 || dtb[mx+width*(my-2)]))
	{
		do																// If We Are Trapped
		{
			mx=int(rand()%(width/2))*2;									// Pick A New Random X Position
			my=int(rand()%(height/2))*2;								// Pick A New Random Y Position
		}
		while (!dtb[mx+width*my]);						// Keep Picking A Random Position Until We Find
//	dtb[mx+width*my] = 1;
	}																	// One That Has Already Been Tagged (Safe Starting Point)

	dir=int(rand()%4);													// Pick A Random Direction

	if ((dir==0) && (mx<=(width-4)))									// If The Direction Is 0 (Right) And We Are Not At The Far Right
	{
		if (dtb[mx+2+width*my]==0)						// And If The Room To The Right Has Not Already Been Visited
		{
			dtb[mx+1+my*width] = 1;											// Update The Texture To Show Path Cut Out Between Rooms
			mx+=2;														// Move To The Right (Room To The Right)
		}
	}

	if ((dir==1) && (my<=(height-4)))									// If The Direction Is 1 (Down) And We Are Not At The Bottom
	{
		if (dtb[mx+width*(my+2)]==0)						// And If The Room Below Has Not Already Been Visited
		{
             dtb[mx+(my+1)*width] = 1;
	//		UpdateTex(mx,my+1);											// Update The Texture To Show Path Cut Out Between Rooms
			my+=2;														// Move Down (Room Below)
		}
	}

	if ((dir==2) && (mx>=2))											// If The Direction Is 2 (Left) And We Are Not At The Far Left
	{
		if (dtb[mx-2+width*my]==0)						// And If The Room To The Left Has Not Already Been Visited
		{
			dtb[mx-1+my*width] = 1;
//UpdateTex(mx-1,my);											// Update The Texture To Show Path Cut Out Between Rooms
			mx-=2;														// Move To The Left (Room To The Left)
		}
	}

	if ((dir==3) && (my>=2))											// If The Direction Is 3 (Up) And We Are Not At The Top
	{
		if (dtb[mx+width*(my-2)]==0)						// And If The Room Above Has Not Already Been Visited
		{
			dtb[mx+(my-1)*width] = 1;
//			UpdateTex(mx,my-1);											// Update The Texture To Show Path Cut Out Between Rooms
			my-=2;														// Move Up (Room Above)
		}
	}
	dtb[mx+my*width] = 1;

//TestMessageBox(_T("OneStepRand(\n") + strFormat(_T("mx = %d  my = %d\ndata = %d"), mx,my,dtb[mx+my*width]));
	
	//	UpdateTex(mx,my);													// Update Current Room
return FALSE;
}
///////////////////////////////////////////////////////////////////////////

void Polygon_2D::CreateRectStroke( Vector_2D v0, Vector_2D v1)
{
   Vector_2D * pV = new Vector_2D(v0.x,v0.y);
	AppendNewPoint(pV);
	

	pV = new Vector_2D(v1.x,v0.y);
	AppendNewPoint(pV);
	

	pV = new Vector_2D(v1.x,v1.y);
	AppendNewPoint(pV);
	

	pV = new Vector_2D(v0.x,v1.y);
	AppendNewPoint(pV);


}
///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
double Polygon_2D::CreateEquilateral(int nn, double a)
{
	double m = 0.5 * a /tan(PI/nn);
	double alf = 2*PI/nn;
	double ff = 0;

	Vector_2D * pV = new Vector_2D(-0.5 *a, -m);
	AppendNewPoint(pV);
	ff = 0;
   for(int i = 0; i < nn; i++)
   {
       pV = new Vector_2D(pV->x + a*cos(ff), pV->y + a *sin(ff));
	   AppendNewPoint(pV);
	   ff += alf;
   }//for(int i = 0; i < nn; i++)


	return m; 
}
//////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::Rotate(Vector_2D vc, double fi)
{
	for ( POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
	{
		 Vector_2D * pnt = (Vector_2D *)m_PointList.GetNext(pos);
		 double xx = pnt->x - vc.x;
		 double yy = pnt->y - vc.x;
		 * pnt = vc + Vector_2D( xx * cos(fi) + yy *  sin(fi), -xx*sin(fi) + yy * cos(fi));
	}
}
///////////////////////////////////////////////////////////////////////

void Polygon_2D::MakeDoublePolygon(Polygon_2D * source)
{      

    DeleteContents(TRUE);
	if(source == NULL)
		return;

//	if(!source->m_PointList.GetCount())
//		return;
	m_bClosed = source->m_bClosed;

	for(POSITION pos = source->m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * ptr = (Vector_2D *)source->m_PointList.GetNext(pos);
		Vector_2D * pV = new Vector_2D(* ptr);
	   AppendNewPoint(pV);
      }

turnPoint = source->turnPoint ;
//_tcscpy_s( strokeName, source->strokeName);
//strokeName[19] = source->strokeName [19];
bgColor = source->bgColor;
fgColor = source->fgColor;
angle = source->angle;
coefX = source->coefX;
coefY = source->coefY;
R2_style = source->R2_style;
fillStyle = source->fillStyle; 
prior = source->prior; 
penWidth = source->penWidth; 
penStyle = source->penStyle; 
typeNumber = source->typeNumber;
identNumber = source->identNumber;

m_curPoint = -1;
//_stprintf_s(str, _T("%s"), strokeName );
//AfxMessageBox( str, MB_OK );

//_stprintf_s(str, _T("%s"),strokeName); 
//AfxMessageBox(str,MB_OK);                     
    	for ( POSITION pos = source->m_PlaneList.GetHeadPosition();pos != NULL;)
	{   
        Polygon_2D * pStroke = (Polygon_2D * )source->m_PlaneList.GetNext(pos);
 	    Polygon_2D * newStroke = new Polygon_2D();
        newStroke->MakeDoublePolygon( pStroke );
        m_PlaneList.AddTail(newStroke);
	} 
 
    
    
}     
///////////////////////////////////////////////////////////////
void Polygon_2D::ReverseOrder(void)
{
/**
	Polygon_2D pl;
	pl.MakeDoubleStroke(this);
	pl.m_PointList.RemoveAll();
	for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * ptr = (Vector_2D *)m_PointList.GetNext(pos);
		pl.m_PointList.AddHead(ptr);
    }
	m_PointList.RemoveAll();
	MakeDoubleStroke(&pl);
	pl.DeleteContents(TRUE);
	/**/
	int tCount = 0;
	POSITION psn = m_PointList.GetTailPosition();
	for (POSITION pos = m_PointList.GetHeadPosition(); tCount < m_PointList.GetCount() / 2; tCount++)
	{
		Vector_2D * pV = (Vector_2D *)m_PointList.GetNext(pos);
		Vector_2D * pU = (Vector_2D *)m_PointList.GetPrev(psn);
		Vector_2D vv(*pV);
		*pV = *pU;
		*pU = vv;
	}
}
/////////////////////////////////////////////////////////////////

double Polygon_2D::Square(void)
{
	if (m_PointList.GetCount() < 3)
		return 0;
	else
	if (m_PointList.GetCount() == 3)
	{
		POSITION pos = m_PointList.GetHeadPosition();
		Vector_2D * pV = (Vector_2D *)m_PointList.GetNext(pos);
		Vector_2D * pU = (Vector_2D *)m_PointList.GetNext(pos);
		Vector_2D v = *pU - *pV;
		pU = (Vector_2D *)m_PointList.GetNext(pos);
		Vector_2D u = *pU - *pV;
		return fabs(v^u);
	}
	else
	{
		double sq = 0;
		for (POSITION pos = m_PointList.GetHeadPosition();pos != NULL;)
		{
			Vector_2D * pV = (Vector_2D *)m_PointList.GetNext(pos);
			pV = pV;
		}
		return sq;
	}
}
///////////////////////////////////////////////////////////////////

BOOL Polygon_2D::AppheadNewPoint(Vector_2D * pV)
{

	POSITION pos = m_PointList.Find(pV);
	if (pos != NULL)
		return FALSE;
	Vector_2D * pT = GetPointOfVector(*pV);
	BOOL bRslt = pT == NULL;
	if (bRslt)
	{
		m_PointList.AddHead(pV);
//		pV->CheckAppendPlaneArea(this);
	}

	return bRslt;
}
//////////////////////////////////////////////////////

BOOL isCrossLines(int firstBgn_x, int firstBgn_y,
	int firstFin_x, int firstFin_y,
	int secondBgn_x, int secondBgn_y,
	int secondFin_x, int secondFin_y,
	int * x, int * y)

{

	double A1 = (double)(firstFin_y - firstBgn_y);
	double B1 = -(double)(firstFin_x - firstBgn_x);
	double C1 = -(double)firstBgn_y * firstFin_x
		+ (double)firstBgn_x * firstFin_y;


	double A2 = (double)(secondFin_y - secondBgn_y);
	double B2 = -(double)(secondFin_x - secondBgn_x);
	double C2 = -(double)secondBgn_y * secondFin_x
		+ (double)secondBgn_x * secondFin_y;


	double det = A1 * B2 - A2 * B1;

	if (det == 0.0)
		return FALSE;

	double xx = (C1 * B2 - C2 * B1) / det;

	double yy = (-C1 * A2 + C2 * A1) / det;

	(*x) = (int)xx;
	(*y) = (int)yy;

	det = det;

	if ((*x) < min(firstBgn_x, firstFin_x))
		return FALSE;


	if ((*x) > max(firstBgn_x, firstFin_x))
		return FALSE;


	if ((*y) < min(firstBgn_y, firstFin_y))
		return FALSE;


	if ((*y) > max(firstBgn_y, firstFin_y))
		return FALSE;


	if ((*x) < min(secondBgn_x, secondFin_x))
		return FALSE;


	if ((*x) > max(secondBgn_x, secondFin_x))
		return FALSE;


	if ((*y) < min(secondBgn_y, secondFin_y))
		return FALSE;


	if ((*y) > max(secondBgn_y, secondFin_y))
		return FALSE;


	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//CString Polygon_2D::strPlaneArea(void)
//{
//	CString wString = _T("Polygon_2D::strPlaneArea:\n");
//	wString += strFormat(_T("numPoints = %d numChilds = %d\n"), m_PointList.GetCount(), m_PlaneList.GetCount());
//	for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
//	{
//		Vector_2D * pnt = (Vector_2D *)m_PointList.GetNext(pos);
//		wString += pnt->strVector() + _T("\n");
//	}
//	return wString;
//}
/////////////////////////////////////////////////////////////////
static POINT  tempArray[1000];
BOOL Polygon_2D::DrawSolidStroke(CDC * pDC, Vector_2D scl, int prrt, Vector_2D parentCenter,
	double parentAngle, double parentCoefX, double parentCoefY)
{

    Vector_2D myCenter = parentCenter;
	double myAngle = parentAngle + angle;
	double csn = cos(myAngle);
	double sns = sin(myAngle);
	double myCoefX = coefX * parentCoefX;
	double myCoefY = coefY * parentCoefY;

	memset(tempArray, 0, 999 * sizeof(POINT));

	//if (m_polyName == _T("BLUE_INIT"))
	//{
	//	Polygon_2D * pTrn = NULL;
	//	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	//	{
	//		Polygon_2D * ptr = (Polygon_2D *)m_PlaneList.GetNext(pos);
	//		if (ptr->m_polyName == _T("track"))
	//		{
	//			pTrn = ptr;
	//			break;
	//		}
	//	}
	//	if (pTrn == NULL)
	//	{
	//		pTrn = new Polygon_2D;
	//		pTrn->m_polyName = _T("track");
	//		Vector_2D * pV = new Vector_2D(0);
	//		m_PlaneList.AddTail(pTrn);
	//		pTrn->AppendNewPoint(pV);
	//		pTrn->fgColor = RGB(255, 255, 0);
	//		pTrn->penWidth = 7;
	//	}
	//	else
	//	{
	//		CRect rct;
	//		m_pView->GetClientRect(&rct);
	//		Vector_2D vc = Vector_2D((rct.left + rct.right) / 2, (rct.top + rct.bottom) / 2);
	////		Vector_2D * pU = (Vector_2D *)pTrn->m_PointList.GetTail();
	//		Vector_2D * pV = new Vector_2D(myCenter );
	//		pTrn->AppendNewPoint(pV);

	////		TestMessageBox(_T(""))
	//	}
	//}

	m_pixArray.RemoveAll();
	if (m_PointList.GetCount()>1)
	{
		int i = 0;
		for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; i++)
		{
			Vector_2D * pPa = (Vector_2D *)m_PointList.GetNext(pos);
			tempArray[i].x = (int)((myCenter.x + myCoefX * pPa->x * csn + myCoefY * pPa->y * sns)*scl.x);
			tempArray[i].y = (int)((myCenter.y - myCoefX * pPa->x * sns + myCoefY * pPa->y * csn)*scl.y);
			if (i > m_pixArray.GetUpperBound())
			{
				CPoint * pti = new CPoint(tempArray[i].x, tempArray[i].y);
				m_pixArray.Add(pti);
			}
		}////for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; i++ )

		int oldMode = pDC->SetROP2(R2_style);
  		if (fillStyle == HS_TRANSPARENT )
			{
				CPen pen(penStyle, penWidth, fgColor);
				CPen * pPen = pDC->SelectObject(&pen);
				if (penWidth)
					pDC->Polyline(tempArray, m_pixArray.GetSize());
				pDC->SelectObject(pPen);
			}
			else
			{
				CBrush brushStroke;

				if (m_pBitmap != NULL)
					brushStroke.CreatePatternBrush(m_pBitmap);
				else
					brushStroke.CreateSolidBrush(bgColor);

				CBrush* pOldBrush = pDC->SelectObject(&brushStroke);
				CPen pen(penStyle, penWidth, fgColor);
				CPen * pOldPen = pDC->SelectObject(&pen);

				pDC->Polygon(tempArray, m_PointList.GetSize());
				pDC->SelectObject(pOldPen);

				pDC->SelectObject(pOldBrush);
				brushStroke.DeleteObject();

				if (fillStyle != HS_SOLID)
				{
					int sMode = pDC->SetBkMode(TRANSPARENT);
					brushStroke.CreateHatchBrush(fillStyle, fgColor);
					pOldBrush = pDC->SelectObject(&brushStroke);

					pDC->Polygon(tempArray, (int)m_pixArray.GetSize());

					pDC->SelectObject(pOldBrush);
					brushStroke.DeleteObject();
					pDC->SetBkMode(sMode);
				}


			} //if ( fillStyle != HS_TRANSPARENT)
	
//		pDC->Ellipse((int)myCenter.x - 5, (int)myCenter.y - 5, (int)myCenter.x + 5, (int)myCenter.y + 5);

		BOOL bSel = FALSE;
		pDC->SetROP2(oldMode);
	}//if(m_PointList.GetCount())
	 
	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pl = (Polygon_2D *)m_PlaneList.GetNext(pos);
		pl->DrawSolidStroke(pDC, scl, prrt, myCenter, myAngle, myCoefX, myCoefY);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////
void Polygon_2D::CreateRandomSegment(CRect rect)
{
	int width = rect.Width()*3/4;
	int height = rect.Height()*3/4;


//	Polygon_2D * pNew = new Polygon_2D;
	Vector_2D * pV = new Vector_2D(rand()%width, rand() % height);
	AppendNewPoint(pV);
	pV = new Vector_2D(rand() % width, rand() % height);
	AppendNewPoint(pV);
	
//	TestMessageBox(_T("1 Polygon_2D::CreateRandomSegment\n") + strPlaneArea());


		bgColor = RGB(0, 255, 255);

     
		double xMin = INFIN_MY;
		double yMin = INFIN_MY;
		double xMax = -INFIN_MY;
		double yMax = -INFIN_MY;

		for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
		{
			Vector_2D * pV = (Vector_2D *)m_PointList.GetNext(pos);
			xMin = min(xMin, pV->x);
			yMin = min(yMin, pV->y);

			xMax = max(xMax, pV->x);
			yMax = max(yMax, pV->y);

		}//for(int  i = 0; i < m_pVesselGL->hullFore.GetSize(); i++)

		Vector_2D tnp((xMin + xMax)*0.5, (yMin + yMax)*0.5);
		Sdvig(-tnp.x, -tnp.y);
	R2_style = R2_COPYPEN;
	fillStyle = HS_SOLID;
	turnPoint = (Vector_2D((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2) - tnp)*0.5;
	
	angle = PI*(rand() % 100) / 50.;
	rot = PI*(50 - (rand() % 100)) / 2000.;
	penWidth = 3;
//	TestMessageBox(_T("Polygon_2D::CreateRandomSegment fin\n") + strPlaneArea());


	}
////////////////////////////////////////////////////////////////////////////////
int Polygon_2D::MergePlaneAreas(void)
{
	BOOL bRslt = FALSE;
	int nCount = (int)m_PlaneList.GetCount();
	int iCount = 0;
	BOOL bFind = TRUE;
	while (bFind)
	{
		bFind = FALSE;
		for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
		{
			POSITION posPrev = pos;
			Polygon_2D * pPl = (Polygon_2D *)m_PlaneList.GetNext(pos);
			if (pPl->bHasProperty(_T("ToRemove")))
				continue;
			for (POSITION psn = pos; psn != NULL;)
			{
				POSITION psnPrev = psn;
				Polygon_2D * pPn = (Polygon_2D *)m_PlaneList.GetNext(psn);
				if (pPn->bHasProperty(_T("ToRemove")))
					continue;
				Vector_2D * pV1 = NULL;
				Vector_2D * pV2 = NULL;
				if (pPl->IsNeighbour(pPn, &pV1, &pV2))
					if (pPl->MergePlaneArea(pPn, pV1, pV2))
					{
						bFind = TRUE;
						iCount++;
						//	  break;
					}//if(pPl->MergePlaneArea(pPn, pV1, pV2))
					 //		}//if(pPl->IsNeighbour(pPn, &pV1, &pV2))
			}//for(POSITION psn = pos; psn != NULL;)

		}//for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)

	}//while(bFind)

	 
	if (!m_PointList.GetCount() && m_PlaneList.GetCount())
	{
		Polygon_2D * pPn = (Polygon_2D *)m_PlaneList.GetHead();
		for (POSITION pos = pPn->m_PointList.GetHeadPosition(); pos != NULL;)
		{
			Vector_2D * pV = (Vector_2D *)pPn->m_PointList.GetNext(pos);
			m_PointList.AddTail(pV);
		}//for(POSITION pos = pPn->m_PointList.GetHeadPosition(); pos != NULL;)
		pPn->m_PointList.RemoveAll();
		m_PlaneList.RemoveHead();
		delete pPn;
	}
	/**/


	if (iCount)
		RemoveDeleted();
	//TestMessageBox("Polygon_2D::MergePlaneAreas fin\n" + strPlaneArea());

	return iCount;
}
////////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::RemoveMidPoints(void)
{
	BOOL bFind = TRUE;
	int nCount = (int)m_PointList.GetCount();
	double prec = GeomTreshold;
	while (bFind && m_PointList.GetCount()>2)
	{
		bFind = FALSE;
		for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
		{
			Vector_2D  * pV = (Vector_2D  *)m_PointList.GetNext(pos);
			Vector_2D  * pV1 = GetNextPoint(pV, TRUE);
			Vector_2D  * pV2 = GetNextPoint(pV1, TRUE);

			if (pV1->dist(*pV) < prec)
				bFind = TRUE;
			if (!bFind)
				if (pV2->dist(*pV1) < prec)
					bFind = TRUE;


			if (!bFind)
			{
				Vector_2D u = Normalize(*pV1 - *pV);
				Vector_2D u1 = Normalize(*pV2 - *pV);
				bFind = fabs(u^u1) < GeomTreshold;
			}
			if (bFind)
			{
				RemovePoint(pV1);
				break;
			}//if(bFind)

			 
		}//for ( POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
	}// while (bFind

	 
	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pPt = (Polygon_2D *)m_PlaneList.GetNext(pos);
		pPt->RemoveMidPoints();
	}//for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)

}
///////////////////////////////////////////////////////////////////////////////////
BOOL Polygon_2D::MergePlaneArea(Polygon_2D * ppl, Vector_2D * pV1, Vector_2D * pV2)
{
	BOOL bNext1 = TRUE;
	Vector_2D * pVV = GetPrevPoint(pV1, TRUE);
	if (pVV != pV2)
		bNext1 = FALSE;
	POSITION pos = m_PointList.Find(bNext1 ? pV2 : pV1);
	Vector_2D * pVa = bNext1 ? pV2 : pV1;

	BOOL bNext2 = TRUE;
	Vector_2D  * pVu = ppl->GetNextPoint(pV1, TRUE);
	if (pVu != pV2)
		bNext2 = FALSE;

	bNext2 = bNext2 || bNext1;

	Vector_2D  * pVn = pVa == pV1 ? pV2 : pV1;
	bNext2 = ppl->GetNextPoint(pVn, TRUE) != pVa;
	do
	{
		pVn = bNext2 ? ppl->GetNextPoint(pVn, TRUE) : ppl->GetPrevPoint(pVn, TRUE);
		if (GetNumberOfPoint(pVn) < 0)  m_PointList.InsertAfter(pos, pVn);
	} while (pVn != pVa);

	ppl->m_PropArray.Add(_T("ToRemove"));
	



	return TRUE;
}
////////////////////////////////////////////////////////////////////////
void Polygon_2D::UpdatePoints(Vector_2D parentCenter, double parentAngle, double parentCoefX, double parentCoefY)
{

	Vector_2D myCenter = parentCenter + turnPoint;
	double myAngle = parentAngle + angle;
	double csn = cos(myAngle);
	double sns = sin(myAngle);
	double myCoefX = coefX * parentCoefX;
	double myCoefY = coefY * parentCoefY;

	angle = 0;
	coefX = coefY = 1;
	turnPoint = Vector_2D(0);

	for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * pPa = (Vector_2D *)m_PointList.GetNext(pos);
		double xx = myCenter.x + myCoefX * pPa->x * csn + myCoefY * pPa->y * sns;
		double yy = myCenter.y - myCoefX * pPa->x * sns + myCoefY * pPa->y * csn;
		*pPa = Vector_2D(xx, yy);

	}////for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; i++ )


	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pl = (Polygon_2D *)m_PlaneList.GetNext(pos);
		Vector_2D tCenter(myCenter + Vector_2D(myCoefX * pl->turnPoint.x * csn + myCoefY * pl->turnPoint.y * sns,
			-myCoefX * pl->turnPoint.x * sns + myCoefY * pl->turnPoint.y * csn));
		pl->UpdatePoints(tCenter, myAngle, myCoefX, myCoefY);
	}
}
/////////////////////////////////////////////////////////////////////////////
double dist2(Vector_2D * p0, Vector_2D * p1)
{
	double xx = p1->x - p0->x;
	double yy = p1->y - p0->y;
	return sqrt(xx*xx + yy*yy);
}
/////////////////////////////////////////////////////////////////////////////////
BOOL isCrossLines(double firstBgn_x, double firstBgn_y,
	double firstFin_x, double firstFin_y,
	double secondBgn_x, double secondBgn_y,
	double secondFin_x, double secondFin_y,
	double * x, double * y)
{

	double A1 = (double)(firstFin_y - firstBgn_y);
	double B1 = -(double)(firstFin_x - firstBgn_x);
	double C1 = -(double)firstBgn_y * firstFin_x
		+ (double)firstBgn_x * firstFin_y;


	double A2 = (double)(secondFin_y - secondBgn_y);
	double B2 = -(double)(secondFin_x - secondBgn_x);
	double C2 = -(double)secondBgn_y * secondFin_x
		+ (double)secondBgn_x * secondFin_y;

	double det = A1 * B2 - A2 * B1;

	if (fabs(det) < GeomTreshold)
		return FALSE;

	(*x) = (C1 * B2 - C2 * B1) / det;
	(*y) = (-C1 * A2 + C2 * A1) / det;

	if (fabs((*x) - firstBgn_x)> GeomTreshold)
	{
		if ((*x) < min(firstBgn_x, firstFin_x))
			return FALSE;
		if ((*x) > max(firstBgn_x, firstFin_x))
			return FALSE;
	}



	if (fabs((*y) - firstBgn_y)> GeomTreshold)
	{
		if ((*y) < min(firstBgn_y, firstFin_y))
			return FALSE;
		if ((*y) > max(firstBgn_y, firstFin_y))
			return FALSE;
	}



	if (fabs((*x) - secondBgn_x)> GeomTreshold)
	{
		if ((*x) < min(secondBgn_x, secondFin_x))
			return FALSE;

		if ((*x) > max(secondBgn_x, secondFin_x))
			return FALSE;
	}


	if (fabs((*y) - secondBgn_y)> GeomTreshold)
	{
		if ((*y) < min(secondBgn_y, secondFin_y))
			return FALSE;
		if ((*y) > max(secondBgn_y, secondFin_y))
			return FALSE;
	}

	//	CString wString;
	//	wString.Format("x1 = %7.2f y1 = %7.2f\nx2 = %7.2f y2 = %7.2f\nx3 = %7.2f y3 = %7.2f\nx4 = %7.2f y4 = %7.2f\nxx = %7.2f yy = %7.2f\n",
	//firstBgn_x, firstBgn_y,
	//firstFin_x, firstFin_y,
	//secondBgn_x, secondBgn_y,
	//secondFin_x, secondFin_y,
	// *x, *y);
	//TestMessageBox("isCrossLines\n" + wString);

	return TRUE;
}

//////////////////////////////////////////////////////////////
Polygon_2D * m_pDr = NULL;
Polygon_2D * m_pDs = NULL;

Vector_2D vDr0(100, -100);
Vector_2D vDs0(-100, 100);
Vector_2D vDr(vDr0);
Vector_2D vDs(vDs0);
Polygon_2D * Polygon_2D::IntersectPolygonDemo(Polygon_2D * pPln)
{
	m_bClosed = TRUE;
	pPln->m_bClosed = TRUE;

	RemoveInLinePoints();
	pPln->RemoveInLinePoints();

	RemoveMidPoints();
	pPln->RemoveMidPoints();

	Polygon_2D * pA = new Polygon_2D;
	pA->fgColor = RGB(255, 0, 255);

	m_polygonList.AddTail(pA);

	IntersectSegmentList(pPln, pA);

	Polygon_2D * pC = new Polygon_2D;
	pC->MakeDoublePolygon(pA);
	pC->m_bClosed = FALSE;
	pC->SetClosed(FALSE);
	pC->fillStyle = HS_TRANSPARENT;
	pC->fgColor = RGB(255, 0, 255);

	m_polygonList.AddTail(pC);

	int nStep = 30;
	vDs = vDs0;
	pC->vel = vDs / nStep;

	m_pDs = pC;

	Polygon_2D * pB = new Polygon_2D;
	pPln->IntersectSegmentList(this, pB);
	pB->m_bClosed = FALSE;
	pB->SetClosed(FALSE);
	pB->SetFgColor(RGB(0, 255, 255));
	pB->fillStyle = HS_TRANSPARENT;

	m_polygonList.AddTail(pB);

	vDr = vDr0;
	pB->vel = vDr / nStep;
	m_pDr = pB;

	pPln->IntersectSegmentList(this, pA);
	pA->MergeChains(pA);

	if (pA != NULL)
		if (!pA->m_PointList.GetCount())
			if (pA->m_PlaneList.GetCount())
			{
				Polygon_2D *p = (Polygon_2D *)pA->m_PlaneList.GetHead();
				for (POSITION pos = p->m_PointList.GetHeadPosition(); pos != NULL;)
				{
					Vector_2D * pV = (Vector_2D *)p->m_PointList.GetNext(pos);
					pA->AppendNewPoint(pV);
				}//for(POSITION pos = p->m_PointList.GetHeadPosition(); pos != NULL;)
				pA->m_PlaneList.RemoveHead();
				p->DeleteContents(FALSE);
				delete p;
			}

	return pA;

}
////////////////////////////////////////////////////////////////////////////
void Polygon_2D::RemoveInLinePoints(void)
{
	BOOL bFind = TRUE;
	if (m_PointList.GetCount() <3)
		return;
	while (bFind)
	{
		bFind = FALSE;
		for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
		{
			Vector_2D * p = (Vector_2D *)m_PointList.GetNext(pos);
			Vector_2D * p1 = GetNextPoint(p, TRUE);
			Vector_2D * p2 = GetNextPoint(p1, TRUE);
			Vector_2D n1 = Normalize(Vector_2D(p1->x - p->x, p1->y - p->y));
			Vector_2D n2 = Normalize(Vector_2D(p2->x - p->x, p2->y - p->y));
			if (fabs(n1^n2) < GeomTreshold)
			{
				//		 TestMessageBox("void Polygon_2D::RemoveInLinePoints\n"
				//			    + strFormat("i1 = %d\n",i1)
				//				+ strPlaneArea());
				POSITION pos = m_PointList.Find(p1);
				m_PointList.RemoveAt(pos);
				bFind = TRUE;
				break;
			}
		}//for (int i = 0; i< m_PointList.GetCount(); i++)
	}
}
/////////////////////////////////////////////////////////////////////
void Polygon_2D::IntersectSegmentList(Polygon_2D * pT, Polygon_2D * pRslt)
{
 //Polygon_2D * pT - target polygon
 //Polygon_2D * pRslt - result set of polygons consisted of points and segments of this polygon inside the target
 if (pT == NULL)
  return;
 Polygon_2D * pStroke = NULL;          //polygon of points and lines of pT inside this  polygon 
 BOOL bChain = FALSE;                  //flag of is current chain polygon continue
 for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
 {
  Vector_2D * pti = (Vector_2D *)m_PointList.GetNext(pos);//current point
  Vector_2D * ptiN = GetNextPoint(pti, TRUE);             //next point
  if (dist2(pti, ptiN)< GeomTreshold)           //if current == next skip
   continue;
  Polygon_2D sCross;                      //polygon of intersection segment(pti, ptiN) with target polygon pT
  pT->IsCrossStroke(pti, ptiN, &sCross);  //if segment intersect target
  if (pT->IsPointInside(pti))             //if current point inside target
  {
   if (!bChain)                           //if chain broken
    if (sCross.m_PointList.GetCount() || pT->IsPointInside(ptiN))//if segment intersect target 
    {
     pStroke = new Polygon_2D;            //start new polygon chain
     pStroke->fillStyle = HS_TRANSPARENT;
     pStroke->penWidth = 3;
     pStroke->fgColor = pRslt->fgColor;
     pStroke->m_bClosed = FALSE;
     pRslt->m_PlaneList.AddTail(pStroke); //new polygon chain appended to result polygon
     bChain = TRUE;
    }//if(!bChain)
   if (bChain)
    pStroke->AppendNewPoint(pti); //current point append to current polygon chain
  }//if(pT->IsPointInside(pti))
  else
   bChain = FALSE;

  while (sCross.m_PointList.GetCount())//check all intersection points
  {
   Vector_2D * pN = (Vector_2D *)sCross.m_PointList.GetHead();
   sCross.m_PointList.RemoveHead();
   if (pN->dist(*pti) < GeomTreshold)
    continue;
   if (pStroke != NULL)
    if (pStroke->m_PointList.GetCount())
    {
     Vector_2D * pvl = (Vector_2D *)pStroke->m_PointList.GetTail();
     Vector_2D vd = (*pvl + *pN)*0.5;
     if (bChain)
      bChain = pT->IsPointInside(&vd);
    }

   if (!bChain)   //if chain broken
   {
    pStroke = new Polygon_2D;               //start new polygon chain
    pStroke->fillStyle = HS_TRANSPARENT;
    pStroke->penWidth = 3;
    pStroke->fgColor = pRslt->fgColor;
    pStroke->m_bClosed = FALSE;
    pRslt->m_PlaneList.AddTail(pStroke);    //new polygon chain appended to result polygon
    bChain = TRUE;
   }//if(!bChain)

   pStroke->AppendNewPoint(pN);         //intersection point append to current polygon chain
  }//while(sCross.m_PointList.GetCount())

  if (pT->IsPointInside(ptiN))              //if next point inside target
  {
   if (!bChain)
   {
    pStroke = new Polygon_2D;                 //start new polygon chain    
    pStroke->fillStyle = HS_TRANSPARENT;
    pStroke->penWidth = 3;
    pStroke->fgColor = pRslt->fgColor;
    pStroke->m_bClosed = FALSE;

    pRslt->m_PlaneList.AddTail(pStroke);       //new polygon chain appended to result polygon
    bChain = TRUE;
   }
   pStroke->AppendNewPoint(ptiN);              //next point append to current polygon chain
  }
  else
   bChain = FALSE;
 }//for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
}
///////////////////////////////////////////////////////////////////////
void Polygon_2D::SetClosed(BOOL bMode)
{
	m_bClosed = bMode;
	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pl = (Polygon_2D *)m_PlaneList.GetNext(pos);
		pl->SetClosed(bMode);
	}
}
////////////////////////////////////////////////////////////////////////////
void Polygon_2D::MergeChains(Polygon_2D * pPl)
{
	//	RemoveEquChain(pPl);


	int nCount = 0;
	while (pPl->m_PlaneList.GetCount())
	{
		int npl = pPl->m_PlaneList.GetCount();
		for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
		{
			Polygon_2D * pA = (Polygon_2D *)m_PlaneList.GetNext(pos);
			pA->fillStyle = HS_SOLID;
			pA->bgColor = RGB(255, 255, 0);
			if (pA->AppendChain(pPl))
			{
				pA->AppendChain(this);
				break;
			}

		}//for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)

		if (npl == pPl->m_PlaneList.GetCount())
			break;

		nCount++;
		if (nCount >= 1000)
			break;

	}
//	if (nCount >= 1000)
//		TestMessageBox("Polygon_2D::MergeChains\nnCount\n" + strFormat("nCount = %d\n", nCount));
	//			MessageBox(NULL, "Polygon_2D::MergeChains\nnCount\n" + strFormat("nCount = %d\n", nCount), "", MB_OK);
}
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL IsInLineSegments(Vector_2D * p00, Vector_2D * p01, Vector_2D * p10, Vector_2D * p11, Vector_2D * pV, Vector_2D * pU)
{
	if ((p00->dist(*p10) < GeomTreshold && p01->dist(*p11) < GeomTreshold) ||
		(p00->dist(*p11) < GeomTreshold && p01->dist(*p10) < GeomTreshold))
	{
		*pV = *p00;
		*pU = *p01;
		return TRUE;
	}



	Vector_2D e = Normalize((*p01) - (*p00));
	Vector_2D en = Normalize((*p11) - (*p10));

	//TestMessageBox("start IsInLineSegments fin\np00 = " + p00->strVector_2D()
	//	+ "\np01 = " + p01->strVector_2D()
	//	+ "\np10 = " + p10->strVector_2D()
	//	+ "\np11 = " + p11->strVector_2D()
	//	+ "\npV = " + pV->strVector_2D()
	//	+ "\npU = " + pU->strVector_2D()
	//	);


	if (fabs(e^en) > GeomTreshold)
		return FALSE;


	en = Normalize((*p01) - (*p10));
	if (!en < GeomTreshold)
		en = Normalize((*p01) - (*p11));
	if (fabs(e^en) > GeomTreshold)
		return FALSE;

	double x00 = 0;
	double x01 = ((*p01) - (*p00))*e;
	double xa = ((*p10) - (*p00))*e;
	double xb = ((*p11) - (*p00))*e;
	double x10 = min(xa, xb);
	double x11 = max(xa, xb);

	if (x11 < x00)
		return FALSE;
	if (x01 < x10)
		return FALSE;
	double x0 = max(x00, x10);
	double x1 = min(x01, x11);


	*pV = *p00 + x0*e;
	*pU = *p00 + x1*e;

	if (fabs(x1 - x0)<GeomTreshold)
		return FALSE;

	//if(fabs(x1-x0)<GeomTreshold)
	//TestMessageBox("start IsInLineSegments fin\np00 = " + p00->strVector_2D()
	//	+ "\np01 = " + p01->strVector_2D()
	//	+ "\np10 = " + p10->strVector_2D()
	//	+ "\np11 = " + p11->strVector_2D()
	//	+ "\npV = " + pV->strVector_2D()
	//	+ "\npU = " + pU->strVector_2D()
	//	);


	//TestMessageBox("fin IsInLineSegments fin\np00 = " + p00->strVector_2D()
	//	+ "\np01 = " + p01->strVector_2D()
	//	+ "\np10 = " + p10->strVector_2D()
	//	+ "\np11 = " + p11->strVector_2D()
	//	+ "\npV = " + pV->strVector_2D()
	//	+ "\npU = " + pU->strVector_2D()
	//	);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
BOOL IsCrossSegments(Vector_2D * p00, Vector_2D * p01, Vector_2D * p10, Vector_2D * p11, Vector_2D * pV)
{
	double xx;
	double yy;
	int nRes = (isCrossLines(p00->x, p00->y, p01->x, p01->y,
		p10->x, p10->y, p11->x, p11->y, &xx, &yy));
	pV->x = xx;
	pV->y = yy;
	return nRes;
	;
}
///////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::IsCrossStroke(Vector_2D * p0, Vector_2D * p1, Polygon_2D * pS)
{
	Polygon_2D sCross;
	int iCount = 0;
	for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; iCount++)
	{
		Vector_2D * pt = (Vector_2D *)m_PointList.GetNext(pos);
		Vector_2D * ptN = GetNextPoint(pt, TRUE);

		if (pt->dist(*ptN) < GeomTreshold)
			continue;
		Vector_2D vv;
		Vector_2D uu;

		if (IsInLineSegments(p0, p1, pt, ptN, &vv, &uu))
			continue;
		else
			if (IsCrossSegments(p0, p1, pt, ptN, &vv))
			{
				Vector_2D * pN = new Vector_2D;
				*pN = vv;
				sCross.AppendNewPoint(pN);
				pN->m_pBgn = pt;
				pN->m_pFin = ptN;
			}//if(IsCrossSegments(p0, p1, ptiPrev, pti, &vv))


	}//for(POSITION pos = m_PointList.GetHeadPosition(); pos != NULL; )

	 //	if(IsPointInside(p0))
	 //		sCross.AddHeadPoint(p0);

	 //	if(IsPointInside(p1))
	 //		sCross.AppendNewPoint(p1);

	double x0 = p1->x - p0->x;
	double y0 = p1->y - p0->y;
	for (int i = 0; i < sCross.m_PointList.GetCount(); i++)
	{
		Vector_2D * pti = (Vector_2D *)sCross.GetPointOfNumber(i);
		for (int j = i; j < sCross.m_PointList.GetCount(); j++)
		{
			Vector_2D * ptj = (Vector_2D *)sCross.GetPointOfNumber(j);
			double xi = pti->x - p0->x;
			double yi = pti->y - p0->y;
			double xj = ptj->x - p0->x;
			double yj = ptj->y - p0->y;
			//		if(x0*xi + y0*yi < x0*xj + y0*yj)
			if (x0*xi + y0*yi > x0*xj + y0*yj)
			{
				Vector_2D vv = *ptj;
				Vector_2D * pb = ptj->m_pBgn;
				Vector_2D * pf = ptj->m_pFin;

				*ptj = *pti;
				ptj->m_pBgn = pti->m_pBgn;
				ptj->m_pFin = pti->m_pFin;

				*pti = vv;
				pti->m_pBgn = pb;
				pti->m_pFin = pf;

			}//if(x0*xi + y0*yi > x0*xj + y0*yj)
		}//for(int j = i; j <  .GetSize(); j++)
	}//for(int i = 0; i <  .GetSize(); i++)

	BOOL bFind = TRUE;
	if (sCross.m_PointList.GetCount()>1)
		while (bFind)
		{
			bFind = FALSE;
			for (POSITION pos = sCross.m_PointList.GetHeadPosition(); pos != NULL; )
			{
				POSITION posPrev = pos;
				Vector_2D * pt = (Vector_2D *)sCross.m_PointList.GetNext(pos);
				Vector_2D * ptN = sCross.GetNextPoint(pt, TRUE);
				if (pt->dist(*ptN) < GeomTreshold)
				{
					bFind = TRUE;
					sCross.m_PointList.RemoveAt(posPrev);
					break;
				}
			}//for(POSITION pos = sCross.m_PointList.GetHeadPosition(); pos != NULL; )
		}

	for (POSITION pos = sCross.m_PointList.GetHeadPosition(); pos != NULL; )
	{
		Vector_2D * pt = (Vector_2D *)sCross.m_PointList.GetNext(pos);
		pS->AppendNewPoint(pt);

		//				 if(bTestPrint)
		//					 ShowImage(SetImage(&sCross,NULL));
		//				 TestMessageBox("Polygon_2D::IsCrossStroke\n" + pt->strVector()
		//					 + "\n" + pt->m_pBgn->strVector() + 
		//				     + "\n" + pt->m_pFin->strVector() + 
		//					 + "\n\n" + sCross.strPlaneArea()
		//					 + "\n\n" + strPlaneArea()); 


	}//for(POSITION pos = sCross.m_PointList.GetHeadPosition(); pos != NULL; )

	sCross.m_PointList.RemoveAll();
}
////////////////////////////////////////////////////////////////////////////////
BOOL Polygon_2D::AppendChain(Polygon_2D * pPl)
{

	Vector_2D * pV0 = GetFirstPoint();
	Vector_2D * pV1 = GetLastPoint();
	if (pV0->dist(*pV1) < GeomTreshold)
		return FALSE;
	for (POSITION psn = pPl->m_PlaneList.GetHeadPosition(); psn != NULL;)
	{
		POSITION psnPrev = psn;
		Polygon_2D * pB = (Polygon_2D *)pPl->m_PlaneList.GetNext(psn);
		if (pB == this)
			continue;
		Vector_2D * pU0 = pB->GetFirstPoint();
		Vector_2D * pU1 = pB->GetLastPoint();
		BOOL bFind = FALSE;
		if (pV1->dist(*pU0) < GeomTreshold)
		{
			for (POSITION ps = pB->m_PointList.GetHeadPosition(); ps != NULL;)
			{
				Vector_2D * pV = (Vector_2D *)pB->m_PointList.GetNext(ps);
				AppendNewPoint(pV);
			}//for(POSITION ps = pB->m_PointList.GetHeadPosition(); ps != NULL;)
			pB->m_PointList.RemoveAll();
			pPl->m_PlaneList.RemoveAt(psnPrev);

			//  if(bTestPrint)ShowImage(pPl->SetImage(pB, this));
			//		TestMessageBox("01 Polygon_2D::AppendChain\n" + strPlaneArea());


			return TRUE;
		}//if(pV0->dist(* pU1) < GeomTreshold)

		if (pV1->dist(*pU1) < GeomTreshold)
		{

			//	if(bTestPrint)	ShowImage(pPl->SetImage(pB, this));
			//		TestMessageBox("10 Polygon_2D::AppendChain\n" + strPlaneArea());
			for (POSITION ps = pB->m_PointList.GetTailPosition(); ps != NULL;)
			{
				Vector_2D * pV = (Vector_2D *)pB->m_PointList.GetPrev(ps);
				AppendNewPoint(pV);
			}//for(POSITION ps = pB->m_PointList.GetHeadPosition(); ps != NULL;)
			pB->m_PointList.RemoveAll();
			pPl->m_PlaneList.RemoveAt(psnPrev);

			//	if(bTestPrint)  ShowImage(pPl->SetImage(pB, this));
			//		TestMessageBox("11 Polygon_2D::AppendChain\n" + strPlaneArea());
			return TRUE;
		}
	}//for(POSITION psn = pPl->m_PlaneList.GetHeadPosition(); psn != NULL;)

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void Polygon_2D::SetVisible(BOOL bMode)
{
	m_bVisible = bMode;
	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pl = (Polygon_2D *)m_PlaneList.GetNext(pos);
		pl->SetVisible(bMode);
	}//for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)

}
////////////////////////////////////////////////////
void Polygon_2D::SetFgColor(COLORREF clr)
{
	fgColor = clr;
	for (POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)
	{
		Polygon_2D * pl = (Polygon_2D *)m_PlaneList.GetNext(pos);
		pl->SetFgColor(clr);
	}//for(POSITION pos = m_PlaneList.GetHeadPosition(); pos != NULL;)

}
////////////////////////////////////////////////////
CString Polygon_2D::strPlaneArea(void)
{
	CString wString = _T("strPlaneArea:\n");
	wString += strFormat(_T("numPoints = %d numChilds = %d\n"), m_PointList.GetCount(), m_PlaneList.GetCount());
	for (POSITION pos = m_PointList.GetHeadPosition(); pos != NULL;)
	{
		Vector_2D * pnt = (Vector_2D *)m_PointList.GetNext(pos);
		wString += pnt->strVector() + _T("\n");
	}
	return wString;
}
/////////////////////////////////////////////////////////////
BOOL isOverlapping(Vector_2D R00, Vector_2D R01, Vector_2D R10, Vector_2D R11, Vector_2D * Q0, Vector_2D * Q1)
{
	Vector_2D r0 = R01 - R00;
	Vector_2D r1 = R11 - R10;
	if (fabs(r1^r0 / (!r1 * !r0)) > GeomTreshold)
		return FALSE;
	if (fabs((R10 - R00) ^ r0 / (!(R10 - R00) * !r0)) > GeomTreshold)
		return FALSE;
	if(r0*r1 >0)           //if segments in same direction
	if ((R00 - R10)* (R00 - R11) <= 0)  //if R00 is inside the segment(R10, R11)
	{
		*Q0 = R00;
		if ((R01 - R10)* (R01 - R11) <= 0) //if R01 is inside the segment(R10, R11)
			*Q1 = R01;
		else
	      *Q1 = R11;
	}
	else
		if ((R10 - R00)* (R10 - R01) < 0)  //if R10 is inside the segment(R00, R01)
		{
			*Q0 = R10;
			if ((R01 - R10)* (R01 - R11) <= 0) //if R01 is inside the segment(R10, R11)
				*Q1 = R01;
			else
				*Q1 = R11;
		}
		else;
	else                      //if segments in contrary direction
		if ((R00 - R10)* (R00 - R11) <= 0)  //if R00 is inside the segment(R10, R11)
		{
			*Q0 = R00;
			if ((R01 - R10)* (R01 - R11) <= 0) //if R01 is inside the segment(R10, R11)
				*Q1 = R01;
			else
				*Q1 = R10;
		}
		else
			if ((R11 - R00)* (R11 - R01) < 0)  //if R11 is inside the segment(R00, R01)
			{
				*Q0 = R11;
				if ((R01 - R10)* (R01 - R11) <= 0) //if R01 is inside the segment(R10, R11)
					*Q1 = R01;
				else
					*Q1 = R10;
			}
			else
				return FALSE;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////
