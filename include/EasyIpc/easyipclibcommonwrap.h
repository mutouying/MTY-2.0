/* -------------------------------------------------------------------------
// 文件名      :	easyipc/EasyIpcLibCommonWrap.h
// 创建人      :	dingping
// 创建时间    :	2013/4/9 星期二 17:05:44
// 功能描述    :	Easy IPC 客户端/服务端公用包装类
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __EASYIPC_EASYIPCLIBCOMMONWRAP_H__
#define __EASYIPC_EASYIPCLIBCOMMONWRAP_H__

#include "easyipcinterface.h"
#include <shlwapi.h>
#include "include\framework\KRegister2.h"

#pragma comment(lib, "shlwapi.lib")

// -------------------------------------------------------------------------
extern LPVOID __stdcall DllQueryInterface(const IID& iid);

inline easyipc::IEasyIpcBundle* GetEasyIpcBundle(void)
{
    return (easyipc::IEasyIpcBundle*)DllQueryInterface(__uuidof(easyipc::IEasyIpcBundle));
}

inline easyipc::IEasyIpcDebug* GetEasyIpcDebug(void)
{
    return (easyipc::IEasyIpcDebug*)DllQueryInterface(__uuidof(easyipc::IEasyIpcDebug));
}

class KEasyIpcBundleWrap
{
public:
	KEasyIpcBundleWrap(LPCVOID pData = NULL, int nSize = 0)
	{
		m_pBundle = GetEasyIpcBundle(); 
		m_bManage = TRUE; 

		SetData(pData, nSize); 
	}

	KEasyIpcBundleWrap(easyipc::IEasyIpcBundle* pBundle, BOOL bManage = FALSE)
	{
		m_pBundle = pBundle; 
		m_bManage = bManage; 
	}

	~KEasyIpcBundleWrap()
	{
		if (m_bManage && m_pBundle)
		{
			m_pBundle->Release(); 
			m_pBundle = NULL; 
		}
	}

	operator easyipc::IEasyIpcBundle*(void)
	{
		return m_pBundle; 
	}

	BOOL PushInt(LPCWSTR pszName, int nValue)
	{
		int iRet = -1; 
		if (m_pBundle)
			iRet = m_pBundle->PushInt(pszName, nValue); 

		return iRet == 1 ? TRUE : FALSE; 
	}

	BOOL PushString(LPCWSTR pszName, LPCWSTR pszValue)
	{
		int iRet = -1; 
		if (m_pBundle)
			iRet = m_pBundle->PushString(pszName, pszValue); 

		return iRet == 1 ? TRUE : FALSE; 
	}

	BOOL PushBinary(LPCWSTR pszName, LPCVOID pBuf, int nSize)
	{
		int iRet = -1; 
		if (m_pBundle)
			iRet = m_pBundle->PushBinary(pszName, pBuf, nSize); 

		return iRet == 1 ? TRUE : FALSE; 
	}

	int GetInt(LPCWSTR pszName)
	{
		int iRet = 0; 
		if (m_pBundle)
			iRet = m_pBundle->GetInt(pszName, 0); 

		return iRet; 
	}

	LPCWSTR GetString(LPCWSTR pszName)
	{
		LPCWSTR pszRet = NULL; 
		if (m_pBundle)
			pszRet = m_pBundle->GetString(pszName, NULL); 

		return pszRet; 
	}

	BOOL GetBinary(LPCWSTR pszName, CStringA& strBin)
	{
		BOOL bRet = FALSE; 
		if (m_pBundle)
		{
			int iRet = m_pBundle->GetBinary(pszName, NULL, 0); 
			if (iRet > 0)
			{
				m_pBundle->GetBinary(pszName, strBin.GetBufferSetLength(iRet), iRet); 
				bRet = TRUE; 
			}
		}

		return bRet; 
	}

	BOOL HasValue(LPCWSTR pszName)
	{
		BOOL bRet = FALSE; 

		if (m_pBundle)
			bRet = m_pBundle->HasValue(pszName); 

		return bRet; 
	}

	void Clear(void)
	{
		if (m_pBundle)
			m_pBundle->Clear(); 
	}

	BOOL DeleteValue(LPCWSTR pszName)
	{
		if (m_pBundle)
			return m_pBundle->DeleteValue(pszName); 

		return FALSE; 
	}

	BOOL SetData(LPCVOID pData, int nSize)
	{
		BOOL bRet = FALSE; 
		if (!pData || !nSize)
			goto Exit0; 
		
		if (m_pBundle)
			bRet = m_pBundle->SetDataBuffer(pData, nSize) == 0 ? TRUE : FALSE; 

Exit0:
		return bRet; 
	}

	LPCSTR GetData(int& nSize)
	{
		if (!m_pBundle)
			return NULL; 

		nSize = m_pBundle->GetDataBuffer(NULL, 0); 
		if (nSize > 0)
			m_pBundle->GetDataBuffer(m_strData.GetBufferSetLength(nSize), nSize); 

		return m_strData; 
	}

	easyipc::IEasyIpcBundle* Detach(void)
	{
		m_bManage = FALSE; 
		easyipc::IEasyIpcBundle* pBundle = m_pBundle; 
		m_pBundle = NULL; 
		return pBundle; 
	}

	void CopyTo(easyipc::IEasyIpcBundle* pBundle)
	{
		if (pBundle && m_pBundle)
			m_pBundle->CopyTo(pBundle); 
	}

private:
	easyipc::IEasyIpcBundle* m_pBundle; 
	BOOL                     m_bManage; 
	CStringA                 m_strData; 
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __EASYIPC_EASYIPCLIBCOMMONWRAP_H__ */
