#ifndef DLG_DLGSETUP_HEAD_FILE
#define DLG_DLGSETUP_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//���ô���
class CDlgSetup : public CFGuiDialog
{
public:
	//���캯��
	CDlgSetup();
	//��������
	virtual ~CDlgSetup();

	//MFC����
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGUI����
public:
	//��ʼ����
	virtual void InitControlUI();
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//�û�����
public:
	//��ʼ����
	VOID InitDataLocation();
	//��������
	VOID LoadDataFromMemory();
	//��������
	VOID LoadDataFromUI();
	//ˢ������
	VOID UpdateDataToMemory();
	//ˢ������
	VOID UpdateDataToUI();
	//Ĭ�Ϻ���
	VOID OnDefault();
	//Ӧ�ú���
	VOID DoApply();
	//ע�ắ��
	void RegeditHotkey();

public:
	//��������
	WORD							m_wBossHotKey;						//�ϰ��ȼ�
	WORD							m_wWhisperHotKey;					//˽���ȼ�
	bool							m_bSendWhisperByEnter;				//�س�����
	bool							m_bFullScreenBrowse;				//ȫ�����
	bool							m_bAutoReceiveBase;					//��ȡ�ͱ�
	bool							m_bNotifyUserInOut;					//������Ϣ
	bool							m_bSalienceTable;					//ͻ������

	//Ĭ������
	WORD							m_wDefaultBossHotKey;				
	WORD							m_wDefaultWhisperHotKey;	
	bool							m_bDefaultSendWhisperByEnter;	
	bool							m_bDefaultFullScreenBrowse;			
	bool							m_bDefaultAutoReceiveBase;			
	bool							m_bDefaultNotifyUserInOut;			
	bool							m_bDefaultSalienceTable;			
};

//////////////////////////////////////////////////////////////////////////////////

#endif