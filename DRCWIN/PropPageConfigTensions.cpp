// PropPageConfigTensions.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigTensions.h"
#include "afxdialogex.h"


// CPropPageConfigTensions dialog

IMPLEMENT_DYNAMIC(CPropPageConfigTensions, CMFCPropertyPage)

CPropPageConfigTensions::CPropPageConfigTensions(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_TENSIONS)
	, m_nSupplyStopTension(0)
	, m_nSupplyShuttleTension(0)
	, m_nSupplyPlayTensionHI(0)
	, m_nSupplyPlayTensionLO(0)
	, m_nSupplyThreadTension(0)
	, m_nTakeupStopTension(0)
	, m_nTakeupShuttleTension(0)
	, m_nTakeupPlayTensionHI(0)
	, m_nTakeupPlayTensionLO(0)
	, m_nTakeupThreadTension(0)
{

}

CPropPageConfigTensions::~CPropPageConfigTensions()
{
}

void CPropPageConfigTensions::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_STOP, m_nTakeupStopTension);
	DDV_MinMaxInt(pDX, m_nTakeupStopTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_SHUTTLE, m_nTakeupShuttleTension);
	DDV_MinMaxInt(pDX, m_nTakeupShuttleTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_PLAY_HI, m_nTakeupPlayTensionHI);
	DDV_MinMaxInt(pDX, m_nTakeupPlayTensionHI, 10, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_PLAY_LO, m_nTakeupPlayTensionLO);
	DDV_MinMaxInt(pDX, m_nTakeupPlayTensionLO, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_THREAD, m_nTakeupThreadTension);
	DDV_MinMaxInt(pDX, m_nTakeupThreadTension, 0, 200);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_STOP, m_nTakeupStopTension);
	DDV_MinMaxInt(pDX, m_nTakeupStopTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_SHUTTLE, m_nTakeupShuttleTension);
	DDV_MinMaxInt(pDX, m_nTakeupShuttleTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_PLAY_HI, m_nTakeupPlayTensionHI);
	DDV_MinMaxInt(pDX, m_nTakeupPlayTensionHI, 10, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_PLAY_LO, m_nTakeupPlayTensionLO);
	DDV_MinMaxInt(pDX, m_nTakeupPlayTensionLO, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_TAKEUP_TENSION_THREAD, m_nTakeupThreadTension);
	DDV_MinMaxInt(pDX, m_nTakeupThreadTension, 0, 200);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_STOP, m_nSupplyStopTension);
	DDV_MinMaxInt(pDX, m_nSupplyStopTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_SHUTTLE, m_nSupplyShuttleTension);
	DDV_MinMaxInt(pDX, m_nSupplyShuttleTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_PLAY_HI, m_nSupplyPlayTensionHI);
	DDV_MinMaxInt(pDX, m_nSupplyPlayTensionHI, 10, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_PLAY_LO, m_nSupplyPlayTensionLO);
	DDV_MinMaxInt(pDX, m_nSupplyPlayTensionLO, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_THREAD, m_nSupplyThreadTension);
	DDV_MinMaxInt(pDX, m_nSupplyThreadTension, 0, 200);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_STOP, m_nSupplyStopTension);
	DDV_MinMaxInt(pDX, m_nSupplyStopTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_SHUTTLE, m_nSupplyShuttleTension);
	DDV_MinMaxInt(pDX, m_nSupplyShuttleTension, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_PLAY_HI, m_nSupplyPlayTensionHI);
	DDV_MinMaxInt(pDX, m_nSupplyPlayTensionHI, 10, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_PLAY_LO, m_nSupplyPlayTensionLO);
	DDV_MinMaxInt(pDX, m_nSupplyPlayTensionLO, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_SUPPLY_TENSION_THREAD, m_nSupplyThreadTension);
	DDV_MinMaxInt(pDX, m_nSupplyThreadTension, 0, 200);
	DDX_Text(pDX, IDC_EDIT_TENSION_SENSOR_GAIN, m_fTensionSensorGain);
	DDV_MinMaxFloat(pDX, m_fTensionSensorGain, 0.0f, 1.0f);
	DDX_Text(pDX, IDC_EDIT_TENSION_SENSOR_OFFSET_1, m_fTensionSensorMidScale1);
	DDV_MinMaxFloat(pDX, m_fTensionSensorMidScale1, 1500.0f, 2500.0f);
	DDX_Text(pDX, IDC_EDIT_TENSION_SENSOR_OFFSET_2, m_fTensionSensorMidScale2);
	DDV_MinMaxFloat(pDX, m_fTensionSensorMidScale2, 1500.0f, 2500.0f);
	DDX_Text(pDX, IDC_EDIT_REEL_RADIUS_GAIN, m_fReelRadiusGain);
	DDV_MinMaxFloat(pDX, m_fReelRadiusGain, 0.01f, 1.0f);
	DDX_Text(pDX, IDC_EDIT_REEL_OFFSET_GAIN, m_fReelOffsetGain);
	DDV_MinMaxFloat(pDX, m_fReelOffsetGain, 0.05f, 1.0f);
}


BEGIN_MESSAGE_MAP(CPropPageConfigTensions, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigTensions message handlers
