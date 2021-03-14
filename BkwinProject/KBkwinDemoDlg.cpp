#include "stdafx.h"
#include "KBkwinDemoDlg.h"
#include "SettingDlg.h"

#include "framework/KTinyXml.h"
#include "framework/KCreateXmlElementFunc.h"

#define  ITEM_HEIGHT 20

KBkwinDemoDlg::KBkwinDemoDlg()
    : CBkDialogViewImplEx<KBkwinDemoDlg>(IDR_MAIN)
{
    RegisterIpcServer();
    LoadMule();
    m_evtFirsetLoadEnd.Create(NULL, TRUE, FALSE);
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

    UnRegisterIpcServer();
}

BOOL KBkwinDemoDlg::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	//testCode();
	//UpdateWindowsUI();
    PostMessage(WM_DELAY_LOADDATA);
    LoadMule();
    return TRUE;
}

LRESULT KBkwinDemoDlg::OnDelayInit(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(m_evtFirsetLoadEnd.Wait(INFINITE))
    {
        UpdateWindowsUI();
    }

    return 0;
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


BOOL KBkwinDemoDlg::LoadMule()
{
    HMODULE    hModule  = NULL;

    typedef IUnknown* (__stdcall *CreateObj_fun)(const IID * iid);

    hModule = ::LoadLibraryW(L"E:\\WorkSpace\\MTY-2.0-GitTest\\bin\\ProcessMgrDll.dll");
    if (NULL == hModule)
    {
        return FALSE;
    }

    CreateObj_fun fn = (CreateObj_fun)GetProcAddress(hModule, "DllQueryInterface");
    if (NULL == fn)
    {
        return FALSE;
    }

    IBaseProcessMgr* pProcessMgr = (IBaseProcessMgr*)fn(&__uuidof(IBaseProcessMgr));
    if(pProcessMgr == NULL)
        return FALSE;

    pProcessMgr->Init();
}

void KBkwinDemoDlg::RegisterIpcServer()
{
    m_strIpcName1 = ipc_first_load_process;
    EASYIPC_REGISTER_METHODEX(m_strIpcName1, KBkwinDemoDlg, FirstLoadProcess, this); 
    EASYIPC_START_INTERFACEEX(m_strIpcName1);

    m_strIpcName2 = ipc_first_load_process_end;
    EASYIPC_REGISTER_METHODEX(m_strIpcName2, KBkwinDemoDlg, FirstLoadEnd, this); 
    EASYIPC_START_INTERFACEEX(m_strIpcName2);

    m_strIpcName3 = ipc_refresh_process;
    EASYIPC_REGISTER_METHODEX(m_strIpcName3, KBkwinDemoDlg, RefreshProcess, this); 
    EASYIPC_START_INTERFACEEX(m_strIpcName3);
}

void KBkwinDemoDlg::UnRegisterIpcServer()
{
    EASYIPC_STOP_INTERFACEEX(m_strIpcName1); 
    EASYIPC_UNREGISTEREX(m_strIpcName1);

    EASYIPC_STOP_INTERFACEEX(m_strIpcName2); 
    EASYIPC_UNREGISTEREX(m_strIpcName1);

    EASYIPC_STOP_INTERFACEEX(m_strIpcName3); 
    EASYIPC_UNREGISTEREX(m_strIpcName1);
}

int KBkwinDemoDlg::FirstLoadProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    KLocker locker(m_csNewItemNotify);

    SetProcessInfo newItem;

    newItem.strProcessFullPath = pParam->GetString(L"strProcessFullPath", NULL);
    newItem.strProcessName = pParam->GetString(L"strProcessName", NULL);
    newItem.dwProcessID = pParam->GetInt(L"dwProcessID", 0);
    newItem.dwCpuUsage = pParam->GetInt(L"dwCpuUsage", 0);
    newItem.strMemoryUser = pParam->GetString(L"strMemoryUser", 0);
    newItem.strUserName = pParam->GetString(L"strUserName", 0);
    newItem.dwParentPID = pParam->GetInt(L"dwParentPID", 0);
    newItem.dwHandleCount = pParam->GetInt(L"dwHandleCount", 0);
    newItem.dwThreadCount = pParam->GetInt(L"dwThreadCount", 0);
    newItem.dwSessionID = pParam->GetInt(L"dwSessionID", 0);

    DWORD dwProcessId = pParam->GetInt(L"dwProcessID", 0);

    m_mapProCache[dwProcessId] = newItem;

    return 0;
}

int KBkwinDemoDlg::FirstLoadEnd(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    UpdateCacheMapToRealMap();
    m_evtFirsetLoadEnd.SetEvent();
    //UpdateWindowsUI();

    return 0;
}

int KBkwinDemoDlg::RefreshProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    return 0;
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

void KBkwinDemoDlg::UpdateCacheMapToRealMap()
{
	m_mapProInfo.clear();
	m_mapProInfo.insert(m_mapProCache.begin(), m_mapProCache.end());

}

