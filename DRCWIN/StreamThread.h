
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

#include "MainFrm.h"
#include "STC1200TCP.h"

///////////////////////////////////////////////////////////////////////////////
// CStreamSocket Class

class CStreamSocket : public CSocket
{
	// Construction
public:
	CStreamSocket();

	// Attributes
public:

	// Operations
public:

	// Implementation
public:

	// Overrides
protected:

	// Implementation
public:

	virtual ~CStreamSocket();
};

///////////////////////////////////////////////////////////////////////////////
// CStreamThread Class

class CStreamThread : public CWinThread
{
public:
	CStreamThread();
	
	// Attributes
	CStreamSocket* GetSocket()
	{
		return m_socket;
	}

	// Operations

public:
	// Overrides
	virtual ~CStreamThread();
	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();
	
	// Implementation
protected:
	INT m_nPortNum;
	CString	m_strIPAddr;
	DWORD m_dwSocketError;
	BOOL m_bConnected;
	CStreamSocket* m_socket;
	int m_nReadCount;
	STC_STATE_MSG m_stateMsg;

	// Helpers
	void CloseTCP(void);
	BOOL OpenTCP(void);
};

// End-Of-File