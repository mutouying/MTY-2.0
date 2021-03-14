//////////////////////////////////////////////////////////////////////////
//   File Name: BkSkin
// Description: BkWindow Skin Definition
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#ifndef __bksink__h__
#define __bksink__h__

#include "bkobject.h"
#include "bkimage.h"
#include <bkres/bkbmppool.h>
#include <bkres/bkpngpool.h>
#include <bkres/bkjpgpool.h>
#include <bkres/bkskinpool.h>

#include "bkbitmap.h"
#include <map>

#pragma warning(disable:4244)

// State Define
enum {
    BkWndState_Normal       = 0x00000000UL, 
    BkWndState_Hover        = 0x00000001UL, 
    BkWndState_PushDown     = 0x00000002UL, 
    BkWndState_Check        = 0x00000004UL, 
    BkWndState_Invisible    = 0x00000008UL, 
    BkWndState_Disable      = 0x00000010UL, 
};

#define IIF_STATE2(the_state, normal_value, hover_value) \
    (((the_state) & BkWndState_Hover) ? (hover_value) : (normal_value))

#define IIF_STATE3(the_state, normal_value, hover_value, pushdown_value) \
    (((the_state) & BkWndState_PushDown) ? (pushdown_value) : IIF_STATE2(the_state, normal_value, hover_value))

#define IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value) \
    (((the_state) & BkWndState_Disable) ? (disable_value) : IIF_STATE3(the_state, normal_value, hover_value, pushdown_value))

class CBkSkinBase : public CBkObject
{
public:
    enum SkinDrawMode
    {
        SkinDrawMode_Normal                         = 0,
        SkinDrawMode_FullCtrlWindow_Stretch         = 1,
    };

public:
	CBkSkinBase()
	{
		m_bLoaded = FALSE;
	}
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState) = NULL;
	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
	}
	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nAlpha)
	{

	}
	virtual void DrawImage(CDCHandle dc, CPoint point[3], CRect &rcSrc)
	{

	}
	virtual void DrawBackground(CDCHandle dc, CRect &rcDraw)
	{
	}
	virtual void DrawImageGray(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
	}

	virtual void DrawFlip(CDCHandle dc, CRect rcDst)
	{

	}

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return TRUE;
    }

	virtual BOOL LoadImg()
	{
		return FALSE;
	}

	virtual void DrawImage(Gdiplus::Graphics& graphics, CRect &rcDraw, CRect &rcSr, Gdiplus::ImageAttributes& imageAttributes)
	{

	}

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState, int nMode)
    {

    }

    virtual void Stretch(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha = 255)
    {

    }

    virtual void DrawNineCels(CDCHandle dc, int nState, const CRect &rcDraw, const CRect &rcCel, BYTE byAlpha)
    {

    }
    //
    enum {
        Frame_Part_All        = 0x0000002FUL, 
        Frame_Part_Top        = 0x00000001UL, 
        Frame_Part_Middle     = 0x00000002UL, 
        Frame_Part_Bottom     = 0x00000004UL, 
        Frame_Part_Left       = 0x00000008UL, 
        Frame_Part_Center     = 0x00000010UL, 
        Frame_Part_Right      = 0x00000020UL, 
    };

    static void HorzExtendDraw(CDCHandle &dc, CBkImage &imgDraw, CRect &rcDraw, LONG lSkinLeft, int nSubImage = -1)
    {
        SIZE sizeSkin;

        imgDraw.GetImageSize(sizeSkin);

        if (sizeSkin.cx > rcDraw.Width())
            rcDraw.right = rcDraw.left + sizeSkin.cx;

        if (imgDraw.GetSubImageWidth() > 0)
        {
            sizeSkin.cx = imgDraw.GetSubImageWidth();
        }

        imgDraw.BitBlt(
            dc, 
            rcDraw.left, rcDraw.top, 
            lSkinLeft, sizeSkin.cy, 
            0, 0, 
            SRCCOPY, nSubImage
            );
        imgDraw.BitBlt(
            dc, 
            rcDraw.right - sizeSkin.cx + lSkinLeft + 1, rcDraw.top, 
            sizeSkin.cx - lSkinLeft - 1, sizeSkin.cy, 
            lSkinLeft + 1, 0, 
            SRCCOPY, nSubImage
            );
        imgDraw.StretchBlt(
            dc, 
            rcDraw.left + lSkinLeft, rcDraw.top, 
            rcDraw.Width() - sizeSkin.cx + 1, sizeSkin.cy, 
            lSkinLeft, 0, 
            1, sizeSkin.cy, 
            SRCCOPY, nSubImage
            );
    }

    static void FrameDraw(CDCHandle &dc, CBkImage &imgDraw, CRect &rcDraw, LONG lSkinLeft, LONG lSkinTop, COLORREF crBg, UINT uDrawPart = Frame_Part_All)
    {
        SIZE sizeSkin;
        CRect rcClient = rcDraw;

        ATLASSERT(dc.m_hDC);
        ATLASSERT(imgDraw.m_hBitmap);
        ATLASSERT(rcDraw.Width() && rcDraw.Height());

        imgDraw.GetImageSize(sizeSkin);

        rcClient.DeflateRect(
            (uDrawPart & Frame_Part_Left)   ? lSkinLeft                     : 0, 
            (uDrawPart & Frame_Part_Top)    ? lSkinTop                      : 0, 
            (uDrawPart & Frame_Part_Right)  ? (sizeSkin.cx - lSkinLeft - 1) : 0, 
            (uDrawPart & Frame_Part_Bottom) ? (sizeSkin.cy - lSkinTop - 1)  : 0 
            );

        if ((Frame_Part_Left | Frame_Part_Top) == (uDrawPart & (Frame_Part_Left | Frame_Part_Top)))
        {
            imgDraw.BitBlt(
                dc, 
                rcDraw.left, rcDraw.top, 
                lSkinLeft, lSkinTop, 
                0, 0, 
                SRCCOPY
                );
        }
        if ((Frame_Part_Right | Frame_Part_Top) == (uDrawPart & (Frame_Part_Right | Frame_Part_Top)))
        {
            imgDraw.BitBlt(
                dc, 
                rcClient.right, rcDraw.top, 
                sizeSkin.cx - lSkinLeft - 1, lSkinTop, 
                lSkinLeft + 1, 0, 
                SRCCOPY
                );
        }
        if ((Frame_Part_Left | Frame_Part_Bottom) == (uDrawPart & (Frame_Part_Left | Frame_Part_Bottom)))
        {
            imgDraw.BitBlt(
                dc, 
                rcDraw.left, rcClient.bottom, 
                lSkinLeft, sizeSkin.cy - lSkinTop - 1, 
                0, lSkinTop + 1, 
                SRCCOPY
                );
        }
        if ((Frame_Part_Right | Frame_Part_Bottom) == (uDrawPart & (Frame_Part_Right | Frame_Part_Bottom)))
        {
            imgDraw.BitBlt(
                dc, 
                rcClient.right, rcClient.bottom, 
                sizeSkin.cx - lSkinLeft - 1, sizeSkin.cy - lSkinTop - 1, 
                lSkinLeft + 1, lSkinTop + 1, 
                SRCCOPY
                );
        }
        if (Frame_Part_Top == (uDrawPart & Frame_Part_Top))
        {
            imgDraw.StretchBlt(
                dc, 
                rcClient.left, rcDraw.top, 
                rcClient.Width(), lSkinTop, 
                lSkinLeft, 0, 
                1, lSkinTop, 
                SRCCOPY
                );
        }
        if (Frame_Part_Left == (uDrawPart & Frame_Part_Left))
        {
            imgDraw.StretchBlt(
                dc, 
                rcDraw.left, rcClient.top, 
                lSkinLeft, rcClient.Height(), 
                0, lSkinTop, 
                lSkinLeft, 1, 
                SRCCOPY
                );
        }
        if (Frame_Part_Bottom == (uDrawPart & Frame_Part_Bottom))
        {
            imgDraw.StretchBlt(
                dc, 
                rcClient.left, rcDraw.bottom - sizeSkin.cy + lSkinTop + 1, 
                rcClient.Width(), sizeSkin.cy - lSkinTop - 1, 
                lSkinLeft, lSkinTop + 1, 
                1, sizeSkin.cy - lSkinTop - 1, 
                SRCCOPY
                );
        }
        if (Frame_Part_Right == (uDrawPart & Frame_Part_Right))
        {
            imgDraw.StretchBlt(
                dc, 
                rcClient.right, rcClient.top, 
                sizeSkin.cx - lSkinLeft - 1, rcClient.Height(), 
                lSkinLeft + 1, lSkinTop, 
                sizeSkin.cx - lSkinLeft - 1, 1, 
                SRCCOPY
                );
        }

        if (CLR_INVALID != crBg)
            dc.FillSolidRect(rcClient, crBg);
    }

    typedef struct _FRG_PARAM 
    {
        LONG lOffset;
        COLORREF crColor;
    } FRG_PARAM;

    typedef BOOL (WINAPI * FnGradientFill)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);

    static BOOL WINAPI GradientFill2(HDC hDC, PTRIVERTEX pVertices, DWORD nVertices, PVOID pMeshElements, ULONG nMeshElements, ULONG dwMode)
    {
        HMODULE hMod = ::LoadLibrary(L"msimg32.dll");
        if (hMod)
        {
            FnGradientFill pfnGradientFill = (FnGradientFill)::GetProcAddress(hMod, "GradientFill");
            if (pfnGradientFill)
                pfnGradientFill(hDC, pVertices, nVertices, pMeshElements, nMeshElements, dwMode);
            ::FreeLibrary(hMod);
        }

        return TRUE;
    }

    static void GradientFillRectV(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount)
    {
        GRADIENT_RECT gRect = {0, 1};
        TRIVERTEX vert[2] = {
            {rcFill.left, rcFill.top, 0, 0, 0, 0}, 
            {rcFill.right, rcFill.top, 0, 0, 0, 0}
        };
        int i = 0;

        for (i = 1; i < nCount && vert[0].y <= rcFill.bottom; i ++)
        {
            vert[0].y = vert[1].y;
            vert[1].y += params[i].lOffset;
            vert[0].Red     = GetRValue(params[i - 1].crColor) << 8;
            vert[0].Green   = GetGValue(params[i - 1].crColor) << 8;
            vert[0].Blue    = GetBValue(params[i - 1].crColor) << 8;
            vert[1].Red     = GetRValue(params[i].crColor) << 8;
            vert[1].Green   = GetGValue(params[i].crColor) << 8;
            vert[1].Blue    = GetBValue(params[i].crColor) << 8;

            HMODULE hMod = ::LoadLibrary(L"msimg32.dll");
            if (hMod)
            {
                GradientFill2(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
            }
        }
    }

    static void GradientFillRectH(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount)
    {
        GRADIENT_RECT gRect = {0, 1};
        TRIVERTEX vert[2] = {
            {rcFill.left, rcFill.top, 0, 0, 0, 0}, 
            {rcFill.left, rcFill.bottom, 0, 0, 0, 0}
        };
        int i = 0;

        for (i = 1; i < nCount && vert[0].x <= rcFill.right; i ++)
        {
            vert[0].x = vert[1].x;
            vert[1].x += params[i].lOffset;
            vert[0].Red     = GetRValue(params[i - 1].crColor) << 8;
            vert[0].Green   = GetGValue(params[i - 1].crColor) << 8;
            vert[0].Blue    = GetBValue(params[i - 1].crColor) << 8;
            vert[1].Red     = GetRValue(params[i].crColor) << 8;
            vert[1].Green   = GetGValue(params[i].crColor) << 8;
            vert[1].Blue    = GetBValue(params[i].crColor) << 8;
            GradientFill2(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
        }
    }

    static void GradientFillRectV(HDC hdc, CRect &rcFill, COLORREF crTop, COLORREF crBottom)
    {
        FRG_PARAM frgDraw[2] = {
            {0, crTop}, 
            {rcFill.Height(), crBottom}, 
        };

        GradientFillRectV(hdc, rcFill, frgDraw, 2);
    }

    static void GradientFillRectH(HDC hdc, CRect &rcFill, COLORREF crLeft, COLORREF crRight)
    {
        FRG_PARAM frgDraw[2] = {
            {0, crLeft}, 
            {rcFill.Width(), crRight}, 
        };

        GradientFillRectH(hdc, rcFill, frgDraw, 2);
    }
	BOOL m_bLoaded;
};

class CBkImageSkin
    : public CBkImage
    , public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkImageSkin, "imglst")

