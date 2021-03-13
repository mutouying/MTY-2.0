#pragma once

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
#define  KEY_SELECT L"key_select_sum"

class ConfigUtil
{
public:
	ConfigUtil GetInstace()
	{
		static  ConfigUtil config = ConfigUtil();
		return config;
	}

	unsigned int GetFlagSet()
	{
		return m_flagSet;
	}

	void SetFlagSet(unsigned int flag)
	{
		m_flagSet = flag;
	}

	unsigned int GetSelectSum()
	{
		return m_SelectSum;
	}

	void SetSelectSum(int sum)
	{
		m_SelectSum = sum;
	}
	


public:
	ConfigUtil(void);
	~ConfigUtil(void);

	BOOL Read(LPCTSTR szValueName, DWORD& dwValue);
	BOOL Read(LPCTSTR szValueName, CString& strValue);
	BOOL Write(LPCTSTR szValueName, DWORD dwValue);
	BOOL Write(LPCTSTR szValueName, LPCTSTR szValue);

private:

	unsigned int   m_flagSet; 
	int   m_SelectSum;

};
