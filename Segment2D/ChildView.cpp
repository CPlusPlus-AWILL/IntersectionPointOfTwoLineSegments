
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Segment2D.h"
#include "..\GlobUse\Polygon_2D.h"
#include "DlgHelp.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL bTestPrint = TRUE;
void TestMessageBox(CString tString)
{
	if (!bTestPrint)
		return;
	int rslt = AfxMessageBox(tString, MB_YESNOCANCEL);
	if (rslt == IDNO)
		bTestPrint = FALSE;
	else
		if (rslt == IDCANCEL)
			exit(0);
}
CString strFormat(CString frmt, ...)
{
	_TCHAR    text[256];      // Место для нашей строки
	va_list    ap;          // Указатель на список аргументов
	if (!frmt.GetLength())     // Если нет текста
		return _T("*** Error Format ***");            // Ничего не делать
	va_start(ap, frmt);           // Разбор строки переменных
	_vstprintf_s(text, frmt, ap); // И конвертирование символов в реальные коды
								  //  vswprintf_s(text, frmt, ap);
	return text;
}
////////////////////////////////////////////////////////////////////////////////


CChildView * m_pView = NULL;  //reference this CChildView window
CObList m_polygonList;        // obect list of polygons to draw


void ResetScene(void);
// CChildView

CChildView::CChildView()
	:m_pDlgHelp(NULL)
//	,m_bPlay(FALSE)
	, m_bPlay(TRUE)
	,m_bRand(TRUE)
{
}

CChildView::~CChildView()
{
	while (m_polygonList.GetCount())
	{
		Polygon_2D * pGl = (Polygon_2D *)m_polygonList.GetTail();
		m_polygonList.RemoveTail();
		pGl->DeleteContents(TRUE);
	}
	m_dcMem.SelectObject(m_pMemBmp);
	m_memBmp.DeleteObject();
	ReleaseDC(&m_dcMem);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_PLAY, &CChildView::OnFilePlay)
	ON_COMMAND(ID_VK_SPACE, &CChildView::OnVkSpace)
	ON_COMMAND(ID_VK_RETURN, &CChildView::OnVkReturn)
	ON_UPDATE_COMMAND_UI(ID_FILE_PLAY, &CChildView::OnUpdateFilePlay)
	ON_COMMAND(ID_VK_RIGHT, &CChildView::OnVkRight)
	ON_COMMAND(ID_HELP, &CChildView::OnHelp)
	ON_COMMAND(ID_VK_SPACE_CTRL, &CChildView::OnVkSpaceCtrl)
	ON_COMMAND(ID_VK_SPACE_ALT, &CChildView::OnVkSpaceAlt)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
CPaintDC dc(this); // device context for painting
DrawMyScene(&dc);
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pView = this;                              //reference to this CChidView window
	srand((unsigned)time(NULL));                 //set random seed 
	m_dcMem.CreateCompatibleDC(GetDC());         //create memory device context
	//create memory device bitmap:
	m_memBmp.CreateCompatibleBitmap(GetDC(),m_dcMem.GetDeviceCaps(HORZRES),m_dcMem.GetDeviceCaps(VERTRES));
	m_pMemBmp = m_dcMem.SelectObject(&m_memBmp); //select memory device bitmap object 
	SetTimer(ID_TIMER_PLAY, 50, NULL);           //start timer for 20 scenes per second
	m_pDlgHelp = new CDlgHelp;                   //non-modal Help Dialog initialization
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
void DrawMyScene(void)
{
	m_pView->DrawMyScene(m_pView->GetDC());
}
extern Polygon_2D * m_pDr;
extern Polygon_2D * m_pDs;