public:
    CBkImageSkin()
		: m_lstretchX(0), m_lstretchY(0)
    {

    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
		if (!m_bLoaded)
		{
			return;
		}
		// ֧��������ʾ�� by ZC. 2010-11-2��
		if (0 != m_lstretchX && 0 != m_lstretchY)
		{
			int nSubImage = dwState;
			int nSrcPosX = 0, nSrcPosY = 0, nSrcWidth = 0, nSrcHeight = 0;

			BITMAP bmp = { 0 }; 
			GetBitmap(&bmp);

			nSrcHeight = bmp.bmHeight;

			if (-1 == nSubImage)
				nSrcWidth = bmp.bmWidth;
			else
				nSrcWidth = m_lSubImageWidth;
			
			dc.SetStretchBltMode(COLORONCOLOR) ;
			CBkImage::StretchBlt(dc, rcDraw.left, rcDraw.top, m_lstretchX, m_lstretchY, nSrcPosX, nSrcPosY, nSrcWidth, nSrcHeight, SRCCOPY, nSubImage);
		}
		else
		{
			CBkImage::Draw(dc, rcDraw.left, rcDraw.top, dwState);
		}
    }

    virtual SIZE GetSkinSize()
    {
		// ֧��������ʾ�� by ZC. 2010-11-2��
		if (0 != m_lstretchX && 0 != m_lstretchY)
		{
			SIZE ret = {m_lstretchX, m_lstretchY};

			return ret;
		}
		
		{
			SIZE ret = {0, 0};

			GetImageSize(ret);

			if (0 != m_lSubImageWidth)
				ret.cx = m_lSubImageWidth;

			return ret;
		}
    }

    virtual BOOL IgnoreState()
    {
        return (0 == m_lSubImageWidth);
    }

    HRESULT OnSrcChange(CStringA& strValue, BOOL bLoading)
    {
		m_strSrc = strValue;

        return S_OK;
    }

	BOOL LoadImg()
	{
		if (m_bLoaded)
			return TRUE;
		Attach(BkBmpPool::GetBitmap((UINT)::StrToIntA(m_strSrc)));
		m_bLoaded = TRUE;
		return m_bLoaded;
	}
protected:

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_CUSTOM_ATTRIBUTE("src", OnSrcChange)
        BKWIN_ENUM_ATTRIBUTE("mode", int, TRUE)
        BKWIN_ENUM_VALUE("none", CBkImage::ModeNone)
        BKWIN_ENUM_VALUE("mask", CBkImage::ModeMaskColor)
        BKWIN_ENUM_VALUE("alpha", CBkImage::ModeAlpha)
        BKWIN_ENUM_END(m_nTransparentMode)
        BKWIN_COLOR_ATTRIBUTE("maskcolor", m_crMask, TRUE)
        BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
		BKWIN_INT_ATTRIBUTE("stretch-x", m_lstretchX, 0)
		BKWIN_INT_ATTRIBUTE("stretch-y", m_lstretchY, 0)
    BKWIN_DECLARE_ATTRIBUTES_END()
protected:
	LONG m_lstretchX; 
	LONG m_lstretchY;
	CStringA m_strSrc;
};

class CBkSkinImgFrame : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinImgFrame, "imgframe")

public:
    CBkSkinImgFrame()
        : m_crBg(CLR_INVALID)
        , m_lSkinParamLeft(0)
        , m_lSkinParamTop(0)
        , m_uDrawPart(Frame_Part_All)
    {
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
		if (!m_bLoaded)
		{
			return;
		}
        if (m_imgSkin.M_HOBJECT)
        {
            FrameDraw(dc, m_imgSkin, rcDraw, m_lSkinParamLeft, m_lSkinParamTop, m_crBg, m_uDrawPart);
        }
    }

    virtual void Stretch(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha /* = 255 */)
    {
        if (!m_bLoaded)
        {
            return;
        }
        if (m_imgSkin.M_HOBJECT)
        {
            FrameDraw(dc, m_imgSkin, rcDraw, m_lSkinParamLeft, m_lSkinParamTop, m_crBg, m_uDrawPart);
        }
    }

    virtual BOOL IgnoreState()
    {
        return m_imgSkin.IgnoreState();
    }

	virtual BOOL LoadImg()
	{
		m_bLoaded = m_imgSkin.LoadImg();
		return m_bLoaded;
	}

protected:
    CBkImageSkin m_imgSkin;
    LONG m_lSkinParamLeft;
    LONG m_lSkinParamTop;
    COLORREF m_crBg;
    UINT m_uDrawPart;

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_CHAIN_ATTRIBUTE(m_imgSkin, TRUE)
//         BKWIN_UINT_ATTRIBUTE("src", m_imgSkin, TRUE)
        BKWIN_COLOR_ATTRIBUTE("crbg", m_crBg, TRUE)
        BKWIN_INT_ATTRIBUTE("left", m_lSkinParamLeft, TRUE)
        BKWIN_INT_ATTRIBUTE("top", m_lSkinParamTop, TRUE)
        BKWIN_ENUM_ATTRIBUTE("part", UINT, TRUE)
            BKWIN_ENUM_VALUE("all", Frame_Part_All)
            BKWIN_ENUM_VALUE("top", (Frame_Part_All & ~Frame_Part_Bottom))
            BKWIN_ENUM_VALUE("middle", (Frame_Part_All & ~(Frame_Part_Bottom | Frame_Part_Top)))
            BKWIN_ENUM_VALUE("bottom", (Frame_Part_All & ~Frame_Part_Top))
            BKWIN_ENUM_VALUE("left", (Frame_Part_All & ~Frame_Part_Right))
            BKWIN_ENUM_VALUE("center", (Frame_Part_All & ~(Frame_Part_Right | Frame_Part_Left)))
            BKWIN_ENUM_VALUE("right", (Frame_Part_All & ~Frame_Part_Left))
        BKWIN_ENUM_END(m_uDrawPart)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
};

class CBkSkinImgHorzExtend : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinImgHorzExtend, "imghorzex")
public:
    CBkSkinImgHorzExtend()
        : /*
        m_lSkinSubWidth(0)
                , */
        m_lSkinParamLeft(0)
    {
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
		if (!m_bLoaded)
		{
			return;
		}

        if (m_imgSkin.M_HOBJECT)
        {
//             m_imgSkin.SetSubImageWidth(m_lSkinSubWidth);
            HorzExtendDraw(
                dc, m_imgSkin, rcDraw, m_lSkinParamLeft, 
                (-1 == dwState) ? -1 : IIF_STATE4(dwState, 0, 1, 2, 3)
//                (dwState & BkWndState_PushDown) ? 2 : ((dwState & BkWndState_Hover) ? 1 : 0)
                );
        }
    }

    virtual void Stretch(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha /* = 255 */)
    {
        if (!m_bLoaded)
        {
            return;
        }

        if (m_imgSkin.M_HOBJECT)
        {
            //             m_imgSkin.SetSubImageWidth(m_lSkinSubWidth);
            HorzExtendDraw(
                dc, m_imgSkin, rcDraw, m_lSkinParamLeft, 
                (-1 == dwState) ? -1 : IIF_STATE4(dwState, 0, 1, 2, 3)
                //                (dwState & BkWndState_PushDown) ? 2 : ((dwState & BkWndState_Hover) ? 1 : 0)
                );
        }
    }

    SIZE GetSkinSize()
    {
        SIZE size;

        m_imgSkin.GetImageSize(size);

//         LONG lSubImageWidth = m_imgSkin.GetSubImageWidth();
//         if (lSubImageWidth > 0)
//             size.cx = lSubImageWidth;

        size.cx = 0;

        return size;
    }

    virtual BOOL IgnoreState()
    {
        return m_imgSkin.IgnoreState();
    }

	virtual BOOL LoadImg()
	{
		m_bLoaded = m_imgSkin.LoadImg();
		return m_bLoaded;
	}
protected:
    CBkImageSkin m_imgSkin;
//     LONG m_lSkinSubWidth;
    LONG m_lSkinParamLeft;

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_CHAIN_ATTRIBUTE(m_imgSkin, TRUE)
//         BKWIN_UINT_ATTRIBUTE("src", m_imgSkin, TRUE)
//         BKWIN_INT_ATTRIBUTE("subwidth", m_lSkinSubWidth, TRUE)
        BKWIN_INT_ATTRIBUTE("left", m_lSkinParamLeft, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
};

class CBkSkinButton : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinButton, "button")
public:
    CBkSkinButton()
        : m_crBorder(RGB(0x70, 0x70, 0x70))
        , m_crBg(RGB(0xEE, 0xEE, 0xEE))
        , m_crBgUpNormal(RGB(0xEE, 0xEE, 0xEE))
        , m_crBgUpHover(RGB(0xEE, 0xEE, 0xEE))
        , m_crBgUpPush(RGB(0xCE, 0xCE, 0xCE))
        , m_crBgDownNormal(RGB(0xD6, 0xD6, 0xD6))
        , m_crBgDownHover(RGB(0xE0, 0xE0, 0xE0))
        , m_crBgDownPush(RGB(0xC0, 0xC0, 0xC0))
		, m_bUseBorder(TRUE)
    {

    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        CRect rcBg = rcDraw;

        dc.FillSolidRect(rcDraw, m_crBg);

		if (m_bUseBorder)
		{
			rcBg.DeflateRect(2, 2);
		}

        if (BkWndState_Disable == (BkWndState_Disable & dwState))
        {
            
        }
        else
            GradientFillRectV(
                dc, rcBg, 
                IIF_STATE3(dwState, m_crBgUpNormal, m_crBgUpHover, m_crBgUpPush), 
                IIF_STATE3(dwState, m_crBgDownNormal, m_crBgDownHover, m_crBgDownPush));

//         rcBg.DeflateRect(2, 2, 2, rcDraw.Height() / 2);
//         dc.FillSolidRect(
//             rcBg, 
//             IIF_STATE3(dwState, m_crBgUpNormal, m_crBgUpHover, m_crBgUpPush)
//             );
// 
//         rcBg.OffsetRect(0, rcBg.Height());
//         dc.FillSolidRect(
//             rcBg, 
//             IIF_STATE3(dwState, m_crBgDownNormal, m_crBgDownHover, m_crBgDownPush)
//             );

		if (m_bUseBorder)
		{
			CPen penFrame;
			penFrame.CreatePen(
				PS_SOLID, 
				1, 
				m_crBorder
				);

			HPEN hpenOld = dc.SelectPen(penFrame);
			HBRUSH hbshOld = NULL, hbshNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

			hbshOld = dc.SelectBrush(hbshNull);

			dc.Rectangle(rcDraw);

			//dc.RoundRect(rcDraw, CPoint(2, 2));

			//         if (dwState & BkWndState_PushDown)
			//         {
			//             rcDraw.DeflateRect(1, 1, 0, 0);
			// 
			//             dc.Rectangle(rcDraw);
			//         }

			dc.SelectBrush(hbshOld);
			dc.SelectPen(hpenOld);
		}        
    }

    virtual BOOL IgnoreState()
    {
        return FALSE;
    }

protected:
    COLORREF m_crBg;
    COLORREF m_crBorder;
    COLORREF m_crBgUpNormal;
    COLORREF m_crBgUpHover;
    COLORREF m_crBgUpPush;
    COLORREF m_crBgDownNormal;
    COLORREF m_crBgDownHover;
    COLORREF m_crBgDownPush;
	BOOL m_bUseBorder;

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_COLOR_ATTRIBUTE("bg", m_crBg, TRUE)
        BKWIN_COLOR_ATTRIBUTE("border", m_crBorder, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgup", m_crBgUpNormal, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bguphover", m_crBgUpHover, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bguppush", m_crBgUpPush, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgdown", m_crBgDownNormal, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgdownhover", m_crBgDownHover, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgdownpush", m_crBgDownPush, TRUE)
		BKWIN_INT_ATTRIBUTE("useborder", m_bUseBorder, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkSkinGradation
    : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinGradation, "gradation")
public:
    CBkSkinGradation()
        : m_uDirection(0)
        , m_crFrom(CLR_INVALID)
        , m_crTo(CLR_INVALID)
        , m_nSize(0)
    {
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        CRect rcGradation = rcDraw;
        CRgn rgnDraw;

        rgnDraw.CreateRectRgn(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

        dc.FillSolidRect(rcDraw, m_crTo);

        int nID = dc.SaveDC();

        dc.SelectClipRgn(rgnDraw);

        if (0 == m_uDirection)
        {
            if (0 < m_nSize)
                rcGradation.right = rcGradation.left + m_nSize;
            GradientFillRectH(dc, rcGradation, m_crFrom, m_crTo);
        }
        else
        {
            if (0 < m_nSize)
                rcGradation.bottom = rcGradation.top + m_nSize;
            GradientFillRectV(dc, rcGradation, m_crFrom, m_crTo);
        }

        dc.RestoreDC(nID);
    }

    virtual void Stretch(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha = 255)
    {
        CRect rcGradation = rcDraw;
        CRgn rgnDraw;

        rgnDraw.CreateRectRgn(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

        dc.FillSolidRect(rcDraw, m_crTo);

        int nID = dc.SaveDC();

        dc.SelectClipRgn(rgnDraw);

        if (0 == m_uDirection)
        {
            if (0 < m_nSize)
                rcGradation.right = rcGradation.left + m_nSize;
            GradientFillRectH(dc, rcGradation, m_crFrom, m_crTo);
        }
        else
        {
            if (0 < m_nSize)
                rcGradation.bottom = rcGradation.top + m_nSize;
            GradientFillRectV(dc, rcGradation, m_crFrom, m_crTo);
        }

        dc.RestoreDC(nID);
    }

protected:
    COLORREF m_crFrom;
    COLORREF m_crTo;
    UINT     m_uDirection;
    int      m_nSize; 
public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_COLOR_ATTRIBUTE("from", m_crFrom, TRUE)
        BKWIN_COLOR_ATTRIBUTE("to", m_crTo, TRUE)
        BKWIN_INT_ATTRIBUTE("size", m_nSize, TRUE)
        BKWIN_ENUM_ATTRIBUTE("direction", UINT, TRUE)
            BKWIN_ENUM_VALUE("horz", 0)
            BKWIN_ENUM_VALUE("vert", 1)
        BKWIN_ENUM_END(m_uDirection)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
};

class CBkPngSkin
    : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPngSkin, "png")

