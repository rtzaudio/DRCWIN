// PropPageConfigLocator.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigLocator.h"
#include "afxdialogex.h"


// CPropPageConfigLocator dialog

IMPLEMENT_DYNAMIC(CPropPageConfigLocator, CMFCPropertyPage)

CPropPageConfigLocator::CPropPageConfigLocator(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_CONFIG_LOCATOR)
	, m_nJogNear(0)
	, m_nJogMid(0)
	, m_nJogFar(0)
{

}

CPropPageConfigLocator::~CPropPageConfigLocator()
{
}

void CPropPageConfigLocator::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JOG_NEAR, m_nJogNear);
	DDV_MinMaxInt(pDX, m_nJogNear, 0, 400);
	DDX_Text(pDX, IDC_EDIT_JOG_MID, m_nJogMid);
	DDV_MinMaxInt(pDX, m_nJogMid, 0, 800);
	DDX_Text(pDX, IDC_EDIT_JOG_FAR, m_nJogFar);
	DDV_MinMaxInt(pDX, m_nJogFar, 0, 1000);
}


BEGIN_MESSAGE_MAP(CPropPageConfigLocator, CMFCPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigLocator message handlers
