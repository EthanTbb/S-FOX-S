#ifndef DLG_DLGREAL_TIP_HEAD_FILE
#define DLG_DLGREAL_TIP_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////

//ʵ������
class CDlgRealTip : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgRealTip();
	//��������
	virtual ~CDlgRealTip();

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