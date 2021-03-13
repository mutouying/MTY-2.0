#pragma once 

/**
 *
 * @NOTE windows helper without business
 *
 */
class WinModule {
public:
    /**
     * @NOTE: return the directory path with backslash
     *      
     */
    static CString GetModuleDirPath(HMODULE hModule);
    /**
     * @NOTE: 
     *      if success and phProcHandle is not null, caller must try to close the handle
     */
    static BOOL ExecCmd(LPCTSTR szPath, LPCTSTR szCmd, BOOL bShow = FALSE, HANDLE* phProcHandle = NULL);
    /**
     * @NOTE: 
     *      return 0 if success, return 1 if need rename and reboot, return 2 if need reboot. if fail, return value less than 0;
     */
    static int DeleteFile(LPCTSTR szPath, BOOL bTryReboot = FALSE);
    /**
     * @NOTE: 
     *      
     */
    static int RebootSystem();
};

#include "winmodule.hpp"