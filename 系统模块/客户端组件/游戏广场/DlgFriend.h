#ifndef DLG_DLGFRIEND_HEAD_FILE
#define DLG_DLGFRIEND_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////////////


class CDlgFriend : public CFGuiDialog, public CMissionItem
{
	//��������
public:
	//���캯��
	CDlgFriend();
	//��������
	virtual ~CDlgFriend();
	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//���غ���
protected:
	//��ʼ��
	void InitControlUI();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
public:
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//�Զ�����Ϣ
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	virtual LPCTSTR GetWindowClassName() const;

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	void LoadFriendUserInfo();
	void UpdateFriendInfo(const tagUserRemoteInfo& UserInfo);
	CNodeElementUI* UpdateFriendOnLine(const tagUserRemoteInfo& UserInfo);
	bool IsOnline(const DWORD dwGameID);

	IClientUserItem* CanChat(const DWORD dwUserID);
	bool Chat(IClientUserItem* pUserItem);
	bool CanJoinGame(bool bTips, WORD& dwTableID, WORD& dwChairID);
	bool JoinGame(const WORD dwTableID, const WORD dwChairID);
	bool CanLookOnGame(bool bTips, WORD& dwTableID, WORD& dwChairID);
	bool LookOnGame(const WORD dwTableID, const WORD dwChairID);

	//�ڲ�����
private:
	typedef vector<tagUserRemoteInfo> VSearchCorrespond;

	CMissionManager		m_MissionManager;		//�������
	VSearchCorrespond	m_vecFriendInfo;		//���ߺ���
	tagUserRemoteInfo	m_CurSelectUserInfo;	//��ǰѡ�еĺ���
	CListExUI*			m_pListFriend;			//�����б�
	CButtonUI*			m_pButtonJionGame;		//������Ϸ
	CButtonUI*			m_pButtonWatchGame;		//�Թ���Ϸ
	CButtonUI*			m_pButtonChat;			//˽��
	CControlUI*			m_pFriendAvatar;		//ͷ��
	CLabelUI*			m_pLabelFriendLevel;	//����vipͼ��
	CLabelUI*			m_pLabelFriendNickName;	//�����ǳ�
	CLabelUI*			m_pLabelFriendInGame;	//��ʲô��Ϸ
	CLabelUI*			m_pLabelFriendInRoom;	//��ʲô����
	CLabelUI*			m_pLabelFriendInTable;	//����һ��
	CLabelUI*			m_pLabelFriendGameID;	//ID��ʾ
};

//////////////////////////////////////////////////////////////////////////////////

#endif