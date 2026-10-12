// PropPageServoGain.cpp : implementation file
//

#include "pch.h"
#include "DRCWIN.h"
#include "PropPageServoGain.h"
#include "afxdialogex.h"


// CPropPageServoGain dialog

IMPLEMENT_DYNAMIC(CPropPageServoGain, CMFCPropertyPage)

CPropPageServoGain::CPropPageServoGain(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_GAINS, pParent)
{

}

CPropPageServoGain::~CPropPageServoGain()
{
}

void CPropPageServoGain::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropPageServoGain, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageServoGain message handlers
