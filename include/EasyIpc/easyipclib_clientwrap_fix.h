#pragma once
#include "EasyIpcInterface.h"

template<class Interface>
inline Interface* GetEasyIpc(void)
{
    LPVOID __stdcall DllQueryInterface(const IID&);

    Interface* pInterface = (Interface*)DllQueryInterface(__uuidof(Interface));  

    return pInterface;
}

class KEasyIpcLibClientWrap
{
public:
	KEasyIpcLibClientWrap(LPCWSTR pszInterface)
	{
		m_pIpcClient = GetEasyIpc<easyipc::IEasyIpcClient>(); 
		m_pParam     = GetEasyIpc<easyipc::IEasyIpcBundle>(); 
		m_pResult    = GetEasyIpc<easyipc::IEasyIpcBundle>(); 
		Connect(pszInterface); 
	}

	~KEasyIpcLibClientWrap(void)
	{
		if (m_pIpcClient)
		{
			m_pIpcClient->Disconnect(); 
			m_pIpcClient->Release(); 
		}

		if (m_pParam)
			m_pParam->Release(); 

		if (m_pResult)
			m_pResult->Release(); 
	}

	int Connect(LPCWSTR pszInterface)
	{
		int iRet = -1; 
		if (m_pIpcClient)
			iRet = m_pIpcClient->Connect(pszInterface); 

		return iRet; 
	}

	int Disconnect(void)
	{
		int iRet = -1; 
		if (m_pIpcClient)
			iRet = m_pIpcClient->Disconnect(); 

		return iRet; 
	}

	BOOL AddParam(LPCWSTR pszName, int nValue)
	{
		int iRet = -1; 
		if (m_pParam)
			iRet = m_pParam->PushInt(pszName, nValue); 

		return iRet == 0 ? TRUE : FALSE; 
	}

	BOOL AddParam(LPCWSTR pszName, LPCWSTR pszValue)
	{
		int iRet = -1; 
		if (m_pParam)
			iRet = m_pParam->PushString(pszName, pszValue); 

		return iRet == 0 ? TRUE : FALSE; 
	}

	BOOL AddParam(LPCWSTR pszName, LPCVOID pBuf, int nSize)
	{
		int iRet = -1; 
		if (m_pParam)
			iRet = m_pParam->PushBinary(pszName, pBuf, nSize); 

		return iRet == 0 ? TRUE : FALSE; 
	}

	BOOL HasValue(LPCWSTR pszName)
	{
		BOOL bRet = FALSE; 
		if (m_pResult)
			bRet = m_pResult->HasValue(pszName); 

		return bRet; 
	}

	int GetInt(LPCWSTR pszName)
	{
		int iRet = -1; 
		if (m_pResult)
			iRet = m_pResult->GetInt(pszName, 0); 

		return iRet; 
	}

	LPCWSTR GetString(LPCWSTR pszName)
	{
		LPCWSTR pszRet = NULL; 
		if (m_pResult)
			pszRet = m_pResult->GetString(pszName, NULL); 

		return pszRet; 
	}

	BOOL GetBinary(LPCWSTR pszName, CStringA& strBin)
	{
		BOOL bRet = FALSE; 
		if (m_pResult)
		{
			int iRet = m_pResult->GetBinary(pszName, NULL, 0); 
			if (iRet > 0)
			{
				m_pResult->GetBinary(pszName, strBin.GetBuffer(iRet), iRet); 
				strBin.ReleaseBuffer(iRet); 
				bRet = TRUE; 
			}
		}

		return bRet; 
	}

	int Call(LPCWSTR pszFunction)
	{
		int iRet = -1; 
		if (!m_pIpcClient || !m_pParam || !m_pResult)
			goto Exit0; 

		m_pResult->Clear(); 
		iRet = m_pIpcClient->CallServer(pszFunction, m_pParam, m_pResult); 
Exit0:
		if (m_pParam)
			m_pParam->Clear(); 

		return iRet; 
	}

private:
	easyipc::IEasyIpcClient* m_pIpcClient; 
	easyipc::IEasyIpcBundle* m_pParam; 
	easyipc::IEasyIpcBundle* m_pResult; 
};
