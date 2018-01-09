#include "StdAfx.h"
#include "TableFrameSink.h"
#include "GameServiceManager.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager()
{
	//���ñ���
	m_hDllInstance = NULL;

	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_EDUCATE);

	//���ܱ�־
	m_GameServiceAttrib.cbDynamicJoin=TRUE;
	m_GameServiceAttrib.cbAndroidUser=TRUE;
	m_GameServiceAttrib.cbOffLineTrustee=FALSE;

	//��������
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,szTreasureDB,CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPTreasureDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("HKFiveCardNewServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("HKFiveCardNew.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager()
{
	if( m_hDllInstance != NULL )
	{
		AfxFreeLibrary(m_hDllInstance);
		m_hDllInstance = NULL;
	}
}

//�ӿڲ�ѯ
VOID * CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//�������
bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	//���ñ���
	GameServiceAttrib=m_GameServiceAttrib;

	return true;
}

//��������
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//Ч�����
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL) return false;

	//��Ԫ����
	GameServiceOption.lCellScore=__max(1L,GameServiceOption.lCellScore);

	//��������
	//�����Ϸ
	//if (GameServiceOption.wServerType&(GAME_GENRE_GOLD|SCORE_GENRE_POSITIVE))
	{
		GameServiceOption.lMinTableScore=__max(100 * GameServiceOption.lCellScore, GameServiceOption.lMinTableScore);
	}

	//�������(ԭ���Ļ�������)
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore, GameServiceOption.lMinTableScore);
	}

	//��������
	if (GameServiceOption.wServerType!=GAME_GENRE_SCORE)
	{
		if(GameServiceOption.lMaxEnterScore<=GameServiceOption.lMinTableScore)
			GameServiceOption.lMaxEnterScore=0L;
		else if(GameServiceOption.lRestrictScore>0)
			GameServiceOption.lMaxEnterScore=__min(GameServiceOption.lMaxEnterScore,GameServiceOption.lRestrictScore);
	}

	return true;
}

//��������
VOID * CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;

	try
	{
		//��������
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		VOID * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��������
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	try
	{
		//����ģ��
		if (m_hDllInstance==NULL)
		{
			m_hDllInstance=AfxLoadLibrary(TEXT("HKFiveCardNewAndroid.dll"));
			if (m_hDllInstance==NULL) throw TEXT("�����˷���ģ�鲻����");
		}

		//Ѱ�Һ���
		ModuleCreateProc * CreateProc=(ModuleCreateProc *)GetProcAddress(m_hDllInstance,"CreateAndroidUserItemSink");
		if (CreateProc==NULL) throw TEXT("�����˷���ģ��������Ϸ�");

		//�������
		return CreateProc(Guid,dwQueryVer);
	}
	catch(...) {}

	return NULL;
}

//��������
VOID * CGameServiceManager::CreateGameDataBaseEngineSink( REFGUID Guid, DWORD dwQueryVer )
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	static CGameServiceManager GameServiceManager;
	return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
