#pragma once
#include <list>
#include <map>

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
    int nCpuUser;   //CPU占用
    SIZE_T nMemoryUser;   // 内存占用（单位：B）
    ULONGLONG ullReadCount;  // I/O 读取
    ULONGLONG ullReadByteCount;   // I/O 读取字节数（单位：B）

    CString strUserName;// 用户名
    CString strProcessFullPath;// 进程文件路径

    DWORD dwParentPID;	//父进程PID
    DWORD dwSessionID;	//会话ID
    int dwHandleCount;	// 句柄数
    int dwThreadCount;	// 线程数

    tagListProcessInfo()
    {
        ZeroMemory(this, sizeof(tagSetProcessInfo));
    }

}SetProcessInfo, *pSetProcessInfo;