extern Vector_2D vDr0;
extern Vector_2D vDs0;
extern Vector_2D vDr;
extern Vector_2D vDs;
void HandleTimerPlayScene(void)
{

	//     UINT delay = 3000;
	UINT delay = 500;
	//if (m_pDs != NULL)
	//{
	//	if (!(m_pDs->turnPoint - vDs) < GeomTreshold)
	//	{
	//		if (fabs(vDs.x) > GeomTreshold)
	//		{
	//			vDs = Vector_2D(0);
	//			POSITION pos = m_polygonList.GetHeadPosition();
	//			Polygon_2D * pp = (Polygon_2D *)m_polygonList.GetNext(pos);
	//			pp->SetVisible(FALSE);
	//			pp = (Polygon_2D *)m_polygonList.GetNext(pos);
	//			pp->SetVisible(FALSE);
	//		}
	//		else
	//		{
	//			m_pView->KillTimer(ID_TIMER_PLAY);
	//			Sleep(delay);
	//			Polygon_2D * ppC = (Polygon_2D *)m_polygonList.GetTail();
	//			ppC->SetVisible(TRUE);
	//			DrawMyScene();


	//			Sleep(delay);
	//			POSITION pos = m_polygonList.GetHeadPosition();
	//			Polygon_2D * pp = (Polygon_2D *)m_polygonList.GetNext(pos);
	//			pp->SetVisible(TRUE);
	//			pp = (Polygon_2D *)m_polygonList.GetNext(pos);
	//			pp->SetVisible(TRUE);
	//			DrawMyScene();
	//			Sleep(delay);
	//			Polygon_2D * ppA = (Polygon_2D *)m_polygonList.GetNext(pos);
	//			ppA->SetVisible(FALSE);
	//			Polygon_2D * ppB = (Polygon_2D *)m_polygonList.GetNext(pos);
	//			ppB->SetVisible(FALSE);
	//			ppC->SetVisible(FALSE);
	//			DrawMyScene();
	//			Sleep(delay);
	//			ppA->SetVisible(TRUE);
	//			ppB->SetVisible(TRUE);
	//			DrawMyScene();
	//			Sleep(delay);
	//			vDs = vDs0;
	//			if (m_pView->m_bPlay)
	//				m_pView->SetTimer(ID_TIMER_PLAY, 50, NULL);
	//		}
	//		Vector_2D v = Vector_2D(0) - m_pDs->vel;
	//		m_pDs->vel = v;
	//		m_pDs->rot = -m_pDs->rot;
	//		v = Vector_2D(0) - m_pDr->vel;
	//		m_pDr->vel = v;
	//		m_pDr->rot = -m_pDr->rot;
	//	}//if(!(m_pDs->m_pGLObject->cntrPt - vDs) < GeomTreshold)
	//}//if(m_pDs != NULL)

	CRect rct;
	m_pView->GetClientRect(&rct);
	Vector_2D vc ((rct.left + rct.right) / 2, (rct.top + rct.bottom) / 2);

	 //UpdateCadr:
	for (POSITION psb = m_polygonList.GetHeadPosition();psb != NULL;)
	{
		Polygon_2D * pRs = (Polygon_2D *)m_polygonList.GetNext(psb);

		Vector_2D * pV0 = (Vector_2D *)pRs->m_PointList.GetHead();
		Vector_2D *	pV = (Vector_2D *)pRs->m_PointList.GetTail();

		int dd = int(!(*pV - *pV0)*0.5);
	
//		pRs->turnPoint += pRs->vel;

		//if (pRs->turnPoint.x >= vc.x + 320 || pRs->turnPoint.x <= vc.x - 320)
		//	pRs->vel = Vector_2D(0) - pRs->vel;
		//if (pRs->turnPoint.y >= vc.y + 240 || pRs->turnPoint.y <= vc.y - 240)
		//	pRs->vel = Vector_2D(0) - pRs->vel;

		//if (pRs->m_PlaneList.GetCount())
		//{
		//	Polygon_2D * pVel = (Polygon_2D *)pRs->m_PlaneList.GetHead();
		//	Vector_2D * pV = (Vector_2D *)pVel->m_PointList.GetTail();
		//	pRs->vel = 0.2*(* pV);
		//}

		BOOL bRev = FALSE;
		if (m_pView->m_bRand)
		{

			bRev = pRs->turnPoint.x >= rct.right - dd || pRs->turnPoint.x <= rct.left + dd;
			if (!bRev)
				bRev = pRs->turnPoint.y >= rct.bottom - dd || pRs->turnPoint.y <= rct.top + dd;
		}
		else
		{
			CPoint * pPt = (CPoint *)pRs->m_pixArray.GetAt(0);

			bRev = pPt->x >= rct.right || pPt->x <= rct.left;
			if (!bRev)
				bRev = pPt->y >= rct.bottom || pPt->y <= rct.top;


			pPt = (CPoint *)pRs->m_pixArray.GetAt(1);

			if (!bRev)
				bRev = pPt->x >= rct.right || pPt->x <= rct.left;
			if (!bRev)
				bRev = pPt->y >= rct.bottom || pPt->y <= rct.top;
		}




		if (bRev)
		{
			pRs->vel = Vector_2D(0) - pRs->vel;
	//		pRs->rot = -pRs->rot;
	//		TestMessageBox(_T("HandleTimerPlayScene\n") + pV0->strVector() + _T("\n") + pV->strVector());

		}

		if (m_pView->m_bRand)
			pRs->turnPoint += pRs->vel;
		else
		{
			*pV0 += pRs->vel;
			*pV += pRs->vel;
		}



		pRs->angle += pRs->rot;
//		Vector_2D R01 = *pV;

//				TestMessageBox(_T("HandleTimerPlayScene\n") + strFormat(_T("vel^(R01-R00) = %g\n"),		pRs->vel ^ (R01 - R00) /(!pRs->vel * !(R01 - R00))));


	}

}
/////////////////////////////////////////////////////////////////////////////

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ID_TIMER_PLAY:
		if (m_bPlay)
		{
			HandleTimerPlayScene();
			DrawMyScene(GetDC());
		}
		break;
	}

	CWnd::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////////////////////

