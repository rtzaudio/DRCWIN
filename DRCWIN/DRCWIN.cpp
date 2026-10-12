
// DRCWIN.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DRCWIN.h"
#include "MainFrm.h"
#include "DRCWINView.h"
#include "DRCWINDoc.h"
#include "About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR g_szSettingsSection[] = _T("Settings");

TCHAR g_szIPAddr[]         = _T("IPAddress");
TCHAR g_szPortNum[]        = _T("PortNumber");
TCHAR g_szLibWind[]        = _T("LibWind");
TCHAR g_szAutoPlay[]       = _T("AutoPlay");
TCHAR g_szAutoConnect[]    = _T("AutoConnect");
TCHAR g_szShowCmd[]        = _T("ShowCmd");
TCHAR g_szShowTracks[]     = _T("ShowTracks");
TCHAR g_szShowTimecode[]   = _T("ShowTimecode");
TCHAR g_szShowVelocity[]   = _T("ShowVelocity");
TCHAR g_szShowTimeDate[]   = _T("ShowTimeDate");
TCHAR g_szTrackCount[]     = _T("TrackCount");
TCHAR g_szConfirmReset[]   = _T("ConfirmZeroReset");
TCHAR g_szUseMachineTime[] = _T("UseMachineTime");

// Default static system color assignment table data

static COLORREF g_SysColors[] =
{
    { RGB(0x3C,0x3C,0x3C)   },  // COLOR_SCOPE_BACKGND
    { RGB(0x69,0x69,0x69)   },  // COLOR_SCOPE_GRID
    { RGB(0x87,0x87,0x97)   },  // COLOR_SCOPE_AXIS
    { RGB(0x32,0xCD,0x32)   },  // COLOR_SCOPE_WAVE
    { RGB(0xFF,0x00,0x00)   },  // COLOR_SCOPE_TRIGGER
    { RGB(0xFF,0xFF,0xFF)   },  // COLOR_SCOPE_TEXT
    { COLOR_BLACK           },  // COLOR_ANALYZER_BACKGND
    { RGB(0x69,0x69,0x69)   },  // COLOR_ANALYZER_GRID
    { RGB(0x87,0x87,0x97)   },  // COLOR_ANALYZER_LABELS
    { RGB(0x32,0xCD,0x32)   },  // COLOR_ANALYZER_WAVE
    { COLOR_RED             },  // COLOR_ANALYZER_PEAK
    { RGB(0xFF,0xFF,0xFF)   },  // COLOR_ANALYZER_TEXT
    { COLOR_DARKGRAY        },  // COLOR_VU_BACKGND
    { COLOR_DARKGREEN       },  // COLOR_VU_SEGS_NORM
    { COLOR_YELLOW          },  // COLOR_VU_SEGS_HIGH
    { COLOR_RED             },  // COLOR_VU_SEGS_MAX
    { COLOR_DARKGRAY        },  // COLOR_PEAK_BACKGND
    { COLOR_DARKGREEN       },  // COLOR_PEAK_SEGS_NORM
    { COLOR_YELLOW          },  // COLOR_PEAK_SEGS_HIGH
    { COLOR_RED             },  // COLOR_PEAK_SEGS_MAX
};

const int MAX_COLORREF = (sizeof(g_SysColors) / sizeof(COLORREF));

////////////////////////////////////////////////////////////////////////////
// CDRCWINApp

BEGIN_MESSAGE_MAP(CDRCWINApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDRCWINApp::OnAppAbout)
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CDRCWINApp construction

CDRCWINApp::CDRCWINApp()
	: CWinAppEx()
{
    m_bShowTracks = TRUE;
    m_bLibWind = FALSE;
    m_bAutoPlay = FALSE;
    m_bAutoConnect = FALSE;
    m_nShowVelocity = TRUE;
    m_bShowTimecode = TRUE;
    m_bShowTimeDate = TRUE;
    m_bUseTapeMachineTime = FALSE;
    m_bConfirmZeroReset = TRUE;
    m_trackCount = STC_MAX_TRACKS;
    m_dcsFound = FALSE;
    m_nAppTheme = 0;

//#ifdef _DEBUG
//	_CrtSetDbgFlag(1);
//#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DRCWIN.AppID.NoVersion"));
}

// The one and only CDRCWINApp object

CDRCWINApp theApp;

////////////////////////////////////////////////////////////////////////////
// CDRCWINApp initialization

BOOL CDRCWINApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

    EnableTaskbarInteraction(FALSE);

    // AfxInitRichEdit2() is required to use RichEdit control	
    // AfxInitRichEdit2();

    // Initialize windows sockets
    WSADATA wsaData;
	if (!AfxSocketInit(&wsaData))
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
    SetRegistryKey(_T("RTZ Professional Audio"));
    
    // Load standard INI file options (including MRU)    
    LoadStdProfileSettings(4);

    InitContextMenuManager();
    InitKeyboardManager();
    InitTooltipManager();
    CMFCToolTipInfo ttParams;
    ttParams.m_bVislManagerTheme = TRUE;
    theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
        RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CDRCWINDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CDRCWINView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    // call DragAcceptFiles only if there's a suffix
    //  In an SDI app, this should occur after ProcessShellCommand
    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

void CDRCWINApp::PreLoadState()
{
}

