/////////////////////////////////////////////////////////////////////////////
// TN026: Custom user defined DDX and DDV Routines  
//
// Notes for implementing dialog data exchange and validation procs:
//  * always start with PrepareCtrl or PrepareEditCtrl
//  * always start with 'pDX->m_bSaveAndValidate' check
//  * pDX->Fail() will throw an exception - so be prepared
//  * avoid creating temporary HWNDs for dialog controls - i.e.
//      use HWNDs for child elements
//  * validation procs should only act if 'm_bSaveAndValidate'
//  * use the suffices:
//      DDX_ = exchange proc
//      DDV_ = validation proc
//

#include "stdafx.h"
#include <float.h>

#include "DDXCustom.h"
#include "colors.h"

/////////////////////////////////////////////////////////////////////////////
// CMFCColorButton DDX Support

#ifdef MORE_COLORS
COLORREF s_colors[] = { 
    COLOR_ALICEBLUE, COLOR_ANTIQUEWHITE, COLOR_AQUA, COLOR_AQUAMARINE,
    COLOR_AZURE, COLOR_BEIGE, COLOR_BISQUE, COLOR_BLACK, COLOR_BLANCHEDALMOND, COLOR_BLUE, 
    COLOR_BLUEVIOLET, COLOR_BROWN, COLOR_BURLYWOOD, COLOR_CADETBLUE, COLOR_CHARTREUSE, COLOR_CHOCOLATE,
    COLOR_CORAL, COLOR_CORNFLOWERBLUE, COLOR_CORNSILK, COLOR_CRIMSON, COLOR_CYAN, COLOR_DARKBLUE,
    COLOR_DARKCYAN, COLOR_DARKGOLDENROD, COLOR_DARKGRAY, COLOR_DARKGREEN, COLOR_DARKKHAKI, 
    COLOR_DARKMAGENTA, COLOR_DARKOLIVEGREEN, COLOR_DARKORANGE, COLOR_DARKORCHID, COLOR_DARKRED,
    COLOR_DARKSALMON, COLOR_DARKSEAGREEN, COLOR_DARKSLATEBLUE, COLOR_DARKSLATEGRAY, COLOR_DARKTURQUOISE,
    COLOR_DARKVIOLET, COLOR_DEEPPINK, COLOR_DEEPSKYBLUE, COLOR_DIMGRAY, COLOR_DODGERBLUE,
    COLOR_FIREBRICK, COLOR_FLORALWHITE, COLOR_FORESTGREEN, COLOR_FUCHSIA, COLOR_GAINSBORO,
    COLOR_GHOSTWHITE, COLOR_GOLD, COLOR_GOLDENROD, COLOR_GRAY, COLOR_GREEN, COLOR_GREENYELLOW,
    COLOR_HONEYDEW, COLOR_HOTPINK, COLOR_INDIANRED, COLOR_INDIGO, COLOR_IVORY, COLOR_KHAKI,
    COLOR_LAVENDER, COLOR_LAVENDERBLUSH, COLOR_LAWNGREEN, COLOR_LEMONCHIFFON, COLOR_LIGHTBLUE,
    COLOR_LIGHTCORAL, COLOR_LIGHTCYAN, COLOR_LIGHTGOLDENRODYELLOW, COLOR_LIGHTGRAY, COLOR_LIGHTGREEN,
    COLOR_LIGHTPINK, COLOR_LIGHTSALMON, COLOR_LIGHTSEAGREEN, COLOR_LIGHTSKYBLUE, COLOR_LIGHTSLATEGRAY,
    COLOR_LIGHTSTEELBLUE, COLOR_LIGHTYELLOW, COLOR_LIME, COLOR_LIMEGREEN, COLOR_LINEN, COLOR_MAGENTA,
    COLOR_MAROON, COLOR_MEDIUMAQUAMARINE, COLOR_MEDIUMBLUE, COLOR_MEDIUMORCHID, COLOR_MEDIUMPURPLE,
    COLOR_MEDIUMSEAGREEN, COLOR_MEDIUMSLATEBLUE, COLOR_MEDIUMSPRINGGREEN, COLOR_MEDIUMTURQUOISE,
    COLOR_MEDIUMVIOLETRED, COLOR_MIDNIGHTBLUE, COLOR_MINTCREAM, COLOR_MISTYROSE, COLOR_MOCCASIN,
    COLOR_NAVAJOWHITE, COLOR_NAVY, COLOR_OLDLACE, COLOR_OLIVE, COLOR_OLIVEDRAB, COLOR_ORANGE,
    COLOR_ORANGERED, COLOR_ORCHID, COLOR_PALEGOLDENROD, COLOR_PALEGREEN, COLOR_PALETURQUOISE,
    COLOR_PALEVIOLETRED, COLOR_PAPAYAWHIP, COLOR_PEACHPUFF, COLOR_PERU, COLOR_PINK,
    COLOR_PLUM, COLOR_POWDERBLUE, COLOR_ROSYBROWN, COLOR_ROYALBLUE,
    COLOR_SADDLEBROWN, COLOR_SALMON, COLOR_SANDYBROWN, COLOR_SEAGREEN, COLOR_SEASHELL, COLOR_SIENNA,
    COLOR_SILVER, COLOR_SKYBLUE, COLOR_SLATEBLUE, COLOR_SLATEGRAY, COLOR_SNOW, COLOR_SPRINGGREEN,
    COLOR_STEELBLUE, COLOR_TAN,  COLOR_TEAL, COLOR_THISTLE, COLOR_TOMATO, COLOR_TRANSPARENT,
    COLOR_TURQUOISE, COLOR_VIOLET, COLOR_WHEAT, COLOR_WHITESMOKE, COLOR_YELLOWGREEN
};
#endif

