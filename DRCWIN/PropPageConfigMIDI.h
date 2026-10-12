#pragma once


// CPropPageConfigMIDI dialog

class CPropPageConfigMIDI : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigMIDI)

public:
	CPropPageConfigMIDI(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigMIDI();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_MIDI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	INT m_nDeviceID;
};
