#include "StdAfx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//��������
#define SERVER_INVALID_TIME			3									//ʧЧʱ��

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CServerListData * CServerListData::m_pServerListData=NULL;				//�б�����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameListItem::CGameListItem(enItemGenre ItemGenre)
{
	//��������
	m_ItemGenre=ItemGenre;
	m_pParentListItem=NULL;

	//����ʱ��
	m_dwUpdateTime=0;

	return;
}

//��������
CGameListItem::~CGameListItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameTypeItem::CGameTypeItem() : CGameListItem(ItemGenre_Type)
{
	//��������
	ZeroMemory(&m_GameType,sizeof(m_GameType));
}

//��������
CGameTypeItem::~CGameTypeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameKindItem::CGameKindItem() : CGameListItem(ItemGenre_Kind)
{
	//��չ����
	m_dwProcessVersion=0L;

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//��������
CGameKindItem::~CGameKindItem()
{
}
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAgentGameKindItem::CAgentGameKindItem() 
{

	//��������
	ZeroMemory(&m_AgentGameKind,sizeof(m_AgentGameKind));

	return;
}

//��������
CAgentGameKindItem::~CAgentGameKindItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerItem::CGameServerItem() : CGameListItem(ItemGenre_Server)
{
	//��������
	m_pGameKindItem=NULL;

	//��չ����
	m_bSignuped=false;
	m_ServerStatus=ServerStatus_Normal;

	//��������
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));
	ZeroMemory(&m_GameMatch,sizeof(m_GameMatch));
}

//��������
CGameServerItem::~CGameServerItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerListData::CServerListData()
{
	//�ӿڱ���
	m_pIServerListDataSink=NULL;

	//��������
	m_GameTypeItemMap.InitHashTable(PRIME_TYPE);
	m_GameKindItemMap.InitHashTable(PRIME_KIND);
	m_GameServerItemMap.InitHashTable(PRIME_SERVER);

	//���ö���
	ASSERT(m_pServerListData==NULL);
	if (m_pServerListData==NULL) m_pServerListData=this;

	return;
}

//��������
CServerListData::~CServerListData()
{
	//��������
	WORD wKey=0;

	//ɾ������
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_GameTypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	m_GameTypeItemMap.RemoveAll();

	//ɾ������
	CGameKindItem * pGameKindItem=NULL;
	Position=m_GameKindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		SafeDelete(pGameKindItem);
	}
	m_GameKindItemMap.RemoveAll();

	//ɾ������
	CGameServerItem * pGameServerItem=NULL;
	Position=m_GameServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		SafeDelete(pGameServerItem);
	}
	m_GameServerItemMap.RemoveAll();

	//ɾ������
	for(INT_PTR nIndex=0;nIndex<m_GameListItemWait.GetCount();nIndex++)
	{
		SafeDelete(m_GameListItemWait[nIndex]);
	}
	m_GameListItemWait.RemoveAll();

	//�ͷŶ���
	ASSERT(m_pServerListData==this);
	if (m_pServerListData==this) m_pServerListData=NULL;

	return;
}

//���ýӿ�
VOID CServerListData::SetServerListDataSink(IServerListDataSink * pIServerListDataSink)
{
	//���ñ���
	m_pIServerListDataSink=pIServerListDataSink;

	return;
}

//���֪ͨ
VOID CServerListData::OnEventListFinish()
{
	//���˴���
	OnFiterAgentGameKind();

	//�¼�֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemFinish();

	return;
}

//���֪ͨ
VOID CServerListData::OnEventMatchFinish()
{
	//��������	
	POSITION Position=NULL;
	DWORD dwFinishTime=(DWORD)time(NULL);
	CGameServerItem * pGameServerItem=NULL;

	//�������
	while (true)
	{
		//��ȡ����
		pGameServerItem=EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//�����ж�
		if (pGameServerItem->IsMatchRoom()==true)
		{
			//�ж�ʱ��
			if(dwFinishTime-pGameServerItem->m_dwUpdateTime>=SERVER_INVALID_TIME)
			{
				DeleteGameServer(pGameServerItem->m_GameServer.wServerID);
			}
		}		

		if (Position==NULL) break;
	}

	//�¼�֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameMatchFinish();

	return;
}

//���֪ͨ
VOID CServerListData::OnEventKindFinish(WORD wKindID)
{
	//��������	
	POSITION Position=NULL;
	DWORD dwFinishTime=(DWORD)time(NULL);
	CGameServerItem * pGameServerItem=NULL;

	//�������
	while (true)
	{
		//��ȡ����
		pGameServerItem=EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//�����ж�
		if (pGameServerItem->m_GameServer.wKindID==wKindID || wKindID==INVALID_WORD)
		{
			//�ж�ʱ��
			if(dwFinishTime-pGameServerItem->m_dwUpdateTime>=SERVER_INVALID_TIME)
			{
				DeleteGameServer(pGameServerItem->m_GameServer.wServerID);
			}
		}		

		if (Position==NULL) break;
	}

	//�¼�֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameKindFinish(wKindID);

	return;
}

