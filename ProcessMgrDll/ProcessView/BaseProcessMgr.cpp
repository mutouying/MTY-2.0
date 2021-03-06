#include "StdAfx.h"
#include "BaseProcessMgr.h"

KBaseProcessMgr::KBaseProcessMgr(void)
: m_pProcMonitor(NULL)
{
}

KBaseProcessMgr::~KBaseProcessMgr(void)
{
}

HRESULT __stdcall KBaseProcessMgr::Init()
{
    m_pProcMonitor = new ProcessMonitor;
    m_pProcMonitor->Init();

    return S_OK;
}

HRESULT __stdcall KBaseProcessMgr::UnInit()
{
    m_pProcMonitor->UnInit();
    delete m_pProcMonitor;
    m_pProcMonitor = NULL;

    return S_OK;
}

HRESULT __stdcall KBaseProcessMgr::Drop()
{
    delete this;
    return S_OK;
}

HRESULT __stdcall KBaseProcessMgr::QueryInterface(IN REFIID riid, OUT void **ppvObject)
{
   return S_OK;
}

ULONG	STDMETHODCALLTYPE KBaseProcessMgr::AddRef(void)
{
     return S_OK;
}

ULONG	STDMETHODCALLTYPE KBaseProcessMgr::Release(void)
{
       return S_OK;
}
