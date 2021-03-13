#include "stdafx.h"
#include "KBkwinDemoDlg.h"

#include "ProcessMgr/IBaseProcessMgr.h"

void InitRes()
{
    #define IDR_DEF_COMMON_SKIN      60000
    #define IDR_DEF_COMMON_STYLE     60001
    #define IDR_DEF_COMMON_STRING    60002

    BkPngPool::Reload();
    BkString::Load(IDR_DEF_COMMON_STRING);
    BkSkin::LoadSkins(IDR_DEF_COMMON_SKIN);
    BkStyle::LoadStyles(IDR_DEF_COMMON_STYLE);
};

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

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE /*hPrevInstance*/,
                       LPTSTR lpCmdLine,
                       int nCmdShow)
{
    InitRes();

    //TestProcessMgr();

    KBkwinDemoDlg dlg;

    dlg.DoModal();

    return 0;
}