public:
    CBkPngSkin()
        : m_uResID(0)
        , m_lSubImageWidth(0)
        , m_pImg(NULL)
        , m_pDpiImg(NULL)
		, m_nLeft(0)
		, m_nTop(0)
		, m_nMarginLeft(0)
		, m_nMarginRight(0)
        , m_nMarginTop(0)
        , m_nMarginBottom(0)
		, m_nFlip(0)
		, m_bGrayCache(FALSE)
		, m_nMiddleTile(0)
		, m_nFlipType(0)
        , m_lWantImgWidth(0)
		, m_dpiImageRate(1)
    {
    }

    ~CBkPngSkin()
    {
		for (std::map<CStringA, Gdiplus::Bitmap*>::iterator iter = m_mapHighQualityImage.begin();
			 iter != m_mapHighQualityImage.end();
			 ++iter)
		{
			Gdiplus::Image* pImage = iter->second;
			if (pImage) 
			{
				delete pImage;
			}
		}
		m_mapHighQualityImage.clear();
    }

    virtual BOOL Load(TiXmlElement* pXmlElem)
    {
        __super::Load(pXmlElem);
        
        if (0 == m_uResID)
            return TRUE;

        //m_pImg = BkPngPool::Get(m_uResID);

        return TRUE;
    }
    void Remove()
    {
        //static void RemoveItem(UINT uResID)
        BkPngPool::RemoveItem(m_uResID);
        m_pImg = NULL;
        m_pDpiImg = NULL;
        m_bLoaded = FALSE;

    }
	BOOL LoadImg()
	{
		if (m_bLoaded)
			return TRUE;
		m_pImg = BkPngPool::Get(m_uResID);
        m_pDpiImg = BkPngPool::GetDpiImg(m_uResID, &m_dpiImageRate);
		m_bLoaded = (m_pImg != NULL);
		return m_bLoaded;
	}
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
		if (!m_bLoaded)
		{
			return;
		}
        if (m_pImg)
        {	
            Gdiplus::Graphics graphics(dc);
			if (m_nFlip)
			{
				m_nFlipType = Gdiplus::RotateNoneFlipX;
			}

			if (m_nFlipType)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip((RotateFlipType)m_nFlipType);
			}

            SIZE size = {0, 0};
            if (m_pImg)
            {
                if (0 == m_lSubImageWidth)
                    size.cx = m_pImg->GetWidth();
                else
                    size.cx = m_lSubImageWidth;
                size.cy = m_pImg->GetHeight();
            }

            if (m_bmpCache.IsNull())
            {
                m_bmpCache.CreateFromImage(m_pImg);
            }

            if (0 == m_lSubImageWidth)
			{
				if (m_nMarginLeft)
				{
					/*graphics.DrawImage(
						m_pImg, 
						Gdiplus::Rect(rcDraw.left, rcDraw.top, m_nMarginLeft, rcDraw.Height()), 
						m_nLeft, 
						m_nTop, 
						m_nMarginLeft, 
						rcDraw.Height(), 
						Gdiplus::UnitPixel);*/

                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left, 
                        rcDraw.top, 
                        m_nMarginLeft, 
                        rcDraw.Height() < size.cy ? rcDraw.Height() : size.cy, 
                        m_bmpCache, 
                        m_nLeft, 
                        m_nTop, 
                        m_nMarginLeft, 
                        rcDraw.Height(),
                        255);
				}
                else if (m_nMarginTop)
                {
                    /*graphics.DrawImage(
                        m_pImg, 
                        Gdiplus::Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), m_nMarginTop), 
                        m_nLeft, 
                        m_nTop, 
                        rcDraw.Width(), 
                        m_nMarginTop, 
                        Gdiplus::UnitPixel);*/

                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left, 
                        rcDraw.top, 
                        rcDraw.Width(), 
                        m_nMarginTop, 
                        m_bmpCache, 
                        m_nLeft, 
                        m_nTop, 
                        rcDraw.Width(), 
                        m_nMarginTop,
                        255);
                }

				if (m_nMarginTop || m_nMarginBottom)
				{
					/*graphics.DrawImage(
						m_pImg, 
						Gdiplus::Rect(rcDraw.left + m_nMarginLeft, rcDraw.top + m_nMarginTop, rcDraw.Width() - m_nMarginLeft - m_nMarginRight, rcDraw.Height() - m_nMarginTop - m_nMarginBottom), 
						m_nLeft + m_nMarginLeft, 
						m_nTop + m_nMarginTop, 
						(size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
						(size.cy < rcDraw.Height() ? size.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
						Gdiplus::UnitPixel);*/

                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left + m_nMarginLeft, 
                        rcDraw.top + m_nMarginTop, 
                        rcDraw.Width() - m_nMarginLeft - m_nMarginRight, 
                        rcDraw.Height() - m_nMarginTop - m_nMarginBottom,
                        m_bmpCache, 
                        m_nLeft + m_nMarginLeft, 
                        m_nTop + m_nMarginTop, 
                        (size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
                        (size.cy < rcDraw.Height() ? size.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
                        255);
				}
				else
				{
                    if (m_nMiddleTile)
                    {
                        //�м����ƽ��
                        int nRemainWidth = rcDraw.Width() - m_nMarginLeft - m_nMarginRight;
                        int nTileWidth = m_pImg->GetWidth() - m_nMarginLeft - m_nMarginRight;
                        if (nRemainWidth > 0 && nTileWidth > 0)
                        {
                            int nDestLeft = rcDraw.left + m_nMarginLeft;
                            //ƽ�̣��ظ������м�Ĳ���
                            while (nRemainWidth >= nTileWidth)
                            {
                                /*graphics.DrawImage(
                                    m_pImg, 
                                    Gdiplus::Rect(nDestLeft, rcDraw.top, nTileWidth, rcDraw.Height()), 
                                    m_nLeft + m_nMarginLeft, 
                                    m_nTop, 
                                    nTileWidth, 
                                    rcDraw.Height(), 
                                    Gdiplus::UnitPixel);*/

                                CBkBitmap::AlphaBlend(
                                    dc, 
                                    nDestLeft, 
                                    rcDraw.top, 
                                    nTileWidth, 
                                    rcDraw.Height(),
                                    m_bmpCache, 
                                    m_nLeft + m_nMarginLeft, 
                                    m_nTop, 
                                    nTileWidth, 
                                    rcDraw.Height(),
                                    255);

                                nRemainWidth -= nTileWidth;
                                nDestLeft += nTileWidth;
                            }
                            //��ʣһС��
                            if (nRemainWidth > 0)
                            {
                                /*graphics.DrawImage(
                                    m_pImg, 
                                    Gdiplus::Rect(nDestLeft, rcDraw.top, nRemainWidth, rcDraw.Height()), 
                                    m_nLeft + m_nMarginLeft, 
                                    m_nTop, 
                                    nRemainWidth, 
                                    rcDraw.Height(), 
                                    Gdiplus::UnitPixel);*/

                                CBkBitmap::AlphaBlend(
                                    dc, 
                                    nDestLeft, 
                                    rcDraw.top, 
                                    nRemainWidth, 
                                    rcDraw.Height(),
                                    m_bmpCache, 
                                    m_nLeft + m_nMarginLeft, 
                                    m_nTop, 
                                    nRemainWidth, 
                                    rcDraw.Height(),
                                    255);
                            }
                        }
                    }
                    else
                    {
					    /*graphics.DrawImage(
						    m_pImg, 
						    Gdiplus::Rect(rcDraw.left + m_nMarginLeft, rcDraw.top, rcDraw.Width() - m_nMarginLeft - m_nMarginRight, rcDraw.Height()), 
						    m_nLeft + m_nMarginLeft, 
						    m_nTop, 
						    (size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
						    rcDraw.Height(), 
						    Gdiplus::UnitPixel);*/

                        CBkBitmap::AlphaBlend(
                            dc, 
                            rcDraw.left + m_nMarginLeft, 
                            rcDraw.top, 
                            rcDraw.Width() - m_nMarginLeft - m_nMarginRight, 
                            rcDraw.Height() < size.cy ? rcDraw.Height() : size.cy,
                            m_bmpCache, 
                            m_nLeft + m_nMarginLeft, 
                            m_nTop, 
                            (size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
                            rcDraw.Height(),
                            255);
                    }
				}				               

				if (m_nMarginRight)
				{
					/*graphics.DrawImage(
						m_pImg, 
						Gdiplus::Rect(rcDraw.right - m_nMarginRight, rcDraw.top, m_nMarginRight, rcDraw.Height()), 
						size.cx - m_nMarginRight, 
						m_nTop, 
						m_nMarginRight, 
						rcDraw.Height(), 
						Gdiplus::UnitPixel);*/

                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.right - m_nMarginRight, 
                        rcDraw.top, 
                        m_nMarginRight, 
                        rcDraw.Height() < size.cy ? rcDraw.Height() : size.cy,
                        m_bmpCache, 
                        size.cx - m_nMarginRight, 
                        m_nTop, 
                        m_nMarginRight, 
                        rcDraw.Height(),
                        255);
				}
                else if (m_nMarginBottom)
                {
                    /*graphics.DrawImage(
                        m_pImg, 
                        Gdiplus::Rect(rcDraw.left, rcDraw.bottom - m_nMarginBottom, rcDraw.Width(), m_nMarginBottom), 
                        m_nLeft, 
                        size.cy - m_nMarginBottom, 
                        rcDraw.Width(), 
                        m_nMarginBottom, 
                        Gdiplus::UnitPixel);*/

                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left, 
                        rcDraw.bottom - m_nMarginBottom, 
                        rcDraw.Width(), 
                        m_nMarginBottom,
                        m_bmpCache, 
                        m_nLeft, 
                        size.cy - m_nMarginBottom, 
                        rcDraw.Width(), 
                        m_nMarginBottom,
                        255);
                }
			}
            else
			{
				int nBlock = m_pImg->GetWidth() / m_lSubImageWidth;
				if (nBlock != 0)
					dwState = dwState % nBlock;
                //graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDraw.left, rcDraw.top, size.cx, size.cy), m_lSubImageWidth * dwState, 0, size.cx, size.cy, Gdiplus::UnitPixel);

                CBkBitmap::AlphaBlend(
                    dc, 
                    rcDraw.left, 
                    rcDraw.top, 
                    size.cx, 
                    size.cy,
                    m_bmpCache, 
                    m_lSubImageWidth * dwState, 
                    0, 
                    size.cx, 
                    size.cy,
                    255);
			}

			if (m_nFlipType)
			{
				using namespace Gdiplus;
				int nRecovery = 0;
				switch(m_nFlipType)
				{
				case Rotate90FlipNone:
					nRecovery = Rotate270FlipNone;	break;
				case Rotate180FlipNone:
					nRecovery = Rotate180FlipNone;	break;
				case Rotate270FlipNone:
					nRecovery = Rotate90FlipNone;	break;

				case RotateNoneFlipX:
					nRecovery = RotateNoneFlipX;	break;
				case Rotate90FlipX:
					nRecovery = Rotate270FlipX;		break;
				case Rotate180FlipX:
					nRecovery = Rotate180FlipX;		break;
				case Rotate270FlipX:
					nRecovery = Rotate90FlipX;		break;
				}

				if (nRecovery != 0)
					m_pImg->RotateFlip((RotateFlipType)nRecovery);
			}
        }
    }

	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
		if (m_pImg)
		{
			if (m_nFlip)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
			Gdiplus::Graphics graphics(dc);
			graphics.DrawImage(
				m_pImg,
				Gdiplus::Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()), 
				rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
				Gdiplus::UnitPixel);
			if (m_nFlip)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
		}
	}

	virtual void DrawImage(Gdiplus::Graphics& graphics, CRect &rcDraw, CRect &rcSr, Gdiplus::ImageAttributes& imageAttributes)
	{
		if (m_pImg)
		{
			graphics.DrawImage( m_pImg, Gdiplus::RectF(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()), 
				(Gdiplus::REAL)rcSr.left, 
				(Gdiplus::REAL)rcSr.top,                                                   
				(Gdiplus::REAL)rcSr.Width(),                                             
				(Gdiplus::REAL)rcSr.Height(), Gdiplus::UnitPixel, &imageAttributes); 
		}
	}

	virtual void DrawImageGray(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
		if (m_pImg)
		{
			int nWidth = rcDraw.Width();
			int nHeight	= rcDraw.Height();

			if (!m_bGrayCache)
			{
				if (m_nFlip)
				{
					using namespace Gdiplus;
					m_pImg->RotateFlip(RotateNoneFlipX);
				}

				HBITMAP hOldBitmap = NULL;
				m_bmpGrayCache.CreateDIBSection(nWidth, nHeight);
				CDC dcMem;
				dcMem.CreateCompatibleDC(NULL);
				hOldBitmap = dcMem.SelectBitmap(m_bmpGrayCache);

				Gdiplus::Graphics graphics(dcMem);
				graphics.DrawImage(
					m_pImg,
					Gdiplus::Rect(0, 0, nWidth, nHeight), 
					rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
					Gdiplus::UnitPixel);

				DWORD dwColor, dwTemp;
				DWORD red, green, blue, alphas;
				for (int x = 0; x < nWidth; x++)
				{
					for(int y = 0; y < nHeight; y++)
					{
						dwColor = m_bmpGrayCache.GetPixel(x, y);
						if (dwColor != 0)
						{
							alphas = dwColor & 0xff000000;
							red = (dwColor & 0xff0000) >> 16;
							green = (dwColor & 0xff00) >> 8;
							blue = dwColor & 0xff;
							dwTemp = (red * 30 + green * 59 + blue * 11) / 100;
							dwColor = dwTemp << 16;
							dwColor |= dwTemp << 8;
							dwColor |= dwTemp;
							dwColor |= alphas;
							m_bmpGrayCache.SetPixel(x, y, dwColor);
						}					
					}
				}
				dcMem.SelectBitmap(hOldBitmap);
				if (m_nFlip)
				{
					using namespace Gdiplus;
					m_pImg->RotateFlip(RotateNoneFlipX);
				}

				m_bGrayCache = TRUE;
			}
			
			CBkBitmap::AlphaBlend(dc.m_hDC, rcDraw.left, rcDraw.top, nWidth, nHeight, m_bmpGrayCache, 0, 0, nWidth, nHeight, 255);			
		}
	}

	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nAlpha)
	{
		if (m_pImg)
		{
			if (m_nFlip)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
			const int nRcWidth = rcDraw.Width();
			const int nRcHeight = rcDraw.Height();

			BITMAPINFO bmi;
			ZeroMemory(&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = nRcWidth;
			bmi.bmiHeader.biHeight = nRcHeight;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage = nRcWidth * nRcHeight * 4;

			HDC hMemDC = CreateCompatibleDC(NULL);
			HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);			
			HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hbitmap);

			Gdiplus::Graphics graphics(hMemDC);
			graphics.DrawImage(
				m_pImg, 
				Gdiplus::Rect(0, 0, nRcWidth, nRcHeight), 
				rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
				Gdiplus::UnitPixel);

			BLENDFUNCTION Blend;
			Blend.BlendOp = 0;
			Blend.BlendFlags = 0;
			Blend.AlphaFormat = AC_SRC_ALPHA;
			Blend.SourceConstantAlpha = (BYTE)nAlpha;

			::AlphaBlend(dc.m_hDC, rcDraw.left, rcDraw.top, nRcWidth, nRcHeight, hMemDC, 0, 0, nRcWidth, nRcHeight, Blend);

			SelectObject(hMemDC, hOriBmp);
			DeleteObject(hbitmap);
			DeleteDC(hMemDC);
			if (m_nFlip)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
		}
	}

	virtual void DrawImage(CDCHandle dc, CPoint point[3], CRect &rcSrc)
	{
		using namespace Gdiplus;

		if (m_nFlip)
		{
			m_pImg->RotateFlip(RotateNoneFlipX);
		}
		
		Point ptDst[3];
		for (int i = 0; i < 3; ++i)
		{
			ptDst[i].X = point[i].x;
			ptDst[i].Y = point[i].y;
		}

		Graphics graphics(dc);
		graphics.DrawImage(m_pImg, ptDst, 3, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), UnitPixel);

		if (m_nFlip)
		{
			m_pImg->RotateFlip(RotateNoneFlipX);
		}
	}

	virtual void DrawBackground(CDCHandle dc, CRect &rcDraw)
	{
		if( m_pImg )
		{
			using namespace Gdiplus;
			if (m_nFlip)
			{
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
			Graphics graphics(dc);
			INT nCx = m_pImg->GetWidth();
			INT nCy = m_pImg->GetHeight();
	
			for( INT nMemCx = 0; nMemCx + nCx <= rcDraw.Width(); nMemCx += nCx )
			{
				for( INT nMemCy = 0; nMemCy + nCy <= rcDraw.Height(); nMemCy += nCy )
				{
					graphics.DrawImage( m_pImg, rcDraw.left + nMemCx, rcDraw.top + nMemCy, nCx, nCy );
				}
			}
			if (m_nFlip)
			{
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
		}
	}

	virtual void DrawImage(Gdiplus::Graphics &graphics, CRect &rcDraw, CRect &rcSr)
	{
		if (m_pImg)
		{
			if (m_nFlip)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
			graphics.DrawImage(
				m_pImg, 
				Gdiplus::Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()), 
				rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
				Gdiplus::UnitPixel);
			if (m_nFlip)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip(RotateNoneFlipX);
			}
		}
	}

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState, int nMode)
    {
        if(nMode < SkinDrawMode_FullCtrlWindow_Stretch  && nMode > SkinDrawMode_FullCtrlWindow_Stretch)
        {
            return Draw( dc, rcDraw, dwState);
        }

        if (!m_bLoaded)
        {
            return;
        }

        if (m_pImg)
        {	
            Gdiplus::Graphics graphics(dc);
			if (m_nFlip)
			{
				m_nFlipType = Gdiplus::RotateNoneFlipX;
			}

			if (m_nFlipType)
			{
				using namespace Gdiplus;
				m_pImg->RotateFlip((RotateFlipType)m_nFlipType);
			}

            SIZE size = {0, 0};
            float fScaleMultiple = 1;
            if(m_lWantImgWidth != 0)
            {
                fScaleMultiple = (float)(m_lWantImgWidth) / (float)(m_pImg->GetWidth());
            }
            LONG lSubImageWidth = m_lSubImageWidth;
            if (m_pImg)
            {
                if (0 == lSubImageWidth)
                    size.cx = m_pImg->GetWidth() * fScaleMultiple;
                else
                    size.cx = lSubImageWidth;
                size.cy = m_pImg->GetHeight() * fScaleMultiple;
            }

            if (m_bmpCache.IsNull())
            {
                m_bmpCache.CreateFromImage(m_pImg, fScaleMultiple);
            }

            if (0 == lSubImageWidth)
			{
				if (m_nMarginLeft)
				{
                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left, 
                        rcDraw.top, 
                        m_nMarginLeft, 
                        rcDraw.Height() < size.cy ? rcDraw.Height() : size.cy, 
                        m_bmpCache, 
                        m_nLeft, 
                        m_nTop, 
                        m_nMarginLeft, 
                        rcDraw.Height(),
                        255);
				}
                else if (m_nMarginTop)
                {

                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left, 
                        rcDraw.top, 
                        rcDraw.Width(), 
                        m_nMarginTop, 
                        m_bmpCache, 
                        m_nLeft, 
                        m_nTop, 
                        rcDraw.Width(), 
                        m_nMarginTop,
                        255);
                }

				if (m_nMarginTop || m_nMarginBottom)
				{
                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left + m_nMarginLeft, 
                        rcDraw.top + m_nMarginTop, 
                        rcDraw.Width() - m_nMarginLeft - m_nMarginRight, 
                        rcDraw.Height() - m_nMarginTop - m_nMarginBottom,
                        m_bmpCache, 
                        m_nLeft + m_nMarginLeft, 
                        m_nTop + m_nMarginTop, 
                        (size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
                        (size.cy < rcDraw.Height() ? size.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
                        255);
				}
				else
				{
                    if (m_nMiddleTile)
                    {
                        //�м����ƽ��
                        int nRemainWidth = rcDraw.Width() - m_nMarginLeft - m_nMarginRight;
                        int nTileWidth = m_bmpCache.GetWidth() - m_nMarginLeft - m_nMarginRight;
                        if (nRemainWidth > 0 && nTileWidth > 0)
                        {
                            int nDestLeft = rcDraw.left + m_nMarginLeft;
                            //ƽ�̣��ظ������м�Ĳ���
                            while (nRemainWidth >= nTileWidth)
                            {
                                CBkBitmap::AlphaBlend(
                                    dc, 
                                    nDestLeft, 
                                    rcDraw.top, 
                                    nTileWidth, 
                                    rcDraw.Height(),
                                    m_bmpCache, 
                                    m_nLeft + m_nMarginLeft, 
                                    m_nTop, 
                                    nTileWidth, 
                                    rcDraw.Height(),
                                    255);

                                nRemainWidth -= nTileWidth;
                                nDestLeft += nTileWidth;
                            }
                            //��ʣһС��
                            if (nRemainWidth > 0)
                            {
                                CBkBitmap::AlphaBlend(
                                    dc, 
                                    nDestLeft, 
                                    rcDraw.top, 
                                    nRemainWidth, 
                                    rcDraw.Height(),
                                    m_bmpCache, 
                                    m_nLeft + m_nMarginLeft, 
                                    m_nTop, 
                                    nRemainWidth, 
                                    rcDraw.Height(),
                                    255);
                            }
                        }
                    }
                    else
                    {
					    
                        SIZE sizeDraw = {rcDraw.Width() - m_nMarginLeft - m_nMarginRight, rcDraw.Height() < size.cy ? rcDraw.Height() : size.cy};
                        _GetDrawSize(nMode, rcDraw, sizeDraw);

                        CBkBitmap::AlphaBlend(
                            dc, 
                            rcDraw.left + m_nMarginLeft, 
                            rcDraw.top, 
                            sizeDraw.cx, 
                            sizeDraw.cy,
                            m_bmpCache, 
                            m_nLeft + m_nMarginLeft, 
                            m_nTop, 
                            size.cx - m_nMarginLeft - m_nMarginRight, 
                            size.cy,
                            255);
                    }
				}				               

				if (m_nMarginRight)
				{
                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.right - m_nMarginRight, 
                        rcDraw.top, 
                        m_nMarginRight, 
                        rcDraw.Height() < size.cy ? rcDraw.Height() : size.cy,
                        m_bmpCache, 
                        size.cx - m_nMarginRight, 
                        m_nTop, 
                        m_nMarginRight, 
                        rcDraw.Height(),
                        255);
				}
                else if (m_nMarginBottom)
                {
                    CBkBitmap::AlphaBlend(
                        dc, 
                        rcDraw.left, 
                        rcDraw.bottom - m_nMarginBottom, 
                        rcDraw.Width(), 
                        m_nMarginBottom,
                        m_bmpCache, 
                        m_nLeft, 
                        size.cy - m_nMarginBottom, 
                        rcDraw.Width(), 
                        m_nMarginBottom,
                        255);
                }
			}
            else
			{

				int nBlock = m_bmpCache.GetWidth() / lSubImageWidth;
				if (nBlock != 0)
					dwState = dwState % nBlock;

                SIZE sizeDraw = size;
                _GetDrawSize(nMode, rcDraw, sizeDraw);

                CBkBitmap::AlphaBlend(
                    dc, 
                    rcDraw.left, 
                    rcDraw.top, 
                    sizeDraw.cx, 
                    sizeDraw.cy,
                    m_bmpCache, 
                    lSubImageWidth * dwState, 
                    0, 
                    size.cx, 
                    size.cy,
                    255);
			}

			if (m_nFlipType)
			{
				using namespace Gdiplus;
				int nRecovery = 0;
				switch(m_nFlipType)
				{
				case Rotate90FlipNone:
					nRecovery = Rotate270FlipNone;	break;
				case Rotate180FlipNone:
					nRecovery = Rotate180FlipNone;	break;
				case Rotate270FlipNone:
					nRecovery = Rotate90FlipNone;	break;

				case RotateNoneFlipX:
					nRecovery = RotateNoneFlipX;	break;
				case Rotate90FlipX:
					nRecovery = Rotate270FlipX;		break;
				case Rotate180FlipX:
					nRecovery = Rotate180FlipX;		break;
				case Rotate270FlipX:
					nRecovery = Rotate90FlipX;		break;
				}

				if (nRecovery != 0)
					m_pImg->RotateFlip((RotateFlipType)nRecovery);
			}
        }
    }

    virtual void Stretch(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha = 255)
    {
        if (!m_bLoaded || !m_pImg)
            return;

        Gdiplus::Image* pImg = NULL;
        LONG lSubImageWidth = m_lSubImageWidth;
        float fScaleRate = bDpiAware ? BKWinDpiAware::GetInstance().GetDpiScaleRate() : 1.0;

#ifdef __BK_STRETCH_USE_HIGHQUALITY__
		if (fScaleRate != 1.0) 
		{
			//��Բ���1.0 dpi�����ø�����������Ⱦ
			StretchByHighquality(dc, rcDraw, dwState, bDpiAware, nAlpha);
			return;
		}	
#endif

        if (bDpiAware && m_pDpiImg && fScaleRate != 1.0 && m_dpiImageRate == fScaleRate)
        {
			// �ٷ����� fScaleRate
            pImg = m_pDpiImg;
            lSubImageWidth *= m_dpiImageRate;
            fScaleRate = 1.0;
        }
        else
        {
            pImg = m_pImg;
        }
        if (m_bmpCache.IsNull())
            m_bmpCache.CreateFromImage(pImg, fScaleRate, lSubImageWidth);

        int iImgTotalSlice          = (lSubImageWidth == 0) ? 1 : pImg->GetWidth() / lSubImageWidth;
        if (iImgTotalSlice == 0)
            iImgTotalSlice = 1;
        int iImgCurSlice            = (dwState == -1) ? 0 : (dwState % iImgTotalSlice);
        int iBmpCacheSubWidth       = m_bmpCache.GetWidth() / iImgTotalSlice;
        int iBmpCacheMarginLeft     = bDpiAware ? m_nMarginLeft * fScaleRate : m_nMarginLeft;
        int iBmpCacheMarginRight    = bDpiAware ? m_nMarginRight * fScaleRate : m_nMarginRight;
        int iBmpCacheMarginTop      = bDpiAware ? m_nMarginTop * fScaleRate : m_nMarginTop;
        int iBmpCacheMarginBottom   = bDpiAware ? m_nMarginBottom * fScaleRate : m_nMarginBottom;
        int iBmpCacheLeft           = bDpiAware ? m_nLeft * fScaleRate : m_nLeft;
        int iBmpCacheTop            = bDpiAware ? m_nTop * fScaleRate : m_nTop;

        CRect rcBmpCache(iBmpCacheSubWidth * iImgCurSlice + iBmpCacheLeft, iBmpCacheTop, iBmpCacheSubWidth * (iImgCurSlice + 1), m_bmpCache.GetHeight());

        //left top
        if (iBmpCacheMarginLeft && iBmpCacheMarginTop)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.left, rcDraw.top, iBmpCacheMarginLeft, iBmpCacheMarginTop, 
                m_bmpCache, rcBmpCache.left, rcBmpCache.top, iBmpCacheMarginLeft, iBmpCacheMarginTop, nAlpha);
        }
        //left bottom
        if (iBmpCacheMarginLeft && iBmpCacheMarginBottom)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.left, rcDraw.bottom - iBmpCacheMarginBottom, iBmpCacheMarginLeft, iBmpCacheMarginBottom, 
                m_bmpCache, rcBmpCache.left, rcBmpCache.bottom - iBmpCacheMarginBottom, iBmpCacheMarginLeft, iBmpCacheMarginBottom, nAlpha);
        }
        //left
        if (iBmpCacheMarginLeft)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.left, rcDraw.top + iBmpCacheMarginTop, iBmpCacheMarginLeft, rcDraw.Height() - iBmpCacheMarginTop - iBmpCacheMarginBottom, 
                m_bmpCache, rcBmpCache.left, rcBmpCache.top + iBmpCacheMarginTop, iBmpCacheMarginLeft, rcBmpCache.Height() - iBmpCacheMarginTop - iBmpCacheMarginBottom, nAlpha);
        }
        //right top
        if (iBmpCacheMarginRight && iBmpCacheMarginTop)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.right - iBmpCacheMarginRight, rcDraw.top, iBmpCacheMarginRight, iBmpCacheMarginTop, 
                m_bmpCache, rcBmpCache.right - iBmpCacheMarginRight, rcBmpCache.top, iBmpCacheMarginRight, iBmpCacheMarginTop, nAlpha);
        }
        //right bottom
        if (iBmpCacheMarginRight && iBmpCacheMarginBottom)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.right - iBmpCacheMarginRight, rcDraw.bottom - iBmpCacheMarginBottom, iBmpCacheMarginRight, iBmpCacheMarginBottom, 
                m_bmpCache, rcBmpCache.right - iBmpCacheMarginRight, rcBmpCache.bottom - iBmpCacheMarginBottom, iBmpCacheMarginRight, iBmpCacheMarginBottom, nAlpha);
        }
        //right
        if (iBmpCacheMarginRight)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.right - iBmpCacheMarginRight, rcDraw.top + iBmpCacheMarginTop, iBmpCacheMarginRight, rcDraw.Height() - iBmpCacheMarginTop - iBmpCacheMarginBottom, 
                m_bmpCache, rcBmpCache.right - iBmpCacheMarginRight, rcBmpCache.top + iBmpCacheMarginTop, iBmpCacheMarginRight, rcBmpCache.Height() - iBmpCacheMarginTop - iBmpCacheMarginBottom, nAlpha);
        }
        //top
        if (iBmpCacheMarginTop)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.left + iBmpCacheMarginLeft, rcDraw.top, rcDraw.Width() - iBmpCacheMarginLeft - iBmpCacheMarginRight, iBmpCacheMarginTop, 
                m_bmpCache, rcBmpCache.left + iBmpCacheMarginLeft, rcBmpCache.top, rcBmpCache.Width() - iBmpCacheMarginLeft - iBmpCacheMarginRight, iBmpCacheMarginTop, nAlpha);
        }
        //bottom
        if (iBmpCacheMarginBottom)
        {
            CBkBitmap::AlphaBlend(
                dc, rcDraw.left + iBmpCacheMarginLeft, rcDraw.bottom - iBmpCacheMarginBottom, rcDraw.Width() - iBmpCacheMarginLeft - iBmpCacheMarginRight, iBmpCacheMarginBottom, 
                m_bmpCache, rcBmpCache.left + iBmpCacheMarginLeft, rcBmpCache.bottom - iBmpCacheMarginBottom, rcBmpCache.Width() - iBmpCacheMarginLeft - iBmpCacheMarginRight, iBmpCacheMarginBottom, nAlpha);
        }
        //center
        CBkBitmap::AlphaBlend(
            dc, rcDraw.left + iBmpCacheMarginLeft, rcDraw.top + iBmpCacheMarginTop, rcDraw.Width() - iBmpCacheMarginLeft - iBmpCacheMarginRight, rcDraw.Height() - iBmpCacheMarginTop - iBmpCacheMarginBottom, 
            m_bmpCache, rcBmpCache.left + iBmpCacheMarginLeft, rcBmpCache.top + iBmpCacheMarginTop, rcBmpCache.Width() - iBmpCacheMarginLeft - iBmpCacheMarginRight, rcBmpCache.Height() - iBmpCacheMarginTop - iBmpCacheMarginBottom, nAlpha);
    }

	void StretchByHighquality(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha = 255)
	{
		if (!m_bLoaded || !m_pImg || nAlpha <= 0)
			return;

		Gdiplus::Image* pImg = NULL;
		float fScaleRate = bDpiAware ? BKWinDpiAware::GetInstance().GetDpiScaleRate() : 1.0;
		float fImgRate = 1.0;
		if (bDpiAware && m_pDpiImg && fScaleRate != 1.0)
		{
			pImg = m_pDpiImg;
			fImgRate = m_dpiImageRate;
		}
		else
		{
			pImg = m_pImg;
		}
		
		LONG lSubImageWidth = m_lSubImageWidth * fImgRate;
		int iImgTotalSlice          = (lSubImageWidth == 0) ? 1 : pImg->GetWidth() / lSubImageWidth;
		if (iImgTotalSlice == 0)
			iImgTotalSlice = 1;
		int iImgCurSlice            = (dwState < 0) ? 0 : (dwState % iImgTotalSlice);

		int iImgSubWidth      = pImg->GetWidth() / iImgTotalSlice;
		int iImgMarginLeft     = m_nMarginLeft * fImgRate;
		int iImgMarginRight    =  m_nMarginRight * fImgRate;
		int iImgMarginTop      = m_nMarginTop * fImgRate;
		int iImgMarginBottom   = m_nMarginBottom * fImgRate;
		int iImgLeft           = m_nLeft * fImgRate;
		int iImgTop            = m_nTop * fImgRate;

		int iDestMarginLeft     = m_nMarginLeft * fScaleRate;
		int iDestMarginRight    =  m_nMarginRight * fScaleRate;
		int iDestMarginTop      = m_nMarginTop * fScaleRate;
		int iDestMarginBottom   = m_nMarginBottom * fScaleRate;

		Gdiplus::Rect rcSrcImage(iImgSubWidth * iImgCurSlice + iImgLeft, iImgTop, iImgSubWidth, pImg->GetHeight() - iImgTop);
		Gdiplus::Image* pImgBitmapToDraw = pImg;
		if (iImgTotalSlice > 1)
		{
			 // ���Ƕ�̬ͼ��ȡsub������¸�sub��һ�����أ��ر����ڰ�ťͼƬ�У����ֱ��ߣ�����clip sub image ��������������
			// gdi+ ��������⣬���������������и����ķ���������
			Gdiplus::Bitmap* pSubImage = GetSubImageFrom(*pImg, rcSrcImage);
			if (pSubImage != NULL) 
			{
				pImgBitmapToDraw = pSubImage;
				rcSrcImage.X = 0;
				rcSrcImage.Y = 0;
				rcSrcImage.Width = pSubImage->GetWidth();
				rcSrcImage.Height = pSubImage->GetHeight();
			}
		}


		Gdiplus::Graphics graphics(dc);
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);

		const float fAlpha = nAlpha / 255.0f;

		Gdiplus::ColorMatrix colorMartrix={
			1,0,0,0,0,
			0,1,0,0,0,
			0,0,1,0,0,
			0,0,0, fAlpha, 0,
			0,0,0,0,1};
		Gdiplus::ImageAttributes imageAttr;
		imageAttr.SetColorMatrix(&colorMartrix);

		Gdiplus::ImageAttributes* pImageAttributes = (fAlpha < 1.0f ? &imageAttr : NULL);

		const bool isHOver = iDestMarginLeft + iDestMarginRight >= rcDraw.Width();
		const bool isVOver = iDestMarginTop + iDestMarginBottom >= rcDraw.Height();
		if (isHOver && isVOver) 
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()),
				Gdiplus::Rect(rcSrcImage.X, rcSrcImage.Y, rcSrcImage.Width, rcSrcImage.Height), 
				pImageAttributes);
			return;
		}

		if (isHOver) 
		{
			if (iImgMarginTop) 
			{
				HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
					Gdiplus::RectF(rcDraw.left, rcDraw.top, rcDraw.Width(), iDestMarginTop),
					Gdiplus::Rect(rcSrcImage.X, rcSrcImage.Y, rcSrcImage.Width, iImgMarginTop), 
					pImageAttributes);
			}

			if (iDestMarginBottom) 
			{
				HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
					Gdiplus::RectF(rcDraw.left, rcDraw.bottom - iDestMarginBottom, rcDraw.Width(), iDestMarginBottom),
					Gdiplus::Rect(rcSrcImage.X, rcSrcImage.GetBottom() - iImgMarginBottom, rcSrcImage.Width, iImgMarginBottom),
					pImageAttributes);
			}

			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left, rcDraw.top + iDestMarginTop, rcDraw.Width(), rcDraw.Height() - iDestMarginBottom - iDestMarginTop),
				Gdiplus::Rect(rcSrcImage.X, rcSrcImage.Y + iImgMarginTop, rcSrcImage.Width, rcSrcImage.Height - iImgMarginBottom - iImgMarginTop), 
				pImageAttributes);
			return;
		}

		if (isVOver) 
		{
			if (iImgMarginLeft) 
			{
				HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
					Gdiplus::RectF(rcDraw.left, rcDraw.top, iDestMarginLeft, rcDraw.Height()),
					Gdiplus::Rect(rcSrcImage.X, rcSrcImage.Y, iImgMarginLeft, rcSrcImage.Height), 
					pImageAttributes);
			}

			if (iImgMarginRight) 
			{
				HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
					Gdiplus::RectF(rcDraw.right - iDestMarginRight, rcDraw.top, iDestMarginRight, rcDraw.Height()),
					Gdiplus::Rect(rcSrcImage.GetRight() - iImgMarginRight, rcSrcImage.Y, iImgMarginRight, rcSrcImage.Height), 
					pImageAttributes);
			}

			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left + iDestMarginLeft, rcDraw.top, rcDraw.Width() - iDestMarginLeft - iDestMarginRight, rcDraw.Height()),
				Gdiplus::Rect(rcSrcImage.X + iImgMarginLeft, rcSrcImage.Y, rcSrcImage.Width - iImgMarginLeft - iImgMarginRight, rcSrcImage.Height), 
				pImageAttributes);
			return;
		}

		//left top
		if (iImgMarginLeft && iImgMarginTop)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left, rcDraw.top, iDestMarginLeft, iDestMarginTop),
				Gdiplus::Rect(rcSrcImage.X, rcSrcImage.Y, iImgMarginLeft, iImgMarginTop),
				pImageAttributes);
		}

		//left bottom
		if (iImgMarginLeft && iImgMarginBottom)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left, rcDraw.bottom - iDestMarginBottom, iDestMarginLeft, iDestMarginBottom), 
				Gdiplus::Rect(rcSrcImage.X, rcSrcImage.GetBottom() - iImgMarginBottom, iImgMarginLeft, iImgMarginBottom),
				pImageAttributes);
		}
		//left
		if (iImgMarginLeft)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left, rcDraw.top + iDestMarginTop, iDestMarginLeft, rcDraw.Height() - iDestMarginTop - iDestMarginBottom),
				Gdiplus::Rect(rcSrcImage.X, rcSrcImage.Y + iImgMarginTop, iImgMarginLeft, rcSrcImage.Height - iImgMarginTop - iImgMarginBottom),
				pImageAttributes);
		}
		//right top
		if (iImgMarginRight && iImgMarginTop)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.right - iDestMarginRight, rcDraw.top, iDestMarginRight, iDestMarginTop), 
				Gdiplus::Rect(rcSrcImage.GetRight() - iImgMarginRight, rcSrcImage.Y, iImgMarginRight, iImgMarginTop),
				pImageAttributes);
		}
		//right bottom
		if (iImgMarginRight && iImgMarginBottom)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.right - iDestMarginRight, rcDraw.bottom - iDestMarginBottom, iDestMarginRight, iDestMarginBottom), 
				Gdiplus::Rect(rcSrcImage.GetRight() - iImgMarginRight, rcSrcImage.GetBottom() - iImgMarginBottom, iImgMarginRight, iImgMarginBottom),
				pImageAttributes);
		}
		//right
		if (iImgMarginRight)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.right - iDestMarginRight, rcDraw.top + iDestMarginTop, iDestMarginRight, rcDraw.Height() - iDestMarginTop - iDestMarginBottom), 
				Gdiplus::Rect(rcSrcImage.GetRight() - iImgMarginRight, rcSrcImage.Y + iImgMarginTop, iImgMarginRight, rcSrcImage.Height - iImgMarginTop - iImgMarginBottom),
				pImageAttributes);
		}
		//top
		if (iImgMarginTop)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left + iDestMarginLeft, rcDraw.top, rcDraw.Width() - iDestMarginLeft - iDestMarginRight, iDestMarginTop), 
				Gdiplus::Rect(rcSrcImage.X + iImgMarginLeft, rcSrcImage.Y, rcSrcImage.Width - iImgMarginLeft - iImgMarginRight, iImgMarginTop),
				pImageAttributes);
		}
		//bottom
		if (iImgMarginBottom)
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left + iDestMarginLeft, rcDraw.bottom - iDestMarginBottom, rcDraw.Width() - iDestMarginLeft - iDestMarginRight, iDestMarginBottom),
				Gdiplus::Rect(rcSrcImage.X + iImgMarginLeft, rcSrcImage.GetBottom() - iImgMarginBottom, rcSrcImage.Width - iImgMarginLeft - iImgMarginRight, iImgMarginBottom),
				pImageAttributes);
		}

		//center
		{
			HighQualityDrawImage(graphics, *pImgBitmapToDraw, 
				Gdiplus::RectF(rcDraw.left + iDestMarginLeft, rcDraw.top + iDestMarginTop, rcDraw.Width() - iDestMarginLeft - iDestMarginRight, rcDraw.Height() - iDestMarginTop - iDestMarginBottom),
				Gdiplus::Rect(rcSrcImage.X + iImgMarginLeft, rcSrcImage.Y + iImgMarginTop, rcSrcImage.Width - iImgMarginLeft - iImgMarginRight, rcSrcImage.Height - iImgMarginTop - iImgMarginBottom),
				pImageAttributes);
		}
	}

	Gdiplus::Bitmap* GetSubImageFrom(Gdiplus::Image& srcImage, const Gdiplus::Rect& rectSrc) 
	{
		CStringA strKey;
		strKey.Format("%I64d_%d_%d_%d_%d", (__int64)(&(srcImage)), rectSrc.X, rectSrc.Y, rectSrc.Width, rectSrc.Height);

		Gdiplus::Bitmap* pImage = NULL;
		std::map<CStringA, Gdiplus::Bitmap*>::iterator iter = m_mapHighQualityImage.find(strKey);
		if (iter == m_mapHighQualityImage.end()) 
		{
			pImage = CloneFromImage(srcImage, rectSrc);
			m_mapHighQualityImage[strKey] = pImage;
		}
		else 
		{
			pImage = iter->second;
		}

		return pImage;
	}

	void HighQualityDrawImage(Gdiplus::Graphics& graphics, Gdiplus::Image& srcImage, const Gdiplus::RectF& rectDest, const Gdiplus::Rect& rectSrc, Gdiplus::ImageAttributes* pImageAttributes = NULL) 
	{
		graphics.DrawImage(&srcImage, rectDest, rectSrc.X, rectSrc.Y, rectSrc.Width, rectSrc.Height, Gdiplus::UnitPixel, pImageAttributes);
	}

	Gdiplus::Bitmap* CloneFromImage(Gdiplus::Image& srcImage, const Gdiplus::Rect& rectSrc) 
	{
		Gdiplus::Bitmap* pImage = new Gdiplus::Bitmap(rectSrc.Width, rectSrc.Height, PixelFormat32bppARGB);
		if (pImage == NULL) 
		{
			return NULL;
		}
		Gdiplus::Graphics grahics(pImage);
		grahics.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

	
		grahics.DrawImage(&srcImage, Gdiplus::RectF(0, 0, rectSrc.Width, rectSrc.Height),
			rectSrc.X, rectSrc.Y, rectSrc.Width, rectSrc.Height, Gdiplus::UnitPixel);

		return pImage;
	}

    virtual void DrawNineCels(CDCHandle dc, int nState, const CRect &rcDraw, const CRect &rcCel, BYTE byAlpha)
    {
        if (!m_bLoaded)
        {
            return;
        }

        if (m_pImg)
        {	
            Gdiplus::Graphics graphics(dc);
            if (m_nFlip)
            {
                m_nFlipType = Gdiplus::RotateNoneFlipX;
            }

            if (m_nFlipType)
            {
                using namespace Gdiplus;
                m_pImg->RotateFlip((RotateFlipType)m_nFlipType);
            }

            SIZE size = {0, 0};
            float fScaleMultiple = 1;
            if(m_lWantImgWidth != 0)
            {
                fScaleMultiple = (float)(m_lWantImgWidth) / (float)(m_pImg->GetWidth());
            }

            LONG lSubImageWidth = m_lSubImageWidth;
            if (m_pImg)
            {
                if (0 == lSubImageWidth)
                    size.cx = m_pImg->GetWidth() * fScaleMultiple;
                else
                    size.cx = lSubImageWidth;
                size.cy = m_pImg->GetHeight() * fScaleMultiple;
            }

            if (m_bmpCache.IsNull())
            {
                m_bmpCache.CreateFromImage(m_pImg, fScaleMultiple);
            }

            //
            int nBlock = m_bmpCache.GetWidth() / lSubImageWidth;
            if (nBlock != 0)
            {
                nState = nState % nBlock;
            }

            SIZE sizeDraw = size;
            _GetDrawSize(SkinDrawMode_FullCtrlWindow_Stretch, rcDraw, sizeDraw);

            CRect rcDrawNew(rcDraw.TopLeft(), sizeDraw);
            CRect rcSrcNew(lSubImageWidth * nState, 0, lSubImageWidth * nState + size.cx, size.cy);
            m_bmpCache.AlphaBlendNineCels(dc, rcDrawNew, m_bmpCache, rcSrcNew, rcCel, byAlpha);

        }
    }

    void _GetDrawSize(int nMode, const CRect &rcDraw, SIZE &sizeDraw)
    {
        switch(nMode)
        {
        case SkinDrawMode_FullCtrlWindow_Stretch:
            sizeDraw.cx = rcDraw.Width();
            sizeDraw.cy = rcDraw.Height();
            break;
        default:
            break;
        }
    }


    /**
    *   ���м�ΪԲ�ģ���ת��ͼ��
    *	�Ȳ�������麯��������������Ҳ֧�����ٿ���
    **/
    void DrawImageWithRotate(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nRotateAngle)
    {
        if (m_pImg)
        {
            using namespace Gdiplus;
            Graphics graphics(dc);
            int nHalfWidth  = rcDraw.Width() / 2.0;
            int nHalfHeight = rcDraw.Height() / 2.0;
            float fRotateCenterX = rcDraw.left + nHalfWidth;
            float RotateCenterY = rcDraw.top +  nHalfHeight;

            graphics.TranslateTransform(fRotateCenterX, RotateCenterY);
            graphics.RotateTransform(nRotateAngle);      
            
            graphics.DrawImage(
                m_pImg, 
                Gdiplus::Rect(0 - nHalfWidth, 0 - nHalfHeight, rcDraw.Width(), rcDraw.Height()), 
                rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
                Gdiplus::UnitPixel);

            graphics.ResetTransform();
        }
    }

	void DrawImageWithRotateHiQuality(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nRotateAngle)
	{
		if (m_pImg)
		{
			using namespace Gdiplus;
			Graphics graphics(dc);

			float fHalfWidth  = rcDraw.Width() / 2.0;
			float fHalfHeight = rcDraw.Height() / 2.0;
			float fRotateCenterX = rcDraw.left + fHalfWidth;
			float RotateCenterY = rcDraw.top +  fHalfHeight;

			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			graphics.TranslateTransform(fRotateCenterX, RotateCenterY);
			graphics.RotateTransform(nRotateAngle);

			graphics.DrawImage(
				m_pImg, 
				Gdiplus::Rect(0.f - fHalfWidth, 0.f - fHalfHeight, (float)rcDraw.Width(), (float)rcDraw.Height()), 
				rcSr.left, 
				rcSr.top, 
				rcSr.Width(), 
				rcSr.Height(), 
				Gdiplus::UnitPixel);

			graphics.ResetTransform();
		}
	}

	void DrawImageWithRotateScale(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nRotateAngle, float fScale)
	{
		if (m_pImg)
		{
			using namespace Gdiplus;
			Graphics graphics(dc);
			int nHalfWidth  = rcDraw.Width() / 2.0;
			int nHalfHeight = rcDraw.Height() / 2.0;
			float fRotateCenterX = rcDraw.left + nHalfWidth;
			float RotateCenterY = rcDraw.top +  nHalfHeight;

			graphics.TranslateTransform(fRotateCenterX, RotateCenterY);
			graphics.RotateTransform(nRotateAngle);
			graphics.ScaleTransform(fScale, fScale);

			graphics.DrawImage(
				m_pImg, 
				Gdiplus::Rect(0 - nHalfWidth, 0 - nHalfHeight, rcDraw.Width(), rcDraw.Height()), 
				rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
				Gdiplus::UnitPixel);

			graphics.ResetTransform();
		}
	}

	void DrawFlip(CDCHandle dc, CRect rcDst)
	{
		if (m_pImg == NULL)
			return;

		using namespace Gdiplus;
		Graphics graphics(dc);

		m_pImg->RotateFlip(RotateNoneFlipX);
		graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDst.left, rcDst.top, rcDst.Width(), rcDst.Height()));
		m_pImg->RotateFlip(RotateNoneFlipX);

		//Gdiplus::Image *pImage = m_pImg->Clone();
		//if (pImage)
		//{
		//	
		//	delete pImage;
		//}
	}

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};
        if (m_pImg)
        {
            if (0 == m_lSubImageWidth)
                ret.cx = m_pImg->GetWidth();
            else
                ret.cx = m_lSubImageWidth;
            ret.cy = m_pImg->GetHeight();
        }

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return 0 == m_lSubImageWidth;
    }

    Gdiplus::Image* GetImage()
    {
        return m_pImg;
    }

