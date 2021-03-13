#include "StdAfx.h"
#include "ProcessMonitor.h"

ProcessMonitor::ProcessMonitor(void)
{
    m_dwInterNal = MONITOR_SLEEP_INTER;
    m_bFirstIn = TRUE;
    m_bInit = FALSE;
}

ProcessMonitor::~ProcessMonitor(void)
{
}

void ProcessMonitor::Init()
{
    m_evtExit.Create(NULL, TRUE, FALSE);

    EnableDebugPrivilege();

    if(m_bFirstIn)
    {
        GetAllProcessInfo();
        m_bFirstIn = FALSE;
    }

    m_atorProcMonitor.Startup("ProcessMonitor::acThread", this, false);
}

void ProcessMonitor::UnInit()
{
    if(m_evtExit.GetHandle())
    {
        m_evtExit.SetEvent();
        m_evtExit.Close();
    }

    ProcessInfo* pItem = NULL;
    std::map<DWORD, ProcessInfo*>::iterator   iter = m_mapProcessInfo.begin();
    for(iter; iter != m_mapProcessInfo.end(); ++iter)
    {
        pItem  = (*iter).second;
        if(pItem)
        {
            pItem->UnInit();
            delete pItem;
            pItem = NULL;
        }
    }

    m_atorProcMonitor.Stop();
}

void ProcessMonitor::GetAllProcessInfo()
{
    FirstLoadProcess();
}

void ProcessMonitor::StartMonitor()
{
    while(TRUE)
    {
        if(m_evtExit.Wait(10))
        {
            break;
        }

        BeginEnumProcess();

        ::Sleep(m_dwInterNal);
    }
}

DWORD ProcessMonitor::GetProcessNum()
{
    
}

BOOL ProcessMonitor::BeginEnumProcess()
{
    DWORD dwProcesses[1024] = {0};
    DWORD dwProcessNum = 0;
    HANDLE hProcess = NULL;
    BOOL bSucess = FALSE;
    ProcessInfo* pInfo = NULL;

    m_setProcId.clear();

    bSucess = ::EnumProcesses(dwProcesses, sizeof(dwProcesses), &dwProcessNum);
    if(!bSucess)
    {
        return bSucess;
    }

    m_dwProcessNum = dwProcessNum / sizeof(DWORD);
    for(DWORD i = 0; i < m_dwProcessNum; ++i)
    {
        if(dwProcesses[i] == 0  || dwProcesses[i] == 4)
            continue;

        hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcesses[i]);
        if(hProcess = NULL)
            continue;

        m_setProcId.insert(dwProcesses[i]);

        if(CheckNewProcess(hProcess, dwProcesses[i]))
        {
            continue;
        }
        CheckProcessChange(dwProcesses[i]);
    }

    CheckProcessExit();
}

void ProcessMonitor::FirstLoadProcess()
{
    DWORD dwProcesses[1024] = {0};
    DWORD dwProcessNum = 0;
    HANDLE hProcess = NULL;
    BOOL bSucess = FALSE;
    ProcessInfo* pInfo = NULL;

    bSucess = ::EnumProcesses(dwProcesses, sizeof(dwProcesses), &dwProcessNum);
    if(!bSucess)
    {
        return;
    }

    m_dwProcessNum = dwProcessNum / sizeof(DWORD);
    for(DWORD i = 0; i < m_dwProcessNum; ++i)
    {
        if(dwProcesses[i] == 0  || dwProcesses[i] == 4)
            continue;

        hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcesses[i]);
        if(hProcess == NULL)
        {
            LRESULT lRes = GetLastError();
            continue;
        }

        pInfo = new ProcessInfo;
        pInfo->Init(hProcess, dwProcesses[i]);
        pInfo->GetProcessInfo();

        m_mapProcessInfo[dwProcesses[i]] = pInfo;
    }

    NotifyLoadData();
    NotifyLoadEnd();
}

BOOL ProcessMonitor::CheckNewProcess(HANDLE hProcess, DWORD dwProcessID)
{
    BOOL bNewProcess = FALSE;

    if(m_mapProcessInfo.find(dwProcessID) == m_mapProcessInfo.end())
    {
        ProcessInfo* pInfo = new ProcessInfo;
        pInfo->Init(hProcess, dwProcessID);
        pInfo->GetProcessInfo();

        m_mapProcessInfo[dwProcessID] = pInfo;

        NotifyDataChange(type_process_new, pInfo, dwProcessID);
        bNewProcess = TRUE;
    }

    return bNewProcess;
}

