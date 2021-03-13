#include "StdAfx.h"

#include "ProcessMgr/IBaseProcessMgr.h"
#include "ProcessView/KBaseProcessMgr.h"

// -------------------------------------------------------------------------

IUnknown* __stdcall DllQueryInterface(const IID& iid)
{
    if (__uuidof(IBaseProcessMgr) == iid)
    {
        return new KBaseProcessMgr; 
    }

    return NULL; 
}