protected:
    Gdiplus::Image* m_pImg;
    Gdiplus::Image* m_pDpiImg;
	double m_dpiImageRate;
    UINT m_uResID;
	LONG m_lSubImageWidth;
	int	 m_nLeft;
	int	 m_nTop;
	int	 m_nMarginLeft;
	int	 m_nMarginRight;
    int  m_nMarginTop;
    int  m_nMarginBottom;
    int  m_nMiddleTile;
	int  m_nFlip;
	int  m_nFlipType;
	CBkBitmap m_bmpGrayCache;
	BOOL m_bGrayCache;
    CBkBitmap m_bmpCache;
    LONG m_lWantImgWidth;


	std::map<CStringA, Gdiplus::Bitmap*> m_mapHighQualityImage;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("src", m_uResID, TRUE)
		BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
		BKWIN_INT_ATTRIBUTE("left", m_nLeft, TRUE)
		BKWIN_INT_ATTRIBUTE("top", m_nTop, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_left", m_nMarginLeft, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_right", m_nMarginRight, TRUE)
        BKWIN_INT_ATTRIBUTE("margin_top", m_nMarginTop, TRUE)           //����margin_top��margin_bottom������png�Ĵ�ֱ����
        BKWIN_INT_ATTRIBUTE("margin_bottom", m_nMarginBottom, TRUE) 
		BKWIN_INT_ATTRIBUTE("flip", m_nFlip, TRUE)
		BKWIN_INT_ATTRIBUTE("fliptype", m_nFlipType, TRUE)
        BKWIN_INT_ATTRIBUTE("middle_tile", m_nMiddleTile, TRUE)     //ͼƬ���м䲿�֣���ȥleft��right������ƽ��
        BKWIN_INT_ATTRIBUTE("wantimgwidth", m_lWantImgWidth, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

// 2011.6.20 by zyb
class CBkJpgSkin
	: public CBkSkinBase
{
	BKOBJ_DECLARE_CLASS_NAME(CBkJpgSkin, "jpg")

public:
	CBkJpgSkin()
		: m_uResID(0)
		, m_lSubImageWidth(0)
		, m_pImg(NULL)
		, m_nLeft(0)
		, m_nTop(0)
		, m_nMarginLeft(0)
		, m_nMarginRight(0)
	{
	}

	~CBkJpgSkin()
	{
	}

	virtual BOOL Load(TiXmlElement* pXmlElem)
	{
		__super::Load(pXmlElem);

		if (0 == m_uResID)
			return TRUE;

		return TRUE;
	}

	BOOL LoadImg()
	{
		if (m_bLoaded)
			return TRUE;
		m_pImg = BkJpgPool::Get(m_uResID);
		m_bLoaded = (m_pImg != NULL);
		return m_bLoaded;
	}

	virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
	{
		if (!m_bLoaded)
		{
			return;
		}

		if (m_pImg)
		{
			Gdiplus::Graphics graphics(dc);

			SIZE size = {0, 0};
			if (m_pImg)
			{
				if (0 == m_lSubImageWidth)
					size.cx = m_pImg->GetWidth();
				else
					size.cx = m_lSubImageWidth;
				size.cy = m_pImg->GetHeight();
			}

			if (0 == m_lSubImageWidth)
			{
				if (m_nMarginLeft)
				{
					graphics.DrawImage(
						m_pImg, 
						Gdiplus::Rect(rcDraw.left, rcDraw.top, m_nMarginLeft, rcDraw.Height()), 
						m_nLeft, 
						m_nTop, 
						m_nMarginLeft, 
						rcDraw.Height(), 
						Gdiplus::UnitPixel);
				}

				graphics.DrawImage(
					m_pImg, 
					Gdiplus::Rect(rcDraw.left + m_nMarginLeft, rcDraw.top, rcDraw.Width() - m_nMarginLeft - m_nMarginRight, rcDraw.Height()), 
					m_nLeft + m_nMarginLeft, 
					m_nTop, 
					(size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
					rcDraw.Height(), 
					Gdiplus::UnitPixel);

				if (m_nMarginRight)
				{
					graphics.DrawImage(
						m_pImg, 
						Gdiplus::Rect(rcDraw.right - m_nMarginRight, rcDraw.top, m_nMarginRight, rcDraw.Height()), 
						size.cx - m_nMarginRight, 
						m_nTop, 
						m_nMarginRight, 
						rcDraw.Height(), 
						Gdiplus::UnitPixel);
				}
			}
			else
				graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDraw.left, rcDraw.top, size.cx, size.cy), m_lSubImageWidth * dwState, 0, size.cx, size.cy, Gdiplus::UnitPixel);
		}
	}

    virtual void Stretch(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bDpiAware, int nAlpha = 255)
    {
        if (!m_bLoaded)
        {
            return;
        }

        if (m_pImg)
        {
            Gdiplus::Graphics graphics(dc);

            SIZE size = {0, 0};
            if (m_pImg)
            {
                if (0 == m_lSubImageWidth)
                    size.cx = m_pImg->GetWidth();
                else
                    size.cx = m_lSubImageWidth;
                size.cy = m_pImg->GetHeight();
            }

            if (0 == m_lSubImageWidth)
            {
                if (m_nMarginLeft)
                {
                    graphics.DrawImage(
                        m_pImg, 
                        Gdiplus::Rect(rcDraw.left, rcDraw.top, m_nMarginLeft, rcDraw.Height()), 
                        m_nLeft, 
                        m_nTop, 
                        m_nMarginLeft, 
                        rcDraw.Height(), 
                        Gdiplus::UnitPixel);
                }

                graphics.DrawImage(
                    m_pImg, 
                    Gdiplus::Rect(rcDraw.left + m_nMarginLeft, rcDraw.top, rcDraw.Width() - m_nMarginLeft - m_nMarginRight, rcDraw.Height()), 
                    m_nLeft + m_nMarginLeft, 
                    m_nTop, 
                    (size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
                    rcDraw.Height(), 
                    Gdiplus::UnitPixel);

                if (m_nMarginRight)
                {
                    graphics.DrawImage(
                        m_pImg, 
                        Gdiplus::Rect(rcDraw.right - m_nMarginRight, rcDraw.top, m_nMarginRight, rcDraw.Height()), 
                        size.cx - m_nMarginRight, 
                        m_nTop, 
                        m_nMarginRight, 
                        rcDraw.Height(), 
                        Gdiplus::UnitPixel);
                }
            }
            else
                graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDraw.left, rcDraw.top, size.cx, size.cy), m_lSubImageWidth * dwState, 0, size.cx, size.cy, Gdiplus::UnitPixel);
        }
    }

	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
		if (m_pImg)
		{
			Gdiplus::Graphics graphics(dc);
			graphics.DrawImage(
				m_pImg, 
				Gdiplus::Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()), 
				rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
				Gdiplus::UnitPixel);
		}
	}

	virtual void DrawImageGray(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
		if (m_pImg)
		{
			Gdiplus::Graphics graphics(dc);
			Gdiplus::ColorMatrix cm= {
				0.3f, 0.3f, 0.3f, 0, 0,
				0.59f,0.59f,0.59f,0, 0,
				0.11f,0.11f,0.11f,0, 0,
				0,    0,    0,    1, 0,
				0,    0,    0,    0, 1};

				Gdiplus::ImageAttributes imAtt;
				imAtt.SetColorMatrix(&cm);

				graphics.DrawImage(
					m_pImg,
					Gdiplus::Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()), 
					rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
					Gdiplus::UnitPixel,
					&imAtt);
		}
	}

	virtual SIZE GetSkinSize()
	{
		SIZE ret = {0, 0};
		if (m_pImg)
		{
			if (0 == m_lSubImageWidth)
				ret.cx = m_pImg->GetWidth();
			else
				ret.cx = m_lSubImageWidth;
			ret.cy = m_pImg->GetHeight();
		}

		return ret;
	}

	virtual BOOL IgnoreState()
	{
		return 0 == m_lSubImageWidth;
	}

