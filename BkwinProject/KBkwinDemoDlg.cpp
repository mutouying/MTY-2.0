#include "stdafx.h"
#include "KBkwinDemoDlg.h"

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
	testCode();
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
	EndDialog(IDCANCEL);
}

void KBkwinDemoDlg::OnSetting()
{
	SettingDlg m_settingDlg;
	
 	if(m_settingDlg.DoModal()==IDOK)
 	{
		// 确定，有更改数据，更新画面
 		
 	}
 	else
 	{
 		// 不需要变化
 	}
	return;
}

void KBkwinDemoDlg::testCode()
{
	m_mapProInfo.clear();
	for(DWORD i = 0; i< 200; ++i)
	{
		SetProcessInfo info(i);
		m_mapProInfo.insert(make_pair(i, info));
	}

}


