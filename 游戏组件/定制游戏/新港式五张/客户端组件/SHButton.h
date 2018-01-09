#pragma once

#include "SkinButtonEx.h"

class CSHButton : public CSkinButtonEx
{
public:
	bool				m_bMouseDown;			// ������
	DWORD				m_dwDownTime;			// ���ʱ��

public:
	CSHButton(void);
	~CSHButton(void);

	// ��Ϣ����
public:
	// ��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	// ������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ʧȥ����
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// �������
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// ��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

};