protected:
	Gdiplus::Image* m_pImg;
	UINT m_uResID;
	LONG m_lSubImageWidth;
	int	 m_nLeft;
	int	 m_nTop;
	int	 m_nMarginLeft;
	int	 m_nMarginRight;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("src", m_uResID, TRUE)
		BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
		BKWIN_INT_ATTRIBUTE("left", m_nLeft, TRUE)
		BKWIN_INT_ATTRIBUTE("top", m_nTop, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_left", m_nMarginLeft, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_right", m_nMarginRight, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkNormalSkin
	: public CBkSkinBase
{
public:
	CBkNormalSkin()
		: m_uResID(0)
		, m_lSubImageWidth(0)
		, m_nLeft(0)
		, m_nTop(0)
		, m_nMarginLeft(0)
		, m_nMarginRight(0)
		, m_nMarginTop(0)
		, m_nMarginBottom(0)
		, m_nFlip(0)
		, m_bGrayCache(FALSE)
		, m_nMiddleTile(0)
		, m_nAlpha(255)
	{
	}

	~CBkNormalSkin()
	{
	}

	virtual BOOL Load(TiXmlElement* pXmlElem)
	{
		__super::Load(pXmlElem);

		if (0 == m_uResID)
			return TRUE;

		//m_pImg = BkPngPool::Get(m_uResID);

		return TRUE;
	}

	virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
	{
		if (!m_bLoaded)
			return;

		SIZE size = {0, 0};
		if (0 == m_lSubImageWidth)
			size.cx = m_bmpCache.GetWidth();
		else
			size.cx = m_lSubImageWidth;
		size.cy = m_bmpCache.GetHeight();

		if (0 == m_lSubImageWidth)
		{
			if (m_nMarginLeft)
			{
				CBkBitmap::AlphaBlend(
					dc, 
					rcDraw.left, 
					rcDraw.top, 
					m_nMarginLeft, 
					rcDraw.Height(), 
					m_bmpCache, 
					m_nLeft, 
					m_nTop, 
					m_nMarginLeft, 
					rcDraw.Height(),
					m_nAlpha);
			}
			else if (m_nMarginTop)
			{
				CBkBitmap::AlphaBlend(
					dc, 
					rcDraw.left, 
					rcDraw.top, 
					rcDraw.Width(), 
					m_nMarginTop, 
					m_bmpCache, 
					m_nLeft, 
					m_nTop, 
					rcDraw.Width(), 
					m_nMarginTop,
					m_nAlpha);
			}

			if (m_nMarginTop || m_nMarginBottom)
			{
				CBkBitmap::AlphaBlend(
					dc, 
					rcDraw.left + m_nMarginLeft, 
					rcDraw.top + m_nMarginTop, 
					rcDraw.Width() - m_nMarginLeft - m_nMarginRight, 
					rcDraw.Height() - m_nMarginTop - m_nMarginBottom,
					m_bmpCache, 
					m_nLeft + m_nMarginLeft, 
					m_nTop + m_nMarginTop, 
					(size.cx < rcDraw.Width() ? size.cx : rcDraw.Width()) - m_nMarginLeft - m_nMarginRight, 
					(size.cy < rcDraw.Height() ? size.cy : rcDraw.Height()) - m_nMarginTop - m_nMarginBottom, 
					m_nAlpha);
			}
			else
			{
				if (m_nMiddleTile)
				{
					//�м����ƽ��
					int nRemainWidth = rcDraw.Width() - m_nMarginLeft - m_nMarginRight;
					int nTileWidth = m_bmpCache.GetWidth() - m_nMarginLeft - m_nMarginRight;
					if (nRemainWidth > 0 && nTileWidth > 0)
					{
						int nDestLeft = rcDraw.left + m_nMarginLeft;
						//ƽ�̣��ظ������м�Ĳ���
						while (nRemainWidth >= nTileWidth)
						{
							CBkBitmap::AlphaBlend(
								dc, 
								nDestLeft, 
								rcDraw.top, 
								nTileWidth, 
								rcDraw.Height(),
								m_bmpCache, 
								m_nLeft + m_nMarginLeft, 
								m_nTop, 
								nTileWidth, 
								rcDraw.Height(),
								m_nAlpha);

							nRemainWidth -= nTileWidth;
							nDestLeft += nTileWidth;
						}
						//��ʣһС��
						if (nRemainWidth > 0)
						{
							CBkBitmap::AlphaBlend(
								dc, 
								nDestLeft, 
								rcDraw.top, 
								nRemainWidth, 
								rcDraw.Height(),
								m_bmpCache, 
								m_nLeft + m_nMarginLeft, 
								m_nTop, 
								nRemainWidth, 
								rcDraw.Height(),
								m_nAlpha);
						}
					}
				}
				else
				{
					CBkBitmap::AlphaBlend(
						dc, 
						rcDraw.left + m_nMarginLeft, 
						rcDraw.top, 
						rcDraw.Width() - m_nMarginLeft - m_nMarginRight, 
						rcDraw.Height(),
						m_bmpCache, 
						m_nLeft + m_nMarginLeft, 
						m_nTop, 
						rcDraw.Width() < (size.cx - m_nMarginLeft - m_nMarginRight) ? rcDraw.Width() : (size.cx - m_nMarginLeft - m_nMarginRight),
						rcDraw.Height() < (size.cy - m_nTop) ? rcDraw.Height() : (size.cy - m_nTop),
						m_nAlpha);
				}
			}				               

			if (m_nMarginRight)
			{
				CBkBitmap::AlphaBlend(
					dc, 
					rcDraw.right - m_nMarginRight, 
					rcDraw.top, 
					m_nMarginRight, 
					rcDraw.Height(),
					m_bmpCache, 
					size.cx - m_nMarginRight, 
					m_nTop, 
					m_nMarginRight, 
					rcDraw.Height(),
					m_nAlpha);
			}
			else if (m_nMarginBottom)
			{
				CBkBitmap::AlphaBlend(
					dc, 
					rcDraw.left, 
					rcDraw.bottom - m_nMarginBottom, 
					rcDraw.Width(), 
					m_nMarginBottom,
					m_bmpCache, 
					m_nLeft, 
					size.cy - m_nMarginBottom, 
					rcDraw.Width(), 
					m_nMarginBottom,
					m_nAlpha);
			}
		}
		else
		{
			int nBlock = m_bmpCache.GetWidth() / m_lSubImageWidth;
			if (nBlock != 0)
				dwState = dwState % nBlock;

			CBkBitmap::AlphaBlend(
				dc, 
				rcDraw.left, 
				rcDraw.top, 
				size.cx, 
				size.cy,
				m_bmpCache, 
				m_lSubImageWidth * dwState, 
				0, 
				size.cx, 
				size.cy,
				m_nAlpha);
		}
	}

	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
		DrawImage(dc, rcDraw, rcSr, m_nAlpha);
	}

	virtual void DrawImageGray(CDCHandle dc, CRect &rcDraw, CRect &rcSr)
	{
		if (!m_bLoaded)
			return;

		if (!m_bGrayCache)
		{
			int nWidth = m_bmpCache.GetWidth();
			int nHeight	= m_bmpCache.GetHeight();
			m_bmpGrayCache.CreateDIBSection(nWidth, nHeight);

			CBkBitmap::AlphaBlend(m_bmpGrayCache, 0, 0, nWidth, nHeight, m_bmpCache, 0, 0, nWidth, nHeight, 255);

			DWORD dwColor, dwTemp;
			DWORD red, green, blue, alphas;
			for (int x = 0; x < nWidth; x++)
			{
				for(int y = 0; y < nHeight; y++)
				{
					dwColor = m_bmpGrayCache.GetPixel(x, y);
					if (dwColor != 0)
					{
						alphas = dwColor & 0xff000000;
						red = (dwColor & 0xff0000) >> 16;
						green = (dwColor & 0xff00) >> 8;
						blue = dwColor & 0xff;
						dwTemp = (red * 30 + green * 59 + blue * 11) / 100;
						dwColor = dwTemp << 16;
						dwColor |= dwTemp << 8;
						dwColor |= dwTemp;
						dwColor |= alphas;
						m_bmpGrayCache.SetPixel(x, y, dwColor);
					}					
				}
			}

			m_bGrayCache = TRUE;
		}

		CBkBitmap::AlphaBlend(dc.m_hDC, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), m_bmpGrayCache, rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), m_nAlpha);			
	}

	virtual void DrawImage(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nAlpha)
	{
		if (!m_bLoaded)
			return;

		CBkBitmap::AlphaBlend(
			dc, 
			rcDraw.left, 
			rcDraw.top, 
			rcDraw.Width(), 
			rcDraw.Height(),
			m_bmpCache, 
			rcSr.left, 
			rcSr.top, 
			rcSr.Width(), 
			rcSr.Height(),
			nAlpha);
	}

	virtual void DrawImage(CDCHandle dc, CPoint point[3], CRect &rcSrc)
	{
		//��ʱ��֧���������
	}

	virtual void DrawBackground(CDCHandle dc, CRect &rcDraw)
	{
		if (!m_bLoaded)
			return;

		INT nCx = m_bmpCache.GetWidth();
		INT nCy = m_bmpCache.GetHeight();

		for( INT nMemCx = 0; nMemCx + nCx <= rcDraw.Width(); nMemCx += nCx )
		{
			for( INT nMemCy = 0; nMemCy + nCy <= rcDraw.Height(); nMemCy += nCy )
			{
				CBkBitmap::AlphaBlend(dc.m_hDC, rcDraw.left + nMemCx, rcDraw.top + nMemCy, nCx, nCy, 
					m_bmpCache, 0, 0, nCx, nCy, m_nAlpha);
			}
		}
	}

	virtual void DrawImage(Gdiplus::Graphics &graphics, CRect &rcDraw, CRect &rcSr)
	{
		
	}

	/**
	*   ���м�ΪԲ�ģ���ת��ͼ��
	*	�Ȳ�������麯��������������Ҳ֧�����ٿ���
	**/
	void DrawImageWithRotate(CDCHandle dc, CRect &rcDraw, CRect &rcSr, int nRotateAngle)
	{
		/*if (m_pImg)
		{
			using namespace Gdiplus;
			Graphics graphics(dc);
			int nHalfWidth  = rcDraw.Width() / 2.0;
			int nHalfHeight = rcDraw.Height() / 2.0;
			float fRotateCenterX = rcDraw.left + nHalfWidth;
			float RotateCenterY = rcDraw.top +  nHalfHeight;

			graphics.TranslateTransform(fRotateCenterX, RotateCenterY);
			graphics.RotateTransform(nRotateAngle);      

			graphics.DrawImage(
				m_pImg, 
				Gdiplus::Rect(0 - nHalfWidth, 0 - nHalfHeight, rcDraw.Width(), rcDraw.Height()), 
				rcSr.left, rcSr.top, rcSr.Width(), rcSr.Height(), 
				Gdiplus::UnitPixel);

			graphics.ResetTransform();
		}*/
	}

	void DrawFlip(CDCHandle dc, CRect rcDst)
	{
		//using namespace Gdiplus;
		//Graphics graphics(dc);

		//m_pImg->RotateFlip(RotateNoneFlipX);
		//graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDst.left, rcDst.top, rcDst.Width(), rcDst.Height()));
		//m_pImg->RotateFlip(RotateNoneFlipX);
	}

	virtual SIZE GetSkinSize()
	{
		SIZE ret = {0, 0};
		if (0 == m_lSubImageWidth)
			ret.cx = m_bmpCache.GetWidth();
		else
			ret.cx = m_lSubImageWidth;
		ret.cy = m_bmpCache.GetHeight();

		return ret;
	}

	virtual BOOL IgnoreState()
	{
		return 0 == m_lSubImageWidth;
	}

