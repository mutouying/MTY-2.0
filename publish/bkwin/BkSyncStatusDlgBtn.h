//********************************************************************
//author:  hexujun@cmcm.com
//create time: 2020/6/22?13:07
//description: ÃèÊö
//*********************************************************************

#ifndef _H_BKSYNCSTATUSDLGBTN_H_
#define _H_BKSYNCSTATUSDLGBTN_H_

#include "bkwin\bkwnddlg.h"

class CBkSyncStatusDlgBtn
	: public CBkDialogButton
{
public:
	BKOBJ_DECLARE_CLASS_NAME(CBkSyncStatusDlgBtn, "syncstatusbtn")

public:
	// Modify BkWindow state
	virtual DWORD ModifyState(DWORD dwStateAdd, DWORD dwStateRemove)
	{
		ModifyChildsStatus(dwStateAdd, dwStateRemove);

		return __super::ModifyState(dwStateAdd, dwStateRemove);
	}

	virtual BOOL NeedRedrawWhenStateChange() 
	{
		return TRUE;
	}

protected:

	void ModifyChildsStatus(DWORD dwStateAdd, DWORD dwStateRemove) 
	{
		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);
			pBkWndChild->ModifyState(dwStateAdd, dwStateRemove);
		}
	}
};

#endif