void DrawScene(CDC * pDC, CRect rect)
{
 pDC->FillRect(rect, &CBrush(RGB(255, 245, 221)));                   //install background colour
 for (POSITION pos = m_polygonList.GetHeadPosition(); pos != NULL;)  //for all the polygons:
 {
  Polygon_2D * ppl = (Polygon_2D *)m_polygonList.GetNext(pos);       //get next polygon 
  ppl->DrawSolidStroke(pDC, Vector_2D(1), 0, ppl->turnPoint, 0., 1., 1.); //draw polygon
 }
}
//////////////////////////////////////////////////////////////////////////////////////////////void CChildView::DrawMyScene(CDC * pDC)
void CChildView::DrawMyScene(CDC * pDC)
{
 CRect rect;                       //this window rectangle
 GetClientRect(&rect);             //get rect sizes
 DrawScene(&m_dcMem, rect);        //draw scene into memory device context
 //copy scene from memory device context into target context pDC;
 pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_dcMem, 0, 0, SRCCOPY);
}
////////////////////////////////////////
void CreateRandomSegment(void)
{
// CRect rect(50, 50, 690, 530);
 CRect rect(50, 50, 690, 530);
 m_pView->GetClientRect(&rect);
 
 int marg = 50;
 Polygon_2D * pNew = new Polygon_2D;
 pNew->CreateRandomSegment(CRect(rect.left + marg, rect.top + marg, rect.right - marg, rect.bottom - marg));
 m_polygonList.AddTail(pNew);
  CRect rct;
 m_pView->GetClientRect(&rct);
 pNew->turnPoint = Vector_2D((rct.left + rct.right) / 2, (rct.top + rct.bottom) / 2);

 }
