#pragma once

#include "bkwin/bkdlgview.h"
#include <math.h>
#include "CBkMsgWnd.h"
#include "BkwndExDef.h"
#include "CBkDrawShadowText.h"

#pragma warning(disable:4244)
#pragma warning(disable:4996)

class CBkRoundRect : public CBkWindow
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkRoundRect, "rundrect")

	CBkRoundRect()
	{
		m_nRoundWidht = 0;
		m_nRoundHeight = 0;
	}

	virtual BOOL RedrawRegion(CDCHandle& dc, CRgn& rgn)
	{
		if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
		{
			BkDCPaint BkDC;
			BeforePaint(dc, BkDC);

			if ( m_pPaintHook != NULL && !m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, TRUE) )
			{
				return TRUE;
			}

			BkSendMessage(WM_PAINT, (WPARAM)(HDC)dc);

			if ( m_pPaintHook != NULL )
			{
				m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, FALSE);
			}

			AfterPaint(dc, BkDC);

			return TRUE;
		}

		return FALSE;
	}

	void OnPaint(CDCHandle dc)
	{
		SetMsgHandled(TRUE);

		/*CRgn rgn;

		rgn.CreateRoundRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom, m_nRoundWidht, m_nRoundHeight);

		
		Gdiplus::SolidBrush solidBrush(Gdiplus::Color::MakeARGB(m_nCrBgAlpha, GetRValue(m_crBg), GetGValue(m_crBg), GetBValue(m_crBg)));
		Gdiplus::Region drawRgn(rgn);
		graphics.FillRegion(&solidBrush, &drawRgn);*/

		int nWidth = m_nRoundWidht * 2;
		int nHeight = m_nRoundHeight * 2;

		INT left = m_rcWindow.left;
		INT right = m_rcWindow.right;
		INT top = m_rcWindow.top;
		INT bottom = m_rcWindow.bottom;
		INT offsetX = m_nRoundWidht * 2;
		INT offsetY = m_nRoundHeight * 2;
		int d = m_nRoundWidht;

		Gdiplus::Graphics graphics(dc);
		Gdiplus::GraphicsPath path;

		path.AddArc(right - offsetX - 1, top, offsetX, offsetY, 270, 90);
		path.AddArc(right-offsetX - 1, bottom-offsetY - 1, offsetX, offsetY, 0, 90);
		path.AddArc(left, bottom - offsetY - 1, offsetX, offsetY, 90, 90);
		path.AddArc(left , top, offsetX, offsetY, 180, 90);
		path.CloseFigure();

		Gdiplus::SolidBrush solidBrush(Gdiplus::Color::MakeARGB(GetStyle().m_nAlphaBg, GetRValue(m_crBg), GetGValue(m_crBg), GetBValue(m_crBg)));
		graphics.FillPath(&solidBrush, &path);	
	}

protected:

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("rndwidht", m_nRoundWidht, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("rndheight", m_nRoundHeight, TRUE)//, 0)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
	BKWIN_END_MSG_MAP()

protected:
	int		 m_nRoundWidht;
	int		 m_nRoundHeight;
};

class CBkRoundRectPath : public CBkRoundRect
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkRoundRectPath, "rundrectpath")

		CBkRoundRectPath()
	{
		bDrawBg = FALSE;
		m_nLineSize = 1;
		m_ncrlineAlpha = 255;
		m_crLine = 0;
	}

	void OnPaint(CDCHandle dc)
	{
		SetMsgHandled(TRUE);

		Gdiplus::Graphics graphics(dc);
		graphics.SetSmoothingMode (Gdiplus::SmoothingModeAntiAlias );
		Gdiplus::Pen pen(Gdiplus::Color::MakeARGB(m_ncrlineAlpha, GetRValue(m_crLine), GetGValue(m_crLine), GetBValue(m_crLine)), ScaleByDpi(m_nLineSize));


		Gdiplus::GraphicsPath path;

		if (m_nRoundWidht > 0 && m_nRoundHeight > 0) 
		{
			int nWidth = ScaleByDpi(m_nRoundWidht) * 2;
			int nHeight = ScaleByDpi(m_nRoundHeight) * 2;

			INT left = m_rcWindow.left;
			INT right = m_rcWindow.right;
			INT top = m_rcWindow.top;
			INT bottom = m_rcWindow.bottom;
			INT offsetX = ScaleByDpi(m_nRoundWidht) * 2;
			INT offsetY = ScaleByDpi(m_nRoundHeight) * 2;
			int d = ScaleByDpi(m_nRoundWidht);

			path.AddArc(right - offsetX - 1, top, offsetX, offsetY, 270, 90);
			path.AddArc(right-offsetX - 1, bottom-offsetY - 1, offsetX, offsetY, 0, 90);
			path.AddArc(left, bottom - offsetY - 1, offsetX, offsetY, 90, 90);
			path.AddArc(left , top, offsetX, offsetY, 180, 90);
			path.CloseFigure();
		}
		else 
		{
			path.AddRectangle(Gdiplus::Rect(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()));
		}

		COLORREF crBg = GetDrawBackgroundColor();
		if (bDrawBg && crBg != CLR_INVALID)
		{
			Gdiplus::SolidBrush solidBrush(Gdiplus::Color::MakeARGB(GetStyle().m_nAlphaBg, GetRValue(crBg), GetGValue(crBg), GetBValue(crBg)));
			graphics.FillPath(&solidBrush, &path);
		}	

		graphics.DrawPath(&pen, &path);
	}


