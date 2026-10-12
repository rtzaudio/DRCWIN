// PropPageSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageSettings.h"
#include "afxdialogex.h"


// CPropPageSettings dialog

IMPLEMENT_DYNAMIC(CPropPageSettings, CMFCPropertyPage)

CPropPageSettings::CPropPageSettings()
	: CMFCPropertyPage(IDD_PROPPAGE_SETTINGS)
	, m_nTheme(0)
	, m_bShowTracks(FALSE)
	, m_bShowVelocity(FALSE)
	, m_bShowTimecode(FALSE)
	, m_bShowTime(FALSE)
	, m_bConfirmZeroReset(FALSE)
	, m_bUseTapeMachineTime(FALSE)
{

}

CPropPageSettings::~CPropPageSettings()
{
}

void CPropPageSettings::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_THEME, m_nTheme);
	DDX_Check(pDX, IDC_CHECK_SHOW_TRACKS, m_bShowTracks);
	DDX_Check(pDX, IDC_CHECK_SHOW_VELOCITY, m_bShowVelocity);
	DDX_Check(pDX, IDC_CHECK_SHOW_TIMECODE, m_bShowTimecode);
	DDX_Check(pDX, IDC_CHECK_SHOW_TIME_DATE, m_bShowTime);
	DDX_Check(pDX, IDC_CHECK_CONFIRM_ZERO_RESET, m_bConfirmZeroReset);
	DDX_Check(pDX, IDC_CHECK_USE_TAPE_MACHINE_TIME, m_bUseTapeMachineTime);
}

BEGIN_MESSAGE_MAP(CPropPageSettings, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageSettings message handlers




BOOL CPropPageSettings::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	return TRUE;
}
