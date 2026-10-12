
// ===========================================================================
// STCSocketCommand.cpp
//
// ===========================================================================

#include "stdafx.h"
#include "DRCWIN.h"
#include "MainFrm.h"
#include "STCSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ===========================================================================
// STCSocketCommand Construction
// ===========================================================================

STCSocketCommand::STCSocketCommand()
{
}

STCSocketCommand::~STCSocketCommand()
{
}

// ===========================================================================
// STCSocketCommand Helpers
// ===========================================================================

DWORD STCSocketCommand::IsConnected(void)
{
	if (!((CMainFrame*)AfxGetMainWnd())->IsConnected())
		return ERROR_CONNECTION_INVALID;
	
	return ERROR_SUCCESS;
}

void STCSocketCommand::InitHeader(STC_COMMAND_HDR& hdr, uint16_t cmd, uint16_t len)
{
	ZeroMemory(&hdr, sizeof(STC_COMMAND_HDR));

	if (!len)
		len = sizeof(STC_COMMAND_HDR) + sizeof(STC_COMMAND_ARG);

	hdr.length  = len;
	hdr.command = cmd;
	hdr.index   = 0;
	hdr.status  = 0;
}

// ===========================================================================
// 
// STCSocketCommand Command and Transport Control Methods
// ===========================================================================

DWORD STCSocketCommand::VersionGet(
	UINT* rev_stc, UINT* rev_dtc,
	BYTE* sn_stc, BYTE* sn_dtc,
	BYTE* mac)
{
	DWORD dwError;
	STC_COMMAND_VERSION_GET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_VERSION_GET, sizeof(msg));

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	if (rev_stc)
		*rev_stc = msg.version_stc;
	
	if (rev_dtc)
		*rev_dtc = msg.version_dtc;

	// return MAC address
	if (mac)
		CopyMemory(mac, msg.macaddr, 6);

	// return STC serial number
	if (sn_stc)
		CopyMemory(sn_stc, msg.sernum_stc, 16);

	// return DTC serial number
	if (sn_dtc)
		CopyMemory(sn_dtc, msg.sernum_dtc, 16);

	dwError = GetLastError();

	return dwError;
}

