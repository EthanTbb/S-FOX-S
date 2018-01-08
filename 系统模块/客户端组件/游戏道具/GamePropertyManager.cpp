#include "StdAfx.h"
#include "GamePropertyManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CGamePropertyManager * CGamePropertyManager::m_pGamePropertyManager=NULL;//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyManager::CGamePropertyManager()
{
	//设置变量
	ASSERT(m_pGamePropertyManager==NULL);
	if (m_pGamePropertyManager==NULL) m_pGamePropertyManager=this;

	return;
}

//析构函数
CGamePropertyManager::~CGamePropertyManager()
{
	//设置变量
	ASSERT(m_pGamePropertyManager==this);
	if (m_pGamePropertyManager==this) m_pGamePropertyManager=NULL;

	//释放对象
	if(m_GamePropertyBaseMap.GetCount()>0)
	{
		//变量定义
		POSITION Position=m_GamePropertyBaseMap.GetStartPosition();

		//释放对象
		DWORD wKey=0;
		CGamePropertyBase * pGamePropertyItem=NULL;
		do
		{	
			m_GamePropertyBaseMap.GetNextAssoc(Position,wKey,pGamePropertyItem);
			if(pGamePropertyItem!=NULL)
			{	
				SafeDelete(pGamePropertyItem);
			}		
			
		}while(Position!=NULL);
	}

	//移除元素
	m_GamePropertyBaseMap.RemoveAll();

	return;
}

//获取道具
CGamePropertyBase * CGamePropertyManager::GetPropertyItem(DWORD dwPropertyID)
{
	//查找道具
	CGamePropertyBase * pGamePropertyItem=NULL;
	m_GamePropertyBaseMap.Lookup(dwPropertyID,pGamePropertyItem);

	return pGamePropertyItem;
}

