#ifndef DLG_SEARCH_USER_HEAD_FILE
#define DLG_SEARCH_USER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlazaViewServer.h"

//////////////////////////////////////////////////////////////////////////////////

//׷����Ϣ
struct tagTrackAccountsInfo
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	DWORD							dwTargetID;							//Ŀ���ʶ
};

//////////////////////////////////////////////////////////////////////////////////

//�����û�
class CDlgSearchUser : public CSkinDialog, public CMissionItem
{
	//��������
protected:
	DWORD							m_dwGameID;							//��Ϸ��ʶ
	TCHAR							m_szNickName[LEN_NICKNAME];			//�û��ǳ�

	//��Ϣ����
protected:
	tagUserRemoteInfo				m_UserRemoteInfo;					//Զ����Ϣ

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��̬����
protected:
	static CDlgSearchUser *			m_pDlgSearchUser;					//����ָ��

	//��������
public:
	//���캯��
	CDlgSearchUser();
	//��������
	virtual ~CDlgSearchUser();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("SearchUser.xml"); } 

	//���غ���
protected:
	//�滭����
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼�����
public:
	//����ر�
	VOID OnViewItemDelete(CPlazaViewServer * pServerViewItem);
	//�û�ɾ��
	VOID OnUserItemDelete(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem);
	//�û�����
	VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem);

	//��������
protected:
	//���غ���
	VOID LoadFriendUserInfo();

	//��̬����
public:
	//��ȡ����
	static CDlgSearchUser * GetInstance() { return m_pDlgSearchUser; }

	//��Ϣ����
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif