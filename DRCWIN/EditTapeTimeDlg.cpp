// EditTapeTime.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "EditTapeTimeDlg.h"
#include "afxdialogex.h"


// CEditTapeTime dialog

IMPLEMENT_DYNAMIC(CEditTapeTimeDlg, CDialogEx)

CEditTapeTimeDlg::CEditTapeTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditTapeTimeDlg::IDD, pParent)
	, m_nHour(0)
	, m_nMins(0)
	, m_nSecs(0)
	, m_nTens(0)
	, m_bActive(FALSE)
	, m_nMemIndex(0)
{

}

CEditTapeTimeDlg::~CEditTapeTimeDlg()
{
}

void CEditTapeTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HOUR, m_ctlEditHour);
	DDX_Control(pDX, IDC_EDIT_MINS, m_ctlEditMins);
	DDX_Control(pDX, IDC_EDIT_SECS, m_ctlEditSecs);
	DDX_Control(pDX, IDC_EDIT_TENS, m_ctlEditTens);
	DDX_Control(pDX, IDC_SPIN_HOUR, m_ctlSpinHour);
	DDX_Control(pDX, IDC_SPIN_MINS, m_ctlSpinMins);
	DDX_Control(pDX, IDC_SPIN_SECS, m_ctlSpinSecs);
	DDX_Control(pDX, IDC_SPIN_TENS, m_ctlSpinTens);
	DDX_Control(pDX, IDC_GROUP_BOX, m_ctrlGroupBox);
	DDX_Text(pDX, IDC_EDIT_HOUR, m_nHour);
	DDV_MinMaxInt(pDX, m_nHour, 0, 1);
	DDX_Text(pDX, IDC_EDIT_MINS, m_nMins);
	DDV_MinMaxInt(pDX, m_nMins, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECS, m_nSecs);
	DDV_MinMaxInt(pDX, m_nSecs, 0, 59);
	DDX_Text(pDX, IDC_EDIT_TENS, m_nTens);
	DDV_MinMaxInt(pDX, m_nTens, 0, 9);
	DDX_Check(pDX, IDC_CHECK_ACTIVE, m_bActive);
}


BEGIN_MESSAGE_MAP(CEditTapeTimeDlg, CDialogEx)
END_MESSAGE_MAP()


// CEditTapeTime message handlers


BOOL CEditTapeTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctlSpinHour.SetRange(0, 1);
	m_ctlSpinMins.SetRange(0, 59);
	m_ctlSpinSecs.SetRange(0, 59);
	m_ctlSpinTens.SetRange(0, 9);
	
	CString str;
	str.Format(_T("Memory[%u] %1u:%2.2u:%2.2u:%1u"), m_nMemIndex, m_nHour, m_nMins, m_nSecs, m_nTens);

	m_ctrlGroupBox.SetWindowTextW(str);

	return TRUE;
}


void CEditTapeTimeDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}
