#include "stdafx.h"

#ifdef USE_MEMPOOL
#include "kbundle.h"
#include "include/objpool/ObjPool.h"

CObjPool<KBundle::DATA_ITEM_INT>   g_MemPoolInt;
CObjPool<KBundle::DATA_ITEM_INT64> g_MemPoolInt64;
CObjPool<KBundle::DATA_ITEM_DOUBLE> g_MemPoolDouble;
CObjPool<KBundle::DATA_ITEM_PTR>    g_MemPoolPtr;
CObjPool<KBundle::DATA_ITEM_STRING> g_MemPoolString;
CObjPool<KBundle::DATA_ITEM_BINARY> g_MemPoolBinary;
CObjPool<KBundle::DATA_ITEM_BUNDLE> g_MemPoolBundle;
CObjPool<KBundle::DATA_ITEM_LIST>   g_MemPoolList;
 
bool __stdcall KBundle::PushInt(__in LPCWSTR pszName, __in INT32 nValue)
{
    Erase(pszName); 

    DATA_ITEM_INT* val = g_MemPoolInt.Alloc();
    val->Assign(nValue);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushInt64(__in LPCWSTR pszName, __in INT64 nValue)
{
    Erase(pszName); 

    DATA_ITEM_INT64* val = g_MemPoolInt64.Alloc();
    val->Assign(nValue);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushDouble(__in LPCWSTR pszName, __in double dValue)
{
    Erase(pszName); 

    DATA_ITEM_DOUBLE* val = g_MemPoolDouble.Alloc();
    val->Assign(dValue);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushPointer(__in LPCWSTR pszName, __in LPVOID pData)
{
    Erase(pszName); 

    DATA_ITEM_PTR* val = g_MemPoolPtr.Alloc();
    val->Assign(pData);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushString(__in LPCWSTR pszName, __in LPCWSTR szString)
{
    Erase(pszName); 

    DATA_ITEM_STRING* val = g_MemPoolString.Alloc();
    val->Assign(szString);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushBinary(__in LPCWSTR pszName, __in LPCVOID pcvData, __in DWORD nSize)
{
    Erase(pszName); 

    DATA_ITEM_BINARY* val = g_MemPoolBinary.Alloc();
    val->Assign(pcvData, nSize);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushBundle(__in LPCWSTR pszName, __in const IBundle* pBundle)
{
    if (pBundle == this)
        return false; 

    DATA_ITEM_BUNDLE* pItem = NULL; 
    if (FindItem(pszName, enumDataType_Bundle, (IDATA_ITEM**)&pItem))
    {
        if (pItem->pBundle == pBundle)
            return true; 
    }

    Erase(pszName); 

    DATA_ITEM_BUNDLE* val = g_MemPoolBundle.Alloc();
    val->Assign(pBundle);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushList(__in LPCWSTR pszName, __in const IBundle*const* pBundle, __in DWORD nSize)
{
    DATA_ITEM_LIST* pItem = NULL; 
    if (FindItem(pszName, enumDataType_List, (IDATA_ITEM**)&pItem))
    {
        pItem->Assign(pBundle, nSize); 
        return true; 
    }

    Erase(pszName); 

    DATA_ITEM_LIST* val = g_MemPoolList.Alloc();
    val->Assign(pBundle, nSize);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

bool __stdcall KBundle::PushListItem(__in LPCWSTR pszName, __in const IBundle* pBundle)
{
    DATA_ITEM_LIST* pItem = NULL; 
    if (FindItem(pszName, enumDataType_List, (IDATA_ITEM**)&pItem))
    {
        pItem->Append(pBundle); 
        return true; 
    }

    Erase(pszName); 

    DATA_ITEM_LIST* val = g_MemPoolList.Alloc();
    val->Assign(&pBundle, 1);
    return m_mapData.insert(std::make_pair(pszName, val)).second;
}

void __stdcall KBundle::FreeItem(IDATA_ITEM* pItem)
{
    switch (pItem->GetType())
    {
    case enumDataType_Int:
        g_MemPoolInt.Free((DATA_ITEM_INT*)pItem); 
        break; 

    case enumDataType_Int64:
        g_MemPoolInt64.Free((DATA_ITEM_INT64*)pItem); 
        break; 

    case enumDataType_Double:
        g_MemPoolDouble.Free((DATA_ITEM_DOUBLE*)pItem); 
        break; 

    case enumDataType_Pointer:
        g_MemPoolPtr.Free((DATA_ITEM_PTR*)pItem); 
        break; 

    case enumDataType_String:
        g_MemPoolString.Free((DATA_ITEM_STRING*)pItem); 
        break; 

    case enumDataType_Bin:
        g_MemPoolBinary.Free((DATA_ITEM_BINARY*)pItem); 
        break; 

    case enumDataType_Bundle:
        g_MemPoolBundle.Free((DATA_ITEM_BUNDLE*)pItem); 
        break; 

    case enumDataType_List:
        g_MemPoolList.Free((DATA_ITEM_LIST*)pItem); 
        break;
    }
}

bool __stdcall KBundle::SetData(__in LPCVOID pcvData, __in DWORD nSize, __in bool bClear)

{
    wchar_t wzName[1024] = {0};
    bool bRet = false; 
    IDATA_ITEM* pItem = NULL; 
    const BUNDLE_HEADER& bundleHeader = *(const BUNDLE_HEADER*)pcvData; 

    if (bClear)
        Clear(); 

    if (!VerifyData(pcvData, nSize))
        goto Exit0; 

    LPCSTR pBegin = (LPCSTR)pcvData; 
    LPCSTR pEnd = pBegin + bundleHeader.nTotalSize; 
    for (pBegin += sizeof(BUNDLE_HEADER); pBegin < pEnd; )
    {
        const BUNDLE_ITEM_HEADER& itemHeader = *(const BUNDLE_ITEM_HEADER*)pBegin; 
        pBegin += sizeof(BUNDLE_ITEM_HEADER); 
        if (pBegin + itemHeader.nNameSize*sizeof(WCHAR) + itemHeader.nDataSize > pEnd)
            goto Exit0; 

        if (itemHeader.nNameSize >= 1024)
            goto Exit0; 

        wmemcpy(wzName, (LPCWSTR)pBegin, itemHeader.nNameSize);
        wzName[itemHeader.nNameSize] = 0;

        pBegin += itemHeader.nNameSize*sizeof(WCHAR); 

        switch (itemHeader.nType)
        {
        case enumDataType_Int:
            {
                pItem = g_MemPoolInt.Alloc(); 
            }
            break; 

        case enumDataType_Int64:
            {
                pItem = g_MemPoolInt64.Alloc();
            }
            break; 

        case enumDataType_Double:
            {
                pItem = g_MemPoolDouble.Alloc();
            }
            break; 

        case enumDataType_Pointer:
            {
                pItem = g_MemPoolPtr.Alloc();
            }
            break; 

        case enumDataType_String:
            {
                pItem = g_MemPoolString.Alloc();
            }
            break; 

        case enumDataType_Bin:
            {
                pItem = g_MemPoolBinary.Alloc();
            }
            break; 

        case enumDataType_Bundle:
            {
                pItem = g_MemPoolBundle.Alloc();
            }
            break; 

        case enumDataType_List:
            {
                pItem = g_MemPoolList.Alloc();
            }
            break; 

        default:
            ATLASSERT(0); 
            goto Exit0; 
        }

        if (!pItem->Deserialize(pBegin, itemHeader.nDataSize))
            goto Exit0; 

        pBegin += itemHeader.nDataSize; 
        if (!m_mapData.insert(std::make_pair(wzName, pItem)).second)
            goto Exit0; 

        pItem = NULL; 
    }

    bRet = true; 
Exit0:
    if (pItem)
        delete pItem; 

    return bRet; 
}
#endif