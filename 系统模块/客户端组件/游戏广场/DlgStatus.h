#ifndef DLG_STATUS_HEAD_FILE
#define DLG_STATUS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//״̬�ӿ�
interface IStatusViewSink
{
	//ȡ������
	virtual VOID OnStatusCancel()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//״̬����
class CDlgStatus : public CFGuiDialog
{

	//��������
public:
	//���캯��
	CDlgStatus();
	//��������
	virtual ~CDlgStatus();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���ܺ���
public:
	//�رմ���
	VOID HideStatusWindow();
	//������Ϣ
	VOID ShowStatusWindow(LPCTSTR pszDescribe);
	//���ýӿ�
	VOID SetStatusViewSink(IStatusViewSink * pIStatusViewSink);

	//��Ϣӳ��
protected:
	//ȡ����ť
	VOID OnBnClickedGiveUp();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

	//״̬����
protected:
	UINT							m_uImageIndex;						//λͼλ��
	bool							m_bFinished;						//��ɱ�־
	CString							m_strDescribe;						//������Ϣ

	//�ӿڱ���
protected:
	IStatusViewSink *				m_pIStatusViewSink;					//�ص��ӿ�

	//�ؼ�����
protected:
	CButtonUI *						m_pbtGiveUp;						//ȡ����ť
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
};

//////////////////////////////////////////////////////////////////////////////////

#endif