#pragma once

#define ALL_ITEM_NUM     10

// flag 设置界面的flag
#define FlagProcessName					0x00000001	
#define FlagProcessPid					0x00000002
#define FlagProcessPath					0x00000004
#define FlagProcessCpu					0x00000008
#define FlagProcessMem					0x00000010

#define FlagProcessUser					0x00000020	
#define FlagProcessParent				0x00000040	
#define FlagProcessHandle				0x00000080
#define FlagProcessThread				0x00000100
#define FlagProcessSession				0x00000200

#define  KEY_FLAG_SET L"key_flag_set"
#define  KEY_SELECT_SUM L"key_select_sum"


class ConfigUtil
{
public:
	static ConfigUtil GetInstace()
	{
		static  ConfigUtil config = ConfigUtil();
		return config;
	}

	unsigned int GetFlagSet()
	{
		LoadConfig();
		return m_flagSet;
	}

// 	void SetFlagSet(unsigned int flag)
// 	{
// 		m_flagSet = flag;
// 	}

	unsigned int GetSelectSum()
	{
		LoadConfig();
		return m_SelectSum;
	}

// 	void SetSelectSum(int sum)
// 	{
// 		m_SelectSum = sum;
// 	}
// 	


public:
	ConfigUtil(void);
	~ConfigUtil(void);

	BOOL Read(LPCTSTR szValueName, DWORD& dwValue);
	BOOL Read(LPCTSTR szValueName, CString& strValue);
	BOOL Write(LPCTSTR szValueName, DWORD dwValue);
	BOOL Write(LPCTSTR szValueName, LPCTSTR szValue);
	BOOL LoadConfig();

private:

	unsigned int   m_flagSet; 
	int   m_SelectSum;

};

#define ConfigUtilInst  ConfigUtil::GetInstace()


//  以下结构体，全部使用Cstring，为了展示方便
typedef struct tagSetProcessInfoCstring
{
	DWORD ProcessID;
	CString strProcessName;   //进程名称
	CString strProcessFullPath;   //进程全路径
	CString dwProcessID;  //进程ID
	CString dwCpuUser;   //CPU占用
	CString dwMemoryUser;   //内存占用

	CString strUserName;// 用户名
	CString dwParentPID;	//父进程PID
	CString dwHandleCount;	// 句柄数
	CString dwThreadCount;	// 线程数
	CString dwSessionID;	//会话ID

	tagSetProcessInfoCstring(DWORD dwPid)
	{	
		ProcessID = dwPid;
		strProcessName = L"strProcessName";
	}

}SetProcessInfoString, *pSetProcessInfoString;