#ifndef DLG_DLGPHONESCANCODE_HEAD_FILE
#define DLG_DLGPHONESCANCODE_HEAD_FILE
#pragma once
#include "ServerListData.h"
//////////////////////////////////////////////////////////////////////////////////

//��ά�봰��
class CPhoneScanCode : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CPhoneScanCode();
	//��������
	virtual ~CPhoneScanCode();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGui����
public:
	//��ʼ����
	virtual void InitControlUI();
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//�û�����
public:
	//��������
	void LoadDataFromExternal(CGameKindItem * pGameKind);
private:
	CGameKindItem * m_pGameKind;
};

//////////////////////////////////////////////////////////////////////////////////

//��ά�봰��
class CPhoneScanCodePlaza : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CPhoneScanCodePlaza();
	//��������
	virtual ~CPhoneScanCodePlaza();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGui����
public:
	//��ʼ����
	virtual void InitControlUI();
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
};

//////////////////////////////////////////////////////////////////////////////////
#endif