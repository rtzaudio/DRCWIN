#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CEditTapeTime dialog

class CEditTapeTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditTapeTimeDlg)

public:
	CEditTapeTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTapeTimeDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_TAPE_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctlEditHour;
	CEdit m_ctlEditMins;
	CEdit m_ctlEditSecs;
	CEdit m_ctlEditTens;
	CSpinButtonCtrl m_ctlSpinHour;
	CSpinButtonCtrl m_ctlSpinMins;
	CSpinButtonCtrl m_ctlSpinSecs;
	CSpinButtonCtrl m_ctlSpinTens;
	CStatic m_ctrlGroupBox;

	int m_nMemIndex;
	int m_nHour;
	int m_nMins;
	int m_nSecs;
	int m_nTens;
	BOOL m_bActive;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