void ProcessMonitor::CheckProcessChange(DWORD dwProcessID)
{
    std::map<DWORD, ProcessInfo*>::iterator iter = m_mapProcessInfo.find(dwProcessID);

    if(iter == m_mapProcessInfo.end())
    {
        return;
    }

    ProcessInfo* item = (*iter).second;
    item->GetProcessInfo();
    if(item->IsDataChange())
    {
        NotifyDataChange(type_process_change, item, dwProcessID);
    }
}

void ProcessMonitor::CheckProcessExit()
{
    std::map<DWORD, ProcessInfo*>::const_iterator   iter = m_mapProcessInfo.begin();
    for(iter; iter != m_mapProcessInfo.end(); ++iter)
    {
        if(m_setProcId.find((*iter).first) == m_setProcId.end())
        {
            NotifyDataChange(type_process_exit, (*iter).second, (*iter).first);
        }
    }
}

void ProcessMonitor::OnActivate(KActor* pActor)
{
    if(pActor == &m_atorProcMonitor)
    {
        StartMonitor();
    }
}

BOOL ProcessMonitor::EnableDebugPrivilege()
{
    /*HANDLE hToken;
    BOOL fOk=FALSE;
    if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount=1;
        LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);

        tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);

        fOk=(GetLastError()==ERROR_SUCCESS);
        CloseHandle(hToken);
    }
    return fOk;*/
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    TOKEN_PRIVILEGES oldtp;
    DWORD dwSize=sizeof(TOKEN_PRIVILEGES);
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        if (GetLastError()==ERROR_CALL_NOT_IMPLEMENTED) return true;
        else return false;
    }
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
        CloseHandle(hToken);
        return false;
    }
    ZeroMemory(&tp, sizeof(tp));
    tp.PrivilegeCount=1;
    tp.Privileges[0].Luid=luid;
    tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
    /* Adjust Token Privileges */
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &oldtp, &dwSize)) {
        CloseHandle(hToken);
        return false;
    }
    // close handles
    CloseHandle(hToken);
    return true;
}

void ProcessMonitor::NotifyLoadData()
{
    KEasyIpcClientWrap client(ipc_first_load_process);

    std::map<DWORD, ProcessInfo*>::const_iterator   iter = m_mapProcessInfo.begin();
    for(iter; iter != m_mapProcessInfo.end(); ++iter)
    {
        IpcAddProcParam(client, (*iter).second, (*iter).first);
        client.Call(L"FirstLoadProcess");
    }
}

void ProcessMonitor::NotifyLoadEnd()
{
    KEasyIpcClientWrap client(ipc_first_load_process_end);
    client.Call(L"FirstLoadEnd");
}

void ProcessMonitor::NotifyDataChange(emProcessChangeType emType, ProcessInfo* pProcInfo, DWORD dwProcessID)
{
    KEasyIpcClientWrap client(ipc_refresh_process);

    client.AddParam(L"ProcessChangeType", emType);
    IpcAddProcParam(client, pProcInfo, dwProcessID);

    client.Call(L"RefreshProcess");
}

void ProcessMonitor::IpcAddProcParam(KEasyIpcClientWrap& client, ProcessInfo* pProcInfo, DWORD dwProcessID)
{
    client.AddParam(L"dwProcessID", dwProcessID);

    client.AddParam(L"strProcessName", pProcInfo->m_pPrcoInfo->strProcessName);
    client.AddParam(L"strProcessFullPath", pProcInfo->m_pPrcoInfo->strProcessFullPath);
    client.AddParam(L"dwProcessID", pProcInfo->m_pPrcoInfo->dwProcessID);
    client.AddParam(L"dwCpuUsage", pProcInfo->m_pPrcoInfo->dwCpuUsage);
    client.AddParam(L"strMemoryUser", pProcInfo->m_pPrcoInfo->strMemoryUser);

    client.AddParam(L"strUserName", pProcInfo->m_pPrcoInfo->strUserName);
    client.AddParam(L"dwParentPID", pProcInfo->m_pPrcoInfo->dwParentPID);
    client.AddParam(L"dwHandleCount", pProcInfo->m_pPrcoInfo->dwHandleCount);
    client.AddParam(L"dwThreadCount", pProcInfo->m_pPrcoInfo->dwThreadCount);
    client.AddParam(L"dwSessionID", pProcInfo->m_pPrcoInfo->dwSessionID);
}