protected:
	UINT m_uResID;
	LONG m_lSubImageWidth;
	int	 m_nLeft;
	int	 m_nTop;
	int	 m_nMarginLeft;
	int	 m_nMarginRight;
	int  m_nMarginTop;
	int  m_nMarginBottom;
	int  m_nMiddleTile;
	int  m_nFlip;
	int  m_nAlpha;
	CBkBitmap m_bmpGrayCache;
	BOOL m_bGrayCache;
	CBkBitmap m_bmpCache;
	CStringA m_strFilePath;
	CStringA m_strAlphaKey;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("src", m_uResID, TRUE)
		BKWIN_STRING_ATTRIBUTE("file", m_strFilePath, TRUE)
		BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
		BKWIN_INT_ATTRIBUTE("left", m_nLeft, TRUE)
		BKWIN_INT_ATTRIBUTE("top", m_nTop, TRUE)
		BKWIN_INT_ATTRIBUTE("alpha", m_nAlpha, TRUE)
		BKWIN_STRING_ATTRIBUTE("extern_alpha_key", m_strAlphaKey, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_left", m_nMarginLeft, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_right", m_nMarginRight, TRUE)
		BKWIN_INT_ATTRIBUTE("margin_top", m_nMarginTop, TRUE)           //����margin_top��margin_bottom������png�Ĵ�ֱ����
		BKWIN_INT_ATTRIBUTE("margin_bottom", m_nMarginBottom, TRUE) 
		BKWIN_INT_ATTRIBUTE("flip", m_nFlip, TRUE)
		BKWIN_INT_ATTRIBUTE("middle_tile", m_nMiddleTile, TRUE)     //ͼƬ���м䲿�֣���ȥleft��right������ƽ��
		BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkNormalPngSkin : public CBkNormalSkin
{
public:
	BKOBJ_DECLARE_CLASS_NAME(CBkNormalPngSkin, "skinpng")

	CBkNormalPngSkin()
	{

	}

	~CBkNormalPngSkin()
	{

	}

	BOOL LoadImg()
	{
		if (m_bLoaded)
			return m_bLoaded;

		m_bLoaded = BkSkinPool::Get(m_uResID, CA2W(m_strFilePath), m_bmpCache, BKRES_PNG_TYPE);
		if (!m_strAlphaKey.IsEmpty())
		{
			m_nAlpha = ::GetPrivateProfileInt(L"alpha", CA2W(m_strAlphaKey), 255, KDubaPath::GetModuleFolder((HMODULE)&__ImageBase) + _T("data\\skin\\skincfg.ini"));
		}

		if (m_nFlip)
		{

		}

		return TRUE;
	}
};

