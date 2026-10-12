#pragma once

// CPropPageGeneral dialog

class CPropPageSettings : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageSettings)

public:
	CPropPageSettings();
	virtual ~CPropPageSettings();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	INT m_nTheme;
	BOOL m_bShowTracks;
	BOOL m_bShowVelocity;
	BOOL m_bShowTimecode;
	BOOL m_bShowTime;
	BOOL m_bConfirmZeroReset;
	BOOL m_bUseTapeMachineTime;
};
