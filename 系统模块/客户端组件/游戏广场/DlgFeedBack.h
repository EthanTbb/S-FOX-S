#ifndef DLG_DLGFEEDBACK_HEAD_FILE
#define DLG_DLGFEEDBACK_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgFeedBack : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgFeedBack();
	//��������
	virtual ~CDlgFeedBack();

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

	//web����
public:
	//JS����
	static jsValue JS_CALL js_msgBox(jsExecState es);
};

//////////////////////////////////////////////////////////////////////////////////

#endif