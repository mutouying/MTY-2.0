#pragma once

#include <map>
#include <set>

#include "Actor/KActor.h"
#include "framework/KEvent.h"
#include "framework/KLocker.h"
#include "ProcessInfo.h"
#include "EasyIpc\easyipcclientwrap.h"

#define MONITOR_SLEEP_INTER 100

class ProcessMonitor:public IActorCallBack
{
public:
    ProcessMonitor(void);
    ~ProcessMonitor(void);

public:
    void Init();
    void UnInit();

private:
    void GetAllProcessInfo();
    void StartMonitor();
    DWORD GetProcessNum();
    BOOL BeginEnumProcess();
    void FirstLoadProcess();
    BOOL CheckNewProcess(HANDLE hProcess, DWORD dwProcessID);
    void CheckProcessChange(DWORD dwProcessID);
    void CheckProcessExit();

private:
    virtual void   OnActivate(KActor* pActor);

private:
    void NotifyLoadData();
    void NotifyLoadEnd();
    void NotifyDataChange(emProcessChangeType emType, ProcessInfo* pProcInfo);
    void IpcAddProcParam(KEasyIpcClientWrap& client, ProcessInfo* pProcInfo);

private:
    BOOL m_bInit;
    BOOL m_bFirstIn;
    KEvent m_evtExit;
    DWORD m_dwInterNal;
    DWORD m_dwProcessNum;

    /*std::set<ProcessInfo*> m_setProcInfo;
    std::set<DWORD>    m_setProcId;*/
    std::map<DWORD, ProcessInfo*> m_mapProcessInfo;
    std::set<DWORD>    m_setProcId;

    KActor m_atorProcMonitor;
    std::map<HANDLE, ProcessInfo*> m_mapProcess;

};
