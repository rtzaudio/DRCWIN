#pragma once


// CPropPageConfigPlay dialog

class CPropPageConfigPlay : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigPlay)

public:
	CPropPageConfigPlay(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigPlay();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_PLAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	FLOAT m_fPlayBoostPGainLO;
	FLOAT m_fPlayBoostIGainLO;
	FLOAT m_fPlayBoostPGainHI;
	FLOAT m_fPlayBoostIGainHI;
	INT m_nPlayBoostEndVelLO;
	INT m_nPlayBoostEndVelHI;
	INT m_nPinchRollerSettleTime;
	INT m_nPlaySettleTime;
	INT m_nBrakeSettleTime;
	BOOL m_bBrakesStopPlay;
	BOOL m_bEngagePinchRoller;
};
