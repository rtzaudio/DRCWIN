///////////////////////////////////////////////////////////////////////////////
// CWorldView.cpp : implementation file
//
// DirectX 2D Cartesian Coordinate World View Class.
//
// This class was specifically designed for use in CAD type applications 
// using DirectX11 on and D2D on Windows7. Thus, all drawing using
// world space floating point coordinates and transforms for all drawing. 
// 
// Most importantly, the method MakeTransform() creates our transform
// for Cartesian coordinates with the center of the world coordinate
// system a zero as shown below. 
//
// This class creates two off-screen render targets to render all 
// display information. The "front" target is the primary drawing 
// surface which most all drawing occurs. The "back" buffer is used
// for drawing objects during drag operations. These two bitmap
// targets are alpha blended during the application window paint
// to render the composite image for high speed drawing.
// 
// TODO:
// Note  Direct3D uses the world and view matrices that you set to
// configure several internal data structures. Each time you set a new world
// or view matrix, the system recalculates the associated internal structures.
// Setting these matrices frequently-for example, thousands of times per
// frame-is computationally time-consuming. You can minimize the number
// of required calculations by concatenating your world and view matrices
// into a world-view matrix that you set as the world matrix, and then
// setting the view matrix to the identity. Keep cached copies of individual
// world and view matrices so that you can modify, concatenate, and reset
// the world matrix as needed. For clarity, in this documentation Direct3D
// samples rarely employ this optimization.

#include "stdafx.h"
#include "resource.h"
#include "WorldView.h"

#ifndef SM_MOUSEWHEELPRESENT
#define SM_MOUSEWHEELPRESENT        75
#endif

#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES     104
#endif

using namespace D2D1;

///////////////////////////////////////////////////////////////////////////////
// CWorldView

IMPLEMENT_DYNCREATE(CWorldView, CView)

BEGIN_MESSAGE_MAP(CWorldView, CView)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_SETTINGCHANGE()
    ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CWorldView::OnDraw2D)
    ON_WM_CREATE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CWorldView

CWorldView::CWorldView()
{
    // Enable D2D support for this window
    //EnableD2DSupport();

    m_zoomScale         = 1.0f;
    m_zoomScaleMin      = 0.01f;
    m_zoomScaleMax      = 10.0f;
    m_mousePos.x        = 0.0f;
    m_mousePos.y        = 0.0f;
    
    m_InverseOverall    = D2D1::Matrix3x2F::Identity();
}

CWorldView::~CWorldView()
{
}

///////////////////////////////////////////////////////////////////////////////
// CWorldView diagnostics

#ifdef _DEBUG
void CWorldView::AssertValid() const
{
	// Bypass CView because of MM_ANISOTROPIC map mode
	CView::AssertValid();
}

void CWorldView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////
// CWorldView Device and View Conversion Helpers

D2D1_POINT_2F CWorldView::GetDevPositionForCurrentDpi(CPoint point)
{ 
    CD2DSizeF dpi = GetRenderTarget()->GetDpi();
    return D2D1::Point2F(point.x * 96 / dpi.width, point.y * 96 / dpi.height);
}

D2D1_POINT_2F CWorldView::GetDevPositionForCurrentDpi(FLOAT x, FLOAT y)
{
    CD2DSizeF dpi = GetRenderTarget()->GetDpi();
    return D2D1::Point2F(x * 96.0f / dpi.width, y * 96.0f / dpi.height);
}

// Convert a window point to a D2D point, accounting for DPI differences

D2D1_POINT_2F CWorldView::WindowPointToTargetPoint(CPoint point)
{
    RECT rect;
    GetClientRect(&rect);
    
    point.x = rect.right  - point.x - 1;
    point.y = rect.bottom - point.y - 1;

    CD2DSizeF dpi = GetRenderTarget()->GetDpi();

    return D2D1::Point2F((point.x * 96.0f / dpi.width), 
                         (point.y * 96.0f / dpi.height));
}

D2D1_POINT_2F CWorldView::DeviceToWorld(CPoint point)
{
    D2D1_POINT_2F devPos = GetDevPositionForCurrentDpi(point);

    // Calculate world coordinates by multiplying target device coordinates
    // by the world overall inverse matrix to transform to a world point.

    D2D1_POINT_2F worldPoint = D2D1::Matrix3x2F::ReinterpretBaseType(&m_InverseOverall)->TransformPoint(devPos);
    
    return worldPoint;
}

D2D1_MATRIX_3X2_F CWorldView::GetViewWorldTransform(CHwndRenderTarget* pRenderTarget)
{
    D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

    // Zoom scale transform
    D2D1_MATRIX_3X2_F scaleTransform;
    scaleTransform = D2D1::Matrix3x2F::Scale(m_zoomScale, m_zoomScale);
    
    // Cartesian coordinate transform
    D2D1_MATRIX_3X2_F cartesianTransform;
    cartesianTransform = D2D1::Matrix3x2F::Translation(rtSize.width * 0.5f, rtSize.height * 0.5f);

    // Scale the view and then transform for Cartesian.
    D2D1_MATRIX_3X2_F worldTransform = scaleTransform * cartesianTransform;

    return worldTransform;
}

