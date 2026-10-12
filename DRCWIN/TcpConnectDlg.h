#pragma once
#include "afxcmn.h"


// CTcpConnectDlg dialog

class CTcpConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTcpConnectDlg)

public:
	CTcpConnectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTcpConnectDlg();

// Dialog Data
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCP_CONNECT };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IPAddrCtrl;
	DWORD	m_IPAddr;
	CString	m_strIPAddr;

	afx_msg void OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	int m_nPortNum;
	BOOL m_bAutoConnect;
	virtual void OnCancel();
	CButton m_checkAutoConnect;
};