void AFXAPI DDX_ColorPicker(CDataExchange *pDX, int nIDC, COLORREF& clr)
{
    HWND hWnd = pDX->PrepareCtrl(nIDC);
    ASSERT(hWnd!=NULL);                
    
    CMFCColorButton* pButton=(CMFCColorButton*)CWnd::FromHandle(hWnd);
    
    if (pDX->m_bSaveAndValidate)
    {
        clr = pButton->GetColor();
    }
    else 
    {
#ifdef MORE_COLORS
        CList<COLORREF,COLORREF> lstColors;

        for (int i=0; i < sizeof(s_colors)/sizeof(COLORREF); i++)
            lstColors.AddTail(s_colors[i]);
        
        pButton->SetDocumentColors(_T("Custom"), lstColors);
#endif
        pButton->SetColor(clr);
    }
}

/////////////////////////////////////////////////////////////////////////////
// convert a string to a double in CAD application units

#if 0
static BOOL UnitsTextFloatParse(CUnits& units, LPCTSTR lpszText, double& d, int& nUnits)
{
	ATLASSERT(lpszText != NULL);

    // Skip any leading white space
    while (*lpszText == _T(' ') || *lpszText == _T('\t'))
		lpszText++;

    // Attempt float value conversion
    d = 0.0;
    TCHAR chFirst = lpszText[0];
    
    //if (_sntscanf_s(szBuffer, _countof(szBuffer), _T("%lf"), &d) != 1)
    double val = _tcstod(lpszText, (LPTSTR*)&lpszText);

    // Check for any convert error
    if (val == 0.0 && chFirst != _T('0'))
		return FALSE;   // could not convert

    // Assume current units mode
    nUnits = units.GetUserUnits();

    // lpszText will point to any non-numeric text folling the float value
    // (e.g. "10.5mil")

    if (lpszText)
    {
        // Check for any suffix units override. If the user entered
        // a mm, mil or inches suffix we use that to override the 
        // the current default units mode for the conversion.

        // Suffix text after anu valid float digits, skip white space
        while (*lpszText == _T(' ') || *lpszText == _T('\t'))
		    lpszText++;

        // Scan for any units suffix change specifier (in, mil, mm)
        if (_tcsicmp(lpszText, _T("mic")) == 0)
            nUnits = USER_UNITS_MIC;
        else if (_tcsicmp(lpszText, _T("mm")) == 0)
            nUnits = USER_UNITS_MM;
        else if (_tcsicmp(lpszText, _T("mil")) == 0)
            nUnits = USER_UNITS_MIL;
        else if ((_tcsicmp(lpszText, _T("in")) == 0) || *lpszText == _T('"'))
            nUnits = USER_UNITS_INCH;
    }

    // Convert user entered value to internal CAD units

    switch(nUnits)
    {
    case USER_UNITS_MIC:
        d = units.mic2u((float)val);
        break;

    case USER_UNITS_MM:
        d = units.mm2u((float)val);
        break;

    case USER_UNITS_MIL:
        d = units.mil2u((float)val);
        break;

    case USER_UNITS_INCH:
        d = units.inch2u((float)val);
        break;

    default:
        return FALSE;
        break;
    }
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Get or set a double/float type in CAD application units

static void UnitsTextFloatFormat(CDataExchange* pDX, CUnits& units, int nIDC,
	void* pData, double value, int nSizeGcvt, BOOL bShowUnitsSuffix)
{
	ASSERT(pData != NULL);

	pDX->PrepareEditCtrl(nIDC);
	HWND hWndCtrl;
	pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);
	
	const int TEXT_BUFFER_SIZE = 400;
	TCHAR szBuffer[TEXT_BUFFER_SIZE];

    if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));

        double d;
        int nUnits;

        if (!UnitsTextFloatParse(units, szBuffer, d, nUnits))
		{
			AfxMessageBox(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		if (nSizeGcvt == FLT_DIG)
			*((FLOAT*)pData) = (FLOAT)d;
		else
			*((double*)pData) = d;
	}
	else
	{
        TCHAR szFmt[64];
        _tcscpy_s(szFmt, _countof(szFmt), _T("%.*g"));

        int nPrecision = 6;

        switch(units.GetUserUnits())
        {
        case USER_UNITS_MIC:
            // Append millimeters suffix if requested
            if (bShowUnitsSuffix)
                _tcscat_s(szFmt, _countof(szFmt), _T(" mic"));
            // Format the return value in micrometers
		    ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer)-1, 
                szFmt, nPrecision, units.u2mic((float)value)));
            break;

        case USER_UNITS_MM:
            // Append millimeters suffix if requested
            if (bShowUnitsSuffix)
                _tcscat_s(szFmt, _countof(szFmt), _T(" mm"));
            // Format the return value in millimeters
		    ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer)-1, 
                szFmt, nPrecision, units.u2mm((float)value)));
            break;

        case USER_UNITS_MIL:
            // Append mil suffix if requested
            if (bShowUnitsSuffix)
                _tcscat_s(szFmt, _countof(szFmt), _T(" mil"));
            // Format the return value in mils
		    ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer)-1,
                szFmt, nPrecision, units.u2mil((float)value)));
            break;

        case USER_UNITS_INCH:
            // Append inches suffix if requested
            if (bShowUnitsSuffix)
                _tcscat_s(szFmt, _countof(szFmt), _T("\""));
            // Format the return value in inches
		    ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer)-1, 
                szFmt, nPrecision, units.u2inch((float)value)));
            break;

        default:
            // units unknown - just set the value as is
		    ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer)-1,
                _T("%.*g"), nPrecision, value));
            break;
        }

        AfxSetWindowText(hWndCtrl, szBuffer);
	}
}
#endif

