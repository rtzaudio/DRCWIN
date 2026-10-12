// XKeyView.cpp : implementation of the CXKeyView class
//

#include "stdafx.h"
#include "DRCWIN.h"
#include "MainFrm.h"
#include "DRCWINDoc.h"
#include "DRCWINView.h"
#include "STCSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEBUG_SHOW_RECTS		0
#define SHOW_REMOTE_DATETIME	1

///////////////////////////////////////////////////////////////////////////////
// Locator Button Map Table

// CDRCWINView

IMPLEMENT_DYNCREATE(CDRCWINView, CView)

BEGIN_MESSAGE_MAP(CDRCWINView, CView)
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CDRCWINView::OnDraw2D)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COORD, &CDRCWINView::OnUpdateIndicatorCoord)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// NOTE: ID'S MUST MATCH BUTTON ARRAY ADD ORDER BELOW IN CONSTRUCTOR!

enum {
	BUTTON_ID_TRACK_ALLREPROMODE = 0,
	BUTTON_ID_TRACK_ALLSYNCMODE,
	BUTTON_ID_TRACK_ALLINPUT,
	BUTTON_ID_TRACK_ALLMONITOR,
	BUTTON_ID_LOCATOR_RESETCOUNTER,
	BUTTON_ID_LOCATOR_AUTOPLAY,
	BUTTON_ID_TRANSPORT_LIBRARYWIND,
	BUTTON_ID_TRACK_STANDBYMONITOR,
	// Start of locator buttons
	BUTTON_ID_CUEPOINT_LOC1,
	BUTTON_ID_CUEPOINT_LOC2,
	BUTTON_ID_CUEPOINT_LOC3,
	BUTTON_ID_CUEPOINT_LOC4,
	BUTTON_ID_CUEPOINT_LOC5,
	BUTTON_ID_CUEPOINT_LOC6,
	BUTTON_ID_CUEPOINT_LOC7,
	BUTTON_ID_CUEPOINT_LOC8,
	BUTTON_ID_CUEPOINT_LOC9,
	BUTTON_ID_LOCATOR_CUEMODE,
	BUTTON_ID_CUEPOINT_LOC0,
	BUTTON_ID_LOCATOR_STOREMODE,
	// Locator command buttons
	BUTTON_ID_LOCATOR_AUTOPUNCH,
	BUTTON_ID_LOCATOR_PUNCHIN,
	BUTTON_ID_LOCATOR_PUNCHOUT,
	BUTTON_ID_LOCATOR_ZERORETURN,
	BUTTON_ID_LOCATOR_AUTOLOOP,
	BUTTON_ID_LOCATOR_MARKIN,
	BUTTON_ID_LOCATOR_MARKOUT,
	BUTTON_ID_LOCATOR_CANCELSEARCH
};

///////////////////////////////////////////////////////////////////////////////
// CDRCWIN construction/destruction

CDRCWINView::CDRCWINView()
{
	m_mousePos.x = 0.0f;
	m_mousePos.y = 0.0f;

	// Enable D2D support for this window:
	EnableD2DSupport();

	CreateD2DResources();
	
	// Buttons in velocity pane area
	m_btnArray.Add(CD2DButton(_T("REPRO"), ID_TRACK_ALLREPRO, 0));
	m_btnArray.Add(CD2DButton(_T("SYNC"), ID_TRACK_ALLSYNC, 0));
	m_btnArray.Add(CD2DButton(_T("INPUT"), ID_TRACK_ALLINPUT, 0));
	m_btnArray.Add(CD2DButton(_T("XMON"), ID_TRACK_ALLMONITOROFF, 0));
	m_btnArray.Add(CD2DButton(_T("ZERO"), ID_LOCATOR_RESETCOUNTER, 0));
	m_btnArray.Add(CD2DButton(_T("AUTO"), ID_LOCATOR_AUTOPLAY, 0, 0, 0, CD2DButton::DF_ATOMIC));
	m_btnArray.Add(CD2DButton(_T("LIBW"), ID_TRANSPORT_LIBRARYWIND, 0, 0, 0, CD2DButton::DF_ATOMIC));
	m_btnArray.Add(CD2DButton(_T("STDBY"), ID_TRACK_STANDBYMONITOR, 0, 0, 0, CD2DButton::DF_ATOMIC));
	// Locator Buttons
	m_btnArray.Add(CD2DButton(_T("1"), ID_CUEPOINT_LOC1, ID_EDITTIME_LOC1, 1, STC_L_LOC1, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("2"), ID_CUEPOINT_LOC2, ID_EDITTIME_LOC2, 2, STC_L_LOC2, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("3"), ID_CUEPOINT_LOC3, ID_EDITTIME_LOC3, 3, STC_L_LOC3, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("4"), ID_CUEPOINT_LOC4, ID_EDITTIME_LOC4, 4, STC_L_LOC4, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("5"), ID_CUEPOINT_LOC5, ID_EDITTIME_LOC5, 5, STC_L_LOC5, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("6"), ID_CUEPOINT_LOC6, ID_EDITTIME_LOC6, 6, STC_L_LOC6, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("7"), ID_CUEPOINT_LOC7, ID_EDITTIME_LOC7, 7, STC_L_LOC7, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("8"), ID_CUEPOINT_LOC8, ID_EDITTIME_LOC8, 8, STC_L_LOC8, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("9"), ID_CUEPOINT_LOC9, ID_EDITTIME_LOC9, 9, STC_L_LOC9, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("CUE\nMODE"), ID_LOCATOR_CUEMODE, 0, 0, STC_L_CUE));
	m_btnArray.Add(CD2DButton(_T("0"), ID_CUEPOINT_LOC0, ID_EDITTIME_LOC0, 0, STC_L_LOC0, CD2DButton::DF_BIGTEXT | CD2DButton::DF_ACTIVE));
	m_btnArray.Add(CD2DButton(_T("STORE\nMODE"), ID_LOCATOR_STOREMODE, 0, 0, STC_L_STORE));
	// Locator Command Buttons
	m_btnArray.Add(CD2DButton(_T("AUTO\nPUNCH"), ID_LOCATOR_AUTOPUNCH, 0, 0, STC_M_PUNCH, CD2DButton::DF_MODE_MASK));
	m_btnArray.Add(CD2DButton(_T("PUNCH\nIN"), ID_LOCATOR_PUNCHIN, ID_EDITTIME_PUNCHIN, STC_CUE_POINT_PUNCH_IN, STC_L_PUNCH_IN));
	m_btnArray.Add(CD2DButton(_T("PUNCH\nOUT"), ID_LOCATOR_PUNCHOUT, ID_EDITTIME_PUNCHOUT, STC_CUE_POINT_PUNCH_OUT, STC_L_PUNCH_OUT));
	m_btnArray.Add(CD2DButton(_T("ZERO\nRET"), ID_LOCATOR_ZERORETURN, ID_EDITTIME_RTZ, STC_CUE_POINT_HOME));
	m_btnArray.Add(CD2DButton(_T("AUTO\nLOOP"), ID_LOCATOR_AUTOLOOP, 0, 0, STC_M_LOOP, CD2DButton::DF_MODE_MASK));
	m_btnArray.Add(CD2DButton(_T("MARK\nIN"), ID_LOCATOR_MARKIN, ID_EDITTIME_MARKIN, STC_CUE_POINT_MARK_IN, STC_L_MARK_IN));
	m_btnArray.Add(CD2DButton(_T("MARK\nOUT"), ID_LOCATOR_MARKOUT, ID_EDITTIME_MARKOUT, STC_CUE_POINT_MARK_OUT, STC_L_MARK_OUT));
	m_btnArray.Add(CD2DButton(_T("CAN"), ID_LOCATOR_CANCELSEARCH, 0, 0 ,0, CD2DButton::DF_MODE_CANCEL));
}

CDRCWINView::~CDRCWINView()
{
	FreeD2DResources();
}

