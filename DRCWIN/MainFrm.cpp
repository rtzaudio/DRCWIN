// =========================================================================
// MainFrm.cpp : implementation of the CMainFrame class
// =========================================================================

#include "stdafx.h"
#include "DRCWIN.h"

#include "MainFrm.h"
#include "DRCWINView.h"
#include "TcpConnectDlg.h"
#include "ConnectingDlg.h"
#include "EditTapeTimeDlg.h"
#include "EditSMPTETimeDlg.h"
#include "PropPageSettings.h"
#include "PropPageConfigGeneral.h"
#include "PropPageConfigLocator.h"
#include "PropPageConfigMIDI.h"
#include "PropPageConfigSMPTE.h"
#include "PropPageConfigShuttle.h"
#include "PropPageConfigPlay.h"
#include "PropPageConfigStop.h"
#include "PropPageConfigTensions.h"
#include "PropPageConfigTransport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern TCHAR g_szSettingsSection[];
extern TCHAR g_szShowCmd[];
extern TCHAR g_szIPAddr[];
extern TCHAR g_szPortNum[];
extern TCHAR g_szAutoConnect[];

static TCHAR BASED_CODE s_szCueFilter[] = {
    _T("Cue Point Memory (*.cpm)|*.cpm|")	\
    _T("All Files (*.*)|*.*|")              \
    _T("|")
};

// =========================================================================
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
ON_WM_CREATE()
ON_WM_SETFOCUS()
ON_WM_CLOSE()
ON_WM_SIZING()
ON_MESSAGE(WM_USER_MODECHANGE, OnTransportModeChange)
ON_MESSAGE(WM_USER_AUTOCONNECT, OnAutoConnect)
ON_COMMAND(ID_CONNECT_TCP, &CMainFrame::OnConnectTcp)
ON_UPDATE_COMMAND_UI(ID_CONNECT_TCP, &CMainFrame::OnUpdateConnectTcp)
ON_COMMAND(ID_CONNECT_DISCONNECT, &CMainFrame::OnConnectDisconnect)
ON_UPDATE_COMMAND_UI(ID_CONNECT_DISCONNECT, &CMainFrame::OnUpdateConnectDisconnect)
ON_COMMAND(ID_TRANSPORT_RECORD, &CMainFrame::OnTransportRecord)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_RECORD, &CMainFrame::OnUpdateTransportRecord)
ON_COMMAND(ID_TRANSPORT_REWIND, &CMainFrame::OnTransportRewind)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_REWIND, &CMainFrame::OnUpdateTransportRewind)
ON_COMMAND(ID_TRANSPORT_STOP, &CMainFrame::OnTransportStop)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_STOP, &CMainFrame::OnUpdateTransportStop)
ON_COMMAND(ID_TRANSPORT_PLAY, &CMainFrame::OnTransportPlay)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_PLAY, &CMainFrame::OnUpdateTransportPlay)
ON_COMMAND(ID_TRANSPORT_FORWARD, &CMainFrame::OnTransportForward)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_FORWARD, &CMainFrame::OnUpdateTransportForward)
ON_COMMAND(ID_TRANSPORT_TAPELIFTER, &CMainFrame::OnTransportTapelifter)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_TAPELIFTER, &CMainFrame::OnUpdateTransportTapelifter)
ON_UPDATE_COMMAND_UI(ID_INDICATOR_CONNECTION, &CMainFrame::OnUpdateIndicatorConnection)
//ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, &CMainFrame::OnUpdateIndicatorMode)
ON_COMMAND(ID_CUEPOINT_LOC0, &CMainFrame::OnCuePointLoc0)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC0, &CMainFrame::OnUpdateCuePointLoc0)
ON_COMMAND(ID_CUEPOINT_LOC1, &CMainFrame::OnCuePointLoc1)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC1, &CMainFrame::OnUpdateCuePointLoc1)
ON_COMMAND(ID_CUEPOINT_LOC2, &CMainFrame::OnCuePointLoc2)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC2, &CMainFrame::OnUpdateCuePointLoc2)
ON_COMMAND(ID_CUEPOINT_LOC3, &CMainFrame::OnCuePointLoc3)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC3, &CMainFrame::OnUpdateCuePointLoc3)
ON_COMMAND(ID_CUEPOINT_LOC4, &CMainFrame::OnCuePointLoc4)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC4, &CMainFrame::OnUpdateCuePointLoc4)
ON_COMMAND(ID_CUEPOINT_LOC5, &CMainFrame::OnCuePointLoc5)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC5, &CMainFrame::OnUpdateCuePointLoc5)
ON_COMMAND(ID_CUEPOINT_LOC6, &CMainFrame::OnCuePointLoc6)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC6, &CMainFrame::OnUpdateCuePointLoc6)
ON_COMMAND(ID_CUEPOINT_LOC7, &CMainFrame::OnCuePointLoc7)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC7, &CMainFrame::OnUpdateCuePointLoc7)
ON_COMMAND(ID_CUEPOINT_LOC8, &CMainFrame::OnCuePointLoc8)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC8, &CMainFrame::OnUpdateCuePointLoc8)
ON_COMMAND(ID_CUEPOINT_LOC9, &CMainFrame::OnCuePointLoc9)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_LOC9, &CMainFrame::OnUpdateCuePointLoc9)
ON_COMMAND(ID_CUEPOINT_MARKIN, &CMainFrame::OnCueMarkIn)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_MARKIN, &CMainFrame::OnUpdateCueMarkIn)
ON_COMMAND(ID_CUEPOINT_MARKOUT, &CMainFrame::OnCueMarkOut)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_MARKOUT, &CMainFrame::OnUpdateCueMarkOut)
ON_COMMAND(ID_CUEPOINT_PUNCHIN, &CMainFrame::OnCuePunchIn)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_PUNCHIN, &CMainFrame::OnUpdateCuePunchIn)
ON_COMMAND(ID_CUEPOINT_PUNCHOUT, &CMainFrame::OnCuePunchOut)
ON_UPDATE_COMMAND_UI(ID_CUEPOINT_PUNCHOUT, &CMainFrame::OnUpdateCuePunchOut)
ON_COMMAND(ID_VIEW_TRANSPORTTOOLBAR, &CMainFrame::OnViewTransporttoolbar)
ON_COMMAND(ID_VIEW_LOCATORTOOLBAR, &CMainFrame::OnViewLocatortoolbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_LOCATORTOOLBAR, &CMainFrame::OnUpdateViewLocatortoolbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_TRANSPORTTOOLBAR, &CMainFrame::OnUpdateViewTransporttoolbar)
ON_COMMAND(ID_MEMORY_CLEARALL, &CMainFrame::OnMemoryClearall)
ON_UPDATE_COMMAND_UI(ID_MEMORY_CLEARALL, &CMainFrame::OnUpdateMemoryClearall)
ON_COMMAND_RANGE(ID_RECORD_TRACK1, ID_RECORD_TRACK24, &CMainFrame::OnRangeRecordTrack)
ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_TRACK1, ID_RECORD_TRACK24, &CMainFrame::OnUpdateRangeRecordTrack)
ON_COMMAND_RANGE(ID_INPUT_TRACK1, ID_INPUT_TRACK24, &CMainFrame::OnRangeInputTrack)
ON_UPDATE_COMMAND_UI_RANGE(ID_INPUT_TRACK1, ID_INPUT_TRACK24, &CMainFrame::OnUpdateRangeInputTrack)
ON_COMMAND_RANGE(ID_REPRO_TRACK1, ID_REPRO_TRACK24, &CMainFrame::OnRangeReproTrack)
ON_UPDATE_COMMAND_UI_RANGE(ID_REPRO_TRACK1, ID_REPRO_TRACK24, &CMainFrame::OnUpdateRangeReproTrack)
ON_COMMAND_RANGE(ID_SYNC_TRACK1, ID_SYNC_TRACK24, &CMainFrame::OnRangeSyncTrack)
ON_UPDATE_COMMAND_UI_RANGE(ID_SYNC_TRACK1, ID_SYNC_TRACK24, &CMainFrame::OnUpdateRangeSyncTrack)
ON_COMMAND_RANGE(ID_MONITOR_TRACK1, ID_MONITOR_TRACK24, &CMainFrame::OnRangeMonitorTrack)
ON_UPDATE_COMMAND_UI_RANGE(ID_MONITOR_TRACK1, ID_MONITOR_TRACK24, &CMainFrame::OnUpdateRangeMonitorTrack)
ON_COMMAND_RANGE(ID_TOGGLEMODE_TRACK1, ID_TOGGLEMODE_TRACK24, &CMainFrame::OnRangeTrackToggleMode)
ON_UPDATE_COMMAND_UI_RANGE(ID_TOGGLEMODE_TRACK1, ID_TOGGLEMODE_TRACK24, &CMainFrame::OnUpdateRangeTrackToggleMode)
ON_COMMAND(ID_TRACK_ALLSAFE, &CMainFrame::OnTrackAllSafe)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLSAFE, &CMainFrame::OnUpdateTrackAllSafe)
ON_COMMAND(ID_TRACK_ALLREADY, &CMainFrame::OnTrackAllReady)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLREADY, &CMainFrame::OnUpdateTrackAllReady)
ON_COMMAND(ID_TRACK_ALLINPUT, &CMainFrame::OnTrackAllInput)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLINPUT, &CMainFrame::OnUpdateTrackAllInput)
ON_COMMAND(ID_TRACK_ALLREPRO, &CMainFrame::OnTrackAllRepro)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLREPRO, &CMainFrame::OnUpdateTrackAllRepro)
ON_COMMAND(ID_TRACK_ALLSYNC, &CMainFrame::OnTrackAllSync)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLSYNC, &CMainFrame::OnUpdateTrackAllSync)
ON_COMMAND(ID_TRACK_ALLMONITOROFF, &CMainFrame::OnTrackAllMonitorOff)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLMONITOROFF, &CMainFrame::OnUpdateTrackAllMonitorOff)
ON_COMMAND(ID_TRACK_ALLMONITORON, &CMainFrame::OnTrackAllMonitorOn)
ON_UPDATE_COMMAND_UI(ID_TRACK_ALLMONITORON, &CMainFrame::OnUpdateTrackAllMonitorOn)
ON_COMMAND(ID_LOCATOR_RESETCOUNTER, &CMainFrame::OnLocatorResetCounter)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_RESETCOUNTER, &CMainFrame::OnUpdateLocatorResetCounter)
ON_COMMAND(ID_LOCATOR_CUEMODE, &CMainFrame::OnLocatorCueMode)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_CUEMODE, &CMainFrame::OnUpdateLocatorCueMode)
ON_COMMAND(ID_LOCATOR_STOREMODE, &CMainFrame::OnLocatorStoreMode)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_STOREMODE, &CMainFrame::OnUpdateLocatorStoreMode)
ON_COMMAND(ID_LOCATOR_AUTOPLAY, &CMainFrame::OnLocatorAutoPlay)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_AUTOPLAY, &CMainFrame::OnUpdateLocatorAutoPlay)
ON_COMMAND(ID_TRANSPORT_LIBRARYWIND, &CMainFrame::OnTransportLibraryWind)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_LIBRARYWIND, &CMainFrame::OnUpdateTransportLibrarywind)
ON_COMMAND(ID_LOCATOR_CANCELSEARCH, &CMainFrame::OnLocatorCancelsearch)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_CANCELSEARCH, &CMainFrame::OnUpdateLocatorCancelsearch)
ON_COMMAND(ID_LOCATOR_AUTOLOOP, &CMainFrame::OnLocatorAutoLoop)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_AUTOLOOP, &CMainFrame::OnUpdateLocatorAutoLoop)
ON_COMMAND(ID_LOCATOR_MARKIN, &CMainFrame::OnLocatorMarkIn)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_MARKIN, &CMainFrame::OnUpdateLocatorMarkIn)
ON_COMMAND(ID_LOCATOR_MARKOUT, &CMainFrame::OnLocatorMarkOut)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_MARKOUT, &CMainFrame::OnUpdateLocatorMarkOut)
ON_COMMAND(ID_LOCATOR_PUNCHIN, &CMainFrame::OnLocatorPunchIn)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_PUNCHIN, &CMainFrame::OnUpdateLocatorPunchIn)
ON_COMMAND(ID_LOCATOR_PUNCHOUT, &CMainFrame::OnLocatorPunchOut)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_PUNCHOUT, &CMainFrame::OnUpdateLocatorPunchOut)
ON_COMMAND(ID_LOCATOR_AUTOPUNCH, &CMainFrame::OnLocatorAutoPunch)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_AUTOPUNCH, &CMainFrame::OnUpdateLocatorAutoPunch)
ON_COMMAND(ID_MEMORY_SAVETODISK, &CMainFrame::OnMemorySavetodisk)
ON_UPDATE_COMMAND_UI(ID_MEMORY_SAVETODISK, &CMainFrame::OnUpdateMemorySavetodisk)
ON_COMMAND(ID_MEMORY_LOADFROMDISK, &CMainFrame::OnMemoryLoadfromdisk)
ON_UPDATE_COMMAND_UI(ID_MEMORY_LOADFROMDISK, &CMainFrame::OnUpdateMemoryLoadfromdisk)
ON_COMMAND_RANGE(ID_EDITTIME_LOC0, ID_EDITTIME_RTZ, &CMainFrame::OnRangeEditTime)
ON_UPDATE_COMMAND_UI_RANGE(ID_EDITTIME_LOC0, ID_EDITTIME_RTZ, &CMainFrame::OnUpdateRangeEditTime)
ON_COMMAND(ID_LOCATOR_ZERORETURN, &CMainFrame::OnLocatorZeroreturn)
ON_UPDATE_COMMAND_UI(ID_LOCATOR_ZERORETURN, &CMainFrame::OnUpdateLocatorZeroreturn)
ON_COMMAND(ID_TAPESPEED_HIGHSPEED, &CMainFrame::OnTapespeedHighspeed)
ON_COMMAND(ID_TAPESPEED_LOWSPEED, &CMainFrame::OnTapespeedLowspeed)
ON_UPDATE_COMMAND_UI(ID_TAPESPEED_LOWSPEED, &CMainFrame::OnUpdateTapespeedLowspeed)
ON_UPDATE_COMMAND_UI(ID_TAPESPEED_HIGHSPEED, &CMainFrame::OnUpdateTapespeedHighspeed)
ON_COMMAND(ID_TRANSPORT_TOGGLEPLAY, &CMainFrame::OnTransportToggleplay)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_TOGGLEPLAY, &CMainFrame::OnUpdateTransportToggleplay)
ON_COMMAND(ID_CONFIGURATION_SAVE, &CMainFrame::OnConfigurationSave)
ON_UPDATE_COMMAND_UI(ID_CONFIGURATION_SAVE, &CMainFrame::OnUpdateConfigurationSave)
ON_COMMAND(ID_CONFIGURATION_RECALL, &CMainFrame::OnConfigurationRecall)
ON_UPDATE_COMMAND_UI(ID_CONFIGURATION_RECALL, &CMainFrame::OnUpdateConfigurationRecall)
ON_COMMAND(ID_CONFIGURATION_RESET, &CMainFrame::OnConfigurationReset)
ON_UPDATE_COMMAND_UI(ID_CONFIGURATION_RESET, &CMainFrame::OnUpdateConfigurationReset)
ON_COMMAND(ID_TRACK_STANDBYMONITOR, &CMainFrame::OnTrackStandbymonitor)
ON_UPDATE_COMMAND_UI(ID_TRACK_STANDBYMONITOR, &CMainFrame::OnUpdateTrackStandbymonitor)
ON_COMMAND(ID_VIEW_SETTINGS, &CMainFrame::OnViewSettings)
ON_COMMAND(ID_MACHINE_CONFIGURATION, &CMainFrame::OnMachineConfiguration)
ON_UPDATE_COMMAND_UI(ID_MACHINE_CONFIGURATION, &CMainFrame::OnUpdateMachineConfiguration)
ON_COMMAND(ID_MACHINE_STORE, &CMainFrame::OnMachineStore)
ON_UPDATE_COMMAND_UI(ID_MACHINE_STORE, &CMainFrame::OnUpdateMachineStore)
ON_COMMAND(ID_MACHINE_RECALL, &CMainFrame::OnMachineRecall)
ON_UPDATE_COMMAND_UI(ID_MACHINE_RECALL, &CMainFrame::OnUpdateMachineRecall)
ON_COMMAND(ID_MACHINE_RESETDEFAULTS, &CMainFrame::OnMachineResetdefaults)
ON_UPDATE_COMMAND_UI(ID_MACHINE_RESETDEFAULTS, &CMainFrame::OnUpdateMachineResetdefaults)
ON_COMMAND(ID_GENERATOR_START, &CMainFrame::OnGeneratorStart)
ON_UPDATE_COMMAND_UI(ID_GENERATOR_START, &CMainFrame::OnUpdateGeneratorStart)
ON_COMMAND(ID_GENERATOR_STOP, &CMainFrame::OnGeneratorStop)
ON_UPDATE_COMMAND_UI(ID_GENERATOR_STOP, &CMainFrame::OnUpdateGeneratorStop)
ON_COMMAND(ID_GENERATOR_RESUME, &CMainFrame::OnGeneratorResume)
ON_UPDATE_COMMAND_UI(ID_GENERATOR_RESUME, &CMainFrame::OnUpdateGeneratorResume)
ON_COMMAND(ID_GENERATOR_EDITTIME, &CMainFrame::OnGeneratorEdittime)
ON_UPDATE_COMMAND_UI(ID_GENERATOR_EDITTIME, &CMainFrame::OnUpdateGeneratorEdittime)
ON_COMMAND(ID_TRANSPORT_PUNCH, &CMainFrame::OnTransportPunch)
ON_UPDATE_COMMAND_UI(ID_TRANSPORT_PUNCH, &CMainFrame::OnUpdateTransportPunch)
END_MESSAGE_MAP()

