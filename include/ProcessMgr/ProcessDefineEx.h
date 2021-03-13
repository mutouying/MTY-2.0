#pragma once


typedef struct tagSetProcessInfo
{
	CString strProcessName;   //��������
	CString strProcessFullPath;   //����ȫ·��
    DWORD dwProcessID;  //����ID
    DWORD dwCpuUser;   //CPUռ��
    CString dwMemoryUser;   //�ڴ�ռ��

    CString strUserName;// �û���
    DWORD dwParentPID;	//������PID
    DWORD dwHandleCount;	// �����
    DWORD dwThreadCount;	// �߳���
    DWORD dwSessionID;	//�ỰID

    tagSetProcessInfo()
    {
		dwProcessID = 0;
		dwCpuUser = 0;   //CPUռ��
		
		dwParentPID = 0;	//������PID
		dwSessionID = 0;	//�ỰID
		dwHandleCount = 0;	// �����
		dwThreadCount = 0;	// �߳���
    }

	tagSetProcessInfo(DWORD dwPid)
	{	
		// ���ԵĹ��캯�� 
		dwParentPID = dwPid;
		dwCpuUser = 0;   //CPUռ��

		dwParentPID = 0;	//������PID
		dwSessionID = 0;	//�ỰID
		dwHandleCount = 0;	// �����
		dwThreadCount = 0;	// �߳���

		strProcessName = L"strProcessName";
	}

}SetProcessInfo, *pSetProcessInfo;