//创建道具
CGamePropertyBase * CGamePropertyManager::CreatePropertyItem(tagPropertyItem & PropertyItem)
{

	//查找现存
	CGamePropertyBase * pGamePropertyItem=NULL;
	m_GamePropertyBaseMap.Lookup(PropertyItem.dwPropertyID,pGamePropertyItem);

	//现存判断
	if (pGamePropertyItem!=NULL)
	{
		//更新道具
		pGamePropertyItem->m_PropertyItem=PropertyItem;
		return pGamePropertyItem;
	}

	//创建道具
	switch (PropertyItem.dwPropertyID)
	{
	case PROPERTY_ID_CAR:			//汽车礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyCar;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_EGG:			//臭蛋礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyEgg;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_CLAP:			//鼓掌礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyClap;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_KISS:			//香吻礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyKiss;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BEER:			//啤酒礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBeer;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_CAKE:			//蛋糕礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyCake;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_RING:			//钻戒礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyRing;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BEAT:			//暴打礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBeat;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BOMB:			//炸弹礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBomb;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_SMOKE:			//香烟礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertySmoke;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_VILLA:			//别墅礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyVilla;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BRICK:			//砖头礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBrick;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_FLOWER:		//鲜花礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyFlower;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_TWO_CARD:    //双倍积分
		{
			try
			{
				pGamePropertyItem=new CGamePropertyTwoScoreCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_FOUR_CARD:    //四倍积分
		{
			try
			{
				pGamePropertyItem=new CGamePropertyFourScoreCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_SCORE_CLEAR:    //负分清零
		{
			try
			{
				pGamePropertyItem=new CGamePropertyScoreClear;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR:    //逃跑清零
		{
			try
			{
				pGamePropertyItem=new CGamePropertyEscapeClear;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_TRUMPET:		  //小喇叭
		{
			try
			{
				pGamePropertyItem=new CGamePropertyTrumpet;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_TYPHON:		  //大喇叭
		{
			try
			{
				pGamePropertyItem=new CGamePropertyTyphon;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_GUARDKICK_CARD:   //防踢卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyGuardKickCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_POSSESS:		  //护身符
		{
			try
			{
				pGamePropertyItem=new CGamePropertyPossess;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_DOUBLE_1HOUR:	//1小时双倍卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyDouble1Hour;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_DOUBLE_3HOUR:	 //3小时双倍卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyDouble3Hour;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_DOUBLE_1WEEK:	 //1周双倍卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyDouble1Week;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_DOUBLE_1DAY:	 //1天双倍卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyDouble1Day;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_AMULET_1HOUR:	//1小时护身卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet1Hour;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_AMULET_3HOUR:	 //3小时护身卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet3Hour;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_AMULET_1WEEK:	 //1周护身卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet1Week;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_AMULET_1DAY:	 //1天护身卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet1Day;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_GUARDKICK_1HOUR:	//1小时防踢卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet1Hour;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_GUARDKICK_3HOUR:	 //3小时防踢卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet3Hour;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_GUARDKICK_1WEEK:	 //1周防踢卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet1Week;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_GUARDKICK_1DAY:	 //1天防踢卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAmulet1Day;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_CRYSTAL:			//水晶	
		{
			try
			{
				pGamePropertyItem=new CGamePropertyCrystal;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_AGATE:			//玛瑙	
		{
			try
			{
				pGamePropertyItem=new CGamePropertyAgate;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_BOULDER:			//玉石	
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBoulder;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_GEM:			//宝石	
		{
			try
			{
				pGamePropertyItem=new CGamePropertyGem;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	case PROPERTY_ID_DIAMOND:			//钻石	
		{
			try
			{
				pGamePropertyItem=new CGamePropertyDiamond;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
			break;
		}
	//case PROPERTY_ID_BLUERING_CARD:	  //蓝钻会员卡
	//	{
	//		try
	//		{
	//			pGamePropertyItem=new CGamePropertyBlueRingCard;
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}

	//		break;
	//	}
	//case PROPERTY_ID_YELLOWRING_CARD:	//黄钻会员卡
	//	{
	//		try
	//		{
	//			pGamePropertyItem=new CGamePropertyYellowRingCard;
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}

	//		break;
	//	}
	//case PROPERTY_ID_WHITERING_CARD:	//白钻会员卡
	//	{
	//		try
	//		{
	//			pGamePropertyItem=new CGamePropertyWhiteRingCard;
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}

	//		break;
	//	}
	//case PROPERTY_ID_REDRING_CARD:		//红钻会员卡
	//	{
	//		try
	//		{
	//			pGamePropertyItem=new CGamePropertyRedRingCard;
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}

	//		break;
	//	}
	//case PROPERTY_ID_VIPROOM_CARD:		//VIP房卡
	//	{
	//		try
	//		{
	//			pGamePropertyItem=new CGamePropertyVipRoomCard;
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}

	//		break;
	//	}
	}

	//创建判断
	if (pGamePropertyItem!=NULL)
	{
		//设置道具
		pGamePropertyItem->m_PropertyItem=PropertyItem;
		m_GamePropertyBaseMap[pGamePropertyItem->m_PropertyItem.dwPropertyID]=pGamePropertyItem;

		return pGamePropertyItem;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyTypeItem::CGamePropertyTypeItem()
{
	//设置变量
	ZeroMemory(&m_PropertyTypeItem,sizeof(m_PropertyTypeItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyRelatItem::CGamePropertyRelatItem()
{
	//设置变量
	ZeroMemory(&m_PropertyRelatItem,sizeof(m_PropertyRelatItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyItem::CGamePropertyItem()
{
	//设置变量
	ZeroMemory(&m_PropertyItem,sizeof(m_PropertyItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertySubItem::CGamePropertySubItem()
{
	//设置变量
	ZeroMemory(&m_PropertySubItem,sizeof(m_PropertySubItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

CGamePropertyListData * CGamePropertyListData::m_pGamePropertyListData = NULL;
//构造函数
CGamePropertyListData::CGamePropertyListData()
{
	//设置质数
	m_GamePropertyTypeItemMap.InitHashTable(PRIME_TYPE);
	m_GamePropertyRelatItemMap.InitHashTable(PRIME_KIND);
	m_GamePropertyItemMap.InitHashTable(PRIME_SERVER);
	m_GamePropertySubItemMap.InitHashTable(PRIME_KIND);

	//设置对象
	ASSERT(m_pGamePropertyListData==NULL);
	if (m_pGamePropertyListData==NULL) m_pGamePropertyListData=this;

	return;
}

//析构函数
CGamePropertyListData::~CGamePropertyListData()
{
	//变量定义
	DWORD wKey=0;

	//删除种类
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	POSITION Position=m_GamePropertyTypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertyTypeItemMap.GetNextAssoc(Position,wKey,pGamePropertyTypeItem);
		SafeDelete(pGamePropertyTypeItem);
	}
	m_GamePropertyTypeItemMap.RemoveAll();


	//删除关系
	CGamePropertyRelatItem * pGamePropertyRelatItem=NULL;
	Position=m_GamePropertyRelatItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertyRelatItemMap.GetNextAssoc(Position,wKey,pGamePropertyRelatItem);
		SafeDelete(pGamePropertyRelatItem);
	}
	m_GamePropertyRelatItemMap.RemoveAll();

	//删除道具
	CGamePropertyItem * pGamePropertyItem=NULL;
	Position=m_GamePropertyItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertyItemMap.GetNextAssoc(Position,wKey,pGamePropertyItem);
		SafeDelete(pGamePropertyItem);
	}
	m_GamePropertyItemMap.RemoveAll();

	//删除子道具
	CGamePropertySubItem * pGamePropertySubItem=NULL;
	Position=m_GamePropertySubItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertySubItemMap.GetNextAssoc(Position,wKey,pGamePropertySubItem);
		SafeDelete(pGamePropertySubItem);
	}
	m_GamePropertySubItemMap.RemoveAll();

	//释放对象
	ASSERT(m_pGamePropertyListData==this);
	if (m_pGamePropertyListData==this) m_pGamePropertyListData=NULL;

	return;
}

//插入种类
bool CGamePropertyListData::InsertGamePropertyTypeItem(tagPropertyTypeItem * ptagPropertyTypeItem)
{
	//效验参数
	ASSERT(ptagPropertyTypeItem!=NULL);
	if (ptagPropertyTypeItem==NULL) return false;

	//查找现存
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	if (m_GamePropertyTypeItemMap.Lookup(ptagPropertyTypeItem->dwTypeID,pGamePropertyTypeItem)==FALSE)
	{
		//创建对象
		try
		{
			pGamePropertyTypeItem=new CGamePropertyTypeItem;
			if (pGamePropertyTypeItem==NULL) return false;
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pGamePropertyTypeItem,sizeof(CGamePropertyTypeItem));
	}

	//设置数据
	CopyMemory(&pGamePropertyTypeItem->m_PropertyTypeItem,ptagPropertyTypeItem,sizeof(tagPropertyTypeItem));

	//设置索引
	m_GamePropertyTypeItemMap[ptagPropertyTypeItem->dwTypeID]=pGamePropertyTypeItem;

	return true;
}

//插入关系
bool CGamePropertyListData::InsertGamePropertyRelatItem(tagPropertyRelatItem * ptagPropertyRelatItem)
{
	//效验参数
	ASSERT(ptagPropertyRelatItem!=NULL);
	if (ptagPropertyRelatItem==NULL) return false;

	//查找现存
	CGamePropertyRelatItem * pPropertyRelatItem=NULL;

	bool bFinder = false;
	DWORD  dwkey= 0;
	DWORD  dwCurrKey =0;
	POSITION Position=NULL;
	while (true)
	{
		if (Position==NULL) Position=m_GamePropertyRelatItemMap.GetStartPosition();
		if (Position!=NULL) m_GamePropertyRelatItemMap.GetNextAssoc(Position,dwkey,pPropertyRelatItem);

		if (pPropertyRelatItem==NULL) break;

		if (pPropertyRelatItem->m_PropertyRelatItem.dwPropertyID == ptagPropertyRelatItem->dwPropertyID
			&& pPropertyRelatItem->m_PropertyRelatItem.dwTypeID == ptagPropertyRelatItem->dwTypeID)
		{
			bFinder = true;
			break;
		}
		dwCurrKey++;

		if (Position==NULL) break;
	}

	//获取子项
	if (bFinder == false)
	{
		//创建对象
		try
		{
			pPropertyRelatItem=new CGamePropertyRelatItem;
			if (pPropertyRelatItem==NULL) return false;
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pPropertyRelatItem,sizeof(CGamePropertyRelatItem));
		dwCurrKey++;
	}
	else
	{
		dwCurrKey = dwkey;
	}

	//设置数据
	CopyMemory(&pPropertyRelatItem->m_PropertyRelatItem,ptagPropertyRelatItem,sizeof(tagPropertyRelatItem));

	//设置索引
	m_GamePropertyRelatItemMap[dwCurrKey]=pPropertyRelatItem;

	return true;
}


//插入道具
bool CGamePropertyListData::InsertGamePropertyItem(tagPropertyItem * ptagPropertyItem)
{
	//效验参数
	ASSERT(ptagPropertyItem!=NULL);
	if (ptagPropertyItem==NULL) return false;

	//查找现存
	CGamePropertyItem * pGamePropertyItem=NULL;

	//获取子项
	if (m_GamePropertyItemMap.Lookup(ptagPropertyItem->dwPropertyID,pGamePropertyItem)==FALSE)
	{
		//创建对象
		try
		{
			pGamePropertyItem=new CGamePropertyItem;
			if (pGamePropertyItem==NULL) return false;
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pGamePropertyItem,sizeof(CGamePropertyItem));
	}


	//设置数据
	CopyMemory(&pGamePropertyItem->m_PropertyItem,ptagPropertyItem,sizeof(tagPropertyItem));

	//设置索引
	m_GamePropertyItemMap[ptagPropertyItem->dwPropertyID]=pGamePropertyItem;

	return true;
}

//插入子道具
bool CGamePropertyListData::InsertGamePropertySubItem(tagPropertySubItem * ptagPropertySubItem)
{
	//效验参数
	ASSERT(ptagPropertySubItem!=NULL);
	if (ptagPropertySubItem==NULL) return false;

	//查找现存
	CGamePropertySubItem * pGamePropertySubItem=NULL;

	bool bFinder = false;
	DWORD  dwkey= 0;
	DWORD  dwCurrKey =0;
	POSITION Position=NULL;
	while (true)
	{
		if (Position==NULL) Position=m_GamePropertySubItemMap.GetStartPosition();
		if (Position!=NULL) m_GamePropertySubItemMap.GetNextAssoc(Position,dwkey,pGamePropertySubItem);

		if (pGamePropertySubItem==NULL) break;

		if (pGamePropertySubItem->m_PropertySubItem.dwPropertyID == ptagPropertySubItem->dwPropertyID
			&& pGamePropertySubItem->m_PropertySubItem.dwOwnerPropertyID == ptagPropertySubItem->dwOwnerPropertyID)
		{
			bFinder = true;
			break;
		}
		dwCurrKey++;

		if (Position==NULL) break;
	}

	//获取子项
	if (bFinder == false)
	{
		//创建对象
		try
		{
			pGamePropertySubItem=new CGamePropertySubItem;
			if (pGamePropertySubItem==NULL) return false;
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pGamePropertySubItem,sizeof(CGamePropertySubItem));
		dwCurrKey++;
	}
	else
	{
		dwCurrKey = dwkey;
	}

	//设置数据
	CopyMemory(&pGamePropertySubItem->m_PropertySubItem,ptagPropertySubItem,sizeof(tagPropertySubItem));

	//设置索引
	m_GamePropertySubItemMap[dwCurrKey]=pGamePropertySubItem;

	return true;
}

//删除种类
bool CGamePropertyListData::DeleteGamePropertyTypeItem(DWORD dwTypeID)
{
	//查找种类
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	if (m_GamePropertyTypeItemMap.Lookup(dwTypeID,pGamePropertyTypeItem)==FALSE) return false;

	//删除数据
	m_GamePropertyTypeItemMap.RemoveKey(dwTypeID);

	return true;
}


//删除道具
bool CGamePropertyListData::DeleteGamePropertyItem(DWORD dwPropertyID)
{
	//查找道具
	CGamePropertyItem * pGamePropertyItem=NULL;
	if (m_GamePropertyItemMap.Lookup(dwPropertyID,pGamePropertyItem)==FALSE) return false;

	//删除数据
	m_GamePropertyItemMap.RemoveKey(dwPropertyID);

	return true;
}


//枚举种类
CGamePropertyTypeItem * CGamePropertyListData::EmunGamePropertyTypeItem(POSITION & Position)
{
	//变量定义
	DWORD wKey=0;
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_GamePropertyTypeItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertyTypeItemMap.GetNextAssoc(Position,wKey,pGamePropertyTypeItem);

	return pGamePropertyTypeItem;
}

//枚举道具
CGamePropertyRelatItem * CGamePropertyListData::EmunGamePropertyRelatItem(POSITION & Position)
{
	//变量定义
	DWORD wKey=0;
	CGamePropertyRelatItem * pGamePropertyRelatItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_GamePropertyRelatItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertyRelatItemMap.GetNextAssoc(Position,wKey,pGamePropertyRelatItem);

	return pGamePropertyRelatItem;
}


//枚举道具
CGamePropertyItem * CGamePropertyListData::EmunGamePropertyItem(POSITION & Position)
{
	//变量定义
	DWORD wKey=0;
	CGamePropertyItem * pGamePropertyItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_GamePropertyItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertyItemMap.GetNextAssoc(Position,wKey,pGamePropertyItem);

	return pGamePropertyItem;
}

//枚举子道具
CGamePropertySubItem * CGamePropertyListData::EmunGamePropertySubItem(POSITION & Position)
{
	//变量定义
	DWORD wKey=0;
	CGamePropertySubItem * pGamePropertySubItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_GamePropertySubItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertySubItemMap.GetNextAssoc(Position,wKey,pGamePropertySubItem);

	return pGamePropertySubItem;
}

//查找种类
CGamePropertyTypeItem * CGamePropertyListData::SearchGamePropertyTypeItem(DWORD dwTypeID)
{
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	m_GamePropertyTypeItemMap.Lookup(dwTypeID,pGamePropertyTypeItem);
	return pGamePropertyTypeItem;
}

//查找道具
CGamePropertyItem * CGamePropertyListData::SearchGamePropertyItem(DWORD dwPropertyID)
{
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_GamePropertyItemMap.Lookup(dwPropertyID,pGamePropertyItem);
	return pGamePropertyItem;
}



//////////////////////////////////////////////////////////////////////////////////