/////////////////////////////////////////////////////////////////////////////////////////
void ResetScene(void)
{
	while (m_polygonList.GetCount())
	{
		Polygon_2D * pGl = (Polygon_2D *)m_polygonList.GetTail();
		m_polygonList.RemoveTail();
		pGl->DeleteContents(TRUE);
	}

	CreateRandomSegment();
	Polygon_2D * pGl = (Polygon_2D *)m_polygonList.GetTail();
	pGl->m_polyName = "RED_INIT";
	pGl->fgColor = RGB(255, 0, 0);
	pGl->vel = Vector_2D(8, 0);


	CreateRandomSegment();
	pGl = (Polygon_2D *)m_polygonList.GetTail();
	pGl->m_polyName = "BLUE_INIT";
	pGl->fgColor = RGB(0, 0, 255);
	pGl->vel = Vector_2D(0, 6);

}
////////////////////////////////////////////////////////////////

void CChildView::OnFilePlay()
{
	if (!m_polygonList.GetCount())
	{
		OnVkSpace();
		m_bPlay = TRUE;
	}
	else
	{
		m_bPlay = !m_bPlay;

		if (m_bPlay)
			SetTimer(ID_TIMER_PLAY, 50, NULL);           //start timer for 20 scenes per second
		else
			KillTimer(ID_TIMER_PLAY);
	}
}


void CChildView::OnVkSpace()
{
	m_bRand = TRUE;
	ResetScene();
	DrawMyScene(GetDC());
	if(m_bPlay)
		SetTimer(ID_TIMER_PLAY, 50, NULL);           //start timer for 20 scenes per second
}
/////////////////////////////////////////////////////////////////////////////////
BOOL isOverlapping(Vector_2D R00, Vector_2D R01, Vector_2D R10, Vector_2D R11, Vector_2D * Q0, Vector_2D * Q1);