// =========================================================================
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	theApp.m_nAppTheme = theApp.GetInt(_T("AppTheme"), 1);

	m_bTcpConnected = FALSE;
	m_bConnecting   = FALSE;
	m_dlgConnecting = NULL;
	m_strIPAddr     = _T("");
	m_nPortNumber   = STC_PORT_STATE;
	m_bRecord       = FALSE;
	m_smpteHour		= 0;
	m_smpteMins     = 0;
	m_smpteSecs     = 0;
	m_smpteFrame    = 0;

	m_velSampleIndex = 0;
	
	for (size_t i=0; i < MAX_VEL_SAMPLES; i++)
		m_velSample[i] = 0.0f;

	ZeroMemory(&m_stateMsg, sizeof(STC_STATE_MSG));

	ZeroMemory(&m_mac[0], sizeof(m_mac));
	ZeroMemory(&m_stcSN[0], sizeof(m_stcSN));
	ZeroMemory(&m_dtcSN[0], sizeof(m_dtcSN));
}

CMainFrame::~CMainFrame()
{

}

// =========================================================================
// WINDOW CREATE HANDLERS

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW),
		::CreateSolidBrush(RGB(0x48,0x48,0x48)),
		theApp.LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
#if 1
	// set the visual manager and style based on persisted value
	//CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	SetAppTheme(theApp.m_nAppTheme);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
