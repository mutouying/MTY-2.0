#include "stdafx.h"
#include "KBkwinDemoDlg.h"
#include "SettingDlg.h"

#include "framework/KTinyXml.h"
#include "ConfigUtil.h"
#include <fstream>
#include <string>


#define  PROCESSINFO_LEN    512
#define  ITEM_HEIGHT 20
#define  ITEM_WIDTH  100

KBkwinDemoDlg::KBkwinDemoDlg()
    : CBkDialogViewImplEx<KBkwinDemoDlg>(IDR_MAIN)
    , m_flagSet(0)
    , m_SelectSum(0)
{
    m_hModule = NULL;
	BkWin::WndShadowHelper<KBkwinDemoDlg>::SetShadowData(12, IDP_SHADOW);
    RegisterIpcServer();
    LoadModule();
    m_evtFirsetLoadEnd.Create(NULL, TRUE, FALSE);
	m_pMenu = NULL;
    m_pProcessMgr = NULL;
    m_bRefreshHeader = FALSE;
    m_pTitleDlg = NULL;

	m_nPid = 0;
	m_evtKillProcess.Create(NULL, TRUE, FALSE);
	m_evtOutputAllProcess.Create(NULL, TRUE, FALSE);
	m_evtOpenFilePos.Create(NULL, TRUE, FALSE);
	m_evtOpenFilePos.Create(NULL, TRUE, FALSE);
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
    if(m_pProcessMgr)
    {
        m_pProcessMgr->UnInit();
        m_pProcessMgr->Drop();
        m_pProcessMgr = NULL;
    }
    if(m_hModule)
    {
        FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
}

BOOL KBkwinDemoDlg::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	//testCode();
	//UpdateWindowsUI();
    UpdateConfig();
    LoadHeaderData(m_vecHeaderData);
    RegisterIpcServer();
    PostMessage(WM_DELAY_LOADDATA);
    LoadModule();

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
	// GetWindowRect(&m_oldRect);
}


BOOL KBkwinDemoDlg::LoadModule()
{
    m_hModule  = NULL;

    typedef IUnknown* (__stdcall *CreateObj_fun)(const IID * iid);

	TCHAR pathm[MAX_PATH];
	GetModuleFileName(NULL, pathm, MAX_PATH);
	CString strModule = pathm;
	int pos = strModule.ReverseFind(L'\\');
	CString strPath = strModule.Left(pos);
	strPath.AppendFormat(L"\\ProcessMgrDll.dll");

    m_hModule = ::LoadLibraryW(strPath);
    if (NULL == m_hModule)
    {
        return FALSE;
    }

    CreateObj_fun fn = (CreateObj_fun)GetProcAddress(m_hModule, "DllQueryInterface");
    if (NULL == fn)
    {
        return FALSE;
    }

    m_pProcessMgr = (IBaseProcessMgr*)fn(&__uuidof(IBaseProcessMgr));
    if(m_pProcessMgr == NULL)
        return FALSE;

    m_pProcessMgr->Init();
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

    m_mapProInfo[dwProcessId] = newItem;

    return 0;
}

int KBkwinDemoDlg::FirstLoadEnd(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    //UpdateCacheMapToRealMap();
    m_evtFirsetLoadEnd.SetEvent();

    return 0;
}

int KBkwinDemoDlg::RefreshProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    return 0;
}

BOOL KBkwinDemoDlg::RefreshListHeader(KTinyXml& tinyXml, int nLeftPos, int nRightPos, const CString& strValue)
{
    BOOL bReturn = FALSE;
    CString strPos;
    CStringA strXml;
    //KTinyXml tinyXml;

    tinyXml.Open("Root", TRUE);
    KTinyXmlRememberPos(tinyXml);

    KCreateXmlElementFunc XmlElFunc(tinyXml);

    bReturn = XmlElFunc.AddTinyChild("text", 100, NULL, NULL, NULL, NULL, "1");
    if(!bReturn)
        goto Exit;

    strPos.Format(L"%d,0,%d,-0", nLeftPos, nRightPos);
    bReturn = XmlElFunc().Write("pos", strPos);
    if(!bReturn)
        goto Exit;

    bReturn = XmlElFunc().Write("crtext", "999999");
    if(!bReturn)
        goto Exit;

    bReturn = XmlElFunc().Write("show", 1);
    if(!bReturn)
        goto Exit;

    bReturn = XmlElFunc().Write("class", "text_center");
    if(!bReturn)
        goto Exit;

    bReturn = XmlElFunc().WriteText(CT2A(strValue));
    if(!bReturn)
        goto Exit;


Exit:
    return bReturn;
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
		UpdateConfig();
 		UpdateWindowsUI();
 	}
	return;
}

