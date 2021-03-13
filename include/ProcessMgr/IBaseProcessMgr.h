#pragma once

[
    uuid("34C7275C-3140-4ab9-B4E4-CDE1497BE3EA")
]

interface IBaseProcessMgr:public IUnknown
{
public:

    virtual HRESULT __stdcall Init()	= 0;
    virtual HRESULT __stdcall UnInit()	= 0;
    virtual HRESULT __stdcall Drop() = 0;


};
