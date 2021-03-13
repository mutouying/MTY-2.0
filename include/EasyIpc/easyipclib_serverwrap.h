/* -------------------------------------------------------------------------
// �ļ���      :	easyipc/EasyIpcServerWrap.h
// ������      :	dingping
// ����ʱ��    :	2013/4/9 ���ڶ� 17:06:55
// ��������    :	Easy IPC ����˰�װ��
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __EASYIPC_EASYIPCSERVERWRAP_H__
#define __EASYIPC_EASYIPCSERVERWRAP_H__

#include "easyipccommonwrap.h"
#include <set>

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
		// ��ʼIPC �ӿڣ�EASYIPC_START_INTERFACE ��Ҫ�� EASYIPC_REGISTER_METHOD(...) ֮�����
		EASYIPC_START_INTERFACE(ICalculate); 

		// ...

		// ֹͣIPC �ӿ�
		EASYIPC_STOP_INTERFACE(ICalculate); 
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

#define EASYIPC_START_INTERFACE(interface) KEasyIpcServerWrap::Instance().Start(L#interface)

#define EASYIPC_STOP_INTERFACE(interface)  KEasyIpcServerWrap::Instance().Stop(L#interface)


#define EASYIPC_REGISTER_METHOD(class, method, obj)                                        \
	static int s_n##class##method = KEasyIpcServerWrap::Instance().Register(               \
		L#class, L#method, EasyIpcExecWrap<class>::CreateObj((obj), &class::method)); 

#define EASYIPC_UNREGISTER(class)                                                          \
	KEasyIpcServerWrap::Instance().Unregister(L#class)


inline easyipc::IEasyIpcServer* GetEasyIpcServer(void)
{
	//return KEasyIpcWrap().GetInterface<easyipc::IEasyIpcServer>(); 
    return (easyipc::IEasyIpcServer*)DllQueryInterface(__uuidof(easyipc::IEasyIpcServer));
}

class KEasyIpcServerWrap
{
public:
	static KEasyIpcServerWrap& Instance(void)
	{
		static KEasyIpcServerWrap ipcServer; 
		return ipcServer; 
	}

	int Register(LPCWSTR pszInterface, LPCWSTR pszFunction, easyipc::IEasyIpcExecObject* pObject)
	{
		int iRet = -1; 

		if (m_pIpcServer)
			iRet = m_pIpcServer->Register(pszInterface, pszFunction, pObject); 

		return iRet; 
	}

	int Unregister(LPCWSTR pszInterface)
	{
		int iRet = -1; 

		if (m_pIpcServer)
			iRet = m_pIpcServer->Unregister(pszInterface); 

		return iRet; 
	}

	int RegardEvent(LPCWSTR pszEventName, easyipc::IEasyEventObject* pObject)
	{
		int iRet = -1; 

		if (m_pIpcServer)
			iRet = m_pIpcServer->RegardEvent(pszEventName, pObject); 

		return iRet; 
	}

	int UnregardEvent(LPCWSTR pszEventName, easyipc::IEasyEventObject* pObject)
	{
		int iRet = -1; 

		if (m_pIpcServer)
			iRet = m_pIpcServer->UnregardEvent(pszEventName, pObject); 

		return iRet; 
	}

	int Start(LPCWSTR pszInterface)
	{
		if (m_pIpcServer)
			return m_pIpcServer->Start(pszInterface); 

		return -1; 
	}

	int Stop(LPCWSTR pszInterface)
	{
		if (m_pIpcServer)
			return m_pIpcServer->Stop(pszInterface); 

		return -1; 
	}

protected:
	KEasyIpcServerWrap(void)
	{
		m_pIpcServer = GetEasyIpcServer(); 
	}

	~KEasyIpcServerWrap(void)
	{
		if (m_pIpcServer)
			m_pIpcServer->Release(); 
	}

private:
	easyipc::IEasyIpcServer* m_pIpcServer; 
};

template<typename classtype>
class EasyIpcExecWrap : public easyipc::IEasyIpcExecObject
{
public:
	typedef int (classtype::*FnRunMethod)(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle* pResult); 

	static easyipc::IEasyIpcExecObject* CreateObj(classtype* pObject, FnRunMethod fnRunMethod)
	{
		if (!pObject || !fnRunMethod)
			return NULL; 

		return new EasyIpcExecWrap(pObject, fnRunMethod); 
	}

protected:
	EasyIpcExecWrap(classtype* pObject, FnRunMethod fnRunMethod)
		: pObj(pObject), fnRun(fnRunMethod)
	{
	}

	virtual void __stdcall Release(void) 
	{
		delete this; 
	}

	virtual int __stdcall Run(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle* pResult) 
	{
		return (pObj->*fnRun)(pParam, pResult); 
	}

private:
	classtype*   pObj; 
	FnRunMethod  fnRun; 
};


// -------------------------------------------------------------------------
// $Log: $

#endif /* __EASYIPC_EASYIPCSERVERWRAP_H__ */
