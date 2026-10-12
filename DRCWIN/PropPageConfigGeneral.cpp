// PropPageConfigGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigGeneral.h"
#include "afxdialogex.h"


// CPropPageConfigGeneral dialog

IMPLEMENT_DYNAMIC(CPropPageConfigGeneral, CMFCPropertyPage)

CPropPageConfigGeneral::CPropPageConfigGeneral(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_GENERAL)
	, m_nShowTimecode(FALSE)
	, m_nBlink7Seg(FALSE)
	, m_strSTCVersion(_T(""))
	, m_strDTCVersion(_T(""))
	, m_stcSN(_T(""))
	, m_stcMAC(_T(""))
	, m_dtcSN(_T(""))
{

}

CPropPageConfigGeneral::~CPropPageConfigGeneral()
{
}

void CPropPageConfigGeneral::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SHOW_TIMECODE, m_nShowTimecode);
	DDX_Check(pDX, IDC_CHECK_BLINK_7SEG, m_nBlink7Seg);
	DDX_Text(pDX, IDC_STATIC_STC_VERSION, m_strSTCVersion);
	DDX_Text(pDX, IDC_STATIC_DTC_VERSION, m_strDTCVersion);
	DDX_Text(pDX, IDC_STATIC_STC_SN, m_stcSN);
	DDX_Text(pDX, IDC_STATIC_STC_MAC, m_stcMAC);
	DDX_Text(pDX, IDC_STATIC_DTC_SN, m_dtcSN);
}


BEGIN_MESSAGE_MAP(CPropPageConfigGeneral, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigGeneral message handlers
