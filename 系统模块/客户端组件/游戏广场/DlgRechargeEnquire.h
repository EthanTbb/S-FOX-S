#ifndef DLG_DLGGAMEAD_HEAD_FILE
#define DLG_DLGGAMEAD_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��ֵ��ʾ
class CDlgRechargeEnquire : public CFGuiDialog
{
public:
	CString							m_strString;						//��ʾ��Ϣ
	//��������
public:
	//���캯��
	CDlgRechargeEnquire();
	//��������
	virtual ~CDlgRechargeEnquire();

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

public:
	VOID UpdateUI();

	INT ShowMessageBox( LPCTSTR pszString);
};

//////////////////////////////////////////////////////////////////////////////////

#endif