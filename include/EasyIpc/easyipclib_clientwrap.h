/* -------------------------------------------------------------------------
// �ļ���      :	easyipc/EasyIpcClientWrap.h
// ������      :	dingping
// ����ʱ��    :	2013/4/9 ���ڶ� 17:07:55
// ��������    :	Easy IPC �ͻ��˰�װ��
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __EASYIPC_EASYIPCCLIENTWRAP_H__
#define __EASYIPC_EASYIPCCLIENTWRAP_H__

#include "EasyIpcCommonWrap.h"
#include "include\easyipc_new\easyipcinterface.h"
#pragma comment(lib, "keasyipc.lib")
LPVOID __stdcall DllQueryInterface(const IID& iid);

// -------------------------------------------------------------------------

/***********************************************************************
IPC ע��ʾ����

һ.ipc�����ʹ�÷�����
	1.IPC���̶���
	interface ICalculate
	{
		virtual int Sum(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle* pResult) = 0; 
	}; 

	struct KCalculate : public ICalculate
	{
		int Sum(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle* pResult)
		{
			int a = pParam->GetInt(L"a", 0); 
			int b = pParam->GetInt(L"b", 0); 
			pResult->PushInt(L"result", a + b); 
			return 0; 
		}
	}

	2.IPCʵ������
	KCalculate ipcCalc;

	3.ע��IPC����
	EASYIPC_REGISTER_METHOD(ICalculate, Sum, &ipcCalc)

	4.������ֹͣipc����
	int main()
	{
		// ��ʼIPC �ӿڣ�EASYIPC_START_SERVER ��Ҫ�� EASYIPC_REGISTER_METHOD(...) ֮�����
		EASYIPC_START_SERVER(); 

		// ...

		// ֹͣIPC �ӿ�
		EASYIPC_STOP_SERVER(); 
	}

	
��.ipc�ͻ��˵��÷���
	void CallSum(void)
	{
		KEasyIpcClientWrap ipcClient(L"KCalculate"); 

		ipcClient.AddParam(L"a", 1); 
		ipcClient.AddParam(L"b", 2); 
		int nRetCode = ipcClient.Call(L"Run"); 
		if (nRetCode == 0)
		{
			int sum = ipcClient.GetInt(L"result"); 
		}
	}


************************************************************************/

inline easyipc::IEasyIpcClient* GetEasyIpcClient(void)
{
	//return KEasyIpcWrap().GetInterface<easyipc::IEasyIpcClient>(); 
    return (easyipc::IEasyIpcClient*)DllQueryInterface(__uuidof(easyipc::IEasyIpcClient));
}

class KEasyIpcClientWrap
{
public:
	KEasyIpcClientWrap(LPCWSTR pszInterface = NULL)
	{
		m_pIpcClient = GetEasyIpcClient(); 
		Connect(pszInterface); 
	}

	~KEasyIpcClientWrap(void)
	{
		if (m_pIpcClient)
		{
			m_pIpcClient->Disconnect(); 
			m_pIpcClient->Release(); 
		}
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
		return m_bundleParam.PushInt(pszName, nValue); 
	}

	BOOL AddParam(LPCWSTR pszName, LPCWSTR pszValue)
	{
		return m_bundleParam.PushString(pszName, pszValue); 
	}

	BOOL AddParam(LPCWSTR pszName, LPCVOID pBuf, int nSize)
	{
		return m_bundleParam.PushBinary(pszName, pBuf, nSize); 
	}

	BOOL AddParam(easyipc::IEasyIpcBundle* pBundle)
	{
		if (pBundle && m_bundleParam)
			pBundle->CopyTo(m_bundleParam); 

		return TRUE; 
	}

	BOOL HasValue(LPCWSTR pszName)
	{
		return m_bundleResult.HasValue(pszName); 
	}

	int GetInt(LPCWSTR pszName)
	{
		return m_bundleResult.GetInt(pszName); 
	}

	LPCWSTR GetString(LPCWSTR pszName)
	{
		return m_bundleResult.GetString(pszName); 
	}

	BOOL GetBinary(LPCWSTR pszName, CStringA& strBin)
	{
		return m_bundleResult.GetBinary(pszName, strBin); 
	}

	int Call(LPCWSTR pszFunction)
	{
		int iRet = -1; 
		if (!m_pIpcClient || !m_bundleParam || !m_bundleResult)
			goto Exit0; 

		m_bundleResult.Clear(); 
		iRet = m_pIpcClient->CallServer(pszFunction, m_bundleParam, m_bundleResult); 
Exit0:
		if (m_bundleParam)
			m_bundleParam.Clear(); 

		return iRet; 
	}

	BOOL IsServerActive(void)
	{
		if (m_pIpcClient)
			return m_pIpcClient->IsServerActive(); 

		return FALSE; 
	}

protected:
	easyipc::IEasyIpcClient* m_pIpcClient; 
	KEasyIpcBundleWrap       m_bundleParam; 
	KEasyIpcBundleWrap       m_bundleResult; 
};


// -------------------------------------------------------------------------
// $Log: $

#endif /* __EASYIPC_EASYIPCCLIENTWRAP_H__ */