//����֪ͨ
VOID CServerListData::OnEventDownLoadFinish(WORD wKindID)
{
	//��������
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);

	//������Ϣ
	if (pGameKindItem!=NULL)
	{
		//��ȡ�汾
		LPCTSTR pszProcessName=pGameKindItem->m_GameKind.szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);

		//��������
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

VOID CServerListData::OnFiterAgentGameKind()
{
	if (GetAgentGameKindCount()==0)
	{
		return;
	}

	//��������
	POSITION Position=NULL;
	POSITION Position2=NULL;
	CGameTypeItem * pGameTypeItem=NULL;
	CGameKindItem * pGameKindItem=NULL;
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	CWHArray<int> KindFiterOut;
	CWHArray<int> TypeFiterOut;
	while (true)
	{
		pGameKindItem=EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;
		pAgentGameKindItem = NULL;
		pAgentGameKindItem = SearchAgentGameKind(pGameKindItem->m_GameKind.wKindID);
		if (pAgentGameKindItem==NULL)
		{
			KindFiterOut.Add(pGameKindItem->m_GameKind.wKindID);
		}
		if (Position==NULL) break;
	}
	int nKindFiertOutCount = (int)KindFiterOut.GetCount();
	for (int i=0;i<nKindFiertOutCount;i++)
	{
		DeleteGameKind(KindFiterOut[i]);
		
	}

	//��������
	Position=NULL;
	pGameKindItem=NULL;
	while (true)
	{
		pGameTypeItem=EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;
		bool bFinder = false;
		while (true)
		{
			pGameKindItem=EmunGameKindItem(Position2);
			if (pGameKindItem==NULL) break;
			if (pGameKindItem->m_GameKind.wTypeID == pGameTypeItem->m_GameType.wTypeID)
			{
				bFinder =true;
				break;
			}
			if (Position2==NULL) break;
		}

		if (!bFinder)
		{
			TypeFiterOut.Add(pGameTypeItem->m_GameType.wTypeID);
		}
		if (Position==NULL) break;
	}
	int nTypeFiertOutCount = (int)TypeFiterOut.GetCount();
	for (int i=0;i<nTypeFiertOutCount;i++)
	{
		DeleteGameType(TypeFiterOut[i]);

	}
}

