// CPropPageConfigGlobal.cpp : implementation file
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "PropPageConfigGlobal.h"
#include "afxdialogex.h"


// CPropPageConfigGlobal dialog

IMPLEMENT_DYNAMIC(CPropPageConfigGlobal, CPropertyPage)

CPropPageConfigGlobal::CPropPageConfigGlobal(CWnd* pParent /*=nullptr*/)
	: CPropertyPage(CPropPageConfigGlobal::IDD)
{

}

CPropPageConfigGlobal::~CPropPageConfigGlobal()
{
}

void CPropPageConfigGlobal::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropPageConfigGlobal, CPropertyPage)
END_MESSAGE_MAP()


// CPropPageConfigGlobal message handlers
