#ifndef DLG_DLGBAGPRESEND_HEAD_FILE
#define DLG_DLGBAGPRESEND_HEAD_FILE

#pragma once
enum PRESEND_MODE
{
	PRESEND_NICKNAME,
	PRESEND_GAMEID
};
//////////////////////////////////////////////////////////////////////////////////

//��������ʹ��
class CDlgBagPresent: public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgBagPresent();
	//��������
	virtual ~CDlgBagPresent();

	void SetProperty(const tagBackpackProperty* pBackpackProperty);
	
	//��ȡ�õ��ĸ���
	int GetUseCount();

	//�Ƿ����͵���
	bool IsGiveProperty();

	//����ģʽ
	PRESEND_MODE GetPresendMode();

	//�����������
	CStdString GetEditReceiveName();

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
	CEditUI*				m_pEditReceiveName;
	int						m_nUseCount;
	bool					m_bIsGiveProperty;
	PRESEND_MODE			m_ePresendMode;
};

//////////////////////////////////////////////////////////////////////////////////

#endif