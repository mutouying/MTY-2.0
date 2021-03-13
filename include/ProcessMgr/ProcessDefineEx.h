#pragma once

enum emProcessChangeType
{
    type_process_new,
    type_process_exit,
    type_process_change
};

#define EASYIPC_REGISTER_METHODEX(name, class, method, obj)                         \
    static int s_n##class##method = KEasyIpcServerWrap::Instance().Register(        \
    name, L#method, EasyIpcExecWrap<class>::CreateObj((obj), &class::method));

#define EASYIPC_START_INTERFACEEX(interface) KEasyIpcServerWrap::Instance().Start(interface)

#define EASYIPC_STOP_INTERFACEEX(interface)  KEasyIpcServerWrap::Instance().Stop(interface)

#define EASYIPC_UNREGISTEREX(class)                                                          \
    KEasyIpcServerWrap::Instance().Unregister(class)


#define ipc_first_load_process L"FirstLoadData"
#define ipc_first_load_process_end L"FirstLoadDataEnd"
#define ipc_refresh_process L"RefreshData"


typedef struct tagSetProcessInfo
{
	CString strProcessName;   //��������
	CString strProcessFullPath;   //����ȫ·��
    DWORD dwProcessID;  //����ID
    DWORD dwCpuUsage;   //CPUռ��
    CString strMemoryUser;   //�ڴ�ռ��

    CString strUserName;// �û���
    DWORD dwParentPID;	//������PID
    DWORD dwHandleCount;	// �����
    DWORD dwThreadCount;	// �߳���
    DWORD dwSessionID;	//�ỰID

    tagSetProcessInfo()
    {
		dwProcessID = 0;
		dwCpuUsage = 0;   //CPUռ��
		
		dwParentPID = 0;	//������PID
		dwSessionID = 0;	//�ỰID
		dwHandleCount = 0;	// �����
		dwThreadCount = 0;	// �߳���
    }

	tagSetProcessInfo(DWORD dwPid)
	{	
		// ���ԵĹ��캯�� 
		dwParentPID = dwPid;
		dwCpuUsage = 0;   //CPUռ��

		dwParentPID = 0;	//������PID
		dwSessionID = 0;	//�ỰID
		dwHandleCount = 0;	// �����
		dwThreadCount = 0;	// �߳���

		strProcessName = L"strProcessName";
	}

}SetProcessInfo, *pSetProcessInfo;