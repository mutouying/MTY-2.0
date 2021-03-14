////////////////////////////////////////////////////////////////////////////////
//      
//      control for kpfwfrm
//      
//      File      : kcombobox.h
//      Version   : 1.0
//      Comment   : �Ի�combobox�ؼ�
//      
//      Create at : 2009-03-10
//		Create by : chenguicheng  
//      
///////////////////////////////////////////////////////////////////////////////

#pragma once 
#include <atlwin.h>
#include <vector>
#include <atlctrls.h>
#include "popuplistwnd.h"
#include "tracktooltip.h"

#define WM_USER_KCOMBOBOX				(WM_USER + 1235)
#define WM_USER_KCOMBOBOX_CLICK         (WM_USER + 1236)

typedef struct tagComBoItem{
	ATL::CString strWebUrl;
	ATL::CString strWebName;
} COMBOITEM;


class KComboBoxPopList : public KPopUpListWnd
{
public:

	KComboBoxPopList()
	{
	}

	~KComboBoxPopList()
	{

	}

	//��combobox ʹ��
	BOOL PopUp( int xPos, int yPos )
	{
		CRect rect;
		::GetClientRect( GetMsgHandle(), &rect );
		ASSERT(m_hWnd);

		CRect newRect( xPos, yPos, xPos + rect.Width(), yPos );
		newRect.bottom += m_vecText.size() * GetItemHeight();

		SetItemWidth( newRect.Width() );

		MoveWindow( newRect, TRUE );
		ShowWindow( SW_SHOWNORMAL );
		SetCapture();

		return TRUE;
	}

	//һ��popʹ��
	BOOL PopUp( int xPos, int yPos, int nWidth )
	{
		CRect rect;
		::GetClientRect( GetMsgHandle(), &rect );
		ASSERT(m_hWnd);

		CRect newRect( xPos, yPos, xPos + nWidth, yPos );
		newRect.bottom += m_vecText.size() * GetItemHeight();

		SetItemWidth( nWidth );

		MoveWindow( newRect, TRUE );
		ShowWindow( SW_SHOWNORMAL );
		SetCapture();

		return TRUE;
	}

	BOOL IsPopUp()
	{
		return IsWindowVisible();
	}

	BOOL HideWnd()
	{
		ASSERT(m_hWnd);
		ShowWindow(SW_HIDE);
		ReleaseCapture();

		return TRUE;
	}

	void SetBorderStyle( COLORREF clr1, COLORREF clr2, COLORREF clr3, COLORREF clrSel)
	{
		m_clrBorder1	= clr1;
		m_clrBorder2	= clr2;
		m_clrBorder3	= clr3;
		m_clrSel		= clrSel;
	}

protected:

	void DrawBkGnd( CDCHandle& dc, int& xPos, int& yPos, COLORREF clrSel )
	{
		CRect rect;
		GetClientRect( &rect );
		dc.FillSolidRect( rect, RGB(0xFF, 0xFF, 0xFF));

		CRect rectSel( rect );
		if ( GetCurIndex() != -1 )
		{
			rectSel.top = GetCurIndex() * GetItemHeight() ;
			rectSel.bottom = ( GetCurIndex() + 1 ) * GetItemHeight();
			dc.FillSolidRect( rectSel, clrSel );
		}
	}

	void DrawBorder( CDCHandle &dc )
	{
		CRect rect;
		GetClientRect( &rect );

		CPen pen;
		HPEN hOldPen = NULL;
		pen.CreatePen( PS_SOLID, 1, m_clrBorder1 );

		hOldPen = dc.SelectPen( pen );
		dc.MoveTo( rect.left, rect.top );
		dc.LineTo( rect.left, rect.bottom - 1 );
		dc.LineTo( rect.right - 1, rect.bottom - 1 );
		dc.LineTo( rect.right - 1, rect.top - 1 );

		if ( hOldPen )
			dc.SelectPen( hOldPen );
	}

	void DrawBorderComboBox( CDCHandle &dc )
	{
		CRect rect;
		GetClientRect( &rect );

		CPen pen;
		HPEN hOldPen = NULL;
		pen.CreatePen( PS_SOLID, 1, m_clrBorder1 );

		hOldPen = dc.SelectPen( pen );
		dc.MoveTo( rect.left, rect.top );
		dc.LineTo( rect.left, rect.bottom - 1 );
		dc.LineTo( rect.right - 1, rect.bottom - 1 );
		dc.LineTo( rect.right - 1, rect.top - 1 );

		if ( hOldPen )
			dc.SelectPen( hOldPen );
	}

