// XKeyDoc.h : interface of the CDRCWinDoc class
//

#pragma once

////////////////////////////////////////////////////////////////////////////
// CDRCWinDoc Class

class CDRCWINDoc : public CDocument
{
protected: // create from serialization only
	CDRCWINDoc();
	DECLARE_DYNCREATE(CDRCWINDoc)

// Attributes
public:
	CMainFrame* GetMainFrame(void)
	{
		return (CMainFrame*)AfxGetMainWnd();
	}
	
	STC_CONFIG_DATA& GetSTCConfigData(void)
	{
		return m_stc;
	}

	DTC_CONFIG_DATA& GetDTCConfigData(void)
	{
		return m_dtc;
	}

// Operations
public:
	void ApplyTrackConfig();


// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDRCWINDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Track assignment state data
	uint8_t	m_trackState[STC_MAX_TRACKS];
	uint8_t m_cueState[STC_MAX_CUE_POINTS];

	// DTC and STC configuration data
	STC_CONFIG_DATA	m_stc;
	DTC_CONFIG_DATA m_dtc;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


