#ifndef DLG_TABLE_PASSWORD_HEAD_FILE
#define DLG_TABLE_PASSWORD_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////
//��������
class CDlgTablePassword : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgTablePassword();
	//��������
	virtual ~CDlgTablePassword();

	//MFC����
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGUI����
public:
	//��ʼ����
	virtual void InitControlUI();
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//��������
public:
	TCHAR								m_szPassword[LEN_PASSWORD];		//��������
};

//////////////////////////////////////////////////////////////////////////

#endif