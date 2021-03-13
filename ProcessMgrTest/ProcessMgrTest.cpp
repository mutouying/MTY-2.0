// ProcessMgrTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ProcessMgr/IBaseProcessMgr.h"
#include "ProcessTest.h"

BOOL TestProcessMgr()
{
    HMODULE    hModule  = NULL;

    typedef IUnknown* (__stdcall *CreateObj_fun)(const IID * iid);

    hModule = ::LoadLibraryW(L"E:\\WorkSpace\\MTY-2.0-GitTest\\bin\\ProcessMgrDll.dll");
    if (NULL == hModule)
    {
        return FALSE;
    }

    CreateObj_fun fn = (CreateObj_fun)GetProcAddress(hModule, "DllQueryInterface");
    if (NULL == fn)
    {
        return FALSE;
    }

    IBaseProcessMgr* pBootPop = (IBaseProcessMgr*)fn(&__uuidof(IBaseProcessMgr));
    if(pBootPop == NULL)
        return FALSE;

    pBootPop->Init();
}

int _tmain(int argc, _TCHAR* argv[])
{
    ProcessTest testor;
    testor.Init();

    if(!TestProcessMgr())
        return 0;

    while(true)
    {
        ::Sleep(1000);
    }
    ::system("pause");
	return 0;
}

