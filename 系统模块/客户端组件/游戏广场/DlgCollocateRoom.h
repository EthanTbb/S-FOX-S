#pragma once

////////////////////////////////////////////////////////////////////////////////////////
//��������
class CDlgCollocateRoom : public CFGuiDialog
{
	//���ñ���
protected:
	CParameterGame *				m_pParameterGame;					//��Ϸ����
	CParameterServer *				m_pParameterServer;					//��������

	//ʤ������
public:
	WORD							m_wMinWinRate;						//���ʤ��
	bool							m_bLimitWinRate;					//����ʤ��

	//��������
public:
	WORD							m_wMaxFleeRate;						//�������
	bool							m_bLimitFleeRate;					//���ƶ���

	//��������
public:
	DWORD							m_lMaxGameScore;					//��߷��� 
	DWORD							m_lMinGameScore;					//��ͷ���
	bool							m_bLimitGameScore;					//���Ʒ���

	//��������
public:
	bool							m_bLimitDetest;						//�������
	bool							m_bLimitSameIP;						//Ч���ַ

public:
	bool							m_bTakePassword;					//�����־
	TCHAR							m_szTablePassword[LEN_PASSWORD];    //��������

	//��������
public:
	//���캯��
	CDlgCollocateRoom();
	//��������
	virtual ~CDlgCollocateRoom();

	//���ܺ���
public:
	//���ò���
	bool InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer);

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgCollocateRoom.xml"); } 
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
    virtual VOID Notify(TNotifyUI &  msg);	

	//���غ���
public:
	//�������
	virtual bool SaveParameter();
	//Ĭ�ϲ���
	virtual bool DefaultParameter();
	//���¿���
	virtual bool UpdateControlStatus();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//��������
protected:
	//��ȡ״̬
	bool GetButtonCheckStatus(LPCTSTR pszButtonName);
	//��ȡ�ַ�
	VOID GetControlText(LPCTSTR pszControlName,TCHAR szString[], WORD wMaxCount);	
};

////////////////////////////////////////////////////////////////////////////////////////
