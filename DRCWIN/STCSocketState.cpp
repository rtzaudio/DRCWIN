
// ChildView.cpp : implementation of the STCSocketState class
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "MainFrm.h"
#include "STCSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// STCSocketState

STCSocketState::STCSocketState()
{
}

STCSocketState::~STCSocketState()
{
	//ShutDown(2);
}

int STCSocketState::ShowError(int nErrorCode)
{
	switch (nErrorCode)
	{
	case WSAEADDRINUSE:
		AfxMessageBox(_T("The specified address is already in use.\n"));
		break;
	case WSAEADDRNOTAVAIL:
		AfxMessageBox(_T("The specified address is not available from ")
			_T("the local machine.\n"));
		break;
	case WSAEAFNOSUPPORT:
		AfxMessageBox(_T("Addresses in the specified family cannot be ")
			_T("used with this socket.\n"));
		break;
	case WSAECONNREFUSED:
		AfxMessageBox(_T("The attempt to connect was refused.\n"));
		break;
	case WSAEDESTADDRREQ:
		AfxMessageBox(_T("A destination address is required.\n"));
		break;
	case WSAEFAULT:
		AfxMessageBox(_T("The lpSockAddrLen argument is incorrect.\n"));
		break;
	case WSAEINVAL:
		AfxMessageBox(_T("The socket is already bound to an address.\n"));
		break;
	case WSAEISCONN:
		AfxMessageBox(_T("The socket is already connected.\n"));
		break;
	case WSAEMFILE:
		AfxMessageBox(_T("No more file descriptors are available.\n"));
		break;
	case WSAENETUNREACH:
		AfxMessageBox(_T("The network cannot be reached from this host ")
			_T("at this time.\n"));
		break;
	case WSAENOBUFS:
		AfxMessageBox(_T("No buffer space is available. The socket ")
			_T("cannot be connected.\n"));
		break;
	case WSAENOTCONN:
		AfxMessageBox(_T("The socket is not connected.\n"));
		break;
	case WSAENOTSOCK:
		AfxMessageBox(_T("The descriptor is a file, not a socket.\n"));
		break;
	case WSAETIMEDOUT:
		AfxMessageBox(_T("The attempt to connect timed out. Make sure the ")
			          _T("TCP/IP address is correct and the MM1200 is powered up ")
			          _T("and connected to the network.\n"));
		break;
	default:
		CString str;
		str.Format(_T("OnConnect error: %d"), nErrorCode);
		AfxMessageBox(str);
		break;
	}
	
	return nErrorCode;
}

void STCSocketState::OnConnect(int nErrorCode)
{
	CMainFrame* pMainFrame = GetMainFrame();

	if (0 != nErrorCode)
	{
		Close();

		pMainFrame->m_bTcpConnected = FALSE;
		pMainFrame->m_bConnecting = FALSE;

		pMainFrame->SetStatusBarConnectionIcon(0);

		theApp.m_dcsFound = FALSE;

		ShowError(nErrorCode);
	}
	else
	{
		pMainFrame->m_bTcpConnected = TRUE;
		pMainFrame->m_bConnecting = FALSE;

		pMainFrame->SetStatusBarConnectionIcon(2);

		TRACE(_T("TCP CONNECTED!\n"));
	}
 
	CAsyncSocket::OnConnect(nErrorCode);
}


void STCSocketState::OnClose(int nErrorCode)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	
	pMainFrame->SetStatusBarConnectionIcon(0);

	TRACE(_T("The TCP/IP Network Connection Closed!"));
	pMainFrame->m_bTcpConnected = FALSE;
	pMainFrame->m_bConnecting = FALSE;	

	theApp.m_dcsFound = FALSE;
}


void STCSocketState::OnReceive(int nErrorCode)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	static int i = 0;
	static uint16_t prev_transport_mode = 0xFFFF;

	i++;

	m_nReadCount = Receive(&m_stateMsg, sizeof(STC_STATE_MSG));

	switch (m_nReadCount)
	{
    case 0:
		OnClose(0);
		Close();
        break;
      
	case SOCKET_ERROR:
        if (GetLastError() != WSAEWOULDBLOCK)
        {
			AfxMessageBox (_T("Error occurred"));
			OnClose(0);
			Close();
        }
        break;

	default:
		//TRACE("TCP rx%u\n", m_nReadCount);
		CopyMemory(&(pMainFrame->m_stateMsg), &m_stateMsg, sizeof(STC_STATE_MSG));

		size_t index = pMainFrame->m_velSampleIndex;
	
		if (index >= MAX_VEL_SAMPLES)
		{
			MoveMemory(&pMainFrame->m_velSample[0], 
					   &pMainFrame->m_velSample[1],
					   sizeof(float) * (MAX_VEL_SAMPLES - 1)); 

			pMainFrame->m_velSample[MAX_VEL_SAMPLES-1] = (float)m_stateMsg.tapeVelocity;
		}
		else
		{
			pMainFrame->m_velSample[index++] = (float)m_stateMsg.tapeVelocity;
			pMainFrame->m_velSampleIndex = index;
		}

		// this updates the status bar mode icons when the UI needs
		// to be updated from a source other than pull down menus and such.

		if (m_stateMsg.transportMode != prev_transport_mode)
		{
			prev_transport_mode = m_stateMsg.transportMode;

			pMainFrame->PostMessage(WM_USER_MODECHANGE, m_stateMsg.transportMode);
		}

		pMainFrame->Invalidate(FALSE);
		break;
   }
	
	CAsyncSocket::OnReceive(nErrorCode);
}