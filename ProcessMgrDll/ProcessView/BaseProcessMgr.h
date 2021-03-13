#pragma once

#include "ProcessMgr/IBaseProcessMgr.h"
#include "ProcessMonitor.h"

#define PROCESS_MONITOR_INNTERNAL 100

class KBaseProcessMgr:public IBaseProcessMgr
{
public:
    KBaseProcessMgr(void);
    ~KBaseProcessMgr(void);

public:
    virtual HRESULT __stdcall Init();
    virtual HRESULT __stdcall UnInit();
    virtual HRESULT __stdcall Drop();

    virtual HRESULT __stdcall QueryInterface( IN REFIID riid, OUT void **ppvObject );
    virtual ULONG  __stdcall AddRef( void );
    virtual ULONG  __stdcall Release( void );

private:
    ProcessMonitor* m_pProcMonitor;
};