void KBkwinDemoDlg::testCode()
{
	m_mapProInfo.clear();
	for(DWORD i = 0; i< 400; ++i)
	{
		SetProcessInfo info(i);
		m_mapProInfo.insert(make_pair(i, info));
	}

}

//void KBkwinDemoDlg::UpdateCacheMapToRealMap()
//{
//	m_mapProInfo.clear();
//	m_mapProInfo.insert(m_mapProCache.begin(), m_mapProCache.end());
//
//}

BOOL KBkwinDemoDlg::AddListItemChilds(SetProcessInfo & data, KCreateXmlElementFunc& XmlElFunc)
{
    KTinyXml tinyXml;
    CString strPos;
    int nLeft, nRight;
    std::vector<CString> vecData;
    LoadRowData(data, vecData);
    int nColWidth = LIST_ITEM_WIDTH / (m_SelectSum + 2);

    for(int i = 0; i < m_SelectSum; ++i)
    {
        int nIndex = i > 1?(i+2):i;
        nLeft = nIndex * nColWidth;
        nRight = i == 1?4*nColWidth:nColWidth + nLeft;
        strPos.Format(L"%d,0,%d,-0", nLeft, nRight);
        XmlElFunc.AddTinySibling("text", IDC_LIST_KEY + i, CT2A(strPos), NULL, NULL, i > 1?"text_center":"text_left");
        XmlElFunc().Write("crtext", "999999");
        XmlElFunc().WriteText(vecData[i]);
        /*if(!m_bRefreshHeader)
        {
            RefreshListHeader(tinyXml, nLeft, nRight, m_vecHeaderData[i]);
        }*/
    }

    /*if(!m_bRefreshHeader)
    {
        CStringA strXml = tinyXml.ToXml();
        CBkPanel* pPanel = static_cast<CBkPanel*>(GetBkItem(IDC_DLG_HEADER));
        if(pPanel != NULL)
        {
            pPanel->LoadChilds(tinyXml.GetElement());
            SetItemNeedReDraw(IDC_DLG_HEADER);
        }

        m_bRefreshHeader = TRUE;
    }*/

    return TRUE;
}

void KBkwinDemoDlg::LoadRowData(SetProcessInfo & processData, std::vector<CString>& vecRowData)
{
    CString strData;

    vecRowData.push_back(processData.strProcessName);
    vecRowData.push_back(processData.strProcessFullPath);
    strData.Format(L"%d", processData.dwProcessID);
    vecRowData.push_back(strData);
    strData.Format(L"%d", processData.dwCpuUsage);
    vecRowData.push_back(strData);
    vecRowData.push_back(processData.strMemoryUser);

    if(m_flagSet & FlagProcessUser)
    {
        vecRowData.push_back(processData.strUserName);
    }
    if(m_flagSet & FlagProcessParent)
    {
        strData.Format(L"%d", processData.dwParentPID);
        vecRowData.push_back(strData);
    }
    if(m_flagSet & FlagProcessHandle)
    {
        strData.Format(L"%d", processData.dwHandleCount);
        vecRowData.push_back(strData);
    }
    if(m_flagSet & FlagProcessThread)
    {
        strData.Format(L"%d", processData.dwThreadCount);
        vecRowData.push_back(strData);
    }
    if(m_flagSet & FlagProcessSession)
    {
        strData.Format(L"%d", processData.dwSessionID);
        vecRowData.push_back(strData);
    }
}

void KBkwinDemoDlg::LoadHeaderData(std::vector<CString>& vecHeaderData)
{

    vecHeaderData.push_back(L"进程名称");
    vecHeaderData.push_back(L"进程全路径");
    vecHeaderData.push_back(L"进程ID");
    vecHeaderData.push_back(L"CPU占用");
    vecHeaderData.push_back(L"内存占用");

    if(m_flagSet & FlagProcessUser)
    {
        vecHeaderData.push_back(L"用户名");
    }
    if(m_flagSet & FlagProcessParent)
    {
        vecHeaderData.push_back(L"父进程PID");
    }
    if(m_flagSet & FlagProcessHandle)
    {
        vecHeaderData.push_back(L"句柄数");
    }
    if(m_flagSet & FlagProcessThread)
    {
        vecHeaderData.push_back(L"线程数");
    }
    if(m_flagSet & FlagProcessSession)
    {
        vecHeaderData.push_back(L"会话ID");
    }
}

