#pragma once

/////////////////////////////////////////////////////////////////////////////
// CConnectingDlg dialog

class CConnectingDlg : public CDialog
{
// Construction
public:
	CConnectingDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL IsCancel(void)
	{ return m_bCancel; }

    BOOL Create(UINT nID, CWnd * pWnd)
    { return CDialog::Create(nID,pWnd); }

    void SetMessage(CString strMessage)
    { m_staticMessage.SetWindowText(strMessage); }

// Dialog Data
	enum { IDD = IDD_CONNECTING	 };
	CStatic			m_staticMessage;
	CProgressCtrl	m_progressControl;
	BOOL			m_bCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy() {delete this;} 

// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};
