#include "StdAfx.h"
#include "ProcessTest.h"

ProcessTest::ProcessTest(void)
{
}

ProcessTest::~ProcessTest(void)
{
}

void ProcessTest::Init()
{
    m_strIpcName1 = ipc_first_load_process;
    EASYIPC_REGISTER_METHODEX(m_strIpcName1, ProcessTest, FirstLoadProcess, this); 
    EASYIPC_START_INTERFACEEX(m_strIpcName1);

    m_strIpcName2 = ipc_first_load_process_end;
    EASYIPC_REGISTER_METHODEX(m_strIpcName2, ProcessTest, FirstLoadEnd, this); 
    EASYIPC_START_INTERFACEEX(m_strIpcName2);

    m_strIpcName3 = ipc_refresh_process;
    EASYIPC_REGISTER_METHODEX(m_strIpcName3, ProcessTest, RefreshProcess, this); 
    EASYIPC_START_INTERFACEEX(m_strIpcName3);
}

void ProcessTest::UnInit()
{
    EASYIPC_STOP_INTERFACEEX(m_strIpcName1); 
    EASYIPC_UNREGISTEREX(m_strIpcName1);

    EASYIPC_STOP_INTERFACEEX(m_strIpcName2); 
    EASYIPC_UNREGISTEREX(m_strIpcName1);

    EASYIPC_STOP_INTERFACEEX(m_strIpcName3); 
    EASYIPC_UNREGISTEREX(m_strIpcName1);
}

int ProcessTest::FirstLoadProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    KLocker locker(m_csNewItemNotify);

    pSetProcessInfo newItem = new SetProcessInfo;

    newItem->strProcessFullPath = pParam->GetString(L"strProcessFullPath", NULL);
    newItem->strProcessName = pParam->GetString(L"strProcessName", NULL);
    newItem->dwProcessID = pParam->GetInt(L"dwProcessID", 0);
    newItem->dwCpuUsage = pParam->GetInt(L"dwCpuUsage", 0);
    newItem->strMemoryUser = pParam->GetString(L"strMemoryUser", 0);
    newItem->strUserName = pParam->GetString(L"strUserName", 0);
    newItem->dwParentPID = pParam->GetInt(L"dwParentPID", 0);
    newItem->dwHandleCount = pParam->GetInt(L"dwHandleCount", 0);
    newItem->dwThreadCount = pParam->GetInt(L"dwThreadCount", 0);
    newItem->dwSessionID = pParam->GetInt(L"dwSessionID", 0);

    DWORD dwProcessId = pParam->GetInt(L"dwProcessID", 0);

    m_mapProcessInfo[dwProcessId] = newItem;

    return 0;
}

int ProcessTest::FirstLoadEnd(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    KLocker locker(m_csNewItemNotify);

    pSetProcessInfo newItem = NULL;

    std::map<DWORD, pSetProcessInfo>::const_iterator   iter = m_mapProcessInfo.begin();
    for(iter; iter != m_mapProcessInfo.end(); ++iter)
    {
        newItem = (*iter).second;
        if(newItem)
        {
            std::wcout << newItem->strProcessFullPath.GetBuffer(0) << "    ";
            std::wcout << newItem->strProcessName.GetBuffer(0) << "    ";
            std::cout << newItem->dwProcessID << "    ";
            std::cout << newItem->dwCpuUsage << "    ";
            std::wcout << newItem->strMemoryUser.GetBuffer(0) << "    ";
            std::wcout << newItem->strUserName.GetBuffer(0) << "    ";
            std::cout << newItem->dwParentPID << "    ";
            std::cout << newItem->dwHandleCount << "    ";
            std::cout << newItem->dwThreadCount << "    ";
            std::cout << newItem->dwSessionID << "    ";

            std::cout << std::endl;
        }
    }

    return 0;
}

int ProcessTest::RefreshProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*)
{
    return 0;
}
