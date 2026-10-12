
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "STCSocket.h"
#include "DRCWINDoc.h"
#include "DRCWINView.h"
#include "ConnectingDlg.h"

#define MAX_VEL_SAMPLES	500

/* The Ampex MM1200 tape roller quadrature encoder wheel has 40 ppr. This gives
 * either 80 or 160 edges per revolution depending on the quadrature encoder
 * configuration set by QEIConfig(). Currently we use Cha-A mode which
 * gives 80 edges per revolution. If Cha-A/B mode is used this must be
 * set to 160.
 */

#define ROLLER_TICKS_PER_REV        80
#define ROLLER_TICKS_PER_REV_F      80.0f

/* This is the diameter of the tape timer roller */
#define ROLLER_CIRCUMFERENCE_F      5.0014f

/* This is the maximum signed position value we can have. Anything past
 * this is treated as a negative position value.
 */
#define MAX_ROLLER_POSITION			(0x7FFFFFFF - 1UL)
#define MIN_ROLLER_POSITION			(-MAX_ROLLER_POSITION - 1)


class CRxMessage : public CObject
{
public:
	STC_STATE_MSG m_state;
};


class CRxStateQueue : public CTypedPtrList<CObList, CRxMessage*>
{
public:
	// Go to the end of the line
	void PutQueue(CRxMessage* newMsg)
	{
		m_cs.Lock();
		AddTail(newMsg);
		m_cs.Unlock();
	}

	// Get first element in line
	CRxMessage* GetQueue(void)
	{
		if (!IsEmpty())
			return NULL;
		CRxMessage* msg;
		m_cs.Lock();
		msg = RemoveHead();
		m_cs.Unlock();
		return msg;
	}

private:
	CCriticalSection m_cs;
};


typedef enum StatBarBitmapIndex {
	sbDISCONNECTED = 0,
	sbCONNECTING,
	sbCONNECTED,
	sbHALT,
	sbTHREAD,
	sbSTOP,
	sbPLAY,
	sbFWD,
	sbREW,
	sbREC,
	sbLIB,
	sbSEARCH,
	sbLOOP,
} StatBarBitmapIndex;


class CMainFrame : public CFrameWndEx
{
public:
	CMainFrame();
protected: 
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	STCSocketState& GetSocketState()
	{ return m_sockState; }
	STCSocketCommand& GetSocketCommand()
	{ return m_sockCommand; }

	BOOL IsConnected(void)
	{ return m_bTcpConnected; }
	BOOL IsTransportMode(UINT mode)
	{ return ((m_stateMsg.transportMode & STC_MODE_MASK) == mode) ? TRUE : FALSE; }
	BOOL IsTransportModeFlags(UINT flags)
	{ return ((m_stateMsg.transportMode & flags) == flags) ? TRUE : FALSE; }
	BOOL IsTransportReady(void)
	{ return (IsTransportMode(STC_MODE_HALT) || IsTransportMode(STC_MODE_THREAD)) ? FALSE : TRUE; }
	BOOL IsTransportSearching(void)
	{ return IsTransportModeFlags(STC_M_SEARCH) ? TRUE : FALSE; }
	BOOL IsTransportLooping(void)
	{ return IsTransportModeFlags(STC_M_LOOP) ? TRUE : FALSE; }
	BOOL IsTransportHaltMode(void)
	{ return IsTransportMode(STC_MODE_HALT); }
	BOOL IsTransportThreadMode(void)
	{ return IsTransportMode(STC_MODE_THREAD); }
	uint32_t GetLocatorButtonMask(void)
	{ return m_stateMsg.ledMaskButton; }
	BOOL IsLocatorButtonMask(uint32_t mask)
	{ return (GetLocatorButtonMask() & mask) ? TRUE : FALSE; }

	BOOL IsHardwareRTC()
	{ return (m_stateMsg.hardwareFlags & STC_HF_RTC) ? TRUE : FALSE; }
	BOOL IsHardwareDCS()
	{ return (m_stateMsg.hardwareFlags & STC_HF_DCS) ? TRUE : FALSE; }
	BOOL IsHardwareSMPTE()
	{ return (m_stateMsg.hardwareFlags & STC_HF_SMPTE) ? TRUE : FALSE; }
	BOOL IsHardwareNCO()
	{ return (m_stateMsg.hardwareFlags & STC_HF_NCO) ? TRUE : FALSE; }

