// PropPageConfigTransport.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigTransport.h"
#include "afxdialogex.h"


// CPropPageConfigTransport dialog

IMPLEMENT_DYNAMIC(CPropPageConfigTransport, CMFCPropertyPage)

CPropPageConfigTransport::CPropPageConfigTransport(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_TRANSPORT)
	, m_nVelocityDetectThreshold(0)
	, m_nRecordStrobePulseTime(0)
	, m_nRecordHoldSettleTime(0)
	, m_nButtonDebounceTime(0)
{

}

CPropPageConfigTransport::~CPropPageConfigTransport()
{
}

void CPropPageConfigTransport::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VEL_DETECT_THRESHOLD, m_nVelocityDetectThreshold);
	DDV_MinMaxInt(pDX, m_nVelocityDetectThreshold, 1, 50);
	DDX_Text(pDX, IDC_EDIT_REC_PULSE_TIME, m_nRecordStrobePulseTime);
	DDV_MinMaxInt(pDX, m_nRecordStrobePulseTime, 10, 100);
	DDX_Text(pDX, IDC_EDIT_REC_HOLD_SETTLE_TIME, m_nRecordHoldSettleTime);
	DDV_MinMaxInt(pDX, m_nRecordHoldSettleTime, 5, 20);
	DDX_Text(pDX, IDC_EDIT_BUTTON_DEBOUNCE_TIME, m_nButtonDebounceTime);
	DDV_MinMaxInt(pDX, m_nButtonDebounceTime, 5, 50);
}


BEGIN_MESSAGE_MAP(CPropPageConfigTransport, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigTransport message handlers