protected:
	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("bdrawbg", bDrawBg, TRUE)//, 0)
		BKWIN_COLOR_ATTRIBUTE("crline", m_crLine, TRUE)
		BKWIN_INT_ATTRIBUTE("crlinealpha", m_ncrlineAlpha, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("linesize", m_nLineSize, TRUE)//, 0)
	BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
	BKWIN_END_MSG_MAP()

protected:

	BOOL bDrawBg;
	COLORREF m_crLine;
	int m_ncrlineAlpha;
	int m_nLineSize;
};

class CBkExpendImg : public CBkWindow
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkExpendImg, "expendimg")

		CBkExpendImg()
	{
		m_nSkinAlpha = 255;
		m_nMarginLeft = 0;
		m_nMarginRight = 0;
		m_nMarginTop = 0;
		m_nMarginBottom = 0;
        m_bStretch = FALSE;
	}

	void OnPaint(CDCHandle dc)
	{
		SetMsgHandled(TRUE);

		CBkSkinBase* pSkin = BkSkin::GetSkin(m_strSkinA);
		if (pSkin == NULL ||
			m_rcWindow.Width() == 0 ||
			m_rcWindow.Height() == 0)
		{
			return;
		}

		CDC			dcMem;
		CBkBitmap	bmpMem;
		CSize		szSKin = pSkin->GetSkinSize();
		CRect		rcWindow(0, 0, m_rcWindow.Width(), m_rcWindow.Height());

		if (szSKin.cx == 0 || szSKin.cy == 0)
		{
			return;
		}

		dcMem.CreateCompatibleDC(dc);
		bmpMem.CreateDIBSection(m_rcWindow.Width(), m_rcWindow.Height());
		HBITMAP hOldBitmap = dcMem.SelectBitmap(bmpMem);
		
		int		  nLeft = 0;
		int		  nTop = 0;
		CBkBitmap bmpCache;
		CDCHandle dcMemHandle;
		CRect	  rcDraw(rcWindow);
		CRect	  rcSrc(0, 0, m_nMarginLeft, m_nMarginTop);

		dcMemHandle.Attach(dcMem);
		bmpCache.CreateDIBSection(szSKin.cx, szSKin.cy);

		{
			CDC dcMem2;
			CDCHandle dcMemHandle2;
			CRect rcDrawSrc(0, 0, szSKin.cx, szSKin.cy);

			dcMem2.CreateCompatibleDC(dc);				
			dcMemHandle2.Attach(dcMem2);
			HBITMAP hOldBitmap2 = dcMem2.SelectBitmap(bmpCache);
			pSkin->DrawImage(dcMemHandle2, rcDrawSrc, rcDrawSrc);
			dcMem2.SelectBitmap(hOldBitmap2);
		}

		//�������Ͻ�
		if (m_nMarginLeft && m_nMarginTop)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.left, 
				rcDraw.top, 
				ScaleByDpi(m_nMarginLeft), 
				ScaleByDpi(m_nMarginTop), 
				bmpCache, 
				nLeft, 
				nTop, 
				m_nMarginLeft, 
				m_nMarginTop,
				255);
		}
		//�������Ͻ�
		if (m_nMarginRight && m_nMarginTop)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.right - ScaleByDpi(m_nMarginRight), 
				rcDraw.top, 
				ScaleByDpi(m_nMarginRight), 
				ScaleByDpi(m_nMarginTop),
				bmpCache, 
				szSKin.cx - m_nMarginRight, 
				nTop, 
				m_nMarginRight, 
				m_nMarginTop,
				255);
		}
		//�������½�
		if (m_nMarginLeft && m_nMarginBottom)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.left, 
				rcDraw.bottom - ScaleByDpi(m_nMarginBottom), 
				ScaleByDpi(m_nMarginLeft), 
				ScaleByDpi(m_nMarginBottom),
				bmpCache, 
				nLeft, 
				szSKin.cy - m_nMarginBottom, 
				m_nMarginLeft, 
				m_nMarginBottom,
				255);
		}
		//�������½�
		if (m_nMarginRight && m_nMarginBottom)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.right - ScaleByDpi(m_nMarginRight), 
				rcDraw.bottom - ScaleByDpi(m_nMarginBottom), 
				ScaleByDpi(m_nMarginRight), 
				ScaleByDpi(m_nMarginBottom),
				bmpCache, 
				szSKin.cx - m_nMarginRight, 
				szSKin.cy - m_nMarginBottom, 
				m_nMarginRight, 
				m_nMarginBottom, 
				255);
		}
		//�������
		if (m_nMarginLeft)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.left, 
				rcDraw.top + ScaleByDpi(m_nMarginTop), 
				ScaleByDpi(m_nMarginLeft), 
				rcDraw.Height() - ScaleByDpi(m_nMarginTop) - ScaleByDpi(m_nMarginBottom),
				bmpCache, 
				nLeft, 
				nTop + m_nMarginTop, 
				m_nMarginLeft, 
				(szSKin.cy < rcDraw.Height() ? szSKin.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
				255);
		}
		//�����ұ�
		if (m_nMarginRight)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.right - ScaleByDpi(m_nMarginRight), 
				rcDraw.top + ScaleByDpi(m_nMarginTop), 
				ScaleByDpi(m_nMarginRight), 
				rcDraw.Height() - ScaleByDpi(m_nMarginTop) - ScaleByDpi(m_nMarginBottom),
				bmpCache, 
				szSKin.cx - m_nMarginRight, 
				nTop + m_nMarginTop, 
				m_nMarginRight, 
				(szSKin.cy < rcDraw.Height() ? szSKin.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
				255);
		}
		//���ƶ���
		if (m_nMarginTop)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.left + ScaleByDpi(m_nMarginLeft), 
				rcDraw.top, 
				rcDraw.Width() - ScaleByDpi(m_nMarginLeft) - ScaleByDpi(m_nMarginRight), 
				ScaleByDpi(m_nMarginTop),
				bmpCache, 
				nLeft + m_nMarginLeft, 
				nTop, 
				(szSKin.cx < rcDraw.Width() ? szSKin.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
				m_nMarginTop, 
				255);
		}
		//���Ƶײ�
		if (m_nMarginBottom)
		{
			CBkBitmap::AlphaBlend(
				dcMemHandle, 
				rcDraw.left + ScaleByDpi(m_nMarginLeft), 
				rcDraw.bottom - ScaleByDpi(m_nMarginBottom), 
				rcDraw.Width() - ScaleByDpi(m_nMarginLeft) - ScaleByDpi(m_nMarginRight), 
				ScaleByDpi(m_nMarginBottom),
				bmpCache, 
				nLeft + m_nMarginLeft, 
				szSKin.cy - m_nMarginBottom, 
				(szSKin.cx < rcDraw.Width() ? szSKin.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
				m_nMarginBottom, 
				255);
		}

		//�����м�
        if (!m_bStretch)
        {
            CBkBitmap::AlphaBlend(
                dcMemHandle, 
                rcDraw.left + ScaleByDpi(m_nMarginLeft), 
                rcDraw.top + ScaleByDpi(m_nMarginTop), 
                rcDraw.Width() - ScaleByDpi(m_nMarginLeft ) - ScaleByDpi(m_nMarginRight), 
                rcDraw.Height() - ScaleByDpi(m_nMarginTop) - ScaleByDpi(m_nMarginBottom),
                bmpCache, 
                nLeft + m_nMarginLeft, 
                nTop + m_nMarginTop, 
                (szSKin.cx < rcDraw.Width() ? szSKin.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
                (szSKin.cy < rcDraw.Height() ? szSKin.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
                255);
        }
        else
        {
            CBkBitmap::AlphaBlend(
                dcMemHandle, 
                rcDraw.left + ScaleByDpi(m_nMarginLeft), 
                rcDraw.top + ScaleByDpi(m_nMarginTop), 
                rcDraw.Width() - ScaleByDpi(m_nMarginLeft) - ScaleByDpi(m_nMarginRight), 
                rcDraw.Height() - ScaleByDpi(m_nMarginTop) - ScaleByDpi(m_nMarginBottom),
                bmpCache, 
                nLeft + m_nMarginLeft, 
                nTop + m_nMarginTop, 
                szSKin.cx - m_nMarginLeft - m_nMarginRight, 
                szSKin.cy - m_nMarginTop - m_nMarginBottom, 
                255);
        }

		dcMem.SelectBitmap(hOldBitmap);

		CBkBitmap::AlphaBlend(dc, 
			m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),
			bmpMem, 0, 0, bmpMem.GetWidth(), bmpMem.GetHeight(), m_nSkinAlpha);
	}

protected:

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("skinalpha", m_nSkinAlpha, TRUE)//, 0)
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinA, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("margin_left", m_nMarginLeft, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("margin_right", m_nMarginRight, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("margin_top", m_nMarginTop, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("margin_bottom", m_nMarginBottom, TRUE)//, 0)
        BKWIN_INT_ATTRIBUTE("stretch", m_bStretch, TRUE)//, 0)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
	BKWIN_END_MSG_MAP()

protected:

	int		 m_nSkinAlpha;
	int		 m_nMarginLeft;
	int		 m_nMarginRight;
	int		 m_nMarginTop;
	int		 m_nMarginBottom;
    BOOL     m_bStretch;
	CStringA m_strSkinA;
};

class CBkRealWndEx : public CBkRealWnd
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkRealWndEx, "realwndex")
	
	CBkRealWndEx()
	{
		m_hWnd = NULL;
	}

	void AttachWnd(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	HWND DetachWnd()
	{
		HWND hWnd = m_hWnd;
		
		m_hWnd = NULL;

		return hWnd;
	}

	virtual void BkSendExtendMessage(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if (!::IsWindow(m_hWnd)) return;
		
		if (Msg == WM_MOVE)
		{
			CPoint ptScreen(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			MoveWindow(m_hWnd, ptScreen.x, ptScreen.y, m_rcWindow.Width(), m_rcWindow.Height(), TRUE);
		}
		else if (Msg == WM_SHOWWINDOW)
		{
			BkSendMessage(WM_SHOWWINDOW, wParam && IsVisible(), lParam);
		}
		else if (Msg == WM_WINDOWPOSCHANGED)
		{
			LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;
			if (lpWndPos)
			{
				CRect rcWindow = m_rcWindow;

				rcWindow.OffsetRect(lpWndPos->x, lpWndPos->y);
				MoveWindow(m_hWnd, rcWindow.left, rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), TRUE);
			}
		}
	}

protected:

	HWND m_hWnd;

protected:

	void OnDestroy()
	{
		if (!IsLayeredWnd())
		{
			SetMsgHandled(FALSE);
			return;
		}
		
		if (::IsWindow(m_hWnd))
		{
			::ShowWindow(m_hWnd, SW_HIDE);
			::DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		if (m_hWndContainer == NULL)
		{
			return;
		}

		if (!IsLayeredWnd())
		{
			SetMsgHandled(FALSE);
			return;
		}

		CRect rcOldWnd = m_rcWindow;

		__super::OnWindowPosChanged(lpWndPos);

		if (rcOldWnd != m_rcWindow)
		{
			CRect rcWnd = m_rcWindow;
			CPoint ptScreen = m_rcWindow.TopLeft();
			HWND  hWndReal = m_hWnd;
			HWND  hWndRealContainner = GetRealContainerWnd();

			if (!IsWindow(hWndRealContainner)) return;
			
			::ClientToScreen(hWndRealContainner, &ptScreen);
			rcWnd.MoveToXY(ptScreen);

			::SetWindowPos(hWndReal, HWND_TOP, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), SWP_NOREDRAW);

			BKNMREALWNDRESIZED nms;
			nms.hdr.code = BKNM_REALWND_RESIZED;
			nms.hdr.hwndFrom = m_hWndContainer;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = GetCmdID();
			nms.rcWnd = m_rcWindow;

			LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

			if (::IsWindowVisible(hWndReal))
			{
				//CRect rcUpdate = m_rcWindow;
				//rcUpdate.MoveToXY(0, 0);
				//::RedrawWindow(_GetRealWindow(), rcUpdate, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				//                 ::RedrawWindow(hWndReal, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
				//::InvalidateRect(hWndReal, NULL, TRUE);
				::SetWindowPos(hWndReal, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			}
		}

	}

	void OnShowWindow(BOOL bShow, UINT nStatus)
	{
		if (!IsLayeredWnd())
		{
			SetMsgHandled(FALSE);
			return;
		}

		CBkWindow::OnShowWindow(bShow, nStatus);

		if (bShow)
		{
			// ��������ڻ��߸����ڵĸ��������أ���Ҫ������Tab�л���������ʾ��ʵ����
			CBkWindow* pWnd = this;

			while (TRUE)
			{
				pWnd = BkWnds::GetWindow(pWnd->GetParent());

				if (NULL == pWnd)
					break;

				if (!pWnd->IsVisible())
					return;
			}
		}

		if (bShow)
		{
			::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		}
		else
		{
			::ShowWindow(m_hWnd, SW_HIDE);
		}
	}

	HWND GetRealContainerWnd()
	{
		HWND hWnd = NULL;

		::SendMessage(m_hWndContainer, WM_BK_GETREAL_CONTAINER_WND, NULL, (LPARAM)&hWnd);

		return hWnd;
	}

	BOOL IsLayeredWnd()
	{
		return ::SendMessage(m_hWndContainer, WM_BK_IS_LAYERED_WND, NULL, NULL);
	}


protected:

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_SHOWWINDOW(OnShowWindow)
	BKWIN_END_MSG_MAP()
};

class CBKShadowText : public CBkWindow
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBKShadowText, "shadowtext")

	CBKShadowText()
	{
		m_bHoverShadow = TRUE;
		m_bNormalShadow = FALSE;
		m_crShadow = RGB(0xFF, 0xFF, 0xFF);
		m_nCrShaowAlpha = 120;
	}

protected:

	void OnPaint(CDCHandle dc)
	{
		if (m_strInnerText.IsEmpty())
			return;

		BOOL bNeedDrawShadow = FALSE;

		if ((m_bNormalShadow && (GetState() & BkWndState_Normal)) ||
			(m_bHoverShadow &&  (GetState() & BkWndState_Hover)))
		{
			bNeedDrawShadow = TRUE;
		}

		if (!bNeedDrawShadow)
		{
			__super::OnPaint(dc);
			return;
		}
	
		BkDCPaint BkDC;
		BeforePaint(dc, BkDC);

		const BkStyle& style = GetStyle();
		DWORD dwTextFormat = (m_nTextAlign == 0 ? style.m_nTextAlign :m_nTextAlign);
		
		if (m_dlgpos.nCount != 4)
		{
			dwTextFormat |= DT_CENTER | DT_VCENTER;
		}

		CBkDrawTextHelper::DrawShawdowText(
			dc, 
			GetInnerText(), -1, 
			m_rcWindow, 
			dwTextFormat, 
			TRUE,
			style.m_nTxtRndHit,
			254,
			Gdiplus::Color::MakeARGB(m_nCrShaowAlpha, GetRValue(m_crShadow), GetGValue(m_crShadow), GetBValue(m_crShadow)));

		AfterPaint(dc, BkDC);
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(TRUE, lParam);
		
		if (m_dlgpos.nCount != 4)
		{
			LPSIZE lpSize = (LPSIZE)lParam;

			lpSize->cx += 4;
			lpSize->cy += 4;
		}

		return 0;
	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		if (m_bHoverShadow)
		{
			return TRUE;
		}

		return __super::NeedRedrawWhenStateChange();
	}


protected:


	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("hovershadow", m_bHoverShadow, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("normalshadow", m_bNormalShadow, TRUE)//, 0)
		BKWIN_INT_ATTRIBUTE("crshadowalpha", m_nCrShaowAlpha, TRUE)//, 0)
		BKWIN_COLOR_ATTRIBUTE("crshaow", m_crShadow, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()

	BOOL	 m_bNormalShadow;
	BOOL	 m_bHoverShadow;
	COLORREF m_crShadow;
	int		 m_nCrShaowAlpha;
};

class CBkRichTextEx : public CBkRichText
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRichTextEx, "richtextex")

	class _LinkInfoEx 
	{
	public:
		CString strText;
		UINT    uCmdID;
		Gdiplus::RectF   rcLink;

		void SetRect(Gdiplus::REAL X, Gdiplus::REAL Y, Gdiplus::REAL Width, Gdiplus::REAL Height)
		{
			rcLink.X = X;
			rcLink.Y = Y;
			rcLink.Width = Width;
			rcLink.Height = Height;
		}
	};

public:

	CBkRichTextEx()
	{

	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;


		return LoadChilds(pTiXmlElem);
	}

	BOOL LoadChilds(TiXmlElement* pTiXmlElem)
	{
		m_lstLink.RemoveAll();
		m_bLinkLoaded = FALSE;
		m_posHover = NULL;

		m_XmlElemChilds = *pTiXmlElem;

		return TRUE;
	}

	WORD GetFontSize() const {
		return LOWORD(m_dwFont);
	}

protected:

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (m_posPushDown && m_posHover == m_posPushDown)
		{
			const _LinkInfoEx& link = m_lstLink.GetAt(m_posPushDown);

			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_COMMAND;
			nms.hdr.hwndFrom = GetContainer();
			nms.hdr.idFrom = ::GetDlgCtrlID(nms.hdr.hwndFrom);
			nms.uItemID = link.uCmdID;
			nms.szItemClass = GetObjectClass();

			LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}

		m_posPushDown = NULL;
	}

	void OnPaint(CDCHandle dc)
	{
		HFONT hftNew = BkFontPool::GetFont(LOWORD(m_dwFont), IsDpiAwareEnable());
		HFONT hftOld = dc.SelectFont(hftNew);

		CRect rcDraw = m_rcWindow;
		CRgn rgnDraw;

		rcDraw.DeflateRect(GetStyle().m_nMarginX, GetStyle().m_nMarginY);

		//if (m_bAutoAdjustYahei && BkFontPool::IsYaHei() && !m_bAdjust)
		//{
		//	int nAddSize = BkFontPool::GetFontSizeAdding(hftNew);

		//	if ((GetStyle().m_nTextAlign & DT_BOTTOM) == DT_BOTTOM)
		//	{
		//		rcDraw.top -= 3 * (nAddSize + 1); 
		//		rcDraw.bottom -= 3 * (nAddSize + 1);
		//		int nMinHeight = BkFontPool::GetDefaultFontSize() + nAddSize + 2 * (nAddSize + 2);
		//		if (rcDraw.Height() < nMinHeight)
		//			rcDraw.bottom += nMinHeight - rcDraw.Height();
		//	}
		//	else if ((GetStyle().m_nTextAlign & DT_VCENTER) == DT_VCENTER)
		//	{
		//		rcDraw.top -= 0; 
		//		rcDraw.bottom -= 0;
		//	}
		//	else
		//	{
		//		rcDraw.top -= 3 * (nAddSize + 1); 
		//		rcDraw.bottom -= 3 * (nAddSize + 1);
		//	}
		//	m_rcWindow = rcDraw; 
		//	m_bAdjust = true;
		//}

		rgnDraw.CreateRectRgnIndirect(rcDraw);

		int nDC = dc.SaveDC();
		dc.SelectClipRgn(rgnDraw);

		Gdiplus::PointF pt(rcDraw.left, rcDraw.top);

		if (GetStyle().m_nTextAlign & DT_CENTER)
		{
			Gdiplus::PointF ptEnd = pt;
			_DrawElement(dc, &m_XmlElemChilds, LOWORD(m_dwFont), ptEnd, TRUE);
			pt.X += (rcDraw.Width() - (ptEnd.X - pt.X)) / 2;
		}

		_DrawElement(dc, &m_XmlElemChilds, LOWORD(m_dwFont), pt);

		_DrawLinks(dc);

		//{
		//	dc.TextOut(pt.X, pt.Y, lpszDraw, nLength);
		//	Gdiplus::Graphics graphics(dc);
		//	Gdiplus::Font fontDraw(dc, dc.GetCurrentFont());
		//	Gdiplus::StringFormat format;
		//	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(254, GetRValue(dc.GetTextColor()), GetGValue(dc.GetTextColor()), GetBValue(dc.GetTextColor())));
		//	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(254, 0xFF, 0xFF, 0xFF));

		//	Gdiplus::RectF layoutRect(rcDraw.left, rcDraw.top, 50, 20);

		//	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		//	graphics.SetTextRenderingHint((Gdiplus::TextRenderingHint)GetStyle().m_nTxtRndHit);

		//	graphics.DrawString(
		//		L"dd",
		//		-1,
		//		&fontDraw,
		//		layoutRect,
		//		&format,
		//		&blackBrush);
		//}

		dc.RestoreDC(nDC);

		dc.SelectFont(hftOld);

		m_bLinkLoaded = TRUE;
	}

	void _AddLink(CDCHandle &dc, TiXmlElement *pElemChild, Gdiplus::PointF &pt)
	{
		_LinkInfoEx newLink;

		HFONT hftOld = NULL;

		if (NULL != m_styleLink.m_ftText)
			hftOld = dc.SelectFont(m_styleLink.m_ftText);

		newLink.strText = CA2T(pElemChild->GetText(), CP_UTF8);

		Gdiplus::SizeF sz;

		//dc.GetTextExtent(newLink.strText, newLink.strText.GetLength(), &sz);
		{
			Gdiplus::Graphics graphics(dc);
			BkGdiplusFont fontDraw(dc, dc.GetCurrentFont());
			Gdiplus::StringFormat format;
			Gdiplus::PointF ptOrigin(0, 0);
			Gdiplus::RectF  rectDraw;

			format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			graphics.MeasureString(newLink.strText, -1, fontDraw, ptOrigin, &rectDraw);

			rectDraw.GetSize(&sz);
		}

		if (NULL != m_styleLink.m_ftText)
			dc.SelectFont(hftOld);


		if (pt.X + sz.Width > m_rcWindow.right - GetStyle().m_nMarginX)
		{
			pt.X = m_rcWindow.left + GetStyle().m_nMarginX;
			pt.Y += GetStyle().m_nLineSpacing;
		}

		newLink.SetRect(pt.X, pt.Y, sz.Width, sz.Height);
		pt.X = newLink.rcLink.X + newLink.rcLink.Width + GetStyle().m_nSpacing;

		pElemChild->Attribute("id", (int *)&newLink.uCmdID);

		m_lstLink.AddTail(newLink);
	}

	void _DrawElement(CDCHandle &dc, TiXmlElement *pElemChild, WORD wFont, Gdiplus::PointF &pt, BOOL bOnlyCalc = FALSE)
	{
		TiXmlNode *pNodeChild = NULL;

		WORD wNewFont = wFont;
		COLORREF crTextOld = CLR_INVALID;

		BOOL bOnlyCalcPoint = bOnlyCalc;

		if (0 == stricmp(pElemChild->Value(), "br"))
		{
			pt.X = m_rcWindow.left + GetStyle().m_nMarginX;
			pt.Y += ScaleByDpi(GetStyle().m_nLineSpacing);
		}
		else if (0 == stricmp(pElemChild->Value(), "b"))
		{
			wNewFont = wFont | BKF_BOLD;
		}
		else if (0 == stricmp(pElemChild->Value(), "i"))
		{
			wNewFont = wFont | BKF_ITALIC;
		}
		else if (0 == stricmp(pElemChild->Value(), "u"))
		{
			wNewFont = wFont | BKF_UNDERLINE;
		}
		else if (0 == stricmp(pElemChild->Value(), "c"))
		{
			crTextOld = dc.SetTextColor(CBkObject::HexStringToColor(pElemChild->Attribute("color")));
		}
		else if (0 == stricmp(pElemChild->Value(), "a"))
		{
			if (!m_bLinkLoaded)
			{
				_AddLink(dc, pElemChild, pt);

				return;
			}

			bOnlyCalcPoint = TRUE;
		}
		else if (0 == stricmp(pElemChild->Value(), "o"))
		{
			pt.X += CBkObject::HexStringToULong(pElemChild->Attribute("offset"));
			if (pt.X > m_rcWindow.right - GetStyle().m_nMarginX)
			{
				int nLineLength = m_rcWindow.right - 2*GetStyle().m_nMarginX;
				int nFlowLen = pt.X - m_rcWindow.right - GetStyle().m_nMarginX;
				int nLine = (nFlowLen+nLineLength-1)/nLineLength;
				pt.Y += GetStyle().m_nLineSpacing*nLine;
				pt.X = m_rcWindow.left + GetStyle().m_nMarginX+nFlowLen%nLineLength;
			}
		}
		else if (0 == stricmp(pElemChild->Value(), "img"))
		{
			CString strSrc = CA2W(pElemChild->Attribute("src"));
			::Gdiplus::Image img(strSrc);
			if (img.GetLastStatus() == Gdiplus::Ok) {
				int width = img.GetWidth();
				int height = img.GetHeight();
				if (height > 0 && height > GetStyle().m_nLineSpacing) {
					width = static_cast<int>((GetStyle().m_nLineSpacing * img.GetWidth()) / height);
					height = GetStyle().m_nLineSpacing;
				}

				if (pt.X + width > m_rcWindow.right - GetStyle().m_nMarginX)
				{
					pt.X = m_rcWindow.left + GetStyle().m_nMarginX;
					pt.Y += GetStyle().m_nLineSpacing;
				}

				Gdiplus::Graphics graphics(dc);
				Gdiplus::Rect destRect(pt.X, pt.Y, ScaleByDpi(width), ScaleByDpi(height));

                graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);
				graphics.DrawImage(&img, destRect);

				pt.X += ScaleByDpi(width);
			}
		}

        dc.SelectFont(BkFontPool::GetFont(wNewFont, IsDpiAwareEnable()));

		while (NULL != (pNodeChild = pElemChild->IterateChildren(pNodeChild)))
		{
			_DrawNode(dc, pNodeChild, wNewFont, pt, bOnlyCalcPoint);
		}

        dc.SelectFont(BkFontPool::GetFont(wFont, IsDpiAwareEnable()));

		if (CLR_INVALID != crTextOld)
			dc.SetTextColor(crTextOld);
	}

	void _DrawLinks(CDCHandle &dc)
	{
		BOOL bDisabled = IsDisabled(TRUE);
		BOOL bHover = FALSE;
		POSITION pos = m_lstLink.GetHeadPosition();

		if (NULL != m_styleLink.m_ftText)
			dc.SelectFont(m_styleLink.m_ftText);

		if (bDisabled)
		{
			if (CLR_INVALID != m_styleLink.m_crDisabledText)
				dc.SetTextColor(m_styleLink.m_crDisabledText);
		}
		else
		{
			if (CLR_INVALID != m_styleLink.m_crText)
				dc.SetTextColor(m_styleLink.m_crText);
		}

		while (pos)
		{
			if (pos == m_posHover)
			{
				if (NULL != m_styleLink.m_ftText)
					dc.SelectFont(m_styleLink.m_ftHover);

				if (CLR_INVALID != m_styleLink.m_crText)
					dc.SetTextColor(m_styleLink.m_crHoverText);

				bHover = TRUE;
			}

			const _LinkInfoEx& link = m_lstLink.GetNext(pos);

			//dc.TextOut(link.rcLink.X, link.rcLink.Y, link.strText);

			{
				Gdiplus::Graphics graphics(dc);
				BkGdiplusFont fontDraw(dc, dc.GetCurrentFont());
				Gdiplus::StringFormat format;
				Gdiplus::SolidBrush blackBrush(Gdiplus::Color(254, GetRValue(dc.GetTextColor()), GetGValue(dc.GetTextColor()), GetBValue(dc.GetTextColor())));
			
				format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
				graphics.SetTextRenderingHint((Gdiplus::TextRenderingHint)GetStyle().m_nTxtRndHit);

				graphics.DrawString(
					link.strText,
					-1,
					fontDraw,
					link.rcLink,
					&format,
					&blackBrush);
			}

			if (bHover)
			{
				if (NULL != m_styleLink.m_ftText)
					dc.SelectFont(m_styleLink.m_ftText);

				if (CLR_INVALID != m_styleLink.m_crText)
					dc.SetTextColor(m_styleLink.m_crText);

				bHover = FALSE;
			}
		}
	}

	void _DrawNode(CDCHandle &dc, TiXmlNode *pNodeChild, WORD wFont, Gdiplus::PointF &pt, BOOL bOnlyCalcPoint = FALSE)
	{
		switch (pNodeChild->Type())
		{
		case TiXmlNode::ELEMENT:
			{
				TiXmlElement *pElemChild = pNodeChild->ToElement();

				_DrawElement(dc, pElemChild, wFont, pt, bOnlyCalcPoint);
			}
			break;

		case TiXmlNode::TEXT:
			{
				CString strText = CA2T(pNodeChild->Value(), CP_UTF8);
				LPCTSTR lpszDraw = strText;
				int nFullLength = strText.GetLength(), nLength = 0;

				while (TRUE)
				{
					nLength = nFullLength - nLength;
					nFullLength = nLength;
					Gdiplus::SizeF sz(0 , 0);

					while (nLength > 0)
					{
	
						Gdiplus::Graphics graphics(dc);
						BkGdiplusFont fontDraw(dc, dc.GetCurrentFont());
						Gdiplus::StringFormat format;
						Gdiplus::PointF ptOrigin(0, 0);
						Gdiplus::RectF  rectDraw;

						format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
						graphics.MeasureString(lpszDraw, nLength, fontDraw, ptOrigin, &rectDraw);

						rectDraw.GetSize(&sz);

						//dc.GetTextExtent(lpszDraw, nLength, &sz);

						if (pt.X + sz.Width <= m_rcWindow.right - GetStyle().m_nMarginX)
							break;

						nLength --;
					}

					if (!bOnlyCalcPoint)
					{
						//dc.TextOut(pt.X, pt.Y, lpszDraw, nLength);
						Gdiplus::Graphics graphics(dc);
						BkGdiplusFont fontDraw(dc, dc.GetCurrentFont());
						Gdiplus::StringFormat format;
						Gdiplus::SolidBrush blackBrush(Gdiplus::Color(254, GetRValue(dc.GetTextColor()), GetGValue(dc.GetTextColor()), GetBValue(dc.GetTextColor())));
						//Gdiplus::SolidBrush blackBrush(Gdiplus::Color(254, 0xFF, 0xFF, 0xFF));

						Gdiplus::RectF layoutRect(pt.X, pt.Y, sz.Width + 1, sz.Height + 1);

						format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
						graphics.SetTextRenderingHint((Gdiplus::TextRenderingHint)GetStyle().m_nTxtRndHit);

						graphics.DrawString(
							lpszDraw,
							-1,
							fontDraw,
							layoutRect,
							&format,
							&blackBrush);
					}

					if (nFullLength == nLength)
					{
						pt.X += sz.Width;
						break;
					}

					pt.X = m_rcWindow.left + GetStyle().m_nMarginX;
					pt.Y += GetStyle().m_nLineSpacing;
					lpszDraw += nLength;
				}

				pt.X += GetStyle().m_nSpacing;
			}
			break;
		}
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		POSITION posHover = NULL;
		POSITION pos = m_lstLink.GetHeadPosition();

		while (pos)
		{
			posHover = pos;
			const _LinkInfoEx& link = m_lstLink.GetNext(pos);
			CRect rcLink(
				link.rcLink.X, 
				link.rcLink.Y, 
				link.rcLink.X + link.rcLink.Width, 
				link.rcLink.Y + link.rcLink.Height);

			if (rcLink.PtInRect(point))
			{
				if (m_posHover != posHover)
				{
					if (m_posHover)
					{
						_LinkInfoEx& linkOldHover = m_lstLink.GetAt(m_posHover);

						CRect rcLinkOld(
							linkOldHover.rcLink.X, 
							linkOldHover.rcLink.Y, 
							linkOldHover.rcLink.X + linkOldHover.rcLink.Width, 
							linkOldHover.rcLink.Y + linkOldHover.rcLink.Height);

						NotifyInvalidateRect(rcLinkOld);
					}

					m_posHover = posHover;
					NotifyInvalidateRect(rcLink);
				}

				return;
			}
		}

		OnMouseLeave();
	}

	void OnMouseLeave()
	{
		if (m_posHover)
		{
			const _LinkInfoEx& linkOldHover = m_lstLink.GetAt(m_posHover);

			CRect rcLink(
				linkOldHover.rcLink.X, 
				linkOldHover.rcLink.Y, 
				linkOldHover.rcLink.X + linkOldHover.rcLink.Width, 
				linkOldHover.rcLink.Y + linkOldHover.rcLink.Height);
			NotifyInvalidateRect(rcLink);
		}

		m_posHover = NULL;
	}
	
	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		__super::OnWindowPosChanged(lpWndPos);
		m_lstLink.RemoveAll();
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		__super::OnLButtonUp(nFlags, point);
	}

