
// ChildView.h : interface of the CChildView class
//


#pragma once
class CDlgHelp;
#define ID_TIMER_PLAY 1

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	CDC	m_dcMem;          //memory device context
	CBitmap	m_memBmp;     //memory bitmap
	CBitmap	* m_pMemBmp;  //reference to device bitmap
	BOOL m_bPlay;         //Flag of the scene to play
	CDlgHelp * m_pDlgHelp;//reference to non-modal Help Dialog
	BOOL m_bRand;
// Operations   
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();
	void DrawMyScene(CDC * pDC);

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFilePlay();
	afx_msg void OnVkSpace();
	afx_msg void OnVkReturn();
	afx_msg void OnUpdateFilePlay(CCmdUI *pCmdUI);
	afx_msg void OnVkRight();
	afx_msg void OnHelp();
	afx_msg void OnVkSpaceCtrl();
	afx_msg void OnVkSpaceAlt();
};