class CBkNormalJPGSkin : public CBkNormalSkin
{
public:
	BKOBJ_DECLARE_CLASS_NAME(CBkNormalJPGSkin, "skinjpg")

		CBkNormalJPGSkin()
	{

	}

	~CBkNormalJPGSkin()
	{

	}

	BOOL LoadImg()
	{
		if (m_bLoaded)
			return m_bLoaded;

		m_bLoaded = BkSkinPool::Get(m_uResID, CA2W(m_strFilePath), m_bmpCache, BKRES_JPG_TYPE);
		if (m_nFlip)
		{

		}
		return TRUE;
	}
};

class BkSkin
{
public:
    BkSkin()
    {
    }

    ~BkSkin()
    {
        _Clear();
    }

    static BOOL LoadSkins(UINT uResID)
    {
        CAtlList<CStringA> lstStrXml;

        _Instance()->_Clear();

        BOOL bRet = BkResManager::LoadResourceAtAll(uResID, lstStrXml, BKRES_TYPE);
        if (!bRet)
            return FALSE;

        POSITION pos = lstStrXml.GetHeadPosition();

        while (pos)
        {
            bRet |= LoadSkins(lstStrXml.GetNext(pos));
        }

        return bRet;
    }

	static BOOL LoadSkins2(UINT uResID)
	{
		CAtlList<CStringA> lstStrXml;

		BOOL bRet = BkResManager::LoadResourceAtAll(uResID, lstStrXml, BKRES_TYPE);
		if (!bRet)
			return FALSE;

		POSITION pos = lstStrXml.GetHeadPosition();

		while (pos)
		{
			bRet |= LoadSkins2(lstStrXml.GetNext(pos));
		}

		return bRet;
	}

