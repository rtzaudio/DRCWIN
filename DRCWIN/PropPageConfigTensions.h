#pragma once


// CPropPageConfigTensions dialog

class CPropPageConfigTensions : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigTensions)

public:
	CPropPageConfigTensions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigTensions();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_TENSIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	INT m_nSupplyStopTension;
	INT m_nSupplyShuttleTension;
	INT m_nSupplyPlayTensionHI;
	INT m_nSupplyPlayTensionLO;
	INT m_nSupplyThreadTension;
	INT m_nTakeupStopTension;
	INT m_nTakeupShuttleTension;
	INT m_nTakeupPlayTensionHI;
	INT m_nTakeupPlayTensionLO;
	INT m_nTakeupThreadTension;
	FLOAT m_fTensionSensorGain;
	FLOAT m_fTensionSensorMidScale1;
	FLOAT m_fTensionSensorMidScale2;
	FLOAT m_fReelRadiusGain;
	FLOAT m_fReelOffsetGain;
};