	BOOL IsTapeSpeed(int speed)
	{ return (m_stateMsg.tapeSpeed == (uint8_t)speed) ? TRUE : FALSE; }
	FLOAT GetTapeSpeed(void)
	{ return (float)m_stateMsg.tapeSpeed; }

	BOOL IsRecord(void)
	{ return m_bRecord; }
	BOOL IsAutoPlay(void)
	{ return theApp.m_bAutoPlay; }
	BOOL IsLibWind(void)
	{ return theApp.m_bLibWind; }
	BOOL IsStandbyMonitor(void)
	{ return (m_stateMsg.monitorFlags) ? TRUE : FALSE; }

	// test if TCP is connected and the DCS is available
	BOOL IsTrackControllerReady(void);
	BOOL IsAnyTracksArmed(void);

	STC_STATE_MSG& GetTransportState(void)
	{ return m_stateMsg; }
	
	UINT GetRecordMenuID(UINT uIndex);
	UINT GetInputMenuID(UINT uIndex);
	UINT GetReproMenuID(UINT uIndex);
	UINT GetSyncMenuID(UINT uIndex);
	UINT GetMonitorMenuID(UINT uIndex);
	int GetTransportModeBitmapID(UINT mode);
	void GetTransportModeString(UINT mode, CString& str);

// Operations
public:
	void SetStatusBarTransportIcon(int nIndex)
	{ m_wndStatusBar.SetPaneIcon(2, m_bmpStatBar[nIndex], COLORREF(RGB(255, 0, 255))); }

	void SetStatusBarConnectionIcon(int nIndex)
	{ m_wndStatusBar.SetPaneIcon(3, m_bmpStatBar[nIndex], COLORREF(RGB(255,0,255))); }

	void GetSystemError(DWORD dwError, CString& str);

	void SecondsToTapeTime(float time, TAPETIME* p);
	void TapeTimeToSeconds(TAPETIME* p, float* time);
	
	void TapeTimeToPosition(TAPETIME* tapeTime, FLOAT tapeSpeed, int* tapePosition);
	void PositionToTapeTime(int tapePosition, FLOAT tapeSpeed, TAPETIME* tapeTime);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	void OpenConnections(void);
	void CloseConnections(void);

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar			m_wndToolBarTransport;
	CMFCToolBar			m_wndToolBarLocator;
	CMFCStatusBar		m_wndStatusBar;
	CBitmap				m_bmpStatBar[20];

