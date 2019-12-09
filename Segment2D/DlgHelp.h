#pragma once
//#include "C:\Program Files\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"
//#include "C:\Program Files\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxcmn.h"
#include "afxwin.h"
#include "afxcmn.h"

// диалоговое окно CDlgHelp

class CDlgHelp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // стандартны?конструкто?
	virtual ~CDlgHelp();
	int m_curRow;
	void AdjustHelpToContent(void);
// Данные диалоговог?окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HELP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержк?DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnOk;
	CListCtrl m_list_todo;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListTodo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTodo(NMHDR *pNMHDR, LRESULT *pResult);
};