///////////////////////////////////////////////////////////////////////////////
// CDRCWINView message handlers

BOOL CDRCWINView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), ::CreateSolidBrush(RGB(0x48, 0x48, 0x48)), NULL);

	//AdjustAll();
	
	return TRUE;
}

void CDRCWINView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	AdjustAll();

	InvalidateRect(NULL, FALSE);
}

void CDRCWINView::OnUpdateIndicatorCoord(CCmdUI* pCmdUI)
{
	CString strMsg;
	strMsg.Format(_T("%.2f, %.2f"), m_mousePos.x, m_mousePos.y);
	pCmdUI->SetText(strMsg.GetBuffer());
}

///////////////////////////////////////////////////////////////////////////////
// CDRCWINView printing

BOOL CDRCWINView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDRCWINView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDRCWINView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

///////////////////////////////////////////////////////////////////////////////
// CXKeyView diagnostics

#ifdef _DEBUG
void CDRCWINView::AssertValid() const
{
	CView::AssertValid();
}

void CDRCWINView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

CDRCWINDoc* CDRCWINView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDRCWINDoc)));
	return (CDRCWINDoc*)m_pDocument;
}

///////////////////////////////////////////////////////////////////////////////
// CXKeyView drawing

void CDRCWINView::OnDraw(CDC* /*pDC*/)
{
	CDRCWINDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

///////////////////////////////////////////////////////////////////////////////
// Device and View Conversion Helpers

void CDRCWINView::CreateD2DResources(void)
{
	// Create brush resources
	
	m_pTextBrush       = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::White, 0.80f));
	
	m_pSolidBrush      = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::White, 1.0f));

	m_pWhiteBrush      = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::White, 0.80f));
	m_pBlackBrush      = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));

	m_pGrayBrush       = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(RGB(0x61, 0x61, 0x61)));
	m_pDarkGrayBrush   = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(RGB(0x22, 0x22, 0x22)));
	m_pLightGrayBrush  = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(RGB(0x81, 0x81, 0x81)));
	
	m_pBackgroundBrush = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(RGB(0x48, 0x48, 0x48)));
	m_pPanelBrush      = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(RGB(0x33, 0x33, 0x33)));

	m_pTapeTimeBrush   = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0xFFFF00, 0.70f));
	
	m_pRecReadyBrush   = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0x550000, 0.50f));
	m_pRecActiveBrush  = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0xFF0000, 1.0f));

	m_pModeInputBrush  = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0xFFC200, 0.70f));
	m_pModeSyncBrush   = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0x2280FF, 0.70f));
	m_pModeReproBrush  = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0x00FF00, 0.50f));
	m_pModeMonBrush    = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::White, 0.75f));

	m_pPlot1Brush      = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::DarkGreen, 0.10f));
	m_pPlot2Brush      = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Green, 0.20f));

	// Create Text Resources

	m_pTextChannelHeading = new CD2DTextFormat(GetRenderTarget(), _T("Arial"), 12, DWRITE_FONT_WEIGHT_BOLD);
	m_pTextChannelHeading->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextChannelHeading->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextSwitchLabel = new CD2DTextFormat(GetRenderTarget(), _T("Arial Narrow"), 9, DWRITE_FONT_WEIGHT_BOLD);
	m_pTextSwitchLabel->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextSwitchLabel->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextInfo = new CD2DTextFormat(GetRenderTarget(), _T("Arial"), 8, DWRITE_FONT_WEIGHT_BOLD);
	m_pTextInfo->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextInfo->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextMode = new CD2DTextFormat(GetRenderTarget(), _T("Arial"), 14, DWRITE_FONT_WEIGHT_BLACK);
	m_pTextMode->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	m_pTextMode->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	m_pTextBigTime = new CD2DTextFormat(GetRenderTarget(), _T("Courier New"), 65, DWRITE_FONT_WEIGHT_BOLD);
	m_pTextBigTime->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	m_pTextBigTime->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextSmallTime = new CD2DTextFormat(GetRenderTarget(), _T("Courier New"), 25, DWRITE_FONT_WEIGHT_BOLD);
	m_pTextSmallTime->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pTextSmallTime->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextTimeHeading = new CD2DTextFormat(GetRenderTarget(), _T("Courier New"), 11, DWRITE_FONT_WEIGHT_BOLD);
	m_pTextTimeHeading->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	m_pTextTimeHeading->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextButton = new CD2DTextFormat(GetRenderTarget(), _T("Arial"), 18,
		DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	m_pTextButton->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextButton->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextButtonSmall = new CD2DTextFormat(GetRenderTarget(), _T("Arial"), 11,
		DWRITE_FONT_WEIGHT_BLACK, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_EXTRA_CONDENSED);
	m_pTextButtonSmall->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextButtonSmall->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextPlotSmall = new CD2DTextFormat(GetRenderTarget(), _T("Arial Narrow"), 8,
		DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	m_pTextPlotSmall->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextPlotSmall->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pTextFormatAxis = new CD2DTextFormat(GetRenderTarget(), _T("Arial"), 11.0f);
	m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pTextFormatAxis->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void CDRCWINView::FreeD2DResources(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// MOUSE HANDLERS

void CDRCWINView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame* pMainFrame = GetMainFrame();

	CD2DPointF ptMouse = GetDevPositionForCurrentDpi(point);

	m_mousePos = ptMouse;

	if (!pMainFrame->IsConnected())
		return;

	for (size_t i=0; i < MAX_TRANSPORT_BUTTONS; i++)
	{
		if (m_btnsTransport[i].Contains(ptMouse))
		{
			m_btnsTransport[i].hover = TRUE;
		}
		else
		{
			if (m_btnsTransport[i].hover)
				m_btnsTransport[i].hover = FALSE;
		}
	}

	for (int i=0; i < m_btnArray.GetCount(); i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		if (btn.Contains(ptMouse))
			btn.hover = TRUE;
		else
			btn.hover = FALSE;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CDRCWINView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame* pMainFrame = GetMainFrame();

	CD2DPointF ptMouse = GetDevPositionForCurrentDpi(point);

	if (!pMainFrame->IsConnected())
		return;

	// Check right click buttons

	for (int i=0; i < m_btnArray.GetCount(); i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		if (btn.Contains(ptMouse))
		    btn.down_r = TRUE;
	}

	InvalidateRect(NULL, FALSE);

	CWnd::OnRButtonDown(nFlags, point);
}


void CDRCWINView::OnLButtonDown(UINT nFlags, CPoint point)
{
	size_t i;
	CMainFrame* pMainFrame = GetMainFrame();

	CD2DPointF ptMouse = GetDevPositionForCurrentDpi(point);

	if (!pMainFrame->IsConnected())
		return;

	if (theApp.m_bShowTracks)
	{
		for (i=0; i < GET_MAX_TRACKS(); i++)
		{
			// Check track arm rectangles for click down
			if (m_btnsTrackArm[i].Contains(ptMouse))
				m_btnsTrackArm[i].down_l = TRUE;

			// Check track mode rectangles for click down
			if (m_btnsTrackMode[i].Contains(ptMouse))
				m_btnsTrackMode[i].down_l = TRUE;

			// Check track monitor rectangles for click down
			if (m_btnsTrackMon[i].Contains(ptMouse))
				m_btnsTrackMon[i].down_l = TRUE;
		}
	}

	// Check transport control buttons for click down
	for (i=0; i < MAX_TRANSPORT_BUTTONS; i++)
	{
		if (m_btnsTransport[i].Contains(ptMouse))
			m_btnsTransport[i].down_l = TRUE;
	}

	// Check locator buttons for click down
	for (int i=0; i < m_btnArray.GetCount(); i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		if (btn.Contains(ptMouse))
			btn.down_l = TRUE;
	}

	InvalidateRect(NULL, FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}


void CDRCWINView::OnRButtonUp(UINT nFlags, CPoint point)
{
	size_t i;
	CMainFrame* pMainFrame = GetMainFrame();

	if (!pMainFrame->IsConnected())
		return;

	CD2DPointF ptMouse = GetDevPositionForCurrentDpi(point);

	// A button was pressed, check the locator control buttons next
	for (i=BUTTON_ID_CUEPOINT_LOC1; i <= BUTTON_ID_LOCATOR_CANCELSEARCH; i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		if (btn.Contains(ptMouse) && btn.down_r)
		{
			if (btn.idCommandAlt)
			{
				pMainFrame->PostMessage(WM_COMMAND, btn.idCommandAlt, 0);
				break;
			}
		}
	}

	// Reset click down state flags
	for (int i=0; i < m_btnArray.GetCount(); i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);
		btn.down_r = FALSE;
	}

	InvalidateRect(NULL, FALSE);

	CWnd::OnRButtonUp(nFlags, point);
}

void CDRCWINView::OnLButtonUp(UINT nFlags, CPoint point)
{
	size_t i;
	CMainFrame* pMainFrame = GetMainFrame();

	if (!pMainFrame->IsConnected())
		return;

	CD2DPointF ptMouse = GetDevPositionForCurrentDpi(point);

	// DO TRANSPORT BUTTON HIT TEST

	if (m_btnsTransport[0].Contains(ptMouse) && m_btnsTransport[0].down_l)
		pMainFrame->PostMessage(WM_COMMAND, ID_TRANSPORT_RECORD, 0);
	else if (m_btnsTransport[1].Contains(ptMouse) && m_btnsTransport[1].down_l)
		pMainFrame->PostMessage(WM_COMMAND, ID_TRANSPORT_PLAY, 0);
	else if (m_btnsTransport[2].Contains(ptMouse) && m_btnsTransport[2].down_l)
		pMainFrame->PostMessage(WM_COMMAND, ID_TRANSPORT_REWIND, 0);
	else if (m_btnsTransport[3].Contains(ptMouse) && m_btnsTransport[3].down_l)
		pMainFrame->PostMessage(WM_COMMAND, ID_TRANSPORT_FORWARD, 0);
	else if (m_btnsTransport[4].Contains(ptMouse) && m_btnsTransport[4].down_l)
		pMainFrame->PostMessage(WM_COMMAND, ID_TRANSPORT_STOP, 0);
	else if (m_btnsTransport[5].Contains(ptMouse) && m_btnsTransport[5].down_l)
		pMainFrame->PostMessage(WM_COMMAND, ID_TRANSPORT_TAPELIFTER, 0);

	// Reset click down state flags
	for (i = 0; i < MAX_TRANSPORT_BUTTONS; i++)
	{
		m_btnsTransport[i].down_l = FALSE;
		m_btnsTransport[i].down_r = FALSE;
	}
	
	// DO TRACK ASSIGN BUTTON HIT TEST

	if (theApp.m_bShowTracks)
	{
		for (i = 0; i < GET_MAX_TRACKS(); i++)
		{
			if (m_btnsTrackArm[i].Contains(ptMouse) && m_btnsTrackArm[i].down_l)
			{
				pMainFrame->PostMessage(WM_COMMAND, pMainFrame->GetRecordMenuID((UINT)i), 0);
				break;
			}

			if (m_btnsTrackMode[i].Contains(ptMouse) && m_btnsTrackMode[i].down_l)
			{
				pMainFrame->PostMessage(WM_COMMAND, ID_TOGGLEMODE_TRACK1 + i, 0);
				break;
			}

			if (m_btnsTrackMon[i].Contains(ptMouse) && m_btnsTrackMon[i].down_l)
			{
				pMainFrame->PostMessage(WM_COMMAND, pMainFrame->GetMonitorMenuID((UINT)i), 0);
				break;
			}

			// Reset track arm and track mode click down state flags
			m_btnsTrackArm[i].down_l = FALSE;
			m_btnsTrackMode[i].down_l = FALSE;
			m_btnsTrackMon[i].down_l = FALSE;
		}
	}

	// DO LOCATE AND OTHER BUTTON HIT TESTS

	for (int i = 0; i < m_btnArray.GetCount(); i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		if (btn.Contains(ptMouse) && btn.down_l)
			pMainFrame->PostMessage(WM_COMMAND, btn.idCommand, 0);
		
		btn.down_l = FALSE;
	}

	InvalidateRect(NULL, FALSE);

	CWnd::OnLButtonUp(nFlags, point);
}

void CDRCWINView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// DRAWING

afx_msg LRESULT CDRCWINView::OnDraw2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	ASSERT_VALID(pRenderTarget);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();
	
	// Fill the entire background area with solid color
	CRect rectClient;
	GetClientRect(rectClient);
	pRenderTarget->FillRectangle(rectClient, m_pBackgroundBrush);

	// Draw the control panels over background
	DrawPanels(pRenderTarget);
	
	// Draw the current tape time information
	DrawTapeTime(pRenderTarget);

	// Draw the current tape mode in operation
	DrawTapeMode(pRenderTarget);

	// If locating, draw the progress bar
	if (theApp.m_bShowTimecode)
		DrawSMPTETime(pRenderTarget);
	else
		DrawProgressBar(pRenderTarget);

	// Draw the velocity graph
	if (theApp.m_nShowVelocity)
		DrawVelocity(pRenderTarget);

	// Draw the track assign button states
	if (theApp.m_bShowTracks)
		DrawTrackAssignButtons(pRenderTarget);

	// Draw the transport control button states
	DrawTransportControlButtons(pRenderTarget);

	// Draw the track and utility button states
	DrawTrackUtils(pRenderTarget);

	// Draw the locator buttons in current state
	DrawLocatorButtons(pRenderTarget);

	return TRUE;
}


void CDRCWINView::DrawPanels(CHwndRenderTarget* pRenderTarget)
{
	CD2DSizeF sizeRadius(5.0f, 5.0f);
	CD2DSizeF sizeRadius2(2.50f, 2.50f);

	// Draw the background panel areas
	if (theApp.m_bShowTracks)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_rectPanel1, sizeRadius), m_pPanelBrush);
	pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_rectPanel2, sizeRadius), m_pPanelBrush);
	pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_rectPanel3, sizeRadius), m_pPanelBrush);

	// Fill the tape time pane
	pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_rectTimePane, sizeRadius), m_pBlackBrush);
	
	// Fill the info pane
	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black, 0.2f);
	m_pSolidBrush->SetColor(color);
	pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_rectInfoPane, sizeRadius), m_pSolidBrush);
}


