#pragma once 
#include "winmodule.h"

inline CString WinModule::GetModuleDirPath(HMODULE hModule) {
    DWORD dwRet = 0;
    CString strTemp;
    TCHAR szFileName[MAX_PATH + 1] = {0};

    dwRet = ::GetModuleFileName(hModule, szFileName, MAX_PATH);
    if (!dwRet) return _T("");

    strTemp = szFileName;
    int nPos = -1;
    nPos = strTemp.ReverseFind(_T('\\'));
    if (-1 == nPos)
    {
        nPos = strTemp.ReverseFind(_T('/'));
    }

    if (-1 == nPos) return _T("");

    return strTemp.Left(nPos + 1);
}

inline BOOL WinModule::ExecCmd(LPCTSTR szPath, LPCTSTR szCmd, BOOL bShow/* = FALSE */, HANDLE* phProcHandle/* = NULL */) {
    CString strExePath;	
    strExePath.Format(_T("\"%s\" %s"), szPath, szCmd);

    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    if (bShow) {
        si.wShowWindow = SW_SHOWDEFAULT;
    }

    PROCESS_INFORMATION pi = { 0 };
    BOOL bRetCode = CreateProcessW(NULL,			// No module name (use command line). 
        (LPWSTR)(LPCTSTR)strExePath,				// Command line. 
        NULL,				// Process handle not inheritable. 
        NULL,				// Thread handle not inheritable. 
        FALSE,				// Set handle inheritance to FALSE. 
        0,					// No creation flags. 
        NULL,				// Use parent's environment block. 
        NULL,				// Use parent's starting directory. 
        &si,				// Pointer to STARTUPINFO structure.
        &pi);				// Pointer to PROCESS_INFORMA-TION structure.

    if (pi.hProcess != NULL) {
        if (bRetCode && phProcHandle != NULL) {
            *phProcHandle = pi.hProcess;
        } else {
            ::CloseHandle(pi.hProcess);
            pi.hProcess = NULL;
        }
    } 
    if (pi.hThread != NULL) {
        ::CloseHandle(pi.hThread);
        pi.hThread = NULL;
    }

    return bRetCode;
}

inline int WinModule::DeleteFile(LPCTSTR szPath, BOOL bTryReboot /*= FALSE*/) {
    int nRet = -1;

    CString strTempFileName;
    strTempFileName.Format(_T("%s_%d_del"), szPath, int(::GetTickCount()));

    DWORD dwAttrib = GetFileAttributes(szPath);
    if (INVALID_FILE_ATTRIBUTES == dwAttrib || 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return nRet;
    }

    if (TRUE == ::DeleteFile(szPath)) {
        nRet = 0;
    } else if (::MoveFileEx(szPath, strTempFileName, MOVEFILE_REPLACE_EXISTING)) {
        if (TRUE == ::DeleteFile(strTempFileName)) {
            nRet = 0;
        } else {
            ::MoveFileEx(strTempFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
            nRet = 1;
        }        
    } else {
        ::MoveFileEx(szPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
        nRet = 2;
    }
    return nRet;
}

inline BOOL WinModule::RebootSystem() {
    HANDLE hToken = NULL;
    BOOL bReturn = FALSE;
    BOOL bRetCode = FALSE;

    do {
        bRetCode = ::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
        if (!bRetCode) {
            break;
        }

        TOKEN_PRIVILEGES tkp; 
        bRetCode = ::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
        if (!bRetCode) {
            break;
        }
        tkp.PrivilegeCount = 1;  
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

        ::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
        if (GetLastError() != ERROR_SUCCESS) {
            break;
        }

        DWORD dwOption = 0;
        dwOption = EWX_REBOOT | EWX_FORCE;
        bRetCode = ::ExitWindowsEx(dwOption, SHTDN_REASON_MAJOR_APPLICATION);
        if (bRetCode != TRUE) {
            break;
        }

        bReturn = TRUE;
    } while(false);

    if (hToken) {
        ::CloseHandle(hToken);
    }
    return bReturn;
}
