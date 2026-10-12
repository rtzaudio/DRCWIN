// <about.cpp> : implementation file
//
// Copyright (C) 2001-2023, RTZ Professional Audio 
//
// Developed by Robert E. Starr, Jr.
//
// The source code contained in this file and associated files is
// copyright protected material. This material contains the confidential, 
// proprietary and trade secret information of RTZ Professional Audio, LLC.
// No disclosure or use of any portions of this material may be made 
// without the express written consent of RTZ Professional Audio, LLC.
//
/////////////////////////////////////////////////////////////////////////////
//
// (res) v1.01  This code implements a standard about dialog box
//              for Win16 and Win32 MFC applications. The only resources
//              required are the IDR_MAINFRAME ICON resource and the dialog
//              box template IDD_ABOUTBOX. The dialog box itself contains
//              several sprintf() style formatting strings used to format
//              the application version, product version, and build release.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "About.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// Enabling this define cause the about code to use VerInfo.InternalName
// field in the about box window title. Otherwise, the predefined MFC string
// constant AFX_IDS_APP_TITLE is used to get the application name for the
// title

#define USE_INTERNAL_NAME_IN_TITLE  0

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
    ON_WM_PAINT()
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAboutDlg::IDD, pParent)
    , m_strWarning(_T(""))
{
    //{{AFX_DATA_INIT(CAboutDlg)
	m_strLicensedTo = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_ABOUT_WARNING, m_strWarning);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    TCHAR szText[1024];

    // Center the dialog in the application windo
    CenterWindow();

    // Set the about window dialog title to include the app-name
#if (USE_INTERNAL_NAME_IN_TITLE > 0)
    GetInternalName(szText, sizeof(szText)/sizeof(TCHAR));
    CString strTitle;
    GetWindowText(strTitle);
    SetWindowText(strTitle + _T(" ") + szText);
#else
    CString strTitle;
    CString strAppName;
    GetWindowText(strTitle);
	int rc = strAppName.LoadString(AFX_IDS_APP_TITLE);
	if (rc)
		SetWindowText(strTitle + _T(" ") + strAppName);
