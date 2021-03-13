#ifndef __RB_UI_DemoListWndDlg_h_
#define __RB_UI_DemoListWndDlg_h_

#include "bkres/bkres.h"
#include "bkwin/CBkDialogViewImplEx.h"
#include "bkwin/bkshadowdrawable.h"
#include "bkwin/BkWndMsgFunction.h"

class SettingDlg
	: public CBkDialogViewImplEx<SettingDlg>
	, public BkWin::WndShadowHelper<SettingDlg>
	, public BkWin::CBkWndMsgFunction<SettingDlg>
{
public:
	SettingDlg();

	enum tagCHECKID{
		ID_CHECK_PRO_NAME		= 101,
		ID_CHECK_PRO_ID			,
		ID_CHECK_PRO_PATH		,
		ID_CHECK_PRO_CPU		,
		ID_CHECK_PRO_MEM		,

		ID_CHECK_PRO_EXT_1		= 106,
		ID_CHECK_PRO_EXT_2		,
		ID_CHECK_PRO_EXT_3		,
		ID_CHECK_PRO_EXT_4		,
		ID_CHECK_PRO_EXT_5		,
	};

	enum	tagBTN
	{
		IDC_BTN_CLOSE		=200,
		IDC_BTN_SURE		=201,
		IDC_BTN_CANCLE		=202,
		IDC_BTN_RECOVER		=203,
	};


protected:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN_EX)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CLOSE, OnBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SURE, OnBtnConfirm)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CANCLE, OnBtnCancle)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(SettingDlg)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN_EX)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		CHAIN_MSG_MAP(BkWin::CBkWndMsgFunction<SettingDlg>)
		CHAIN_MSG_MAP(CBkDialogViewImplEx<SettingDlg>)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	void OnBtnClose();
	void OnBtnConfirm();
	void OnBtnCancle();
	void OnSysCommand(UINT nID, CPoint point);


private:
	void ShowDoModalMsgBox(BkSharePtr<CString> msg);
	void InitSeting();
	void    SaveSetting();

	int   uFunRet; // 保存的标志位, 顺序读写
};

#endif //__RB_UI_DemoListWndDlg_h_
