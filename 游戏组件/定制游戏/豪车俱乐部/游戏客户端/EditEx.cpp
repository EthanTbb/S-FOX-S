#include "StdAfx.h"
#include ".\editex.h"

CEditEx::CEditEx(void)
{
}

CEditEx::~CEditEx(void)
{
}
BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nChar == VK_RETURN)
	{
		GetParent()->SendMessage(WM_SENDEITD_MESSAGE);
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
