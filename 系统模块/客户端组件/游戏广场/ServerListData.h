#ifndef SERVER_LIST_DATA_HEAD_FILE
#define SERVER_LIST_DATA_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//子项类型
enum enItemGenre
{
	ItemGenre_Type,					//游戏种类
	ItemGenre_Kind,					//游戏类型
	ItemGenre_Server,				//游戏房间
};

//房间状态
enum enServerStatus
{
	ServerStatus_Normal,			//正常状态
	ServerStatus_Entrance,			//正在使用
	ServerStatus_EverEntrance,		//曾经进入
};

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CGameListItem;
class CGameTypeItem;
class CGameKindItem;
class CGameServerItem;
class CAgentGameKindItem;

//数组说明
typedef CWHArray<CGameListItem *> CGameListItemArray;
typedef CWHArray<CGameTypeItem *> CGameTypeItemArray;
typedef CWHArray<CGameKindItem *> CGameKindItemArray;
typedef CWHArray<CGameServerItem *> CGameServerItemArray;
typedef CWHArray<CAgentGameKindItem *> CCAgentGameKindItemArray;

//索引说明
typedef CMap<WORD,WORD,CGameTypeItem *,CGameTypeItem * &> CGameTypeItemMap;
typedef CMap<WORD,WORD,CGameKindItem *,CGameKindItem * &> CGameKindItemMap;
typedef CMap<WORD,WORD,CGameServerItem *,CGameServerItem * &> CGameServerItemMap;
typedef CMap<WORD,WORD,CAgentGameKindItem *,CAgentGameKindItem * &> CAgentGameKindItemMap;

//////////////////////////////////////////////////////////////////////////////////

//列表接口
interface IServerListDataSink
{
	//状态通知
public:
	//完成通知
	virtual VOID OnGameItemFinish()=NULL;
	//完成通知
	virtual VOID OnGameMatchFinish()=NULL;
	//完成通知
	virtual VOID OnGameKindFinish(WORD wKindID)=NULL;

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem)=NULL;
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem)=NULL;
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//列表子项
class CGameListItem
{
	//友元定义
	friend class CServerListData;

	//属性数据
protected:
	enItemGenre						m_ItemGenre;						//子项类型
	CGameListItem *					m_pParentListItem;					//父项子项

	//更新变量
public:
	//bool							m_bUpdateItem;						//更新标志
	DWORD							m_dwUpdateTime;						//更新时间

	//函数定义
protected:
	//构造函数
	CGameListItem(enItemGenre ItemGenre);
	//析构函数
	virtual ~CGameListItem();

	//功能函数
public:
	//获取类型
	enItemGenre GetItemGenre() { return m_ItemGenre; }
	//获取父项
	CGameListItem * GetParentListItem() { return m_pParentListItem; }

	//重载函数
public:
	//排列索引
	virtual WORD GetSortID()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//种类结构
class CGameTypeItem : public CGameListItem
{
	//属性数据
public:
	tagGameType						m_GameType;							//种类信息

	//函数定义
public:
	//构造函数
	CGameTypeItem();
	//析构函数
	virtual ~CGameTypeItem();

