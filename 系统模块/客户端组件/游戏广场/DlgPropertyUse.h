#ifndef DLG_DLGPROPERTYUSE_HEAD_FILE
#define DLG_DLGPROPERTYUSE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//����ʹ��
class CDlgPropertyUseServer : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgPropertyUseServer();
	//��������
	virtual ~CDlgPropertyUseServer();

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
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��ʼ�ؼ�
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
	
	//����UI
	CVerticalLayoutUI* CreateItem(tagBackpackProperty& BackpackProperty);

	//����UI
	CContainerUI* FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID);

	//���µ���
	VOID UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty);
	
	//���µ���
	bool UpdateProperty(const tagBackpackProperty& BackpackProperty);

public:
	//���ýӿ�
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem* pIMySelfUserItem, IClientUserItem* pTargetUserItem);
	//��Ϣ����
	bool OnServerPropertyUseMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//��������
protected:
	ITCPSocket *				m_pITCPSocket;						//����ӿ�
	IClientUserItem*			m_pIMySelfUserItem;					//�Լ�ָ��
	IClientUserItem*			m_pTargetUserItem;					//Ŀ��ָ��
	vector<tagBackpackProperty> m_vBackPack;
	tagBackpackProperty			m_curSelectProp;					//ѡ�е���
	CPageLayoutUI*				m_pPageLayoutUI;
};

//////////////////////////////////////////////////////////////////////////////////

#endif