void CWorldView::NormalizeRect(CD2DRectF& rect)
{
   if (rect.left > rect.right)
   {
      FLOAT r = rect.right;
      rect.right = rect.left;
      rect.left = r;
   }
   if (rect.top > rect.bottom)
   {
      FLOAT b = rect.bottom;
      rect.bottom = rect.top;
      rect.top = b;
   }
}

///////////////////////////////////////////////////////////////////////////////
// This method creates a the cartesian world transform as shown below
// and includes the current zoom level and window pan translate position
// in the final transform matrix returned.
//
//            |
//      (-,+) | (+,+)
//            |
//    --------+---------     /* center point is (0,0) */
//            |
//      (-,-) | (+,-)
//            |

D2D1::Matrix3x2F CWorldView::MakeTransform()
{
    // Get the render target size
    D2D1_SIZE_F rtSize = GetRenderTarget()->GetSize();

    // Cartesian center coordinate transform
    D2D1::Matrix3x2F cartesianTransform = D2D1::Matrix3x2F::Translation(-rtSize.width * 0.5f, -rtSize.height * 0.5f);
    cartesianTransform.Invert();
    cartesianTransform._11 = 1.0f;
    cartesianTransform._22 = -1.0f;

    // Zoom scale transform
    D2D1::Matrix3x2F scaleTransform = D2D1::Matrix3x2F::Scale(m_zoomScale, m_zoomScale);

    // Translate transform for the pan view offset
    //D2D1::Matrix3x2F translateTransform = D2D1::Matrix3x2F::Translation(-m_translateX, m_translateY);

    // Scale the view and then transform for Cartesian.
    //D2D1::Matrix3x2F overallTransform = translateTransform * scaleTransform * cartesianTransform;
    D2D1::Matrix3x2F overallTransform = scaleTransform * cartesianTransform;

    m_InverseOverall = overallTransform;
    m_InverseOverall.Invert();

    // Final transform adjusted by the pan view offset.
    return overallTransform;
}

D2D1::Matrix3x2F CWorldView::MakeTransform(CXFORM& xform)
{
    // Get the render target size
    D2D1_SIZE_F rtSize = GetRenderTarget()->GetSize();

    D2D1::Matrix3x2F prevTransform;

    // Cartesian center coordinate transform
    D2D1::Matrix3x2F cartesianTransform = D2D1::Matrix3x2F::Translation(-rtSize.width * 0.5f, -rtSize.height * 0.5f);
    cartesianTransform.Invert();
    cartesianTransform._11 = 1.0f;
    cartesianTransform._22 = -1.0f;

    if (xform.m_mirror)
        cartesianTransform._22 = 1.0f;

    // Zoom scale transform
    D2D1::Matrix3x2F scaleTransform = D2D1::Matrix3x2F::Scale(m_zoomScale, m_zoomScale);
    
    // Rotate at center transform
    D2D1_POINT_2F center;
    center.x = xform.m_x;
    center.y = xform.m_y;
    D2D1::Matrix3x2F rotateTransform = D2D1::Matrix3x2F::Rotation(xform.m_angle, center);

    // Translate transform for the pan view offset
    //D2D1::Matrix3x2F translateTransform = D2D1::Matrix3x2F::Translation(-m_translateX, m_translateY);

    // Scale the view and then transform for Cartesian.
    //D2D1::Matrix3x2F overallTransform = rotateTransform * translateTransform * scaleTransform * cartesianTransform;
    D2D1::Matrix3x2F overallTransform = rotateTransform * scaleTransform * cartesianTransform;

    m_InverseOverall = overallTransform;
    m_InverseOverall.Invert();
    
    return overallTransform;
}

///////////////////////////////////////////////////////////////////////////////
// Drawing, Sizing and Rendering Methods

// Handle resizing of the render target

void CWorldView::OnSize(UINT nType, int cx, int cy)
{
    CHwndRenderTarget* pRenderTarget = GetRenderTarget();

    CView::OnSize(nType, cx, cy);
}

/* No background erase needed, we render everything at once in DirectX */

BOOL CWorldView::OnEraseBkgnd(CDC* pDC)
{
    // Don't erase background, D2D renders the full image
    return TRUE;
}

void CWorldView::OnDraw(CDC* pDC)
{
    TRACE(_T("CWorldView::OnDraw()\n"));
}

///////////////////////////////////////////////////////////////////////////////
//

void CWorldView::CreateResources(CHwndRenderTarget* pRenderTarget)
{

}

afx_msg LRESULT CWorldView::OnDraw2D(WPARAM wParam, LPARAM lParam)
{
    OnRender((CHwndRenderTarget*)lParam);
    return TRUE;
}

void CWorldView::OnRender(CHwndRenderTarget* pRenderTarget)
{
    TRACE(_T("CWorldView::OnRender()\n"));
}

// End-Of-File