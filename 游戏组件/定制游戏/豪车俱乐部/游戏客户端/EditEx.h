#pragma once
#include "afxwin.h"

#define				WM_SENDEITD_MESSAGE			WM_USER+300				//�س���Ϣ

class CEditEx :	public CEdit
{
public:
	CEditEx(void);
	~CEditEx(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
