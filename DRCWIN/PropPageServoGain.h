#pragma once


// CPropPageServoGain dialog

class CPropPageServoGain : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageServoGain)

public:
	CPropPageServoGain(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageServoGain();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_GAINS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