void CChildView::OnVkReturn()
{
	if (m_polygonList.GetCount() != 2)
		return;

	KillTimer(ID_TIMER_PLAY);

//	POSITION pos = m_polygonList.GetTailPosition();
	Polygon_2D * pA = (Polygon_2D *)m_polygonList.GetHead();
	Polygon_2D * pB = (Polygon_2D *)m_polygonList.GetTail();

	Vector_2D vv = pA->vel;

	Vector_2D * pU = (Vector_2D *)pA->m_PointList.GetHead();
	Vector_2D t00 = *pU;
	pU = (Vector_2D *)pA->m_PointList.GetTail();
	Vector_2D t01 = *pU;

//					TestMessageBox(_T("CChildView::OnVkReturn Start\n") + strFormat(_T("vel^(R01-R00) = %g\n"),		vv ^ (t01 - t00) /(!vv * !(t01 - t00))) + vv.strVector() + _T("\n") + (t01-t00).strVector());


	pA->rot = pB->rot = 0;
	pA->vel = pB->vel = 0;

	Polygon_2D * p0 = new Polygon_2D;
	p0->MakeDoublePolygon(pA);

	Polygon_2D * p1 = new Polygon_2D;
	p1->MakeDoublePolygon(pB);

	//TestMessageBox("CWeiler2DView::OnVkReturn\n" + 

	p0->UpdatePoints(Vector_2D(0), 0, 1, 1);
	p1->UpdatePoints(Vector_2D(0), 0, 1, 1);

	Vector_2D * pV = (Vector_2D *)p0->m_PointList.GetHead();
	Vector_2D R00 = *pV;
	pV = (Vector_2D *)p0->m_PointList.GetTail();
	Vector_2D R01 = *pV;
	Vector_2D r0 = R01-R00;
	
	pV = (Vector_2D *)p1->m_PointList.GetHead();
	Vector_2D R10 = *pV;
	pV = (Vector_2D *)p1->m_PointList.GetTail();
	Vector_2D R11 = *pV;
	Vector_2D r1 = R11 - R10;


//			TestMessageBox(_T("CChildView::OnVkReturn 2\n") + strFormat(_T("p0->vel^(r0) = %g\n"), (vv ^ r0)/(!vv * !r0)) + vv.strVector() + _T("\n") + r0.strVector());


	if (fabs((r1^r0) / (!r1 * !r0)) > GeomTreshold)
	{
		Vector_2D R = (r0 * (R11^R10) - r1 *(R01^R00)) / (r1^r0);
		Polygon_2D * pC = new Polygon_2D; 
		pC->CreateEquilateral(4, 4);   //create the mark point of intersection;
		pC->bgColor = RGB(255, 255, 0);
		pC->turnPoint = R;
		m_polygonList.AddTail(pC);

		if ((R - R00)* (R - R01) > 0)
		{
			pC = new Polygon_2D;
			if ((R - R00)* (R - R00) < (R - R01)* (R - R01))
				pV = new Vector_2D(R00);
			else
				pV = new Vector_2D(R01);
			pC->AppendNewPoint(pV);
			pV = new Vector_2D(R);
			pC->AppendNewPoint(pV);
			pC->SetFgColor(RGB(255, 0, 255));
			pC->penStyle = PS_DOT;
			pC->R2_style = R2_COPYPEN;
			m_polygonList.AddTail(pC);
		}


		if ((R - R10)* (R - R11) > 0)
		{
			pC = new Polygon_2D;
			if ((R - R10)* (R - R10) < (R - R11)* (R - R11))
				pV = new Vector_2D(R10);
			else
				pV = new Vector_2D(R11);
			pC->AppendNewPoint(pV);
			pV = new Vector_2D(R);
			pC->AppendNewPoint(pV);
			pC->SetFgColor(RGB(0, 255, 255));
			pC->R2_style = R2_COPYPEN;
			pC->penStyle = PS_DOT;
			m_polygonList.AddTail(pC);
		}
	}
	else
		if (fabs(r0 ^ (R10 - R00)) / (!r0 * !(R10 - R00)) > GeomTreshold)
			MessageBox(_T("No intersection."), _T("No intersection:"), MB_OK | MB_ICONERROR);
		else
		{
			Vector_2D Q0, Q1;
			if (isOverlapping(R00, R01, R10, R11, &Q0, &Q1))
			{
				Polygon_2D * pC = new Polygon_2D; //create the first mark point inside the segment(R10, R11)
				pC->CreateEquilateral(4, 6);
				pC->bgColor = RGB(255, 255, 0);
				pC->turnPoint = Q0;
				m_polygonList.AddTail(pC);

				pC = new Polygon_2D;             //create the second mark point inside the segment(R10, R11)
				pC->CreateEquilateral(4, 6);
				pC->bgColor = RGB(255, 255, 0);
				pC->turnPoint = Q1;
				m_polygonList.AddTail(pC);
			}
			else
				MessageBox(_T("No overlapping."), _T("No overlapping:"), MB_OK | MB_ICONERROR);
		}



	DrawMyScene(GetDC());


//	TestMessageBox(_T("CChildView::OnVkReturn FIN\n") + R.strVector());
}


void CChildView::OnUpdateFilePlay(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bPlay);
}


void CChildView::OnVkRight()
{
	if (!m_bPlay)
	{
		HandleTimerPlayScene();
		DrawMyScene(GetDC());
	}
}


void CChildView::OnHelp()
{
	m_pDlgHelp->ShowWindow(SW_SHOWNORMAL);
}


