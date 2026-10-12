// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "ConnectingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectingDlg dialog

CConnectingDlg::CConnectingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectingDlg)
	//}}AFX_DATA_INIT
	m_bCancel = FALSE;
}


void CConnectingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectingDlg)
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectingDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectingDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CConnectingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectingDlg message handlers


BOOL CConnectingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;
}


void CConnectingDlg::OnBnClickedCancel()
{
	m_bCancel = TRUE;

	CDialog::OnCancel();
}
