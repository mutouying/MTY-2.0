#include "stdafx.h"
#include "SettingDlg.h"
#include "ConfigUtil.h"

SettingDlg::SettingDlg()
    : CBkDialogViewImplEx<SettingDlg>(IDR_DLG_DEMO_SETTING)
{
	m_flagSet = 0;
	m_SelectSum = 0;
	
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
	m_flagSet = 0;
	m_SelectSum = 0;
	if ( GetItemCheck(ID_CHECK_PRO_NAME) )
	{
 		m_flagSet |= FlagProcessName;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_ID) )
	{
		m_flagSet |= FlagProcessPid;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_PATH) )
	{
		m_flagSet |= FlagProcessPath;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_CPU) )
	{
		m_flagSet |= FlagProcessCpu;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_MEM) )
	{
		m_flagSet |= FlagProcessMem;
		m_SelectSum ++;
	}


	if ( GetItemCheck(ID_CHECK_PRO_EXT_1) )
	{
		m_flagSet |= FlagProcessUser;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_EXT_2) )
	{
		m_flagSet |= FlagProcessParent;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_EXT_3) )
	{
		m_flagSet |= FlagProcessHandle;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_EXT_4) )
	{
		m_flagSet |= FlagProcessThread;
		m_SelectSum ++;
	}

	if ( GetItemCheck(ID_CHECK_PRO_EXT_5) )
	{
		m_flagSet |= FlagProcessSession;
		m_SelectSum ++;
	}

	// CGlobalSetting::m_GlobalFlag.uColumnChoosed = uFunRet;
}