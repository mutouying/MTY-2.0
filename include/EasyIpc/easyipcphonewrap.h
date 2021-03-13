#ifndef __EASYIPCPHONEQUERY_H__
#define __EASYIPCPHONEQUERY_H__

#include "EasyIpcClientWrap.h"


class KIpcPhoneQuery
{
public:

	KIpcPhoneQuery() : m_ipcClient(L"KMobileConnIpcServer")
	{
	}

	~KIpcPhoneQuery()
	{
	}

	/*
	* @brief ��ȡmobileduba�Ƿ�ȫ������
	*
	* @param[in]  bInstall   ���ص�ǰ�ֻ��ֻ������Ƿ�װ��
	*
	* @return 0 �ɹ�,����Ϊʧ�ܴ�����
	*/
	int __stdcall QueryDubaInstState(BOOL& bInstall)
	{
		int nReturn = -1;
		int nInstaState = 0;

		if (m_ipcClient.Call(L"QueryDubaInstState") != 0)
			goto Exit0;


		bInstall = FALSE;

		nInstaState = m_ipcClient.GetInt(L"dubainst");
		if (nInstaState == -1)
			goto Exit0;

		if (nInstaState == 1)
			bInstall = TRUE;
		else
			bInstall = FALSE;
		
		nReturn = 0;

	Exit0:
		return nReturn;
	}

private:
	KEasyIpcClientWrap m_ipcClient;

};


#endif