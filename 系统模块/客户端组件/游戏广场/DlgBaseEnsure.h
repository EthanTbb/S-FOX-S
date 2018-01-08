#ifndef DLG_BASE_ENSURE_HEAD_FILE
#define DLG_BASE_ENSURE_HEAD_FILE
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//�ͱ�����
class CDlgBaseEnsure : public CFGuiDialog,public CMissionItem
{
	//���Ʊ���
protected:
	bool							m_bLoadBaseEnsure;					//���صͱ�
	bool							m_bTakeBaseEnsure;					//��ȡ�ͱ�
	//�ڴ�����
protected:
	tagBaseEnsureParameter			m_BaseEnsureParameter;				//�ͱ�����
	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CDlgBaseEnsure();
	//��������
	virtual ~CDlgBaseEnsure();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();
	//�����¼�
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼�����
public:
	//��ʼ����
	virtual VOID OnMissionStart();
	//��ֹ����
	virtual VOID OnMissionConclude();

	//��������
protected:
	//���صͱ�
	virtual VOID PerformLoadBaseEnsure();
	//��ȡ�ͱ�
	virtual VOID PerformTakeBaseEnsure();
public:
	//ˢ�¿ؼ�
	void UpdateUI();
};
///////////////////////////////////////////////////////////////////////////////////////////////////

//�ͱ�����
class CDlgBaseEnsureServer : public CDlgBaseEnsure
{
	//��������
public:
	//���캯��
	CDlgBaseEnsureServer();
	//��������
	virtual ~CDlgBaseEnsureServer();

	//��������
protected:
	//���صͱ�
	virtual VOID PerformLoadBaseEnsure();
	//��ȡ�ͱ�
	virtual VOID PerformTakeBaseEnsure();

	//���ܺ���
public:
	//����
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//ǩ����Ϣ
	bool OnServerBaseEnsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

};

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