void CDRCWINView::DrawTrackAssignButtons(CHwndRenderTarget* pRenderTarget)
{
	uint8_t mode;
	CString str;
	CD2DSolidColorBrush* brushFace;
	CD2DSolidColorBrush* brushText;
	CD2DSizeF sizeRadius2(2.50f, 2.50f);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	size_t numTracks = (size_t)GET_MAX_TRACKS();

	BOOL enabled = pMainFrame->IsTrackControllerReady();

	for (size_t i = 0; i < numTracks; i++)
	{
		//-----------------------------------------------------------
		// ROW-1: Draw the channel# heading
		//-----------------------------------------------------------

		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTrackHeadings[i], sizeRadius2), m_pDarkGrayBrush);
		
		if (i < numTracks)
		{
			str.Format(_T("%u"), (int)(i + 1));
			pRenderTarget->DrawText(str, m_btnsTrackHeadings[i], m_pWhiteBrush, m_pTextChannelHeading);
		}

		//-----------------------------------------------------------
		// ROW-2: Draw the channel ready/record status button
		//-----------------------------------------------------------

		if (state.trackState[i] & STC_T_READY)
		{
			if ((state.trackState[i] & STC_T_RECORD) && (state.transportMode & STC_M_RECORD))
			{
				str = _T("REC");
				brushFace = m_pRecActiveBrush;
			}
			else
			{
				str = _T("RDY");
				brushFace = m_pRecReadyBrush;
			}
		}
		else
		{
			str = _T("SAFE");
			brushFace = m_pGrayBrush;
		}

		if (i >= numTracks)
			brushFace = m_pGrayBrush;

		brushText = (enabled) ? m_pWhiteBrush : m_pDarkGrayBrush;

		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTrackArm[i], sizeRadius2), brushFace);

		if (i < numTracks)
		{
			pRenderTarget->DrawText(str, m_btnsTrackArm[i], brushText, m_pTextSwitchLabel);
		}
		
		//-----------------------------------------------------------
		// ROW-3: Draw the current track mode status button
		//-----------------------------------------------------------

		mode = (state.trackState[i] & STC_TRACK_MASK);

		if (state.trackState[i] & STC_T_STANDBY)
		{
			if (state.trackState[i] & STC_T_MONITOR)
				mode = STC_TRACK_INPUT;
		}

		switch (mode)
		{
		case STC_TRACK_INPUT:
			brushFace = m_pModeInputBrush;
			str = _T("INP");
			break;
		case STC_TRACK_REPRO:
			brushFace = m_pModeReproBrush;
			str = _T("REPR");
			break;
		case STC_TRACK_SYNC:
			brushFace = m_pModeSyncBrush;
			str = _T("SYNC");
			break;
		default:
			brushFace = m_pGrayBrush;
			str = _T(" ");
			break;
		}
		
		if (!enabled || (i >= numTracks))
		{
			brushFace = m_pGrayBrush;
			brushText = m_pDarkGrayBrush;
		}
		else
		{
			//brushText = m_pPanelBrush;
		}
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTrackMode[i], sizeRadius2), brushFace);

		if (i < numTracks)
		{
			pRenderTarget->DrawText(str, m_btnsTrackMode[i], brushText, m_pTextSwitchLabel);
		}

		if (!enabled || (i >= numTracks))
		{
			brushFace = m_pGrayBrush;
			brushText = m_pDarkGrayBrush;
		}
		else
		{
			// Draw input monitor mode according to it's state
			if (state.trackState[i] & STC_T_MONITOR)
			{
				if (state.trackState[i] & STC_T_STANDBY)
				{
					//brushFace = m_pWhiteBrush;
					//brushText = m_pDarkGrayBrush;
				}
				else
				{
					brushFace = m_pModeMonBrush;
					brushText = m_pBlackBrush;
				}
			}
			else
			{
				brushFace = m_pLightGrayBrush;
				brushText = m_pDarkGrayBrush;
			}
		}
		
		//-----------------------------------------------------------
		// ROW-4: Draw the monitor mode button and text
		//-----------------------------------------------------------

		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTrackMon[i], sizeRadius2), brushFace);

		//str = (state.trackState[i] & STC_T_STANDBY) ? _T("INP") : _T("MON");
		str = _T("MON");

		if (i < numTracks)
		{
			pRenderTarget->DrawText(str, m_btnsTrackMon[i], brushText, m_pTextSwitchLabel);
		}
	}
}


