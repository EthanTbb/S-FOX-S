#ifndef DLG_DLGCONTACT_HEAD_FILE
#define DLG_DLGCONTACT_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////
//��ϵ����
class CDlgContact : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgContact();
	//��������
	virtual ~CDlgContact();

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

};

//////////////////////////////////////////////////////////////////////////////////
#endif