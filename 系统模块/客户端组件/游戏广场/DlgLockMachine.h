#ifndef DLG_LOCK_MACHINE_HEAD_FILE
#define DLG_LOCK_MACHINE_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgLockMachine : public CFGuiDialog, public CMissionItem
{
public:
	//���캯��
	CDlgLockMachine();
	//��������
	virtual ~CDlgLockMachine();

	//MFC����
protected:
	//��ʼ����
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
	virtual VOID OnCancel();

	//FGui����
public:
	//��ʼ����
	virtual void InitControlUI();
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//������Ϣ
public:
	//��������
	void OnNotifyLock(TNotifyUI &  msg);
	//��������
	void OnNotifyUnLock(TNotifyUI &  msg);

	//�����¼�
public:
	//��ʼ����
	virtual VOID OnMissionStart();
	//��ֹ����
	virtual VOID OnMissionConclude();
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼���Ϣ
public:	
	//��ֹ�Ự
	bool ConcludeMission();
	//����Ự
	bool AvtiveMission(BYTE byMissionType);
	//ʧ�ܻỰ
	void OnMissionLinkError(INT nErrorCode);
	//ʧ�ܻỰ
	void OnMissionShutError(BYTE cbShutReason);

	//������Ϣ
public:	
	//�����Ự
	void OnMissionLinkLock();
	//�����ɹ�
	void OnMissionReadLockSuccess(VOID * pData, WORD wDataSize);
	//����ʧ��
	void OnMissionReadLockFailure(VOID * pData, WORD wDataSize);


	//�û���Ϣ
public:
	//��������
	void LoadDataFromUI();
	//��������
	void LoadDataFromMemory();
	//��������
	void UpdateDataToUI();
	//��������
	void UpdateDataToMemory();
	//��֤����
	bool EfficacyData();
	//ִ������
	void OnMachineLock(BYTE byLock);
	//��������
	void OnSendData();

	//��������
protected:
	BYTE							m_byMachine;						//�󶨱�־
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����
	//�������
protected:
	CMissionManager					m_MissionManager;					//�������
	BYTE							m_byMissionType;					//�Ự��ʶ

};

//////////////////////////////////////////////////////////////////////////////////

#endif