void CDRCWINView::DrawTransportControlButtons(CHwndRenderTarget* pRenderTarget)
{
	CD2DSolidColorBrush* brush;
	CD2DSizeF sizeRadius2(2.50f, 2.50f);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	// DRAW THE BIG TRANSPORT CONTROL BUTTONS

	//if (state.ledMaskTransport & STC_L_REC)
	if (state.transportMode & STC_M_RECORD)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTransport[0], sizeRadius2), m_pRecActiveBrush);

	if ((state.ledMaskTransport & STC_L_PLAY) || m_btnsTransport[1].down_l)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTransport[1], sizeRadius2), m_pGrayBrush);

	if ((state.ledMaskTransport & STC_L_REW) || m_btnsTransport[2].down_l)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTransport[2], sizeRadius2), m_pGrayBrush);

	if ((state.ledMaskTransport & STC_L_FWD) || m_btnsTransport[3].down_l)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTransport[3], sizeRadius2), m_pGrayBrush);

	if ((state.ledMaskTransport & STC_L_STOP) || m_btnsTransport[4].down_l)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTransport[4], sizeRadius2), m_pGrayBrush);

	if ((state.transportMode & STC_M_LIFTERS) || m_btnsTransport[5].down_l)
		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(m_btnsTransport[5], sizeRadius2), m_pGrayBrush);

	for (size_t i=0; i < MAX_TRANSPORT_BUTTONS; i++)
	{
		static TCHAR* szButtonText[MAX_TRANSPORT_BUTTONS] = {
			_T("R E C"),		// 0
			_T("P L A Y"),		// 1
			_T("R E W"),		// 2
			_T("F W D"),		// 3
			_T("S T O P"),		// 4
			_T("LDEF")			// 5
		};

		if (!pMainFrame->IsConnected())
		{
			brush = m_pDarkGrayBrush;
		}
		else
		{
			brush = m_pWhiteBrush;

			if (i == 0)
			{
				//if (state.ledMaskTransport & STC_L_REC)
				if (state.transportMode & STC_M_RECORD)
					brush = m_pWhiteBrush;
				else if (pMainFrame->m_bRecord)
					brush = m_pRecActiveBrush;
			}

			if (!pMainFrame->IsTransportReady() && (i < 4))
				brush = m_pDarkGrayBrush;
		}

		if (i == MAX_TRANSPORT_BUTTONS - 1)
			pRenderTarget->DrawText(szButtonText[i], m_btnsTransport[i], brush, m_pTextButtonSmall);
		else
			pRenderTarget->DrawText(szButtonText[i], m_btnsTransport[i], brush, m_pTextButton);

		if (m_btnsTransport[i].down_l)
			pRenderTarget->DrawRoundedRectangle(CD2DRoundedRect(m_btnsTransport[i], sizeRadius2), m_pWhiteBrush);
		else
			pRenderTarget->DrawRoundedRectangle(CD2DRoundedRect(m_btnsTransport[i], sizeRadius2), m_pLightGrayBrush, 0.75f);
	}
}


void CDRCWINView::DrawLocatorButtons(CHwndRenderTarget* pRenderTarget)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CD2DSolidColorBrush* brush;
	CD2DSizeF sizeRadius2(2.50f, 2.50f);

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	// DRAW THE LOCATOR BUTTONS

	for (int i=BUTTON_ID_CUEPOINT_LOC1; i <= BUTTON_ID_LOCATOR_CANCELSEARCH; i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);
		
		if (btn.flags & CD2DButton::DF_MODE_CANCEL)
		{
			// For cancel button we need to check if the transport is searching
			// or looping before drawing in the solid background.

			if (pMainFrame->IsTransportSearching() || pMainFrame->IsTransportLooping() || btn.down_l)
				pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(btn, sizeRadius2), m_pGrayBrush);
		}
		else if (btn.flags & CD2DButton::DF_MODE_MASK)
		{
			// Check for transport mode mask option before
			// drawing in the solid background.

			if ((state.transportMode & btn.mask) || btn.down_l)
				pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(btn, sizeRadius2), m_pGrayBrush);
		}
		else
		{
			// Otherwise check LED mask before drawing in background

			if ((state.ledMaskButton & btn.mask) || btn.down_l)
				pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(btn, sizeRadius2), m_pGrayBrush);
		}

		brush = m_pWhiteBrush;

		if (!pMainFrame->IsConnected())
		{
			brush = m_pDarkGrayBrush;
		}
		else
		{
			// Is it a cue point button?
			if (btn.flags & CD2DButton::DF_ACTIVE)
			{
				// Map cue state index from value in button table
				size_t n = btn.index;

				// If cue point button is active, draw it in white
				if (!(state.cueState[n] & STC_CF_ACTIVE))
					brush = m_pDarkGrayBrush;
			}
		}

		// Draw digits in large font, all other buttons use smaller font
		if (btn.flags & CD2DButton::DF_BIGTEXT)
			pRenderTarget->DrawText(btn.label, btn, brush, m_pTextButton);
		else
			pRenderTarget->DrawText(btn.label, btn, brush, m_pTextButtonSmall);

		// Draw outside rect in white to show click down state
		if (btn.down_l)
			pRenderTarget->DrawRoundedRectangle(CD2DRoundedRect(btn, sizeRadius2), m_pWhiteBrush);
		else
			pRenderTarget->DrawRoundedRectangle(CD2DRoundedRect(btn, sizeRadius2), m_pLightGrayBrush, 0.75f);
	}
}


void CDRCWINView::DrawTrackUtils(CHwndRenderTarget* pRenderTarget)
{
	CString str;

	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DButton& btnAuto  = m_btnArray.ElementAt(BUTTON_ID_LOCATOR_AUTOPLAY);
	CD2DButton& btnLibw  = m_btnArray.ElementAt(BUTTON_ID_TRANSPORT_LIBRARYWIND);
	CD2DButton& btnStdby = m_btnArray.ElementAt(BUTTON_ID_TRACK_STANDBYMONITOR);

	// Draw the AUTO play button

	CD2DSolidColorBrush* brush = (pMainFrame->IsConnected()) ? m_pLightGrayBrush : m_pGrayBrush;
	
	pRenderTarget->DrawRectangle(btnAuto, brush);
	
	if (pMainFrame->IsConnected())
	{
		if (pMainFrame->IsAutoPlay() || btnAuto.down_l)
		{
			pRenderTarget->FillRectangle(btnAuto, m_pLightGrayBrush);
			brush = m_pDarkGrayBrush;
		}
		else
		{
			brush = m_pLightGrayBrush;
		}
	}

	pRenderTarget->DrawText(btnAuto.label, btnAuto, brush, m_pTextInfo);

	// Draw the LIBW button

	brush = (pMainFrame->IsConnected()) ? m_pLightGrayBrush : m_pGrayBrush;

	pRenderTarget->DrawRectangle(btnLibw, brush);

	if (pMainFrame->IsConnected())
	{
		if (pMainFrame->IsLibWind() || btnLibw.down_l)
		{
			pRenderTarget->FillRectangle(btnLibw, m_pLightGrayBrush);
			brush = m_pDarkGrayBrush;
		}
		else
		{
			brush = m_pLightGrayBrush;
		}
	}

	pRenderTarget->DrawText(btnLibw.label, btnLibw, brush, m_pTextInfo);

	// Draw the STDBY button

	brush = (pMainFrame->IsConnected()) ? m_pLightGrayBrush : m_pGrayBrush;

	pRenderTarget->DrawRectangle(btnStdby, brush);

	if (pMainFrame->IsConnected())
	{
		if (pMainFrame->IsStandbyMonitor() || btnStdby.down_l)
		{
			pRenderTarget->FillRectangle(btnStdby, m_pLightGrayBrush);
			brush = m_pDarkGrayBrush;
		}
		else
		{
			brush = m_pLightGrayBrush;
		}
	}

	pRenderTarget->DrawText(btnStdby.label, btnStdby, brush, m_pTextInfo);

	// Draw all other non-atomic draw buttons

	for (int i= BUTTON_ID_TRACK_ALLREPROMODE; i <= BUTTON_ID_TRACK_STANDBYMONITOR; i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		if (btn.flags & CD2DButton::DF_ATOMIC)
			continue;

		brush = (pMainFrame->IsConnected()) ? m_pLightGrayBrush : m_pGrayBrush;

		if (pMainFrame->IsConnected())
		{
			if (btn.IsLeftClickDown())
			{
				brush = m_pDarkGrayBrush;
				pRenderTarget->FillRectangle(btn, m_pLightGrayBrush);
			}
			else
			{
				brush = m_pLightGrayBrush;
			}
		}

		pRenderTarget->DrawRectangle(btn, brush);
		pRenderTarget->DrawText(btn.label, btn, brush, m_pTextInfo);
	}

#if (DEBUG_SHOW_RECTS > 0)
	//pRenderTarget->DrawRectangle(m_rectInfo1, m_pWhiteBrush, 0.5f);
	//pRenderTarget->DrawRectangle(m_rectInfo2, m_pWhiteBrush, 0.5f);
	//pRenderTarget->DrawRectangle(m_rectInfo3, m_pWhiteBrush, 0.5f);
	pRenderTarget->DrawRectangle(m_rectInfoPane, m_pWhiteBrush, 0.5f);
#endif
}


void CDRCWINView::DrawCaptionText(CHwndRenderTarget* pRenderTarget,
	FLOAT xPos, FLOAT yPos,
	FLOAT width1, CString strText1,
	FLOAT width2, CString strText2)
{
	FLOAT alpha;
	D2D1_COLOR_F color;

	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	CD2DRectF rect1(xPos, yPos, xPos + width1, yPos + 14.0f);
	CD2DRectF rect2(xPos + width1 + 1, yPos, xPos + width1 + width2 + 1, rect1.bottom);
	
	alpha = 0.75f;

	CD2DSolidColorBrush* pBrush = (pMainFrame->IsConnected()) ? m_pTapeTimeBrush : m_pDarkGrayBrush;

	color = D2D1::ColorF(D2D1::ColorF::White, alpha);
	m_pSolidBrush->SetColor(color);
	pRenderTarget->FillRectangle(rect1, pBrush);

	color = D2D1::ColorF(D2D1::ColorF::Black, alpha);
	m_pSolidBrush->SetColor(color);
	pRenderTarget->FillRectangle(rect2, m_pSolidBrush);

	FLOAT inset = 1.0f;

	rect1.top += inset - 0.5f;
	rect1.left += inset + 2.0f;
	rect1.bottom -= inset;
	rect1.right -= inset;

	rect2.top += inset - 0.5f;
	rect2.left += inset + 2.0f;
	rect2.bottom -= inset;
	rect2.right -= inset;

	m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	color = D2D1::ColorF(D2D1::ColorF::Black, alpha);
	m_pTextBrush->SetColor(color);
	pRenderTarget->DrawText(strText1, rect1, m_pTextBrush, m_pTextFormatAxis, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	pRenderTarget->DrawText(strText2, rect2, pBrush, m_pTextFormatAxis, D2D1_DRAW_TEXT_OPTIONS_CLIP);
}


void CDRCWINView::DrawVelocity(CHwndRenderTarget* pRenderTarget)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DRectF rect = m_rectInfoPlot;

	rect.left += 5.0f;
	rect.top += 5.0f;
	rect.right -= 5.0f;
	rect.bottom -= 5.0f;

	// Draw the graph first
	DrawVelocityGraph(pRenderTarget, rect);

	// Now plot the velocity graph data
	DrawVelocityPlot(pRenderTarget, rect);

	if (state.errorCount > 0)
	{
		CString str;
		str.Format(_T("ENCODER ERRORS\n%u"), state.errorCount);
		pRenderTarget->DrawText(str, rect, m_pRecActiveBrush, m_pTextButtonSmall);
	}
}


void CDRCWINView::DrawVelocityGraph(CHwndRenderTarget* pRenderTarget, CD2DRectF rect)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	CD2DSolidColorBrush* brush = m_pLightGrayBrush;
	
	// Plot the graph first

	rect.left += 8.0f;

	pRenderTarget->DrawRectangle(rect, brush, 0.50);

	rect.left += 0.50f;
	rect.top += 0.50f;
	rect.right -= 0.50f;
	rect.bottom -= 0.50f;

	float h = rect.bottom - rect.top;
	float w = rect.right - rect.left;

	brush = m_pGrayBrush;

	// draw vertical grid lines
	for (int i=0; i < 10; i++)
	{
		float x = (w / 11.0f) * (float)(i + 1);

		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left + x, rect.top),
			D2D1::Point2F(rect.left + x, rect.bottom),
			brush,
			0.25f);
	}

	// draw horizontal grid lines
	for (int i=0; i < 3; i++)
	{
		float y = (h / 4.0f) * (float)(i + 1);

		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left, rect.top + y),
			D2D1::Point2F(rect.right, rect.top + y),
			brush,
			0.25f);
	}

	// add the labels
	CD2DRectF rectText(rect.left - 11.0f, rect.top, rect.left - 3.0f, rect.bottom);
	pRenderTarget->DrawText(_T("VEL"), rectText, m_pGrayBrush, m_pTextPlotSmall);
	//pRenderTarget->DrawRectangle(rectText, m_pWhiteBrush, 0.5f);
}


void CDRCWINView::DrawVelocityPlot(CHwndRenderTarget* pRenderTarget, CD2DRectF rect)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DSolidColorBrush* brush1 = m_pPlot1Brush;
	CD2DSolidColorBrush* brush2 = m_pPlot2Brush;
	
	if (!pMainFrame->IsConnected())
	{
		brush1 = m_pDarkGrayBrush;
		brush2 = m_pDarkGrayBrush;
		return;
	}

	// Now plot the velocity graph data

	rect.left += 2.0f + 8.0f;
	rect.top += 2.0f;
	rect.right -= 2.0f;
	rect.bottom -= 2.0f;

	float h = rect.bottom - rect.top;
	float w = rect.right - rect.left;

	float step = w / (float)MAX_VEL_SAMPLES;

	for (size_t i=0; i < MAX_VEL_SAMPLES; i++)
	{
		float x = (float)i * step;

		float v = pMainFrame->m_velSample[i];

		if (v > 100.0f)
			v = 100.0f;

		float y1 = rect.bottom;
		float y2 = rect.top;

		float val = (FLOAT)v * 0.01f;

		float y = ((y2 - y1) * val) + y1;
#if 0
		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left + x, rect.top), 		// draw from point
			D2D1::Point2F(rect.left + x, y),				// draw to point
			brush1,
			1.0f);
#endif
		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left + x, rect.bottom),		// draw from point
			D2D1::Point2F(rect.left + x, y),				// draw to point
			brush2,
			1.0f);

		// Now draw black line indicating progress

		if (state.searching)
		{
			FLOAT x1 = rect.left;
			FLOAT x2 = rect.right;

			FLOAT progress = (FLOAT)state.searchProgress * 0.01f;

			FLOAT x = ((x2 - x1) * progress) + x1;

			if (x > x2)
				x = x2;

			if (x < x1)
				x = x1;

			pRenderTarget->DrawLine(
				D2D1::Point2F(x, rect.top),		// draw from point
				D2D1::Point2F(x, rect.bottom),	// draw to point
				m_pLightGrayBrush,
				1.0f);
		}
	}
}


void CDRCWINView::DrawTapeTime(CHwndRenderTarget* pRenderTarget)
{
	TCHAR szBuf[64];

	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	TCHAR sign = (state.tapeTime.flags & F_TAPETIME_PLUS) ? '+' : '-';

	CD2DSolidColorBrush* pBrush = (pMainFrame->IsConnected()) ? m_pTapeTimeBrush : m_pDarkGrayBrush;

	// Draw the tape time +h:mm:ss in big font

	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("%c%1u:%2.2u:%2.2u"),
		sign,
		state.tapeTime.hour,
		state.tapeTime.mins,
		state.tapeTime.secs);

	pRenderTarget->DrawText(szBuf, m_rectTimeBig, pBrush, m_pTextBigTime);

	// Draw the tape time tens of secs in smaller font
	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T(":%u"), state.tapeTime.tens);
	pRenderTarget->DrawText(szBuf, m_rectTimeSmall, pBrush, m_pTextSmallTime);

	CD2DRectF rectHeading(m_rectTimeBig.left, m_rectTimeBig.top - 18,
		                  m_rectTimeBig.right, m_rectTimeBig.top + 8);

	rectHeading.right = rectHeading.left + 130;
	pRenderTarget->DrawText(_T("HR"), rectHeading, pBrush, m_pTextTimeHeading);
	
	rectHeading.right += 100;
	pRenderTarget->DrawText(_T("MIN"), rectHeading, pBrush, m_pTextTimeHeading);
	
	rectHeading.right += 118;
	pRenderTarget->DrawText(_T("SEC"), rectHeading, pBrush, m_pTextTimeHeading);
	
	rectHeading.right += 62;
	pRenderTarget->DrawText(_T("TEN"), rectHeading, pBrush, m_pTextTimeHeading);

#if 0	//(DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(rectHeading, m_pWhiteBrush, 0.5f);		// DEBUG
	pRenderTarget->DrawRectangle(m_rectTimeBig, m_pWhiteBrush, 0.5f);		// DEBUG
	pRenderTarget->DrawRectangle(m_rectTimeSmall, m_pWhiteBrush, 0.5f);		// DEBUG
#endif

	// Display the current tape speed

	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("%u IPS"), state.tapeSpeed);

	DrawCaptionText(pRenderTarget, 
		m_rectTimePane.left + 10.0f, m_rectTimePane.top + 10.0f,
		45.0f, _T("SPEED:"), 150.0f, szBuf);

	// Display the tape width

	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("%u IN"), state.tapeSize);

	DrawCaptionText(pRenderTarget, 
		m_rectTimePane.left + 100.0f, m_rectTimePane.top + 10.0f,
		35.0f, _T("TAPE:"), 130.0f, szBuf);
	
	// Display the SMPTE controller status

	if (state.hardwareFlags & STC_HF_SMPTE)
	{
		if (state.smpteMode == 1)
			_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("STRIPE"));
		else if (state.smpteMode == 2)
			_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("SLAVE"));
		else
			_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("OFF"));

		DrawCaptionText(pRenderTarget,
			m_rectTimePane.left + 170.0f, m_rectTimePane.top + 10.0f,
			45.0f, _T("SMPTE:"), 130.0f, szBuf);
	}

#if 0
	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("%X"), state.hardwareFlags);
	DrawCaptionText(pRenderTarget, m_rectTimePane.left + 170.0f, m_rectTimePane.top + 10.0f,
		45.0f, _T("SMPTE:"), 170.0f, szBuf);
#endif

	// Now draw the actual date and time 
	
	if (theApp.m_bShowTimeDate)
	{
		TCHAR buf[32];
		TCHAR ampm[8];

		if (theApp.m_bUseTapeMachineTime)
		{
			DWRITE_TEXT_ALIGNMENT align = m_pTextFormatAxis->Get()->GetTextAlignment();

			uint8_t hour = state.dateTime.hour;

			_tcscpy_s(ampm, _T("AM"));

			// Convert to 12 hour format
			if (hour >= 12)
			{
				_tcscpy_s(ampm, _T("PM"));
				hour -= 12;
			}

			// Adjust if midnight hour.
			if (hour == 0)
				hour = 12;

			_snwprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("%1u:%-2.2u %s"),
				hour,
				state.dateTime.min,
				ampm);

			m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			pRenderTarget->DrawText(buf, m_rectTimeProgress, pBrush, m_pTextFormatAxis);

			_snwprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("%u/%u/%4.4u"),
				state.dateTime.month + 1,
				state.dateTime.date + 1,
				state.dateTime.year + 2000);

			m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			pRenderTarget->DrawText(buf, m_rectTimeProgress, pBrush, m_pTextFormatAxis);

			m_pTextFormatAxis->Get()->SetTextAlignment(align);
		}
		else
		{
			time_t ltime;
			struct tm today;

			time(&ltime);

			_tcscpy_s(ampm, _T("AM"));

			if (_localtime64_s(&today, &ltime) == 0)
			{
				// Convert to 12 hour format
				if (today.tm_hour >= 12)
				{
					_tcscpy_s(ampm, _T("PM"));
					today.tm_hour -= 12;
				}

				// Adjust if midnight hour.
				if (today.tm_hour == 0)
					today.tm_hour = 12;

				DWRITE_TEXT_ALIGNMENT align = m_pTextFormatAxis->Get()->GetTextAlignment();

				_snwprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("%1u:%-2.2u %s"),
					today.tm_hour, today.tm_min, ampm);

				m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				pRenderTarget->DrawText(buf, m_rectTimeProgress, pBrush, m_pTextFormatAxis);

				_snwprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("%u/%u/%4.4u"),
					today.tm_mon + 1, today.tm_mday, today.tm_year + 1900);

				m_pTextFormatAxis->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				pRenderTarget->DrawText(buf, m_rectTimeProgress, pBrush, m_pTextFormatAxis);

				m_pTextFormatAxis->Get()->SetTextAlignment(align);
			}
		}
	}
#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectTimeSmall, m_pWhiteBrush, 0.5f);		// DEBUG
#endif
}


void CDRCWINView::DrawSMPTETime(CHwndRenderTarget* pRenderTarget)
{
	TCHAR szBuf[64];

	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DSolidColorBrush* pBrush = (pMainFrame->IsConnected()) ? m_pTapeTimeBrush : m_pDarkGrayBrush;

	// Draw the tape time +h:mm:ss in big font

	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("%2.2u:%2.2u:%2.2u:%2.2u"),
		state.smpteTime.hour,
		state.smpteTime.mins,
		state.smpteTime.secs,
		state.smpteTime.frame);
	
	DWRITE_TEXT_ALIGNMENT align = m_pTextSmallTime->Get()->GetTextAlignment();
	
	m_pTextSmallTime->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	
	pRenderTarget->DrawText(szBuf, m_rectTimeProgress, pBrush, m_pTextSmallTime);

	m_pTextSmallTime->Get()->SetTextAlignment(align);
}


void CDRCWINView::DrawTapeMode(CHwndRenderTarget* pRenderTarget)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CString str;
	CD2DSolidColorBrush* pBrush;
	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	// Get the current transport mode text.
	GetModeText(state, str);

	// Check if we're connected 
	if (!pMainFrame->IsConnected())
	{
		pBrush = m_pDarkGrayBrush;
	}
	else
	{
		// Default brush to use for text
		pBrush = m_pTapeTimeBrush;
		
		// If the transport is in halt, thread or record mode, display text in RED
		if ((pMainFrame->IsTransportReady() == FALSE) || 
			(pMainFrame->IsTransportReady() && (state.transportMode & STC_M_RECORD)))
		{
			D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Red, 0.9f);
			m_pSolidBrush->SetColor(color);
			pBrush = m_pSolidBrush;
		}
#if 0
		else
		{	
			// If any track is armed for record, draw text in dark red
			// to indicate that at least one track is armed for record.
			// Subsequently, pressing the rec+play buttons will engage
			// record on all tracks that are armed and ready.

			if (pMainFrame->IsAnyTracksArmed())
			{
				D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::DarkRed, 0.9f);
				m_pSolidBrush->SetColor(color);
				pBrush = m_pSolidBrush;
			}
		}
#endif
	}

	pRenderTarget->DrawText(str, m_rectTimeMode, pBrush, m_pTextMode);

#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectTimeMode, m_pWhiteBrush, 0.5f);	// DEBUG
#endif
}


void CDRCWINView::DrawProgressBar(CHwndRenderTarget* pRenderTarget)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DSolidColorBrush* pBrush = (pMainFrame->IsConnected()) ? m_pTapeTimeBrush : m_pDarkGrayBrush;

	CD2DRectF rect = m_rectTimeProgress;

	rect.left   += 100.0f;
	rect.top    += 2.0f;
	rect.right  -= 100.0f;
	rect.bottom -= 4.0f;

	if ((state.transportMode & STC_M_SEARCH) && (pMainFrame->IsConnected()))
	{
		CD2DRectF rect2(rect);

		/* Draw progress bar */
		rect2.left   += 2.0f;
		rect2.top    += 2.0f;
		rect2.right  -= 2.0f;
		rect2.bottom -= 2.0f;

		FLOAT x1 = rect2.left;
		FLOAT x2 = rect2.right;

		FLOAT progress = (FLOAT)state.searchProgress * 0.01f;

		FLOAT x = ((x2 - x1) * progress) + x1;

		if (x > x2)
			x = x2;

		if (x < x1)
			x = x1;

		rect2.right = x - 1.0f;

		pRenderTarget->DrawRectangle(rect, pBrush, 0.5f);
		pRenderTarget->FillRectangle(rect2, pBrush);
	}
	else
	{
		FLOAT alpha = (pMainFrame->IsConnected()) ? 0.25f : 0.75f;

		pRenderTarget->DrawRectangle(rect, pBrush, alpha);
	}

#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectTimeProgress, m_pGrayBrush, 1.0f);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Helper Methods

void CDRCWINView::GetModeText(STC_STATE_MSG& msg, CString& str)
{
	UINT32 mode = (msg.transportMode & STC_MODE_MASK);

	switch (mode)
	{
	case STC_MODE_HALT:
		str = _T("TAPE OUT");
		break;

	case STC_MODE_THREAD:
		str = _T("THREAD");
		break;

	case STC_MODE_STOP:
		str = _T("STOP");
		break;

	case STC_MODE_PLAY:
		str = _T("PLAY");
		break;

	case STC_MODE_FWD:
		str = _T("FWD");
		break;

	case STC_MODE_REW:
		str = _T("REW");
		break;

	default:
		str = _T("???");
		break;
	}
	
	// Add, or set, any transport mode suffix modifiers

	if (msg.transportMode & STC_M_RECORD)
		str = _T("RECORD");
	
	if (msg.transportMode & STC_M_LIBWIND)
		str = _T("LIB-WIND");

	if (msg.transportMode & STC_M_SEARCH)
		str = _T("SEARCH");

	if (msg.transportMode & STC_M_LOOP)
		str = _T("LOOP");
}

void CDRCWINView::PersistRatio(
	const CD2DSizeF& orig,
	CD2DSizeF& dest,
	CD2DPointF& remainder)
{
	FLOAT newSize;
	FLOAT ratio1 = (FLOAT)(orig.width / orig.height);
	FLOAT ratio2 = (FLOAT)(dest.width / dest.height);

	// Do nothing if they are the same
	if (ratio1 > ratio2)
	{
		// Shrink hieght
		newSize = dest.width / ratio1;
		remainder.x = 0;
		remainder.y = dest.height - newSize;
		dest.width = newSize;
	}
	else if (ratio2 > ratio1)
	{
		// Shrink width
		newSize = dest.height * ratio1;
		remainder.x = dest.width - newSize;
		remainder.y = 0;
		dest.width = newSize;
	}
}


