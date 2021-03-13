#pragma once 

/*
reg_path:
    HKEY_LOCAL_MACHINE : software\sysoptasst
                uninstalltime :
                operation_user_city_number : 
                AllowProductRecommend: 
    HKEY_LOCAL_MACHINE : software\sysoptasst\ascommon
                installpath : 
                installtime : 
                tid1 : 
                tid2 : 
                tod1 : 
                tod2 : 
                iid  : 
                uninstall_count :
                detect_exception_timeout : 
                IEInfobarNoTipsEable: 
                traymode : 
                Lang: chs
                LogLevel : 
                simplelog : 
                needWriteLog:   <KLogMgrEx>
    HKEY_LOCAL_MACHINE : software\sysoptasst\ascommon\recommend
                LastShowIEInfoBarTime :
                IEInfobarNoTipsEable :
    HKEY_LOCAL_MACHINE : software\sysoptasst\ascommon\update
                asliveupdatetime :
    HKEY_LOCAL_MACHINE : software\sysoptasst\ascommon\ASEngine
                GhostFlag
                SilentFlag
                AutoLockIe
    HKEY_LOCAL_MACHINE : software\sysoptasst\ascommon\asflag
                KSilentFlag

    HKEY_LOCAL_MACHINE : software\sysoptasst\dumprecord

    HKEY_LOCAL_MACHINE : SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sysoptasst
                DisplayVersion :

    HKEY_CLASSES_ROOT : CLSID\\{2E79D13E-7339-4661-92EA-93AD506EF2E2}\\Implemented Categories\\{32C1137B-5DAB-4004-993F-46C7C3F7B560}
                idex  : uuid
                uid   : svrid
                uninstall_count: 
                
    HKEY_LOCAL_MACHINE : software\sysoptasst\union\setup
                联盟注册表位置
    HKEY_LOCAL_MACHINE : software\sysoptasst\union
                ProgramPath : 

    HKEY_CURRENT_USER  : software\sysoptasst\asvip (raw : HKEY_CURRENT_USER\Software\Kingsoft\KVip)
                lb_disable_message
                vipflag
*/

/*
    主窗口类名： "publish\AsWndMsgWrapper.h"
*/

#define REG_AS_ROOT                     _T("software\\sysoptasst")
#define REG_AS_COMMON                   _T("software\\sysoptasst\\ascommon")
#define REG_AS_UPDATE                   _T("software\\sysoptasst\\ascommon\\update")
#define REG_AS_NoInterruptKey	        _T("software\\sysoptasst\\ascommon\\ninterrupt")
#define REG_AS_ASENGINE      	        _T("software\\sysoptasst\\ascommon\\ASEngine")
#define REG_AS_RECOMMEND      	        _T("software\\sysoptasst\\ascommon\\recommend")
#define REG_AS_FLAGS                    _T("software\\sysoptasst\\ascommon\\asflag")
#define REG_AS_VIP                      _T("software\\sysoptasst\\asvip")
#define REG_AS_HEALTHEYE                _T("software\\sysoptasst\\khealtheye")

#define REG_REUPDATE_FLAG               _T("Software\\Classes\\CLSID\\{3891EAEA-211C-4692-837E-773A5A899D8D}")
#define REG_AS_UNINSTALL                _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\sysoptasst")

//> old : UID_REG = L"CLSID\\{9B7A98EC-7EF9-468c-ACC8-37C793DBD7E0}\\Implemented Categories\\{9B4EEDF7-FC98-4fa0-8440-9D1BC57B5F2F}";
#define REG_AS_UID_REG                  _T("CLSID\\{2E79D13E-7339-4661-92EA-93AD506EF2E2}\\Implemented Categories\\{32C1137B-5DAB-4004-993F-46C7C3F7B560}")                                          

#define INSTALL_REBOOT_FLAG_REG_PATH    _T("SOFTWARE\\sysoptasst")
#define INSTALL_REBOOT_FLAG_REG_KEY     _T("is_boot_start")

#define TRYNO_DEFAULT                   (6100)
#define PRODUCT_ID_INFOC                (550)

//> CSIDL_COMMON_APPDATA 下临时目录
#define DIR_IN_COMMON_APPDATA           _T("sysoptasst\\as\\")
