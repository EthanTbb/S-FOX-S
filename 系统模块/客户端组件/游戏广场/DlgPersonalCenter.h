#ifndef DLG_DLGPERSONALCENTER_HEAD_FILE
#define DLG_DLGPERSONALCENTER_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgPersonalCenter : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgPersonalCenter();
	//��������
	virtual ~CDlgPersonalCenter();

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
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgPersonalCenter.xml"); } 

};

//////////////////////////////////////////////////////////////////////////////////

#endif