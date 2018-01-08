#ifndef DLG_MYSPREAD_HEAD_FILE
#define DLG_MYSPREAD_HEAD_FILE
#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////

//�ƹ㴰��
class CDlgMySpread : public CFGuiDialog,public CMissionItem
{
public:
	//���캯��
	CDlgMySpread();
	//��������
	virtual ~CDlgMySpread();

    //MFC����
protected:
	//��������
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
	//���ƺ���
	void OnNotifyCopy(TNotifyUI &  msg);
	//���Ӻ���
	void OnNotifyLink(TNotifyUI &  msg);

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
	//���ػỰ
	void OnMissionLinkLoad();
	//���ػỰ
	void OnMissionReadLoad(VOID * pData, WORD wDataSize);

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
	//���ؽ���
	void PerformLoadSpreadReward();

	//���ñ���
protected:
	tagUserSpreadInfo				m_UserSpreadInfo;					//�ƹ���Ϣ
	TCHAR							m_szSpreadLink[MAX_PATH];			//�ƹ�����
	//�������
protected:
	CMissionManager					m_MissionManager;					//�������
	BYTE							m_byMissionType;					//�Ự��ʶ

};
///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
