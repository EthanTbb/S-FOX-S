#ifndef DLG_DLGSHOP_DETALES_HEAD_FILE
#define DLG_DLGSHOP_DETALES_HEAD_FILE

#pragma once
#include "ServerListData.h"

struct tagBuyGamePropertyInfo
{
	DWORD					dwPropertyID;						//����ID
	DWORD					dwBuyNumber;						//��������	
	BYTE					cbScoreType;						//��������
	BYTE					cbUse;								//�Ƿ�ʹ��
};

//ʹ�õ���
struct tagUseGamePropertyInfo
{
	DWORD							dwUserID;							//ʹ����
	DWORD							dwRecvUserID;						//���߶�˭ʹ��
	DWORD							dwPropID;							//����ID
	WORD							wPropCount;							//ʹ����Ŀ
};

enum BUY_MODE
{
	BUY_IMMEDIATELY_USE = 0,	//���������ʹ��
	BUY_INTO_BAG				//�������뱳��
};

//�̳ǽӿ�
interface IGamePropertyAction
{
	//��ѯ�¼�
	virtual VOID PerformQueryInfo()=NULL;	
	//�����¼�
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo)=NULL;
	//ʹ���¼�
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo)=NULL;
};
//////////////////////////////////////////////////////////////////////////////////

//���򴰿�
class CDlgShopDetails : public CFGuiDialog, public CMissionItem
{
	friend class CDlgShopMain;
	//��������
public:
	//���캯��
	CDlgShopDetails();
	//��������
	virtual ~CDlgShopDetails();

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
	void LoadDataFromExternal(CGamePropertyTypeItem *pGamePropertyTypeItem,CGamePropertyItem *pGamePropertyItem);

	//��������
	VOID InitData();

	VOID LoadDataFromMemory();

	VOID LoadDataFromUI();

	VOID UpdateData();

	//����ٿ�
	VOID InitUI();

	VOID UpdateUI();

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//
	void SetShopMainSink(IGamePropertyAction* pIGamePropertyAction);

	//����Ĭ��
	void SetDefaultBuy(int nValue);
	
	//�Ƿ��������ʹ��
	bool IsImmediatelyUseProp();

	//�������
protected:
	CMissionManager					m_MissionManager;						//�������

	//���߱���
protected:
	IGamePropertyAction				*m_pIGamePropertyAction;
	WORD							m_dwBuyNumber;							//��������
	BYTE							m_cbConsumeType;						//��������
	BUY_MODE						m_eBuyMode;								//��������
	CGamePropertyTypeItem			*m_pCurrGamePropertyTypeItem;			//��������
	CGamePropertyItem				*m_pCurrGamePropertyItem;				//��������
	int								m_CurrBagPropertyNum;					//��������

};

//////////////////////////////////////////////////////////////////////////////////

#endif