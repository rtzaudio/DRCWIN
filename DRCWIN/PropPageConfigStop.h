#pragma once


// CPropPageConfigStop dialog

class CPropPageConfigStop : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigStop)

public:
	CPropPageConfigStop(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigStop();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_STOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	INT m_nStopBrakeTorque;
	BOOL m_bLiftersAtStop;
	BOOL m_BrakesAtStop;
	BOOL m_bEndOfTapeDetect;
};
