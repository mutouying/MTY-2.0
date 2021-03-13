#pragma once
#include <list>
#include <map>

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
    int nCpuUser;   //CPUռ��
    SIZE_T nMemoryUser;   // �ڴ�ռ�ã���λ��B��
    ULONGLONG ullReadCount;  // I/O ��ȡ
    ULONGLONG ullReadByteCount;   // I/O ��ȡ�ֽ�������λ��B��

    CString strUserName;// �û���
    CString strProcessFullPath;// �����ļ�·��

    DWORD dwParentPID;	//������PID
    DWORD dwSessionID;	//�ỰID
    int dwHandleCount;	// �����
    int dwThreadCount;	// �߳���

    tagListProcessInfo()
    {
        ZeroMemory(this, sizeof(tagSetProcessInfo));
    }

}SetProcessInfo, *pSetProcessInfo;