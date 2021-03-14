#ifndef __KCheckVersion__h__
#define __KCheckVersion__h__

#include "include\infoc\KProduceType.h"
#include <atlstr.h>

#ifdef _KCHECKVERSION_USE_DLL_PATH_
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

class KCheckVersion
{
public:
	static KCheckVersion& Instance()
	{
		static KCheckVersion inst;
		return inst;
	}

	BOOL IsDubaVersion()
	{
		return m_emProductType == enum_ProductType_SysoptAsst;
	}

	KInfoc_Product_Type GetCurrentVersion()
	{
		return m_emProductType;
	}

protected:
	KCheckVersion(void)
	{
		m_emProductType = GetInstalledType();
	}

	~KCheckVersion(void)
	{

	}

    KInfoc_Product_Type GetInstalledType()
    {
        KInfoc_Product_Type emProductType = enum_ProductType_SysoptAsst;
        return emProductType;
    }

#if 0
	KInfoc_Product_Type GetInstalledType()
	{
		KInfoc_Product_Type emProductType = enum_ProductType_Duba;

		TCHAR szModulePath[MAX_PATH + 1] = {0};

#ifdef _KCHECKVERSION_USE_DLL_PATH_
		::GetModuleFileName((HMODULE)&__ImageBase, szModulePath, MAX_PATH);
#else
		::GetModuleFileName(NULL, szModulePath, MAX_PATH);
#endif

		if (IsInstalledInDuba(szModulePath))
		{
			emProductType = enum_ProductType_Duba;
		}
		else if (IsInstalledInDriverGenius(szModulePath))
		{
			emProductType = enum_ProductType_DriverGenius;
		}
		else if (IsInstalledInKProtect(szModulePath))
		{
			emProductType = enum_ProductType_KProtect;
		}
		else if (IsInstalledInKDesk(szModulePath))
		{
			emProductType = enum_ProductType_KDesk;
		}
		else if (IsInstalledInKSafe(szModulePath))
		{
			emProductType = enum_ProductType_KSafe;
		}
		else if (IsInstalledInKWifi(szModulePath))
		{
			emProductType = enum_ProductType_KWifi;
		}
		else if (IsInstalledInKCalendar(szModulePath))
		{
			emProductType = enum_ProductType_KCalendar;
		}
		else if (IsInstalledInLieBaoBrowser(szModulePath))
		{
			emProductType = enum_ProductType_Liebao;
		}
		else if (IsInstalledInJiGuangPDFReader(szModulePath)) 
		{
			emProductType = enum_ProductType_JiGuangPDFReader;
		}

		return emProductType;
	}

	BOOL IsSubPath(CString path, CString subPath)
	{
		if (path.IsEmpty() || subPath.IsEmpty())
		{
			return FALSE;
		}

		path.MakeLower();
		path.TrimRight(L'\\');

		subPath.MakeLower();
		subPath.TrimRight(L'\\');
		
		return subPath.Find(path) == 0;
	}

	BOOL IsInstalledInKSafe(LPCTSTR lpszModulePath)
	{
//		BOOL bRet = FALSE;
		TCHAR szInstallPath[MAX_PATH + 1] = {0};
		ULONG ulSzie = MAX_PATH;
		CRegKey regKey;
		regKey.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\KSafe");
		regKey.QueryStringValue(L"Install Path", szInstallPath, &ulSzie);
		regKey.Close();

		return IsSubPath(szInstallPath, lpszModulePath);
	}

	BOOL IsInstalledInDriverGenius(LPCTSTR lpszModulePath)
	{
//		BOOL bRet = FALSE;
		TCHAR szInstallPath[MAX_PATH + 1] = {0};
		ULONG ulSzie = MAX_PATH;
		CRegKey regKey;
		regKey.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MyDrivers\\DriverGenius", KEY_READ | KEY_WOW64_32KEY);
		regKey.QueryStringValue(L"AppPath", szInstallPath, &ulSzie);
		regKey.Close();

		return IsSubPath(szInstallPath, lpszModulePath);
	}

	BOOL IsInstalledInDuba(LPCTSTR lpszModulePath)
	{
		CRegKey key;
		
		LONG lRet = key.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\sysoptasst", KEY_READ|KEY_WOW64_32KEY);
		if (ERROR_SUCCESS != lRet)
		{
			return FALSE;
		}

		TCHAR szInstallPath[MAX_PATH + 1] = {0};
		ULONG ulSzie = MAX_PATH;
		key.QueryStringValue(L"ProgramPath", szInstallPath, &ulSzie);
		key.Close();
		if (0 == ::wcslen(szInstallPath))
		{
			return FALSE;
		}

		return IsSubPath(szInstallPath, lpszModulePath);
	}

