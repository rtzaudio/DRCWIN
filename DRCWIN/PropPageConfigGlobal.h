#pragma once


// CPropPageConfigGlobal dialog

class CPropPageConfigGlobal : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigGlobal)

public:
	CPropPageConfigGlobal(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPropPageConfigGlobal();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CONFIG_GLOBAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
