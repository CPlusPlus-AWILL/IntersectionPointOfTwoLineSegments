#pragma once
#include "Vector_2D.h"


class Segment_2D;
class CMyImage;

#define HS_SOLID           7
#define HS_TRANSPARENT     8

class Polygon_2D: public CObject
{
public:
	Polygon_2D(void);
	~Polygon_2D(void);

	Vector_2D turnPoint;
	COLORREF bgColor;
    COLORREF fgColor;
    double           angle;
    double	         coefX;
    double			 coefY; 										
    __int16 R2_style;
    __int16 fillStyle;
    __int16 prior;
    __int16 penWidth;
    __int16 penStyle;
    __int16 typeNumber;
    __int16 identNumber;

	BOOL m_bClosed;
	BOOL m_bVisible;
	CStringW m_polyName;

	int m_curPoint;


  CObList m_PointList;
  CObList m_PlaneList;
  CObList m_printItemList;

 CPtrArray m_pixArray;   

 CStringArray m_PropArray;
  Polygon_2D * m_pParentPolygon;
 
   
   CBitmap * m_pBitmap;

  double rot;
  Vector_2D vel;

  BOOL AppendNewPoint(Vector_2D * pV);
  Vector_2D * GetPointOfVector(Vector_2D v);

  

  void DeleteContents(BOOL bPoint);
  double GetTrueAngle(void);

  void RemoveInLinePoints(void);
  void IntersectSegmentList(Polygon_2D * pT, Polygon_2D * pRslt);
  Vector_2D * Polygon_2D::GetNextPoint(Vector_2D * pV, BOOL bAll);
  Vector_2D * Polygon_2D::GetPrevPoint(Vector_2D * pV, BOOL bAll);
 
  BOOL IsPointInside(Vector_2D * pT);
  BOOL ReadFile( CFile * pFile );
  int GetNumberOfPoint(Vector_2D * pVt);
 
  void  GetMinMaxPoint( Vector_2D tPoint, Vector_2D * pMin,  Vector_2D * pMax);
  int IsPointInBound(Vector_2D * pv);
Vector_2D GetPoint(int idx);
void DeletePoint(int nmbr);
void RemovePoint(Vector_2D * pnt);
void MakeDoubleStroke(Polygon_2D * source);
void ResetPointArray(void);
Vector_2D * GetPointOfNumber(int nmbr);
void Sdvig(double x, double y);
int GetNumber(Vector_2D * pnt);
Polygon_2D * isPointTouchStroke(CPoint point);
Vector_2D * FindPointInStrokeOfPix(CPoint point);
BOOL DrawSolidStroke(CDC * pDC, Vector_2D scl, int prrt ,Vector_2D parentCenter,
         double parentAngle, double parentCoefX, double parentCoefY);

int GetCount();
CRect GetRect(CRect rect, Vector_2D parentCenter,
         double parentAngle, double parentCoefX, double parentCoefY,
         Vector_2D scl);

void LeftToRight();
void TopToBottom();
Polygon_2D  * GetStroke(int idx, int * counter );

Vector_2D * GetFirstPoint(void);
Vector_2D * GetLastPoint(void);
void CreateRandomSegment(CRect rect);
BOOL MergePlaneArea(Polygon_2D * ppl, Vector_2D * pV1, Vector_2D * pV2);
void RemoveMidPoints(void);
int MergePlaneAreas(void);
BOOL bHasProperty(CString strProp);
BOOL IsNeighbour(Polygon_2D * pPl, Vector_2D ** pV1, Vector_2D ** pV2);
BOOL bHasPoint(Vector_2D * pV);
BOOL bHasSlab(Vector_2D  * pV, Polygon_2D * pPl, Vector_2D  ** pV2);
void RemoveDeleted(void);
void EquPoints(void);
void EquPoints(Polygon_2D * pPn);
void CreateRectStroke( Vector_2D v0, Vector_2D v1);
void UpdatePoints(Vector_2D parentCenter,double parentAngle, double parentCoefX, double parentCoefY);
void MergeChains(Polygon_2D * pPln);
BOOL AppendChain(Polygon_2D * pPl);
BOOL AddHeadPoint(Vector_2D * pV);
void IsCrossStroke(Vector_2D * p0, Vector_2D * p1, Polygon_2D * pS);
double CreateEquilateral(int nn, double a);

void Rotate(Vector_2D vc, double fi);

void MakeDoublePolygon(Polygon_2D * source);
void ReverseOrder(void);
Polygon_2D * IntersectPolygonDemo(Polygon_2D * pPln);
void SetClosed(BOOL bMode);

double Square(void);
BOOL AppheadNewPoint(Vector_2D * pV);
CString strPlaneArea(void);

void SetVisible(BOOL bMode);
void SetFgColor(COLORREF clr);
};


