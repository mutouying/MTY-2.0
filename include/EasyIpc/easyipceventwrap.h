/* -------------------------------------------------------------------------
// �ļ���      :	easyipc/easyipceventwrap.h
// ������      :	dingping
// ����ʱ��    :	2013/9/4 ������ 20:28:46
// ��������    :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __EASYIPC_EASYIPCEVENTWRAP_H__
#define __EASYIPC_EASYIPCEVENTWRAP_H__

#include "EasyIpcClientWrap.h"
#include "EasyIpcServerWrap.h"

// -------------------------------------------------------------------------

/***********************************************************************
IPC �¼�ʹ��ʾ����

/******* �¼����� *******
// 1. �����¼�����
KEasyIpcEventTrigger eventTrigger; 

// 2. ����¼�����
eventTrigger.AddParam(L"a", L"abc"); 
eventTrigger.AddParam(L"b", 1); 

// 3. ͬ�������¼�����������ʱ���м��������յ��¼�֪ͨ
eventTrigger.TriggerEvent(L"abc"); 

eventTrigger.AddParam(L"a", L"1234"); 
// 4. �첽�����¼�����������ʱ�����߲�һ���յ�֪ͨ
eventTrigger.TriggerEvent(L"123", TRUE); 

// 5. ������첽�����¼�����Ҫ�ڶ����˳�ʱ�ȴ��¼��������
// ����������߿����޷��յ��¼�֪ͨ
eventTrigger.WaitEventPusherEnd(); 


/******* �¼����� *******
KCalculate ipcEvent; 

// �¼���Ӧ����ԭ��
void CLASSTYPE::OnEvent(LPCWSTR pszEventName, easyipc::IEasyIpcBundle* pParam); 

// ��ע�¼�          �¼���  �¼������ص�          �¼����ն���
EASYIPC_REGARD_EVENT(L"abc", &KCalculate::OnEvent, &ipcEvent); 
EASYIPC_REGARD_EVENT(L"123", &KCalculate::OnEvent, &ipcEvent); 

system("pause"); 

// ȡ���¼���ע
EASYIPC_UNREGARD_EVENT(L"abc", &KCalculate::OnEvent, &ipcEvent); 
EASYIPC_UNREGARD_EVENT(L"123", &KCalculate::OnEvent, &ipcEvent); 

************************************************************************/

#define EASYIPC_REGARD_EVENT(name, method, obj)                                            \
	KEasyIpcServerWrap::Instance().RegardEvent(                                            \
		name, CreateEventObj((obj), (method)))

#define EASYIPC_UNREGARD_EVENT(name, method, obj)                                          \
	KEasyIpcServerWrap::Instance().UnregardEvent(                                          \
		name, CreateEventObj((obj), (method)))

class KEasyIpcEventTrigger : public KEasyIpcClientWrap
{
public:
	/*
	@fn     TriggerEvent
	@brief  �����¼�
	@arg    pszEventName  �¼���
	@arg    bAsyn         �Ƿ��첽������һ��ͬ���������ɣ����Ƕ�����Ҫ���ر�ߡ�
	@                     �첽�����Ļ����˳�ʱ��ҪWaitEventPusherEnd�������п�
	@                     �ܵ��¼������ղ���֪ͨ
	@return 0���ɹ�������ʧ��
	@remark ������ͬ�������첽����ֻ�Ƿ��������ݱ㷵�أ�����ȴ������߻ص�����ִ����ɣ�
	@*/
	int TriggerEvent(LPCWSTR pszEventName, BOOL bAsyn = FALSE)
	{
		int iRet = -1; 
		easyipc::IEasyEventCallback* pCallback = NULL; 
		if (!m_pIpcClient || !m_bundleParam)
			goto Exit0; 

		m_bundleResult.Clear(); 
		iRet = m_pIpcClient->TriggerEvent(pszEventName, m_bundleParam, pCallback, bAsyn); 
Exit0:
		if (m_bundleParam)
			m_bundleParam.Clear(); 

		return iRet; 
	}