void CChildView::OnVkSpaceCtrl()
{
	m_bRand = FALSE;
	while (m_polygonList.GetCount())
	{
		Polygon_2D * pGl = (Polygon_2D *)m_polygonList.GetTail();
		m_polygonList.RemoveTail();
		pGl->DeleteContents(TRUE);
	}

	CreateRandomSegment();
	Polygon_2D * pRed = (Polygon_2D *)m_polygonList.GetTail();
	pRed->m_polyName = "RED_INIT";
	pRed->fgColor = RGB(255, 0, 0);
	pRed->rot = 0;
	pRed->vel = Vector_2D(8, 0);


	Polygon_2D * p0 = new Polygon_2D;
	p0->MakeDoublePolygon(pRed);
	p0->turnPoint = pRed->turnPoint;
	p0->angle = pRed->angle;

	Vector_2D * pV0 = (Vector_2D *)p0->m_PointList.GetHead();
	Vector_2D R00 = *pV0;
	Vector_2D * pV1 = (Vector_2D *)p0->m_PointList.GetTail();
	Vector_2D R01 = *pV1;

	*pV0 += 0.025 * ( rand() % 100 - 50.)*(R01 - R00);
	*pV1 += 0.025 * (rand() % 100 - 50.)*(R01 - R00);
//	pNew->CreateRandomSegment(CRect(rect.left + marg, rect.top + marg, rect.right - marg, rect.bottom - marg));
	m_polygonList.AddTail(p0);
	CRect rct;
	m_pView->GetClientRect(&rct);
	p0->turnPoint = Vector_2D((rct.left + rct.right) / 2, (rct.top + rct.bottom) / 2);



	Polygon_2D * pGl = (Polygon_2D *)m_polygonList.GetTail();
	pGl->m_polyName = "BLUE_INIT";
	pGl->fgColor = RGB(0, 0, 255);
	pGl->rot = 0;
	pGl->vel = Vector_2D(0, 6);
	DrawMyScene(GetDC());
	if (m_bPlay)
		SetTimer(ID_TIMER_PLAY, 50, NULL);           //start timer for 20 scenes per second

}


void CChildView::OnVkSpaceAlt()
{
	m_bRand = FALSE;

	while (m_polygonList.GetCount())
	{
		Polygon_2D * pGl = (Polygon_2D *)m_polygonList.GetTail();
		m_polygonList.RemoveTail();
		pGl->DeleteContents(TRUE);
	}

	CreateRandomSegment();
	Polygon_2D * pRed = (Polygon_2D *)m_polygonList.GetHead();
	pRed->m_polyName = "RED_INIT";
	pRed->fgColor = RGB(255, 0, 0);
	pRed->rot = 0;
	


	Polygon_2D * p0 = new Polygon_2D;
	p0->MakeDoublePolygon(pRed);

	pRed->penWidth = 7;

	Vector_2D * pV0 = (Vector_2D *)pRed->m_PointList.GetHead();
	Vector_2D R00 = *pV0;
	Vector_2D * pV1 = (Vector_2D *)pRed->m_PointList.GetTail();
	Vector_2D R01 = *pV1;

	
	Vector_2D e0 = Normalize(R01 - R00);
	Vector_2D vt = 10.*e0;
	pRed->vel = Vector_2D(vt.x, vt.y);
//	pRed->vel = (R01 - R00)*0.05;
	//pRed->vel.y = -pRed->vel.y;

	*pV0 += 0.025 * (rand() % 100 - 50.)*(R01 - R00);
	*pV1 += 0.025 * (rand() % 100 - 50.)*(R01 - R00);

	m_polygonList.AddTail(p0);
	CRect rct;
	m_pView->GetClientRect(&rct);
//	p0->turnPoint = Vector_2D((rct.left + rct.right) / 2, (rct.top + rct.bottom) / 2);
	p0->vel = Vector_2D(0) - pRed->vel;

	p0->m_polyName = "BLUE_INIT";
	p0->fgColor = RGB(0, 0, 255);
	p0->rot = 0;
	//pGl->vel = Vector_2D(0)-pRed->vel;

	//Polygon_2D * pVel = new Polygon_2D;
	//Vector_2D * pV = new Vector_2D(0);
	//pVel->AppendNewPoint(pV);
	// pV = new Vector_2D(5* p0->vel);
	//pVel->AppendNewPoint(pV);
	//p0->m_PlaneList.AddTail(pVel);
	//pVel->fgColor = RGB(0, 255, 0);
	//pVel->penWidth = 5;
	DrawMyScene(GetDC());
	if (m_bPlay)
		SetTimer(ID_TIMER_PLAY, 50, NULL);           //start timer for 20 scenes per second

//		TestMessageBox(_T("CChildViewOnVkSpaceAlt FIN\n") + strFormat(_T("e0^(R01-R00) = %g\nvel^(R01-R00) = %g\n"), e0 ^ (R01 - R00)/!(R01-R00), p0->vel ^ (R01 - R00) /(!p0->vel * !(R01 - R00))));

}