//��������
VOID CServerListData::SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount)
{
	//��������
	CGameKindItem * pGameKindItem=SearchGameKind(wKindID);

	//��������
	if (pGameKindItem!=NULL)
	{
		//���ñ���
		pGameKindItem->m_GameKind.dwOnLineCount=dwOnLineCount;

		//�¼�֪ͨ
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//��������
VOID CServerListData::SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount)
{
	//��������
	CGameServerItem * pGameServerItem=SearchGameServer(wServerID);

	//��������
	if (pGameServerItem!=NULL)
	{
		//���ñ���
		pGameServerItem->m_GameServer.dwOnLineCount=dwOnLineCount;

		//�¼�֪ͨ
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return;
}

//��������
bool CServerListData::InsertGameType(tagGameType * pGameType)
{
	//Ч�����
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//��������
	CGameTypeItem * pGameTypeItem=NULL;
	bool bInsert=(m_GameTypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameTypeItem=new CGameTypeItem;
		if (pGameTypeItem==NULL) return false;
	}

	//��������
	CopyMemory(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameTypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		//�������
		m_pIServerListDataSink->OnGameItemInsert(pGameTypeItem);
	}
	else
	{
		//�������
		m_pIServerListDataSink->OnGameItemUpdate(pGameTypeItem);
	}

	return true;
}

//��������
bool CServerListData::InsertGameKind(tagGameKind * pGameKind)
{
	//Ч�����
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//��������
	CGameKindItem * pGameKindItem=NULL;
	bool bInsert=(m_GameKindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameKindItem=new CGameKindItem;		
		if (pGameKindItem==NULL) return false;
	}

	//��ȡ�汾
	if (bInsert==true)
	{
		LPCTSTR pszProcessName=pGameKind->szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);
	}

	//��������
	CopyMemory(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//Ѱ�Ҹ���
	pGameKindItem->m_pParentListItem=SearchGameType(pGameKind->wTypeID);

	//�����ж�
	if (pGameKindItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameKindItem);
		return true;
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameKindItemMap[pGameKind->wKindID]=pGameKindItem;

		//��������
		m_pIServerListDataSink->OnGameItemInsert(pGameKindItem);
	}
	else
	{
		//��������
		m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return true;
}

//���뷿��
bool CServerListData::InsertGameServer(tagGameServer * pGameServer)
{
	//Ч�����
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//��������
	CGameServerItem * pGameServerItem=NULL;
	bool bInsert=(m_GameServerItemMap.Lookup(pGameServer->wServerID,pGameServerItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameServerItem=new CGameServerItem;
		if (pGameServerItem==NULL) return false;
	}

	//��������
	CopyMemory(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));

	//Ѱ�Ҹ���
	pGameServerItem->m_pGameKindItem=SearchGameKind(pGameServer->wKindID);
	pGameServerItem->m_pParentListItem=SearchGameKind(pGameServer->wKindID);

	//�����ж�
	if (pGameServerItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameServerItem);
		return true;
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameServerItemMap[pGameServer->wServerID]=pGameServerItem;

		//��������
		m_pIServerListDataSink->OnGameItemInsert(pGameServerItem);
	}
	else 
	{
		//��������
		m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return true;
}

//���������Ϸ
bool CServerListData::InsertAgentGameKind(tagAgentGameKind * pAgentGameKind)
{
	//Ч�����
	ASSERT(pAgentGameKind!=NULL);
	if (pAgentGameKind==NULL) return false;

	//��������
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	bool bInsert=(m_AgentGameKindItemMap.Lookup(pAgentGameKind->wKindID,pAgentGameKindItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pAgentGameKindItem=new CAgentGameKindItem;
		if (pAgentGameKindItem==NULL) return false;
	}

	//��������
	CopyMemory(&pAgentGameKindItem->m_AgentGameKind,pAgentGameKind,sizeof(tagAgentGameKind));

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_AgentGameKindItemMap[pAgentGameKind->wKindID]=pAgentGameKindItem;

	}

	return true;
}

//ɾ������
bool CServerListData::DeleteGameType(WORD wTypeID)
{
	//��������
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameTypeItem);

	//��������
	SafeDelete(pGameTypeItem);

	//ɾ������
	m_GameTypeItemMap.RemoveKey(wTypeID);

	return true;
}

//ɾ������
bool CServerListData::DeleteGameKind(WORD wKindID)
{
	//��������
	CGameKindItem * pGameKindItem=NULL;
	if (m_GameKindItemMap.Lookup(wKindID,pGameKindItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameKindItem);

	//ɾ������
	SafeDelete(pGameKindItem);

	//ɾ������
	m_GameKindItemMap.RemoveKey(wKindID);

	return true;
}

//ɾ������
bool CServerListData::DeleteGameServer(WORD wServerID)
{
	//���ҷ���
	CGameServerItem * pGameServerItem=NULL;
	if (m_GameServerItemMap.Lookup(wServerID,pGameServerItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameServerItem);

	//ɾ������
	SafeDelete(pGameServerItem);

	//ɾ������
	m_GameServerItemMap.RemoveKey(wServerID);

	return false;
}

bool CServerListData::DeleteAgentGameKindItem(WORD wKindID)
{
	//��������
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	if (m_AgentGameKindItemMap.Lookup(wKindID,pAgentGameKindItem)==FALSE) return false;

	//ɾ������
	SafeDelete(pAgentGameKindItem);

	//ɾ������
	m_AgentGameKindItemMap.RemoveKey(wKindID);
	return true;
}

//ö������
CGameTypeItem * CServerListData::EmunGameTypeItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameTypeItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//ö������
CGameKindItem * CServerListData::EmunGameKindItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameKindItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);

	return pGameKindItem;
}

//ö�ٷ���
CGameServerItem * CServerListData::EmunGameServerItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameServerItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);

	return pGameServerItem;
}

//ö�ٴ�����Ϸ
CAgentGameKindItem * CServerListData::EmunAgentGameKindItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_AgentGameKindItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���Ҵ�����Ϸ
	WORD wKey=0;
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	m_AgentGameKindItemMap.GetNextAssoc(Position,wKey,pAgentGameKindItem);

	return pAgentGameKindItem;
}

//��������
CGameTypeItem * CServerListData::SearchGameType(WORD wTypeID)
{
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//��������
CGameKindItem * CServerListData::SearchGameKind(WORD wKindID)
{
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//���ҷ���
CGameServerItem * CServerListData::SearchGameServer(WORD wServerID)
{
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.Lookup(wServerID,pGameServerItem);
	return pGameServerItem;
}

//���Ҵ�����Ϸ
CAgentGameKindItem * CServerListData::SearchAgentGameKind(WORD wAgentGameKindID)
{
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	m_AgentGameKindItemMap.Lookup(wAgentGameKindID,pAgentGameKindItem);
	return pAgentGameKindItem;
}

//////////////////////////////////////////////////////////////////////////////////