#else
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS (CMFCVisualManagerOfficeXP));
#endif
	//CMFCToolBarComboBoxButton::SetFlatMode();
	//CMFCToolBarComboBoxButton::SetCenterVert();
	
	if (!CreateMenuBar())
		return -1;

	if (!CreateToolBars())
		return -1;

	if (!CreateStatusBar())
		return -1;

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTransport.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarLocator.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBarTransport);
	DockPane(&m_wndToolBarLocator);
	
	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);

	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	
	TRACE("Sizeof STC_STATE_MSG = %d\n", sizeof(STC_STATE_MSG));
	
	OnTransportModeChange(STC_MODE_STOP, 0);

	theApp.m_bAutoConnect = theApp.GetProfileInt(g_szSettingsSection, g_szAutoConnect, 0);

	if (theApp.m_bAutoConnect)
	{
		TCHAR szBuf[64];

		// Read the default port number to use
		m_nPortNumber = theApp.GetProfileInt(g_szSettingsSection, g_szPortNum, STC_PORT_STATE);

		// Read the last address used from the registry
		m_dwIPAddr = (DWORD)(theApp.GetProfileInt(g_szSettingsSection, g_szIPAddr, 0));
		
		// convert host address to network order
		m_dwIPAddr = htonl(m_dwIPAddr);

		// Now get the address as a string
		InetNtop(AF_INET, &m_dwIPAddr, szBuf, sizeof(szBuf)/sizeof(TCHAR));
		m_strIPAddr = szBuf;

		// Queue up auto-connect message to follow after we return
		PostMessage(WM_USER_AUTOCONNECT, 0, 0);
	}

	return 0;
}

// =========================================================================
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}


void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CMainFrame message handlers

const int WIDTH = 800;
const int HEIGHT = 350;

void CMainFrame::OnSizing(UINT nSide, LPRECT lpRect)
{
	CDockingManager* pDockMgr = GetDockingManager();

	CRect rcWindow;
	CRect rcClient;
	CRect rectPane = pDockMgr->GetOuterEdgeBounds();

	// Get the window and client dimensions
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);

	// Edges
	int border = (rcWindow.right - rcWindow.left) - rcClient.right;
	int header = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

	// Window minimum width and height
	int width  = WIDTH + border;
	int height = HEIGHT + header;	// +rectPane.Height();

#if 0
	TRACE(_T("h=%d ch=%d\n"), height, lpRect->bottom - lpRect->top);

	if (lpRect->bottom - lpRect->top < height)
		lpRect->bottom = lpRect->top + height;


	// Minimum size
	if (lpRect->right - lpRect->left < width)
		lpRect->right = lpRect->left + width;

	if (lpRect->bottom - lpRect->top < height)
		lpRect->bottom = lpRect->top + height;

	// Maximum width
	if (lpRect->right - lpRect->left > /*STEP +*/ border)
		lpRect->right = lpRect->left + /*STEP +*/ border;

	// Offered width and height
	width  = lpRect->right - lpRect->left;
	height = lpRect->bottom - lpRect->top;

	switch (nSide)
	{
		case WMSZ_LEFT:
		case WMSZ_RIGHT:
			height = (((width - border) * HEIGHT) / WIDTH) + header;	// +toolbar.rect.bottom;
			lpRect->bottom = lpRect->top + height;
			break;

		case WMSZ_TOP:
		case WMSZ_BOTTOM:
			width = ((((height/*- toolbar.rect.bottom*/) - header) * WIDTH) / HEIGHT) + border;
			lpRect->right = lpRect->left + width;
			break;

		default:
			width = ((((height/* - toolbar.rect.bottom*/) - header) * WIDTH) / HEIGHT) + border;
			lpRect->right = lpRect->left + width;
			break;
	}
#endif

	CFrameWndEx::OnSizing(nSide, lpRect);
}


void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	//m_wndView.SetFocus();
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
#if 0
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
#endif

	// otherwise, do default handling
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::DestroyWindow()
{
	// Shutdown and close the sockets
	GetSocketState().ShutDown(2);
	GetSocketState().Close();
	GetSocketCommand().Close();

	return CFrameWndEx::DestroyWindow();
}

void CMainFrame::OnClose()
{
	// Save all the tool/status/info bars states.
	//SaveBarState(_T("DRCWIN"));

	//TRACE(_T("CMainFrame::OnClose()\n"));
    CFrameWndEx::OnClose();
}

// =========================================================================
// CMainFrame Menu Handlers
// =========================================================================

// Status Bar Connect Pane Indicator
void CMainFrame::OnUpdateIndicatorConnection(CCmdUI* pCmdUI)
{
	if (m_bTcpConnected)
	{
		pCmdUI->SetText(m_strIPAddr);
	}
	else
	{
		if (m_bConnecting)
			pCmdUI->SetText(_T("CONNECTING..."));
		else
			pCmdUI->SetText(_T("OFFLINE"));
	}
}


void CMainFrame::OnUpdateIndicatorMode(CCmdUI* pCmdUI)
{
	uint16_t mode;
	CString str;

	mode = m_stateMsg.transportMode & STC_MODE_MASK;

	switch (mode)
	{
	case STC_MODE_HALT:
		str = _T("TAPE OUT");
		SetStatusBarTransportIcon(sbHALT);
		break;

	case STC_MODE_THREAD:
		str = _T("THREAD");
		SetStatusBarTransportIcon(sbTHREAD);
		break;

	case STC_MODE_STOP:
		str = _T("STOP");
		SetStatusBarTransportIcon(sbSTOP);
		break;

	case STC_MODE_PLAY:
		str = _T("PLAY");
		SetStatusBarTransportIcon(sbPLAY);
		break;

	case STC_MODE_FWD:
		str = _T("FWD");
		SetStatusBarTransportIcon(sbFWD);
		break;

	case STC_MODE_REW:
		str = _T("REW");
		SetStatusBarTransportIcon(sbREW);
		break;

	default:
		str = _T("???");
		break;
	}

	// Add, or set, any transport mode suffix modifiers

	if (m_stateMsg.transportMode & STC_M_RECORD)
	{
		str = _T("RECORD");
		SetStatusBarTransportIcon(sbREC);
	}

	if (m_stateMsg.transportMode & STC_M_LIBWIND)
	{
		str = _T("LIB-WIND");
		SetStatusBarTransportIcon(sbLIB);
	}

	if (m_stateMsg.transportMode & STC_M_SEARCH)
	{
		str = _T("SEARCH");
		SetStatusBarTransportIcon(sbSEARCH);
	}

	if (m_stateMsg.transportMode & STC_M_LOOP)
	{
		str = _T("LOOP");
		SetStatusBarTransportIcon(sbLOOP);
	}

	pCmdUI->SetText(str);
}

void CMainFrame::OnConnectTcp()
{
	CTcpConnectDlg connectDlg;
    
	connectDlg.m_IPAddr = (DWORD)(AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szIPAddr, 0));
	connectDlg.m_nPortNum = AfxGetApp()->GetProfileInt(g_szSettingsSection, g_szPortNum, STC_PORT_STATE);
	connectDlg.m_bAutoConnect = theApp.m_bAutoConnect;

	if (IsConnected())
	{
		AfxMessageBox(_T("You are already connected!"), MB_OK | MB_ICONSTOP);
		return;
	}

	INT_PTR rc;

	if ((rc = connectDlg.DoModal()) == IDOK)
	{
		AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szIPAddr, (int)connectDlg.m_IPAddr);
		AfxGetApp()->WriteProfileInt(g_szSettingsSection, g_szPortNum, connectDlg.m_nPortNum);

		// Save the IP address string to connect to
		m_strIPAddr   = connectDlg.m_strIPAddr;
		m_nPortNumber = connectDlg.m_nPortNum; 
		
		theApp.m_bAutoConnect = connectDlg.m_bAutoConnect;

		OpenConnections();
	}
	else if (rc == IDCANCEL)
	{
		theApp.m_bAutoConnect = connectDlg.m_bAutoConnect;
	}
}

void CMainFrame::OnUpdateConnectTcp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsConnected() ? FALSE : TRUE);
}

// =========================================================================
// DISCONNECT CLIENT FROM NETWORK

void CMainFrame::OnConnectDisconnect()
{
	CloseConnections();
}

void CMainFrame::OnUpdateConnectDisconnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// TOGGLE RECORD ARM MODE

void CMainFrame::OnTransportRecord()
{
	InvalidateRect(NULL, FALSE);

	m_bRecord = (m_bRecord) ? FALSE : TRUE;
}

void CMainFrame::OnUpdateTransportRecord(CCmdUI *pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck((m_bRecord) ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// TRANSPORT - STOP

void CMainFrame::OnTransportStop()
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportStop();
}

void CMainFrame::OnUpdateTransportStop(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck(IsTransportMode(STC_MODE_STOP) ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// TRANSPORT - PLAY

void CMainFrame::OnTransportPlay()
{
	// toggle record
	GetSocketCommand().TransportPlay((m_bRecord) ? 1 : 0);
}

void CMainFrame::OnUpdateTransportPlay(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck(IsTransportMode(STC_MODE_PLAY));
	pCmdUI->Enable(IsTransportReady());
}

// =========================================================================
// TRANSPORT - REWIND

void CMainFrame::OnTransportRewind()
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportRewind(IsLibWind() ? STC_M_LIBWIND : 0);
}

void CMainFrame::OnUpdateTransportRewind(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck(IsTransportMode(STC_MODE_REW));
	pCmdUI->Enable(IsTransportReady());
}

// =========================================================================
// TRANSPORT - FORWARD

void CMainFrame::OnTransportForward()
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportForward(IsLibWind() ? STC_M_LIBWIND : 0);
}

void CMainFrame::OnUpdateTransportForward(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck(IsTransportMode(STC_MODE_FWD));
	pCmdUI->Enable(IsTransportReady());
}

// =========================================================================
// TRANSPORT - TOGGLE PLAY/STOP

void CMainFrame::OnTransportToggleplay()
{
	if (IsTransportMode(STC_MODE_STOP))
	{
		GetSocketCommand().TransportPlay((m_bRecord) ? 1 : 0);
	}
	else if (IsTransportMode(STC_MODE_PLAY))
	{
		GetSocketCommand().TransportStop();
	}
}

void CMainFrame::OnUpdateTransportToggleplay(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(IsTransportMode(STC_MODE_STOP) || IsTransportMode(STC_MODE_PLAY));
}

// =========================================================================
// TRANSPORT - PUNCH IN OR OUT IN PLAY MODE

void CMainFrame::OnTransportPunch()
{
	GetSocketCommand().TransportPlay(1);
}

void CMainFrame::OnUpdateTransportPunch(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(IsTransportMode(STC_MODE_PLAY));
}

// =========================================================================
// TRANSPORT - TOGGLE LIFTERS

void CMainFrame::OnTransportTapelifter()
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportLifter();
}

