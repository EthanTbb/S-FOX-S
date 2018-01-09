#include "StdAfx.h"
#include ".\shbutton.h"

//ʱ���ʶ
#define IDI_SEND_MESSAGE				21									//��ʶ
#define TIME_SEND_MESSAGE				30									//ʱ��

CSHButton::CSHButton(void)
{
	m_bMouseDown = false;
}

CSHButton::~CSHButton(void)
{
}
BEGIN_MESSAGE_MAP(CSHButton, CSkinButtonEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CSHButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSkinButtonEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer( IDI_SEND_MESSAGE, TIME_SEND_MESSAGE, NULL);

	return 0;
}


// �滭����
void CSHButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	m_bMouseDown = ((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);
	CSkinButtonEx::DrawItem(lpDrawItemStruct);
}


// ��������
void CSHButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseDown = true;
	m_dwDownTime = GetTickCount();
	CSkinButtonEx::OnLButtonDown(nFlags, point);
}

// ʧȥ����
void CSHButton::OnKillFocus(CWnd* pNewWnd)
{
	m_bMouseDown = false;
	CSkinButtonEx::OnKillFocus(pNewWnd);
}

// �������
void CSHButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bMouseDown = false;
	CSkinButtonEx::OnLButtonUp(nFlags, point);
}

// ��ʱ��
void CSHButton::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == IDI_SEND_MESSAGE && m_bHovering && m_bMouseDown && GetTickCount() - m_dwDownTime > 800 )
	{
		UINT uControlID = GetWindowLong(m_hWnd,GWL_ID);
		::SendMessage(GetParent()->GetSafeHwnd(),WM_COMMAND,MAKELPARAM(uControlID,BN_CLICKED),(LPARAM)(::GetDlgItem(GetParent()->GetSafeHwnd(),uControlID)));
	}

	CSkinButtonEx::OnTimer(nIDEvent);
}



