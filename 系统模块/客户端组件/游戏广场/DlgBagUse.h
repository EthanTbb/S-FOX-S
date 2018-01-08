#ifndef DLG_DLGBAGUSE_HEAD_FILE
#define DLG_DLGBAGUSE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//����ʹ��
class CDlgBagUse : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgBagUse();
	//��������
	virtual ~CDlgBagUse();

	void SetProperty(const tagBackpackProperty* pBackpackProperty);
	
	//��ȡ����
	int GetUseCount();
	//�Ƿ�ʹ��
	bool IsUseProperty();
	//�Ƿ�����
	bool IsGiveProperty();

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
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);

	//��������
	VOID InitData();

	VOID LoadDataFromMemory();

	VOID LoadDataFromUI();

	VOID UpdateData();

	//����ٿ�
	VOID InitUI();

	VOID UpdateUI();
	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

private:
	tagBackpackProperty		m_BackpackProperty;
	CNumberUI*				m_pNumberUseCount;
	CButtonUI*				m_pButtonIncrease;
	CButtonUI*				m_pButtonDecrease;
	CLabelUI*				m_pLabelUseAfterCount;
	int						m_nUseCount;
	bool                    m_bIsUseProperty;
	bool					m_bIsGiveProperty;
};

//////////////////////////////////////////////////////////////////////////////////

#endif