void KBkwinDemoDlg::OnListItemLButtonUp( int nListItem )
{

// 	CString strKey;
// 	strKey = GetListItemChildCtrlText(IDC_WND_HOT, nListItem, IDC_WND_HOTKEY);
// 	

}

void KBkwinDemoDlg::OnListItemMouseHover( int nListItem )
{

	// SetListItemChildCtrlAttribute(IDC_LIST_PROC, nListItem, IDC_LIST_KEY, "crtext", "333333");
}

void KBkwinDemoDlg::OnListItemMouseLeave( int nListItem )
{

	// SetListItemChildCtrlAttribute(IDC_LIST_PROC, nListItem, IDC_LIST_KEY, "crtext", "999999");
}

void KBkwinDemoDlg::UpdateWindowsUI()
{
	if(m_mapProInfo.empty())
	{
		return;
	}
	//DeleteAllListItem(IDC_LIST_PROC);


	// TODO 第一列的行头在 这里添加
	//AddOnekeyToListWnd( SetProcessInfo() , 0);

	int nIndex = 0;
	for (map<DWORD, SetProcessInfo>::iterator it= m_mapProInfo.begin();
		it != m_mapProInfo.end(); ++it)
	{
		SetProcessInfo info = it->second;
		AddOnekeyToListWnd(info, nIndex++);
	}
    //map<DWORD, SetProcessInfo>::iterator it= m_mapProInfo.begin();
    //AddOnekeyToListWnd((*it).second, nIndex++);


	UpdateLayoutList(IDC_LIST_PROC);
}

BOOL KBkwinDemoDlg::AddOnekeyToListWnd( SetProcessInfo & data , int nIndex)
{
	BOOL bReturn = FALSE;
	KTinyXml tinyXml;

	do 
	{
		BOOL bRetCode = FALSE;

		if (NULL == tinyXml.Open("Root", TRUE))
		{
			break;
		}

		if (TRUE)
		{
			KTinyXmlRememberPos(tinyXml);
			bRetCode = CreateHotkeyListItemXml(data, tinyXml, nIndex);
		}

		if (!bRetCode || !tinyXml.FirstChild())
		{
			goto Exit0;
		}
		CStringA strXml = tinyXml.ToXml();
		int nListItem = AppendListItem(IDC_LIST_PROC, tinyXml.GetElement(), -1, FALSE);
		if (nListItem == -1) goto Exit0;

		bReturn = TRUE;

	}while(FALSE);

Exit0:
	return bReturn;
}

BOOL KBkwinDemoDlg::CreateHotkeyListItemXml(SetProcessInfo & data, KTinyXml tinyXml, int nIndex )
{
	int nwidth = 100; //可变, 根据选了多少个长度
	CString pos;
	int posFirstLeft;
	int posFirstRight;
	CString str;

	BOOL bReturn = FALSE;
	CStringA StrPos;
	KCreateXmlElementFunc XmlElFunc(tinyXml);

	bReturn = XmlElFunc.AddTinyChild("listitem", 100 + nIndex, NULL, NULL, NULL, NULL, "1");
	if (FALSE == bReturn) goto Exit0;


	bReturn = XmlElFunc().Write("height", ITEM_HEIGHT);
	if (FALSE == bReturn) goto Exit0;

	bReturn = XmlElFunc().Write("class", "listitem_class");
	if (FALSE == bReturn) goto Exit0;

	bReturn = XmlElFunc.AddTinyChild("dlg", NULL,  "0,0,-0,-0", NULL, NULL, NULL, "1");
	if (FALSE == bReturn) goto Exit0;
 
	posFirstLeft = 0;
	posFirstRight = posFirstLeft + nwidth;
	pos.AppendFormat(L"%d,0,%d,-0", posFirstLeft, posFirstRight);
	bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_KEY, CT2A(pos), NULL, NULL, "text_center");
	if (FALSE == bReturn) return bReturn;
	XmlElFunc().Write("crtext", "999999");
	XmlElFunc().WriteText(data.strProcessName);

	pos.Empty();
	posFirstLeft = posFirstRight;
	posFirstRight = posFirstLeft + nwidth;
	pos.AppendFormat(L"%d,0,%d,-0", posFirstLeft, posFirstRight);
	bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_KEY, CT2A(pos), NULL, NULL, "text_center");
	if (FALSE == bReturn) return bReturn;
	XmlElFunc().Write("crtext", "999999");
	str.Empty();
	str.AppendFormat(L"%d", data.dwParentPID);
	XmlElFunc().WriteText(str);

Exit0:
	return bReturn;
}

void KBkwinDemoDlg::OnListItemChildLBtnUp(int nListItem, int nChildCtrlId)
{
// 	BkSharePtr<CString> strMsg = BkSharePtr<CString>::createObject();
// 
// 	strMsg.DataRef().Format(L"DemoListWndDlg::OnListItemChildLBtnUp:%d %d", nListItem, nChildCtrlId);
// 	PostRunnable(BkWin::FuncFactory::create_class_func(this, &DemoListWndDlg::ShowDoModalMsgBox, strMsg));
}
