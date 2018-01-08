#ifndef DLG_EXCHANGE_HEAD_FILE
#define DLG_EXCHANGE_HEAD_FILE
#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////
//�һ�����
class CDlgExchange : public CSkinDialog
{
	//��ʾ����
protected:
	SCORE							m_lExchangeScore;					//�һ�����
	SCORE							m_lConsumeBeans;					//������Ϸ��
	SCORE							m_lPresentScore;					//���ͻ���

	//��������
protected:
	DWORD							m_dwExchangeRate;					//�һ�����
	WORD							m_wMemberCount;						//��Ա��Ŀ
	tagMemberParameter				m_MemberParameter[10];				//��Ա����

	//��������
public:
	//���캯��
	CDlgExchange();
	//��������
	virtual ~CDlgExchange();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgExchange.xml"); }

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//�滭�ӿ�
public:
	//�滭����
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);	
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//�¼�����
public:
	//��ʼ����
	virtual VOID OnMissionStart();
	//��ֹ����
	virtual VOID OnMissionConclude();

	//ִ�к���
protected:
	//�һ���Ϸ��
	virtual VOID PerformExchangeScore();
	//�����Ա
	virtual VOID PerformPurchaseMember();
	//���ز���
	virtual VOID PerformLoadExchangeParam();

	//�ؼ��¼�
protected:
	//ȷ�϶һ�
	VOID OnClickedConfirmExchange();
	//ȷ�Ϲ���
	VOID OnClickedConfirmPurchase();
	//����һ�
	VOID OnTextLinkMoreExchange();
	//�ı����
	VOID OnEditIngotTextChange();
	//������
	VOID OnComboControlItemChange();

	//��������
protected:
	//��ȡ����
	SCORE GetUserControlNumber(LPCTSTR pszNumber);
	//ת���ַ�
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//�����һ�
class CDlgExchangePlaza : public CDlgExchange,public CMissionItem
{
	//���Ʊ���
protected:
	bool							m_bLoadExchangeParam;				//���ز���
	bool							m_bPurchaseMember;					//�����Ա
	bool							m_bExchangeScore;					//�һ���Ϸ��

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CDlgExchangePlaza();
	//��������
	virtual ~CDlgExchangePlaza();

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//ִ�к���
protected:
	//�һ���Ϸ��
	virtual VOID PerformExchangeScore();
	//�����Ա
	virtual VOID PerformPurchaseMember();
	//���ز���
	virtual VOID PerformLoadExchangeParam();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

//����һ�
class CDlgExchangeServer : public CDlgExchange
{
	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�

	//��������
public:
	//���캯��
	CDlgExchangeServer();
	//��������
	virtual ~CDlgExchangeServer();

	//ִ�к���
protected:
	//�һ���Ϸ��
	virtual VOID PerformExchangeScore();
	//�����Ա
	virtual VOID PerformPurchaseMember();
	//���ز���
	virtual VOID PerformLoadExchangeParam();

	//���ܺ���
public:
	//��������
	VOID Initialization(ITCPSocket * pITCPSocket);
	//������Ϣ
	bool OnExchangeMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
