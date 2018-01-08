#ifndef DLG_DLGINDIVIDUAL_HEAD_FILE
#define DLG_DLGINDIVIDUAL_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgIndividual : public CFGuiDialog, public CMissionItem
{
	//��������
public:
	//���캯��
	CDlgIndividual();
	//��������
	virtual ~CDlgIndividual();

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
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//�����¼�
public:
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
	//��������
protected:
	//��ѯ����
	void OnMissionLinkIndividualLoad();
	//�޸�����
	void OnMissionLinkIndividualModify();
	//ʵ����֤
	void OnMissionLinkIndividualAuth();
	//��¼����
	void OnMissionLinkLogonModify();
	//��������
	void OnMissionLinkInsureModify();
	//��������
	void OnMissionLinkInsureSet();

	//���ܺ���
public:
	//���½���
	void UpdateUI();
	//��������
	void UpdateData();
	//��������
	void LoadDataFromMemory();
	//����ͷ��
	void UpdateUserFace();
	//�����ַ�
	VOID SetControlItemText(LPCTSTR pszControlName,LPCTSTR pszText);
	//��ȡ�ַ�
	VOID GetControlItemText(LPCTSTR pszControlName, TCHAR szString[], WORD wMaxCount);
	//����ǿ��
	VOID SetPassLevel(LPCTSTR szPowerScale, LPCTSTR szPassControl);
	//���ý���
	VOID SetControlFocus(LPCTSTR pszControlName);
	//��֤�˻�
	bool EfficacyStandard();
	//��֤��ϸ
	bool EfficacyExtendInfo();
	//��֤ʵ��
	bool EfficacyReal();
	//��֤��¼
	bool EfficacyModifyLogonPassward();
	//��֤����
	bool EfficacyModifyInsurePassward();
	//��֤����
	bool EfficacyOpenInsurePassward();

	//���ܺ���
public:
	//����ҳ��
	void SetTabSelectedIndex(BYTE cbIndex = 0);

	//�������
protected:
	BYTE							m_byMissionType;						//��������
	CMissionManager					m_MissionManager;						//�������

	//ҳ�����
protected:
	BYTE							m_cbSelectedIndex;						//����ҳ��
	tagCustomFaceInfo96				m_CustomFaceInfo96;						//�Զ����ͷ��96*96
};

//////////////////////////////////////////////////////////////////////////////////

#endif