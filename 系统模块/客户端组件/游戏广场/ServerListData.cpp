#include "StdAfx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//常量定义
#define SERVER_INVALID_TIME			3									//失效时间

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CServerListData * CServerListData::m_pServerListData=NULL;				//列表数据

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameListItem::CGameListItem(enItemGenre ItemGenre)
{
	//属性数据
	m_ItemGenre=ItemGenre;
	m_pParentListItem=NULL;

	//更新时间
	m_dwUpdateTime=0;

	return;
}

//析构函数
CGameListItem::~CGameListItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameTypeItem::CGameTypeItem() : CGameListItem(ItemGenre_Type)
{
	//属性数据
	ZeroMemory(&m_GameType,sizeof(m_GameType));
}

//析构函数
CGameTypeItem::~CGameTypeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameKindItem::CGameKindItem() : CGameListItem(ItemGenre_Kind)
{
	//扩展数据
	m_dwProcessVersion=0L;

	//属性数据
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//析构函数
CGameKindItem::~CGameKindItem()
{
}
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAgentGameKindItem::CAgentGameKindItem() 
{

	//属性数据
	ZeroMemory(&m_AgentGameKind,sizeof(m_AgentGameKind));

	return;
}

//析构函数
CAgentGameKindItem::~CAgentGameKindItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServerItem::CGameServerItem() : CGameListItem(ItemGenre_Server)
{
	//辅助变量
	m_pGameKindItem=NULL;

	//扩展数据
	m_bSignuped=false;
	m_ServerStatus=ServerStatus_Normal;

	//属性数据
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));
	ZeroMemory(&m_GameMatch,sizeof(m_GameMatch));
}

//析构函数
CGameServerItem::~CGameServerItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListData::CServerListData()
{
	//接口变量
	m_pIServerListDataSink=NULL;

	//设置质数
	m_GameTypeItemMap.InitHashTable(PRIME_TYPE);
	m_GameKindItemMap.InitHashTable(PRIME_KIND);
	m_GameServerItemMap.InitHashTable(PRIME_SERVER);

	//设置对象
	ASSERT(m_pServerListData==NULL);
	if (m_pServerListData==NULL) m_pServerListData=this;

	return;
}

//析构函数
CServerListData::~CServerListData()
{
	//变量定义
	WORD wKey=0;

	//删除种类
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_GameTypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	m_GameTypeItemMap.RemoveAll();

	//删除类型
	CGameKindItem * pGameKindItem=NULL;
	Position=m_GameKindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		SafeDelete(pGameKindItem);
	}
	m_GameKindItemMap.RemoveAll();

	//删除房间
	CGameServerItem * pGameServerItem=NULL;
	Position=m_GameServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		SafeDelete(pGameServerItem);
	}
	m_GameServerItemMap.RemoveAll();

	//删除待决
	for(INT_PTR nIndex=0;nIndex<m_GameListItemWait.GetCount();nIndex++)
	{
		SafeDelete(m_GameListItemWait[nIndex]);
	}
	m_GameListItemWait.RemoveAll();

	//释放对象
	ASSERT(m_pServerListData==this);
	if (m_pServerListData==this) m_pServerListData=NULL;

	return;
}

//设置接口
VOID CServerListData::SetServerListDataSink(IServerListDataSink * pIServerListDataSink)
{
	//设置变量
	m_pIServerListDataSink=pIServerListDataSink;

	return;
}

//完成通知
VOID CServerListData::OnEventListFinish()
{
	//过滤代理
	OnFiterAgentGameKind();

	//事件通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemFinish();

	return;
}

//完成通知
VOID CServerListData::OnEventMatchFinish()
{
	//变量定义	
	POSITION Position=NULL;
	DWORD dwFinishTime=(DWORD)time(NULL);
	CGameServerItem * pGameServerItem=NULL;

	//插入对象
	while (true)
	{
		//获取对象
		pGameServerItem=EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//类型判断
		if (pGameServerItem->IsMatchRoom()==true)
		{
			//判断时间
			if(dwFinishTime-pGameServerItem->m_dwUpdateTime>=SERVER_INVALID_TIME)
			{
				DeleteGameServer(pGameServerItem->m_GameServer.wServerID);
			}
		}		

		if (Position==NULL) break;
	}

	//事件通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameMatchFinish();

	return;
}

