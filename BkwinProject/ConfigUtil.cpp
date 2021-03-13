#include "StdAfx.h"
#include "ConfigUtil.h"
#include "KRegister2.h"

ConfigUtil::ConfigUtil(void)
{
}

ConfigUtil::~ConfigUtil(void)
{
}

BOOL ConfigUtil::Read(LPCTSTR szValueName, DWORD& dwValue)
{
	CString strPath;
	DWORD Temp;
	KRegister2 regKey;

	if (FALSE == regKey.Open32(HKEY_CURRENT_USER, _T("software\\kingsoft\\bkwinProject")))
		goto Exit0;

	if (FALSE == regKey.Read(szValueName, Temp))
		goto Exit0;

	dwValue = Temp;
	return TRUE;

Exit0:
	return FALSE ;
}

BOOL ConfigUtil::Read(LPCTSTR szValueName, CString& strValue)
{
	CString strPath;
	CString strTemp;
	KRegister2 regKey;

	if (FALSE == regKey.Open32(HKEY_CURRENT_USER, _T("software\\kingsoft\\bkwinProject")))
		goto Exit0;

	if (FALSE == regKey.Read(szValueName, strTemp))
		goto Exit0;

	strPath = strTemp;
	return TRUE;

Exit0:
	return FALSE;
}

BOOL ConfigUtil::Write(LPCTSTR szValueName, DWORD dwValue)
{
	KRegister2 regKey;
	if (FALSE == regKey.Open32(HKEY_CURRENT_USER, _T("software\\kingsoft\\bkwinProject")))
		goto Exit0;

	if (FALSE == regKey.Write(szValueName, dwValue))
		goto Exit0;
	return TRUE;

Exit0:
	return FALSE;
}

BOOL ConfigUtil::Write(LPCTSTR szValueName, LPCTSTR szValue)
{
	KRegister2 regKey;
	if (FALSE == regKey.Open32(HKEY_CURRENT_USER, _T("software\\kingsoft\\bkwinProject")))
		goto Exit0;

	if (FALSE == regKey.Write(szValueName, szValue))
		goto Exit0;
	return TRUE;

Exit0:
	return FALSE;
}
