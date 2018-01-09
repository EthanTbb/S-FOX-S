#pragma once
#include "afxwin.h"

#define				WM_SENDEITD_MESSAGE			WM_USER+300				//回车消息

class CEditEx :	public CEdit
{
public:
	CEditEx(void);
	~CEditEx(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
