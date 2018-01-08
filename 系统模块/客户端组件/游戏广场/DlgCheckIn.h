#ifndef DLG_CHECK_IN_HEAD_FILE
#define DLG_CHECK_IN_HEAD_FILE
#pragma once

/////////////////////////////////////////////////////////////////////////////////////////
//ǩ������
class CDlgCheckIn :	public CFGuiDialog,public CMissionItem
{
	//�Ƿ���ȡ
protected:
	bool							m_bButtonEnableMemberDayGift;
	bool							m_bButtonEnableMemberDayPresent;
	//ǩ������
protected:
	bool							m_bTodayChecked;					//ǩ����ʶ
	WORD							m_wDateIndex;						//��������
	SCORE							m_lRewardGold[LEN_WEEK];			//�������														

	//�����ʶ
protected:
	bool							m_bCheckInDone;						//ִ��ǩ��
	bool							m_bQueryCheckInfo;					//��ѯ��Ϣ	
	//��Ա��ʶ
protected:
	bool							m_bMemberQueryInfo;					//��Ա��ѯ
	bool							m_bMemberDayGift;					//��Ա���
	bool							m_bMemberDayPresent;				//��Ա�ͽ�	
	tagGiftPropertyInfo				m_GiftSub[50];						//��������
	DWORD							m_wGiftSubCount;					//��������
	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

public:
	CDlgCheckIn();
	virtual ~CDlgCheckIn();

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();

	//��ť�¼�
protected:
	//���ǩ��
	VOID OnButtonCheckInClicked();

	//������
	VOID OnButtonGiftClicked();

	//��������
protected:
	//ִ�в�ѯ
	virtual VOID PerformQueryCheckInfo();
	//ִ��ǩ��
	virtual VOID PerformCheckInDone();
	//��Ա���
	virtual VOID PerformMemberDayGift();
	//��Ա�ͽ�
	virtual VOID PerformMemberDayPresent();
	//��Ա��ѯ
	virtual VOID PerformMemberQueryInfo();

	//���»�Ա
	VOID UpdateMember();
	//���½���
	VOID UpdateCheckIn();
	//���ò���
	VOID SetSignLayout(LPCTSTR szLayoutName, const int nDay, const bool bSign, const int nSignGold);
	//�¼�����
public:
	//��ʼ����
	VOID OnMissionStart();
	//��ֹ����
	VOID OnMissionConclude();

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};


//����ǩ��
class CDlgCheckInServer : public CDlgCheckIn
{
	//��������
public:
	//���캯��
	CDlgCheckInServer();
	//��������
	virtual ~CDlgCheckInServer();

	//��������
protected:
	//ִ�в�ѯ
	virtual VOID PerformQueryCheckInfo();
	//ִ��ǩ��
	virtual VOID PerformCheckInDone();
	//��Ա���
	virtual VOID PerformMemberDayGift();
	//��Ա�ͽ�
	virtual VOID PerformMemberDayPresent();
	//��Ա��ѯ
	virtual VOID PerformMemberQueryInfo();
	//���ܺ���
public:
	//����
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//ǩ����Ϣ
	bool OnServerCheckInMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��



};


/////////////////////////////////////////////////////////////////////////////////////////
#endif
