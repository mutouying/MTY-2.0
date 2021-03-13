#pragma once

#include "bkres/bkres.h"
#include "bkwin/CBkDialogViewImplEx.h"
#include "bkwin/bkshadowdrawable.h"
#include "bkwin/CBkDialogMenu.h"

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
    };

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN_EX)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MIN, OnBtnMin)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CLOSE, OnBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SETTING,OnSetting)
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

protected:
	virtual void OnBkMenuCmd(CBkDialogMenu*  pDialogMenu, LPCWSTR lpszMenuName);
	void OnBtnMin();
protected:
	CBkDialogMenu* m_pMenu;

private:
	RECT					m_oldRect;
};