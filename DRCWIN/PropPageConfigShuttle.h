#pragma once


// CPropPageConfigShuttle dialog

class CPropPageConfigShuttle : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigShuttle)

public:
	CPropPageConfigShuttle(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigShuttle();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_SHUTTLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	FLOAT m_fFwdBackTensionGain;
	FLOAT m_fRewBackTensionGain;
	INT m_nShuttleVelocity;
	INT m_nLibWindVelocity;
	INT m_nAutoSlowVelocity;
	INT m_nAutoSlowAtOffset;
	INT m_nAutoSlowAtVelocity;
	INT m_nLifterSettleTime;
	FLOAT m_fShuttlePGain;
	FLOAT m_fShuttleIGain;
	FLOAT m_fShuttleDGain;
};
