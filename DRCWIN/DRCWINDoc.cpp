///////////////////////////////////////////////////////////////////////////////
// XKeyDoc.cpp : implementation of the CDRCWinDoc class
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "MainFrm.h"
#include "DRCWINView.h"
#include "DRCWINDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDRCWinDoc

IMPLEMENT_DYNCREATE(CDRCWINDoc, CDocument)

BEGIN_MESSAGE_MAP(CDRCWINDoc, CDocument)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CDRCWinDoc construction/destruction

CDRCWINDoc::CDRCWINDoc()
{
	ZeroMemory(&m_trackState, sizeof(m_trackState));
	ZeroMemory(&m_cueState, sizeof(m_cueState));
}

CDRCWINDoc::~CDRCWINDoc()
{
}

///////////////////////////////////////////////////////////////////////////////
// CDRCWinDoc serialization

void CDRCWINDoc::Serialize(CArchive& ar)
{
	int i;
	
	CMainFrame* pMainFrame = GetMainFrame();

	if (ar.IsStoring())
	{
		memcpy(m_trackState, pMainFrame->m_stateMsg.trackState, STC_MAX_TRACKS);
		memcpy(m_cueState, pMainFrame->m_stateMsg.cueState, STC_MAX_CUE_POINTS);

		// Write STC machine configuration data

		ar << m_stc.magic;
		ar << m_stc.version;
		ar << m_stc.build;
		ar << m_stc.length;
		ar << m_stc.showLongTime;
		ar << m_stc.searchBlink;
		ar << m_stc.jog_vel_far;
		ar << m_stc.jog_vel_mid;
		ar << m_stc.jog_vel_near;
		ar << m_stc.ref_freq;
		ar << m_stc.tapeSpeed;
		ar << m_stc.smpteFPS;
		ar << m_stc.midiDevID;
		ar << m_stc.reserved;

		for (i=0; i < STC_MAX_TRACKS; i++)
			ar << m_trackState[i];

		for (i=0; i < STC_MAX_CUE_POINTS; i++)
			ar << m_cueState[i];

		// Write DTC machine configuration data

		ar << m_dtc.magic;
		ar << m_dtc.version;
		ar << m_dtc.build;
		ar << m_dtc.debug;
		ar << m_dtc.pinch_settle_time;
		ar << m_dtc.lifter_settle_time;
		ar << m_dtc.brake_settle_time;
		ar << m_dtc.play_settle_time;
		ar << m_dtc.rechold_settle_time;
		ar << m_dtc.record_pulse_time;
		ar << m_dtc.vel_detect_threshold;
		ar << m_dtc.debounce;
		ar << m_dtc.sysflags;
		ar << m_dtc.reel_radius_gain;
		ar << m_dtc.reel_offset_gain;
		ar << m_dtc.tension_sensor_gain;
		ar << m_dtc.tension_sensor_midscale1;
		ar << m_dtc.tension_sensor_midscale2;
		ar << m_dtc.thread_supply_tension;
		ar << m_dtc.thread_takeup_tension;
		ar << m_dtc.stop_supply_tension;
		ar << m_dtc.stop_takeup_tension;
		ar << m_dtc.stop_brake_torque;
		ar << m_dtc.shuttle_supply_tension;
		ar << m_dtc.shuttle_takeup_tension;
		ar << m_dtc.shuttle_velocity;
		ar << m_dtc.shuttle_lib_velocity;
		ar << m_dtc.shuttle_autoslow_velocity;
		ar << m_dtc.autoslow_at_offset;
		ar << m_dtc.autoslow_at_velocity;
		ar << m_dtc.shuttle_fwd_holdback_gain;
		ar << m_dtc.shuttle_rew_holdback_gain;
		ar << m_dtc.shuttle_servo_pgain;
		ar << m_dtc.shuttle_servo_igain;
		ar << m_dtc.shuttle_servo_dgain;
		ar << m_dtc.play_hi_supply_tension;
		ar << m_dtc.play_hi_takeup_tension;
		ar << m_dtc.play_hi_boost_end;
		ar << m_dtc.play_hi_boost_pgain;
		ar << m_dtc.play_hi_boost_igain;
		ar << m_dtc.play_lo_supply_tension;
		ar << m_dtc.play_lo_takeup_tension;
		ar << m_dtc.play_lo_boost_end;
		ar << m_dtc.play_lo_boost_pgain;
		ar << m_dtc.play_lo_boost_igain;
	}
	else
	{
		// Read STC machine configuration data

		ar >> m_stc.magic;
		ar >> m_stc.version;
		ar >> m_stc.build;
		ar >> m_stc.length;
		ar >> m_stc.showLongTime;
		ar >> m_stc.searchBlink;
		ar >> m_stc.jog_vel_far;
		ar >> m_stc.jog_vel_mid;
		ar >> m_stc.jog_vel_near;
		ar >> m_stc.ref_freq;
		ar >> m_stc.tapeSpeed;
		ar >> m_stc.smpteFPS;
		ar >> m_stc.midiDevID;
		ar >> m_stc.reserved;

		for (i=0; i < STC_MAX_TRACKS; i++)
			ar >> m_trackState[i];

		for (i=0; i < STC_MAX_CUE_POINTS; i++)
			ar >> m_cueState[i];

		// Read DTC machine configuration data

		ar >> m_dtc.magic;
		ar >> m_dtc.version;
		ar >> m_dtc.build;
		ar >> m_dtc.debug;
		ar >> m_dtc.pinch_settle_time;
		ar >> m_dtc.lifter_settle_time;
		ar >> m_dtc.brake_settle_time;
		ar >> m_dtc.play_settle_time;
		ar >> m_dtc.rechold_settle_time;
		ar >> m_dtc.record_pulse_time;
		ar >> m_dtc.vel_detect_threshold;
		ar >> m_dtc.debounce;
		ar >> m_dtc.sysflags;
		ar >> m_dtc.reel_radius_gain;
		ar >> m_dtc.reel_offset_gain;
		ar >> m_dtc.tension_sensor_gain;
		ar >> m_dtc.tension_sensor_midscale1;
		ar >> m_dtc.tension_sensor_midscale2;
		ar >> m_dtc.thread_supply_tension;
		ar >> m_dtc.thread_takeup_tension;
		ar >> m_dtc.stop_supply_tension;
		ar >> m_dtc.stop_takeup_tension;
		ar >> m_dtc.stop_brake_torque;
		ar >> m_dtc.shuttle_supply_tension;
		ar >> m_dtc.shuttle_takeup_tension;
		ar >> m_dtc.shuttle_velocity;
		ar >> m_dtc.shuttle_lib_velocity;
		ar >> m_dtc.shuttle_autoslow_velocity;
		ar >> m_dtc.autoslow_at_offset;
		ar >> m_dtc.autoslow_at_velocity;
		ar >> m_dtc.shuttle_fwd_holdback_gain;
		ar >> m_dtc.shuttle_rew_holdback_gain;
		ar >> m_dtc.shuttle_servo_pgain;
		ar >> m_dtc.shuttle_servo_igain;
		ar >> m_dtc.shuttle_servo_dgain;
		ar >> m_dtc.play_hi_supply_tension;
		ar >> m_dtc.play_hi_takeup_tension;
		ar >> m_dtc.play_hi_boost_end;
		ar >> m_dtc.play_hi_boost_pgain;
		ar >> m_dtc.play_hi_boost_igain;
		ar >> m_dtc.play_lo_supply_tension;
		ar >> m_dtc.play_lo_takeup_tension;
		ar >> m_dtc.play_lo_boost_end;
		ar >> m_dtc.play_lo_boost_pgain;
		ar >> m_dtc.play_lo_boost_igain;

		//ApplyTrackConfig();
	}
}

// Apply all track states for the document to the machine if connected.

void CDRCWINDoc::ApplyTrackConfig()
{
	UINT i;

	CMainFrame* pMainFrame = GetMainFrame();

	STCSocketCommand& cmd = pMainFrame->GetSocketCommand();

	if (pMainFrame->IsConnected())
	{
		for (i=0; i < STC_MAX_TRACKS; i++)
			cmd.TrackSetState(i, m_trackState[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
// CDRCWinDoc diagnostics

#ifdef _DEBUG
void CDRCWINDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDRCWINDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////
// CDRCWinDoc commands

BOOL CDRCWINDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL CDRCWINDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;
	
	if (GetMainFrame()->IsConnected())
	{
		if (AfxMessageBox(IDS_CONFIRM_LOAD_TRACK_CONFIG, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			ApplyTrackConfig();
		}
	}

    return TRUE;
}

BOOL CDRCWINDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return CDocument::OnSaveDocument(lpszPathName);
}

void CDRCWINDoc::DeleteContents()
{
    CDocument::DeleteContents();
}


// End-Of-File