	BOOL IsInstalledInKProtect(LPCTSTR lpszModulePath)
	{
		CRegKey key;

		LONG lRet = key.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\kbasesrv", KEY_READ|KEY_WOW64_32KEY);
		if (ERROR_SUCCESS != lRet)
		{
			return FALSE;
		}

		TCHAR szInstallPath[MAX_PATH + 1] = {0};
		ULONG ulSzie = MAX_PATH;
		key.QueryStringValue(L"ProgramPath", szInstallPath, &ulSzie);
		key.Close();
		if (0 == ::wcslen(szInstallPath))
		{
			return FALSE;
		}

		return IsSubPath(szInstallPath, lpszModulePath);
	}

	BOOL IsInstalledInKDesk(LPCTSTR lpszModulePath)
	{
        return FALSE;
	}

	BOOL IsInstalledInKWifi(LPCTSTR lpszModulePath)
	{
		CRegKey key;

		LONG lRet = key.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\sysoptasst\\Kwifi", KEY_READ|KEY_WOW64_32KEY);
		if (ERROR_SUCCESS != lRet)
		{
			return FALSE;
		}

		TCHAR szInstallPath[MAX_PATH + 1] = {0};
		ULONG ulSzie = MAX_PATH;
		key.QueryStringValue(L"WorkPath", szInstallPath, &ulSzie);
		key.Close();
		szInstallPath[MAX_PATH] = L'\0';
		if (0 == ::wcslen(szInstallPath))
		{
			return FALSE;
		}

		return IsSubPath(szInstallPath, lpszModulePath);
	}

	BOOL IsInstalledInKCalendar(LPCTSTR lpszModulePath)
	{
        return FALSE;
	}

	BOOL QueryInstallDir(
		LPCWSTR lpSubKey, 
		CString& strResult, 
		HKEY hRoot = HKEY_LOCAL_MACHINE, 
		LPCWSTR lpValueName = L"ProgramPath", 
		REGSAM sam = KEY_READ|KEY_WOW64_32KEY)
	{
		if (NULL == hRoot || NULL == lpSubKey || NULL == lpValueName)
		{
			return FALSE;
		}

		if (0 == lpSubKey[0] || 0 == lpValueName[0])	//²»ÄÜÊÇ¿Õ×Ö·û
		{
			return FALSE;
		}

		CRegKey key;
		LONG lRet = key.Open(hRoot, lpSubKey, sam);
		if (ERROR_SUCCESS != lRet)
		{
			return FALSE;
		}

		TCHAR szInstallPath[MAX_PATH + 1] = {0};
		ULONG ulSize = MAX_PATH;
		key.QueryStringValue(lpValueName, szInstallPath, &ulSize);
		key.Close();

		szInstallPath[MAX_PATH] = L'\0';
		if (0 == ::wcslen(szInstallPath))
		{
			return FALSE;
		}

		strResult = szInstallPath;
		return TRUE;
	}

	BOOL IsInstalledInLieBaoBrowser(LPCTSTR lpszModulePath)
	{
		CString strLieBaoBrowserInstallDir;
		if (!QueryInstallDir(L"SOFTWARE\\liebao", strLieBaoBrowserInstallDir))
		{
			return FALSE;
		}

		return IsSubPath(strLieBaoBrowserInstallDir, lpszModulePath);
	}

	BOOL IsInstalledInJiGuangPDFReader(LPCTSTR lpszModulePath)
	{
		CString strInstallDir;
		if (!QueryInstallDir(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Fastpdf", strInstallDir, HKEY_LOCAL_MACHINE, L"UninstallString"))
		{
			return FALSE;
		}

		if(strInstallDir.IsEmpty()) 
		{
			return FALSE;
		}

		strInstallDir.Replace(L'/', L'\\');
		int nIndex = strInstallDir.ReverseFind(L'\\');
		if(nIndex == -1) 
		{
			return FALSE;
		}

		strInstallDir = strInstallDir.Mid(0, nIndex);

		return IsSubPath(strInstallDir, lpszModulePath);
	}
#endif

protected:
	KInfoc_Product_Type	m_emProductType;
};

#endif	//__KCheckVersion__h__