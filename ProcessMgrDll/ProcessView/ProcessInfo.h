#pragma once

#include "ProcessMgr/ProcessDefineEx.h"

#include <winternl.h> 
#include <ntstatus.h>

#define SYSTEM_PID 4

class ProcessInfo
{
public:
    ProcessInfo(void);
    ~ProcessInfo(void);

    typedef LONG (__stdcall *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);

    void Init(HANDLE hProcess, DWORD dwProcessID);
    void GetProcessInfo();
    BOOL IsDataChange();

private:
    CString GetProcessName();
    CString GetProcessFullPath();
    DWORD GetProcessID();
    DWORD GetCpuUsage();
    CString GetMemoryUse();

    CString GetUserName();
    DWORD GetParentPID();
    DWORD GetHandleNum();
    DWORD GetThreadNum();
    DWORD GetSessionID();

    ULONGLONG GetFileTimeSpanMiniSeconds(const FILETIME& ftSubtract, const FILETIME& ftSubtractBy);
    LONGLONG FileTime2Utc(const FILETIME* ftime);
    int GetProcessNum();

    BOOL InitNTQueryInterface();
    void CheckDataChange(pSetProcessInfo item);

public:
    HANDLE m_hProcess;
    BOOL m_bDataChange;
    BOOL m_bFirstInit;
    DWORD m_dwProcPID;
    pSetProcessInfo m_pPrcoInfo;

    LONGLONG _last_time;
    LONGLONG _last_system_time;

    PROCNTQSIP m_NtQueryInformationProcess;
};
