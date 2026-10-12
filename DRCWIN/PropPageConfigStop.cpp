// PropPageConfigStop.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigStop.h"
#include "afxdialogex.h"


// CPropPageConfigStop dialog

IMPLEMENT_DYNAMIC(CPropPageConfigStop, CMFCPropertyPage)

CPropPageConfigStop::CPropPageConfigStop(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_STOP)
	, m_nStopBrakeTorque(0)
	, m_bLiftersAtStop(FALSE)
	, m_BrakesAtStop(FALSE)
	, m_bEndOfTapeDetect(FALSE)
{

}

CPropPageConfigStop::~CPropPageConfigStop()
{
}

void CPropPageConfigStop::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STOP_BRAKE_TORQUE, m_nStopBrakeTorque);
	DDV_MinMaxInt(pDX, m_nStopBrakeTorque, 300, 900);
	DDX_Check(pDX, IDC_CHECK_LIFTERS_AT_STOP, m_bLiftersAtStop);
	DDX_Check(pDX, IDC_CHECK_BRAKES_AT_STOP, m_BrakesAtStop);
	DDX_Check(pDX, IDC_CHECK_STOP_AT_EOT, m_bEndOfTapeDetect);
}


BEGIN_MESSAGE_MAP(CPropPageConfigStop, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigStop message handlers
