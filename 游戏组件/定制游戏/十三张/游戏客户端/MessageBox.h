#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

#define	IDM_MSG_INFO			(WM_USER+1000)							  //ȷ����Ϣ
//////////////////////////////////////////////////////////////////////////


//��Ϸ����
class CMessageBox : public CDialog
{
	//��������
protected:
	CString							m_szMsg;								//�ַ���
	UINT							m_nTimerCount;							//ʣ��ʱ��

	//�ؼ�����
protected:
	CSkinButton						m_btOK;									//ȷ����ť
	CSkinButton						m_btClose;								//ȡ����ť

	//��Դ����
protected:
	CBitImage						m_ImageBack;							//����ͼ

	//��������
public:
	//���캯��
	CMessageBox();
	//��������
	virtual ~CMessageBox();

	//���ܺ���
public:
	//������ʾ
	void ShowMessage(LPCTSTR pszMsg);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣӳ��
protected:

	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//��ʱ��Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