	//重载函数
public:
	//排列索引
	virtual WORD GetSortID() { return m_GameType.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//类型结构
class CGameKindItem : public CGameListItem
{
	//属性数据
public:
	tagGameKind						m_GameKind;							//类型信息

	//扩展数据
public:
	DWORD							m_dwProcessVersion;					//游戏版本

	//函数定义
public:
	//构造函数
	CGameKindItem();
	//析构函数
	virtual ~CGameKindItem();

	//重载函数
public:
	//排列索引
	virtual WORD GetSortID() { return m_GameKind.wSortID; }
};
//////////////////////////////////////////////////////////////////////////////////

//代理类型
class CAgentGameKindItem 
{
	//属性数据
public:
	tagAgentGameKind						m_AgentGameKind;							//类型信息

	//函数定义
public:
	//构造函数
	CAgentGameKindItem();
	//析构函数
	virtual ~CAgentGameKindItem();

	//重载函数
public:
	//排列索引
	virtual WORD GetSortID() { return m_AgentGameKind.wSortID; }
};

//////////////////////////////////////////////////////////////////////////

//房间结构
class CGameServerItem : public CGameListItem
{
	//属性数据
public:
	tagGameServer					m_GameServer;						//房间信息
	tagGameMatch					m_GameMatch;						//比赛信息

	//用户数据
public:
	bool							m_bSignuped;						//报名标识

	//扩展数据
public:
	enServerStatus					m_ServerStatus;						//房间状态

	//辅助变量
public:
	CGameKindItem *					m_pGameKindItem;					//游戏类型

	//函数定义
public:
	//构造函数
	CGameServerItem();
	//析构函数
	virtual ~CGameServerItem();

	//重载函数
public:	
	//排列索引
	virtual WORD GetSortID() { return m_GameServer.wSortID; }	 
	//比赛房间
	virtual bool IsMatchRoom() { return (m_GameServer.wServerType&GAME_GENRE_MATCH)!=0; }
};

//////////////////////////////////////////////////////////////////////////////////

//列表服务
class CServerListData
{
	//索引变量
protected:
	CGameTypeItemMap				m_GameTypeItemMap;					//种类索引
	CGameKindItemMap				m_GameKindItemMap;					//类型索引
	CGameServerItemMap				m_GameServerItemMap;				//房间索引
	CAgentGameKindItemMap			m_AgentGameKindItemMap;				//代理索引

	//内核变量
protected:
	CGameListItemArray				m_GameListItemWait;					//等待子项
	IServerListDataSink *			m_pIServerListDataSink;				//回调接口

	//静态变量
protected:
	static CServerListData *		m_pServerListData;					//列表数据

	//函数定义
public:
	//构造函数
	CServerListData();
	//析构函数
	virtual ~CServerListData();

	//配置函数
public:
	//设置接口
	VOID SetServerListDataSink(IServerListDataSink * pIServerListDataSink);

	//状态通知
public:
	//完成通知
	VOID OnEventListFinish();
	//完成通知
	VOID OnEventMatchFinish();
	//完成通知
	VOID OnEventKindFinish(WORD wKindID);	
	//下载通知
	VOID OnEventDownLoadFinish(WORD wKindID);
	
	//代理游戏
public:
	//过滤代理游戏
	VOID OnFiterAgentGameKind();

	//人数函数
public:
	//设置人数
	VOID SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount);
	//设置人数
	VOID SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount);

	//插入函数
public:
	//插入种类
	bool InsertGameType(tagGameType * pGameType);
	//插入类型
	bool InsertGameKind(tagGameKind * pGameKind);
	//插入房间
	bool InsertGameServer(tagGameServer * pGameServer);
	//插入代理游戏
	bool InsertAgentGameKind(tagAgentGameKind * pAgentGameKind);

	//删除函数
public:
	//删除种类
	bool DeleteGameType(WORD wTypeID);
	//删除类型
	bool DeleteGameKind(WORD wKindID);
	//删除房间
	bool DeleteGameServer(WORD wServerID);
	//删除代理
	bool DeleteAgentGameKindItem(WORD wKindID);

	//枚举函数
public:
	//枚举种类
	CGameTypeItem * EmunGameTypeItem(POSITION & Pos);
	//枚举类型
	CGameKindItem * EmunGameKindItem(POSITION & Pos);
	//枚举房间
	CGameServerItem * EmunGameServerItem(POSITION & Pos);
	//枚举代理游戏
	CAgentGameKindItem * EmunAgentGameKindItem(POSITION & Pos);

	//查找函数
public:
	//查找种类
	CGameTypeItem * SearchGameType(WORD wTypeID);
	//查找类型
	CGameKindItem * SearchGameKind(WORD wKindID);
	//查找房间
	CGameServerItem * SearchGameServer(WORD wServerID);
	//查找代理游戏
	CAgentGameKindItem * SearchAgentGameKind(WORD wAgentGameKindID);

	//数目函数
public:
	//种类数目
	DWORD GetGameTypeCount() { return (DWORD)m_GameTypeItemMap.GetCount(); }
	//类型数目
	DWORD GetGameKindCount() { return (DWORD)m_GameKindItemMap.GetCount(); }
	//房间数目
	DWORD GetGameServerCount() { return (DWORD)m_GameServerItemMap.GetCount(); }
	//代理游戏数目
	DWORD GetAgentGameKindCount() { return (DWORD)m_AgentGameKindItemMap.GetCount(); }
	//静态函数
public:
	//获取对象
	static CServerListData * GetInstance() { return m_pServerListData; }
};

//////////////////////////////////////////////////////////////////////////////////


#endif
