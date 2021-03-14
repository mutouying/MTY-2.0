#ifndef _BKBITMAP_H_
#define _BKBITMAP_H_

#include <GdiPlus.h>
#include "cargb.h"

#pragma warning(disable:4018)

typedef DWORD	KARGB;
typedef PDWORD  PKARGB;
typedef BYTE	K_BYTE;

//#define KARGB_GetA(argb)	   ((K_BYTE)(argb>>24))
//#define KARGB_GetR(argb)	   ((K_BYTE)(argb>>16))
//#define KARGB_GetG(argb)	   ((K_BYTE)(argb>>8))
//#define KARGB_GetB(argb)	   ((K_BYTE)(argb))
//#define MAKE_KARGB(a, r, g, b) ((DWORD(a)<<24) | (DWORD(r) << 16) | (DWORD(g) << 8) | DWORD(b))

class CBkBitmap : public CBitmap
{
public:
	CBkBitmap()
	{
		m_pBytes = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	virtual ~CBkBitmap()
	{

	}

    BOOL CreateFromImage(Gdiplus::Image* pImage)
    {
        if (pImage != NULL)
        {
            int nWidth = pImage->GetWidth();
            int nHeight = pImage->GetHeight();

            if (CreateDIBSection(nWidth, nHeight))
            {
				ClearAllPix();

                HDC hDesktop = GetDC(NULL);
                HDC hMemDC = CreateCompatibleDC(hDesktop);
                if (hMemDC)
                {
                    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

                    Gdiplus::Graphics gs(hMemDC);
                    gs.DrawImage(pImage, Gdiplus::Rect(0, 0, nWidth, nHeight), 0, 0, nWidth, nHeight, Gdiplus::UnitPixel);

                    SelectObject(hMemDC, hOldBmp);
                    DeleteDC(hMemDC);
                }
                ReleaseDC(NULL, hDesktop);
            }
        }

        return (m_hBitmap != NULL);
    }

	void CpyFrom(const CBkBitmap& bmpSrc)
	{
		DeleteObject();

		if (bmpSrc.m_hBitmap != NULL && 
			bmpSrc.m_pBytes != NULL &&
			bmpSrc.GetWidth() > 0 &&
			bmpSrc.GetHeight() > 0)
		{
			CreateDIBSection(bmpSrc.GetWidth(), bmpSrc.GetHeight());
		}

		if (m_pBytes)
		{
			memcpy(m_pBytes, bmpSrc.m_pBytes, sizeof(DWORD) * GetWidth() * GetHeight());
		}
	}

	void MakeToGray()
	{
		if (m_hBitmap == NULL || m_pBytes == NULL)
		{
			return;
		}

		DWORD	dwCount = GetWidth() * GetHeight();
		DWORD*	pdwPixels = (DWORD*)m_pBytes;

		CARGB	argb;
		BYTE	byData = 0;

		for (DWORD i = 0; i < dwCount; ++i)
		{
			argb.SetColor(*(pdwPixels + i));
			if (argb.GetArgb() == 0)
			{
				continue;
			}

			byData = (BYTE)(0.299 * argb.GetRed() + 0.578 * argb.GetGreen() + 0.114 * argb.GetBlue());
			argb.SetColor(argb.GetAlpha(), byData, byData, byData);

			*(pdwPixels + i) = argb.GetArgb();
		}
	}

    COLORREF GetAverageColor() const
    {
        if (m_hBitmap == NULL || m_pBytes == NULL)
        {
            return RGB(0, 0, 0);
        }

        DWORD	dwCount = GetWidth() * GetHeight();
        DWORD*	pdwPixels = (DWORD*)m_pBytes;

        CARGB	argb;
        BYTE	byData = 0;

        double fAvgRed = 0.0, fAvgGreen = 0.0, fAvgBlue = 0.0;

        for (DWORD i = 0; i < dwCount; ++i)
        {
            argb.SetColor(*(pdwPixels + i));
            if (argb.GetArgb() == 0)
            {
                continue;
            }


            fAvgRed = (fAvgRed * i + double(argb.GetRed()) ) / (i + 1);
            fAvgGreen = (fAvgGreen * i + double(argb.GetGreen()) ) / (i + 1);
            fAvgBlue = (fAvgBlue * i + double(argb.GetBlue()) ) / (i + 1);
        }

        int nRed = min(255, max(0, (int)fAvgRed));
        int nGreen = min(255, max(0, (int)fAvgGreen));
        int nBlue = min(255, max(0, (int)fAvgBlue));

        return RGB(nRed, nGreen, nBlue);
    }

	bool IsHasColor() const
	{
		if (m_hBitmap == NULL || m_pBytes == NULL)
		{
			return false;
		}

		DWORD	dwCount = GetWidth() * GetHeight();
		DWORD*	pdwPixels = (DWORD*)m_pBytes;

		CARGB	argb;
		BYTE	byData = 0;

		double fAvgRed = 0.0, fAvgGreen = 0.0, fAvgBlue = 0.0;

		for (DWORD i = 0; i < dwCount; ++i)
		{
			argb.SetColor(*(pdwPixels + i));
			if (argb.GetArgb() != 0)
			{
				return true;
			}
		}

		return false;
	}

    void AdjustColor(KARGB argb)
    {
        if (NULL == m_pBytes)
        {
            return;
        }

        int	  nPixelCount = m_nWidth * m_nHeight;
        DWORD *pDwPixel = (DWORD*)m_pBytes;

        DWORD dwFlag = 0xFF000000;
        for (int i = 0; i < nPixelCount; i++)
        {
            DWORD dwClr = *(pDwPixel + i);
            *(pDwPixel + i) = (dwClr & dwFlag) + (argb & ~dwFlag);
        }
    }	

	BOOL CreateDIBSection(int nWidth, int nHeight)
	{
		BOOL bReturn = FALSE;
		BYTE* pBits = NULL;
		BITMAPINFOHEADER bmih;
		ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth = nWidth;
		bmih.biHeight = nHeight;
		bmih.biPlanes = 1;
		bmih.biBitCount = 32;
		bmih.biCompression = BI_RGB;

		if (m_hBitmap != NULL)
		{
			DeleteObject();
			m_hBitmap = NULL;
			m_pBytes = NULL;
		}

		m_nWidth = 0;
		m_nHeight = 0;
		m_pBytes = NULL;
		m_hBitmap = NULL;

		if (nWidth <= 0 || nHeight <= 0) goto Exit0;

		HDC hDestopDC = ::GetDC(NULL);
		CBitmap::CreateDIBSection(hDestopDC, (BITMAPINFO *) &bmih, 0, (VOID**)&pBits, NULL, 0);

		if (hDestopDC)
			::ReleaseDC(NULL, hDestopDC);

		if (pBits)
		{
			m_nWidth = nWidth;
			m_nHeight = nHeight;
			m_pBytes = pBits;

			bReturn = TRUE;
		}

	Exit0:
		return bReturn;
	}

	BOOL CreateDIBSectionWithBmpInfo(int nWidth, int nHeight, const BITMAPINFO * pBitmapInfo)
	{
		if (pBitmapInfo == NULL)
		{
			return FALSE;
		}

		BOOL bReturn = FALSE;
		BYTE* pBits = NULL;

		if (m_hBitmap != NULL)
		{
			DeleteObject();
			m_hBitmap = NULL;
			m_pBytes = NULL;
		}

		m_nWidth = 0;
		m_nHeight = 0;
		m_pBytes = NULL;

		if (nWidth <= 0 || nHeight <= 0) goto Exit0;

		HDC hDestopDC = ::GetDC(NULL);
		CBitmap::CreateDIBSection(hDestopDC, pBitmapInfo, 0, (VOID**)&pBits, NULL, 0);

		if (hDestopDC)
			::ReleaseDC(NULL, hDestopDC);

		if (pBits)
		{
			m_nWidth = nWidth;
			m_nHeight = nHeight;
			m_pBytes = pBits;

			bReturn = TRUE;
		}

	Exit0:
		return bReturn;
	}

	int GetWidth() const
	{
		return m_nWidth;
	}

	int GetHeight() const
	{
		return m_nHeight;
	}

	KARGB GetPixel(int nX, int nY) const
	{
		if (m_pBytes == NULL)
			return 0;

		DWORD *pDwPixel = (DWORD*)m_pBytes;

		if (nX >= m_nWidth || nX < 0)
			return 0;

		if (nY >= m_nHeight || nY < 0)
			return 0;

		return pDwPixel[(m_nHeight - nY - 1) * m_nWidth + nX];
	}

	void SetPixel(int nX, int nY, KARGB dwArgb)
	{
		if (m_pBytes == NULL)
			return;

		DWORD *pDwPixel = (DWORD*)m_pBytes;

		if (nX >= m_nWidth || nX < 0)
			return ;

		if (nY >= m_nHeight || nY < 0)
			return ;

		pDwPixel[(m_nHeight - nY - 1) * m_nWidth + nX] = dwArgb;
	}

	void SetAllPixel(KARGB argb)
	{
		if (m_pBytes == NULL)
			return;

		int	  nPixelCount = m_nWidth * m_nHeight;
		DWORD *pDwPixel = (DWORD*)m_pBytes;

		for (int i = 0; i < nPixelCount; i++)
		{
			*pDwPixel = argb;
			++pDwPixel;
		}
	}	

	void SetAlpha(BYTE byAlpha)
	{
		DWORD dwAlpha = byAlpha << 24;
		DWORD *pDwPixel = (DWORD*)m_pBytes;

		if (pDwPixel == NULL)
			return;

		for (int i = 0; i < m_nWidth; ++i)
		{
			for (int j = 0; j < m_nHeight; ++j)
			{
				BYTE *pAlpha = (BYTE*)(&pDwPixel[(m_nHeight - j - 1) * m_nWidth + i]);
				pAlpha[3] = byAlpha;
			}
		}
	}

	static BOOL Bitblt(CDCHandle dc, int x, int y, int w, int h, CBitmap &bitmap, int nsx, int nsy, DWORD dwDrop = SRCCOPY)
	{
		HBITMAP hOldBitmap = NULL;
		CDC dcMem;
		HDC hDCDesktop = ::GetDC(NULL);
		BOOL bReturn = FALSE;
		
		dcMem.CreateCompatibleDC(hDCDesktop);
		hOldBitmap = dcMem.SelectBitmap(bitmap);

		bReturn = dc.BitBlt(x, y, w, h, dcMem, nsx, nsy, dwDrop);
		dcMem.SelectBitmap(hOldBitmap);

		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

    static BOOL Bitblt(CDCHandle dc, int x, int y, int w, int h, HBITMAP bitmap, int nsx, int nsy, DWORD dwDrop = SRCCOPY)
    {
        HBITMAP hOldBitmap = NULL;
        CDC dcMem;
        HDC hDCDesktop = ::GetDC(NULL);
        BOOL bReturn = FALSE;

        dcMem.CreateCompatibleDC(hDCDesktop);
        hOldBitmap = dcMem.SelectBitmap(bitmap);

        bReturn = dc.BitBlt(x, y, w, h, dcMem, nsx, nsy, dwDrop);
        dcMem.SelectBitmap(hOldBitmap);

        ::ReleaseDC(NULL, hDCDesktop);
        return bReturn;
    }

	static BOOL Bitblt(CBitmap &bitmapDst, int x, int y, int w, int h, CBitmap &bitmapSrc, int nsx, int nsy, DWORD dwDrop = SRCCOPY)
	{
		BOOL bReturn = FALSE;

		HBITMAP hOldBitmapSrc = NULL;
		HBITMAP hOldBitmapDst = NULL;
		CDC dcMemSrc;
		CDC dcMemDst;
		HDC hDCDesktop = ::GetDC(NULL);

		dcMemSrc.CreateCompatibleDC(hDCDesktop);
		hOldBitmapSrc = dcMemSrc.SelectBitmap(bitmapSrc);
		
		dcMemDst.CreateCompatibleDC(hDCDesktop);
		hOldBitmapDst = dcMemDst.SelectBitmap(bitmapDst);

		bReturn = dcMemDst.BitBlt(x, y, w, h, dcMemSrc, nsx, nsy, dwDrop);

		dcMemSrc.SelectBitmap(hOldBitmapSrc);
		dcMemDst.SelectBitmap(hOldBitmapDst);

		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL StretchBlt(CDCHandle dc, int x, int y, int w, int h, CBkBitmap &bitmap, int nsx, int nsy, int nsw, int nsh, DWORD dwDrop = SRCCOPY)
	{
		BOOL bReturn = FALSE;
		CDC dcSrc;
		HBITMAP hOldBitmapDst = NULL;

		if (nsw > bitmap.GetWidth() - nsx)
			nsw = bitmap.GetWidth() - nsx;

		if (nsw < 0)
			nsw = 0;

		if (nsh > bitmap.GetHeight() - nsy)
			nsh = bitmap.GetHeight() - nsy;

		if (nsh < 0)
			nsh = 0;

		if (nsw == 0 || nsh == 0)
		{
			bReturn = TRUE;
			goto Exit0;
		}


		dcSrc.CreateCompatibleDC(NULL);
		hOldBitmapDst = dcSrc.SelectBitmap(bitmap);

		bReturn = ::StretchBlt(dc.m_hDC, x, y, w, h, dcSrc, nsx, nsy, nsw, nsh, dwDrop);

		dcSrc.SelectBitmap(hOldBitmapDst);

	Exit0:
		return bReturn;
	}

	static BOOL StretchBlt(CBkBitmap &bitmapDest, int x, int y, int w, int h, CBkBitmap &bitmapSrc, int nsx, int nsy, int nsw, int nsh, DWORD dwDrop = SRCCOPY)
	{
		BOOL bReturn = FALSE;
		CDC dcDest;
		HBITMAP hBmpOld = NULL;

		dcDest.CreateCompatibleDC(NULL);
		hBmpOld= dcDest.SelectBitmap(bitmapDest);

		bReturn = StretchBlt(dcDest.m_hDC, x, y, w, h, bitmapSrc, nsx, nsy, nsw, nsh, dwDrop);
		dcDest.SelectBitmap(hBmpOld);

		return bReturn;
	}

	static BOOL Bitblt(CBitmap &bitmapDst, int x, int y, int w, int h, HBITMAP hBitmap, int nsx, int nsy, DWORD dwDrop = SRCCOPY)
	{
		BOOL bReturn = FALSE;

		HBITMAP hOldBitmapSrc = NULL;
		HBITMAP hOldBitmapDst = NULL;
		CDC dcMemSrc;
		CDC dcMemDst;
		HDC hDCDesktop = ::GetDC(NULL);

		dcMemSrc.CreateCompatibleDC(hDCDesktop);
		hOldBitmapSrc = dcMemSrc.SelectBitmap(hBitmap);

		dcMemDst.CreateCompatibleDC(hDCDesktop);
		hOldBitmapDst = dcMemDst.SelectBitmap(bitmapDst);

		bReturn = dcMemDst.BitBlt(x, y, w, h, dcMemSrc, nsx, nsy, dwDrop);

		dcMemSrc.SelectBitmap(hOldBitmapSrc);
		dcMemDst.SelectBitmap(hOldBitmapDst);

		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL Bitblt(CBitmap &bitmapDst, int x, int y, int w, int h, HDC hdcSrc, int nsx, int nsy, DWORD dwDrop = SRCCOPY)
	{
		BOOL bReturn = FALSE;

		HBITMAP hOldBitmapDst = NULL;
		CDC dcMemDst;
		HDC hDCDesktop = ::GetDC(NULL);

		dcMemDst.CreateCompatibleDC(hDCDesktop);
		hOldBitmapDst = dcMemDst.SelectBitmap(bitmapDst);

		bReturn = dcMemDst.BitBlt(x, y, w, h, hdcSrc, nsx, nsy, dwDrop);

		dcMemDst.SelectBitmap(hOldBitmapDst);
		::ReleaseDC(NULL, hDCDesktop);

		return bReturn;
	}

	static BOOL AlphaBlend(CBkBitmap &bitmapDst, int x, int y, int w, int h, CBkBitmap &bitmap, int nsx, int nsy, int nsw, int nsh, int nAlpha)
	{
		BOOL bReturn = FALSE;
		CDC dcDst;
		HBITMAP hOldBitmapDst = NULL;
		HDC hDCDesktop = ::GetDC(NULL);

		dcDst.CreateCompatibleDC(hDCDesktop);
		hOldBitmapDst = dcDst.SelectBitmap(bitmapDst);

		bReturn = AlphaBlend(dcDst.m_hDC, x, y, w, h, bitmap, nsx, nsy, nsw, nsh, nAlpha);

		dcDst.SelectBitmap(hOldBitmapDst);
		
		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL AlphaBlend(HDC hDstDc, int x, int y, int w, int h, CBkBitmap &bitmap, int nsx, int nsy, int nsw, int nsh, int nAlpha)
	{
		HBITMAP hOldBitmap = NULL;
		CDC dcMem;
		BOOL bReturn = FALSE;
		//HDC hDCDestop = ::GetDC(NULL);

		if (nsw > bitmap.GetWidth() - nsx)
			nsw = bitmap.GetWidth() - nsx;

		if (nsw < 0)
			nsw = 0;

		if (nsh > bitmap.GetHeight() - nsy)
			nsh = bitmap.GetHeight() - nsy;

		if (nsh < 0)
			nsh = 0;

		if (nsw == 0 || nsh == 0)
		{
			bReturn = TRUE;
			goto Exit0;
		}

		dcMem.CreateCompatibleDC(hDstDc);
		hOldBitmap = dcMem.SelectBitmap(bitmap);

		BLENDFUNCTION Blend;
		Blend.BlendOp = 0;
		Blend.BlendFlags = 0;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		Blend.SourceConstantAlpha = (BYTE)nAlpha;

		bReturn = ::AlphaBlend(hDstDc, x, y, w, h, dcMem, nsx, nsy, nsw, nsh, Blend);

		dcMem.SelectBitmap(hOldBitmap);

	Exit0:
		return bReturn;
	}

    static void AlphaBlendNineCels(HDC hdc, const CRect &rcDraw, CBkBitmap &bmp, const CRect &rcSrc, const CRect &rcCel, BYTE byAlpha)
    {
        int x = rcDraw.left, y = rcDraw.top, w = rcDraw.Width(), h = rcDraw.Height();

        //CRect��right��bottom�����ߣ�������ʵ�ʵ��ҵ�
        CRect dest[9], src[9];

        //Fill data
        {
            int u1 = rcCel.left, u3 = rcCel.right, u2 = rcSrc.Width() - u1 - u3;
            int v1 = rcCel.top, v3 = rcCel.bottom, v2 = rcSrc.Height() - v1 - v3;

            int nu1 = u1, nu2 = w - u1 - u3, nu3 = u3;
            int nv1 = v1, nv2 = h - v1 - v3, nv3 = v3;

            //���9�������� Src
            {
                int vs[] = {v1, v2, v3};
                int ay = 0;
                for(int i = 0; i < 3; ++i)
                {
                    src[i * 3] = CRect(0, ay, u1, vs[i]);
                    src[i * 3 + 1] = CRect(u1, ay, u2, vs[i]);
                    src[i * 3 + 2] = CRect(u1 + u2, ay, u3, vs[i]);
                    ay += vs[i];
                }
            }

            //���9�������� Dest
            {
                int ay = 0;
                int vs2[] = {nv1, nv2, nv3};
                for(int i = 0; i < 3; ++i)
                {
                    dest[i * 3] = CRect(0, ay, nu1, vs2[i]);
                    dest[i * 3 + 1] = CRect(nu1, ay, nu2, vs2[i]);
                    dest[i * 3 + 2] = CRect(nu1 + nu2, ay, nu3, vs2[i]);
                    ay += vs2[i];
                }
            }
        }
        


        CDC dcMem;
        dcMem.CreateCompatibleDC(hdc);

        HBITMAP hOldBitmap = dcMem.SelectBitmap(bmp);

        BLENDFUNCTION blend;
        blend.BlendOp = 0;
        blend.BlendFlags = 0;
        blend.AlphaFormat = AC_SRC_ALPHA;
        blend.SourceConstantAlpha = (BYTE)byAlpha;

        for(int i = 0; i < 9; ++i)
        {
            dest[i].left += x;
            dest[i].top += y;

            src[i].left += rcSrc.left;
            src[i].top += rcSrc.top;

            if(src[i].right <= 0 || src[i].bottom <= 0 || dest[i].right <= 0 || dest[i].bottom <= 0)
            {
                continue;
            }

            ::AlphaBlend(hdc, dest[i].left, dest[i].top, dest[i].right, dest[i].bottom, 
                dcMem, src[i].left, src[i].top, src[i].right, src[i].bottom, 
                blend);
        }

        dcMem.SelectBitmap(hOldBitmap);
    }

	void SetRgnPix(HRGN hRgn, KARGB dwPix)
	{
		if (m_pBytes == NULL || hRgn == NULL) return;

		for (UINT i = 0; i < m_nWidth; i ++)
		{
			for (UINT j = 0; j < m_nHeight; j++)
			{
				if (::PtInRegion(hRgn, i, j))
				{
					SetPixel(i, j, dwPix);
				}
			}
		}
	}

	/*
	 * ��bmp�����ز�����ָ������ʱ���Ÿı�alpha
	 */                                                                     
	void SetAlpha2(BYTE byAlpha, KARGB argb)
	{
		if (m_pBytes == NULL) 
			return;

		BYTE*  pAlpha = NULL;
		PKARGB pDwPixel = (PKARGB)m_pBytes;
		int	   nPixelCount = m_nHeight * m_nWidth;

		for (int i = 0; i < nPixelCount; i++, pDwPixel++)
		{
			if (*pDwPixel != argb)
			{
				((PBYTE)pDwPixel)[3] = byAlpha;
			}
		}
	}

	void ClearAllPix()
	{
		if(m_pBytes == NULL) return;
		ZeroMemory(m_pBytes, m_nWidth * m_nHeight * 4);
	}

	void GetImageSize(SIZE& sizeImage)
	{
		sizeImage.cx = m_nWidth;
		sizeImage.cy = m_nHeight;
	}

	BOOL DeleteObject()
	{
		if (NULL == m_hBitmap)
			return FALSE;

		m_pBytes = NULL;
		m_nWidth = 0;
		m_nHeight = 0;

		return ::DeleteObject(Detach());
	}

	void Attach(const CBkBitmap& bmp)
	{
		m_hBitmap = bmp.m_hBitmap;

		m_pBytes = bmp.m_pBytes;
		m_nWidth = bmp.m_nWidth;
		m_nHeight = bmp.m_nHeight;
	}

	HBITMAP Detach()
	{
		HBITMAP hBmp = m_hBitmap;

		m_hBitmap = NULL;
		m_pBytes = NULL;
		m_nWidth = 0;
		m_nHeight = 0;

		return hBmp;
	}

	void ClearRngPix(HRGN hRgn)
	{
		if (hRgn == NULL || m_pBytes == NULL || m_nWidth <= 0 || m_nHeight <= 0) 
			return;

		CDC		   dcMem;
		CBkBitmap  memBmp;

		CRgn rgnMem;

		rgnMem.CreateRectRgn(0, 0, m_nWidth, m_nHeight);
		rgnMem.CombineRgn(hRgn, RGN_DIFF);

		dcMem.CreateCompatibleDC(NULL);
		memBmp.CreateDIBSection(m_nWidth, m_nHeight);

		HBITMAP hBmpOld = dcMem.SelectBitmap(memBmp);

		dcMem.SelectClipRgn(rgnMem);
		AlphaBlend(dcMem, 0, 0, m_nWidth, m_nHeight, *this, 0, 0, m_nWidth, m_nHeight, 255);

		dcMem.SelectBitmap(hBmpOld);

		DeleteObject();
		Attach(memBmp);
		memBmp.Detach();
	}

	BOOL Draw(HDC hDC, int nX = 0, int nY = 0, DWORD dwDrop = SRCCOPY)
	{
		return CBkBitmap::Bitblt(CDCHandle(hDC), nX, nY, m_nWidth, m_nHeight, *this, 0, 0, dwDrop);
	}

	BOOL SaveFile(LPCWSTR szFileName, LPCWSTR szMode) const
	{
		BOOL bReturn = FALSE;

		FILE* pFile = NULL;

		int nDataSize = 0;
		LPBYTE pByData = NULL;

		BITMAPFILEHEADER bfh = {0};
		BITMAPINFOHEADER bih = {0};

		if (szFileName == NULL || m_pBytes == NULL)
		{
			goto Exit0;
		}

		nDataSize = m_nHeight * m_nWidth * 4;
		if (nDataSize <= 0) goto Exit0;


		// λͼ��һ���֣��ļ���Ϣ
		bfh.bfType = 0x4d42;		
		bfh.bfReserved1 = 0; 
		bfh.bfReserved2 = 0; 
		bfh.bfSize = nDataSize + sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
		bfh.bfOffBits = bfh.bfSize - nDataSize;

		
		// λͼ�ڶ����֣�������Ϣ
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = m_nWidth;
		bih.biHeight = m_nHeight;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = 0;
		bih.biSizeImage = nDataSize;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed = 0;
		bih.biClrImportant = 0;

		pByData = new BYTE[bfh.bfSize];
		if (pByData == NULL) goto Exit0;

		memcpy(pByData, &bfh, sizeof(BITMAPFILEHEADER));
		memcpy(pByData + sizeof(BITMAPFILEHEADER), &bih, sizeof(BITMAPINFOHEADER));
		memcpy(pByData + bfh.bfOffBits, m_pBytes, nDataSize);
		
		if (szMode)
			pFile = _wfopen(szFileName, szMode);
		else
			pFile = _wfopen(szFileName, L"wb+");

		if (pFile == NULL) goto Exit0;
		if (fwrite(pByData, bfh.bfSize, 1, pFile) != 1)
			goto Exit0;

		bReturn = TRUE;

	Exit0:

		if (pByData) delete[] pByData;
		if (pFile) fclose(pFile);

		return bReturn;
	}

	BOOL UpdateLayeredWnd(
		HWND	 hWnd,
		HDC		 hDcDest = NULL,
		int		 nPtSrcX = 0,
		int		 nPtSrcY = 0,
		BYTE	 bySourceConstantAlpha = 255, 
		BYTE	 byAlphaFormat = AC_SRC_ALPHA, 
		BYTE	 byBlendOp = 0,
		BYTE	 byBlendFlags = 0,
		COLORREF crKey = RGB(0, 0, 0),
		DWORD	 dwFlags = ULW_ALPHA) const
	{
		RECT	rcWnd = { 0};
		BOOL	bReturn = FALSE;

		if (::IsWindow(hWnd))
		{
			::GetWindowRect(hWnd, &rcWnd);

			bReturn = UpdateLayeredWnd(
				hWnd, 
				rcWnd,
				hDcDest,
				nPtSrcX,
				nPtSrcY,
				bySourceConstantAlpha,
				byAlphaFormat, 
				byBlendOp,
				byBlendFlags,
				crKey,
				dwFlags);
		}
	
		return bReturn;
	}

	BOOL UpdateLayeredWnd(
		HWND	 hWnd,
		RECT&	 rcWnd,
		HDC		 hDcDest = NULL,
		int		 nPtSrcX = 0,
		int		 nPtSrcY = 0,
		BYTE	 bySourceConstantAlpha = 255, 
		BYTE	 byAlphaFormat = AC_SRC_ALPHA, 
		BYTE	 byBlendOp = AC_SRC_OVER,
		BYTE	 byBlendFlags = 0,
		COLORREF crKey = RGB(0, 0, 0),
		DWORD	 dwFlags = ULW_ALPHA) const
	{

		return S_UpdateLayeredWnd(
			m_hBitmap,
			hWnd, 
			rcWnd,
			hDcDest,
			nPtSrcX,
			nPtSrcY,
			bySourceConstantAlpha,
			byAlphaFormat, 
			byBlendOp,
			byBlendFlags,
			crKey,
			dwFlags);
	}

	static BOOL S_UpdateLayeredWnd(
		HBITMAP	 hBitmap,
		HWND	 hWnd,
		RECT&	 rcWnd,
		HDC		 hDcDest = NULL,
		int		 nPtSrcX = 0,
		int		 nPtSrcY = 0,
		BYTE	 bySourceConstantAlpha = 255, 
		BYTE	 byAlphaFormat = AC_SRC_ALPHA, 
		BYTE	 byBlendOp = AC_SRC_OVER,
		BYTE	 byBlendFlags = 0,
		COLORREF crKey = RGB(0, 0, 0),
		DWORD	 dwFlags = ULW_ALPHA)
	{
		BOOL		  bReturn = FALSE;
		CDC			  dcMem;
		BLENDFUNCTION m_Blend;

		if (hBitmap == NULL || !IsWindow(hWnd))
		{
			return bReturn;
		}

		dcMem.CreateCompatibleDC(NULL);

		HBITMAP hOdlBmp  =  dcMem.SelectBitmap(hBitmap);
		POINT   ptWinPos =  {rcWnd.left, rcWnd.top};

		m_Blend.BlendOp = byBlendOp;
		m_Blend.BlendFlags = byBlendFlags;
		m_Blend.AlphaFormat = byAlphaFormat;
		m_Blend.SourceConstantAlpha = bySourceConstantAlpha;

		SIZE  size = { rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top};
		RECT  rcClient = { 0, 0, size.cx, size.cy};
		POINT ptSrc = { nPtSrcX, nPtSrcY};

		bReturn = ::UpdateLayeredWindow(
			hWnd, 
			hDcDest, 
			&ptWinPos, 
			&size, 
			dcMem,
			&ptSrc, 
			crKey, 
			&m_Blend, 
			dwFlags);

		dcMem.SelectBitmap(hOdlBmp);

		return bReturn;
	}

    BOOL CreateFromImage(Gdiplus::Image* pImage, float fScaleMultiple, int subWidth = 0)
    {
        if (pImage == NULL)
            return (m_hBitmap != NULL);

        int iImgWidth = pImage->GetWidth();
        int iImgHeight = pImage->GetHeight();

        int iBmpWidth = iImgWidth * fScaleMultiple;
        int iBmpHeight = iImgHeight * fScaleMultiple;

        int iSlice = 1;
        if (subWidth != 0 && iImgWidth != 0 && iImgWidth >= subWidth)
        {
            iSlice = iImgWidth / subWidth;
            int iDistance = iBmpWidth % iSlice;
            iBmpWidth += (iDistance != 0 ? (iSlice - iDistance) : 0);
        }

        if (!CreateDIBSection(iBmpWidth, iBmpHeight))
            return (m_hBitmap != NULL);

        ClearAllPix();
        HDC hDesktop = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hDesktop);
        if (hMemDC)
        {
            HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
            Gdiplus::Graphics gs(hMemDC);
            gs.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);

            if (fScaleMultiple == 1.0 || iSlice == 1)
            {
                gs.DrawImage(pImage, Gdiplus::Rect(0, 0, iBmpWidth, iBmpHeight), 0, 0, iImgWidth, iImgHeight, Gdiplus::UnitPixel);
            }
            else
            {
                Gdiplus::Bitmap imgCache(iImgWidth, iImgHeight, pImage->GetPixelFormat());
                Gdiplus::Graphics* gCache = Gdiplus::Graphics::FromImage(&imgCache);
                if (gCache)
                {
                    gCache->Clear(Gdiplus::Color::Transparent);
                    gCache->DrawImage(pImage, 0, 0, iImgWidth, iImgHeight);

                    int iImgSubWidth = iImgWidth / iSlice;
                    int iBmpSubWidth = iBmpWidth / iSlice;
                    for (int i = 0; i < iSlice; i++)
                    {
                        Gdiplus::Bitmap* subCache = imgCache.Clone(Gdiplus::RectF(iImgSubWidth * i, 0, iImgSubWidth, iImgHeight), PixelFormatDontCare);
                        if (!subCache)
                            break;

                        gs.DrawImage(subCache, Gdiplus::Rect(iBmpSubWidth * i, 0, iBmpSubWidth, iBmpHeight), 0, 0, iImgSubWidth, iImgHeight, Gdiplus::UnitPixel);
                        delete subCache;
                    }
                }
            }
            SelectObject(hMemDC, hOldBmp);
            DeleteDC(hMemDC);
        }
        ReleaseDC(NULL, hDesktop);
        return (m_hBitmap != NULL);
    }
	int m_nWidth;
	int m_nHeight;
	BYTE *m_pBytes;
};

#pragma warning(default:4018)

#endif