protected:

	CAtlList<_LinkInfoEx> m_lstLink;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()	
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		 MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		 MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	BKWIN_END_MSG_MAP()
};

class CBkImageEx2 : public CBkImageEx
{

	BKOBJ_DECLARE_CLASS_NAME(CBkImageEx2, "imgex2")
public:

	CBkImageEx2()
	{
		m_bStretch = FALSE;
		m_nTopDraw = 0;
	}

protected:

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		CBkWindow::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);

		if (pSkin && m_dlgpos.nCount != 4)
		{
			SIZE sizeImage;
			sizeImage = pSkin->GetSkinSize();

			if (sizeImage.cx)
				pSize->cx = sizeImage.cx;
			if (sizeImage.cy)
				pSize->cy = sizeImage.cy;

			return TRUE;
		}

		return 0;
	}

	void OnPaint(CDCHandle dc)
	{
		CBkSkinBase *pBkskin = BkSkin::GetSkin(m_strSkinName);
		if (pBkskin)
		{
			SIZE szSkin = pBkskin->GetSkinSize();
			CRect rcSrc(0, m_nTopDraw, szSkin.cx, szSkin.cy);
			rcSrc.OffsetRect(m_nSub * szSkin.cx, 0);

			CRect rcDraw = m_rcWindow;

			if (rcSrc.top > szSkin.cy)
			{
				rcSrc.top = szSkin.cy;
			}

			if (!m_bStretch)
			{
				int nWidthMin = min(rcDraw.Width(), rcSrc.Width());
				int nHeightMin = min(rcDraw.Height(), rcSrc.Height());

				rcDraw.right = rcDraw.left + nWidthMin;
				rcDraw.bottom = rcDraw.top + nHeightMin;

				rcSrc.right = rcSrc.left + nWidthMin;
				rcSrc.bottom = rcSrc.top + nHeightMin;
			}

			pBkskin->DrawImage(dc, rcDraw, rcSrc, m_nAlpha);
		}
	}

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("stretch", m_bStretch, FALSE)
		BKWIN_INT_ATTRIBUTE("topdraw", m_nTopDraw, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()

	BOOL m_bStretch;
	int  m_nTopDraw;
};

