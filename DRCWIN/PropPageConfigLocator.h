#pragma once


// CPropPageConfigLocator dialog

class CPropPageConfigLocator : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigLocator)

public:
	CPropPageConfigLocator(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigLocator();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_LOCATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	INT m_nJogNear;
	INT m_nJogMid;
	INT m_nJogFar;
};
