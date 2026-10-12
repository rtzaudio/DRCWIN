// CDRCWINView.h : interface of the CDRCWINView class
//

#pragma once

//const size_t MAX_CHANNELS = 24;
const size_t MAX_TRANSPORT_BUTTONS = 6;

///////////////////////////////////////////////////////////////////////////////
// Custom D2D Button

class CD2DButton : public CD2DRectF
{
public:
	CD2DButton() : CD2DRectF()
		, idCommand(0)
		, idCommandAlt(0)
		, down_l(FALSE)
		, down_r(FALSE)
		, up_l(TRUE)
		, up_r(TRUE)
		, hover(FALSE)
		, enable(TRUE)
		, index(0)
		, mask(0)
		, flags(0)
	{
		label = _T("");
	}
	
	// Draw Flags
	enum {
		DF_ATOMIC      = 0x00000001,
		DF_BIGTEXT     = 0x00000002,
		DF_ACTIVE      = 0x00000004,
		DF_LAMP_MASK   = 0x00000008,
		DF_MODE_MASK   = 0x00000010,
		DF_MODE_CANCEL = 0x00000020
	};

	CD2DButton(TCHAR* szLabel, INT nCommandID, INT nCommandAltID=0, 
		       size_t indexCueState=0, UINT maskLED=0, UINT drawFlags=0) : CD2DRectF()
		, idCommand(nCommandID)
		, idCommandAlt(nCommandAltID)
		, index(indexCueState)
		, mask(maskLED)
		, flags(drawFlags)
		, down_l(FALSE)
		, down_r(FALSE)
		, up_l(TRUE)
		, up_r(TRUE)
		, hover(FALSE)
		, enable(TRUE)
	{
		label = szLabel;
	}

	void SetRect(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom)
	{
		left = fLeft, top = fTop, right = fRight, bottom = fBottom;
	}

	void SetRect(const CD2DRectF& rect)
	{
		left = rect.left, top = rect.top, right = rect.right, bottom = rect.bottom;
	}

	void Offset(FLOAT x, FLOAT y)
	{
		left += x; right += x; top += y; bottom += y;
	}

	FLOAT Height()
	{
		return((FLOAT)abs((int)(bottom - top)));
	}

	FLOAT Width()
	{
		return((FLOAT)abs((int)(right - left)));
	}

	FLOAT Area()
	{
		return Height() * Width();
	}

	CD2DPointF TopLeft()
	{
		return CD2DPointF(left, top);
	}

	CD2DPointF BottomRight()
	{
		return CD2DPointF(right, bottom);
	}

	BOOL Contains(CD2DPointF& pt)
	{
		if ((pt.x >= left) && (pt.x <= right) && (pt.y >= top) && (pt.y <= bottom))
			return TRUE;
		return FALSE;
	}

	BOOL IsLeftClickDown(void)
	{
		return down_l;
	}

	BOOL IsRightClickDown(void)
	{
		return down_r;
	}

	BOOL IsLeftClickUp(void)
	{
		return up_l;
	}

	BOOL IsRickClickUp(void)
	{
		return up_r;
	}

	BOOL IsHover(void)
	{
		return hover;
	}

	BOOL IsEnable(void)
	{
		return enable;
	}

public:
	BOOL	enable;
	BOOL	hover;
	BOOL	down_l;
	BOOL	down_r;
	BOOL	up_l;
	BOOL	up_r;
	UINT	flags;			// draw flags
	INT		idCommand;		// menu command ID
	INT		idCommandAlt;	// right click alternaate command
	size_t	index;			// index into locate state table
	UINT	mask;			// led bit flag mask
	CString label;			// button lable text
};

// All point arrays use the following type
typedef CArray<CD2DButton, CD2DButton> CD2DButtonArray;


///////////////////////////////////////////////////////////////////////////////
// Direct Draw View Class

class CDRCWINView : public CView
{
protected: // create from serialization only
	CDRCWINView();
	DECLARE_DYNCREATE(CDRCWINView)

// Attributes
public:
	CDRCWINDoc* GetDocument() const;

	CMainFrame* GetMainFrame(void)
	{
		return (CMainFrame*)AfxGetMainWnd();
	}

// Operations
public:
	void AdjustAll(void);

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDRCWINView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CD2DSolidColorBrush* m_pBackgroundBrush;
	CD2DSolidColorBrush* m_pPanelBrush;
	CD2DSolidColorBrush* m_pBlackBrush;
	CD2DSolidColorBrush* m_pWhiteBrush;
	CD2DSolidColorBrush* m_pTextBrush;
	CD2DSolidColorBrush* m_pSolidBrush;
	CD2DSolidColorBrush* m_pGrayBrush;
	CD2DSolidColorBrush* m_pLightGrayBrush;
	CD2DSolidColorBrush* m_pDarkGrayBrush;
	CD2DSolidColorBrush* m_pRecActiveBrush;
	CD2DSolidColorBrush* m_pRecReadyBrush;
	CD2DSolidColorBrush* m_pTapeTimeBrush;
	CD2DSolidColorBrush* m_pModeInputBrush;
	CD2DSolidColorBrush* m_pModeSyncBrush;
	CD2DSolidColorBrush* m_pModeReproBrush;
	CD2DSolidColorBrush* m_pModeMonBrush;
	CD2DSolidColorBrush* m_pPlot1Brush;
	CD2DSolidColorBrush* m_pPlot2Brush;

