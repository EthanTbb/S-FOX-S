#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
typedef CMap<DWORD,DWORD,CGamePropertyBase *,CGamePropertyBase *> CGamePropertyBaseMap;

//////////////////////////////////////////////////////////////////////////////////

//���߹���
class GAME_PROPERTY_CLASS CGamePropertyManager
{
	//��������
protected:
	CGamePropertyBaseMap				m_GamePropertyBaseMap;				//��������

	//��̬����
protected:
	static CGamePropertyManager *	m_pGamePropertyManager;				//����ָ��

	//��������
public:
	//���캯��
	CGamePropertyManager();
	//��������
	virtual ~CGamePropertyManager();

	//���ú���
public:
	//��ȡ����
	CGamePropertyBase * GetPropertyItem(DWORD dwPropertyID);
	//��������
	CGamePropertyBase * CreatePropertyItem(tagPropertyItem & PropertyItem);

	//��̬����
public:
	//��ȡ����
	static CGamePropertyManager * GetInstance() { return m_pGamePropertyManager; }
};

//////////////////////////////////////////////////////////////////////////////////

//����ṹ
class  GAME_PROPERTY_CLASS CGamePropertyTypeItem 
{
	//��������
public:
	tagPropertyTypeItem				m_PropertyTypeItem;	

	//��������
public:
	//���캯��
	CGamePropertyTypeItem();

	//���غ���
public:
	//��������
	virtual DWORD GetSortID() { return m_PropertyTypeItem.dwSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//��ϵ�ṹ
class GAME_PROPERTY_CLASS CGamePropertyRelatItem 
{
	//��������
public:
	tagPropertyRelatItem				m_PropertyRelatItem;	

	//��������
public:
	//���캯��
	CGamePropertyRelatItem();
};

//////////////////////////////////////////////////////////////////////////////////

//���߽ṹ
class  GAME_PROPERTY_CLASS CGamePropertyItem 
{
	//��������
public:
	tagPropertyItem					m_PropertyItem;	

	//��������
public:
	//���캯��
	CGamePropertyItem();
	//���غ���
public:
	//��������
	virtual DWORD GetSortID() { return m_PropertyItem.dwSortID;; }
};

//////////////////////////////////////////////////////////////////////////////////

//���߽ṹ
class GAME_PROPERTY_CLASS CGamePropertySubItem 
{
	//��������
public:
	tagPropertySubItem					m_PropertySubItem;	

	//��������
public:
	//���캯��
	CGamePropertySubItem();
};

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CWHArray<CGamePropertyTypeItem *> CGamePropertyTypeItemArray;
typedef CWHArray<CGamePropertyRelatItem *>	CGamePropertyRelatItemArray;
typedef CWHArray<CGamePropertyItem *>	CGamePropertyItemArray;
typedef CWHArray<CGamePropertySubItem *>	CGamePropertySubItemArray;


//����˵��
typedef CMap<DWORD,DWORD,CGamePropertyTypeItem *,CGamePropertyTypeItem * &> CGamePropertyTypeItemMap;
typedef CMap<DWORD,DWORD,CGamePropertyRelatItem *,CGamePropertyRelatItem * &> CGamePropertyRelatItemMap;
typedef CMap<DWORD,DWORD,CGamePropertyItem *,CGamePropertyItem * &> CGamePropertyItemMap;
typedef CMap<DWORD,DWORD,CGamePropertySubItem *,CGamePropertySubItem * &> CGamePropertySubItemMap;

//////////////////////////////////////////////////////////////////////////////////
//�б����
class GAME_PROPERTY_CLASS CGamePropertyListData
{
	//��������
protected:
	CGamePropertyTypeItemMap					m_GamePropertyTypeItemMap;				//��������
	CGamePropertyRelatItemMap					m_GamePropertyRelatItemMap;				//��ϵ����
	CGamePropertyItemMap						m_GamePropertyItemMap;					//��������
	CGamePropertySubItemMap						m_GamePropertySubItemMap;				//�ӵ�������

	//��������
public:
	//���캯��
	CGamePropertyListData();
	//��������
	virtual ~CGamePropertyListData();

	//��̬����
protected:
	static CGamePropertyListData *		m_pGamePropertyListData;					//�б�����

	//����ӿ�
public:
	//��������
	bool InsertGamePropertyTypeItem(tagPropertyTypeItem * ptagPropertyTypeItem);
	//�����ϵ
	bool InsertGamePropertyRelatItem(tagPropertyRelatItem * ptagPropertyRelatItem);
	//�������
	bool InsertGamePropertyItem(tagPropertyItem * ptagPropertyItem);
	//�����ӵ���
	bool InsertGamePropertySubItem(tagPropertySubItem * ptagPropertySubItem);

	//ɾ���ӿ�
public:
	//ɾ������
	bool DeleteGamePropertyTypeItem(DWORD dwTypeID);
	//ɾ������
	bool DeleteGamePropertyItem(DWORD wPropertyID);

	//ö�ٽӿ�
public:
	//ö������
	CGamePropertyTypeItem * EmunGamePropertyTypeItem(POSITION & Position);
	//ö�ٵ���
	CGamePropertyRelatItem * EmunGamePropertyRelatItem(POSITION & Position);
	//ö�ٵ���
	CGamePropertyItem * EmunGamePropertyItem(POSITION & Position);
	//ö���ӵ���
	CGamePropertySubItem * EmunGamePropertySubItem(POSITION & Position);

	//���ҽӿ�
public:
	//��������
	CGamePropertyTypeItem * SearchGamePropertyTypeItem(DWORD dwTypeID);
	//���ҵ���
	CGamePropertyItem * SearchGamePropertyItem(DWORD wPropertyID);


	//��Ŀ�ӿ�
public:
	//������Ŀ
	DWORD GetGamePropertyTypeCount() { return (DWORD)m_GamePropertyTypeItemMap.GetCount(); }
	//������Ŀ
	DWORD GetGamePropertyRelatCount() { return (DWORD)m_GamePropertyRelatItemMap.GetCount(); }
	//������Ŀ
	DWORD GetGamePropertyCount() { return (DWORD)m_GamePropertyItemMap.GetCount(); }
	//�ӵ�����Ŀ
	DWORD GetGamePropertySubCount() { return (DWORD)m_GamePropertySubItemMap.GetCount(); }


	//��̬����
public:
	//��ȡ����
	static CGamePropertyListData * GetInstance() { return m_pGamePropertyListData; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif