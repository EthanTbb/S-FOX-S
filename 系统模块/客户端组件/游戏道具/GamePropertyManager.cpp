#include "StdAfx.h"
#include "GamePropertyManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CGamePropertyManager * CGamePropertyManager::m_pGamePropertyManager=NULL;//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyManager::CGamePropertyManager()
{
	//���ñ���
	ASSERT(m_pGamePropertyManager==NULL);
	if (m_pGamePropertyManager==NULL) m_pGamePropertyManager=this;

	return;
}

//��������
CGamePropertyManager::~CGamePropertyManager()
{
	//���ñ���
	ASSERT(m_pGamePropertyManager==this);
	if (m_pGamePropertyManager==this) m_pGamePropertyManager=NULL;

	//�ͷŶ���
	if(m_GamePropertyBaseMap.GetCount()>0)
	{
		//��������
		POSITION Position=m_GamePropertyBaseMap.GetStartPosition();

		//�ͷŶ���
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

	//�Ƴ�Ԫ��
	m_GamePropertyBaseMap.RemoveAll();

	return;
}

//��ȡ����
CGamePropertyBase * CGamePropertyManager::GetPropertyItem(DWORD dwPropertyID)
{
	//���ҵ���
	CGamePropertyBase * pGamePropertyItem=NULL;
	m_GamePropertyBaseMap.Lookup(dwPropertyID,pGamePropertyItem);

	return pGamePropertyItem;
}

//��������
CGamePropertyBase * CGamePropertyManager::CreatePropertyItem(tagPropertyItem & PropertyItem)
{

	//�����ִ�
	CGamePropertyBase * pGamePropertyItem=NULL;
	m_GamePropertyBaseMap.Lookup(PropertyItem.dwPropertyID,pGamePropertyItem);

	//�ִ��ж�
	if (pGamePropertyItem!=NULL)
	{
		//���µ���
		pGamePropertyItem->m_PropertyItem=PropertyItem;
		return pGamePropertyItem;
	}

	//��������
	switch (PropertyItem.dwPropertyID)
	{
	case PROPERTY_ID_CAR:			//��������
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
	case PROPERTY_ID_EGG:			//��������
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
	case PROPERTY_ID_CLAP:			//��������
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
	case PROPERTY_ID_KISS:			//��������
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
	case PROPERTY_ID_BEER:			//ơ������
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
	case PROPERTY_ID_CAKE:			//��������
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
	case PROPERTY_ID_RING:			//�������
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
	case PROPERTY_ID_BEAT:			//��������
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
	case PROPERTY_ID_BOMB:			//ը������
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
	case PROPERTY_ID_SMOKE:			//��������
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
	case PROPERTY_ID_VILLA:			//��������
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
	case PROPERTY_ID_BRICK:			//שͷ����
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
	case PROPERTY_ID_FLOWER:		//�ʻ�����
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
	case PROPERTY_ID_TWO_CARD:    //˫������
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
	case PROPERTY_ID_FOUR_CARD:    //�ı�����
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
	case PROPERTY_ID_SCORE_CLEAR:    //��������
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
	case PROPERTY_ID_ESCAPE_CLEAR:    //��������
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
	case PROPERTY_ID_TRUMPET:		  //С����
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
	case PROPERTY_ID_TYPHON:		  //������
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
	case PROPERTY_ID_GUARDKICK_CARD:   //���߿�
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
	case PROPERTY_ID_POSSESS:		  //�����
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
	case PROPERTY_ID_DOUBLE_1HOUR:	//1Сʱ˫����
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
	case PROPERTY_ID_DOUBLE_3HOUR:	 //3Сʱ˫����
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
	case PROPERTY_ID_DOUBLE_1WEEK:	 //1��˫����
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
	case PROPERTY_ID_DOUBLE_1DAY:	 //1��˫����
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
	case PROPERTY_ID_AMULET_1HOUR:	//1Сʱ����
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
	case PROPERTY_ID_AMULET_3HOUR:	 //3Сʱ����
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
	case PROPERTY_ID_AMULET_1WEEK:	 //1�ܻ���
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
	case PROPERTY_ID_AMULET_1DAY:	 //1�커��
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
	case PROPERTY_ID_GUARDKICK_1HOUR:	//1Сʱ���߿�
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
	case PROPERTY_ID_GUARDKICK_3HOUR:	 //3Сʱ���߿�
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
	case PROPERTY_ID_GUARDKICK_1WEEK:	 //1�ܷ��߿�
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
	case PROPERTY_ID_GUARDKICK_1DAY:	 //1����߿�
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
	case PROPERTY_ID_CRYSTAL:			//ˮ��	
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
	case PROPERTY_ID_AGATE:			//���	
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
	case PROPERTY_ID_BOULDER:			//��ʯ	
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
	case PROPERTY_ID_GEM:			//��ʯ	
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
	case PROPERTY_ID_DIAMOND:			//��ʯ	
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
	//case PROPERTY_ID_BLUERING_CARD:	  //�����Ա��
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
	//case PROPERTY_ID_YELLOWRING_CARD:	//�����Ա��
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
	//case PROPERTY_ID_WHITERING_CARD:	//�����Ա��
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
	//case PROPERTY_ID_REDRING_CARD:		//�����Ա��
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
	//case PROPERTY_ID_VIPROOM_CARD:		//VIP����
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

	//�����ж�
	if (pGamePropertyItem!=NULL)
	{
		//���õ���
		pGamePropertyItem->m_PropertyItem=PropertyItem;
		m_GamePropertyBaseMap[pGamePropertyItem->m_PropertyItem.dwPropertyID]=pGamePropertyItem;

		return pGamePropertyItem;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyTypeItem::CGamePropertyTypeItem()
{
	//���ñ���
	ZeroMemory(&m_PropertyTypeItem,sizeof(m_PropertyTypeItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyRelatItem::CGamePropertyRelatItem()
{
	//���ñ���
	ZeroMemory(&m_PropertyRelatItem,sizeof(m_PropertyRelatItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyItem::CGamePropertyItem()
{
	//���ñ���
	ZeroMemory(&m_PropertyItem,sizeof(m_PropertyItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertySubItem::CGamePropertySubItem()
{
	//���ñ���
	ZeroMemory(&m_PropertySubItem,sizeof(m_PropertySubItem));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

CGamePropertyListData * CGamePropertyListData::m_pGamePropertyListData = NULL;
//���캯��
CGamePropertyListData::CGamePropertyListData()
{
	//��������
	m_GamePropertyTypeItemMap.InitHashTable(PRIME_TYPE);
	m_GamePropertyRelatItemMap.InitHashTable(PRIME_KIND);
	m_GamePropertyItemMap.InitHashTable(PRIME_SERVER);
	m_GamePropertySubItemMap.InitHashTable(PRIME_KIND);

	//���ö���
	ASSERT(m_pGamePropertyListData==NULL);
	if (m_pGamePropertyListData==NULL) m_pGamePropertyListData=this;

	return;
}

//��������
CGamePropertyListData::~CGamePropertyListData()
{
	//��������
	DWORD wKey=0;

	//ɾ������
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	POSITION Position=m_GamePropertyTypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertyTypeItemMap.GetNextAssoc(Position,wKey,pGamePropertyTypeItem);
		SafeDelete(pGamePropertyTypeItem);
	}
	m_GamePropertyTypeItemMap.RemoveAll();


	//ɾ����ϵ
	CGamePropertyRelatItem * pGamePropertyRelatItem=NULL;
	Position=m_GamePropertyRelatItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertyRelatItemMap.GetNextAssoc(Position,wKey,pGamePropertyRelatItem);
		SafeDelete(pGamePropertyRelatItem);
	}
	m_GamePropertyRelatItemMap.RemoveAll();

	//ɾ������
	CGamePropertyItem * pGamePropertyItem=NULL;
	Position=m_GamePropertyItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertyItemMap.GetNextAssoc(Position,wKey,pGamePropertyItem);
		SafeDelete(pGamePropertyItem);
	}
	m_GamePropertyItemMap.RemoveAll();

	//ɾ���ӵ���
	CGamePropertySubItem * pGamePropertySubItem=NULL;
	Position=m_GamePropertySubItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePropertySubItemMap.GetNextAssoc(Position,wKey,pGamePropertySubItem);
		SafeDelete(pGamePropertySubItem);
	}
	m_GamePropertySubItemMap.RemoveAll();

	//�ͷŶ���
	ASSERT(m_pGamePropertyListData==this);
	if (m_pGamePropertyListData==this) m_pGamePropertyListData=NULL;

	return;
}

//��������
bool CGamePropertyListData::InsertGamePropertyTypeItem(tagPropertyTypeItem * ptagPropertyTypeItem)
{
	//Ч�����
	ASSERT(ptagPropertyTypeItem!=NULL);
	if (ptagPropertyTypeItem==NULL) return false;

	//�����ִ�
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	if (m_GamePropertyTypeItemMap.Lookup(ptagPropertyTypeItem->dwTypeID,pGamePropertyTypeItem)==FALSE)
	{
		//��������
		try
		{
			pGamePropertyTypeItem=new CGamePropertyTypeItem;
			if (pGamePropertyTypeItem==NULL) return false;
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pGamePropertyTypeItem,sizeof(CGamePropertyTypeItem));
	}

	//��������
	CopyMemory(&pGamePropertyTypeItem->m_PropertyTypeItem,ptagPropertyTypeItem,sizeof(tagPropertyTypeItem));

	//��������
	m_GamePropertyTypeItemMap[ptagPropertyTypeItem->dwTypeID]=pGamePropertyTypeItem;

	return true;
}

//�����ϵ
bool CGamePropertyListData::InsertGamePropertyRelatItem(tagPropertyRelatItem * ptagPropertyRelatItem)
{
	//Ч�����
	ASSERT(ptagPropertyRelatItem!=NULL);
	if (ptagPropertyRelatItem==NULL) return false;

	//�����ִ�
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

	//��ȡ����
	if (bFinder == false)
	{
		//��������
		try
		{
			pPropertyRelatItem=new CGamePropertyRelatItem;
			if (pPropertyRelatItem==NULL) return false;
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pPropertyRelatItem,sizeof(CGamePropertyRelatItem));
		dwCurrKey++;
	}
	else
	{
		dwCurrKey = dwkey;
	}

	//��������
	CopyMemory(&pPropertyRelatItem->m_PropertyRelatItem,ptagPropertyRelatItem,sizeof(tagPropertyRelatItem));

	//��������
	m_GamePropertyRelatItemMap[dwCurrKey]=pPropertyRelatItem;

	return true;
}


//�������
bool CGamePropertyListData::InsertGamePropertyItem(tagPropertyItem * ptagPropertyItem)
{
	//Ч�����
	ASSERT(ptagPropertyItem!=NULL);
	if (ptagPropertyItem==NULL) return false;

	//�����ִ�
	CGamePropertyItem * pGamePropertyItem=NULL;

	//��ȡ����
	if (m_GamePropertyItemMap.Lookup(ptagPropertyItem->dwPropertyID,pGamePropertyItem)==FALSE)
	{
		//��������
		try
		{
			pGamePropertyItem=new CGamePropertyItem;
			if (pGamePropertyItem==NULL) return false;
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pGamePropertyItem,sizeof(CGamePropertyItem));
	}


	//��������
	CopyMemory(&pGamePropertyItem->m_PropertyItem,ptagPropertyItem,sizeof(tagPropertyItem));

	//��������
	m_GamePropertyItemMap[ptagPropertyItem->dwPropertyID]=pGamePropertyItem;

	return true;
}

//�����ӵ���
bool CGamePropertyListData::InsertGamePropertySubItem(tagPropertySubItem * ptagPropertySubItem)
{
	//Ч�����
	ASSERT(ptagPropertySubItem!=NULL);
	if (ptagPropertySubItem==NULL) return false;

	//�����ִ�
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

	//��ȡ����
	if (bFinder == false)
	{
		//��������
		try
		{
			pGamePropertySubItem=new CGamePropertySubItem;
			if (pGamePropertySubItem==NULL) return false;
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pGamePropertySubItem,sizeof(CGamePropertySubItem));
		dwCurrKey++;
	}
	else
	{
		dwCurrKey = dwkey;
	}

	//��������
	CopyMemory(&pGamePropertySubItem->m_PropertySubItem,ptagPropertySubItem,sizeof(tagPropertySubItem));

	//��������
	m_GamePropertySubItemMap[dwCurrKey]=pGamePropertySubItem;

	return true;
}

//ɾ������
bool CGamePropertyListData::DeleteGamePropertyTypeItem(DWORD dwTypeID)
{
	//��������
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	if (m_GamePropertyTypeItemMap.Lookup(dwTypeID,pGamePropertyTypeItem)==FALSE) return false;

	//ɾ������
	m_GamePropertyTypeItemMap.RemoveKey(dwTypeID);

	return true;
}


//ɾ������
bool CGamePropertyListData::DeleteGamePropertyItem(DWORD dwPropertyID)
{
	//���ҵ���
	CGamePropertyItem * pGamePropertyItem=NULL;
	if (m_GamePropertyItemMap.Lookup(dwPropertyID,pGamePropertyItem)==FALSE) return false;

	//ɾ������
	m_GamePropertyItemMap.RemoveKey(dwPropertyID);

	return true;
}


//ö������
CGamePropertyTypeItem * CGamePropertyListData::EmunGamePropertyTypeItem(POSITION & Position)
{
	//��������
	DWORD wKey=0;
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_GamePropertyTypeItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertyTypeItemMap.GetNextAssoc(Position,wKey,pGamePropertyTypeItem);

	return pGamePropertyTypeItem;
}

//ö�ٵ���
CGamePropertyRelatItem * CGamePropertyListData::EmunGamePropertyRelatItem(POSITION & Position)
{
	//��������
	DWORD wKey=0;
	CGamePropertyRelatItem * pGamePropertyRelatItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_GamePropertyRelatItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertyRelatItemMap.GetNextAssoc(Position,wKey,pGamePropertyRelatItem);

	return pGamePropertyRelatItem;
}


//ö�ٵ���
CGamePropertyItem * CGamePropertyListData::EmunGamePropertyItem(POSITION & Position)
{
	//��������
	DWORD wKey=0;
	CGamePropertyItem * pGamePropertyItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_GamePropertyItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertyItemMap.GetNextAssoc(Position,wKey,pGamePropertyItem);

	return pGamePropertyItem;
}

//ö���ӵ���
CGamePropertySubItem * CGamePropertyListData::EmunGamePropertySubItem(POSITION & Position)
{
	//��������
	DWORD wKey=0;
	CGamePropertySubItem * pGamePropertySubItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_GamePropertySubItemMap.GetStartPosition();
	if (Position!=NULL) m_GamePropertySubItemMap.GetNextAssoc(Position,wKey,pGamePropertySubItem);

	return pGamePropertySubItem;
}

//��������
CGamePropertyTypeItem * CGamePropertyListData::SearchGamePropertyTypeItem(DWORD dwTypeID)
{
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	m_GamePropertyTypeItemMap.Lookup(dwTypeID,pGamePropertyTypeItem);
	return pGamePropertyTypeItem;
}

//���ҵ���
CGamePropertyItem * CGamePropertyListData::SearchGamePropertyItem(DWORD dwPropertyID)
{
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_GamePropertyItemMap.Lookup(dwPropertyID,pGamePropertyItem);
	return pGamePropertyItem;
}



//////////////////////////////////////////////////////////////////////////////////
