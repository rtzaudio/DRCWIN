// EditSMPTETime.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "EditSMPTETimeDlg.h"
#include "afxdialogex.h"


// CEditSMPTETime dialog

IMPLEMENT_DYNAMIC(CEditSMPTETimeDlg, CDialogEx)

CEditSMPTETimeDlg::CEditSMPTETimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditSMPTETimeDlg::IDD, pParent)
	, m_nHour(0)
	, m_nMins(0)
	, m_nSecs(0)
	, m_nFrame(0)
{

}

CEditSMPTETimeDlg::~CEditSMPTETimeDlg()
{
}

void CEditSMPTETimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HOUR, m_ctlEditHour);
	DDX_Control(pDX, IDC_EDIT_MINS, m_ctlEditMins);
	DDX_Control(pDX, IDC_EDIT_SECS, m_ctlEditSecs);
	DDX_Control(pDX, IDC_EDIT_FRAME, m_ctlEditFrame);
	DDX_Control(pDX, IDC_SPIN_HOUR, m_ctlSpinHour);
	DDX_Control(pDX, IDC_SPIN_MINS, m_ctlSpinMins);
	DDX_Control(pDX, IDC_SPIN_SECS, m_ctlSpinSecs);
	DDX_Control(pDX, IDC_SPIN_FRAME, m_ctlSpinFrame);
	DDX_Control(pDX, IDC_GROUP_BOX, m_ctrlGroupBox);
	DDX_Text(pDX, IDC_EDIT_HOUR, m_nHour);
	DDV_MinMaxInt(pDX, m_nHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MINS, m_nMins);
	DDV_MinMaxInt(pDX, m_nMins, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECS, m_nSecs);
	DDV_MinMaxInt(pDX, m_nSecs, 0, 59);
	DDX_Text(pDX, IDC_EDIT_FRAME, m_nFrame);
	DDV_MinMaxInt(pDX, m_nFrame, 0, 29);
}


BEGIN_MESSAGE_MAP(CEditSMPTETimeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, &CEditSMPTETimeDlg::OnBnClickedButtonZero)
END_MESSAGE_MAP()


// CEditSMPTETime message handlers


BOOL CEditSMPTETimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctlSpinHour.SetRange(0, 24);
	m_ctlSpinMins.SetRange(0, 59);
	m_ctlSpinSecs.SetRange(0, 59);
	m_ctlSpinFrame.SetRange(0, 29);

	UpdateGroupboxCaption();

	return TRUE;
}


void CEditSMPTETimeDlg::UpdateGroupboxCaption(void)
{
	CString str;
	str.Format(_T("%2u:%2.2u:%2.2u.%2.2u"), m_nHour, m_nMins, m_nSecs, m_nFrame);
	m_ctrlGroupBox.SetWindowTextW(str);
}


void CEditSMPTETimeDlg::OnOK()
{
	CDialogEx::OnOK();
}


void CEditSMPTETimeDlg::OnBnClickedButtonZero()
{
	m_nHour = m_nMins = m_nSecs = m_nFrame = 0;

	UpdateGroupboxCaption();

	UpdateData(FALSE);
}