class CBkLineEx : public CBkLine
{
	BKOBJ_DECLARE_CLASS_NAME(CBkLineEx, "hrex")
public:

	CBkLineEx()
	{
		//m_nAlpha = 255;
	}

protected:

	// Do nothing
	void OnPaint(CDCHandle dc)
	{
		DWORD dwColor = (CLR_INVALID == m_crBg) ? GetStyle().m_crBg : m_crBg;

		using namespace Gdiplus;

		Graphics graphics(dc);
		Pen penBlack(Gdiplus::Color(GetStyle().m_nAlphaBg, GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor)), m_nLineSize);

		penBlack.SetDashStyle((DashStyle)m_nPenStyle);

		Point points[2] = 
		{
			Point(m_rcWindow.left, m_rcWindow.top),
			Point(m_rcWindow.right, m_rcWindow.bottom),
		};				

		graphics.DrawLines(&penBlack, points, 2);
	}

	//BKWIN_DECLARE_ATTRIBUTES_BEGIN()
	//	BKWIN_INT_ATTRIBUTE("alpha", m_nAlpha, FALSE)
	//BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
	BKWIN_END_MSG_MAP()

//	int m_nAlpha;
	
};

class CBkDlgBtnEx2 : public CBkDialogButton
{

	BKOBJ_DECLARE_CLASS_NAME(CBkDlgBtnEx2, "dlgbtnEx2")

public:

	CBkDlgBtnEx2()
	{
	}

	~CBkDlgBtnEx2()
	{
	}

	// Draw background default
	virtual void DrawBkgnd(CDCHandle& dc)
	{
		const BkStyle& theStyle = GetStyle();
		if (!theStyle.m_strSkinName.IsEmpty())
		{
			CBkSkinBase* pSkin = BkSkin::GetSkin(theStyle.m_strSkinName);
			if (pSkin)
			{
                pSkin->Stretch(dc, m_rcWindow, IIF_STATE4(GetState(), 0, 1, 2, 3), IsDpiAwareEnable());
			}
		}
		else
		{
			CBkDialogButton::DrawBkgnd(dc);
		}
	}
};

#pragma warning(default:4244)
#pragma warning(default:4996)