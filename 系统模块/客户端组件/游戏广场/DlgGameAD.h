#ifndef DLG_DLGGAMEAD_HEAD_FILE
#define DLG_DLGGAMEAD_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgGameAD : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgGameAD();
	//��������
	virtual ~CDlgGameAD();

	//���غ���
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