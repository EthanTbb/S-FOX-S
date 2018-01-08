#ifndef SERVER_LIST_DATA_HEAD_FILE
#define SERVER_LIST_DATA_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��������
enum enItemGenre
{
	ItemGenre_Type,					//��Ϸ����
	ItemGenre_Kind,					//��Ϸ����
	ItemGenre_Server,				//��Ϸ����
};

//����״̬
enum enServerStatus
{
	ServerStatus_Normal,			//����״̬
	ServerStatus_Entrance,			//����ʹ��
	ServerStatus_EverEntrance,		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CGameListItem;
class CGameTypeItem;
class CGameKindItem;
class CGameServerItem;
class CAgentGameKindItem;

//����˵��
typedef CWHArray<CGameListItem *> CGameListItemArray;
typedef CWHArray<CGameTypeItem *> CGameTypeItemArray;
typedef CWHArray<CGameKindItem *> CGameKindItemArray;
typedef CWHArray<CGameServerItem *> CGameServerItemArray;
typedef CWHArray<CAgentGameKindItem *> CCAgentGameKindItemArray;

//����˵��
typedef CMap<WORD,WORD,CGameTypeItem *,CGameTypeItem * &> CGameTypeItemMap;
typedef CMap<WORD,WORD,CGameKindItem *,CGameKindItem * &> CGameKindItemMap;
typedef CMap<WORD,WORD,CGameServerItem *,CGameServerItem * &> CGameServerItemMap;
typedef CMap<WORD,WORD,CAgentGameKindItem *,CAgentGameKindItem * &> CAgentGameKindItemMap;

//////////////////////////////////////////////////////////////////////////////////

//�б�ӿ�
interface IServerListDataSink
{
	//״̬֪ͨ
public:
	//���֪ͨ
	virtual VOID OnGameItemFinish()=NULL;
	//���֪ͨ
	virtual VOID OnGameMatchFinish()=NULL;
	//���֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID)=NULL;

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem)=NULL;
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem)=NULL;
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�б�����
class CGameListItem
{
	//��Ԫ����
	friend class CServerListData;

	//��������
protected:
	enItemGenre						m_ItemGenre;						//��������
	CGameListItem *					m_pParentListItem;					//��������

	//���±���
public:
	//bool							m_bUpdateItem;						//���±�־
	DWORD							m_dwUpdateTime;						//����ʱ��

	//��������
protected:
	//���캯��
	CGameListItem(enItemGenre ItemGenre);
	//��������
	virtual ~CGameListItem();

	//���ܺ���
public:
	//��ȡ����
	enItemGenre GetItemGenre() { return m_ItemGenre; }
	//��ȡ����
	CGameListItem * GetParentListItem() { return m_pParentListItem; }

	//���غ���
public:
	//��������
	virtual WORD GetSortID()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//����ṹ
class CGameTypeItem : public CGameListItem
{
	//��������
public:
	tagGameType						m_GameType;							//������Ϣ

	//��������
public:
	//���캯��
	CGameTypeItem();
	//��������
	virtual ~CGameTypeItem();

