#ifndef DLG_DLGINSUREOPEN_HEAD_FILE
#define DLG_DLGINSUREOPEN_HEAD_FILE

#pragma once
class CDlgInsure;
//////////////////////////////////////////////////////////////////////////////////

//��ͨ����
class CDlgInsureOpen: public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgInsureOpen();
	//��������
	virtual ~CDlgInsureOpen();

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
	virtual LPCTSTR GetSkinFile();

};

//////////////////////////////////////////////////////////////////////////////////

#endif