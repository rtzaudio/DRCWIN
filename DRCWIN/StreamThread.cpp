
// StreamThread.cpp : implementation of the CStreamThread class
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "StreamThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// CStreamSocket Class

CStreamSocket::CStreamSocket()
{
}

CStreamSocket::~CStreamSocket()
{
}

///////////////////////////////////////////////////////////////////////////////
// CStreamThread Class

CStreamThread::CStreamThread()
{
	m_socket = NULL;
	m_bConnected = FALSE;
	m_dwSocketError = 0;
	m_nPortNum = 0;
	m_strIPAddr = CString(_T(""));
}

CStreamThread::~CStreamThread()
{

}

///////////////////////////////////////////////////////////////////////////////
// CStreamThread InitInstance and Run Handler

BOOL CStreamThread::InitInstance()
{
    TRACE(_T("CStreamThread::InitInstance()\n"));
    return TRUE;
}

int CStreamThread::ExitInstance()
{
    // Clean up resources
    return CWinThread::ExitInstance();
}

int CStreamThread::Run()
{
    TRACE(_T("CStreamThread::Run() - THREAD BEGIN\n"));

	if (OpenTCP())
	{
		TRACE(_T("CStreamThread::Connected()!\n"));

		for (;;)
		{
			m_nReadCount = m_socket->Receive(&m_stateMsg, sizeof(STC_STATE_MSG));

			if (m_nReadCount <= 0)
			{
				break;
			}
		}

		CloseTCP();
	}

	TRACE(_T("CStreamThread::Run() - THREAD END\n"));

	return 0;
}

#if 0
int CStreamThread::ReceiveThread()
{
	int wsok;
	WSADATA wsad;

	WSACleanup(); 
	Sleep(2);

	wsok = WSAStartup(MAKEWORD(2,1), &wsad);

	if (wsok)
	    return errno;	
	
	
	if ( !wsok ) 
    	WSACleanup();
    	
    return 0;
}
#endif


///////////////////////////////////////////////////////////////////////////////
// CStreamThread Helpers

BOOL CStreamThread::OpenTCP(void)
{
	// Set the flag to indicate connection in progress
	m_bConnected = FALSE;

	m_socket = new CStreamSocket;
	
	// Create the streaming receive socket
	if (!m_socket->Create())
	{
		m_dwSocketError = GetLastError();
		TRACE(_T("CStreamThread::Create Socket Error %d\n"), m_dwSocketError);
		delete m_socket;
		return FALSE;
	}

	// Set socket option to reuse the address
	BOOL fOptval = TRUE;
	if (m_socket->SetSockOpt(SO_REUSEADDR, &fOptval, sizeof(BOOL)) == FALSE)
	{
		m_dwSocketError = GetLastError();
		TRACE(_T("CStreamThread::SetSockOp() Error %d\n"), m_dwSocketError);
	}

	// Now attempt to connect to the host
	TRACE(_T("CStreamThread::Connecting...\n"));
	m_bConnected = m_socket->Connect(m_strIPAddr, m_nPortNum);
	if (!m_bConnected)
	{
		m_dwSocketError = GetLastError();
		TRACE(_T("CStreamThread::Connect() Error %d\n"), m_dwSocketError);
		delete m_socket;
	}

	return m_bConnected;
}

void CStreamThread::CloseTCP(void)
{
	TRACE(_T("CStreamThread::CloseConnections()\n"));

	if (m_socket)
	{
		// Close the socket handle if it was connected
		if (m_bConnected)
		{
			m_socket->Close();
			m_bConnected = FALSE;
		}
		
		// Delete the socket object
		delete m_socket;
		m_socket = NULL;
	}
}

// End-Of-File