	CD2DTextFormat* m_pTextInfo;
	CD2DTextFormat* m_pTextMode;
	CD2DTextFormat* m_pTextSmallTime;
	CD2DTextFormat* m_pTextTimeHeading;
	CD2DTextFormat* m_pTextBigTime;
	CD2DTextFormat* m_pTextChannelHeading;
	CD2DTextFormat* m_pTextSwitchLabel;
	CD2DTextFormat* m_pTextButton;
	CD2DTextFormat* m_pTextButtonSmall;
	CD2DTextFormat* m_pTextPlotSmall;
	CD2DTextFormat* m_pTextFormatAxis;

	CD2DButton m_btnsTrackHeadings[STC_MAX_TRACKS];
	CD2DButton m_btnsTrackArm[STC_MAX_TRACKS];
	CD2DButton m_btnsTrackMode[STC_MAX_TRACKS];
	CD2DButton m_btnsTrackMon[STC_MAX_TRACKS];
	CD2DButton m_btnsTransport[MAX_TRANSPORT_BUTTONS];
	//CD2DButton m_btnsLocate[MAX_LOCATE_BUTTONS];
	//CD2DButton m_btnAutoPlay;
	//CD2DButton m_btnLibWind;
	//CD2DButton m_btnReset;
	
	CD2DButtonArray m_btnArray;

	CD2DRectF m_rectPanel1;
	CD2DRectF m_rectPanel2;
	CD2DRectF m_rectPanel3;
	CD2DRectF m_rectTimePane;
	CD2DRectF m_rectTimeBig;
	CD2DRectF m_rectTimeSmall;
	CD2DRectF m_rectTimeMode;
	CD2DRectF m_rectTimeProgress;
	CD2DRectF m_rectInfoPane;
	CD2DRectF m_rectInfoArea;
	CD2DRectF m_rectInfoPlot;

	D2D1_POINT_2F	m_mousePos;

protected:
	// Helpers
	void CreateD2DResources(void);
	void FreeD2DResources(void);
	void GetModeText(STC_STATE_MSG& msg, CString& str);
	void DrawPanels(CHwndRenderTarget* pRenderTarget);
	void DrawTrackAssignButtons(CHwndRenderTarget* pRenderTarget);
	void DrawTransportControlButtons(CHwndRenderTarget* pRenderTarget);
	void DrawLocatorButtons(CHwndRenderTarget* pRenderTarget);
	void DrawTapeMode(CHwndRenderTarget* pRenderTarget);
	void DrawTapeTime(CHwndRenderTarget* pRenderTarget);
	void DrawSMPTETime(CHwndRenderTarget* pRenderTarget);
	void DrawTrackUtils(CHwndRenderTarget* pRenderTarget);
	void DrawProgressBar(CHwndRenderTarget* pRenderTarget);
	void DrawVelocity(CHwndRenderTarget* pRenderTarget);
	void DrawVelocityGraph(CHwndRenderTarget* pRenderTarget, CD2DRectF rect);
	void DrawVelocityPlot(CHwndRenderTarget* pRenderTarget, CD2DRectF rect);
	void DrawCaptionText(CHwndRenderTarget* pRenderTarget, FLOAT xPos, FLOAT yPos, 
		FLOAT width1, CString strText1, FLOAT width2, CString strText2);

	void PersistRatio(const CD2DSizeF& orig, CD2DSizeF& dest, CD2DPointF& remainder);

	D2D1_POINT_2F GetDevPositionForCurrentDpi(CPoint point)
	{
		CD2DSizeF dpi = GetRenderTarget()->GetDpi();
		return D2D1::Point2F(point.x * 96 / dpi.width, point.y * 96 / dpi.height);
	}

	D2D1_POINT_2F GetDevPositionForCurrentDpi(FLOAT x, FLOAT y)
	{
		CD2DSizeF dpi = GetRenderTarget()->GetDpi();
		return D2D1::Point2F(x * 96.0f / dpi.width, y * 96.0f / dpi.height);
	}

	// Convert a window point to a D2D point, accounting for DPI differences
	D2D1_POINT_2F WindowPointToTargetPoint(CPoint point)
	{
		RECT rect;
		GetClientRect(&rect);

		point.x = rect.right - point.x - 1;
		point.y = rect.bottom - point.y - 1;

		CD2DSizeF dpi = GetRenderTarget()->GetDpi();

		return D2D1::Point2F((point.x * 96.0f / dpi.width), (point.y * 96.0f / dpi.height));
	}

// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateIndicatorCoord(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

//#ifndef _DEBUG  // debug version in DRCWINView.cpp
//inline CDRCWINDoc* CDRCWINDoc::GetDocument() const
//   { return reinterpret_cast<CDRCWINDoc*>(m_pDocument); }
//#endif