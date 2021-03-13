#pragma once 
#include "ascore\asdefines.h"

class AsHelper {
public:
    static CString GetTryNoFromIni(LPCTSTR szInstallDir);
    static BOOL GetInstallPathFromReg(CString& strInstallPath);
    static BOOL GetInstallTimeFromReg(DWORD& dwInstallTime);
    static BOOL GetInstallTimeFromReg(CString& strInstallTime);
    static BOOL GetInstallTimeFromReg(SYSTEMTIME& installTime);
    static BOOL IsSysoptasstInstalled();
    static BOOL GetUuidFromReg(CString& strUuid);
    static BOOL GetSrvIDFromReg(CString& strSrvID);
    static BOOL GetInstalledDays(DWORD& dwDays);
    static BOOL GetAsMainVersion(CString& strVersion);
    static int WaitAstrayExit(LPCTSTR szDirPath, int nWaitMs);

public:
    static BOOL DeleteRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName = NULL);
    static BOOL ReadRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, DWORD& value);
    static BOOL ReadRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, CString& value);
    static BOOL WriteRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, DWORD dwValue);
    static BOOL WriteRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, LPCTSTR szValue);
};

#include "ascore\ashelper.hpp"