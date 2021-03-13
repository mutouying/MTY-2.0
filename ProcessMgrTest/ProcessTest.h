#pragma once

#include "EasyIpc/easyipcserverwrap.h"
#include "ProcessMgr/ProcessDefineEx.h"
#include "framework/KLocker.h"
#include <map>
#include <iostream>

class ProcessTest
{
public:
    ProcessTest(void);
    ~ProcessTest(void);

    void Init();
    void UnInit();

private:
    int FirstLoadProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*);
    int FirstLoadEnd(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*);
    int RefreshProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*);

private:
    KCriticalSesion  m_csNewItemNotify;
    CString m_strIpcName1;
    CString m_strIpcName2;
    CString m_strIpcName3;

    std::map<DWORD, pSetProcessInfo> m_mapProcessInfo;
};
