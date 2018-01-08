#ifndef DLG_ACCREDIT_HEAD_FILE
#define DLG_ACCREDIT_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��֤����
class CDlgAccredit : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgAccredit();
	//��������
	virtual ~CDlgAccredit();

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
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgAccredit.xml"); } 

	//��Ϣ����
public:
	//��ȡ����
	LPCTSTR GetPassPortNumber();

	//��������
private:
	//�������
	VOID RandPassportNumber();

	//��ť�¼�
protected:
	//���ð�ť
	VOID OnClickedButtonReset(CControlUI * pControlUI);
	//���ְ�ť
	VOID OnClickedButtonNumber(CControlUI * pControlUI);
	//�˸�ť
	VOID OnClickedButtonBackSpace(CControlUI * pControlUI);
};

//////////////////////////////////////////////////////////////////////////////////

#endif