	//���غ���
public:
	//��������
	virtual WORD GetSortID() { return m_GameType.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//���ͽṹ
class CGameKindItem : public CGameListItem
{
	//��������
public:
	tagGameKind						m_GameKind;							//������Ϣ

	//��չ����
public:
	DWORD							m_dwProcessVersion;					//��Ϸ�汾

	//��������
public:
	//���캯��
	CGameKindItem();
	//��������
	virtual ~CGameKindItem();

	//���غ���
public:
	//��������
	virtual WORD GetSortID() { return m_GameKind.wSortID; }
};
//////////////////////////////////////////////////////////////////////////////////

//��������
class CAgentGameKindItem 
{
	//��������
public:
	tagAgentGameKind						m_AgentGameKind;							//������Ϣ

	//��������
public:
	//���캯��
	CAgentGameKindItem();
	//��������
	virtual ~CAgentGameKindItem();

	//���غ���
public:
	//��������
	virtual WORD GetSortID() { return m_AgentGameKind.wSortID; }
};

//////////////////////////////////////////////////////////////////////////

//����ṹ
class CGameServerItem : public CGameListItem
{
	//��������
public:
	tagGameServer					m_GameServer;						//������Ϣ
	tagGameMatch					m_GameMatch;						//������Ϣ

	//�û�����
public:
	bool							m_bSignuped;						//������ʶ

	//��չ����
public:
	enServerStatus					m_ServerStatus;						//����״̬

	//��������
public:
	CGameKindItem *					m_pGameKindItem;					//��Ϸ����

	//��������
public:
	//���캯��
	CGameServerItem();
	//��������
	virtual ~CGameServerItem();

	//���غ���
public:	
	//��������
	virtual WORD GetSortID() { return m_GameServer.wSortID; }	 
	//��������
	virtual bool IsMatchRoom() { return (m_GameServer.wServerType&GAME_GENRE_MATCH)!=0; }
};

//////////////////////////////////////////////////////////////////////////////////

//�б����
class CServerListData
{
	//��������
protected:
	CGameTypeItemMap				m_GameTypeItemMap;					//��������
	CGameKindItemMap				m_GameKindItemMap;					//��������
	CGameServerItemMap				m_GameServerItemMap;				//��������
	CAgentGameKindItemMap			m_AgentGameKindItemMap;				//��������

	//�ں˱���
protected:
	CGameListItemArray				m_GameListItemWait;					//�ȴ�����
	IServerListDataSink *			m_pIServerListDataSink;				//�ص��ӿ�

	//��̬����
protected:
	static CServerListData *		m_pServerListData;					//�б�����

	//��������
public:
	//���캯��
	CServerListData();
	//��������
	virtual ~CServerListData();

	//���ú���
public:
	//���ýӿ�
	VOID SetServerListDataSink(IServerListDataSink * pIServerListDataSink);

	//״̬֪ͨ
public:
	//���֪ͨ
	VOID OnEventListFinish();
	//���֪ͨ
	VOID OnEventMatchFinish();
	//���֪ͨ
	VOID OnEventKindFinish(WORD wKindID);	
	//����֪ͨ
	VOID OnEventDownLoadFinish(WORD wKindID);
	
	//������Ϸ
public:
	//���˴�����Ϸ
	VOID OnFiterAgentGameKind();

	//��������
public:
	//��������
	VOID SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount);
	//��������
	VOID SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount);

	//���뺯��
public:
	//��������
	bool InsertGameType(tagGameType * pGameType);
	//��������
	bool InsertGameKind(tagGameKind * pGameKind);
	//���뷿��
	bool InsertGameServer(tagGameServer * pGameServer);
	//���������Ϸ
	bool InsertAgentGameKind(tagAgentGameKind * pAgentGameKind);

	//ɾ������
public:
	//ɾ������
	bool DeleteGameType(WORD wTypeID);
	//ɾ������
	bool DeleteGameKind(WORD wKindID);
	//ɾ������
	bool DeleteGameServer(WORD wServerID);
	//ɾ������
	bool DeleteAgentGameKindItem(WORD wKindID);

	//ö�ٺ���
public:
	//ö������
	CGameTypeItem * EmunGameTypeItem(POSITION & Pos);
	//ö������
	CGameKindItem * EmunGameKindItem(POSITION & Pos);
	//ö�ٷ���
	CGameServerItem * EmunGameServerItem(POSITION & Pos);
	//ö�ٴ�����Ϸ
	CAgentGameKindItem * EmunAgentGameKindItem(POSITION & Pos);

	//���Һ���
public:
	//��������
	CGameTypeItem * SearchGameType(WORD wTypeID);
	//��������
	CGameKindItem * SearchGameKind(WORD wKindID);
	//���ҷ���
	CGameServerItem * SearchGameServer(WORD wServerID);
	//���Ҵ�����Ϸ
	CAgentGameKindItem * SearchAgentGameKind(WORD wAgentGameKindID);

	//��Ŀ����
public:
	//������Ŀ
	DWORD GetGameTypeCount() { return (DWORD)m_GameTypeItemMap.GetCount(); }
	//������Ŀ
	DWORD GetGameKindCount() { return (DWORD)m_GameKindItemMap.GetCount(); }
	//������Ŀ
	DWORD GetGameServerCount() { return (DWORD)m_GameServerItemMap.GetCount(); }
	//������Ϸ��Ŀ
	DWORD GetAgentGameKindCount() { return (DWORD)m_AgentGameKindItemMap.GetCount(); }
	//��̬����
public:
	//��ȡ����
	static CServerListData * GetInstance() { return m_pServerListData; }
};

//////////////////////////////////////////////////////////////////////////////////


#endif
