#include "StdAfx.h"
#include "ProcessInfo.h"
#include <TlHelp32.h>


ProcessInfo::ProcessInfo(void):
m_hProcess(NULL)
, m_pPrcoInfo(NULL)
, m_dwProcPID(0)
, m_bDataChange(FALSE)
, m_bFirstInit(TRUE)
, _last_system_time(0)
, _last_time(0)
, m_NtQueryInformationProcess(NULL)
{
}

ProcessInfo::~ProcessInfo(void)
{
}

void ProcessInfo::Init(HANDLE hProcess, DWORD dwProcessID)
{
    m_hProcess = hProcess;
    m_dwProcPID = dwProcessID;
    m_pPrcoInfo = new SetProcessInfo;
    m_bFirstInit = FALSE;

    InitNTQueryInterface();
}

void ProcessInfo::UnInit()
{
    if(m_hProcess)
    {
        CloseHandle(m_hProcess);
    }

    if(m_pPrcoInfo)
    {
        delete m_pPrcoInfo;
        m_pPrcoInfo = NULL;
    }
}

void ProcessInfo::GetProcessInfo()
{
    if(m_pPrcoInfo == NULL)
         return;

    pSetProcessInfo item = new SetProcessInfo;

    item->strProcessFullPath = GetProcessFullPath();
    item->strProcessName = GetProcessName();
    item->dwProcessID = GetProcessID();
    item->dwCpuUsage = GetCpuUsage();
    item->strMemoryUser = GetMemoryUse();
    item->strUserName = GetUserName();
    item->dwParentPID = GetParentPID();
    item->dwHandleCount = GetHandleNum();
    item->dwThreadCount = GetThreadNum();
    item->dwSessionID = GetSessionID();

    CheckDataChange(item);
}

BOOL ProcessInfo::IsDataChange()
{
    return m_bDataChange;
}

CString ProcessInfo::GetProcessName()
{
    CString strFullPath = GetProcessFullPath();

    if(strFullPath.IsEmpty())
    {
       return strFullPath;
    }

    int nPos = strFullPath.ReverseFind('\\');
    strFullPath = strFullPath.Right(strFullPath.GetLength() - nPos - 1);

    return strFullPath;
}

CString ProcessInfo::GetProcessFullPath()
{
    if(m_dwProcPID == SYSTEM_PID)
        return "";

    TCHAR lpszFullPath[MAX_PATH] = {0};

     GetModuleFileNameEx(m_hProcess, NULL, lpszFullPath, MAX_PATH);

    return lpszFullPath;
}

DWORD ProcessInfo::GetProcessID()
{
    return m_dwProcPID;
}

DWORD ProcessInfo::GetCpuUsage()
{
    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    ULONGLONG system_time;
    ULONGLONG time;
    LONGLONG system_time_delta;
    LONGLONG time_delta;

    DWORD exitcode;

    float cpu = -1;

    GetSystemTimeAsFileTime(&now);

    //计算占用CPU的百分比
    if (!GetProcessTimes(m_hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
    {
        return -1;
    }
    system_time = (FileTime2Utc(&kernel_time) + FileTime2Utc(&user_time))
        / GetProcessNum();
    time = FileTime2Utc(&now);

    //判断是否为首次计算
    if ((_last_system_time == 0) || (_last_time == 0))
    {
        _last_system_time = system_time;
        _last_time = time;
        return 0;
    }

    system_time_delta = system_time - _last_system_time;
    time_delta = time - _last_time;

    if (time_delta == 0) {
        return -1;
    }

    cpu = (float)system_time_delta * 100 / (float)time_delta;
    _last_system_time = system_time;
    _last_time = time;
    return cpu;
}

int ProcessInfo::GetProcessNum()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
}


BOOL ProcessInfo::InitNTQueryInterface()
{
    BOOL hResult = FALSE;

    HMODULE hNtdllModule = ::LoadLibrary(TEXT("ntdll.dll"));
    if (!hNtdllModule)
        return hResult;

    m_NtQueryInformationProcess = (PROCNTQSIP)::GetProcAddress(hNtdllModule, "NtQueryInformationProcess");
    if(m_NtQueryInformationProcess)
        hResult = TRUE;

    return hResult;
}

void ProcessInfo::CheckDataChange(pSetProcessInfo item)
{
    if(!m_bFirstInit)
    {
        if(m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            || m_pPrcoInfo->strProcessFullPath != item->strProcessFullPath
            )
        {
            m_bDataChange = TRUE;
            delete m_pPrcoInfo;
            m_pPrcoInfo = item;
        }
        else
        {
            m_bDataChange = FALSE;
            delete item;
        }
    }
}

LONGLONG ProcessInfo::FileTime2Utc(const FILETIME* ftime)
{
    LARGE_INTEGER li;

    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

CString ProcessInfo::GetMemoryUse()
{
    PROCESS_MEMORY_COUNTERS pmc;
    ::GetProcessMemoryInfo(m_hProcess, &pmc, sizeof(pmc));

    CString strMemSize;
    SIZE_T sMemSize = pmc.WorkingSetSize;
    if(sMemSize >= 1024 * 1024)
    {
        strMemSize.Format(L"%0.2f MB", (float)sMemSize / (1024 * 1024));
    }
    else if(sMemSize > 1024)
    {
        strMemSize.Format(L"%0.2f KB", (float)sMemSize / (1024));
    }
    else
    {
        strMemSize.Format(L"%0.2f B", (float)sMemSize);
    }

    return strMemSize;
}

CString ProcessInfo::GetUserName()
{
    TCHAR strBuffer[256] ={0};
    DWORD dwSize = 256;
    ::GetUserName(strBuffer,&dwSize);

    return strBuffer;
}

DWORD ProcessInfo::GetParentPID()
{
    NTSTATUS    status = STATUS_SUCCESS;
    DWORD dwParentPID = DWORD(-1);
    PROCESS_BASIC_INFORMATION pbi = { 0 };


    if (!m_NtQueryInformationProcess)
        return dwParentPID;

    status = m_NtQueryInformationProcess(m_hProcess, ProcessBasicInformation, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
    if(status == STATUS_SUCCESS)
        dwParentPID = DWORD(pbi.Reserved3);

    return dwParentPID;
}

DWORD ProcessInfo::GetHandleNum()
{
    LONG lRes = -1;
    DWORD dwErrRet = 0;
    DWORD dwRetSize = 0;
    DWORD dwHandleNum = 0;

    lRes = m_NtQueryInformationProcess(m_hProcess, ProcessBasicInformation, &dwHandleNum, sizeof(DWORD), &dwRetSize);
    if(lRes < 0)
        return dwErrRet;

    return dwHandleNum;
}

DWORD ProcessInfo::GetThreadNum()
{
    DWORD dwThreadNum= 0;
    char Buff[9];
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return dwThreadNum;
    }
    BOOL bMore = ::Process32First(hProcessSnap,&pe32);

    HANDLE hProcess;

    while(bMore)
    {
        bMore = Process32Next(hProcessSnap,&pe32);
        dwThreadNum++;
    }

    return dwThreadNum;
}

DWORD ProcessInfo::GetSessionID()
{
    DWORD dwSessionId = -1;
    ProcessIdToSessionId(m_dwProcPID,&dwSessionId);

    return dwSessionId;
}
