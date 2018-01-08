#ifndef DLG_ENQUIRE_HEAD_FILE
#define DLG_ENQUIRE_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//�رմ���
class CDlgEnquire : public CFGuiDialog
{

	//��������
public:
	//���캯��
	CDlgEnquire();
	//��������
	virtual ~CDlgEnquire();

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

	//��ť��Ϣ
protected:
	//�رմ���
	VOID OnBnClickedClosePlaza();
	//ȡ������
	VOID OnBnClickedCloseServer();
	//�л��ʺ�
	VOID OnBnClickedSwitchAccounts();

};

//////////////////////////////////////////////////////////////////////////////////

#endif