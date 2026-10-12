#pragma once


// CPropPageConfigSMPTE dialog

class CPropPageConfigSMPTE : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigSMPTE)

public:
	CPropPageConfigSMPTE(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigSMPTE();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_SMPTE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	FLOAT m_fRefClock;
	INT m_fps;
};
