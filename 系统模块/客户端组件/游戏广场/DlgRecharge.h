#ifndef DLG_DLGRECHARGE_HEAD_FILE
#define DLG_DLGRECHARGE_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��ֵ����
class CDlgRecharge : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgRecharge();
	//��������
	virtual ~CDlgRecharge();

	//MFC����
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
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