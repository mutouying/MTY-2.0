#include "stdafx.h"
#include "KBkwinDemoDlg.h"

#include "DemoCmnCtrlSetDlg.h"
#include "SettingDlg.h"

KBkwinDemoDlg::KBkwinDemoDlg()
    : CBkDialogViewImplEx<KBkwinDemoDlg>(IDR_MAIN)
{
	m_pMenu = NULL;
}

KBkwinDemoDlg::~KBkwinDemoDlg()
{
	if(m_pMenu) 
	{
		if (m_pMenu->IsWindow()) {
			m_pMenu->DestroyWindow();
		}
		delete m_pMenu;
		m_pMenu = NULL;
	}
}

BOOL KBkwinDemoDlg::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
    return TRUE;
}

void KBkwinDemoDlg::OnSysCommand(UINT nID, CPoint point)
{
    if (nID == SC_CLOSE)
    {
        if( ::IsWindowEnabled(m_hWnd))
        {
            OnBtnClose();
        }
    }
    else
    {
        SetMsgHandled(FALSE);
    }
}

void KBkwinDemoDlg::OnBkMenuCmd(CBkDialogMenu* pDialogMenu, LPCWSTR lpszMenuName)
{	
	CString strCmd;

	strCmd.Format(L"OnBkMenuCmd name=%s", lpszMenuName);
	MessageBox(strCmd);
}

void KBkwinDemoDlg::OnBtnMin()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE | HTCAPTION, 0);
	GetWindowRect(&m_oldRect);
}


void KBkwinDemoDlg::OnBtnClose()
{
	// TODO  save to ×¢²á±í

	ShowWindow(SW_HIDE);
	EndDialog(IDCANCEL);
}

void KBkwinDemoDlg::OnSetting()
{
	SettingDlg m_settingDlg;
	m_settingDlg.DoModal();
// 	KChildDlg m_settingDlg;            
// 	if(m_settingDlg.DoModal(GetViewHWND(),NULL)==IDOK)
// 	{
// 		CGlobalSetting::m_GlobalFlag.uColumnChoosed=m_settingDlg.uFunRet;
// 	}
// 	else
// 	{
// 		return;
// 	}
// 	m_wndListProcessInfo.m_list.DeleteAllItems(); // row	
// 
// 	m_wndListProcessInfo.m_list.DeleteColumns();  // header
// 
// 
// 	// modlue info
// 	_DeleteAllModuleInfo();
// 	// init header
// 	_InitListCtrlHeader();
// 	_RefreshListCtrl(m_mapCopy, TRUE);
// 	m_wndListProcessInfo.SetScrollPos(SB_HORZ, 0);
// 	m_wndListProcessInfo.ResetScrollBars(SB_HORZ, 0, FALSE);
// 	m_wndListProcessInfo.m_list.SetItemHighligth(0, -1, 0, 0);
// 	m_wndListProcessInfo.m_list.ResetScrollBars();
// 	m_wndListProcessInfo.m_list.Invalidate();

}