DWORD STCSocketCommand::TransportStop(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_STOP msg;
	
	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_STOP, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::TransportPlay(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_PLAY msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_PLAY, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::TransportRewind(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_REW msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_REW, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::TransportForward(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_FWD msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_FWD, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::TransportLifter(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_LIFTER msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_LIFTER, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

// STCSocketCommand Cue Point Methods

DWORD STCSocketCommand::CuePointClear(UINT index, UINT flags)
{
	DWORD dwError;
	STC_COMMAND_CUEPOINT_CLEAR msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_CUEPOINT_CLEAR, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = index;
	msg.arg.param2.U = flags;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::CuePointStore(UINT index, INT cuepoint, UINT flags)
{
	DWORD dwError;
	STC_COMMAND_CUEPOINT_STORE msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_CUEPOINT_STORE, sizeof(msg));

	msg.hdr.index    = (uint16_t)index;

	msg.arg.bitflags = 0;
	msg.arg.param1.I = cuepoint;
	msg.arg.param2.U = flags;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::CuePointSet(UINT index, INT cuepoint, UINT flags)
{
	DWORD dwError;
	STC_COMMAND_CUEPOINT_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_CUEPOINT_SET, sizeof(msg));

	msg.hdr.index    = (uint16_t)index;

	msg.arg.bitflags = 0;
	msg.arg.param1.I = cuepoint;
	msg.arg.param2.U = flags;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::CuePointGet(UINT index, INT* cuepoint, UINT* flags)
{
	DWORD dwError;
	STC_COMMAND_CUEPOINT_GET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_CUEPOINT_GET, sizeof(msg));
	
	msg.hdr.index    = (uint16_t)index;

	msg.arg.bitflags = 0;
	msg.arg.param1.U = index;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

    *cuepoint = msg.arg.param1.I;		// return cue point position
    *flags    = msg.arg.param2.U;		// return cue point flags (STC_CF_ACTIVE, etc)

	dwError = GetLastError();
	
	return dwError;
}

// STCSocketCommand Locator Methods

DWORD STCSocketCommand::LocatorSetMode(UINT mode)
{
	DWORD dwError;
	STC_COMMAND_LOCATE_MODE_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_LOCATE_MODE_SET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = mode;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}
	
DWORD STCSocketCommand::LocatorZeroReturn(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_LOCATE msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_LOCATE, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = STC_CUE_POINT_HOME;
	msg.arg.param2.U = flags;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::LocatorZeroReset(void)
{
	DWORD dwError;
	STC_COMMAND_ZERO_RESET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_ZERO_RESET, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::LocatorSearch(UINT index, UINT flags)
{
	DWORD dwError;
	STC_COMMAND_LOCATE msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_LOCATE, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = index;
	msg.arg.param2.U = flags;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::LocatorAutoLoop(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_LOCATE_AUTO_LOOP msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_LOCATE_AUTO_LOOP, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::LocatorAutoPunchSet(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_AUTO_PUNCH_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_AUTO_PUNCH_SET, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::LocatorCancel(UINT flags)
{
	DWORD dwError;
	STC_COMMAND_CANCEL msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_CANCEL, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

// Track/Channel State Methods

DWORD STCSocketCommand::TrackGetCount(UINT* numTracks, BOOL* dcsFound)
{
	DWORD dwError;
	STC_COMMAND_TRACK_GET_COUNT msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_TRACK_GET_COUNT, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	*numTracks = (UINT)msg.arg.param1.U;
	*dcsFound  = (BOOL)msg.arg.param2.U;

	dwError = GetLastError();

	return dwError;
}

DWORD STCSocketCommand::TrackSetState(UINT index, UINT mode)
{
	DWORD dwError;
	STC_COMMAND_TRACK_SET_STATE msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_TRACK_SET_STATE, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = index & 0xFF;
	msg.arg.param2.U = mode & 0xFF;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::TrackMaskAll(UINT setmask, UINT clearmask)
{
	DWORD dwError;
	STC_COMMAND_TRACK_MASK_ALL msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_TRACK_MASK_ALL, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = setmask;
	msg.arg.param2.U = clearmask;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

DWORD STCSocketCommand::TrackMaskToggleAll(UINT mask)
{
	DWORD dwError;
	STC_COMMAND_TRACK_TOGGLE_ALL msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_TRACK_TOGGLE_ALL, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = mask;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();

	return dwError;
}

DWORD STCSocketCommand::TrackModeAll(UINT mode)
{
	DWORD dwError;
	STC_COMMAND_TRACK_MODE_ALL msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;
	
	InitHeader(msg.hdr, STC_CMD_TRACK_MODE_ALL, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = mode;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();
	
	return dwError;
}

// DTC Load/Store/Reset track confg to EPROM: 0=load, 1=store, 2=reset
DWORD STCSocketCommand::ConfigEPROM(UINT opcode)
{
	DWORD dwError;
	STC_COMMAND_CONFIG_EPROM msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_CONFIG_EPROM, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = opcode;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();

	return dwError;
}

DWORD STCSocketCommand::MonitorSet(BOOL standby)
{
	DWORD dwError;
	STC_COMMAND_MONITOR msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_MONITOR, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = standby;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();

	return dwError;
}

DWORD STCSocketCommand::TapeSpeedSet(UINT speed)
{
	DWORD dwError;
	STC_COMMAND_TAPE_SPEED_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_TAPE_SPEED_SET, sizeof(msg));
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = speed;
	msg.arg.param2.U = 0;
	
	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	dwError = GetLastError();

	return dwError;
}

DWORD STCSocketCommand::MachineConfig(UINT opcode)
{
	DWORD dwError;
	STC_COMMAND_MACHINE_CONFIG msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_MACHINE_CONFIG, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = opcode;		/* 0=load, 1=store, 2=defaults */
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		/* Check if the STC IPC to the DTC failed */
		if (msg.hdr.status != 0)
			dwError = ERROR_READ_FAULT;
	}

	return dwError;
}

DWORD STCSocketCommand::MachineConfigGet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc)
{
	DWORD dwError;
	STC_COMMAND_MACHINE_CONFIG_GET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	ZeroMemory(&stc, sizeof(STC_CONFIG_DATA));
	ZeroMemory(&dtc, sizeof(DTC_CONFIG_DATA));

	InitHeader(msg.hdr, STC_CMD_MACHINE_CONFIG_GET, 0);
	
	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	Send(&msg, msg.hdr.length, 0);
	
	msg.hdr.length = sizeof(msg);

	Receive(&msg, msg.hdr.length, 0);

	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		/* Check if the STC IPC to the DTC failed */
		if (msg.hdr.status != 0)
		{
			dwError = ERROR_READ_FAULT;
		}
		else
		{
			CopyMemory(&stc, &msg.stc, sizeof(STC_CONFIG_DATA));
			CopyMemory(&dtc, &msg.dtc, sizeof(DTC_CONFIG_DATA));
		}
	}

	return dwError;
}

DWORD STCSocketCommand::MachineConfigSet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc)
{
	DWORD dwError;
	STC_COMMAND_MACHINE_CONFIG_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_MACHINE_CONFIG_SET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	CopyMemory(&msg.stc, &stc, sizeof(STC_CONFIG_DATA));
	CopyMemory(&msg.dtc, &dtc, sizeof(DTC_CONFIG_DATA));

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	/* Check if the STC IPC to the DTC failed */
	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		/* Check if the STC IPC to the DTC failed */
		if (msg.hdr.status != 0)
			dwError = ERROR_READ_FAULT;
	}

	return dwError;
}


DWORD STCSocketCommand::RTCTimeDateGet(DATETIME& datetime)
{
	DWORD dwError;
	STC_COMMAND_RTC_TIMEDATE_GET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_RTC_TIMEDATE_GET, sizeof(msg));

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	// Check if the STC IPC to the DTC failed
	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		// Check if RTC read failed
		if (msg.hdr.status != 0)
		{
			// Translate to a win32 error code
			dwError = ERROR_DEVICE_HARDWARE_ERROR;
		}
		else
		{
			// Copy time/date return to callers buffer
			CopyMemory(&datetime, &msg.datetime, sizeof(DATETIME));
		}
	}

	return dwError;
}


DWORD STCSocketCommand::RTCTimeDateSet(DATETIME& datetime)
{
	DWORD dwError;
	STC_COMMAND_RTC_TIMEDATE_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_RTC_TIMEDATE_SET, sizeof(msg));

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	// Check if the STC IPC to the DTC failed
	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		// Check if time/date set failed
		if (msg.hdr.status != 0)
		{
			// Translate to a win32 error code
			dwError = ERROR_INVALID_TIME;
		}
	}

	return dwError;
}


DWORD STCSocketCommand::MACAddrGet(uint8_t* mac, uint8_t* sn_stc, uint8_t* sn_dtc)
{
	DWORD dwError;
	STC_COMMAND_MACADDR_GET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_MACADDR_GET, sizeof(msg));

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	// Check if the STC IPC to the DTC failed
	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		// return MAC address
		if (mac)
			CopyMemory(mac, msg.macaddr, 6);

		// return STC serial number
		if (sn_stc)
			CopyMemory(sn_stc, msg.sernum_stc, 16);

		// return DTC serial number
		if (sn_dtc)
			CopyMemory(sn_dtc, msg.sernum_dtc, 16);
	}

	return dwError;
}


DWORD STCSocketCommand::SMPTEEncoderCtrl(UINT ctrl, UINT flags)
{
	DWORD dwError;
	STC_COMMAND_SMPTE_ENCODER_CTRL msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_SMPTE_ENCODER_CTRL, sizeof(msg));

	msg.ctrl  = (uint16_t)ctrl;
	msg.flags = (uint16_t)flags;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	/* Check if the STC IPC to the DTC failed */
	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		/* Check if the STC IPC to the DTC failed */
		if (msg.hdr.status != 0)
			dwError = ERROR_READ_FAULT;
	}

	return dwError;
}


DWORD STCSocketCommand::SMPTEEncoderSetTime(UINT hours, UINT mins, UINT secs, UINT frame)
{
	DWORD dwError;
	STC_COMMAND_SMPTE_TIME_SET msg;

	// First make sure socket is connected
	if ((dwError = IsConnected()) != ERROR_SUCCESS)
		return dwError;

	InitHeader(msg.hdr, STC_CMD_SMPTE_TIME_SET, sizeof(msg));

	msg.hours = (uint8_t)hours;
	msg.mins  = (uint8_t)mins;
	msg.secs  = (uint8_t)secs;
	msg.frame = (uint8_t)frame;

	Send(&msg, msg.hdr.length, 0);

	Receive(&msg, msg.hdr.length, 0);

	/* Check if the STC IPC to the DTC failed */
	if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		/* Check if the STC IPC to the DTC failed */
		if (msg.hdr.status != 0)
			dwError = ERROR_READ_FAULT;
	}

	return dwError;
}

// End-Of-File