void CDRCWINView::AdjustAll(void)
{
	int count;
	CRect rectClient;
	GetClientRect(rectClient);

	// Setup the panel1 rectangle area that goes under all the 
	// track assignment buttons. This panel gets filled with dark
	// background shade color.

	m_rectPanel1 = CD2DRectF(rectClient.left + 5.0f, rectClient.top + 5.0f, rectClient.right - 5.0f, 130.0f);
	
	// === SETUP TRACK NUMBER HEADING RECTS ===

	FLOAT space  = 5.0f;
	FLOAT width  = 35.0f;
	FLOAT xPos   = 10.0f;
	FLOAT yPos   = 10.0f;
	FLOAT height = 18.0f;

	FLOAT block = ((m_rectPanel1.right - space) - m_rectPanel1.left) / (float)GET_MAX_TRACKS();
	
	if (theApp.m_bShowTracks)
	{
		xPos = m_rectPanel1.left;

		for (size_t i = 0; i < GET_MAX_TRACKS(); i++)
		{
			m_btnsTrackHeadings[i].SetRect(xPos+space, yPos, xPos + block, yPos + height);
			xPos += block;
		}

		xPos = m_rectPanel1.left;
		yPos = m_btnsTrackHeadings[0].bottom + space;

		for (size_t i = 0; i < GET_MAX_TRACKS(); i++)
		{
			m_btnsTrackArm[i].SetRect(xPos + space, yPos, xPos + block, yPos + height);
			xPos += block;
		}
	}

	// === SETUP TRACK MODE SWITCH RECTS ===

	xPos = m_rectPanel1.left;
	yPos = m_btnsTrackArm[0].bottom + space;
	//height = 20.0f;

	for (size_t i = 0; i < GET_MAX_TRACKS(); i++)
	{
		m_btnsTrackMode[i].SetRect(
			xPos + space, 
			yPos, 
			xPos + block, 
			yPos + height);

		m_btnsTrackMon[i].SetRect(
			xPos + space, 
			m_btnsTrackMode[i].bottom + space,
			xPos + block, 
			m_btnsTrackMode[i].bottom + space + height);

		xPos += block;
	}

	m_rectPanel1.bottom = m_btnsTrackMon[0].bottom + space;
	m_rectPanel1.right  = rectClient.Width() - 5.0f;

	// Setup the panel2 rectangle area that goes under all the 
	// tape transport buttons. This panel gets filled with dark
	// background shade color.

	if (!theApp.m_bShowTracks)
	{
		m_rectPanel2.left   = m_rectPanel1.left;
		m_rectPanel2.top    = 5.0f;
		m_rectPanel2.right  = m_rectPanel1.right;
		m_rectPanel2.bottom = m_rectPanel2.top + 215.0f;
	}
	else
	{
		m_rectPanel2.left   = m_rectPanel1.left;
		m_rectPanel2.top    = m_rectPanel1.bottom + space;
		m_rectPanel2.right  = m_rectPanel1.right;
		m_rectPanel2.bottom = m_rectPanel2.top + 215.0f;
	}

	// === SETUP TAPE TIME DISPLAY RECTS ===

	xPos = m_rectPanel2.left + 10.0f;
	yPos = m_rectPanel2.top  + 10.0f;

	height = 130.0f;	// 60.0f;
	width  = 380.0f;			// 270.0f

	// Setup black tape time background pane rect
	m_rectTimePane = CD2DRectF(xPos, yPos, xPos + width, yPos + height);

	// Setup big tape time text rect
	m_rectTimeBig = m_rectTimePane;
	//m_rectTimeBig.left += 5.0f;
	m_rectTimeBig.right  -= 5.0f;
	m_rectTimeBig.top    += 45.0f;
	m_rectTimeBig.bottom -= 25.0f;

	// Setup small tape time text rect	
	m_rectTimeSmall.left   = m_rectTimeBig.right + 2.0f;
	m_rectTimeSmall.top    = m_rectTimeBig.top + -2.0f;
	m_rectTimeSmall.right  = m_rectTimeSmall.left + 80.0f;
	m_rectTimeSmall.bottom = m_rectTimeBig.bottom - 15.0f;

	// Setup small rect for current transport mode text
	m_rectTimeMode = CD2DRectF(xPos + 200.0f, yPos + 10.0f, xPos + 445.0f, yPos + 35.0f);

	// === SETUP BIG TRANSPORT CONTROL BUTTON RECTS ===

	height = 50.0f;
	width = 100.0f;

	//yPos = m_rectTime.bottom + 10.0f;
	yPos = (m_rectPanel2.bottom - 10.0f) - 50.0f;

	for (size_t i = 0; i < MAX_TRANSPORT_BUTTONS; i++)
	{
		float btn_space = (i == 0) ? 40.0f : 10.0f;

		if (i == MAX_TRANSPORT_BUTTONS - 1)
		{
			float x = xPos + 35.0f;
			float y = yPos;
			m_btnsTransport[i].SetRect(x, y, x + width - 40.0f, y + height - 20.0f);
		}
		else
		{
			m_btnsTransport[i].SetRect(xPos, yPos, xPos + width, yPos + height);
		}

		xPos += width + btn_space;
	}

	// Set panel2 right hand rectangle side from right hand transport button edge
	m_rectPanel2.right = xPos - 5.0f;
	// Adjust time rect right hand side with fwd button edge
	m_rectTimePane.right = m_btnsTransport[3].right;

	// Setup small rect for locate progress bar in bottom of tape time pane
	m_rectTimeProgress.left   = m_rectTimePane.left + 10.0f;
	m_rectTimeProgress.top    = m_rectTimePane.bottom - 20.0f;
	m_rectTimeProgress.right  = m_rectTimePane.right - 10.0f;
	m_rectTimeProgress.bottom = m_rectTimeProgress.top + 14.0f;

	// === SETUP INFO AND VELOCITY PLOT RECTS ===

	// Adjust info panel to the right of time panel
	m_rectInfoPane = CD2DRectF(
		m_rectTimePane.right + 10.0f,
		m_rectTimePane.top,
		m_rectPanel2.right - 10.0f,
		m_rectTimePane.bottom);

	//m_rectPanel2.bottom = m_rectButtonsTransport[0].bottom + 10.0f;
	m_rectInfoArea = m_rectInfoPane;
	m_rectInfoArea.left += 5.0f;
	m_rectInfoArea.top += 5.0f;
	m_rectInfoArea.right -= 5.0f;
	m_rectInfoArea.bottom -= 5.0f;

	float btnWidth = 40.0f;
	float btnSpace = 7.0f;
	float btnHeight = 16.0f;

	xPos = m_rectInfoArea.left + 7.0f;
	yPos = m_rectInfoArea.top + 5.0f;
	
	count = 0;

	for (int i=BUTTON_ID_TRACK_ALLREPROMODE; i <= BUTTON_ID_TRACK_STANDBYMONITOR; i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		btn.SetRect(xPos, yPos, xPos + btnWidth, yPos + btnHeight);

		xPos += btnWidth + btnSpace;

		if ((count % 4) == 3)
		{
			yPos += btnHeight + btnSpace;
			xPos = m_rectInfoArea.left + 7.0f;
		}
		
		++count;
	}

	// Setup panel3 rectangle
	m_rectPanel3.left  = m_rectPanel2.right + space;
	m_rectPanel3.top   = m_rectPanel2.top;
	m_rectPanel3.right = m_rectPanel1.right;
	// FIX - size fixed for now
	m_rectPanel3.right = 980;
	m_rectPanel3.bottom = m_rectPanel2.bottom;

	// Setup velocity plot window  
	m_rectInfoPlot.left   = m_rectInfoPane.left;
	m_rectInfoPlot.top    = m_rectInfoArea.bottom - 60;
	m_rectInfoPlot.right  = m_rectInfoPane.right - space;
	m_rectInfoPlot.bottom = m_rectInfoPlot.top + 60.0f;
	
	// === SETUP LOCATOR BUTTONS IN PANEL3 ===

	xPos = m_rectPanel3.left + 10.0f;
	yPos = m_rectPanel3.top + 10.0f;

	height = 45.0f;
	width  = 45.0f;

	count = 0;
	
	for (int i = BUTTON_ID_CUEPOINT_LOC1; i <= BUTTON_ID_LOCATOR_STOREMODE; i++)
	{
		CD2DButton& btn = m_btnArray.ElementAt(i);

		btn.SetRect(xPos, yPos, xPos + width, yPos + height);

		xPos += width + space;

		if ((count % 3) == 2)
		{
			yPos += height + space;
			xPos = m_rectPanel3.left + 10.0f;
		}
		
		++count;
	}

	CD2DButton& btn = m_btnArray.ElementAt(BUTTON_ID_CUEPOINT_LOC3);

	xPos = btn.right + space + 10.0f;
	yPos = btn.top;

	count = 0;

	for (int i=BUTTON_ID_LOCATOR_AUTOPUNCH; i <= BUTTON_ID_LOCATOR_CANCELSEARCH; i++)
	{
		m_btnArray.ElementAt(i).SetRect(xPos, yPos, xPos + width, yPos + height);

		yPos += height + space;

		if ((count % 4) == 3)
		{
			yPos = btn.top;
			xPos += width + space;
		}
		
		++count;
	}
}

// End-Of-File