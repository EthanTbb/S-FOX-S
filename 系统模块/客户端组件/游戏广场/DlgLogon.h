#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "DlgAccredit.h"
#include "Stdafx.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//��¼��ʽ
#define LOGON_BY_VISITOR			2									//�ο͵�¼
#define LOGON_BY_GAME_ID			1									//I D ��¼
#define LOGON_BY_ACCOUNTS			0									//�ʺŵ�¼

//////////////////////////////////////////////////////////////////////////////////

//�ʺ���Ϣ
struct tagAccountsInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szGameID[16];						//��Ϸ��ʶ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	time_t							tAcitveTime;						//��¼ʱ��
};

//���鶨��
typedef CWHArray<tagAccountsInfo *>	CAccountsInfoArray;					//�ʺ�����

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CDlgLogon : public CFGuiDialog
{

	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile(); 
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);

	//���ܺ���
public:
	//��ʾ��֤
	bool ShowAccreditWindow();
	//��������
	WORD ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();
	//�����ʺ�
	void LoadAccountsInfo();
	//��ַ��Ϣ
	void LoadLogonServerInfo();

	//��ť��Ϣ
protected:
	//ע���ʺ�
	void OnBnClickedRegister();
	//��������
	void OnBnForgetPassword();
	//ɾ����¼
	void OnBnClickedDeleteRecord(int index);

	//�ؼ�����
protected:
	//ѡ��ı�
	void OnSelchangeAccounts();
	//�༭�ı�
	void OnCbnEditchangeAccounts();

	//���ñ���
public:
	BYTE							m_cbLogonMode;						//��¼ģʽ
	BYTE							m_cbRemPassword;					//��ס����

	//��¼��Ϣ
public:
	DWORD							m_dwGameID;							//��Ϸ I D
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//��Ϸ�ʺ�
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							m_szLogonServer[LEN_SERVER];		//��¼��ַ
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//���֤��
	//����ؼ�
protected:
	CDlgAccredit *					m_pDlgAccedit;						//��֤����
	//��������
protected:
	CAccountsInfoArray				m_AccountsInfoArray;				//�ʺ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif