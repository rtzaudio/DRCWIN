// PropPageConfigSMPTE.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigSMPTE.h"
#include "afxdialogex.h"


// CPropPageConfigSMPTE dialog

IMPLEMENT_DYNAMIC(CPropPageConfigSMPTE, CMFCPropertyPage)

CPropPageConfigSMPTE::CPropPageConfigSMPTE(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_SMPTE)
	, m_fRefClock(9600.0f)
	, m_fps(0)
{

}

CPropPageConfigSMPTE::~CPropPageConfigSMPTE()
{
}

void CPropPageConfigSMPTE::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REFCLK, m_fRefClock);
	DDV_MinMaxFloat(pDX, m_fRefClock, 2400.0f, 18000.0f);
	DDX_CBIndex(pDX, IDC_COMBO_FRAME_RATE, m_fps);
}


BEGIN_MESSAGE_MAP(CPropPageConfigSMPTE, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigSMPTE message handlers
