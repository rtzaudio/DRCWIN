// PropPageConfigShuttle.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigShuttle.h"
#include "afxdialogex.h"


// CPropPageConfigShuttle dialog

IMPLEMENT_DYNAMIC(CPropPageConfigShuttle, CMFCPropertyPage)

CPropPageConfigShuttle::CPropPageConfigShuttle(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_SHUTTLE)
	, m_nShuttleVelocity(0)
	, m_nLibWindVelocity(0)
	, m_nAutoSlowVelocity(0)
	, m_nAutoSlowAtOffset(0)
	, m_nAutoSlowAtVelocity(0)
	, m_nLifterSettleTime(0)
{

}

CPropPageConfigShuttle::~CPropPageConfigShuttle()
{
}

void CPropPageConfigShuttle::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOLDBACK_FWD_GAIN, m_fFwdBackTensionGain);
	DDV_MinMaxFloat(pDX, m_fFwdBackTensionGain, 0.005f, 0.25f);
	DDX_Text(pDX, IDC_EDIT_HOLDBACK_REW_GAIN, m_fRewBackTensionGain);
	DDV_MinMaxFloat(pDX, m_fRewBackTensionGain, 0.005f, 0.25f);
	DDX_Text(pDX, IDC_EDIT_SHUTTLE_VELOCITY, m_nShuttleVelocity);
	DDV_MinMaxInt(pDX, m_nShuttleVelocity, 100, 1200);
	DDX_Text(pDX, IDC_EDIT_LIBWIND_VELOCITY, m_nLibWindVelocity);
	DDV_MinMaxInt(pDX, m_nLibWindVelocity, 50, 500);
	DDX_Text(pDX, IDC_EDIT_SLOW_VELOCITY, m_nAutoSlowVelocity);
	DDV_MinMaxInt(pDX, m_nAutoSlowVelocity, 0, 500);
	DDX_Text(pDX, IDC_EDIT_SLOW_AT_OFFSET, m_nAutoSlowAtOffset);
	DDV_MinMaxInt(pDX, m_nAutoSlowAtOffset, 0, 100);
	DDX_Text(pDX, IDC_EDIT_SLOW_AT_VELOCITY, m_nAutoSlowAtVelocity);
	DDV_MinMaxInt(pDX, m_nAutoSlowAtVelocity, 0, 800);
	DDX_Text(pDX, IDC_EDIT_LIFTER_SETTLE_TIME, m_nLifterSettleTime);
	DDV_MinMaxInt(pDX, m_nLifterSettleTime, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_P_GAIN, m_fShuttlePGain);
	DDV_MinMaxFloat(pDX, m_fShuttlePGain, 0.0f, 10.0f);
	DDX_Text(pDX, IDC_EDIT_I_GAIN, m_fShuttleIGain);
	DDV_MinMaxFloat(pDX, m_fShuttleIGain, 0.0f, 5.0f);
	DDX_Text(pDX, IDC_EDIT_D_GAIN, m_fShuttleDGain);
	DDV_MinMaxFloat(pDX, m_fShuttleDGain, 0.0f, 5.0f);
}


BEGIN_MESSAGE_MAP(CPropPageConfigShuttle, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigShuttle message handlers