	BOOL CreateMenuBar(void);
	BOOL CreateToolBars(void);
	BOOL CreateStatusBar(void);
	void SearchOrStore(UINT cueIndex);
	void UpdateLocatorButtonUI(CCmdUI* pCmdUI, UINT ledmask);
	void SetAppTheme(UINT nTheme);
	void GetSerialNumberString(CString& str, uint8_t* sn);
	void GetMACString(CString& str, uint8_t* mac);

public:
	BOOL				m_bTcpConnected;
	BOOL				m_bConnecting;
	BOOL				m_bRecord;
	STCSocketState 		m_sockState;
	STCSocketCommand	m_sockCommand;
	CConnectingDlg*		m_dlgConnecting;
	CString				m_strIPAddr;
	DWORD				m_dwIPAddr;
	INT					m_nPortNumber;
	STC_STATE_MSG		m_stateMsg;
	size_t				m_velSampleIndex;
	float				m_velSample[MAX_VEL_SAMPLES];
	uint8_t				m_mac[6];
	uint8_t				m_stcSN[16];
	uint8_t				m_dtcSN[16];
	uint32_t			m_stcVersion;
	uint32_t			m_dtcVersion;
	INT					m_smpteHour;
	INT					m_smpteMins;
	INT					m_smpteSecs;
	INT					m_smpteFrame;
// Generated message map functions
public:
	virtual BOOL DestroyWindow();

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnTransportModeChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoConnect(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnConnectTcp();
	afx_msg void OnUpdateConnectTcp(CCmdUI *pCmdUI);
	afx_msg void OnConnectDisconnect();
	afx_msg void OnUpdateConnectDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorConnection(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorMode(CCmdUI* pCmdUI);
	afx_msg void OnTransportRecord();
	afx_msg void OnUpdateTransportRecord(CCmdUI *pCmdUI);
	afx_msg void OnTransportRewind();
	afx_msg void OnUpdateTransportRewind(CCmdUI *pCmdUI);
	afx_msg void OnTransportStop();
	afx_msg void OnUpdateTransportStop(CCmdUI *pCmdUI);
	afx_msg void OnTransportTapelifter();
	afx_msg void OnUpdateTransportTapelifter(CCmdUI *pCmdUI);
	afx_msg void OnTransportPlay();
	afx_msg void OnUpdateTransportPlay(CCmdUI *pCmdUI);
	afx_msg void OnTransportForward();
	afx_msg void OnUpdateTransportForward(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc0();
	afx_msg void OnUpdateCuePointLoc0(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc1();
	afx_msg void OnUpdateCuePointLoc1(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc2();
	afx_msg void OnUpdateCuePointLoc2(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc3();
	afx_msg void OnUpdateCuePointLoc3(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc4();
	afx_msg void OnUpdateCuePointLoc4(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc5();
	afx_msg void OnUpdateCuePointLoc5(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc6();
	afx_msg void OnUpdateCuePointLoc6(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc7();
	afx_msg void OnUpdateCuePointLoc7(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc8();
	afx_msg void OnUpdateCuePointLoc8(CCmdUI *pCmdUI);
	afx_msg void OnCuePointLoc9();
	afx_msg void OnUpdateCuePointLoc9(CCmdUI *pCmdUI);
	afx_msg void OnCueMarkIn();
	afx_msg void OnUpdateCueMarkIn(CCmdUI* pCmdUI);
	afx_msg void OnCueMarkOut();
	afx_msg void OnUpdateCueMarkOut(CCmdUI* pCmdUI);
	afx_msg void OnCuePunchIn();
	afx_msg void OnUpdateCuePunchIn(CCmdUI* pCmdUI);
	afx_msg void OnCuePunchOut();
	afx_msg void OnUpdateCuePunchOut(CCmdUI* pCmdUI);
	afx_msg void OnViewTransporttoolbar();
	afx_msg void OnViewLocatortoolbar();
	afx_msg void OnUpdateViewLocatortoolbar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewTransporttoolbar(CCmdUI *pCmdUI);
	afx_msg void OnMemoryClearall();
	afx_msg void OnUpdateMemoryClearall(CCmdUI *pCmdUI);
	afx_msg void OnRangeRecordTrack(UINT uID);
	afx_msg void OnUpdateRangeRecordTrack(CCmdUI *pCmdUI);
	afx_msg void OnRangeInputTrack(UINT uID);
	afx_msg void OnUpdateRangeInputTrack(CCmdUI* pCmdUI);
	afx_msg void OnRangeReproTrack(UINT uID);
	afx_msg void OnUpdateRangeReproTrack(CCmdUI *pCmdUI);
	afx_msg void OnRangeSyncTrack(UINT uID);
	afx_msg void OnUpdateRangeSyncTrack(CCmdUI *pCmdUI);
	afx_msg void OnRangeMonitorTrack(UINT uID);
	afx_msg void OnUpdateRangeMonitorTrack(CCmdUI* pCmdUI);
	afx_msg void OnRangeTrackToggleMode(UINT uID);
	afx_msg void OnUpdateRangeTrackToggleMode(CCmdUI *pCmdUI);
	afx_msg void OnTrackAllSafe();
	afx_msg void OnUpdateTrackAllSafe(CCmdUI *pCmdUI);
	afx_msg void OnTrackAllReady();
	afx_msg void OnUpdateTrackAllReady(CCmdUI *pCmdUI);
	afx_msg void OnTrackAllInput();
	afx_msg void OnUpdateTrackAllInput(CCmdUI* pCmdUI);
	afx_msg void OnTrackAllRepro();
	afx_msg void OnUpdateTrackAllRepro(CCmdUI *pCmdUI);
	afx_msg void OnTrackAllSync();
	afx_msg void OnUpdateTrackAllSync(CCmdUI *pCmdUI);
	afx_msg void OnLocatorResetCounter();
	afx_msg void OnUpdateLocatorResetCounter(CCmdUI *pCmdUI);
	afx_msg void OnLocatorCueMode();
	afx_msg void OnUpdateLocatorCueMode(CCmdUI *pCmdUI);
	afx_msg void OnLocatorStoreMode();
	afx_msg void OnUpdateLocatorStoreMode(CCmdUI *pCmdUI);
	afx_msg void OnLocatorAutoPlay();
	afx_msg void OnUpdateLocatorAutoPlay(CCmdUI *pCmdUI);
	afx_msg void OnTransportLibraryWind();
	afx_msg void OnUpdateTransportLibrarywind(CCmdUI *pCmdUI);
	afx_msg void OnLocatorCancelsearch();
	afx_msg void OnUpdateLocatorCancelsearch(CCmdUI *pCmdUI);
	afx_msg void OnLocatorAutoLoop();
	afx_msg void OnUpdateLocatorAutoLoop(CCmdUI *pCmdUI);
	afx_msg void OnLocatorMarkIn();
	afx_msg void OnUpdateLocatorMarkIn(CCmdUI *pCmdUI);
	afx_msg void OnLocatorMarkOut();
	afx_msg void OnUpdateLocatorMarkOut(CCmdUI *pCmdUI);
	afx_msg void OnLocatorAutoPunch();
	afx_msg void OnUpdateLocatorAutoPunch(CCmdUI *pCmdUI);
	afx_msg void OnLocatorPunchIn();
	afx_msg void OnUpdateLocatorPunchIn(CCmdUI *pCmdUI);
	afx_msg void OnLocatorPunchOut();
	afx_msg void OnUpdateLocatorPunchOut(CCmdUI *pCmdUI);
	afx_msg void OnMemorySavetodisk();
	afx_msg void OnUpdateMemorySavetodisk(CCmdUI *pCmdUI);
	afx_msg void OnMemoryLoadfromdisk();
	afx_msg void OnUpdateMemoryLoadfromdisk(CCmdUI *pCmdUI);
	afx_msg void OnRangeEditTime(UINT uID);
	afx_msg void OnUpdateRangeEditTime(CCmdUI* pCmdUI);
	afx_msg void OnLocatorZeroreturn();
	afx_msg void OnUpdateLocatorZeroreturn(CCmdUI* pCmdUI);
	afx_msg void OnTapespeedHighspeed();
	afx_msg void OnTapespeedLowspeed();
	afx_msg void OnUpdateTapespeedLowspeed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTapespeedHighspeed(CCmdUI* pCmdUI);
	afx_msg void OnTransportToggleplay();
	afx_msg void OnUpdateTransportToggleplay(CCmdUI* pCmdUI);
	afx_msg void OnConfigurationSave();
	afx_msg void OnUpdateConfigurationSave(CCmdUI* pCmdUI);
	afx_msg void OnConfigurationRecall();
	afx_msg void OnUpdateConfigurationRecall(CCmdUI* pCmdUI);
	afx_msg void OnConfigurationReset();
	afx_msg void OnUpdateConfigurationReset(CCmdUI* pCmdUI);
	afx_msg void OnTrackAllMonitorOn();
	afx_msg void OnUpdateTrackAllMonitorOn(CCmdUI* pCmdUI);
	afx_msg void OnTrackAllMonitorOff();
	afx_msg void OnUpdateTrackAllMonitorOff(CCmdUI* pCmdUI);
	afx_msg void OnTrackStandbymonitor();
	afx_msg void OnUpdateTrackStandbymonitor(CCmdUI* pCmdUI);
	afx_msg void OnViewSettings();
	afx_msg void OnMachineConfiguration();
	afx_msg void OnUpdateMachineConfiguration(CCmdUI* pCmdUI);
	afx_msg void OnMachineStore();
	afx_msg void OnUpdateMachineStore(CCmdUI* pCmdUI);
	afx_msg void OnMachineRecall();
	afx_msg void OnUpdateMachineRecall(CCmdUI* pCmdUI);
	afx_msg void OnMachineResetdefaults();
	afx_msg void OnUpdateMachineResetdefaults(CCmdUI* pCmdUI);
	afx_msg void OnGeneratorStart();
	afx_msg void OnUpdateGeneratorStart(CCmdUI* pCmdUI);
	afx_msg void OnGeneratorStop();
	afx_msg void OnUpdateGeneratorStop(CCmdUI* pCmdUI);
	afx_msg void OnGeneratorResume();
	afx_msg void OnUpdateGeneratorResume(CCmdUI* pCmdUI);
	afx_msg void OnGeneratorEdittime();
	afx_msg void OnUpdateGeneratorEdittime(CCmdUI* pCmdUI);
	afx_msg void OnTransportPunch();
	afx_msg void OnUpdateTransportPunch(CCmdUI* pCmdUI);
};

// End-Of-File