// Fill a combo box with a list of string items that are comma delimited

static int CBAddItems(HWND hWndCtrl, CString& strItems, int nSelect)
{
    int nCount;
    int nCurPos;
    CString strText(strItems);
    CString strTip;
    CString strToken;
    
    nCount = nCurPos = 0;

    // Reset the combo box content
    ::SendMessage(hWndCtrl, CB_RESETCONTENT, 0, 0L);

    // Add the pin length option items
    if ((strToken = strText.Tokenize(_T(","), nCurPos)) != _T(""))
    {
        do {
            // trim any leading/trailing spaces */
            strToken.Trim();
            //LPCTSTR pszItem = (LPCSTR)strToken.GetString();
            // Add the string
            int index = (int)::SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LPARAM)strToken.GetString());
            if (index == CB_ERR)
                break;

            // Get the next string token to insert into the list box.
            strToken = strText.Tokenize(_T(","), nCurPos);
            ++nCount;
        }  while (strToken != _T(""));
    }

    if (nSelect != -1)
		::SendMessage(hWndCtrl, CB_SETCURSEL, (WPARAM)nSelect, 0L);

    return nCount;
}

/////////////////////////////////////////////////////////////////////////////
// Global DDX Dialog Helper Functions

#if 0
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, FLOAT& value, CUnits& units)
{
	UnitsTextFloatFormat(pDX, units, nIDC, &value, value, FLT_DIG, TRUE);
}

#endif

void AFXAPI DDX_CBIndex(CDataExchange* pDX, int nIDC, int& index, CString strItems)
{
	pDX->PrepareCtrl(nIDC);
    HWND hWndCtrl;
    pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);

    if (pDX->m_bSaveAndValidate)
    {
		index = (int)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L);
    }
	else
    {
        CBAddItems(hWndCtrl, strItems, index);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Exposed Global Helper Functions

#if 0
BOOL AFXAPI DDXHelper_UnitsTextFloatParse(CUnits& units, LPCTSTR lpszText, double& d, int& nUnits)
{
    return UnitsTextFloatParse(units, lpszText, d, nUnits);
}
#endif

// End-Of-File