#endif

    // Set the application name from the file description resource.
    GetFileDescription(szText, sizeof(szText)/sizeof(TCHAR));
    SetDlgItemText(IDC_ABOUT_FILE_DESCRIPTION, szText);

    // Get the copyright from the resource file and set in the dialog.
    GetLegalCopyright(szText, sizeof(szText)/sizeof(TCHAR));
    SetDlgItemText(IDC_ABOUT_COPYRIGHT, szText);

    // Format the app version string and set it
    CString strTemp;
    CString strVersion;
    CString strRelease;
    GetVersionString(strVersion, strRelease);
    strTemp.Format(_T("Version %s"), strVersion.GetBuffer());
    SetDlgItemText(IDC_ABOUT_VERSION, strTemp);

    // Format the app release string and set it
    strTemp.Format(_T("Product Release %s"), strRelease.GetBuffer());
    SetDlgItemText(IDC_ABOUT_RELEASE, strTemp);

    // Change the font type on some of the controls
    ModifyDlgItemFont(IDC_ABOUT_FILE_DESCRIPTION, FW_BOLD, 0);
    //ModifyDlgItemFont(IDC_STATIC_LICENSED_TO_HDR, FW_BOLD, 0);
    SetDlgItemFont(IDC_ABOUT_WARNING, _T("Arial"), FW_THIN, 7);
    
    if (m_strWarning.GetLength() == 0)
    {
        rc = m_strWarning.LoadString(IDS_ABOUT_WARNING);
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAboutDlg::OnPaint()
{
    CPaintDC dc(this);
#if 0
    CRect rcCtrl;
    CWnd* pCtrl = GetDlgItem(IDC_MM1200_BITMAP);
    pCtrl->GetWindowRect(&rcCtrl);
    ScreenToClient(&rcCtrl);

    CRect rcDraw = rcCtrl;
    COLORREF clr = GetSysColor(COLOR_BTNSHADOW);
    InflateRect(&rcDraw, 1, 1);
    //OffsetRect(&rcDraw, 4, 4);
#if 1
    dc.Draw3dRect(&rcDraw, clr, clr);
#else
    CBrush brush;
    brush.CreateSolidBrush(clr);
    // Save original brush.
    CBrush* pOrigBrush = (CBrush*)dc.SelectObject(&brush);
    //dc.FillRect(rcDraw, &brush);
    dc.Rectangle(rcDraw);
    // Restore original brush.
    dc.SelectObject(pOrigBrush);
#endif
#endif
}

BOOL
CAboutDlg::GetVersionString(
    CString& strVersion,
    CString& strRelease)
{
    DWORD dwVersionMS;
    DWORD dwVersionLS;
    DWORD dwProductVersionMS;
    DWORD dwProductVersionLS;

    if (!GetVersion(&dwVersionMS,
                    &dwVersionLS,
                    &dwProductVersionMS,
                    &dwProductVersionLS))
    {
        return FALSE;
    }

#ifdef _WIN64
    // Format the version string
    strVersion.Format(_T("%u.%u.%u (Build %u) 64-Bit"),
                      HIWORD(dwVersionMS), LOWORD(dwVersionMS),
                      HIWORD(dwVersionLS), LOWORD(dwVersionLS));
#else
    // Format the version string
    strVersion.Format(_T("%u.%u.%u (Build %u) 32-Bit"),
        HIWORD(dwVersionMS), LOWORD(dwVersionMS),
        HIWORD(dwVersionLS), LOWORD(dwVersionLS));
#endif

    // Format the release string
    strRelease.Format(_T("%u.%u"),
                      HIWORD(dwProductVersionMS),
                      LOWORD(dwProductVersionMS));
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Read the program copyright info from resource image

/////////////////////////////////////////////////////////////////////////////
// Read the program version string data from the resource image.

BOOL CAboutDlg::GetVersionResource( LPCTSTR lpszStringName, LPTSTR lpszString, UINT uiMaxLen )
{
    TCHAR       szFileName[1024];
    TCHAR       sz[1024];
    LPVOID      lpBuffer;
    HINSTANCE   hInst;
    DWORD       dwSize;
    DWORD       dwHandle;
    UINT        uiLen;
    DWORD       dwLanguage;
    HANDLE      hVerInfo;
    LPVOID      lpVerInfo;

    /*-- Read the product version info from the resource file --*/
#ifdef _WIN32
    hInst = NULL;
#else
    hInst = AfxGetInstanceHandle();
#endif

    if (GetModuleFileName(hInst, szFileName, _countof(szFileName)) == 0)
        return FALSE;

    if ((dwSize = GetFileVersionInfoSize(szFileName, &dwHandle)) == 0L)
        return FALSE;

    if ((hVerInfo = GlobalAlloc(GMEM_MOVEABLE|GMEM_DISCARDABLE|GMEM_ZEROINIT, dwSize)) == NULL)
        return FALSE;

    if ((lpVerInfo = GlobalLock(hVerInfo)) == NULL) {
        GlobalFree(hVerInfo);
        return FALSE;
    }

    if (!GetFileVersionInfo(szFileName, 0, dwSize, lpVerInfo)) {
        GlobalUnlock(hVerInfo);
        GlobalFree(hVerInfo);
        return FALSE;
    }

    wsprintf( sz, _T("\\VarFileInfo\\Translation") );
    if ( !VerQueryValue(lpVerInfo, sz, &lpBuffer, &uiLen) ) {
        GlobalUnlock(hVerInfo);
        GlobalFree(hVerInfo);
        return FALSE;
    }

    dwLanguage = MAKELONG( ((LPWORD)lpBuffer)[1], ((LPWORD)lpBuffer)[0] );

    wsprintf( sz, _T("\\StringFileInfo\\%08lX\\%s"),
              dwLanguage, lpszStringName );
    if ( !VerQueryValue(lpVerInfo, sz, &lpBuffer, &uiLen ) ) {
        GlobalUnlock(hVerInfo);
        GlobalFree(hVerInfo);
        return FALSE;
    }

    wcsncpy_s( lpszString, uiMaxLen-1, (LPTSTR)lpBuffer, uiMaxLen );
    lpszString[uiMaxLen-1] = (TCHAR)'\0';

    GlobalUnlock(hVerInfo);
    GlobalFree(hVerInfo);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Read the program DWORD version info from resource image

BOOL CAboutDlg::GetVersion(DWORD *pdwVersionMS, DWORD *pdwVersionLS,
                           DWORD *pdwProductVersionMS, DWORD *pdwProductVersionLS)
{
    TCHAR       szFileName[1024];
    TCHAR       sz[1024];
    LPVOID      lpBuffer;
    HINSTANCE   hInst;
    DWORD       dwSize;
    DWORD       dwHandle;
    UINT        uiLen;
    HANDLE      hVerInfo;
    LPVOID      lpVerInfo;
    VS_FIXEDFILEINFO FAR *lpFixedFileInfo;

    *pdwVersionMS = 0;
    *pdwVersionLS = 0;

    *pdwProductVersionMS = 0;
    *pdwProductVersionLS = 0;

    /*-- Read the product version info from the resource file --*/
#ifdef _WIN32
    hInst = NULL;
#else
    hInst = AfxGetInstanceHandle();
#endif

    if (GetModuleFileName(hInst, szFileName, _countof(szFileName)) == 0)
        return FALSE;

    if ((dwSize = GetFileVersionInfoSize(szFileName, &dwHandle)) == 0L)
        return FALSE;

    if ((hVerInfo = GlobalAlloc(GMEM_MOVEABLE|GMEM_DISCARDABLE|GMEM_ZEROINIT, dwSize)) == NULL)
        return FALSE;

    lpVerInfo = GlobalLock(hVerInfo);

	if (lpVerInfo != NULL)
	{
		GetFileVersionInfo(szFileName, 0, dwSize, lpVerInfo);

		wsprintf(sz, _T("\\"));
		VerQueryValue(lpVerInfo, sz, &lpBuffer, &uiLen);

		lpFixedFileInfo = (VS_FIXEDFILEINFO FAR*)lpBuffer;

		*pdwVersionMS = lpFixedFileInfo->dwFileVersionMS;
		*pdwVersionLS = lpFixedFileInfo->dwFileVersionLS;

		*pdwProductVersionMS = lpFixedFileInfo->dwProductVersionMS;
		*pdwProductVersionLS = lpFixedFileInfo->dwProductVersionLS;
	}

    GlobalUnlock(hVerInfo);
    GlobalFree(hVerInfo);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Change the font of a dialog item

HFONT CAboutDlg::SetDlgItemFont(int nCtrlID, LPCTSTR lpszFontname, int nWeight, int nPointSize)
{
    CDC*    pDC;
    HFONT   hFont;

    // get the window dc
    pDC = GetDC();

    int nFontHeight = -(nPointSize * pDC->GetDeviceCaps(LOGPIXELSY)) / 72;

    hFont = ::CreateFont(nFontHeight, 0, 0, 0, nWeight, (BYTE)0, (BYTE)0, (BYTE)0,
                         ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                         FF_DONTCARE, lpszFontname);
    ReleaseDC(pDC);
    
    if (hFont)
        SendDlgItemMessage(nCtrlID, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(1, 0));
                         
    return hFont;
}

/////////////////////////////////////////////////////////////////////////////
// Change the font attributes of a dialog box control 

HFONT CAboutDlg::ModifyDlgItemFont(int nCtrlID, int nWeight, int nPointSize)
{
	HFONT   hFont;
	LOGFONT lf;
	CDC* pDC;

	// Get current font and change the attributes

	if ((hFont = (HFONT)SendDlgItemMessage(nCtrlID, WM_GETFONT, 0, 0L)) != NULL)
	{
		pDC = GetDC();

		if (::GetObject(hFont, sizeof(LOGFONT), (LPSTR)&lf))
		{
			if (nWeight)
				lf.lfWeight = nWeight;

			if (nPointSize)
				lf.lfHeight += nPointSize;

			if ((hFont = ::CreateFontIndirect((LPLOGFONT)&lf)) != NULL)
			{
				SendDlgItemMessage(nCtrlID, WM_SETFONT, (WPARAM)hFont, (LPARAM)0L);
			}
		}

		ReleaseDC(pDC);
	}

	return hFont;
}