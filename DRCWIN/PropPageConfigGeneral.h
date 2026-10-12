#pragma once


// CPropPageConfigGeneral dialog

class CPropPageConfigGeneral : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigGeneral)

public:
	CPropPageConfigGeneral(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigGeneral();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CONFIG_GENERAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	INT m_nShowTimecode;
	INT m_nBlink7Seg;
	CString m_strSTCVersion;
	CString m_strDTCVersion;
	CString m_stcSN;
	CString m_stcMAC;
	CString m_dtcSN;
};
