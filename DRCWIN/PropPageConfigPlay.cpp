// PropPageConfigPlay.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigPlay.h"
#include "afxdialogex.h"


// CPropPageConfigPlay dialog

IMPLEMENT_DYNAMIC(CPropPageConfigPlay, CMFCPropertyPage)

CPropPageConfigPlay::CPropPageConfigPlay(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_PLAY)
	, m_nPlayBoostEndVelLO(0)
	, m_nPlayBoostEndVelHI(0)
	, m_nPinchRollerSettleTime(0)
	, m_nPlaySettleTime(0)
	, m_nBrakeSettleTime(0)
	, m_bBrakesStopPlay(FALSE)
	, m_bEngagePinchRoller(FALSE)
	, m_fPlayBoostPGainLO(0)
	, m_fPlayBoostIGainLO(0)
	, m_fPlayBoostPGainHI(0)
	, m_fPlayBoostIGainHI(0)
{

}

CPropPageConfigPlay::~CPropPageConfigPlay()
{
}

void CPropPageConfigPlay::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PLAY_BOOST_PGAIN_LO, m_fPlayBoostPGainLO);
	DDV_MinMaxFloat(pDX, m_fPlayBoostPGainLO, 0, 5);
	DDX_Text(pDX, IDC_EDIT_PLAY_BOOST_IGAIN_LO, m_fPlayBoostIGainLO);
	DDV_MinMaxFloat(pDX, m_fPlayBoostIGainLO, 0, 2);
	DDX_Text(pDX, IDC_EDIT_EXIT_BOOST_AT_VEL_LO, m_nPlayBoostEndVelLO);
	DDV_MinMaxInt(pDX, m_nPlayBoostEndVelLO, 25, 100);
	DDX_Text(pDX, IDC_EDIT_PLAY_BOOST_PGAIN_HI, m_fPlayBoostPGainHI);
	DDV_MinMaxFloat(pDX, m_fPlayBoostPGainHI, 0, 5);
	DDX_Text(pDX, IDC_EDIT_PLAY_BOOST_IGAIN_HI, m_fPlayBoostIGainHI);
	DDV_MinMaxFloat(pDX, m_fPlayBoostIGainHI, 0, 2);
	DDX_Text(pDX, IDC_EDIT_EXIT_BOOST_AT_VEL_HI, m_nPlayBoostEndVelHI);
	DDV_MinMaxInt(pDX, m_nPlayBoostEndVelHI, 100, 150);
	DDX_Text(pDX, IDC_EDIT_PINCH_ROLLER_SETTLE_TIME, m_nPinchRollerSettleTime);
	DDV_MinMaxInt(pDX, m_nPinchRollerSettleTime, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_PLAY_SETTLE_TIME, m_nPlaySettleTime);
	DDV_MinMaxInt(pDX, m_nPlaySettleTime, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_BRAKE_SETTLE_TIME, m_nBrakeSettleTime);
	DDV_MinMaxInt(pDX, m_nBrakeSettleTime, 0, 2000);
	DDX_Check(pDX, IDC_CHECK_BRAKES_STOP_PLAY, m_bBrakesStopPlay);
	DDX_Check(pDX, IDC_CHECK_ENGAGE_PINCH_ROLLER, m_bEngagePinchRoller);
}


BEGIN_MESSAGE_MAP(CPropPageConfigPlay, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigPlay message handlers
