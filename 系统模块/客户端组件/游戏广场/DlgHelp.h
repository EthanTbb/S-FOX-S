#ifndef DLG_DLGHELP_HEAD_FILE
#define DLG_DLGHELP_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgHelp : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgHelp();
	//��������
	virtual ~CDlgHelp();

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