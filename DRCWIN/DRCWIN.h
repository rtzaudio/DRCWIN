
// DRCWIN.h : main header file for the DRCWIN application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "STC1200TCP.h"

#define  COLOR_SCOPE_BACKGND        0
#define  COLOR_SCOPE_GRID           1
#define  COLOR_SCOPE_AXIS           2
#define  COLOR_SCOPE_PLOT           3
#define  COLOR_SCOPE_TRIGGER        4
#define  COLOR_SCOPE_TEXT           5
#define  COLOR_ANALYZER_BACKGND     6
#define  COLOR_ANALYZER_GRID        7
#define  COLOR_ANALYZER_LEGENDS     8
#define  COLOR_ANALYZER_WAVE        9
#define  COLOR_ANALYZER_PEAK        10
#define  COLOR_ANALYZER_TEXT        11
#define  COLOR_VU_BACKGND           12
#define  COLOR_VU_SEGS_NORM         13
#define  COLOR_VU_SEGS_WARN         14
#define  COLOR_VU_SEGS_CLIP         15
#define  COLOR_PEAK_BACKGND         16
#define  COLOR_PEAK_SEGS_NORM       17
#define  COLOR_PEAK_SEGS_WARN       18
#define  COLOR_PEAK_SEGS_CLIP       19

// CDRCWINApp:
// See DRCWIN.cpp for the implementation of this class
//

#define WM_USER_MODECHANGE	(WM_USER + 100)
#define WM_USER_AUTOCONNECT	(WM_USER + 101)


class CDRCWINApp : public CWinAppEx
{
public:
	CDRCWINApp();

	int ShowSystemError(CString strMessage, DWORD dwError, UINT nIDHelp=0);
	int ShowSystemError(INT nMessageID, DWORD dwError, UINT nIDHelp=0);

	UINT	m_nAppTheme;
	//BOOL  m_bHiColorIcons;
	BOOL	m_bLibWind;
	BOOL	m_bAutoPlay;
	BOOL	m_bShowTracks;
	INT		m_nShowVelocity;
	BOOL	m_bShowTimecode;
	BOOL	m_bShowTimeDate;
	BOOL	m_bUseTapeMachineTime;
	BOOL	m_bConfirmZeroReset;
	size_t  m_trackCount;		/* num of tracks machine supports 8, 16 or 24 */
	BOOL    m_dcsFound;			/* true if DCS track controller found */
	BOOL	m_bAutoConnect;

public:
    BOOL WriteSectionFloat(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, float val);
    float GetSectionFloat( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, float val);

    BOOL WriteSectionDouble(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, double val);
    double GetSectionDouble( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, double val);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState();		// called before anything is loaded
	virtual void LoadCustomState();		// called after everything is loaded
	//virtual void PreSaveState();		// called before anything is saved
	virtual void SaveCustomState();		// called after everything is saved

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

// Global Helpers

COLORREF GetSystemColor(int nColorIndex);
BOOL SetSystemColor(int nColorIndex, COLORREF color);

void LoadSystemColorSettings();
void SaveSystemColorSettings();

extern CDRCWINApp theApp;

#define GET_MAX_TRACKS()		(theApp.m_trackCount)
#define IS_DCS_FOUND()			(theApp.m_dcsFound)