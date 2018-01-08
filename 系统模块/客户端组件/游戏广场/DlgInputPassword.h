#ifndef DLG_INPUT_PASSWD_HEAD_FILE
#define DLG_INPUT_PASSWD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgInputPassword : public CFGuiDialog
{
	//��������
protected:
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����

	//�ؼ�����
protected:
	CButtonUI *						m_pbtOk;							//ȷ����ť
	CButtonUI *                     m_pbtCancel;                        //ȡ����ť	
	CEditUI *						m_pEditPassword;					//�û�����

	//��������
public:
	//���캯��
	CDlgInputPassword();
	//��������
	virtual ~CDlgInputPassword();

	//���غ���
protected:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����ť
	virtual VOID OnCancel();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���ܺ���
public:
	//��ȡ����
	VOID GetServerPasswd(TCHAR szServerPasswd[LEN_PASSWORD]);
};

#endif