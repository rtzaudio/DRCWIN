///////////////////////////////////////////////////////////////////////////////
// The window coordinate system is transformed to Cartesian coordinates
// with the center of the world coordinate system a zero as shown below. 
//
//            |
//      (-,+) | (+,+)
//            |
//      ------+-------      /* center point is (0,0) */
//            |
//      (-,-) | (+,-)
//            |
//
// DirectX D2D uses DIPS internally Q: [How do I support different units in D2D?]
//
// A: You can look at a DIP one of two ways. In mark-up (e.g. XAML, XPS) a 
// DIP is indeed 1/96” and when you record a DIP it should render as the 
// equivalent physical length on any system. What about for a rasterizer 
// like D2D though? Rasterizers fundamentally render pixels, not physical units. 
//
// DPI stands for Dots Per Inch and it relates a physical unit (a DIP) to a 
// pixel (a dot). Essentially, what this means is that the DPI implies a 
// scale factor that is applied to all of the primitives in D2D. The scale 
// is just dpi/96. So, you can use the DPI to set ANY scale you want. If you
// want your unit (fathoms) to be 10 pixels, you’d set the DPI to 9.6 and 
// then all of your input “dips” can be considered to be in that unit (fathoms). 
// In your case, you want you unit to be in 0.001”. You can retrieve the desktop
// dpi (ID2D1Factory::GetDesktopDpi), this tells you how many dots fits in an inch.
// The render target DPI would hence be 0.001 * 96 * desktop DPI. This would
// leave you working in 0.001” units (assuming the desktop DPI is accurate). 

#pragma once

#ifndef WM_ZOOM_NOTIFY
#define WM_ZOOM_NOTIFY      (WM_USER+200)
#endif

///////////////////////////////////////////////////////////////////////////////
// CWorldView Transform Structure

class CXFORM
{
public:
    CXFORM() :
        m_x(0.0f),
        m_y(0.0f),
        m_scale(1.0f),
        m_angle(0.0f),
        m_mirror(FALSE)
    {
    }
    
    CXFORM(FLOAT x, FLOAT y, FLOAT scale=1.0f, FLOAT angle=0.0f, BOOL mirror=FALSE)
    {
        m_x      = x;
        m_y      = y;
        m_scale  = scale;
        m_angle  = angle;
        m_mirror = mirror;
    }

    void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
	    {
            ar << m_x;
            ar << m_y;
            ar << m_scale;
            ar << m_angle;
            ar << m_mirror;
	    }
	    else
	    {
            ar >> m_x;
            ar >> m_y;
            ar >> m_scale;
            ar >> m_angle;
            ar >> m_mirror;
	    }
    }

public:
    // X/Y define the offset of a shape relative to the origin 
    // of the Cartesian coordinate system. The definition of
    // shapes can be nested and the x and y attributes are always
    // interpreted relative to the local coordinate system of the 
    // shape to which the transformation applies. The default
    // value for x and y is 0.0.

    FLOAT   m_x;        // x-offset relative to cartesian coordinates
    FLOAT   m_y;        // y-offset relative to cartesian coordinates
    FLOAT   m_scale;    // scale to render object
    FLOAT   m_angle;    // rotation angle in degrees (0.0f - 360.0f)
    BOOL    m_mirror;   // all x dimensions are set to a –x value
};

///////////////////////////////////////////////////////////////////////////////
// CWorldView Class Implementation for DirextX CAD world view class

class CWorldView : public CView
{
    DECLARE_DYNCREATE(CWorldView)

public:
    // Zoom View Operations
protected:
    CWorldView();          // protected constructor used by dynamic creation
    virtual ~CWorldView();

public:
    // Attributes

    FLOAT Inch2DIPS(FLOAT inches)
    { return inches * 96.0f; }

    FLOAT DIPS2Inch(FLOAT dips)
    { return dips / 96.0f; }

    FLOAT PointSize2DIPS(FLOAT points)
    { return (points/72.0f) * 96.0f; }

    void SetZoomScaleMax(FLOAT max)
    { m_zoomScaleMax = max; }

    FLOAT GetZoomScaleMax()
    { return m_zoomScaleMax; }

    void SetZoomScaleMin(FLOAT min)
    { m_zoomScaleMin = min; }

    FLOAT GetZoomScaleMin()
    { return m_zoomScaleMin; }

    FLOAT GetZoomScale(void)
    { return m_zoomScale; }

    // Operations
    void NormalizeRect(CD2DRectF& rect);

    void ColorRefToD2DColor(COLORREF clr, D2D1_COLOR_F& clrD2D, FLOAT alpha=1.0f)
    {
        clrD2D.r = GetRValue(clr) /  255.0f;
        clrD2D.g = GetGValue(clr) /  255.0f;
        clrD2D.b = GetBValue(clr) /  255.0f;
        clrD2D.a = alpha;
    }

    // Transform Methods
    D2D1_POINT_2F GetDevPositionForCurrentDpi(CPoint point);
    D2D1_POINT_2F GetDevPositionForCurrentDpi(FLOAT x, FLOAT y);
    D2D1_POINT_2F WindowPointToTargetPoint(CPoint point);
    D2D1_POINT_2F DeviceToWorld(CPoint point);
    D2D1_POINT_2F DeviceToWorld(CPoint point, D2D1_MATRIX_3X2_F world);
    D2D1::Matrix3x2F MakeTransform();
    D2D1::Matrix3x2F MakeTransform(CXFORM& xform);
    D2D1_MATRIX_3X2_F GetViewWorldTransform(CHwndRenderTarget* pRenderTarget);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    D2D1::Matrix3x2F    m_InverseOverall;       // inverse transform
    D2D1_POINT_2F       m_mousePos;             // current mouse position in world units
    FLOAT               m_zoomScale;
    FLOAT               m_zoomScaleMin;
    FLOAT               m_zoomScaleMax;

    // Helper functions
    void CreateResources(CHwndRenderTarget* pRenderTarget);

    virtual void OnDraw(CDC* /*pDC*/);
    // This should be overriden in the derived class
    virtual void OnRender(CHwndRenderTarget* pRenderTarget);

public:
    DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);
};

// End-Of-File