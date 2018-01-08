#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//道具索引
typedef CMap<DWORD,DWORD,CGamePropertyBase *,CGamePropertyBase *> CGamePropertyBaseMap;

//////////////////////////////////////////////////////////////////////////////////

//道具管理
class GAME_PROPERTY_CLASS CGamePropertyManager
{
	//变量定义
protected:
	CGamePropertyBaseMap				m_GamePropertyBaseMap;				//道具索引

	//静态变量
protected:
	static CGamePropertyManager *	m_pGamePropertyManager;				//对象指针

	//函数定义
public:
	//构造函数
	CGamePropertyManager();
	//析构函数
	virtual ~CGamePropertyManager();

	//配置函数
public:
	//获取道具
	CGamePropertyBase * GetPropertyItem(DWORD dwPropertyID);
	//创建道具
	CGamePropertyBase * CreatePropertyItem(tagPropertyItem & PropertyItem);

	//静态函数
public:
	//获取对象
	static CGamePropertyManager * GetInstance() { return m_pGamePropertyManager; }
};

//////////////////////////////////////////////////////////////////////////////////

//种类结构
class  GAME_PROPERTY_CLASS CGamePropertyTypeItem 
{
	//变量定义
public:
	tagPropertyTypeItem				m_PropertyTypeItem;	

	//函数定义
public:
	//构造函数
	CGamePropertyTypeItem();

	//重载函数
public:
	//排列索引
	virtual DWORD GetSortID() { return m_PropertyTypeItem.dwSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//关系结构
class GAME_PROPERTY_CLASS CGamePropertyRelatItem 
{
	//变量定义
public:
	tagPropertyRelatItem				m_PropertyRelatItem;	

	//函数定义
public:
	//构造函数
	CGamePropertyRelatItem();
};

//////////////////////////////////////////////////////////////////////////////////

//道具结构
class  GAME_PROPERTY_CLASS CGamePropertyItem 
{
	//变量定义
public:
	tagPropertyItem					m_PropertyItem;	

	//函数定义
public:
	//构造函数
	CGamePropertyItem();
	//重载函数
public:
	//排列索引
	virtual DWORD GetSortID() { return m_PropertyItem.dwSortID;; }
};

//////////////////////////////////////////////////////////////////////////////////

//道具结构
class GAME_PROPERTY_CLASS CGamePropertySubItem 
{
	//变量定义
public:
	tagPropertySubItem					m_PropertySubItem;	

	//函数定义
public:
	//构造函数
	CGamePropertySubItem();
};

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CWHArray<CGamePropertyTypeItem *> CGamePropertyTypeItemArray;
typedef CWHArray<CGamePropertyRelatItem *>	CGamePropertyRelatItemArray;
typedef CWHArray<CGamePropertyItem *>	CGamePropertyItemArray;
typedef CWHArray<CGamePropertySubItem *>	CGamePropertySubItemArray;


//索引说明
typedef CMap<DWORD,DWORD,CGamePropertyTypeItem *,CGamePropertyTypeItem * &> CGamePropertyTypeItemMap;
typedef CMap<DWORD,DWORD,CGamePropertyRelatItem *,CGamePropertyRelatItem * &> CGamePropertyRelatItemMap;
typedef CMap<DWORD,DWORD,CGamePropertyItem *,CGamePropertyItem * &> CGamePropertyItemMap;
typedef CMap<DWORD,DWORD,CGamePropertySubItem *,CGamePropertySubItem * &> CGamePropertySubItemMap;

//////////////////////////////////////////////////////////////////////////////////
//列表服务
class GAME_PROPERTY_CLASS CGamePropertyListData
{
	//索引变量
protected:
	CGamePropertyTypeItemMap					m_GamePropertyTypeItemMap;				//种类索引
	CGamePropertyRelatItemMap					m_GamePropertyRelatItemMap;				//关系索引
	CGamePropertyItemMap						m_GamePropertyItemMap;					//道具索引
	CGamePropertySubItemMap						m_GamePropertySubItemMap;				//子道具索引

	//函数定义
public:
	//构造函数
	CGamePropertyListData();
	//析构函数
	virtual ~CGamePropertyListData();

	//静态变量
protected:
	static CGamePropertyListData *		m_pGamePropertyListData;					//列表数据

	//插入接口
public:
	//插入种类
	bool InsertGamePropertyTypeItem(tagPropertyTypeItem * ptagPropertyTypeItem);
	//插入关系
	bool InsertGamePropertyRelatItem(tagPropertyRelatItem * ptagPropertyRelatItem);
	//插入道具
	bool InsertGamePropertyItem(tagPropertyItem * ptagPropertyItem);
	//插入子道具
	bool InsertGamePropertySubItem(tagPropertySubItem * ptagPropertySubItem);

	//删除接口
public:
	//删除种类
	bool DeleteGamePropertyTypeItem(DWORD dwTypeID);
	//删除道具
	bool DeleteGamePropertyItem(DWORD wPropertyID);

	//枚举接口
public:
	//枚举种类
	CGamePropertyTypeItem * EmunGamePropertyTypeItem(POSITION & Position);
	//枚举道具
	CGamePropertyRelatItem * EmunGamePropertyRelatItem(POSITION & Position);
	//枚举道具
	CGamePropertyItem * EmunGamePropertyItem(POSITION & Position);
	//枚举子道具
	CGamePropertySubItem * EmunGamePropertySubItem(POSITION & Position);

	//查找接口
public:
	//查找种类
	CGamePropertyTypeItem * SearchGamePropertyTypeItem(DWORD dwTypeID);
	//查找道具
	CGamePropertyItem * SearchGamePropertyItem(DWORD wPropertyID);


	//数目接口
public:
	//种类数目
	DWORD GetGamePropertyTypeCount() { return (DWORD)m_GamePropertyTypeItemMap.GetCount(); }
	//道具数目
	DWORD GetGamePropertyRelatCount() { return (DWORD)m_GamePropertyRelatItemMap.GetCount(); }
	//道具数目
	DWORD GetGamePropertyCount() { return (DWORD)m_GamePropertyItemMap.GetCount(); }
	//子道具数目
	DWORD GetGamePropertySubCount() { return (DWORD)m_GamePropertySubItemMap.GetCount(); }


	//静态函数
public:
	//获取对象
	static CGamePropertyListData * GetInstance() { return m_pGamePropertyListData; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif