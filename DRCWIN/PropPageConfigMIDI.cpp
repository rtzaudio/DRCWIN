// PropPageConfigMIDI.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigMIDI.h"
#include "afxdialogex.h"

// CPropPageConfigMIDI dialog

IMPLEMENT_DYNAMIC(CPropPageConfigMIDI, CMFCPropertyPage)

CPropPageConfigMIDI::CPropPageConfigMIDI(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_MIDI)
	, m_nDeviceID(0)
{

}

CPropPageConfigMIDI::~CPropPageConfigMIDI()
{
}

void CPropPageConfigMIDI::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEVID, m_nDeviceID);
	DDV_MinMaxInt(pDX, m_nDeviceID, 0, 255);
}


BEGIN_MESSAGE_MAP(CPropPageConfigMIDI, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigMIDI message handlers
