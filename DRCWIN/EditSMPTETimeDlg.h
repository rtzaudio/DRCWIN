#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CEditSMPTETime dialog

class CEditSMPTETimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditSMPTETimeDlg)

public:
	CEditSMPTETimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditSMPTETimeDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_SMPTE_TIME };

	void UpdateGroupboxCaption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctlEditHour;
	CEdit m_ctlEditMins;
	CEdit m_ctlEditSecs;
	CEdit m_ctlEditFrame;
	CSpinButtonCtrl m_ctlSpinHour;
	CSpinButtonCtrl m_ctlSpinMins;
	CSpinButtonCtrl m_ctlSpinSecs;
	CSpinButtonCtrl m_ctlSpinFrame;
	CStatic m_ctrlGroupBox;

	int m_nHour;
	int m_nMins;
	int m_nSecs;
	int m_nFrame;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonZero();
};
