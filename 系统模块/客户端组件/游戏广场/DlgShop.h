#ifndef DLG_DLGSHOP_HEAD_FILE
#define DLG_DLGSHOP_HEAD_FILE

#pragma once
#include "ServerListData.h"
#include "DlgShopDetails.h"
//////////////////////////////////////////////////////////////////////////////////

//�̳Ǵ���
class CDlgShopMain : public CFGuiDialog, public IGamePropertyAction
{
	//��������
public:
	//���캯��
	CDlgShopMain();
	//��������
	virtual ~CDlgShopMain();
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
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//�û�����
public:

	//��������
	VOID InitData();



	VOID LoadDataFromUI();

	VOID UpdateData();

	//����ٿ�
	VOID InitUI();

	VOID UpdateUI();

	//�����ڴ�
	void LoadDataFromMemory();
	//����ҳ��
	void CreateUI();

public:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//��ѯ�¼�
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo);
	//ʹ���¼�
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo);

	//��������
public:
	//��������
	VOID SetPropertyInfo(CPageLayoutUI* pPageLayoutUI, tagPropertyInfo *ptagPropertyInfo, BYTE cbIndex = 0);
	
	//��ʾ�ɹ���Ϣ
	void ShowSuccessMessage(const CMD_GP_S_PropertyUse* pPropertyUse);

	//�������
protected:
	//CMissionManager				m_MissionManager;						//�������
	bool							m_bQueryProperty;						//��ѯ����
	bool							m_bBuyProperty;							//�������
	bool							m_bUseProperty;							//ʹ�õ���
	tagUseGamePropertyInfo			m_PropertyUseInfo;						//ʹ����Ϣ
	
	//�ؼ�����
protected:	
	CDlgShopDetails*				m_pDlgShopDetails;
public:					
	tagBuyGamePropertyInfo			m_tagBuyGamePropertyInfo;

public:
	vector<CGamePropertyTypeItem *> m_vGamePropertyTypeList;
	vector<CGamePropertyRelatItem *> m_vGamePropertyRelatList;
	vector<CGamePropertyItem *> m_vGamePropertyList;
	CGamePropertyTypeItem * m_CurrGamePropertyType;
};

//�̳Ǵ���
class CDlgShopPlaza : public CDlgShopMain, public CMissionItem
{
	//��������
public:
	//���캯��
	CDlgShopPlaza();
	//��������
	virtual ~CDlgShopPlaza();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();

public:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//��ѯ�¼�
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo);
	//ʹ���¼�
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo);

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�������
protected:
	CMissionManager					m_MissionManager;						//�������


};

//�����̳�
class CDlgShopServer : public CDlgShopMain
{
	//��������
public:
	//���캯��
	CDlgShopServer();
	//��������
	virtual ~CDlgShopServer();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();

public:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//��ѯ�¼�
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo);
	//ʹ���¼�
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo);

	//���ܺ���
public:
	//��������
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//������Ϣ
	bool OnServerShopMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	
	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
};

//////////////////////////////////////////////////////////////////////////////////

#endif