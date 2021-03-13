#pragma once
#include "ascore\ashelper.h"
#include "ascore\winmodule.h"
#include "framework\kregister2.h"
#include "framework\KTime.h"

inline CString AsHelper::GetTryNoFromIni(LPCTSTR szInstallDir) {
    TCHAR   szVersion[MAX_PATH] = {0};
    CString strTemp;
    CString strPath;

    CString strInstallDir = szInstallDir;
    if (strInstallDir.IsEmpty()) {
        GetInstallPathFromReg(strInstallDir);
    }

    strPath = strInstallDir + _T("ressrc\\chs\\uplive.svr");
    if (::GetPrivateProfileString(_T("common"),  _T("TryNo"), _T(""), szVersion, sizeof(szVersion), strPath))
        strTemp = szVersion;

    return strTemp;
}

inline BOOL AsHelper::GetInstallPathFromReg(CString& strInstallPath) {
    strInstallPath.Empty();
    if (TRUE == ReadRegKeyValue(HKEY_LOCAL_MACHINE, REG_AS_COMMON, L"installpath", strInstallPath)) {
        CString strTemp = strInstallPath.Right(1);
        if (!strInstallPath.IsEmpty() && strTemp != _T("\\") && strTemp != _T("/")) {
            strInstallPath.Append(_T("\\"));
        }
        return TRUE;
    }
    return FALSE;
}

inline BOOL AsHelper::GetInstallTimeFromReg(CString& strInstallTime) {
    DWORD dwInstallTime = 0;
    if (FALSE == GetInstallTimeFromReg(dwInstallTime)) {
        return FALSE;
    }
    SYSTEMTIME stTime = KTime((__time64_t)dwInstallTime).GetSystemTime();
    strInstallTime.Format(_T("%d-%d-%d %d:%d:%d"), stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
    return TRUE;
}

inline BOOL AsHelper::GetInstallTimeFromReg(DWORD& dwInstallTime) {
    return ReadRegKeyValue(HKEY_LOCAL_MACHINE, REG_AS_COMMON, L"installtime", dwInstallTime);
}

inline BOOL AsHelper::GetInstallTimeFromReg(SYSTEMTIME& stTime) {
    DWORD dwInstallTime = 0;
    if (FALSE == GetInstallTimeFromReg(dwInstallTime)) {
        return FALSE;
    }
    stTime = KTime((__time64_t)dwInstallTime).GetSystemTime();
    return TRUE;
}

inline BOOL AsHelper::GetUuidFromReg(CString& strUuid) {
    BOOL bRet = ReadRegKeyValue(HKEY_CLASSES_ROOT, REG_AS_UID_REG, L"idex", strUuid);
    if (bRet != TRUE) {
        return bRet;
    }

    if (strUuid.GetLength() != 32) {
        return FALSE;
    }
    strUuid.MakeUpper();
    return TRUE;
}

inline BOOL AsHelper::GetSrvIDFromReg(CString& strSrvID) {
    BOOL bRet = ReadRegKeyValue(HKEY_CLASSES_ROOT, REG_AS_UID_REG, L"uid", strSrvID);
    if (bRet != TRUE) {
        return bRet;
    }

    if (strSrvID.GetLength() != 32) {
        return FALSE;
    }
    strSrvID.MakeUpper();
    return TRUE;
}

inline BOOL AsHelper::IsSysoptasstInstalled() {
    BOOL bInstalled = FALSE;

    CString strInstallPath;
    if (FALSE == AsHelper::GetInstallPathFromReg(strInstallPath)) {
        return bInstalled;
    }
    if (strInstallPath.IsEmpty()) {
        return bInstalled;
    }

    LPCTSTR szFileList[] = {
        _T("astray.exe"),
        _T("ascore.exe"),
    };
    
    bInstalled = TRUE;
    for (int i = 0; i < sizeof(szFileList) / sizeof(LPCTSTR); ++i) {
        if (_taccess(strInstallPath + szFileList[i], 0) != 0) {
            bInstalled = FALSE;
            break;
        }
    }
    return bInstalled;
}

inline BOOL AsHelper::ReadRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, DWORD& value) {
    KRegister2 reg;
    if (!reg.Open(hRootKey, szSubKey, TRUE))
        return FALSE;
    return reg.Read(szValueName, value);
}

inline BOOL AsHelper::ReadRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, CString& value) {
    KRegister2 reg;
    if (!reg.Open(hRootKey, szSubKey, TRUE))
        return FALSE;
    return reg.Read(szValueName, value);
}