void CMainFrame::OnUpdateTransportTapelifter(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->SetCheck(IsTransportModeFlags(STC_M_LIFTERS) ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// TRANSPORT - TAPE SPEED

void CMainFrame::OnTapespeedHighspeed()
{
	GetSocketCommand().TapeSpeedSet(30);
}

void CMainFrame::OnUpdateTapespeedHighspeed(CCmdUI* pCmdUI)
{
	if (!IsTrackControllerReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->SetCheck(IsTapeSpeed(30));
}

void CMainFrame::OnTapespeedLowspeed()
{
	GetSocketCommand().TapeSpeedSet(15);
}

void CMainFrame::OnUpdateTapespeedLowspeed(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IS_DCS_FOUND())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->SetCheck(IsTapeSpeed(15));
}

// =========================================================================
// TRANSPORT - LIBRARY WIND

void CMainFrame::OnTransportLibraryWind()
{
	theApp.m_bLibWind = (theApp.m_bLibWind) ? FALSE : TRUE;
}

void CMainFrame::OnUpdateTransportLibrarywind(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck(theApp.m_bLibWind ? 1 : 0);
	// Redraw the window
	Invalidate(FALSE);
}

// =========================================================================
// TOGGLE AUTO-PLAY LOCAL FLAG

void CMainFrame::OnLocatorAutoPlay()
{
	theApp.m_bAutoPlay = (theApp.m_bAutoPlay) ? FALSE : TRUE;
}

void CMainFrame::OnUpdateLocatorAutoPlay(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->SetCheck(theApp.m_bAutoPlay ? 1 : 0);
	// Redraw the window
	Invalidate(FALSE);
}

// =========================================================================
// LOCATOR HELPER METHODS

void CMainFrame::SearchOrStore(UINT cueIndex)
{
	WORD flags = 0;

	if (IsLocatorButtonMask(STC_L_STORE))
	{
		GetSocketCommand().CuePointStore(cueIndex, -1, STC_CF_ACTIVE);
	}
	else
	{
		if (IsAutoPlay())
			flags |= STC_CF_AUTO_PLAY;

		if (IsRecord())
			flags |= STC_CF_AUTO_REC;

		GetSocketCommand().LocatorSearch(cueIndex, flags);
	}
}

void CMainFrame::UpdateLocatorButtonUI(CCmdUI* pCmdUI, UINT ledmask)
{
	if (!IsConnected())	// || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//pCmdUI->SetCheck(IsLocatorButtonMask(STC_L_LOC0));
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// LOCATOR - CUE POINT 0

void CMainFrame::OnCuePointLoc0()
{
	SearchOrStore(0);
}

void CMainFrame::OnUpdateCuePointLoc0(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC0);
}

// =========================================================================
// LOCATOR - CUE POINT 1

void CMainFrame::OnCuePointLoc1()
{
	SearchOrStore(1);
}

void CMainFrame::OnUpdateCuePointLoc1(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC1);
}

// =========================================================================
// LOCATOR - CUE POINT 2

void CMainFrame::OnCuePointLoc2()
{
	SearchOrStore(2);
}

void CMainFrame::OnUpdateCuePointLoc2(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC2);
}

// =========================================================================
// LOCATOR - CUE POINT 3

void CMainFrame::OnCuePointLoc3()
{
	SearchOrStore(3);
}

void CMainFrame::OnUpdateCuePointLoc3(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC3);
}

// =========================================================================
// LOCATOR - CUE POINT 4

void CMainFrame::OnCuePointLoc4()
{
	SearchOrStore(4);
}

void CMainFrame::OnUpdateCuePointLoc4(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC4);
}

// =========================================================================
// LOCATOR - CUE POINT 5

void CMainFrame::OnCuePointLoc5()
{
	SearchOrStore(5);
}

void CMainFrame::OnUpdateCuePointLoc5(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC5);
}

// =========================================================================
// LOCATOR - CUE POINT 6

void CMainFrame::OnCuePointLoc6()
{
	SearchOrStore(6);
}

void CMainFrame::OnUpdateCuePointLoc6(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC6);
}

// =========================================================================
// LOCATOR - CUE POINT 7

void CMainFrame::OnCuePointLoc7()
{
	SearchOrStore(7);
}

void CMainFrame::OnUpdateCuePointLoc7(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC7);
}

// =========================================================================
// LOCATOR - CUE POINT 8

void CMainFrame::OnCuePointLoc8()
{
	SearchOrStore(8);
}

void CMainFrame::OnUpdateCuePointLoc8(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC8);
}

// =========================================================================
// LOCATOR - CUE POINT 9

void CMainFrame::OnCuePointLoc9()
{
	SearchOrStore(9);
}

void CMainFrame::OnUpdateCuePointLoc9(CCmdUI *pCmdUI)
{
	UpdateLocatorButtonUI(pCmdUI, STC_L_LOC9);
}

// =========================================================================
// SEARCH - TO AUTO-LOOP MARK IN TIME

void CMainFrame::OnCueMarkIn()
{
	WORD flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_MARK_IN, flags);
}

void CMainFrame::OnUpdateCueMarkIn(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// SEARCH - TO AUTO-LOOP MARK OUT TIME

void CMainFrame::OnCueMarkOut()
{
	WORD flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_MARK_OUT, flags);
}

void CMainFrame::OnUpdateCueMarkOut(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// SEARCH - TO PUNCH IN TIME

void CMainFrame::OnCuePunchIn()
{
	WORD flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_PUNCH_IN, flags);
}

void CMainFrame::OnUpdateCuePunchIn(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// SEARCH - TO PUNCH OUT TIME

void CMainFrame::OnCuePunchOut()
{
	WORD flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_PUNCH_OUT, flags);
}

