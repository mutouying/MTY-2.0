#include "StdAfx.h"

#include "ProcessMgr/IBaseProcessMgr.h"
#include "ProcessView/BaseProcessMgr.h"

// -------------------------------------------------------------------------

IUnknown* __stdcall DllQueryInterface(const IID& iid)
{
    if (__uuidof(IBaseProcessMgr) == iid)
    {
        return new KBaseProcessMgr; 
    }

    return NULL; 
}