//完成通知
VOID CServerListData::OnEventKindFinish(WORD wKindID)
{
	//变量定义	
	POSITION Position=NULL;
	DWORD dwFinishTime=(DWORD)time(NULL);
	CGameServerItem * pGameServerItem=NULL;

	//插入对象
	while (true)
	{
		//获取对象
		pGameServerItem=EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//类型判断
		if (pGameServerItem->m_GameServer.wKindID==wKindID || wKindID==INVALID_WORD)
		{
			//判断时间
			if(dwFinishTime-pGameServerItem->m_dwUpdateTime>=SERVER_INVALID_TIME)
			{
				DeleteGameServer(pGameServerItem->m_GameServer.wServerID);
			}
		}		

		if (Position==NULL) break;
	}

	//事件通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameKindFinish(wKindID);

	return;
}

//下载通知
VOID CServerListData::OnEventDownLoadFinish(WORD wKindID)
{
	//变量定义
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);

	//更新信息
	if (pGameKindItem!=NULL)
	{
		//获取版本
		LPCTSTR pszProcessName=pGameKindItem->m_GameKind.szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);

		//更新子项
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

	//过滤类型
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

	//过滤类型
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

//设置人数
VOID CServerListData::SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount)
{
	//搜索类型
	CGameKindItem * pGameKindItem=SearchGameKind(wKindID);

	//设置人数
	if (pGameKindItem!=NULL)
	{
		//设置变量
		pGameKindItem->m_GameKind.dwOnLineCount=dwOnLineCount;

		//事件通知
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
VOID CServerListData::SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount)
{
	//搜索房间
	CGameServerItem * pGameServerItem=SearchGameServer(wServerID);

	//设置人数
	if (pGameServerItem!=NULL)
	{
		//设置变量
		pGameServerItem->m_GameServer.dwOnLineCount=dwOnLineCount;

		//事件通知
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return;
}

//插入种类
bool CServerListData::InsertGameType(tagGameType * pGameType)
{
	//效验参数
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//变量定义
	CGameTypeItem * pGameTypeItem=NULL;
	bool bInsert=(m_GameTypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameTypeItem=new CGameTypeItem;
		if (pGameTypeItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameTypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		//界面更新
		m_pIServerListDataSink->OnGameItemInsert(pGameTypeItem);
	}
	else
	{
		//界面更新
		m_pIServerListDataSink->OnGameItemUpdate(pGameTypeItem);
	}

	return true;
}

//插入类型
bool CServerListData::InsertGameKind(tagGameKind * pGameKind)
{
	//效验参数
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//变量定义
	CGameKindItem * pGameKindItem=NULL;
	bool bInsert=(m_GameKindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameKindItem=new CGameKindItem;		
		if (pGameKindItem==NULL) return false;
	}

	//获取版本
	if (bInsert==true)
	{
		LPCTSTR pszProcessName=pGameKind->szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);
	}

	//设置数据
	CopyMemory(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//寻找父项
	pGameKindItem->m_pParentListItem=SearchGameType(pGameKind->wTypeID);

	//待决判断
	if (pGameKindItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameKindItem);
		return true;
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameKindItemMap[pGameKind->wKindID]=pGameKindItem;

		//插入子项
		m_pIServerListDataSink->OnGameItemInsert(pGameKindItem);
	}
	else
	{
		//更新子项
		m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return true;
}

//插入房间
bool CServerListData::InsertGameServer(tagGameServer * pGameServer)
{
	//效验参数
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//变量定义
	CGameServerItem * pGameServerItem=NULL;
	bool bInsert=(m_GameServerItemMap.Lookup(pGameServer->wServerID,pGameServerItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameServerItem=new CGameServerItem;
		if (pGameServerItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));

	//寻找父项
	pGameServerItem->m_pGameKindItem=SearchGameKind(pGameServer->wKindID);
	pGameServerItem->m_pParentListItem=SearchGameKind(pGameServer->wKindID);

	//待决判断
	if (pGameServerItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameServerItem);
		return true;
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameServerItemMap[pGameServer->wServerID]=pGameServerItem;

		//插入子项
		m_pIServerListDataSink->OnGameItemInsert(pGameServerItem);
	}
	else 
	{
		//更新子项
		m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return true;
}

//插入代理游戏
bool CServerListData::InsertAgentGameKind(tagAgentGameKind * pAgentGameKind)
{
	//效验参数
	ASSERT(pAgentGameKind!=NULL);
	if (pAgentGameKind==NULL) return false;

	//变量定义
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	bool bInsert=(m_AgentGameKindItemMap.Lookup(pAgentGameKind->wKindID,pAgentGameKindItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pAgentGameKindItem=new CAgentGameKindItem;
		if (pAgentGameKindItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pAgentGameKindItem->m_AgentGameKind,pAgentGameKind,sizeof(tagAgentGameKind));

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_AgentGameKindItemMap[pAgentGameKind->wKindID]=pAgentGameKindItem;

	}

	return true;
}

//删除种类
bool CServerListData::DeleteGameType(WORD wTypeID)
{
	//查找种类
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameTypeItem);

	//重置数据
	SafeDelete(pGameTypeItem);

	//删除数据
	m_GameTypeItemMap.RemoveKey(wTypeID);

	return true;
}

//删除类型
bool CServerListData::DeleteGameKind(WORD wKindID)
{
	//查找类型
	CGameKindItem * pGameKindItem=NULL;
	if (m_GameKindItemMap.Lookup(wKindID,pGameKindItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameKindItem);

	//删除数据
	SafeDelete(pGameKindItem);

	//删除数据
	m_GameKindItemMap.RemoveKey(wKindID);

	return true;
}

//删除房间
bool CServerListData::DeleteGameServer(WORD wServerID)
{
	//查找房间
	CGameServerItem * pGameServerItem=NULL;
	if (m_GameServerItemMap.Lookup(wServerID,pGameServerItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameServerItem);

	//删除数据
	SafeDelete(pGameServerItem);

	//删除数据
	m_GameServerItemMap.RemoveKey(wServerID);

	return false;
}

bool CServerListData::DeleteAgentGameKindItem(WORD wKindID)
{
	//查找类型
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	if (m_AgentGameKindItemMap.Lookup(wKindID,pAgentGameKindItem)==FALSE) return false;

	//删除数据
	SafeDelete(pAgentGameKindItem);

	//删除数据
	m_AgentGameKindItemMap.RemoveKey(wKindID);
	return true;
}

//枚举种类
CGameTypeItem * CServerListData::EmunGameTypeItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameTypeItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//枚举类型
CGameKindItem * CServerListData::EmunGameKindItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameKindItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);

	return pGameKindItem;
}

//枚举房间
CGameServerItem * CServerListData::EmunGameServerItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameServerItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);

	return pGameServerItem;
}

//枚举代理游戏
CAgentGameKindItem * CServerListData::EmunAgentGameKindItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_AgentGameKindItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找代理游戏
	WORD wKey=0;
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	m_AgentGameKindItemMap.GetNextAssoc(Position,wKey,pAgentGameKindItem);

	return pAgentGameKindItem;
}

//查找种类
CGameTypeItem * CServerListData::SearchGameType(WORD wTypeID)
{
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//查找类型
CGameKindItem * CServerListData::SearchGameKind(WORD wKindID)
{
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//查找房间
CGameServerItem * CServerListData::SearchGameServer(WORD wServerID)
{
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.Lookup(wServerID,pGameServerItem);
	return pGameServerItem;
}

//查找代理游戏
CAgentGameKindItem * CServerListData::SearchAgentGameKind(WORD wAgentGameKindID)
{
	CAgentGameKindItem * pAgentGameKindItem=NULL;
	m_AgentGameKindItemMap.Lookup(wAgentGameKindID,pAgentGameKindItem);
	return pAgentGameKindItem;
}

//////////////////////////////////////////////////////////////////////////////////