void CMainFrame::OnUpdateCuePunchOut(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// SEARCH - RETURN TO ZERO HOME

void CMainFrame::OnLocatorZeroreturn()
{
	GetSocketCommand().LocatorZeroReturn();
}

void CMainFrame::OnUpdateLocatorZeroreturn(CCmdUI* pCmdUI)
{
	if (!IsConnected() || !IsTransportReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

// =========================================================================
// LOCATOR - TOGGLE BETWEEN CUE/STORE MODE

void CMainFrame::OnLocatorCueMode()
{
	GetSocketCommand().LocatorSetMode(0);
}

void CMainFrame::OnUpdateLocatorCueMode(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->SetCheck((GetTransportState().ledMaskButton & STC_L_CUE) ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnLocatorStoreMode()
{

	GetSocketCommand().LocatorSetMode(1);
}

void CMainFrame::OnUpdateLocatorStoreMode(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->SetCheck((GetTransportState().ledMaskButton & STC_L_STORE) ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

// =========================================================================
// TRACK - RECORD READY UI HANDLERS

void CMainFrame::OnRangeRecordTrack(UINT uID)
{
	uint32_t track = (uint32_t)(uID - ID_RECORD_TRACK1);
	ASSERT(track < 24);

	// Toggle track arm state for the channel
	uint8_t mask = GetTransportState().trackState[track];
	mask ^= STC_T_READY;
	GetSocketCommand().TrackSetState((WORD)track, mask);
}

void CMainFrame::OnUpdateRangeRecordTrack(CCmdUI *pCmdUI)
{
	int check = 0;
	BOOL enable = IsTrackControllerReady();
	
	uint32_t track = (uint32_t)(pCmdUI->m_nID - ID_RECORD_TRACK1);
	ASSERT(track < 24);

	if (track >= GET_MAX_TRACKS())
	{
		enable = FALSE;
	}
	else
	{
		BYTE flags = GetTransportState().trackState[track];

		if (flags & STC_T_READY)
		{
			check = 1;
		}
	}

	pCmdUI->SetCheck(check);
    pCmdUI->Enable(enable);
}

// =========================================================================
// TRACK - MONITOR INPUT UI HANDLERS

void CMainFrame::OnRangeMonitorTrack(UINT uID)
{
	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(uID - ID_MONITOR_TRACK1);
	ASSERT(track < 24);

	// Toggle track monitor state for the channel
	uint8_t mask = GetTransportState().trackState[track];
			
	// Toggle monitor enable flag for the track
	mask ^= STC_T_MONITOR;
			
	// If not in monitor mode, clear standby bit!
	if (!(mask & STC_T_MONITOR))
	{
		mask &= ~(STC_T_STANDBY);
	}
	else
	{
		// If master monitor enabled, enable standby mode for
		// the track so it will switch to standby input mode.
		if (state.monitorFlags)
			mask |= STC_T_STANDBY;
	}

	GetSocketCommand().TrackSetState((WORD)track, mask);
}

void CMainFrame::OnUpdateRangeMonitorTrack(CCmdUI* pCmdUI)
{
	int check = 0;
	BOOL enable = IsTrackControllerReady();

	uint32_t track = (uint32_t)(pCmdUI->m_nID - ID_MONITOR_TRACK1);
	ASSERT(track < 24);

	if (track >= GET_MAX_TRACKS())
	{
		enable = FALSE;
	}
	else
	{
		BYTE flags = GetTransportState().trackState[track];

		if (flags & STC_T_MONITOR)
		{
			check = 1;
		}
	}

	pCmdUI->SetCheck(check);
	pCmdUI->Enable(enable);
}

// =========================================================================
// TRACK - INPUT MODE SELECT UI HANDLERS

void CMainFrame::OnRangeInputTrack(UINT uID)
{
	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(uID - ID_INPUT_TRACK1);
	ASSERT(track < 24);

	// Preserve upper flag bits
	uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);

	uint8_t mode = STC_TRACK_INPUT | flags;

	GetSocketCommand().TrackSetState((WORD)track, (WORD)mode);
}

void CMainFrame::OnUpdateRangeInputTrack(CCmdUI* pCmdUI)
{
	int check = 0;
	BOOL enable = IsTrackControllerReady();

	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(pCmdUI->m_nID - ID_INPUT_TRACK1);
	ASSERT(track < 24);

	if (track >= GET_MAX_TRACKS())
	{
		enable = FALSE;
	}
	else
	{
		if ((state.trackState[track] & STC_TRACK_MASK) == STC_TRACK_INPUT)
		{
			check = 1;
		}
	}

	pCmdUI->SetCheck(check);
	pCmdUI->Enable(enable);
}

// =========================================================================
// TRACK - REPRO MODE SELECT UI HANDLERS

void CMainFrame::OnRangeReproTrack(UINT uID)
{
	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(uID - ID_REPRO_TRACK1);
	ASSERT(track < 24);
			
	// Preserve upper flag bits
	uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
		
	uint8_t mode = STC_TRACK_REPRO | flags;

	GetSocketCommand().TrackSetState((WORD)track, (WORD)mode);
}

void CMainFrame::OnUpdateRangeReproTrack(CCmdUI *pCmdUI)
{
	int check = 0;
	BOOL enable = IsTrackControllerReady();

	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(pCmdUI->m_nID - ID_REPRO_TRACK1);
	ASSERT(track < 24);

	if (track >= GET_MAX_TRACKS())
	{
		enable = FALSE;
	}
	else
	{
		if ((state.trackState[track] & STC_TRACK_MASK) == STC_TRACK_REPRO)
		{
			check = 1;
		}	
	}

	pCmdUI->SetCheck(check);
    pCmdUI->Enable(enable);
}

// =========================================================================
// TRACK - SYNC MODE SELECT UI HANDLERS

void CMainFrame::OnRangeSyncTrack(UINT uID)
{
	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(uID - ID_SYNC_TRACK1);
	ASSERT(track < 24);

	// Preserve upper flag bits
	uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
	uint8_t mode = STC_TRACK_SYNC | flags;

	GetSocketCommand().TrackSetState((WORD)track, (WORD)mode);
}

void CMainFrame::OnUpdateRangeSyncTrack(CCmdUI *pCmdUI)
{
	int check = 0;
	BOOL enable = IsTrackControllerReady();

	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(pCmdUI->m_nID - ID_SYNC_TRACK1);
	ASSERT(track < 24);

	if (track >= GET_MAX_TRACKS())
	{
		enable = FALSE;
	}
	else
	{
		if ((state.trackState[track] & STC_TRACK_MASK) == STC_TRACK_SYNC)
		{
			check = 1;
		}	
	}
	
	pCmdUI->SetCheck(check);
    pCmdUI->Enable(enable);
}

// =========================================================================
// TRACK - TOGGLE INPUT MODE FROM INPUT, SYNC AND REPRO

void CMainFrame::OnRangeTrackToggleMode(UINT uID)
{
	STC_STATE_MSG& state = GetTransportState();

	uint32_t track = (uint32_t)(uID - ID_TOGGLEMODE_TRACK1);
	ASSERT(track < 24);

	// Toggle track mode between input, sync and repro
	uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
	uint8_t mode  = (state.trackState[track] & STC_TRACK_MASK);
		
	// If they are trying to change the channel assignment while
	// it's already in standby mode, then just disable standby
	// monitor and go back to the default channel state.
	if (flags & STC_T_STANDBY)
	{
		flags &= ~(STC_T_STANDBY | STC_T_MONITOR);
	}
	else
	{
		// Otherwise, advance to the next track mode.
		++mode;
	}
			
	// Check for wrap around to next track state
	if (mode > STC_TRACK_INPUT)
		mode = STC_TRACK_REPRO;

	// Combine the new mode with track state flags and set it.
	mode |= flags;

	// Set the new track state
	GetSocketCommand().TrackSetState((WORD)track, (WORD)mode);
}

void CMainFrame::OnUpdateRangeTrackToggleMode(CCmdUI *pCmdUI)
{
	BOOL enable = IsTrackControllerReady();

    pCmdUI->Enable(enable);
}

// =========================================================================
// TRACK - SET ALL TRACKS TO SAFE MODE

void CMainFrame::OnTrackAllSafe()
{
	GetSocketCommand().TrackMaskAll(0, STC_T_READY);
}

void CMainFrame::OnUpdateTrackAllSafe(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TRACKS TO READY MODE

void CMainFrame::OnTrackAllReady()
{
	GetSocketCommand().TrackMaskAll(STC_T_READY, 0);
}

void CMainFrame::OnUpdateTrackAllReady(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TO INPUT MODE

void CMainFrame::OnTrackAllInput()
{
	GetSocketCommand().TrackModeAll(STC_TRACK_INPUT);
}

void CMainFrame::OnUpdateTrackAllInput(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TO REPRO MODE

void CMainFrame::OnTrackAllRepro()
{
	GetSocketCommand().TrackModeAll(STC_TRACK_REPRO);
}

void CMainFrame::OnUpdateTrackAllRepro(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TO SYNC MODE

void CMainFrame::OnTrackAllSync()
{
	GetSocketCommand().TrackModeAll(STC_TRACK_SYNC);
}

void CMainFrame::OnUpdateTrackAllSync(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TO MONITOR INPUT MODE

void CMainFrame::OnTrackAllMonitorOn()
{
	GetSocketCommand().TrackMaskAll(STC_T_MONITOR, 0);
}

void CMainFrame::OnUpdateTrackAllMonitorOn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TO MONITOR INPUT MODE

void CMainFrame::OnTrackAllMonitorOff()
{
	GetSocketCommand().TrackMaskAll(0, STC_T_MONITOR | STC_T_STANDBY);
}

void CMainFrame::OnUpdateTrackAllMonitorOff(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - TOGGLE STANDBY MONITOR MODE

void CMainFrame::OnTrackStandbymonitor()
{
	BOOL state;
	state = IsStandbyMonitor() ? FALSE : TRUE;
	GetSocketCommand().MonitorSet(state);
}

void CMainFrame::OnUpdateTrackStandbymonitor(CCmdUI* pCmdUI)
{
	if (!IsTrackControllerReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(IsStandbyMonitor());
}

// =========================================================================
// TRACK CONFIGURATION - RECALL DEFAULTS FROM EPROM

void CMainFrame::OnConfigurationRecall()
{
	if (AfxMessageBox(IDS_TRACK_CONFIG_RECALL, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		GetSocketCommand().ConfigEPROM(0);
	}
}

void CMainFrame::OnUpdateConfigurationRecall(CCmdUI* pCmdUI)
{
	if (!IsTrackControllerReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// TRACK CONFIGURATION - SAVE TO EPROM AS DEFAULT

void CMainFrame::OnConfigurationSave()
{
	if (AfxMessageBox(IDS_TRACK_CONFIG_STORE, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		GetSocketCommand().ConfigEPROM(1);
	}
}

void CMainFrame::OnUpdateConfigurationSave(CCmdUI* pCmdUI)
{
	if (!IsTrackControllerReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// TRACK CONFIGURATION - RESET ALL TO DEFAULT VALUES

void CMainFrame::OnConfigurationReset()
{
	if (AfxMessageBox(IDS_TRACK_CONFIG_RESET, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		// Reset to defaults
		GetSocketCommand().ConfigEPROM(2);
		
		// Recall the new default values into memory
		GetSocketCommand().ConfigEPROM(0);
	}
}

void CMainFrame::OnUpdateConfigurationReset(CCmdUI* pCmdUI)
{
	if (!IsTrackControllerReady())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// LOCATOR - RESET COUNTER TO ZERO

void CMainFrame::OnLocatorResetCounter()
{
	BOOL reset = FALSE;

	if (!theApp.m_bConfirmZeroReset)
	{
		reset = TRUE;
	}
	else
	{
		if (AfxMessageBox(IDS_CONFIRM_ZERO_RESET, MB_YESNO | MB_ICONQUESTION) == IDYES)
			reset = TRUE;
	}
	
	if (reset)
		GetSocketCommand().LocatorZeroReset();
}

void CMainFrame::OnUpdateLocatorResetCounter(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// LOCATOR - CANCEL SEARCH

void CMainFrame::OnLocatorCancelsearch()
{
	GetSocketCommand().LocatorCancel();
}

void CMainFrame::OnUpdateLocatorCancelsearch(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

    pCmdUI->Enable(IsTransportSearching() || IsTransportLooping());
}

// =========================================================================
// LOCATOR - AUTO LOOP MODE

void CMainFrame::OnLocatorAutoLoop()
{
	UINT cue_flags = IsRecord() ? STC_CF_AUTO_REC : 0;

	GetSocketCommand().LocatorAutoLoop(cue_flags);
}

void CMainFrame::OnUpdateLocatorAutoLoop(CCmdUI *pCmdUI)
{
	if (!IsConnected())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
    pCmdUI->Enable(IsTransportReady());
}

// =========================================================================
// LOCATOR - MARK IN

void CMainFrame::OnLocatorMarkIn()
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_MARK_IN, -1, STC_CF_ACTIVE);
}

void CMainFrame::OnUpdateLocatorMarkIn(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());	// && IsTransportReady());
}

// =========================================================================
// LOCATOR - MARK OUT

void CMainFrame::OnLocatorMarkOut()
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_MARK_OUT, -1, STC_CF_ACTIVE);
}

void CMainFrame::OnUpdateLocatorMarkOut(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());	// && IsTransportReady());
}

// =========================================================================
// LOCATOR - AUTO PUNCH MODE

void CMainFrame::OnLocatorAutoPunch()
{
	STC_STATE_MSG& state = GetTransportState();
	// toggle auto-punch mode flag
	if (state.transportMode & STC_M_PUNCH)
		GetSocketCommand().LocatorAutoPunchSet(0);
	else
		GetSocketCommand().LocatorAutoPunchSet(1);
}

void CMainFrame::OnUpdateLocatorAutoPunch(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());
}

void CMainFrame::OnLocatorPunchIn()
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_PUNCH_IN, -1, STC_CF_ACTIVE);
}

void CMainFrame::OnUpdateLocatorPunchIn(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());
}

void CMainFrame::OnLocatorPunchOut()
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_PUNCH_OUT, -1, STC_CF_ACTIVE);
}

void CMainFrame::OnUpdateLocatorPunchOut(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());
}

// =========================================================================
// MEMORY - CLEAR ALL CUE POINT MEMORIES

void CMainFrame::OnMemoryClearall()
{
	if (AfxMessageBox(IDS_MEMORY_CLEAR_ALL, MB_YESNO | MB_ICONQUESTION) == IDYES)
		GetSocketCommand().CuePointClear(STC_ALL_CUEPOINTS);
}

void CMainFrame::OnUpdateMemoryClearall(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// MEMORY - SAVE CUE POINTS TO DISK

void CMainFrame::OnMemorySavetodisk()
{
	UINT bytes;
    INT_PTR rc;
    CFile file;
    CFileException ex;
	CString strFilename;
	CString strMsg;
	DWORD dwResult;

	STC_STATE_MSG state = GetTransportState();

    CFileDialog dlg(FALSE,
                    _T("*.cpm"),
                    NULL,
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    s_szCueFilter);

    if ((rc = dlg.DoModal()) != IDOK)
    {
        if (dlg.GetFileName().GetLength() == 0)
        {
            AfxMessageBox(_T("No filename specified!"), MB_OK|MB_ICONERROR);
        }
        return;
	}

	strFilename = dlg.GetPathName();

	if (!file.Open(strFilename, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString strMsg;
		strMsg.Format(_T("File create failed!\n\n%s"), szError);
		AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
		return;
	}
    
	try
    {
		INT count = STC_MAX_CUE_POINTS;
		
		// Write the number of cue points out first
		bytes = sizeof(count);
        file.Write(&count, sizeof(count));

		// Loop reading each cue point from server and store the result.
		for (int i=0; i < count; i++)
		{
			INT cuepoint;
			UINT flags;
		
			dwResult = GetSocketCommand().CuePointGet((UINT)i, &cuepoint, &flags);

			if (dwResult != ERROR_SUCCESS)
			{
				strMsg.Format(_T("Error reading cue point data from server!\n\n%u"), dwResult);
				AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
				break;
			}

			bytes = sizeof(cuepoint);
			file.Write(&cuepoint, sizeof(cuepoint));

			bytes = sizeof(flags);
			file.Write(&flags, sizeof(flags));
		}
	}

    catch(CFileException* ex)
    {
        TCHAR szError[1024];
        ex->GetErrorMessage(szError, 1024);
        strMsg.Format(_T("File write failed!\n\n%s"), szError);
        AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
        file.Close();
		return;
    }

    file.Close();

	m_wndStatusBar.SetPaneText(0, _T("Cue Point Memories Saved!"));
}

void CMainFrame::OnUpdateMemorySavetodisk(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());
}

// =========================================================================
// MEMORY - LOAD CUE POINTS FROM DISK

void CMainFrame::OnMemoryLoadfromdisk()
{
    INT_PTR rc;
    BOOL bSuccess = FALSE;
    CFile file;
	CString strMsg;
    CFileException ex;
	DWORD dwResult;

    CFileDialog dlg(TRUE,
                    _T("*.cpm"),
                    NULL,
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    s_szCueFilter);

    if ((rc = dlg.DoModal()) != IDOK)
    {
        if (dlg.GetFileName().GetLength() == 0)
        {
            AfxMessageBox(_T("No filename specified!"), MB_OK|MB_ICONERROR);
            return;
        }
	}

	CString strFilename = dlg.GetPathName();

	if (!file.Open(strFilename, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString strMsg;
		strMsg.Format(_T("File open failed!\n\n%s"), szError);
		AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
		return;
	}

	try
    {
		int count = 0;
		
		// Write the number of cue points out first
        file.Read(&count, sizeof(count));

		// Loop reading each cue point from server and store the result.
		for (int i=0; i < count; i++)
		{
			INT cuepoint;
			UINT flags;

			file.Read(&cuepoint, sizeof(cuepoint));
			file.Read(&flags, sizeof(flags));

			dwResult = GetSocketCommand().CuePointSet((UINT)i, cuepoint, flags);

			if (dwResult != ERROR_SUCCESS)
			{
				strMsg.Format(_T("Error writing cue point data to server!\n\n%u"), dwResult);
				AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
				break;
			}
		}
    }

    catch(CFileException* ex)
    {
        TCHAR szError[1024];
        ex->GetErrorMessage(szError, 1024);
        strMsg.Format(_T("File read error!\n\n%s"), szError);
        AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
        file.Close();
		return;
    }

	file.Close();

	m_wndStatusBar.SetPaneText(0, _T("Cue Point Memories Loaded!"));
}

void CMainFrame::OnUpdateMemoryLoadfromdisk(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsConnected());
}

// =========================================================================
// MEMORY - EDIT CUE POINT MEMORY TIME

void CMainFrame::OnRangeEditTime(UINT uID)
{
	int pos;
	UINT cuestate;
	INT_PTR rc;
	TAPETIME tt;
	CEditTapeTimeDlg dlg;

	// Should be 30 or 15 IPS
	FLOAT speed = GetTapeSpeed();
	
	// Id's ID_EDITCUEPOINT_LOC0 to ID_EDITCUEPOINT_RTZ
	// must be in sequential ascending order.

	int index = uID - ID_EDITTIME_LOC0;
	
	if ((index < 0) || (index >= STC_MAX_CUE_POINTS))
		return;

	if (!IsConnected())
		return;

	GetSocketCommand().CuePointGet(index, &pos, &cuestate);

	// Convert position to tape tape h:mm:ss
	PositionToTapeTime(pos, speed, &tt);
	
	dlg.m_nHour = tt.hour;
	dlg.m_nMins = tt.mins;
	dlg.m_nSecs = tt.secs;
	dlg.m_nTens = tt.tens;
	dlg.m_bActive = (cuestate & STC_CF_ACTIVE) ? 1 : 0;
	dlg.m_nMemIndex = index;

	// Prompt the edit time dialog
	if ((rc = dlg.DoModal()) == IDOK)
	{
		tt.hour = dlg.m_nHour;
		tt.mins = dlg.m_nMins;
		tt.secs = dlg.m_nSecs;
		tt.tens = dlg.m_nTens;

		if (dlg.m_bActive)
			cuestate |= STC_CF_ACTIVE;
		else
			cuestate &= ~(STC_CF_ACTIVE);

		// Convert tape time h:mm:ss to position
		TapeTimeToPosition(&tt, speed, &pos);

		GetSocketCommand().CuePointSet(index, pos, cuestate);
	}
}

void CMainFrame::OnUpdateRangeEditTime(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// VIEW - TOGGLE TRANSPORT TOOLBAR

void CMainFrame::OnViewTransporttoolbar()
{
	BOOL bShow = (m_wndToolBarTransport.IsVisible()) ? FALSE : TRUE;
	ShowPane(&m_wndToolBarTransport, bShow, FALSE, FALSE);
}

void CMainFrame::OnUpdateViewTransporttoolbar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_wndToolBarTransport.IsVisible() ? 1 : 0);
}

// =========================================================================
// VIEW - TOGGLE LOCATOR TOOLBAR

void CMainFrame::OnViewLocatortoolbar()
{
	BOOL bShow = (m_wndToolBarLocator.IsVisible()) ? FALSE : TRUE;
	ShowPane(&m_wndToolBarLocator, bShow, FALSE, FALSE);
}

void CMainFrame::OnUpdateViewLocatortoolbar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_wndToolBarLocator.IsVisible() ? 1 : 0);
}

// =========================================================================
// VIEW - SETTINGS

void CMainFrame::OnViewSettings()
{
	INT_PTR rc;
	CPropertySheet  sheet(_T("Settings"));
	CPropPageSettings pgSettings;

	// Add the general settings page
	sheet.AddPage(&pgSettings);
	
	pgSettings.m_nTheme              = theApp.m_nAppTheme;
	pgSettings.m_bShowTimecode       = theApp.m_bShowTimecode;
	pgSettings.m_bShowTime           = theApp.m_bShowTimeDate;
	pgSettings.m_bUseTapeMachineTime = theApp.m_bUseTapeMachineTime;
	pgSettings.m_bShowVelocity       = theApp.m_nShowVelocity;
	pgSettings.m_bShowTracks         = theApp.m_bShowTracks;
	pgSettings.m_bConfirmZeroReset   = theApp.m_bConfirmZeroReset;

	if ((rc = sheet.DoModal()) == IDOK)
	{
		theApp.m_nAppTheme           = pgSettings.m_nTheme;
		theApp.m_bShowTimecode       = pgSettings.m_bShowTimecode;
		theApp.m_bShowTimeDate       = pgSettings.m_bShowTime;
		theApp.m_nShowVelocity       = pgSettings.m_bShowVelocity;
		theApp.m_bShowTracks         = pgSettings.m_bShowTracks;
		theApp.m_bConfirmZeroReset   = pgSettings.m_bConfirmZeroReset;
		theApp.m_bUseTapeMachineTime = pgSettings.m_bUseTapeMachineTime;

		((CDRCWINView*)GetActiveView())->AdjustAll();
		Invalidate(FALSE);

		SetAppTheme(pgSettings.m_nTheme);
		RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
		theApp.WriteInt(_T("AppTheme"), theApp.m_nAppTheme);
		theApp.SaveCustomState();
	}
}

// =========================================================================
// VIEW - CONFIGURATION

void CMainFrame::OnMachineConfiguration()
{
	INT_PTR rc;
	DWORD status;
	CMFCPropertySheet sheet(_T("Machine Configuration"));
	
	CPropPageConfigGeneral		pgGeneral;
	CPropPageConfigMIDI			pgMIDI;
	CPropPageConfigSMPTE		pgSMPTE;
	CPropPageConfigLocator		pgLocator;
	CPropPageConfigShuttle		pgShuttle;
	CPropPageConfigPlay			pgPlay;
	CPropPageConfigStop			pgStop;
	CPropPageConfigTensions		pgTensions;
	CPropPageConfigTransport	pgTransport;
	CString str;

	CDRCWINDoc* pDocument = (CDRCWINDoc*)GetActiveDocument();

	if (pDocument == NULL)
		return;

	STC_CONFIG_DATA& stc = pDocument->GetSTCConfigData();
	DTC_CONFIG_DATA& dtc = pDocument->GetDTCConfigData();

	// Read STC & STC configuration parameters from the machine */
	status = GetSocketCommand().MachineConfigGet(stc, dtc);

	if (status != 0)
	{
		AfxMessageBox(IDS_ERROR_READING_DTC_CONFIG, MB_OK | MB_ICONSTOP);
		return;
	}
	
	// Add the general settings page
	sheet.AddPage(&pgGeneral);
	sheet.AddPage(&pgMIDI);
	sheet.AddPage(&pgSMPTE);
	sheet.AddPage(&pgLocator);
	sheet.AddPage(&pgTransport);
	sheet.AddPage(&pgTensions);
	sheet.AddPage(&pgStop);
	sheet.AddPage(&pgPlay);
	sheet.AddPage(&pgShuttle);

	CString strSTCVersion;
	CString strDTCVersion;
	
	strSTCVersion.Format(_T("v%u.%02u.%03u"), 
		(stc.version >> 16), 
		(stc.version & 0xFF), 
		stc.build);

	strDTCVersion.Format(_T("v%u.%02u.%03u"),
		(dtc.version >> 16),
		(dtc.version & 0xFF),
		dtc.build);
	
	// Format MAC address string
	GetMACString(str, m_mac);
	pgGeneral.m_stcMAC = str;

	// Format serial# strings
	GetSerialNumberString(str, m_stcSN);
	pgGeneral.m_stcSN = str;

	GetSerialNumberString(str, m_dtcSN);
	pgGeneral.m_dtcSN = str;

	// TRANSPORT settings */
	pgGeneral.m_nShowTimecode = stc.showLongTime;
	pgGeneral.m_nBlink7Seg    = stc.searchBlink;
	pgGeneral.m_strSTCVersion = strSTCVersion;
	pgGeneral.m_strDTCVersion = strDTCVersion;

	// MIDI parameters
	pgMIDI.m_nDeviceID = stc.midiDevID;
	
	// SMPTE parameters
	pgSMPTE.m_fRefClock = stc.ref_freq;
	pgSMPTE.m_fps       = stc.smpteFPS;

	// LOCATOR parameters
	pgLocator.m_nJogNear = stc.jog_vel_near;
	pgLocator.m_nJogMid  = stc.jog_vel_mid;
	pgLocator.m_nJogFar  = stc.jog_vel_far;

	// PLAY Mode parameters
	pgPlay.m_fPlayBoostPGainLO      = dtc.play_lo_boost_pgain;
	pgPlay.m_fPlayBoostIGainLO      = dtc.play_lo_boost_igain;
	pgPlay.m_nPlayBoostEndVelLO     = dtc.play_lo_boost_end;
	pgPlay.m_fPlayBoostPGainHI      = dtc.play_hi_boost_pgain;
	pgPlay.m_fPlayBoostIGainHI      = dtc.play_hi_boost_igain;
	pgPlay.m_nPlayBoostEndVelHI     = dtc.play_hi_boost_end;
	pgPlay.m_nPinchRollerSettleTime = dtc.pinch_settle_time;
	pgPlay.m_nPlaySettleTime        = dtc.play_settle_time;
	pgPlay.m_nBrakeSettleTime       = dtc.brake_settle_time;
	pgPlay.m_bBrakesStopPlay        = (dtc.sysflags & DTC_SF_BRAKES_STOP_PLAY) ? 1 : 0;
	pgPlay.m_bEngagePinchRoller     = (dtc.sysflags & DTC_SF_ENGAGE_PINCH_ROLLER) ? 1 : 0;

	// TRANSPORT parameters
	pgTransport.m_nVelocityDetectThreshold = dtc.vel_detect_threshold;
	pgTransport.m_nRecordStrobePulseTime = dtc.record_pulse_time;
	pgTransport.m_nRecordHoldSettleTime = dtc.rechold_settle_time;
	pgTransport.m_nButtonDebounceTime =  dtc.debounce;
	
	// STOP parameters
	pgStop.m_nStopBrakeTorque = dtc.stop_brake_torque;
	pgStop.m_bLiftersAtStop = (dtc.sysflags & DTC_SF_LIFTER_AT_STOP) ? 1 : 0;
	pgStop.m_BrakesAtStop = (dtc.sysflags & DTC_SF_BRAKES_AT_STOP) ? 1 : 0;
	pgStop.m_bEndOfTapeDetect = (dtc.sysflags & DTC_SF_STOP_AT_TAPE_END) ? 1 : 0;
	
	// TENSIONS parameters
	pgTensions.m_nSupplyStopTension = dtc.stop_supply_tension;
	pgTensions.m_nSupplyShuttleTension = dtc.shuttle_supply_tension;
	pgTensions.m_nSupplyPlayTensionHI = dtc.play_hi_supply_tension;
	pgTensions.m_nSupplyPlayTensionLO = dtc.play_lo_supply_tension;
	pgTensions.m_nSupplyThreadTension = dtc.thread_supply_tension;
	pgTensions.m_nTakeupStopTension = dtc.stop_takeup_tension;
	pgTensions.m_nTakeupShuttleTension = dtc.shuttle_takeup_tension;
	pgTensions.m_nTakeupPlayTensionHI = dtc.play_hi_takeup_tension;
	pgTensions.m_nTakeupPlayTensionLO = dtc.play_lo_takeup_tension;
	pgTensions.m_nTakeupThreadTension = dtc.thread_takeup_tension;
	pgTensions.m_fTensionSensorGain = dtc.tension_sensor_gain;
	pgTensions.m_fTensionSensorMidScale1 = dtc.tension_sensor_midscale1;
	pgTensions.m_fTensionSensorMidScale2 = dtc.tension_sensor_midscale2;
	pgTensions.m_fReelRadiusGain = dtc.reel_radius_gain;
	pgTensions.m_fReelOffsetGain = dtc.reel_offset_gain;

	// SHUTTLE parameters
	pgShuttle.m_fFwdBackTensionGain = dtc.shuttle_fwd_holdback_gain;
	pgShuttle.m_fRewBackTensionGain = dtc.shuttle_rew_holdback_gain;
	pgShuttle.m_nShuttleVelocity = dtc.shuttle_velocity;
	pgShuttle.m_nLibWindVelocity = dtc.shuttle_lib_velocity;
	pgShuttle.m_nAutoSlowVelocity = dtc.shuttle_autoslow_velocity;
	pgShuttle.m_nAutoSlowAtOffset = dtc.autoslow_at_offset;
	pgShuttle.m_nAutoSlowAtVelocity = dtc.autoslow_at_velocity;
	pgShuttle.m_nLifterSettleTime = dtc.lifter_settle_time;
	pgShuttle.m_fShuttlePGain = dtc.shuttle_servo_pgain;
	pgShuttle.m_fShuttleIGain = dtc.shuttle_servo_igain;
	pgShuttle.m_fShuttleDGain = dtc.shuttle_servo_dgain;

	// Show tabbed dialog with all option settings */
	if ((rc = sheet.DoModal()) == IDOK)
	{
		// General page settings */
		stc.showLongTime = pgGeneral.m_nShowTimecode;
		stc.searchBlink  = pgGeneral.m_nBlink7Seg;

		// MIDI parameters
		stc.midiDevID = pgMIDI.m_nDeviceID;

		// SMPTE parameters
		stc.ref_freq = pgSMPTE.m_fRefClock;
		stc.smpteFPS = pgSMPTE.m_fps;

		// LOCATOR parameters
		stc.jog_vel_near = pgLocator.m_nJogNear;
		stc.jog_vel_mid = pgLocator.m_nJogMid;
		stc.jog_vel_far = pgLocator.m_nJogFar;

		// PLAY parameters
		dtc.play_lo_boost_pgain = pgPlay.m_fPlayBoostPGainLO;
		dtc.play_lo_boost_igain = pgPlay.m_fPlayBoostIGainLO;
		dtc.play_lo_boost_end = pgPlay.m_nPlayBoostEndVelLO;
		dtc.play_hi_boost_pgain = pgPlay.m_fPlayBoostPGainHI;
		dtc.play_hi_boost_igain = pgPlay.m_fPlayBoostIGainHI;
		dtc.play_hi_boost_end = pgPlay.m_nPlayBoostEndVelHI;
		dtc.pinch_settle_time = pgPlay.m_nPinchRollerSettleTime;
		dtc.play_settle_time = pgPlay.m_nPlaySettleTime;
		dtc.brake_settle_time = pgPlay.m_nBrakeSettleTime;

		if (pgPlay.m_bBrakesStopPlay)
			dtc.sysflags |= DTC_SF_BRAKES_STOP_PLAY;
		else
			dtc.sysflags &= ~(DTC_SF_BRAKES_STOP_PLAY);

		if (pgPlay.m_bEngagePinchRoller)
			dtc.sysflags |= DTC_SF_ENGAGE_PINCH_ROLLER;
		else
			dtc.sysflags &= ~(DTC_SF_ENGAGE_PINCH_ROLLER);

		// TRANSPORT parameters
		dtc.vel_detect_threshold = pgTransport.m_nVelocityDetectThreshold;
		dtc.record_pulse_time = pgTransport.m_nRecordStrobePulseTime;
		dtc.rechold_settle_time = pgTransport.m_nRecordHoldSettleTime;
		dtc.debounce = pgTransport.m_nButtonDebounceTime;
		
		// STOP parameters
		dtc.stop_brake_torque = pgStop.m_nStopBrakeTorque;

		if (pgStop.m_bLiftersAtStop)
			dtc.sysflags |= DTC_SF_LIFTER_AT_STOP;
		else
			dtc.sysflags &= ~(DTC_SF_LIFTER_AT_STOP);

		if (pgStop.m_BrakesAtStop)
			dtc.sysflags |= DTC_SF_BRAKES_AT_STOP;
		else
			dtc.sysflags &= ~(DTC_SF_BRAKES_AT_STOP);

		if (pgStop.m_bEndOfTapeDetect)
			dtc.sysflags |= DTC_SF_STOP_AT_TAPE_END;
		else
			dtc.sysflags &= ~(DTC_SF_STOP_AT_TAPE_END);

		// TENSION parameters
		dtc.stop_supply_tension = pgTensions.m_nSupplyStopTension;
		dtc.shuttle_supply_tension = pgTensions.m_nSupplyShuttleTension;
		dtc.play_hi_supply_tension = pgTensions.m_nSupplyPlayTensionHI;
		dtc.play_lo_supply_tension = pgTensions.m_nSupplyPlayTensionLO;
		dtc.thread_supply_tension = pgTensions.m_nSupplyThreadTension;
		dtc.stop_takeup_tension = pgTensions.m_nTakeupStopTension;
		dtc.shuttle_takeup_tension = pgTensions.m_nTakeupShuttleTension;
		dtc.play_hi_takeup_tension = pgTensions.m_nTakeupPlayTensionHI;
		dtc.play_lo_takeup_tension = pgTensions.m_nTakeupPlayTensionLO;
		dtc.thread_takeup_tension = pgTensions.m_nTakeupThreadTension;
		dtc.tension_sensor_gain = pgTensions.m_fTensionSensorGain;
		dtc.tension_sensor_midscale1 = pgTensions.m_fTensionSensorMidScale1;
		dtc.tension_sensor_midscale2 = pgTensions.m_fTensionSensorMidScale2;
		dtc.reel_radius_gain = pgTensions.m_fReelRadiusGain;
		dtc.reel_offset_gain = pgTensions.m_fReelOffsetGain;

		dtc.shuttle_fwd_holdback_gain = pgShuttle.m_fFwdBackTensionGain;
		dtc.shuttle_rew_holdback_gain = pgShuttle.m_fRewBackTensionGain;
		dtc.shuttle_velocity = pgShuttle.m_nShuttleVelocity;
		dtc.shuttle_lib_velocity = pgShuttle.m_nLibWindVelocity;
		dtc.shuttle_autoslow_velocity = pgShuttle.m_nAutoSlowVelocity;
		dtc.autoslow_at_offset = pgShuttle.m_nAutoSlowAtOffset;
		dtc.autoslow_at_velocity = pgShuttle.m_nAutoSlowAtVelocity;
		dtc.lifter_settle_time = pgShuttle.m_nLifterSettleTime;
		dtc.shuttle_servo_pgain = pgShuttle.m_fShuttlePGain;
		dtc.shuttle_servo_igain = pgShuttle.m_fShuttleIGain;
		dtc.shuttle_servo_dgain = pgShuttle.m_fShuttleDGain;

		//pDocument->SetModifiedFlag(TRUE);

		// Save the new parameters to the machine
		GetSocketCommand().MachineConfigSet(stc, dtc);
	}
}

void CMainFrame::OnUpdateMachineConfiguration(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// VIEW - MACHINE STORE CONFIGURATION

void CMainFrame::OnMachineStore()
{
	DWORD rc;

	if (AfxMessageBox(IDS_MACHINE_CONFIG_STORE, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		// Store STC/DTC config to defaults

		rc = GetSocketCommand().MachineConfig(1);

		if (rc != ERROR_SUCCESS)
			AfxMessageBox(IDS_ERROR_STORE_CONFIG, MB_OK | MB_ICONSTOP);
	}
}

void CMainFrame::OnUpdateMachineStore(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// VIEW - MACHINE RECALL CONFIGURATION

void CMainFrame::OnMachineRecall()
{
	DWORD rc;

	if (AfxMessageBox(IDS_MACHINE_CONFIG_RECALL, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		// Recall STC/DTC config from EPROM

		rc = GetSocketCommand().MachineConfig(0);

		if (rc != ERROR_SUCCESS)
			AfxMessageBox(IDS_ERROR_RECALL_CONFIG, MB_OK | MB_ICONSTOP);
	}
}

void CMainFrame::OnUpdateMachineRecall(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// VIEW - MACHINE RESET TO DEFAULT CONFIGURATION

void CMainFrame::OnMachineResetdefaults()
{
	DWORD rc;

	if (AfxMessageBox(IDS_MACHINE_CONFIG_RESET, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		// Reset all STC & DTC parameters to default values
		rc = GetSocketCommand().MachineConfig(2);

		if (rc != ERROR_SUCCESS)
		{
			AfxMessageBox(IDS_ERROR_RESET_CONFIG, MB_OK | MB_ICONSTOP);
		}
		else
		{
			if (rc != ERROR_SUCCESS)
			{
				AfxMessageBox(IDS_ERROR_RECALL_CONFIG, MB_OK | MB_ICONSTOP);
			}
			else
			{
				CDRCWINDoc* pDocument = (CDRCWINDoc*)GetActiveDocument();
				
				if (pDocument)
				{
					STC_CONFIG_DATA& stc = pDocument->GetSTCConfigData();
					DTC_CONFIG_DATA& dtc = pDocument->GetDTCConfigData();

					// Now read the new default STC & STC configuration parameter
					// values back from the from the machine into our document
					// configuration memory buffers. The document can then be
					// saved to disk with all the configuration parameters.

					rc = GetSocketCommand().MachineConfigGet(stc, dtc);
				}
			}
		}
	}
}

void CMainFrame::OnUpdateMachineResetdefaults(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsConnected());
}

// =========================================================================
// SMPTE - STOP SMPTE GENERATOR

void CMainFrame::OnGeneratorStop()
{
	DWORD dwError;
	
	dwError = GetSocketCommand().SMPTEEncoderCtrl(STC_SMPTE_ENCODER_CTRL_STOP);

	if (dwError)
		theApp.ShowSystemError(IDS_ERROR_SMPTE_CTRL, dwError);
}

void CMainFrame::OnUpdateGeneratorStop(CCmdUI* pCmdUI)
{
	BOOL enable = FALSE;

	if (IsHardwareSMPTE() && IsConnected())
	{
		if (m_stateMsg.smpteMode == STC_SMPTE_ENCODER)
			enable = TRUE;
	}

	pCmdUI->Enable(enable);
}

// =========================================================================
// SMPTE - START SMPTE GENERATOR

void CMainFrame::OnGeneratorStart()
{
	DWORD dwError;
	
	// Set the starting time for the encoder
	dwError = GetSocketCommand().SMPTEEncoderSetTime(
		m_smpteHour, m_smpteMins,
		m_smpteSecs, m_smpteFrame);

	if (dwError == ERROR_SUCCESS)
	{
		// Start the encoder at the time set per above 
		dwError = GetSocketCommand().SMPTEEncoderCtrl(STC_SMPTE_ENCODER_CTRL_START, 0);
	}

	if (dwError)
		theApp.ShowSystemError(IDS_ERROR_SMPTE_CTRL, dwError);
}

void CMainFrame::OnUpdateGeneratorStart(CCmdUI* pCmdUI)
{
	BOOL enable = FALSE;

	if (IsHardwareSMPTE() && IsConnected())
	{
		if (m_stateMsg.smpteMode == STC_SMPTE_OFF)
			enable = TRUE;
	}

	pCmdUI->Enable(enable);
}

// =========================================================================
// SMPTE - RESUME SMPTE GENERATOR

void CMainFrame::OnGeneratorResume()
{
	DWORD dwError;

	dwError = GetSocketCommand().SMPTEEncoderCtrl(STC_SMPTE_ENCODER_CTRL_START, 0);

	if (dwError)
		theApp.ShowSystemError(IDS_ERROR_SMPTE_CTRL, dwError);
}

void CMainFrame::OnUpdateGeneratorResume(CCmdUI* pCmdUI)
{
	BOOL enable = FALSE;

	if (IsHardwareSMPTE() && IsConnected())
	{
		if (m_stateMsg.smpteMode == STC_SMPTE_OFF)
			enable = TRUE;
	}

	pCmdUI->Enable(enable);
}

// =========================================================================
// SMPTE - EDIT TIME

void CMainFrame::OnGeneratorEdittime()
{
	INT_PTR rc;
	CEditSMPTETimeDlg dlg;

	dlg.m_nHour  = m_smpteHour;
	dlg.m_nMins  = m_smpteMins;
	dlg.m_nSecs  = m_smpteSecs;
	dlg.m_nFrame = m_smpteFrame;

	// Prompt the edit time dialog
	if ((rc = dlg.DoModal()) == IDOK)
	{
		m_smpteHour  = dlg.m_nHour;
		m_smpteMins  = dlg.m_nMins;
		m_smpteSecs  = dlg.m_nSecs;
		m_smpteFrame = dlg.m_nFrame;
	}
}

void CMainFrame::OnUpdateGeneratorEdittime(CCmdUI* pCmdUI)
{
	BOOL enable = FALSE;

	if (IsHardwareSMPTE() && IsConnected())
		enable = TRUE;

	pCmdUI->Enable(enable);
}

LRESULT CMainFrame::OnTransportModeChange(WPARAM wParam, LPARAM lParam)
{
	CString str;

	uint16_t mode = (uint16_t)wParam;
	
	// Update status bar pane with current mode icon
	INT nID = GetTransportModeBitmapID(mode);

	// Get current transport mode as a string
	GetTransportModeString(mode, str);

	// Update status bar pane icon
	SetStatusBarTransportIcon(nID);

	// Update status bar pane with current mode text
	m_wndStatusBar.SetPaneText(2, str);

	return 0;
}

LRESULT CMainFrame::OnAutoConnect(WPARAM wParam, LPARAM lParam)
{
	if (theApp.m_bAutoConnect)
	{
		OpenConnections();
	}

	return 0;
}

// End-Of-File