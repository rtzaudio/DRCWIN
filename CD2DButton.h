#pragma once

class CD2DButtonRect : public CD2DRectF
{
public:
	CD2DButtonRect() : CD2DRectF()
	{
	}

	CD2DButtonRect(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom)
	{
		left = fLeft, top = fTop, right = fRight, bottom = fBottom;
	}

	// Copy constructor
	CD2DButtonRect::CD2DButtonRect(const CD2DRectF& rect)
	{
		left = rect.left, top = rect.top, right = rect.right, bottom = rect.bottom;
	}

	void Offset(FLOAT x, FLOAT y)
	{
		left += x; right += x; top += y; bottom += y;
	}

	FLOAT Height()
	{
		return(fabs(bottom - top));
	}

	FLOAT Width()
	{
		return(fabs(right - left));
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
};



class CD2DButton
{
public:
	CD2DButton(CString strText)
		, text(strText)
		, down_l(FALSE)
		, down_r(FALSE)
		, up_l(TRUE)
		, up_r(TRUE)
		, hover(FALSE)
		, enable(TRUE)
	{
	}

	CD2DButton(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom)
		, left(fLeft)
		, top(fTop)
		, right(fRight)
		, bottom(fBottom)
		, down_l(FALSE)
		, down_r(FALSE)
		, up_l(TRUE)
		, up_r(TRUE)
		, hover(FALSE)
		, enable(TRUE)
	{
	}

	CD2DButton::CD2DButton(const CD2DRectF& rect)
	{
		rectButton.left   = rect.left;
		rectButton.top    = rect.top;
		rectButton.right  = rect.right;
		rectButton.bottom = rect.bottom;
	
		hover = down_l = down_r = FALSE;
		enable = up_l = up_r = TRUE;
	}

	virtual CD2DButton::~CD2DButton()
	{

	}

	// Operations

	void CDRCWINView::DrawButton(
		CHwndRenderTarget* pRenderTarget,
		CD2DTextFormat* pTextFormat,
		CD2DSolidColorBrush* brushFace,
		CD2DSolidColorBrush* brushText,
		)
	{
		CD2DSizeF sizeRadius2(2.50f, 2.50f);

		pRenderTarget->FillRoundedRectangle(CD2DRoundedRect(rectButton, sizeRadius2), brushFace);

		pRenderTarget->DrawText(strText, rectButton, brushText, pTextFormat);
	}
		
public:
	CD2DButtonRect	rectButton;
	CString			strText;
	BOOL			enable;
	BOOL			hover;
	BOOL			down_l;
	BOOL			down_r;
	BOOL			up_l;
	BOOL			up_r;
};