void KBkwinDemoDlg::CreateListHeader()
{
    CStringA strHeaderXml;
    int nColWidth = LIST_ITEM_WIDTH / (m_SelectSum + 2);
    TiXmlDocument* pXmlDoc = new TiXmlDocument;

    strHeaderXml.Append("<dlg>\r\n");
    
    std::vector<CString>::iterator iter = m_vecHeaderData.begin();
    int iDex = 0;
    for(iter; iter != m_vecHeaderData.end(); ++iter, ++iDex)
    {
        CStringA strItem;
        int nIndex = iDex > 1?(iDex+2):iDex;
        int nLeft = nIndex * nColWidth;
        int nRight = iDex == 1?4*nColWidth:nColWidth + nLeft;
        strItem.Format("<text pos=\"%d,0,%d,-0\"  show=\"1\" class=\"text_center\">%s</text>\r\n", nLeft, nRight, CT2A(*iter));
        strHeaderXml.Append(strItem);
    }

     strHeaderXml.Append("</dlg>\r\n");

    m_pTitleDlg = (CBkDialog*)FindChildByCmdID(IDC_DLG_HEADER);

    pXmlDoc->Parse(strHeaderXml, NULL, TIXML_ENCODING_UTF8);
    m_pTitleDlg->AppendChildEx(pXmlDoc->RootElement(), TRUE);
    m_pTitleDlg->NotifyInvalidate();
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

	
	// AddTableTitleToListWnd();  	//  第一列的行头 添加
    CreateListHeader();

	// TODO 第一列的行头在 这里添加
	int nIndex = 0;
	for (map<DWORD, SetProcessInfo>::iterator it= m_mapProInfo.begin();
		it != m_mapProInfo.end(); ++it)
	{
		AddListItem(it->second, nIndex++);
	}

	UpdateLayoutList(IDC_LIST_PROC);
}

BOOL KBkwinDemoDlg::AddListItem( SetProcessInfo & data , int nIndex)
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

		KTinyXmlRememberPos(tinyXml);
		bRetCode = CreateListItemXml(data, tinyXml, nIndex);

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

