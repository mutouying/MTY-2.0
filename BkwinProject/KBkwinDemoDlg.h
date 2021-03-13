#pragma once

#include "bkres/bkres.h"
#include "bkwin/CBkDialogViewImplEx.h"
#include "bkwin/bkshadowdrawable.h"
#include "bkwin/CBkDialogMenu.h"
#include "ProcessMgr/ProcessDefineEx.h"
#include "framework/KTinyXml.h"

using namespace std;
class KBkwinDemoDlg
    : public CBkDialogViewImplEx<KBkwinDemoDlg>
	, public BkWin::WndShadowHelper<KBkwinDemoDlg>
	, public CBkDialogMenu::IBKMenuCmdListener
{
public:
    KBkwinDemoDlg();
	~KBkwinDemoDlg();

    enum
    {
		IDC_BTN_MIN								=	100,
		IDC_BTN_CLOSE							=	102,
		IDC_BTN_SETTING							=	103,

		IDC_LIST_PROC							= 1000,
		IDC_LIST_KEY							= 1001,
    };

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN_EX)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MIN, OnBtnMin)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CLOSE, OnBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SETTING,OnSetting)

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
	
		CHAIN_MSG_MAP(CBkDialogViewImplEx<KBkwinDemoDlg>)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
    void OnBtnClose();
    void OnSysCommand(UINT nID, CPoint point);
	void OnSetting();

	void OnListItemLButtonUp( int nListItem);
	void OnListItemMouseHover(int nListItem);
	void OnListItemMouseLeave(int nListItem);
	void OnListItemChildLBtnUp(int nListItem, int nChildCtrlId);

	void UpdateWindowsUI();
	BOOL AddOnekeyToListWnd(SetProcessInfo& data , int nIndex);
	BOOL CreateHotkeyListItemXml(SetProcessInfo & data, KTinyXml tinyXml, int nIndex );
	void testCode();
	void UpdateCacheMapToRealMap();
protected:
	virtual void OnBkMenuCmd(CBkDialogMenu*  pDialogMenu, LPCWSTR lpszMenuName);
	void OnBtnMin();
protected:
	CBkDialogMenu* m_pMenu;

	map<DWORD, SetProcessInfo> m_mapProInfo;
	map<DWORD, SetProcessInfo> m_mapProCache;
private:
	RECT					m_oldRect;
};