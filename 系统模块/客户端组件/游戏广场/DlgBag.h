#ifndef DLG_DLGBAG_HEAD_FILE
#define DLG_DLGBAG_HEAD_FILE

#pragma once

#include "DlgBagPresent.h"
class CDlgBagUse;
//////////////////////////////////////////////////////////////////////////////////

//�ҵı���
class CDlgBagMain : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgBagMain();
	//��������
	virtual ~CDlgBagMain();
	//�Ƿ�ʹ��
	bool IsUseProperty();
	//�Ƿ�����
	bool IsGiveProperty();
	//��������
	void EndDialog();

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
	//�������
	VOID Clear();

	//��������
	CVerticalLayoutUI* CreateItem(tagBackpackProperty& BackpackProperty);

	//���µ���
	VOID UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty);

	//�������
	bool AddByType(CContainerUI* pVerticalLayoutUI, const int nType=-1);

	//����UI
	CContainerUI* FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID);

	//���µ���
	bool UpdateProperty(const tagBackpackProperty& BackpackProperty);

	//��ʾʹ��
	virtual INT_PTR ShowUseProperty(const tagBackpackProperty* pBackpackProperty);

	//���ٵ���
	bool ReduceProperty(const DWORD dwPropID, const DWORD dwPropCount);

	//�ɹ���Ϣ
	void ShowSuccessMessage(const CMD_GP_S_PropertyUse* pPropertyUse);
	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//�ؼ�����
private:
	CPageLayoutUI*				m_pPageLayoutUI[5];
	CDlgBagUse*					m_pDlgBagUse;

public:
	vector<tagBackpackProperty> m_vBackPack;
	tagBackpackProperty			m_curSelectProp;		//ѡ�е���
	bool						m_bIsUseProperty;
	bool						m_bIsGiveProperty;
	CStdString					m_sGiveReceiveName;		//�����ǳ�
	DWORD						m_dwGiveReceiveID;		//����I D
	PRESEND_MODE				m_ePresendMode;			//���ͷ�ʽ
};

//////////////////////////////////////////////////////////////////////////////////

//�ҵı���(����)
class CDlgBagPlaza : public CDlgBagMain, public CMissionItem
{
	//��������
public:
	//���캯��
	CDlgBagPlaza();
	//��������
	virtual ~CDlgBagPlaza();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

public:
	//ʹ�õ���
	virtual INT_PTR ShowUseProperty(const tagBackpackProperty* pBackpackProperty);
	
	//��������
	bool AvtiveMission();
	bool QuerySendPresentMisson();

private:
	CMissionManager				m_MissionManager;		//�������
	bool						m_bQueryBackpack;		//��ѯ����
	bool						m_bUseBackpackProp;		//ʹ����Ʒ
	bool						m_bGiveBackpackProp;	//������Ʒ
	bool						m_bGetSendPresent;		//��ȡ����
};

///////////////////////////////////////////////////////////////////////////////


//�ҵı���
class CDlgBagServer : public CDlgBagMain
{
	//��������
public:
	//���캯��
	CDlgBagServer();
	//��������
	virtual ~CDlgBagServer();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();

public:
	//��ѯ�¼�
	VOID PerformQueryInfo();
	//ʹ����Ʒ
	VOID PerformUseProperty();
	//��ѯ����
	VOID PerformQuerySendPresent();

	//���ܺ���
public:
	//����
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//������Ϣ
	bool OnServerBagMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

public:
	//��ʾʹ��
	virtual INT_PTR ShowUseProperty(const tagBackpackProperty* pBackpackProperty);
	//��ѯ����
	bool QuerySendPresentMisson();

	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
	bool							m_bQueryBackpack;					//��ѯ����
	bool							m_bUseBackpackProp;					//ʹ����Ʒ
};

#endif