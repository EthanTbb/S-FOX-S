#ifndef DLG_DLGGAME_HEAD_FILE
#define DLG_DLGGAME_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgGame : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgGame();
	//��������
	virtual ~CDlgGame();

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