	void DrawBorderNormal( CDCHandle &dc )
	{
		CRect rect;
		GetClientRect( &rect );

		CBrush brush( ::CreateSolidBrush( m_clrBorder1 ) );
		dc.FrameRect( rect, brush );
	}

	virtual void DrawItemText( CDCHandle &dc, int& xPos, int& yPos )
	{
		CFont font( GetNormalFont() );
		HFONT hOldFont = dc.SelectFont( font );

		CRect rect;
		GetClientRect( &rect );
		rect.left += 4;

		dc.SetBkMode( TRANSPARENT );
		for ( int i = 0; i < m_vecText.size(); i ++ )
		{
			rect.top = i * GetItemHeight();
			rect.bottom = ( i + 1 ) * GetItemHeight();
			dc.DrawText( m_vecText[i], 
						m_vecText[i].GetLength(), 
						&rect, 
						DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS );
		}

		if ( hOldFont )
		{
			dc.SelectFont( hOldFont );
		}
	}

protected:
	
	COLORREF	m_clrBorder1;
	COLORREF	m_clrBorder2;
	COLORREF	m_clrBorder3;
};


enum{
	enum_TYPE_NONE			= 0x0000,
	enum_TYPE_EDIT			= 0x0001,		
	enum_TYPE_TIP			= 0x0002,
	enum_TYPE_LEFTLINE		= 0x0004,
};

class KComboBox: public CWindowImpl<KComboBox>
{
public:

	KComboBox()
	:m_pPopList(NULL),
	m_bChildSel(FALSE),
	m_bMouseOver(FALSE),
	m_clrBorder1(RGB(0x76,0x84,0x8F)),
	m_clrBorder2(RGB(0xDA,0xDB,0xDD)),
	m_clrBorder3(RGB(0xEE,0xEE,0xF0)),
	m_clrListSel(RGB(0xE6,0xF6,0xE9)),
	m_nCurSel(0),
    m_bEnableInput(TRUE),
	m_nType( enum_TYPE_EDIT | enum_TYPE_TIP | enum_TYPE_LEFTLINE )
	{
		m_pPopList = new KComboBoxPopList;
	}

	~KComboBox()
	{
		CLear();
	}

	DECLARE_WND_CLASS( _T("KCOMBOBOX") )

	BEGIN_MSG_MAP(KComboBox)
		MESSAGE_HANDLER( WM_PAINT,			OnPaint )
		MESSAGE_HANDLER( WM_CREATE,			OnCreate )
		MESSAGE_HANDLER( WM_LBUTTONDOWN,	OnLButtonDown )
		MESSAGE_HANDLER( WM_MOUSEMOVE,		OnMouseMove)
		MESSAGE_HANDLER( WM_DESTROY,		OnDestroy)
		MESSAGE_HANDLER( WM_USER_POPUPLIST,	OnUserSelect)
		MESSAGE_HANDLER( WM_MOUSELEAVE,		OnMouseLeave)
		MSG_WM_SIZE(OnSize)

	END_MSG_MAP()


	void SetType( int nType )
	{
		m_nType = nType;
	}

	void AddType( int nType )
	{
		m_nType |= m_nType;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CPaintDC dc( m_hWnd );

		//drawborder
		DrawBorder( dc );

		//drawText
		DrawSelText( dc );

		//draw bmp
		DrawBtn( dc );		

		return TRUE;
	}

	void SetSel( int nIndex )
	{
		m_nCurSel = nIndex;
		ATLASSERT( m_nCurSel < m_vecItem.size() );

		if( m_nCurSel < m_vecItem.size() )
		{
			COMBOITEM* pItem = m_vecItem[m_nCurSel];
			if( pItem )
				m_strSelText = pItem->strWebName;

			if( enum_TYPE_EDIT & m_nType )
			{
				m_wndEdit.SetWindowText( pItem->strWebUrl );
			}
		}
	}

	int GetSel()
	{
		return m_nCurSel;
	}

	void DrawBorder( CPaintDC& dc )
	{
		CRect rect;
		GetClientRect( &rect );		
		//CBrush brush1(::CreateSolidBrush(RGB(47,64,92)));
		//dc.FillRect(&rect,brush1);
		CBrush brush2(::CreateSolidBrush(RGB(255,255,255)));
		CPen pen(::CreatePen(PS_SOLID,1,m_clrBorder1));
		dc.SelectBrush(brush2);		
		dc.SelectPen(pen);
		//dc.FrameRect( &rect, brush1);
		POINT p;
		p.x = 6;
		p.y = 6;
		dc.RoundRect(&rect, p);
	}