BOOL KBkwinDemoDlg::CreateListItemXml(SetProcessInfo & data, KTinyXml tinyXml, int nIndex )
{
	BOOL bReturn = FALSE;
	KCreateXmlElementFunc XmlElFunc(tinyXml);

	bReturn = XmlElFunc.AddTinyChild("listitem", 100 + nIndex, NULL, NULL, NULL, NULL, "1");
	if (FALSE == bReturn) goto Exit0;


	bReturn = XmlElFunc().Write("height", ITEM_HEIGHT);
	if (FALSE == bReturn) goto Exit0;

	bReturn = XmlElFunc().Write("class", "listitem_class");
	if (FALSE == bReturn) goto Exit0;

	bReturn = XmlElFunc.AddTinyChild("dlg", NULL,  "0,0,-0,-0", NULL, NULL, NULL, "1");
	if (FALSE == bReturn) goto Exit0;

    AddListItemChilds(data, XmlElFunc);

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

void KBkwinDemoDlg::UpdateConfig()
{
	m_flagSet = ConfigUtilInst.GetFlagSet();
	m_SelectSum = ConfigUtilInst.GetSelectSum();
	//m_nItemWidth = ITEM_WIDTH * (ALL_ITEM_NUM / m_SelectSum); 
}

BOOL KBkwinDemoDlg::AddTableTitleToListWnd()
{
// 	BOOL bReturn = FALSE;
// 	KTinyXml tinyXml;
// 
// 	do 
// 	{
// 		BOOL bRetCode = FALSE;
// 
// 		if (NULL == tinyXml.Open("Root", TRUE))
// 		{
// 			break;
// 		}
// 
// 		if (TRUE)
// 		{
// 			KTinyXmlRememberPos(tinyXml);
// 			bRetCode = CreateTitleListItemXml(data, tinyXml, nIndex);
// 		}
// 
// 		if (!bRetCode || !tinyXml.FirstChild())
// 		{
// 			goto Exit0;
// 		}
// 		CStringA strXml = tinyXml.ToXml();
// 		int nListItem = AppendListItem(IDC_LIST_PROC, tinyXml.GetElement(), -1, FALSE);
// 		if (nListItem == -1) goto Exit0;
// 
// 		bReturn = TRUE;
// 
// 	}while(FALSE);
// 
// Exit0:
// 	return bReturn;
}

// BOOL KBkwinDemoDlg::CreateTextItemXml(SetProcessInfo & data, KTinyXml tinyXml, unsigned int flag )
// {
// 	int nwidth = m_nItemWidth ; //可变, 根据选了多少个长度
// 	CString pos;
// 	int posFirstLeft;
// 	int posFirstRight;
// 	CString str;
// 
// 	BOOL bReturn = FALSE;
// 	CStringA StrPos;
// 	KCreateXmlElementFunc XmlElFunc(tinyXml);
// 
// 	bReturn = XmlElFunc.AddTinyChild("listitem", 0, NULL, NULL, NULL, NULL, "1");
// 	if (FALSE == bReturn) goto Exit0;
// 
// 
// 	bReturn = XmlElFunc().Write("height", ITEM_HEIGHT);
// 	if (FALSE == bReturn) goto Exit0;
// 
// 	bReturn = XmlElFunc().Write("class", "listitem_class");
// 	if (FALSE == bReturn) goto Exit0;
// 
// 	bReturn = XmlElFunc.AddTinyChild("dlg", NULL,  "0,0,-0,-0", NULL, NULL, NULL, "1");
// 	if (FALSE == bReturn) goto Exit0;
// 
// 	posFirstLeft = 0;
// 	posFirstRight = posFirstLeft + nwidth;
// 	pos.AppendFormat(L"%d,0,%d,-0", posFirstLeft, posFirstRight);
// 	bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_KEY, CT2A(pos), NULL, NULL, "text_center");
// 	if (FALSE == bReturn) return bReturn;
// 	XmlElFunc().Write("crtext", "999999");
// 	XmlElFunc().WriteText(data.strProcessName);
// 
// 	pos.Empty();
// 	posFirstLeft = posFirstRight;
// 	posFirstRight = posFirstLeft + nwidth;
// 	pos.AppendFormat(L"%d,0,%d,-0", posFirstLeft, posFirstRight);
// 	bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_KEY, CT2A(pos), NULL, NULL, "text_center");
// 	if (FALSE == bReturn) return bReturn;
// 	XmlElFunc().Write("crtext", "999999");
// 	str.Empty();
// 	str.AppendFormat(L"%d", data.dwParentPID);
// 	XmlElFunc().WriteText(str);
// 
// Exit0:
// 	return bReturn;
// }

void KBkwinDemoDlg::OnActivate(KActor* pActor)
{
	if(pActor == &m_atorLogic)
	{
		StartProLogic();
	}
}


void KBkwinDemoDlg::StartProLogic()
{
	while(TRUE)
	{
		int object = m_evtOutputAllProcess.WaitThreeEvent(m_evtOpenFilePos, m_evtOpenFileAtt, FALSE, 100);
		switch (object)
		{
		case KEvent::WAIT_RESULT_OBJECT_0:
			// 导出所有文件
			break;
		case KEvent::WAIT_RESULT_OBJECT_1:
			// 打开文件位置
			break;
		case KEvent::WAIT_RESULT_OBJECT_2:
			// 定位文件属性
			break;
		case KEvent::WAIT_RESULT_TIME_OUT:
		default:
			break;
		}
		::Sleep(100);
	}
}

int KBkwinDemoDlg::OnBtnKillProcess()
{
	OutputAllProcess();
	map<DWORD, SetProcessInfo>::iterator iter;
	iter = m_mapProInfo.find(m_nPid);
	if (iter!= m_mapProInfo.end())
	{
		KillProcessById(m_nPid);
		m_mapProInfo.erase(iter);
		UpdateWindowsUI();
	}
	return 1;
}

int   KBkwinDemoDlg::KillProcessById(int pid)
{
	int id = 0;
	id = pid;
	HANDLE hProcess=NULL;
	hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,id);	//打开目标进程
	if (hProcess==NULL) {
		wprintf(L"\nOpen Process fAiled:%d\n",GetLastError());
		return -1;
	}
	DWORD ret=TerminateProcess(hProcess,0);	//结束目标进程
	if(ret==0) {
		wprintf(L"%d",GetLastError());
		return -1;
	}
	return 1;
}

int   KBkwinDemoDlg::OutputAllProcess()
{
	ofstream output;
	CString file_name = L"processinfo.txt"; 
	output.open(file_name, std::ofstream::binary);
	for (map<DWORD, SetProcessInfo>::iterator iter = m_mapProInfo.begin(); iter!=m_mapProInfo.end(); ++iter )
	{
		SetProcessInfo n = iter->second;
		
		char szBuf[PROCESSINFO_LEN] = { 0 };
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s: %s\n", "strProcessNane", CT2A(n.strProcessName,CP_UTF8));
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %s\n", "strProcessFullPath", CT2A(n.strProcessFullPath,CP_UTF8));
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %d\n", "dwProcessID", n.dwParentPID);
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %d\n", "dwCpuUsage", n.dwCpuUsage);
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %s\n", "strMemoryUser", CT2A(n.strMemoryUser,CP_UTF8));
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %s\n", "strUserName", CT2A(n.strUserName,CP_UTF8));
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %d\n", "dwParentPID", n.dwParentPID);
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %d\n", "dwHandleCount", n.dwHandleCount);
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %d\n", "dwThreadCount", n.dwThreadCount);
		output.write(szBuf, strlen(szBuf));

		memset(szBuf, 0,PROCESSINFO_LEN );
		sprintf_s(szBuf, PROCESSINFO_LEN, "%s %d\n", "dwThreadCount", n.dwThreadCount);
		output.write(szBuf, strlen(szBuf));

	}
	output.close();
	return 0;
}
