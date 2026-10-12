#pragma once


// CPropPageConfigTransport dialog

class CPropPageConfigTransport : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigTransport)

public:
	CPropPageConfigTransport(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigTransport();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_TRANSPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	INT m_nVelocityDetectThreshold;
	INT m_nRecordStrobePulseTime;
	INT m_nRecordHoldSettleTime;
	INT m_nButtonDebounceTime;
};
