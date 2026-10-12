// DDX helpers in DDXCustom.cpp. 

// Custom DDX CUnits Helpers
void AFXAPI DDX_ColorPicker(CDataExchange *pDX, int nIDC, COLORREF& clr);

//void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, FLOAT& value, CUnits& units);
//void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, double& value, CUnits& units);

void AFXAPI DDX_CBIndexLineStyles(CDataExchange* pDX, int nIDC, int& index);
void AFXAPI DDX_CBIndex(CDataExchange* pDX, int nIDC, int& index, CString strItems);

// Exposed Utility Helpers
//BOOL AFXAPI DDXHelper_UnitsTextFloatParse(CUnits& units, LPCTSTR lpszText, double& d, int& nUnits);

// End-Of-File