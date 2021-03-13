#pragma once


typedef struct tagSetProcessInfo
{
	CString strProcessName;   //进程名称
	CString strProcessFullPath;   //进程全路径
    DWORD dwProcessID;  //进程ID
    DWORD dwCpuUser;   //CPU占用
    CString dwMemoryUser;   //内存占用

    CString strUserName;// 用户名
    DWORD dwParentPID;	//父进程PID
    DWORD dwHandleCount;	// 句柄数
    DWORD dwThreadCount;	// 线程数
    DWORD dwSessionID;	//会话ID

    tagSetProcessInfo()
    {
		dwProcessID = 0;
		dwCpuUser = 0;   //CPU占用
		
		dwParentPID = 0;	//父进程PID
		dwSessionID = 0;	//会话ID
		dwHandleCount = 0;	// 句柄数
		dwThreadCount = 0;	// 线程数
    }

	tagSetProcessInfo(DWORD dwPid)
	{	
		// 测试的构造函数 
		dwParentPID = dwPid;
		dwCpuUser = 0;   //CPU占用

		dwParentPID = 0;	//父进程PID
		dwSessionID = 0;	//会话ID
		dwHandleCount = 0;	// 句柄数
		dwThreadCount = 0;	// 线程数

		strProcessName = L"strProcessName";
	}

}SetProcessInfo, *pSetProcessInfo;