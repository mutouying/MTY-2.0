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
	CString strProcessName;   //进程名称
	CString strProcessFullPath;   //进程全路径
    DWORD dwProcessID;  //进程ID
    DWORD dwCpuUsage;   //CPU占用
    CString strMemoryUser;   //内存占用

    CString strUserName;// 用户名
    DWORD dwParentPID;	//父进程PID
    DWORD dwHandleCount;	// 句柄数
    DWORD dwThreadCount;	// 线程数
    DWORD dwSessionID;	//会话ID

    tagSetProcessInfo()
    {
		dwProcessID = 0;
		dwCpuUsage = 0;   //CPU占用
		
		dwParentPID = 0;	//父进程PID
		dwSessionID = 0;	//会话ID
		dwHandleCount = 0;	// 句柄数
		dwThreadCount = 0;	// 线程数
    }

	tagSetProcessInfo(DWORD dwPid)
	{	
		// 测试的构造函数 
		dwParentPID = dwPid;
		dwCpuUsage = 0;   //CPU占用

		dwParentPID = 0;	//父进程PID
		dwSessionID = 0;	//会话ID
		dwHandleCount = 0;	// 句柄数
		dwThreadCount = 0;	// 线程数

		strProcessName = L"strProcessName";
	}

}SetProcessInfo, *pSetProcessInfo;