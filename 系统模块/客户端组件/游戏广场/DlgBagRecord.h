#ifndef DLG_DLGBAGRECORD_HEAD_FILE
#define DLG_DLGBAGRECORD_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//�ҵı���
class CDlgBagRecord : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgBagRecord();
	//��������
	virtual ~CDlgBagRecord();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
public:
	//��ʼ����
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
};

//////////////////////////////////////////////////////////////////////////////////
#endif