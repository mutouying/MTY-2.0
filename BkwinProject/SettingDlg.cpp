#include "stdafx.h"
#include "SettingDlg.h"

SettingDlg::SettingDlg()
    : CBkDialogViewImplEx<SettingDlg>(IDR_DLG_DEMO_SETTING)
{
	
}

BOOL SettingDlg::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
    return TRUE;
}

void SettingDlg::OnBtnClose()
{
    EndDialog(IDCLOSE);
}

void SettingDlg::OnSysCommand(UINT nID, CPoint point)
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

void SettingDlg::OnListItemLBtnUp(int nListItem)
{
	BkSharePtr<CString> strMsg = BkSharePtr<CString>::createObject();

	strMsg.DataRef().Format(L"DemoListWndDlg::OnListItemLBtnUp:%d", nListItem);
	PostRunnable(BkWin::FuncFactory::create_class_func(this, &SettingDlg::ShowDoModalMsgBox, strMsg));
}

void SettingDlg::ShowDoModalMsgBox(BkSharePtr<CString> msg)
{
	MessageBox(msg.DataRef());
}

void SettingDlg::OnListItemChildLBtnUp(int nListItem, int nChildCtrlId)
{
	BkSharePtr<CString> strMsg = BkSharePtr<CString>::createObject();
	
	strMsg.DataRef().Format(L"DemoListWndDlg::OnListItemChildLBtnUp:%d %d", nListItem, nChildCtrlId);
	PostRunnable(BkWin::FuncFactory::create_class_func(this, &SettingDlg::ShowDoModalMsgBox, strMsg));
}