void CDRCWINApp::LoadCustomState()
{
    m_bAutoConnect = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szAutoConnect, 0);
    m_bAutoPlay = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szAutoPlay, 0);
    m_bLibWind = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szLibWind, 0);
    m_bShowTracks = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szShowTracks, 1);
    m_bShowTimecode = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szShowTimecode, 0);
    m_nShowVelocity = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szShowVelocity, 2);
    m_bShowTimeDate = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szShowTimeDate, 1);
    m_bUseTapeMachineTime = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szUseMachineTime, 1);

    m_bConfirmZeroReset = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szConfirmReset, 1);
    m_trackCount = (size_t)AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szTrackCount, 24);

    if ((m_trackCount < 8) || (m_trackCount > 24))
        m_trackCount = 24;
}

void CDRCWINApp::SaveCustomState()
{
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szAutoConnect, m_bAutoConnect);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szAutoPlay, m_bAutoPlay);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szLibWind, m_bLibWind);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szShowTracks, m_bShowTracks);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szShowTimecode, m_bShowTimecode);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szShowVelocity, m_nShowVelocity);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szShowTimeDate, m_bShowTimeDate);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szUseMachineTime, m_bUseTapeMachineTime);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szConfirmReset, m_bConfirmZeroReset);
    AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szTrackCount, (int)m_trackCount);
}

int CDRCWINApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinAppEx::ExitInstance();
}

// Show error message string with the WIN32 error string 
// concatenated in a message box.

int CDRCWINApp::ShowSystemError(CString strMessage, DWORD dwError, UINT nIDHelp)
{
    // Get the Win32 system error message string.
    LPTSTR pszSystemError = NULL;

    DWORD dwLength = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                     FORMAT_MESSAGE_FROM_SYSTEM,
                                     NULL,
                                     dwError,
                                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                     (LPTSTR)&pszSystemError,
                                     0,
                                     NULL);
    CString str;

    if (dwLength && pszSystemError)
        str.Format(_T("%s\n\nError: %s"), strMessage.GetBuffer(), pszSystemError);
    else
        str.Format(_T("%s\n\nError: 0x%X"), strMessage.GetBuffer(), dwError);

	strMessage.ReleaseBuffer();

    return AfxMessageBox(str, nIDHelp);		// MB_OK|MB_ICONSTOP);
}

int CDRCWINApp::ShowSystemError(INT nMessageID, DWORD dwError, UINT nIDHelp)
{
    CString str;
    CString strMessage;

    if (!strMessage.LoadString(nMessageID))
        strMessage = _T("Internal Error Loading Message Text!");

    // Get the Win32 system error message string.
    LPTSTR pszSystemError = NULL;

    DWORD dwLength = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszSystemError,
        0,
        NULL);

    if (dwLength && pszSystemError)
        str.Format(_T("%s\n\nError: %s"), strMessage, pszSystemError);
    else
        str.Format(_T("%s\n\nError: 0x%X"), strMessage, dwError);
    
    LocalFree(pszSystemError);

    return AfxMessageBox(str, nIDHelp);		// MB_OK|MB_ICONSTOP);
}

////////////////////////////////////////////////////////////////////////////
// CDRCWINApp message handlers

// App command to run the dialog
void CDRCWINApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDRCWINApp message handlers


////////////////////////////////////////////////////////////////////////////
// GLOBAL HELPER FUNCTIONS

// Global System Color Helpers

COLORREF GetSystemColor(int nColorIndex)
{
    if (nColorIndex >= MAX_COLORREF)
        return COLOR_LIGHTGRAY;

    return g_SysColors[nColorIndex];
}

BOOL SetSystemColor(int nColorIndex, COLORREF color)
{
    if (nColorIndex >= MAX_COLORREF)
        return FALSE;

    g_SysColors[nColorIndex] = color;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////
// Load or save the current system color settings to the registry

TCHAR szColorSection[] = _T("Colors");

void SaveSystemColorSettings()
{
    for (int i=0; i < MAX_COLORREF; i++)
    {
        CString str;
        str.Format(_T("Color%u"), i);
        theApp.WriteSectionBinary(szColorSection, str, (LPBYTE)&g_SysColors[i], sizeof(COLORREF));
    }
}

void LoadSystemColorSettings()
{
    // Default signal generator control values
    for (int i=0; i < MAX_COLORREF; i++)
    {
        UINT uBytes;
        LPBYTE ppData = NULL;
        CString str;
        str.Format(_T("Color%u"), i);
        if (theApp.GetSectionBinary(szColorSection, str, &ppData, &uBytes))
        {
            if (ppData)
            {
                g_SysColors[i]= *((COLORREF*)ppData);
                delete ppData;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////
// Registry Helpers

BOOL CDRCWINApp::WriteSectionFloat(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, float val)
{
    return WriteSectionBinary(lpszSubSection, lpszEntry, (LPBYTE)&val, sizeof(float));
}

float CDRCWINApp::GetSectionFloat( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, float val)
{
    UINT uBytes;
    LPBYTE ppData = NULL;

    if (GetSectionBinary(lpszSubSection, lpszEntry, &ppData, &uBytes))
    {
        if ((ppData) && (uBytes == sizeof(float)))
        {
            val = *((float*)ppData);
            delete ppData;
        }
    }
    
    return val;
}

BOOL CDRCWINApp::WriteSectionDouble(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, double val)
{
    return WriteSectionBinary(lpszSubSection, lpszEntry, (LPBYTE)&val, sizeof(double));
}

double CDRCWINApp::GetSectionDouble( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, double val)
{
    UINT uBytes;
    LPBYTE ppData = NULL;

    if (GetSectionBinary(lpszSubSection, lpszEntry, &ppData, &uBytes))
    {
        if ((ppData) && (uBytes == sizeof(double)))
        {
            val = *((double*)ppData);
            delete ppData;
        }
    }
    
    return val;
}
