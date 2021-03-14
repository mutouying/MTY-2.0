#pragma once

#include "bkres/bkres.h"
#include "bkwin/CBkDialogViewImplEx.h"
#include "bkwin/bkshadowdrawable.h"
#include "bkwin/CBkDialogMenu.h"
#include "ProcessMgr/ProcessDefineEx.h"
#include "framework/KTinyXml.h"

#include "EasyIpc/easyipcserverwrap.h"
#include "framework/KLocker.h"
#include "ProcessMgr/IBaseProcessMgr.h"
#include "framework/KEvent.h"
#include "Actor/KActor.h"

#define WM_DELAY_LOADDATA  WM_USER + 100

using namespace std;
class KBkwinDemoDlg
    : public CBkDialogViewImplEx<KBkwinDemoDlg>
	, public BkWin::WndShadowHelper<KBkwinDemoDlg>
	, public CBkDialogMenu::IBKMenuCmdListener
	, public IActorCallBack
{
public:
    KBkwinDemoDlg();
	~KBkwinDemoDlg();

    enum
    {
		IDC_BTN_MIN								=	100,
		IDC_BTN_CLOSE							=	102,
		IDC_BTN_SETTING							=	103,

		IDC_BTN_KILL_PROCESS					=    203,

		IDC_LIST_PROC							= 1000,
		IDC_LIST_KEY							= 1001,
    };

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN_EX)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MIN, OnBtnMin)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CLOSE, OnBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SETTING,OnSetting)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_KILL_PROCESS,OnBtnKillProcess)

		BK_LISTWND_NOTIFY_BEGIN(IDC_LIST_PROC)
			BK_LISTWND_LISTITEM_MOUSEHOVER(OnListItemMouseHover)
			BK_LISTWND_LISTITEM_LBUTTONUP(OnListItemLButtonUp)
			BK_LISTWND_LISTITEM_CHILD_LBUTTONUP(OnListItemChildLBtnUp)
		BK_LISTWND_NOTIFY_END()
    BK_NOTIFY_MAP_END()

    BEGIN_MSG_MAP_EX(CBkDialogViewImplEx<KBkwinDemoDlg>)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN_EX)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)

        MESSAGE_HANDLER_EX(WM_DELAY_LOADDATA, OnDelayInit)
	
		CHAIN_MSG_MAP(CBkDialogViewImplEx<KBkwinDemoDlg>)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
    LRESULT OnDelayInit(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void OnBtnClose();
    void OnSysCommand(UINT nID, CPoint point);
	void OnSetting();

	void OnListItemLButtonUp( int nListItem);
	void OnListItemMouseHover(int nListItem);
	void OnListItemMouseLeave(int nListItem);
	void OnListItemChildLBtnUp(int nListItem, int nChildCtrlId);

	void UpdateConfig();
	BOOL AddTableTitleToListWnd();
	void UpdateWindowsUI();
	BOOL AddOnekeyToListWnd(SetProcessInfo& data , int nIndex);
	BOOL CreateHotkeyListItemXml(SetProcessInfo & data, KTinyXml tinyXml, int nIndex );
	void testCode();
	void UpdateCacheMapToRealMap();
	int OnBtnKillProcess();
	int KillProcessById(int pid);
	virtual void   OnActivate(KActor* pActor);
	void StartProLogic();
	int OutputAllProcess();
protected:
	virtual void OnBkMenuCmd(CBkDialogMenu*  pDialogMenu, LPCWSTR lpszMenuName);
	void OnBtnMin();
protected:
	CBkDialogMenu* m_pMenu;

private:
    BOOL LoadMule();
    void RegisterIpcServer();
    void UnRegisterIpcServer();
    int FirstLoadProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*);
    int FirstLoadEnd(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*);
    int RefreshProcess(easyipc::IEasyIpcBundle* pParam, easyipc::IEasyIpcBundle*);

	map<DWORD, SetProcessInfo> m_mapProInfo;
	map<DWORD, SetProcessInfo> m_mapProCache;
private:
	// RECT	m_oldRect;
	int		m_nItemWidth;
	unsigned int   m_flagSet; 
	int   m_SelectSum;

    KCriticalSesion  m_csNewItemNotify;
    CString m_strIpcName1;
    CString m_strIpcName2;
    CString m_strIpcName3;
    KEvent m_evtFirsetLoadEnd;

	int   m_nPid;
	KActor m_atorLogic;
	KEvent m_evtKillProcess;
	KEvent m_evtOutputAllProcess;
	KEvent m_evtOpenFilePos;
	KEvent m_evtOpenFileAtt;
};