    static BOOL LoadSkins(LPCSTR lpszXml)
    {
        TiXmlDocument xmlDoc;

        xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);

        if (xmlDoc.Error())
            return FALSE;

        _Instance()->_LoadSkins(xmlDoc.RootElement());

        return TRUE;
    }

	static BOOL LoadSkins2(LPCSTR lpszXml)
	{
		TiXmlDocument xmlDoc;

		xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);

		if (xmlDoc.Error())
			return FALSE;

		_Instance()->_LoadSkins2(xmlDoc.RootElement());

		return TRUE;
	}

	void _LoadSkins2(TiXmlElement *pXmlSkinRootElem)
	{
		LPCSTR lpszSkinName = NULL, lpszTypeName = NULL;

		if (!pXmlSkinRootElem)
			return;

		if (strcmp(pXmlSkinRootElem->Value(), "skins") != 0)
			return;

		__BkSkinPool::CPair *pairRet= NULL;
		CBkSkinBase* pSkin = NULL;

		for (TiXmlElement* pXmlChild = pXmlSkinRootElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
		{
			lpszSkinName = pXmlChild->Attribute("name");
			lpszTypeName = pXmlChild->Value();
			if (!lpszSkinName || !lpszTypeName)
				continue;

			pairRet = m_mapPool.Lookup(lpszSkinName);

			pSkin = _CreateBkSkinByName(lpszTypeName);
			if (!pSkin) continue;

			if (pairRet)
			{
				if (pairRet->m_value)
					delete pairRet->m_value;

				m_mapPool.RemoveKey(lpszSkinName);
			}

			pSkin->Load(pXmlChild);
			m_mapPool[lpszSkinName] = pSkin;
		}
	}

    static CBkSkinBase* GetSkin(LPCSTR lpszSkinName)
    {
        __BkSkinPool::CPair *pairRet = _Instance()->m_mapPool.Lookup(lpszSkinName);

        if (pairRet)
		{
			pairRet->m_value->LoadImg();
            return pairRet->m_value;
		}
        else
            return NULL;
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapPool.GetCount();
    }

    static BOOL Draw(CDCHandle dc, LPCSTR lpszSkinName, CRect rcDraw, DWORD dwState)
    {
        CBkSkinBase *pSkin = GetSkin(lpszSkinName);
        if (!pSkin)
            return FALSE;

        pSkin->Draw(dc, rcDraw, dwState);

        return TRUE;
    }

    static BOOL Draw(Gdiplus::Graphics &canvas, LPCSTR lpszSkinName, CRect rcDraw, DWORD dwState)
    {
        CBkSkinBase *pSkin = GetSkin(lpszSkinName);
        if (!pSkin)
            return FALSE;

		HDC hDC = canvas.GetHDC();
        pSkin->Draw(hDC, rcDraw, dwState);
		canvas.ReleaseHDC(hDC);

        return TRUE;
    }
protected:

    typedef CAtlMap<CStringA, CBkSkinBase *> __BkSkinPool;

    static BkSkin* ms_pInstance;

    static BkSkin* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkSkin;
        return ms_pInstance;
    }

//     static BkSkin& _Instance()
//     {
//         static BkSkin s_obj;
// 
//         return s_obj;
//     }

    __BkSkinPool m_mapPool;

    void _Clear()
    {
        POSITION pos = m_mapPool.GetStartPosition();

        while (pos)
        {
            CBkSkinBase *pSkin = m_mapPool.GetNextValue(pos);
            delete pSkin;
        }

        m_mapPool.RemoveAll();
    }

    void _LoadSkins(TiXmlElement *pXmlSkinRootElem)
    {
        LPCSTR lpszSkinName = NULL, lpszTypeName = NULL;

        if (!pXmlSkinRootElem)
            return;

        if (strcmp(pXmlSkinRootElem->Value(), "skins") != 0)
            return;

        for (TiXmlElement* pXmlChild = pXmlSkinRootElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
        {
            lpszSkinName = pXmlChild->Attribute("name");
            lpszTypeName = pXmlChild->Value();
            if (!lpszSkinName || !lpszTypeName || NULL != m_mapPool.Lookup(lpszSkinName))
                continue;


            CBkSkinBase *pSkin = _CreateBkSkinByName(lpszTypeName);
            if (!pSkin)
                continue;

            pSkin->Load(pXmlChild);
            m_mapPool[lpszSkinName] = pSkin;
        }
    }

    static CBkSkinBase* _CreateBkSkinByName(LPCSTR lpszName)
    {
        CBkSkinBase *pNewSkin = NULL;

        pNewSkin = CBkImageSkin::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinImgFrame::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinButton::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinImgHorzExtend::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinGradation::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkPngSkin::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

		pNewSkin = CBkJpgSkin::CheckAndNew(lpszName);
		if (pNewSkin)
			return pNewSkin;

		pNewSkin = CBkNormalPngSkin::CheckAndNew(lpszName);
		if (pNewSkin)
			return pNewSkin;

		pNewSkin = CBkNormalJPGSkin::CheckAndNew(lpszName);
		if (pNewSkin)
			return pNewSkin;

        return NULL;
    }
};

#pragma warning(default:4244)

__declspec(selectany) BkSkin* BkSkin::ms_pInstance = NULL;

#endif // __bksink__h__