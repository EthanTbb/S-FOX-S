#ifndef DLG_DLGTASK_HEAD_FILE
#define DLG_DLGTASK_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////////////
//���Ͷ���
typedef CMap<WORD,WORD,tagTaskStatus *,tagTaskStatus *>  CTaskStatusMap;
typedef CMap<WORD,WORD,tagTaskParameter *,tagTaskParameter *>  CTaskParameterMap;

//���񴰿�
class CDlgTask : public CFGuiDialog ,public CMissionItem
{
	//�����ʶ
protected:
	bool									m_bLoadTask;						//��������
	bool									m_bTakeTask;						//��ȡ����
	bool									m_bTaskReward;						//��ȡ����
	bool									m_bTaskGiveUp;						//��������
	//�������
protected:	
	WORD									m_wTaskID;							//�����ʶ
	tagTaskStatus *							m_pTaskStatus;						//����״̬	
	tagTaskParameter *						m_pTaskParameter;					//�������	

	//�洢����
protected:
	CTaskStatusMap							m_TaskStatusMap;					//����״̬
	CTaskParameterMap						m_TaskParameterMap;					//��������
	//�������
protected:
	CMissionManager							m_MissionManager;					//�������
	//��ʾ����
protected:
	vector<tagTaskParameter *> m_vecReceiving;
	vector<tagTaskParameter *> m_vecReceived;
	vector<tagTaskParameter *> m_vecCompleted;
	vector<tagTaskParameter *> m_vecFail;
	vector<tagTaskParameter *> m_vecAward;
	//��������
public:
	//���캯��
	CDlgTask();
	//��������
	virtual ~CDlgTask();

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
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

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
	//���ܺ���
protected:
	//��������
	virtual VOID PerformLoadTask();
	//��ȡ����
	virtual VOID PerformTakeTask();
	//��ȡ����
	virtual VOID PerformRewardTask();
	//��������
	virtual VOID PerformGiveUpTask();	

public:
	//�������
	BOOL AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize);
	//�������
	BOOL AddMineTaskItem(tagTaskStatus & TaskStatus);
	//ɾ������
	BOOL DeleteMineTaskItem(tagTaskStatus & TaskStatus);
	//��������
	VOID LoadDataFromMemory();

	//��������
	VOID AddTask(CPageLayoutUI * page,tagTaskParameter * pTaskParameter);

	//ˢ��UI
	VOID UpdateUI();

	//��ȡ����
	VOID ViewTaskReceiving(CControlUI * pControlUI);
	//��������
	VOID ViewTaskReceived(CControlUI * pControlUI);
	//��ȡ����
	VOID ViewTaskCompleted(CControlUI * pControlUI);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgTaskServer : public CDlgTask
{
	//��������
public:
	//���캯��
	CDlgTaskServer();
	//��������
	virtual ~CDlgTaskServer();

	//���ܺ���
protected:
	//��������
	virtual VOID PerformLoadTask();
	//��ȡ����
	virtual VOID PerformTakeTask();
	//��ȡ����
	virtual VOID PerformRewardTask();
	//��������
	virtual VOID PerformGiveUpTask();	

	//���ܺ���
public:
	//��������
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//������Ϣ
	bool OnTaskMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif