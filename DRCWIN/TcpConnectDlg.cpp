// TcpConnect.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "TcpConnectDlg.h"
#include "afxdialogex.h"


// CTcpConnectDlg dialog

IMPLEMENT_DYNAMIC(CTcpConnectDlg, CDialogEx)

CTcpConnectDlg::CTcpConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTcpConnectDlg::IDD, pParent)
	, m_IPAddr(0)
	, m_strIPAddr("")
	, m_nPortNum(0)
	, m_bAutoConnect(FALSE)
{

}

CTcpConnectDlg::~CTcpConnectDlg()
{
}

void CTcpConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddrCtrl);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_IPAddr);
	DDX_Text(pDX, IDC_EDIT1, m_nPortNum);
	DDV_MinMaxInt(pDX, m_nPortNum, 1000, 20000);
	DDX_Check(pDX, IDC_CHECK_AUTOCONNECT, m_bAutoConnect);
	DDX_Control(pDX, IDC_CHECK_AUTOCONNECT, m_checkAutoConnect);
}


BEGIN_MESSAGE_MAP(CTcpConnectDlg, CDialogEx)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &CTcpConnectDlg::OnIpnFieldchangedIpaddress)
	ON_BN_CLICKED(IDOK, &CTcpConnectDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CTcpConnectDlg message handlers


void CTcpConnectDlg::OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTcpConnectDlg::OnBnClickedOk()
{
	BYTE Field[4];
	m_IPAddrCtrl.GetAddress(Field[0], Field[1], Field[2], Field[3]);
	m_strIPAddr.Format(_T("%u.%u.%u.%u"), Field[0], Field[1], Field[2], Field[3]);

	CDialogEx::OnOK();
}

void CTcpConnectDlg::OnCancel()
{
	m_bAutoConnect = (m_checkAutoConnect.GetCheck() == BST_CHECKED) ? true : false;

	CDialogEx::OnCancel();
}
