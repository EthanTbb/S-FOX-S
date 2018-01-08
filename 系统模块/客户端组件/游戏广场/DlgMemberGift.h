#ifndef DLG_MEMBER_GIFT_HEAD_FILE
#define DLG_MEMBER_GIFT_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��Ա����
class CDlgMemberGift : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgMemberGift();
	//��������
	virtual ~CDlgMemberGift();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���غ���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

public:
	//��������
	void LoadDataFromMemory();

	//����ҳ��
	void CreateUI();

	//ˢ�½���
	void UpdateUI();
public:
	//������ȡ
	void SetEnableReceive(bool EnableReceive);

public:
	vector<CGamePropertyItem *>		m_vGamePropertyList;
	vector<CGamePropertySubItem*>	m_vGamePropertySubList;		
	bool							m_bEnableReceive;
};

//////////////////////////////////////////////////////////////////////////////////

#endif