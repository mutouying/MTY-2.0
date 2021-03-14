/*************************************************
//  File name:       IBkRunnableContext.h
//  Date:              2020/12/14
//  <author>      hexujun@cmcm.com
//  <time>          2020/12/14
//   <version>     1.0    
*************************************************/
#ifndef _BKWIN_IBkRunnableContext_h_
#define _BKWIN_IBkRunnableContext_h_

#include "BkRunnable.h"

namespace BkWin {\

class IBkRunnableContext 
{
public:
	/**
	 * ����Runnable�������������߳� ����runnable�������߳�ִ��
	 * �ο� BkRunnable.h�ļ��ģ�BkWin::FuncFactory����runnable������
	 */
	virtual BOOL SendRunnable(BkSafePtr<IRunnable>& runnable) = 0;

	/*
	* PostRunnable: ���ȴ������� �����������߳� ����runnable�������߳�ִ��
	* LPVOID lpTarget ���Ӳ���������ͨ���Ĳ���cancel runnable
	* uDelayTime ������ʱִ��
	* return runnable unique id, ���ø�id����ȡ��
	*/
	virtual DWORD  PostRunnable(BkSafePtr<IRunnable>& runnable, LPVOID lpTarget = NULL, UINT uDelayTime = 0) = 0;

	/**
	 * ȡ����û��ִ�е�ruannble
	 */
	virtual void CancelRunnableByID(DWORD dwRunnbaleID) = 0;
	virtual void CancelTargetRunnables(LPVOID lpszTarget) = 0;
};
}
#endif