	void DrawSelText( CPaintDC& dc )
	{
		CRect rect;
		GetClientRect( &rect );
		rect.left += 4;
		rect.top += 2;
		rect.bottom -= 3;
		//rect.right -= 0;

		//rect.right -= rect.Height();

		if( enum_TYPE_EDIT & m_nType )
		{
			
		}
		else
		{
			HFONT hOldFont = dc.SelectFont( m_Font );
			int nMode = dc.SetBkMode( TRANSPARENT );
			dc.DrawText( m_strSelText, -1, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
			dc.SetBkMode( nMode );
			if( hOldFont )
				dc.SelectFont( hOldFont );
		}
	}

	void DrawBtn( CPaintDC& dc )
	{
		CRect rect;
		GetClientRect( &rect );

		CRgn rgn;
		CPoint point[3];
		int nHeight = rect.Height() / 3;

		if( enum_TYPE_LEFTLINE & m_nType )
		{
			CPen pen;
			pen.CreatePen( PS_SOLID, 1, m_clrBorder1 );
			HPEN hOldPen = dc.SelectPen( pen );
			dc.MoveTo( rect.right - 4 * nHeight + 3, rect.top + 4 );
			dc.LineTo( rect.right - 4 * nHeight + 3, rect.bottom - 4);
			if( hOldPen )
				dc.SelectPen( hOldPen );
		}

		point[0].x = rect.right - 3 * nHeight + 2 ;
		point[0].y = nHeight + 1;

		point[1].x = rect.right - nHeight + 1;
		point[1].y = nHeight + 1;

		point[2].x = rect.right - 2 * nHeight + 1;
		point[2].y = 2 * nHeight + 1;

		rgn.CreatePolygonRgn(point, 3, ALTERNATE );
		CBrush brush;

		brush = ::CreateSolidBrush(RGB(46,148,252));
		dc.FillRgn( rgn, brush );

		
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		TEXTVEC		vecText;
		HBITMAPVEC	vecBmp;
		CRect		rect;
		InitWebName( vecText );
		GetClientRect( &rect );

		m_bChildSel = TRUE;

		if ( m_pPopList )
		{
			if (rect.Height())
				m_pPopList->SetItemHeight( rect.Height() );
			m_pPopList->SetBorderStyle( m_clrBorder1, m_clrBorder2, m_clrBorder3, m_clrListSel);
			m_pPopList->CreatePopUpList( m_hWnd, GetParent(), vecBmp, vecText, RGB(0xFF, 0xFF, 0xFF) );
		}

		m_Font = GetNormalFont();

		if( m_nType & enum_TYPE_EDIT )
		{
			int nHeight = 4 * ( rect.Height() / 3 );
			m_wndEdit.Create( m_hWnd, 
				CRect( rect.left + 4, rect.top + 2, rect.right - nHeight + 3, rect.bottom - 2),
				NULL,
				WS_VISIBLE | WS_CHILDWINDOW | ES_AUTOVSCROLL | ES_AUTOHSCROLL ,  0,
				1023);
			m_wndEdit.BringWindowToTop();
			m_wndEdit.ShowWindow( SW_SHOW );
			
			m_wndEdit.SetFont(m_Font);
			m_wndEdit.SetWindowText( m_strSelText );
            EnableInput(m_bEnableInput);
		}		

		if( m_nType & enum_TYPE_TIP )
		{
			m_trackTooltip.Init( m_hWnd );
			m_trackTooltip.SetRelay( TRUE, 500 );
		}
		
		return TRUE;
	}

	void OnSize(UINT nType, CSize size)
	{
		if (SIZE_MINIMIZED != nType && ::IsWindow(m_hWnd))
		{
			CRect rcClient;
			GetClientRect(rcClient);
			int nHeight = 4 * ( rcClient.Height() / 3 );

			m_wndEdit.MoveWindow(CRect( rcClient.left + 4, rcClient.top + 5, rcClient.right - nHeight + 3, rcClient.bottom - 5));

			if (size == m_sizeWnd)
			{
				SetMsgHandled(FALSE);
				return;
			}

			CRect rcWindow;
			CRgn rgnWindow;

			GetClientRect(rcClient);

			rgnWindow.CreateRoundRectRgn(rcClient.left,rcClient.top,rcClient.right+1,rcClient.bottom+1,5,5);
			SetWindowRgn(rgnWindow);

			m_sizeWnd = size;
		}

		SetMsgHandled(FALSE);
	}

	HFONT GetNormalFont()
	{
		CFontHandle hFont;
		LOGFONT lf;
		RtlZeroMemory(&lf, sizeof(LOGFONT));
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 12 ;
		lf.lfWeight = 400;
		_tcscpy(lf.lfFaceName, TEXT("����") );
		hFont.CreateFontIndirect(&lf);

		return hFont;
	}

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CRect rect;
		GetClientRect( &rect );
		ClientToScreen( &rect );

        CRect rcEdit;
        m_wndEdit.GetWindowRect(rcEdit);
        ScreenToClient(&rcEdit);
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        EnableInput(TRUE);

		if( m_pPopList && !rcEdit.PtInRect(pt))
		{
			if( m_pPopList->IsWindow() )
				m_pPopList->PopUp( rect.left, rect.bottom );
		}
        
        if(m_hNotifyWnd)
        {
            ::PostMessageW(m_hNotifyWnd, WM_USER_KCOMBOBOX_CLICK, 0, 0);
        }

		return TRUE;
	}

	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if ( m_bMouseOver )
			return TRUE;

