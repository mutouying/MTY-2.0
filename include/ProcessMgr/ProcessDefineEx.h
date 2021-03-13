#pragma once

typedef struct tagListProcessInfo
{
    CString strProcessName;   //��������
    DWORD dwProcessID;  //����ID
    CString strProcessFullPath;   //����ȫ·��
    int nCpuUser;   //CPUռ��
    SIZE_T nMemoryUser;   //�ڴ�ռ��

    tagListProcessInfo()
    {
        ZeroMemory(this, sizeof(tagListProcessInfo));
    }

}ListProcessInfo, *pListProcessInfo;

typedef struct tagSetProcessInfo
{
    DWORD dwCpuUser;   //CPUռ��
    SIZE_T nMemoryUser;   // �ڴ�ռ�ã���λ��B��
    ULONGLONG ullReadCount;  // I/O ��ȡ
    ULONGLONG ullReadByteCount;   // I/O ��ȡ�ֽ�������λ��B��

    CString strUserName;// �û���
    CString strProcessFullPath;// �����ļ�·��

    DWORD dwParentPID;	//������PID
    DWORD dwSessionID;	//�ỰID
    DWORD dwHandleCount;	// �����
    DWORD dwThreadCount;	// �߳���

    tagSetProcessInfo()
    {
		dwCpuUser = 0;   //CPUռ��
		nMemoryUser  = 0;   // �ڴ�ռ�ã���λ��B��
		ullReadCount = 0; // I/O ��ȡ
		ullReadByteCount  = 0;   // I/O ��ȡ�ֽ�������λ��B��

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
		nMemoryUser  = 0;   // �ڴ�ռ�ã���λ��B��
		ullReadCount = 0; // I/O ��ȡ
		ullReadByteCount  = 0;   // I/O ��ȡ�ֽ�������λ��B��

		dwSessionID = 0;	//�ỰID
		dwHandleCount = 0;	// �����
		dwThreadCount = 0;	// �߳���
		strProcessFullPath = L"strProcessFullPath";
	}

}SetProcessInfo, *pSetProcessInfo;