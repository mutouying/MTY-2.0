#pragma once

typedef struct tagListProcessInfo
{
    CString strProcessName;   //进程名称
    DWORD dwProcessID;  //进程ID
    CString strProcessFullPath;   //进程全路径
    int nCpuUser;   //CPU占用
    SIZE_T nMemoryUser;   //内存占用

    tagListProcessInfo()
    {
        ZeroMemory(this, sizeof(tagListProcessInfo));
    }

}ListProcessInfo, *pListProcessInfo;

typedef struct tagSetProcessInfo
{
    DWORD dwCpuUser;   //CPU占用
    SIZE_T nMemoryUser;   // 内存占用（单位：B）
    ULONGLONG ullReadCount;  // I/O 读取
    ULONGLONG ullReadByteCount;   // I/O 读取字节数（单位：B）

    CString strUserName;// 用户名
    CString strProcessFullPath;// 进程文件路径

    DWORD dwParentPID;	//父进程PID
    DWORD dwSessionID;	//会话ID
    DWORD dwHandleCount;	// 句柄数
    DWORD dwThreadCount;	// 线程数

    tagSetProcessInfo()
    {
		dwCpuUser = 0;   //CPU占用
		nMemoryUser  = 0;   // 内存占用（单位：B）
		ullReadCount = 0; // I/O 读取
		ullReadByteCount  = 0;   // I/O 读取字节数（单位：B）

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
		nMemoryUser  = 0;   // 内存占用（单位：B）
		ullReadCount = 0; // I/O 读取
		ullReadByteCount  = 0;   // I/O 读取字节数（单位：B）

		dwSessionID = 0;	//会话ID
		dwHandleCount = 0;	// 句柄数
		dwThreadCount = 0;	// 线程数
		strProcessFullPath = L"strProcessFullPath";
	}

}SetProcessInfo, *pSetProcessInfo;