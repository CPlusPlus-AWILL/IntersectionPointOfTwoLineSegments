// DlgHelp.cpp: файл реализации
//

#include "stdafx.h"
#include "Segment2D.h"
#include "DlgHelp.h"
#include "ChildView.h"
#include "afxdialogex.h"

#pragma warning(disable : 4996)

_TCHAR * CommonCommand[] =
{
	_T("Ctrl + P    :Play / Stop"),
	_T("Alt + F4    :Exit"),
	_T("Space Bar   :Random"),
	_T("Ctrl + Space Bar   :Collinear"),
	_T("Alt + Space Bar   :Overlapping"),
	_T("Enter       :Start Clipping"),
	_T("F1          :This Help"),
	_T("-->         :Next Scene(if Stopped)"),
	_T(""),
};
// диалоговое окно CDlgHelp

IMPLEMENT_DYNAMIC(CDlgHelp, CDialogEx)

CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_HELP, pParent)
	, m_curRow(-1)
{
	Create(IDD_DLG_HELP);
}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_LIST_TODO, m_list_todo);
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgHelp::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TODO, &CDlgHelp::OnLvnItemchangedListTodo)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TODO, &CDlgHelp::OnNMDblclkListTodo)
END_MESSAGE_MAP()


// обработчики сообщений CDlgHelp


BOOL CDlgHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDialog::OnInitDialog();
	SetIcon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);



	CRect rect;
	m_list_todo.GetWindowRect(&rect);
	m_list_todo.InsertColumn(0, _T("Control"), LVCFMT_LEFT, 120);
	m_list_todo.InsertColumn(1, _T("Action performed"), LVCFMT_LEFT, rect.Width() - 125);
	m_list_todo.SetExtendedStyle(m_list_todo.GetExtendedStyle() | LVS_EX_GRIDLINES);

	int i = 0;
	_TCHAR str[256];
		while (CommonCommand[i][0])
		{
			CString s = CommonCommand[i];
			_tcscpy_s(str, s);
			CString aString = _tcstok(str, _T(":"));
			CString bString = s.Mid(aString.GetLength() + 1);
			while (aString.Right(1) == _T(" "))
				aString = aString.Left(aString.GetLength() - 1);

			while (bString.Right(1) == _T(" "))
				bString = bString.Left(bString.GetLength() - 1);

			m_list_todo.InsertItem(i, aString);
			m_list_todo.SetItem(i, 1, LVIF_TEXT, bString, 0, 0, 0, 0);
			i++;
		}

	AdjustHelpToContent();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}
void CDlgHelp::AdjustHelpToContent(void)
{
	CRect lRect;
	m_list_todo.GetWindowRect(&lRect);
	ScreenToClient(&lRect);

	CRect iRect;
	m_list_todo.GetItemRect(0, &iRect, LVIR_BOUNDS);

	int nHeight = (m_list_todo.GetItemCount() + 2)* iRect.Height();
	m_list_todo.MoveWindow(lRect.left, lRect.top, lRect.Width(), nHeight);

	CRect bRect;
	m_btnOk.GetWindowRect(&bRect);
	ScreenToClient(&bRect);

	m_list_todo.GetWindowRect(&lRect);
	ScreenToClient(&lRect);

	m_btnOk.MoveWindow(bRect.left, lRect.bottom + 3, bRect.Width(), bRect.Height());
	m_btnOk.GetWindowRect(&bRect);

	CRect rect;
	GetWindowRect(&rect);
	MoveWindow(rect.left, rect.top, rect.Width(), bRect.bottom + 10 - rect.top);
}
////////////////////////////////////////////////////////////////////////////////////
extern CChildView * m_pView;

void CDlgHelp::OnBnClickedOk()
{

		CString tString;
		if (m_curRow < 0)
			return;
		tString = m_list_todo.GetItemText(m_curRow, 0);

		
		if (tString == _T("Ctrl + P")) m_pView->OnFilePlay();
		else if (tString == _T("Alt + F4")) exit(0);
		else if (tString == _T("Space Bar")) m_pView->OnVkSpace();
		else if (tString == _T("Ctrl + Space Bar")) m_pView->OnVkSpaceCtrl();
		else if (tString == _T("Alt + Space Bar")) m_pView->OnVkSpaceAlt();
		else if (tString == _T("Enter")) m_pView->OnVkReturn();
		else if (tString == _T("-->"))     m_pView->OnVkRight();
	
	}
	///////////////////////////////////////////////////////////////////////


void CDlgHelp::OnLvnItemchangedListTodo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	m_curRow = pNMLV->iItem;
    m_btnOk.EnableWindow(m_curRow >= 0);
	*pResult = 0;
}


void CDlgHelp::OnNMDblclkListTodo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnBnClickedOk();
	*pResult = 0;
}
