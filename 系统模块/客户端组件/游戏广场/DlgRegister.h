#ifndef DLG_REGISTER_HEAD_FILE
#define DLG_REGISTER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//ע�ᴰ��
class CDlgRegister : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgRegister();
	//��������
	virtual ~CDlgRegister();
	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
	virtual VOID OnCancel();
	//���ܺ���
public:
	//��������
	WORD ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);
	//У����
	void OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg=NULL);
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile(); 
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();
	//��ʱ��֤
	void PerformVerifyIndividual(WORD wPreVerifyMask);
	//��ʾ��Ϣ
	void SelectedTipInfo(WORD wPreVerifyMask);
	//��ȡ�ַ�
	void GetControlString(CControlUI * pControlUI,TCHAR szString[], WORD wMaxCount);
	//��Ϣ����
public:
	//��������
	void OnEnChangeLogonPass();
	//������֤
	void OnBnClickedChangeValidate();
	//ͬ������
	void OnBnClickedAgrrArticle();
public:
	bool							m_bVerifyIndividual;				//��֤����
	WORD							m_wVerifyMask;						//У������							
	//��������
public:
	WORD							m_wFaceID;							//ͷ���ʶ
	BYTE							m_cbGender;							//�û��Ա�
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//ע���ʺ�
	TCHAR							m_szNickName[LEN_NICKNAME];			//ע���ǳ�
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//��¼����
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//��������
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//֤������
	TCHAR							m_szCompellation[LEN_COMPELLATION];	//��ʵ����
	WORD                            m_wLastVerifyAccountsResult;        //�ϴ���֤
	WORD                            m_wLastVerifyNickNameResult;        //�ϴ���֤
	WORD							m_dwAgentID;							//�����ʶ
	WORD							m_dwSpreaderGameID;					//�ƹ��ʶ

	//���ñ���
protected:
	BYTE							m_cbRemPassword;					//��ס����
	BYTE							m_cbLogonPassLevel;					//����ǿ��

};

//////////////////////////////////////////////////////////////////////////////////

#endif