inline BOOL AsHelper::WriteRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, DWORD dwValue) {
    KRegister2 reg;
    if (!reg.Open(hRootKey, szSubKey, FALSE)) {
        return FALSE;
    }
    return reg.Write(szValueName, dwValue);
}

inline BOOL AsHelper::WriteRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName, LPCTSTR szValue) {
    KRegister2 reg;
    if (!reg.Open(hRootKey, szSubKey, FALSE)) {
        return FALSE;
    }
    return reg.Write(szValueName, szValue);
}

inline BOOL AsHelper::DeleteRegKeyValue(HKEY hRootKey, LPCTSTR szSubKey, LPCTSTR szValueName /* = NULL */) {
    KRegister2 reg;
    if (szValueName == NULL) {
        return reg.DeleteSubKeyTree(hRootKey, szSubKey);
    }

    if (FALSE == reg.Open(hRootKey,szSubKey, FALSE)) {
        return FALSE;
    }
    return reg.DeleteValue(szValueName);
}

inline BOOL AsHelper::GetInstalledDays(DWORD& dwDays) {
    SYSTEMTIME stInstallTime;
    if (FALSE == AsHelper::GetInstallTimeFromReg(stInstallTime)) {
        return FALSE;
    } 

    KTime ktInstall(KTime::SystemTimeToTime64(stInstallTime));
    KTimeSpan ktDelta = KTime::GetCurrentTime() - ktInstall;
    dwDays = ktDelta.GetDays();

    return TRUE;
}

inline BOOL AsHelper::GetAsMainVersion(CString& strVersion) {
    TCHAR	szTmp[MAX_PATH] ={0};
    CString sDisplayVer, sVer, sMainVersion, sInstallPath;
    CString	sPath, strVer, strYear;

    if (FALSE == AsHelper::GetInstallPathFromReg(sInstallPath)) {
        return FALSE;
    }

    sPath.Format(_T("%sressrc\\%s\\asmain.ini"), sInstallPath, _T("chs"));
    ::GetPrivateProfileString(_T("infos"), _T("version"), _T(""), szTmp, MAX_PATH, sPath);
    sVer = szTmp;
    ::GetPrivateProfileString(_T("DisplayVersions"), _T("Version"), _T(""), szTmp, MAX_PATH, sPath);
    sDisplayVer = szTmp;
    ::GetPrivateProfileString(_T("DisplayVersions"), _T("MainVersion"), _T(""), szTmp, MAX_PATH, sPath);
    sMainVersion = szTmp;

    if (sVer.GetLength() > 6)
    {
        sVer = sVer.Right(6);
    }

    int nIndex  = sDisplayVer.Find('.');
    strYear = sDisplayVer.Left(4);

    if (sMainVersion.IsEmpty())
    {
        if (nIndex != -1)
        {
            sDisplayVer = sDisplayVer.Mid(nIndex+1);
        }
        strVer.Format(_T("%s.%s.%s"), strYear, sDisplayVer, sVer);
    }
    else
    {
        if (nIndex != -1)
        {
            sDisplayVer = sDisplayVer.Mid(nIndex+3);
        }
        strVer.Format(_T("%s.%s.%s.%s"), sMainVersion, strYear, sDisplayVer, sVer);
    }

    strVersion = strVer;
    return TRUE;
}

inline int AsHelper::WaitAstrayExit(LPCTSTR szDirPath, int nWaitMs) {
    int nReturn = -1;
    CString strExePath;

    do {
        if (szDirPath == NULL) {
            strExePath = WinModule::GetModuleDirPath(NULL);
        } else {
            strExePath = szDirPath;
        }

        if (FALSE == strExePath.IsEmpty()) {
            CString strTemp = strExePath.Right(1);
            if (strTemp != _T("\\") && strTemp != _T("/")) {
                strExePath.Append(_T("\\"));
            }
        }
        strExePath.Append(_T("astray.exe"));
        if (_taccess(strExePath, 0) != 0) {
            nReturn = -1;
            break;
        }

        HANDLE hProcHandle = NULL;
        if (FALSE == WinModule::ExecCmd(strExePath, _T("/stop /uninstall"), FALSE, &hProcHandle)) {
            nReturn = -2;
            break;
        }

        DWORD dwWaitRet = ::WaitForSingleObject(hProcHandle, nWaitMs);
        switch (dwWaitRet) {
        case WAIT_OBJECT_0:
            nReturn = 0;
            break;
        case WAIT_TIMEOUT:
            nReturn = -3;
            break;
        case WAIT_FAILED:
            nReturn = -4;
            break;
        default:
            nReturn = -5;
            break;
        }
        
        if (NULL != hProcHandle) {
            ::CloseHandle(hProcHandle);
        }
    } while (false);

    return nReturn;
}
