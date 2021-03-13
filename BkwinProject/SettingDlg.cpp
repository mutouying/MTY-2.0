#include "stdafx.h"
#include "SettingDlg.h"

SettingDlg::SettingDlg()
    : CBkDialogViewImplEx<SettingDlg>(IDR_DLG_DEMO_SETTING)
{
	
}

BOOL SettingDlg::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	InitSeting();
    return TRUE;
}

void SettingDlg::OnBtnClose()
{
    EndDialog(IDCLOSE);
}

void SettingDlg::OnBtnCancle()
{
	EndDialog(IDCANCEL);
}

void SettingDlg::OnBtnConfirm()
{
	SaveSetting();
	EndDialog(IDOK);
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



void SettingDlg::ShowDoModalMsgBox(BkSharePtr<CString> msg)
{
	MessageBox(msg.DataRef());
}


void SettingDlg::InitSeting()
{
	// SetItemCheck(ID_CHECK_CPUCurrentUsage , (CGlobalSetting::m_GlobalFlag.uColumnChoosed & defCPUCurrentUsage)>0);
}

void SettingDlg::SaveSetting()
{
	(uFunRet) = 0;
// 	if ( GetItemCheck(ID_CHECK_CPUCurrentUsage) )
// 		uFunRet |= defCPUCurrentUsage;

	// CGlobalSetting::m_GlobalFlag.uColumnChoosed = uFunRet;
}