#ifndef DLG_TASK_CENTER_HEAD_FILE
#define DLG_TASK_CENTER_HEAD_FILE
#pragma once

//�����ļ�
#include "UITaskList.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//���Ͷ���
typedef CMap<WORD,WORD,tagTaskStatus *,tagTaskStatus *>  CTaskStatusMap;
typedef CMap<WORD,WORD,tagTaskParameter *,tagTaskParameter *>  CTaskParameterMap;

////////////////////////////////////////////////////////////////////////////////////////////////////
//����ָ��
class CDlgTaskGuide : public CFGuiDialog
{
	//��Դ����
protected:
	CButtonUI *						m_btLook;							//�鿴��ť
	CButtonUI *						m_btReturn;							//���ذ�ť	

	//��Դ����
protected:
	CFont							m_FontText;							//�������

	//����ؼ�
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��������
public:
	//���캯��
	CDlgTaskGuide();
	//��������
	virtual ~CDlgTaskGuide();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȡ���¼�
	virtual VOID OnCancel();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
class CDlgTaskNotifyer : public CFGuiDialog
{
	//��Ϣ����
protected:
	TCHAR							m_szTaskName[LEN_TASK_NAME];		//��������

	//��Դ����
protected:
	CFont							m_FontText;							//�������

	//����ؼ�
protected:
	CTextUI *						m_TextReward;						//������ǩ
	CButtonUI *						m_btClose;							//�رհ�ť
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��������
public:
	//���캯��
	CDlgTaskNotifyer();
	//��������
	virtual ~CDlgTaskNotifyer();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���ܺ���
public:
	//��������
	VOID SetTaskName(LPCTSTR pszTaskName);

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȡ���¼�
	virtual VOID OnCancel();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
class CDlgTaskCenter : public CSkinDialog
{
	//��������
protected:
	CFont									m_FontText;							//�������
	CRect									m_rcItemSelected;					//��������	

	//�������
protected:	
	WORD									m_wTaskID;							//�����ʶ
	tagTaskStatus *							m_pTaskStatus;						//����״̬	
	tagTaskParameter *						m_pTaskParameter;					//�������	

	//�洢����
protected:
	CTaskStatusMap							m_TaskStatusMap;					//����״̬
	CTaskParameterMap						m_TaskParameterMap;					//�������

	//�ؼ�����
protected:
	CTaskUI *								m_pTaskList;						//�����б�
	CButtonUI *								m_btTaskTake;						//��ȡ����
	CButtonUI *								m_btTakeReward;						//��ȡ����

	//�ڵ����
protected:
	Node *									m_pNodeSelfTask;					//�ҵ�����
	Node *									m_pNodeAllTask;						//��������

	//��������
public:
	//���캯��
	CDlgTaskCenter();
	//��������
	virtual ~CDlgTaskCenter();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//�滭�ӿ�
public:
	//�滭����
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
protected:
	//��������
	virtual VOID PerformLoadTask() { return; }
	//��ȡ����
	virtual VOID PerformTakeTask() { return; }
	//��ȡ����
	virtual VOID PerformRewardTask() { return; }

	//�¼�����
public:
	//��ʼ����
	virtual VOID OnMissionStart();
	//��ֹ����
	virtual VOID OnMissionConclude();

	//��������
protected:
	//�����ؼ�
	VOID RectifyControlUI();
	//Ĭ��ѡ��
	VOID SelectDefaultItem();
	//�������
	BOOL AddMineTaskItem(tagTaskStatus & TaskStatus);
	//�������
	BOOL AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize);
	//����״̬
	VOID UpdateTaskStatus(WORD wTaskID,BYTE cbTaskStatus);

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgTaskPlazaCenter : public CDlgTaskCenter,public CMissionItem
{
	//�����ʶ
protected:
	bool									m_bLoadTask;						//��������
	bool									m_bTakeTask;						//��ȡ����
	bool									m_bTaskReward;						//��ȡ����

	//�������
protected:
	CMissionManager							m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CDlgTaskPlazaCenter();
	//��������
	virtual ~CDlgTaskPlazaCenter();

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
protected:
	//��������
	virtual VOID PerformLoadTask();
	//��ȡ����
	virtual VOID PerformTakeTask();
	//��ȡ����
	virtual VOID PerformRewardTask();

};

////////////////////////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgTaskServerCenter : public CDlgTaskCenter
{
	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�

	//��������
public:
	//���캯��
	CDlgTaskServerCenter();
	//��������
	virtual ~CDlgTaskServerCenter();

	//���ܺ���
protected:
	//��������
	virtual VOID PerformLoadTask();
	//��ȡ����
	virtual VOID PerformTakeTask();
	//��ȡ����
	virtual VOID PerformRewardTask();

	//���ܺ���
public:
	//��������
	VOID Initialization(ITCPSocket * pITCPSocket);
	//������Ϣ
	bool OnTaskMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