		if ( m_bMouseOver == FALSE  )
		{
			m_bMouseOver = TRUE;
			StartTrackMouseLeave();

			if( (enum_TYPE_TIP & m_nType) && m_strTip.GetLength() > 0 )
			{
				m_trackTooltip.ActiveTooltip( TRUE );
				CRect rect;
				GetWindowRect( rect );
				m_trackTooltip.ShowText( CPoint( rect.left - 12, rect.top - 45 ), m_strTip, -1 );
			}
		}

		//Invalidate();

		return TRUE;
	}

	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		m_bMouseOver = FALSE;

		m_trackTooltip.ActiveTooltip( FALSE );
		//Invalidate();

		return TRUE;
	}

	BOOL StartTrackMouseLeave()
	{
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		return _TrackMouseEvent(&tme);
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if ( m_pPopList )
		{
			if( m_pPopList->IsWindow() )
				m_pPopList->DestroyWindow();

			delete m_pPopList;
			m_pPopList = NULL;
		}

		if( m_nType & enum_TYPE_EDIT )
		{
			if( m_wndEdit.IsWindow() )
				m_wndEdit.DestroyWindow();
		}

		if( m_nType & enum_TYPE_TIP )
			m_trackTooltip.UnInit();

		return TRUE;
	}

	LRESULT OnUserSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		m_nCurSel = (int) wParam;
		m_strSelText = (LPCTSTR) lParam;

		m_bChildSel = TRUE;

		::SendMessage( GetParent(), WM_USER_KCOMBOBOX, wParam, lParam );

		if( m_nCurSel < m_vecItem.size() )
		{
			COMBOITEM* pItem = m_vecItem[m_nCurSel];
			if( m_wndEdit.IsWindow() && pItem )
				m_wndEdit.SetWindowText( pItem->strWebUrl );
		}

		Invalidate();
		return TRUE;
	}

	void SetBorderStyle( COLORREF clr1, COLORREF clr2, COLORREF clr3, COLORREF clrSel=RGB(0xE6,0xF6,0xE9))
	{
		m_clrBorder1 = clr1;
		m_clrBorder2 = clr2;
		m_clrBorder3 = clr3;
		m_clrListSel = clrSel;

		if( m_pPopList )
			m_pPopList->SetBorderStyle(clr1, clr2, clr3, clrSel);
	}

	LPCTSTR GetInputWebUrl( )
	{
		ATL::CString strInput;
		if( m_wndEdit.IsWindow() )
			m_wndEdit.GetWindowText( strInput );

		COMBOITEM* pItem = CheckWebExist( strInput );

		if( pItem )
			m_strSelText = pItem->strWebUrl;
		else
			m_strSelText = strInput;

		return m_strSelText;
	}

	LPCTSTR GetInputWebName( )
	{
		ATL::CString strInput;
		if( m_wndEdit.IsWindow() )
			m_wndEdit.GetWindowText( strInput );

		COMBOITEM* pItem = CheckWebExist( strInput );
		
		if( pItem )
			m_strSelText = pItem->strWebName;
		else
			m_strSelText = strInput;

		return m_strSelText;
	}

	void SetInputText( LPCTSTR pszText )
	{
		m_strSelText = pszText;
		if( m_wndEdit.IsWindow() )
			m_wndEdit.SetWindowText( m_strSelText );
	}

	void SetLabelString( LPCTSTR pszLabel )
	{
		m_strLabel = pszLabel;
	}

	void SetTip(LPCTSTR pszTip)
	{
		m_strTip = pszTip;
	}

	//�û����
	BOOL AddString( LPCTSTR pszUrl )
	{
		return AddString( pszUrl, pszUrl );
	}

	COMBOITEM* CheckWebExist( LPCTSTR pszWeb )
	{
		BOOL bFind = FALSE;

		for ( int i = 0; i < m_vecItem.size(); i++ )
		{
			COMBOITEM * pItem = m_vecItem[i];
			if( pItem )
			{
				if( _tcsicmp( pszWeb, pItem->strWebUrl ) == 0 )
				{
					return pItem;
				}

				if( _tcsicmp( pszWeb, pItem->strWebName ) == 0 )
				{
					return pItem;
				}
			}
		}

		return NULL;
	}

	//���ڳ�ʼ��
	BOOL AddInitString( LPCTSTR pszWebUrl, LPCTSTR pszWebName )
	{
		COMBOITEM* pItemFind = CheckWebExist( pszWebUrl );
		//�Ҳ����������
		if( pItemFind )
			return FALSE;

		COMBOITEM *pItem = new COMBOITEM;
		pItem->strWebName = pszWebName;
		pItem->strWebUrl = pszWebUrl;
		m_vecItem.push_back( pItem );

		return TRUE;
	}

	BOOL AddString( LPCTSTR pszWebUrl, LPCTSTR pszWebName )
	{
		
		if( !AddInitString( pszWebUrl, pszWebName ) )
			return FALSE;

		HBITMAPVEC		vecBmp;
		TEXTVEC			vecText;

		InitWebName( vecText );
		if( m_pPopList)
		{
			if( m_pPopList->IsWindow() )
				m_pPopList->DestroyWindow();

			m_pPopList->CreatePopUpList( m_hWnd, GetParent(), vecBmp, vecText, RGB(0xFF, 0xFF, 0xFF) );
		}

		return TRUE;
	}

	void InitWebName( TEXTVEC& vecText )
	{
		for ( int i = 0; i < m_vecItem.size(); i++ )
		{
			COMBOITEM * pItem = m_vecItem[i];
			if( pItem )
			{
				vecText.push_back( pItem->strWebName );
			}
		}
	}

	void CLear()
	{
		for ( int i = 0; i < m_vecItem.size(); i++ )
			delete m_vecItem[i];

		m_vecItem.clear();
	}

    void EnableInput(BOOL bEnable)
    {
        if(!m_wndEdit.IsWindow())
            return;

        if(bEnable == m_wndEdit.IsWindowEnabled())
            return;

        m_wndEdit.EnableWindow(bEnable);

        ATL::CString strInput;
        m_wndEdit.GetWindowText(strInput);
        if(bEnable)
        {
            if(strInput == m_strDisableText)
            {
                m_wndEdit.SetWindowText(L"");
            }
            m_wndEdit.SetFocus();
        }
        else
        {
            if(strInput.GetLength() == 0)
            {
                m_wndEdit.SetWindowText(m_strDisableText);
            }
        }
    }

    void SetEnableInput(BOOL bEnable)
    {
        m_bEnableInput = bEnable;
        EnableInput(bEnable);
    }

    void SetDisableInputText(LPCTSTR pszInput)
    {
        m_strDisableText = pszInput;
    }

    void SetNotifyWnd(HWND hWnd)
    {
        m_hNotifyWnd = hWnd;
    }


protected:

	HWND					m_hParent;
    HWND                    m_hNotifyWnd;
	KComboBoxPopList*		m_pPopList;
	CEdit					m_wndEdit;
	BOOL					m_bChildSel;
	ATL::CString			m_strSelText;
    ATL::CString            m_strDisableText;
	ATL::CString			m_strLabel;
	CString					m_strTip;
	BOOL					m_bMouseOver;
    BOOL                    m_bEnableInput;

	COLORREF				m_clrBorder1;
	COLORREF				m_clrBorder2;
	COLORREF				m_clrBorder3;
	COLORREF				m_clrListSel;
	std::vector<COMBOITEM*>	m_vecItem;

	UINT					m_uTimer;
	int						m_nCurSel; 
	KTrackTooltip			m_trackTooltip;
	CFont					m_Font;
	int						m_nType;

	SIZE					m_sizeWnd;
};