	/*
	@fn     TriggerEvent
	@brief  �����¼�
	@arg    pszEventName  �¼���
	@arg    pThis         �¼��ص�thisָ��
	@arg    fnRunMethod   �¼��ص�������ÿһ���¼���Ӧ����һ�λص�
	@arg    bAsyn         ͬ��
	@return 0���ɹ�������ʧ��
	@remark 
	@*/
	template<typename classtype>
	int TriggerEvent(LPCWSTR pszEventName, 
		classtype* pThis, 
		BOOL (classtype::*fnRunMethod)(LPCWSTR , easyipc::IEasyIpcBundle*), 
		BOOL bAsyn = FALSE)
	{
		int iRet = -1; 
		easyipc::IEasyEventCallback* pCallback = NULL; 
		if (!m_pIpcClient || !m_bundleParam)
			goto Exit0; 

		if (pThis && fnRunMethod)
			pCallback = CreateEventCallback(pThis, fnRunMethod); 

		m_bundleResult.Clear(); 
		iRet = m_pIpcClient->TriggerEvent(pszEventName, m_bundleParam, pCallback, bAsyn); 
Exit0:
		if (m_bundleParam)
			m_bundleParam.Clear(); 

		return iRet; 
	}

	/*
	@fn     WaitEventPusherEnd
	@brief  �ȴ��¼�������ɣ������첽�����¼�
	@arg    dwMilliseconds  �ȴ�ʱ��
	@return ��0�ɹ�����֮ʧ��
	@remark 
	@*/
	BOOL WaitEventPusherEnd(DWORD dwMilliseconds = INFINITE)
	{
		if (m_pIpcClient)
			return m_pIpcClient->WaitEventPusherEnd(dwMilliseconds); 

		return FALSE; 
	}
};

template<typename classtype>
class EasyEventWrap : public easyipc::IEasyEventObject
{
public:
	typedef void (classtype::*FnRunMethod)(LPCWSTR pszEventName, easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle* pResult); 

	EasyEventWrap(classtype* pObject, FnRunMethod fnRunMethod)
		: pObj(pObject), fnRun(fnRunMethod)
	{
	}

protected:
	virtual void __stdcall Release(void) 
	{
		delete this; 
	}

	virtual void __stdcall Run(LPCWSTR pszEventName, easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle* pResult)
	{
		return (pObj->*fnRun)(pszEventName, pParam, pResult); 
	}

	virtual BOOL __stdcall Equal( IEasyEventObject* pEventObject ) const
	{
		EasyEventWrap<classtype>* pEvent = (EasyEventWrap<classtype>*)pEventObject; 

		return pObj == pEvent->pObj && fnRun == pEvent->fnRun; 
	}

private:
	classtype*   pObj; 
	FnRunMethod  fnRun; 
};

template<typename classtype>
static easyipc::IEasyEventObject* CreateEventObj(classtype* pObject, 
												 void (classtype::*fnRunMethod)(LPCWSTR, easyipc::IEasyIpcBundle*, easyipc::IEasyIpcBundle*))
{
	if (!pObject || !fnRunMethod)
		return NULL; 

	return new EasyEventWrap<classtype>(pObject, fnRunMethod); 
}

template<typename classtype>
class EasyEventCallback : public easyipc::IEasyEventCallback
{
public:
	typedef BOOL (classtype::*FnRunMethod)(LPCWSTR pszEventName, easyipc::IEasyIpcBundle* pParam); 

	EasyEventCallback(classtype* pObject, FnRunMethod fnRunMethod)
		: pObj(pObject), fnRun(fnRunMethod)
	{
	}

protected:
	virtual void __stdcall Release(void) 
	{
		delete this; 
	}

	virtual BOOL __stdcall Run(LPCWSTR pszEventName, easyipc::IEasyIpcBundle* pParam)
	{
		return (pObj->*fnRun)(pszEventName, pParam); 
	}

private:
	classtype*   pObj; 
	FnRunMethod  fnRun; 
};

template<typename classtype>
static easyipc::IEasyEventCallback* CreateEventCallback(classtype* pObject, 
												 BOOL (classtype::*fnRunMethod)(LPCWSTR, easyipc::IEasyIpcBundle*))
{
	if (!pObject || !fnRunMethod)
		return NULL; 

	return new EasyEventCallback<classtype>(pObject, fnRunMethod); 
}


// -------------------------------------------------------------------------
// $Log: $

#endif /* __EASYIPC_EASYIPCEVENTWRAP_H__ */
