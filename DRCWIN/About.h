// about.h : header file
//
// Copyright (C) 2001 - 2002, Sigma Software Research
//
// Developed by Robert E. Starr, Jr. KG4LNE/WPSD889
//
// The source code contained in this file and associated files 
// is copyright protected material. This material contains the
// confidential, proprietary and trade secret information of
// Sigma Software Research. No disclosure or use of any portions
// of this material may be made without the express written consent
// of Sigma Software Research.
//  
/////////////////////////////////////////////////////////////////////////////

#ifndef __ABOUT_H___
#define __ABOUT_H___

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
// Construction
public:
    CAboutDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString m_strWarning;
	CString	m_strProductLicense;
	CString	m_strLicensedTo;
	//}}AFX_DATA

// Operations
public:
    void SetLicencedToText(CString str)
        { m_strLicensedTo = str; }

    // Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    //Helpers
    HFONT SetDlgItemFont(int nCtrlID, LPCTSTR lpszFontname, int nWeight, int nPointSize);
    HFONT ModifyDlgItemFont(int nCtrlID, int nWeight, int nPointSize);
    
public:
    BOOL GetVersion(DWORD *pdwVersionMS, DWORD *pdwVersionLS,
                    DWORD *pdwProductVersionMS, DWORD *pdwProductVersionLS);

    BOOL GetVersionString(CString& strVersion, CString& strRelease);
    BOOL GetVersionResource(LPCTSTR lpszStringName, LPTSTR lpszString, UINT uiMaxLen);

	BOOL GetComments(LPTSTR lpszComments, UINT uiMaxLen)
	{ return GetVersionResource(_T("Comments"), lpszComments, uiMaxLen); }

	BOOL GetCompanyName(LPTSTR lpszCompanyName, UINT uiMaxLen)
	{ return GetVersionResource(_T("CompanyName"), lpszCompanyName, uiMaxLen); }

	BOOL GetFileDescription(LPTSTR lpszFileDescription, UINT uiMaxLen)
	{ return GetVersionResource(_T("FileDescription"), lpszFileDescription, uiMaxLen); }

	BOOL GetFileVersion(LPTSTR lpszFileVersion, UINT uiMaxLen)
	{ return GetVersionResource(_T("FileVersion"), lpszFileVersion, uiMaxLen); }

	BOOL GetInternalName(LPTSTR lpszInternalName, UINT uiMaxLen)
	{ return GetVersionResource(_T("InternalName"), lpszInternalName, uiMaxLen); }

	BOOL GetLegalCopyright(LPTSTR lpszLegalCopyright, UINT uiMaxLen)
	{ return GetVersionResource(_T("LegalCopyright"), lpszLegalCopyright, uiMaxLen); }

	BOOL GetLegalTrademarks(LPTSTR lpszLegalTrademarks, UINT uiMaxLen)
	{ return GetVersionResource(_T("LegalTrademarks"), lpszLegalTrademarks, uiMaxLen); }

	BOOL GetProductName(LPTSTR lpszProductName, UINT uiMaxLen)
	{ return GetVersionResource(_T("ProductName"), lpszProductName, uiMaxLen); }

    // Generated message map functions
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
};

#endif //__ABOUT_H___

