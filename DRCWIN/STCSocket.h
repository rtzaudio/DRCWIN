// ChildView.h : interface of the CChildView class
//

#pragma once

#include "STC1200TCP.h"

class CMainFrame;

class STCSocketState : public CAsyncSocket
{
// Construction
public:
	STCSocketState();

// Attributes
public:
	CMainFrame* GetMainFrame(void)
	{ return (CMainFrame*)AfxGetMainWnd(); }

// Operations
public:
	int ShowError(int nErrorCode);

// Implementation
public:
	int				m_nReadCount;
	STC_STATE_MSG	m_stateMsg;

// Overrides
protected:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//virtual void OnSend(int nErrorCode);
	//virtual void OnOutOfBandData(int nErrorCode);
	//virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);

// Implementation
public:
	virtual ~STCSocketState();
};


class STCSocketCommand : public CSocket
{
// Construction
public:
	STCSocketCommand();

// Attributes
public:
	void InitHeader(STC_COMMAND_HDR& hdr, uint16_t cmd, uint16_t len=0);

// Operations
public:
	DWORD VersionGet(UINT* rev_stc, UINT* rev_dtc,
		BYTE* sn_stc, BYTE* sn_dtc,
		BYTE* mac);

	DWORD TransportStop(UINT flags=0);
	DWORD TransportPlay(UINT flags=0);
	DWORD TransportRewind(UINT flags=0);
	DWORD TransportForward(UINT flags=0);
	DWORD TransportLifter(UINT flags=0);
	
	DWORD LocatorSetMode(UINT mode);
	DWORD LocatorZeroReset(void);
	DWORD LocatorZeroReturn(UINT flags=0);
	DWORD LocatorSearch(UINT index, UINT flags=0);
	DWORD LocatorAutoLoop(UINT flags=0);
	DWORD LocatorAutoPunchSet(UINT flags=0);
	DWORD LocatorCancel(UINT flags=0);

	DWORD CuePointClear(UINT index, UINT flags=0);
	DWORD CuePointStore(UINT index, INT cuepoint, UINT flags=0);
	DWORD CuePointGet(UINT index, INT* cuepoint, UINT *flags);
	DWORD CuePointSet(UINT index, INT cuepoint, UINT flags=0);

	DWORD TrackGetCount(UINT* numTracks, BOOL* dcsFound);
	DWORD TrackSetState(UINT index, UINT mode);
	DWORD TrackMaskAll(UINT setmask, UINT clearmask);
	DWORD TrackMaskToggleAll(UINT mask);
	DWORD TrackModeAll(UINT newmode);
	
	DWORD MonitorSet(BOOL standby);
	DWORD TapeSpeedSet(UINT speed=30);

	DWORD ConfigEPROM(UINT opcode = 0);

	DWORD MachineConfig(UINT opcode);
	DWORD MachineConfigGet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc);
	DWORD MachineConfigSet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc);
	
	DWORD SMPTEEncoderCtrl(UINT ctrl=STC_SMPTE_ENCODER_CTRL_START, UINT flags=STC_ENCODER_CTRL_F_RESET);
	DWORD SMPTEEncoderSetTime(UINT hours, UINT mins, UINT secs, UINT frame);

	DWORD RTCTimeDateGet(DATETIME& datetime);
	DWORD RTCTimeDateSet(DATETIME& datetime);

	DWORD MACAddrGet(uint8_t* mac, uint8_t* sn_stc=NULL, uint8_t* sn_dtc=NULL);

// Implementation
public:
	DWORD IsConnected(void);

// Overrides
protected:
	//virtual void OnAccept(int nErrorCode);

// Implementation
public:
	virtual ~STCSocketCommand();
};
