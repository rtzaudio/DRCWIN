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


static UINT indicators[] =
{
	ID_SEPARATOR,				// status line indicator
	ID_INDICATOR_COORD,
	ID_INDICATOR_MODE,
	ID_INDICATOR_CONNECTION,
};

/////////////////////////////////////////////////////////////////////////////
// Helpers that return menu ID's for a track number

UINT CMainFrame::GetRecordMenuID(UINT uIndex)
{
	const UINT s_recMenuID[STC_MAX_TRACKS] = {
		ID_RECORD_TRACK1,  ID_RECORD_TRACK2,  ID_RECORD_TRACK3,  ID_RECORD_TRACK4,
		ID_RECORD_TRACK5,  ID_RECORD_TRACK6,  ID_RECORD_TRACK7,  ID_RECORD_TRACK8,
		ID_RECORD_TRACK9,  ID_RECORD_TRACK10, ID_RECORD_TRACK11, ID_RECORD_TRACK12,
		ID_RECORD_TRACK13, ID_RECORD_TRACK14, ID_RECORD_TRACK15, ID_RECORD_TRACK16,
		ID_RECORD_TRACK17, ID_RECORD_TRACK18, ID_RECORD_TRACK19, ID_RECORD_TRACK20,
		ID_RECORD_TRACK21, ID_RECORD_TRACK22, ID_RECORD_TRACK23, ID_RECORD_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_recMenuID[uIndex];
}

UINT CMainFrame::GetReproMenuID(UINT uIndex)
{
	const UINT s_reproMenuID[STC_MAX_TRACKS] = {
		ID_REPRO_TRACK1,  ID_REPRO_TRACK2,  ID_REPRO_TRACK3,  ID_REPRO_TRACK4,
		ID_REPRO_TRACK5,  ID_REPRO_TRACK6,  ID_REPRO_TRACK7,  ID_REPRO_TRACK8,
		ID_REPRO_TRACK9,  ID_REPRO_TRACK10, ID_REPRO_TRACK11, ID_REPRO_TRACK12,
		ID_REPRO_TRACK13, ID_REPRO_TRACK14, ID_REPRO_TRACK15, ID_REPRO_TRACK16,
		ID_REPRO_TRACK17, ID_REPRO_TRACK18, ID_REPRO_TRACK19, ID_REPRO_TRACK20,
		ID_REPRO_TRACK21, ID_REPRO_TRACK22, ID_REPRO_TRACK23, ID_REPRO_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_reproMenuID[uIndex];
}

UINT CMainFrame::GetInputMenuID(UINT uIndex)
{
	const UINT s_syncMenuID[STC_MAX_TRACKS] = {
		ID_INPUT_TRACK1,  ID_INPUT_TRACK2,  ID_INPUT_TRACK3,  ID_INPUT_TRACK4,
		ID_INPUT_TRACK5,  ID_INPUT_TRACK6,  ID_INPUT_TRACK7,  ID_INPUT_TRACK8,
		ID_INPUT_TRACK9,  ID_INPUT_TRACK10, ID_INPUT_TRACK11, ID_INPUT_TRACK12,
		ID_INPUT_TRACK13, ID_INPUT_TRACK14, ID_INPUT_TRACK15, ID_INPUT_TRACK16,
		ID_INPUT_TRACK17, ID_INPUT_TRACK18, ID_INPUT_TRACK19, ID_INPUT_TRACK20,
		ID_INPUT_TRACK21, ID_INPUT_TRACK22, ID_INPUT_TRACK23, ID_INPUT_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_syncMenuID[uIndex];
}

UINT CMainFrame::GetSyncMenuID(UINT uIndex)
{
	const UINT s_syncMenuID[STC_MAX_TRACKS] = {
		ID_SYNC_TRACK1,  ID_SYNC_TRACK2,  ID_SYNC_TRACK3,  ID_SYNC_TRACK4,
		ID_SYNC_TRACK5,  ID_SYNC_TRACK6,  ID_SYNC_TRACK7,  ID_SYNC_TRACK8,
		ID_SYNC_TRACK9,  ID_SYNC_TRACK10, ID_SYNC_TRACK11, ID_SYNC_TRACK12,
		ID_SYNC_TRACK13, ID_SYNC_TRACK14, ID_SYNC_TRACK15, ID_SYNC_TRACK16,
		ID_SYNC_TRACK17, ID_SYNC_TRACK18, ID_SYNC_TRACK19, ID_SYNC_TRACK20,
		ID_SYNC_TRACK21, ID_SYNC_TRACK22, ID_SYNC_TRACK23, ID_SYNC_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_syncMenuID[uIndex];
}

UINT CMainFrame::GetMonitorMenuID(UINT uIndex)
{
	const UINT s_syncMenuID[STC_MAX_TRACKS] = {
		ID_MONITOR_TRACK1,  ID_MONITOR_TRACK2,  ID_MONITOR_TRACK3,  ID_MONITOR_TRACK4,
		ID_MONITOR_TRACK5,  ID_MONITOR_TRACK6,  ID_MONITOR_TRACK7,  ID_MONITOR_TRACK8,
		ID_MONITOR_TRACK9,  ID_MONITOR_TRACK10, ID_MONITOR_TRACK11, ID_MONITOR_TRACK12,
		ID_MONITOR_TRACK13, ID_MONITOR_TRACK14, ID_MONITOR_TRACK15, ID_MONITOR_TRACK16,
		ID_MONITOR_TRACK17, ID_MONITOR_TRACK18, ID_MONITOR_TRACK19, ID_MONITOR_TRACK20,
		ID_MONITOR_TRACK21, ID_MONITOR_TRACK22, ID_MONITOR_TRACK23, ID_MONITOR_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_syncMenuID[uIndex];
}

BOOL CMainFrame::IsTrackControllerReady(void)
{
	if (IsConnected())
	{
		if (IS_DCS_FOUND())
			return TRUE;
	}
	
	return FALSE;
}

BOOL CMainFrame::IsAnyTracksArmed(void)
{
	size_t i;

	for (i=0; i < STC_MAX_TRACKS; i++)
	{
		if (m_stateMsg.trackState[i] & STC_T_READY)
			return TRUE;
	}
	
	return FALSE;
}

// =========================================================================
// Return Windows system error code as a string

void CMainFrame::GetSystemError(DWORD dwError, CString& str)
{
	// Retrieve the system error message for the last-error code
	LPTSTR lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	str = lpMsgBuf;

	LocalFree(lpMsgBuf);
}

// =========================================================================
// Converts 128-bit serial number into human readable format

void CMainFrame::GetSerialNumberString(CString& str, uint8_t* sn)
{
	str.Format(_T("%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X"),
		sn[0], sn[1], sn[2], sn[3],
		sn[4], sn[5], sn[6], sn[7],
		sn[8], sn[9], sn[10], sn[11],
		sn[12], sn[13], sn[14], sn[15]);
}

// =========================================================================
// Converts 6-byte MAC address into human readable format

void CMainFrame::GetMACString(CString& str, uint8_t* mac)
{
	str.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),
		mac[0], mac[1], mac[2],
		mac[3], mac[4], mac[5]);
}

// =========================================================================
// Set the current UI theme look

void CMainFrame::SetAppTheme(UINT nTheme)
{
	theApp.m_nAppTheme = nTheme;

	switch (theApp.m_nAppTheme)
	{
	case 0:
		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
		break;

	case 1:
		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
		break;

	case 2:
		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
		break;

	default:
		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);

	}
}

// =========================================================================
// Convert absolute encoder position to tape time units.

#define INV_ROLLER_TICKS_PER_REV    (1.0f / ROLLER_TICKS_PER_REV_F)

void CMainFrame::PositionToTapeTime(int tapePosition, FLOAT tapeSpeed, TAPETIME* tapeTime)
{
	/* Get the current encoder position */
	//float position = fabsf((float)tapePosition);
	float position = (float)tapePosition;

	/* Calculate the number of revolutions from the position */
	//float revolutions = position / ROLLER_TICKS_PER_REV_F;
	float revolutions = position * INV_ROLLER_TICKS_PER_REV;

	/* Calculate the distance in inches based on the number of revolutions */
	float distance = revolutions * ROLLER_CIRCUMFERENCE_F;

	/* Get the current speed setting */
	float invspeed = (1.0f / tapeSpeed);

	/* Finally, calculate the time in seconds from the distance */
	float seconds = distance * invspeed;

	/* Convert the total seconds value into binary H:MM:SS:T values */
	SecondsToTapeTime(seconds, tapeTime);
}

// =========================================================================
// Convert tape time to absolute encoder position units.

void CMainFrame::TapeTimeToPosition(TAPETIME* tapeTime, FLOAT tapeSpeed, int* tapePosition)
{
	float time;

	/* Convert tape to total seconds */
	TapeTimeToSeconds(tapeTime, &time);

	/* Calculate the distance in inches */
	float distance = tapeSpeed * time;

	float position = (distance / ROLLER_CIRCUMFERENCE_F) * ROLLER_TICKS_PER_REV_F;

	*tapePosition = (int)position;
}

// =========================================================================
// This function takes a 32-bit time value in total seconds and
// calculates the hours, minutes and seconds members.

#define SECS_DAY    (24L * 60L * 60L)

void CMainFrame::SecondsToTapeTime(float time, TAPETIME* p)
{
	float ftime = fabsf(time);

	uint32_t dayclock = (uint32_t)ftime % SECS_DAY;

	float intpart;
	float fractpart = modff(ftime, &intpart);

	p->hour = (uint8_t)(dayclock / 3600);
	p->mins = (uint8_t)((dayclock % 3600) / 60);
	p->secs = (uint8_t)(dayclock % 60);
	p->tens = (uint8_t)(fractpart * 10.0f);
	p->frame = (uint8_t)(fractpart * 30.0f);
	p->flags = (time < 0.0f) ? 0 : F_TAPETIME_PLUS;
}

void CMainFrame::TapeTimeToSeconds(TAPETIME* p, float* time)
{
	float secs;

	secs = (float)(p->hour * 3600);
	secs += (float)(p->mins % 3600) * 60.0f;
	secs += (float)(p->secs % 60);
	secs += (float)(p->tens % 10) * 0.10f;
	//secs += (float)(p->frame % 30) * (1.0f/30.0f);

	*time = secs + 0.1f;
}

// Returns a status bar bitmap ID that respresents the
// current transport mode state

int CMainFrame::GetTransportModeBitmapID(UINT mode)
{
	int id = IDB_STATBAR_DISCONNECTED;

	switch (mode & STC_MODE_MASK)
	{
	case STC_MODE_HALT:
		id = sbHALT;
		break;

	case STC_MODE_THREAD:
		id = sbTHREAD;
		break;

	case STC_MODE_STOP:
		id = sbSTOP;
		break;

	case STC_MODE_PLAY:
		id = sbPLAY;
		break;

	case STC_MODE_FWD:
		id = sbFWD;
		break;

	case STC_MODE_REW:
		id = sbREW;
		break;

	default:
		id = sbHALT;
		break;
	}

	// Add, or set, any transport mode suffix modifiers

	if (mode & STC_M_RECORD)
		id = sbREC;

	if (mode & STC_M_LIBWIND)
		id = sbLIB;

	if (mode & STC_M_SEARCH)
		id = sbSEARCH;

	if (mode & STC_M_LOOP)
		id = sbLOOP;

	return id;
}

// Returns the transport mode as a string 

void CMainFrame::GetTransportModeString(UINT mode, CString& str)
{
	UINT modeNum = mode & STC_MODE_MASK;

	switch (modeNum)
	{
	case STC_MODE_HALT:
		str = _T("TAPE OUT");
		break;

	case STC_MODE_THREAD:
		str = _T("THREAD");
		break;

	case STC_MODE_STOP:
		str = _T("STOP");
		break;

	case STC_MODE_PLAY:
		str = _T("PLAY");
		break;

	case STC_MODE_FWD:
		str = _T("FWD");
		break;

	case STC_MODE_REW:
		str = _T("REW");
		break;

	default:
		str = _T("???");
		break;
	}

	// Add, or set, any transport mode suffix modifiers

	if (mode & STC_M_RECORD)
		str = _T("RECORD");

	if (mode & STC_M_LIBWIND)
		str = _T("LIB-WIND");

	if (mode & STC_M_SEARCH)
		str = _T("SEARCH");

	if ((mode & STC_M_LOOP) && (modeNum == STC_MODE_PLAY))
		str = _T("LOOP");
}

BOOL CMainFrame::CreateMenuBar(void)
{
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return FALSE;
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | 
		CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_GRIPPER | CBRS_BORDER_3D);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	return TRUE;
}

BOOL CMainFrame::CreateToolBars(void)
{
	CRect rcBorders = CRect(1, 1, 1, 1);
	DWORD tbStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

	// Create the transport controller toolbar

	if (!m_wndToolBarTransport.CreateEx(this, TBSTYLE_FLAT, tbStyle, rcBorders, IDR_TOOLBAR_TRANSPORT) ||
		!m_wndToolBarTransport.LoadToolBar(IDR_TOOLBAR_TRANSPORT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBarTransport.SetWindowText(_T("Transport"));

	// Create the locator tool bar

	if (!m_wndToolBarLocator.CreateEx(this, TBSTYLE_FLAT, tbStyle, rcBorders, IDR_TOOLBAR_LOCATOR) ||
		!m_wndToolBarLocator.LoadToolBar(IDR_TOOLBAR_LOCATOR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBarLocator.SetWindowText(_T("Locator"));

	return TRUE;
}

BOOL CMainFrame::CreateStatusBar(void)
{
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	
	m_wndStatusBar.SetPaneWidth(1, 100);	// mouse coordinates pane
	m_wndStatusBar.SetPaneWidth(2, 100);	// transport mode status pane
	m_wndStatusBar.SetPaneWidth(3, 120);	// connection status pane
		
	m_bmpStatBar[sbDISCONNECTED].LoadBitmap(IDB_STATBAR_DISCONNECTED);
	m_bmpStatBar[sbCONNECTING].LoadBitmap(IDB_STATBAR_CONNECTING);
	m_bmpStatBar[sbCONNECTED].LoadBitmap(IDB_STATBAR_CONNECTED);
	m_bmpStatBar[sbHALT].LoadBitmap(IDB_STATBAR_HALT);
	m_bmpStatBar[sbTHREAD].LoadBitmap(IDB_STATBAR_THREAD);
	m_bmpStatBar[sbSTOP].LoadBitmap(IDB_STATBAR_STOP);
	m_bmpStatBar[sbPLAY].LoadBitmap(IDB_STATBAR_PLAY);
	m_bmpStatBar[sbFWD].LoadBitmap(IDB_STATBAR_FWD);
	m_bmpStatBar[sbREW].LoadBitmap(IDB_STATBAR_REW);
	m_bmpStatBar[sbREC].LoadBitmap(IDB_STATBAR_REC);
	m_bmpStatBar[sbLIB].LoadBitmap(IDB_STATBAR_LIB);
	m_bmpStatBar[sbSEARCH].LoadBitmap(IDB_STATBAR_SEARCH);
	m_bmpStatBar[sbLOOP].LoadBitmap(IDB_STATBAR_LOOP);

	SetStatusBarConnectionIcon(sbDISCONNECTED);
	SetStatusBarTransportIcon(sbHALT);

	return TRUE;
}

// =========================================================================
// OPEN/CLOSE TCP NETWORK CONNECTIONS

void CMainFrame::OpenConnections(void)
{
	DWORD dwError = ERROR_SUCCESS;
	BOOL flag = TRUE;

	m_dlgConnecting = new CConnectingDlg(this);

	CString str;
    m_dlgConnecting->Create(IDD_CONNECTING, NULL);	//(CView*)&m_wndView);
	str.Format(_T("Connecting to %s"), m_strIPAddr.GetString());
    m_dlgConnecting->SetMessage(str);
    m_dlgConnecting->CenterWindow();
    m_dlgConnecting->ShowWindow(SW_SHOWNORMAL);
    m_dlgConnecting->UpdateWindow();

	//EnableWindow(FALSE);

	// Set status bar icon to connecting state
	SetStatusBarConnectionIcon(1);

	// Set the flag to indicate connection in progress
	m_bConnecting   = TRUE;
    m_bTcpConnected = FALSE;
	
	theApp.m_dcsFound = FALSE;

	// Create the streaming receive socket
	GetSocketState().Create();
	
	// Enable rx, close and connect notifications
	GetSocketState().AsyncSelect(FD_READ|FD_CLOSE|FD_CONNECT);
	
	// Indicate our application can reuse it's address
	GetSocketState().SetSockOpt(SO_REUSEADDR, &flag, sizeof(BOOL));

	// Now kick off the async TCP/IP connect
	GetSocketState().Connect(m_strIPAddr, m_nPortNumber);

	TRACE(_T("Waiting for connect..."));

	BOOL bDoingBackgroundProcessing = TRUE;

	while (bDoingBackgroundProcessing)
	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!AfxGetApp()->PumpMessage())
			{
				bDoingBackgroundProcessing = FALSE;
				::PostQuitMessage(0);
				break;
			}
		}
	   
		// let MFC do its idle processing
		LONG lIdle = 0;
		while (AfxGetApp()->OnIdle(lIdle++));

		if (!m_bConnecting)
		{
			flag = TRUE;
			dwError = GetSocketState().SetSockOpt(SO_KEEPALIVE, &flag, sizeof(flag), SOL_SOCKET);

			TRACE(_T("Connect completed...\n"));
			break;
		}

		if (m_dlgConnecting->IsCancel())
		{
			TRACE(_T("Connect Canceled!\n"));
			GetSocketState().ShutDown(2);
		
			CloseConnections();
			GetSocketState().Close();
			break;
		}
	}

	EnableWindow(TRUE);

	// First attempt to open a command/response socket on a separate port.
	if (m_bTcpConnected)
	{
		UINT uPort = m_nPortNumber + 1;
		DWORD dwError = ERROR_SUCCESS;

		// Create the command socket
		GetSocketCommand().Create();

		GetSocketCommand().SetSockOpt(SO_REUSEADDR, &flag, sizeof(BOOL));

		// Connect to the command socket
		if (!GetSocketCommand().Connect(m_strIPAddr, uPort))
			dwError = GetLastError();

		if (dwError == ERROR_SUCCESS)
		{
			flag = TRUE;
			dwError = GetSocketCommand().SetSockOpt(SO_KEEPALIVE, &flag, sizeof(flag), SOL_SOCKET);

			SetStatusBarConnectionIcon(2);

			// Get and check the STC and DTC versions to make sure we are compatible

			UINT rev_stc, rev_dtc;

			rev_stc = rev_dtc = 0;

			dwError = GetSocketCommand().VersionGet(&rev_stc, &rev_dtc, m_stcSN, m_dtcSN, m_mac);

			if (dwError != ERROR_SUCCESS)
			{
				rev_stc = rev_dtc = 0;
				TRACE(_T("Error getting version info - DTC may not be present!"));
			}
			
			m_stcVersion = rev_stc;
			m_dtcVersion = rev_dtc;

			// Requires STC version 3.0 or greater!
			if (((rev_stc >> 16) >= 3) && ((rev_stc & 0xFF) >= 0) || (rev_stc == 0))
			{
				// Query the STC to see how many tracks it's configured to support
				// if the DCS channel controller is installed in the machine.

				UINT numTracks;
				BOOL dcsFound;

				if (GetSocketCommand().TrackGetCount(&numTracks, &dcsFound) == ERROR_SUCCESS)
				{
					TRACE(_T("Machine has %u Tracks\n"), numTracks);

					if (numTracks != theApp.m_trackCount)
					{
						if ((numTracks >= 8) && (numTracks <= 24))
							theApp.m_trackCount = numTracks;
						else
							theApp.m_trackCount = 24;

						theApp.SaveCustomState();
					}

					// Indicates if the STC has a DCS track controller available
					theApp.m_dcsFound = dcsFound;
				}
			}
			else
			{
				// Close the connecting modeless dialog
				m_dlgConnecting->DestroyWindow();

				// Close connections
				CloseConnections();

				// Display error message
				CString str;
				CString fmt;
				if (!fmt.LoadString(IDS_ERROR_STC_VERSION))
					return;
				str.Format(fmt, (rev_stc >> 16), (rev_stc & 0xFF)); 
				AfxMessageBox(str, MB_OK | MB_ICONSTOP);
				Invalidate(FALSE);
				return;
			}
		}
		else
		{
			CString str;
			CString fmt;
			if (fmt.LoadString(IDS_ERROR_SOCKET_OPEN))
			{
				GetSystemError(dwError, str);
				str.Format(fmt, str.GetBuffer());
				str.ReleaseBuffer();
				//SetStatusBarConnectionIcon(0);
				AfxMessageBox(str, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Close the connecting modeless dialog
	m_dlgConnecting->DestroyWindow();

	// Update the client window
	((CDRCWINView*)GetActiveView())->AdjustAll();

	Invalidate(FALSE);
}

void CMainFrame::CloseConnections(void)
{
	TRACE(_T("CloseConnections()\n"));

	SetStatusBarConnectionIcon(0);

	// close socket if open
	GetSocketState().Close();
	GetSocketCommand().Close();

	m_bTcpConnected = m_bConnecting = FALSE;
	
	theApp.m_dcsFound = FALSE;

	ZeroMemory(&m_stateMsg, sizeof(STC_STATE_MSG));

	// Redraw the window
	Invalidate(FALSE);
}

// End-Of-File