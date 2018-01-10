#include "StdAfx.h"
#include "Resource.h"
#include "TableFrameSink.h"
#include "GameServiceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager()
{
	//�ؼ�����
	//m_pDlgCustomRule=NULL;

	//�ں�����
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_EDUCATE);

	//���ܱ�־
	m_GameServiceAttrib.cbDynamicJoin=FALSE;
	m_GameServiceAttrib.cbAndroidUser=FALSE;
	m_GameServiceAttrib.cbOffLineTrustee=FALSE;

	//��������
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPRunFastHNDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("RunFastHN.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("RunFastHNServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager()
{
}

//�ӿڲ�ѯ
VOID * CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
//	QUERYINTERFACE(IGameServiceCustomRule,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
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
	//���ֵ���
	GameServiceOption.lCellScore=__max(1L,GameServiceOption.lCellScore);


	//�Ƹ�����
	if ((GameServiceOption.wServerType&GAME_GENRE_GOLD)!=0)
	{
		GameServiceOption.lMinTableScore=__max(GameServiceOption.lCellScore*52,GameServiceOption.lMinTableScore);
	}

	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}
	return true;
}

////��ȡ����
//bool CGameServiceManager::SaveCustomRule()
//{
//	return true;
//}

////��������
//HWND CGameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
//{
//	//��������
//	if (m_pDlgCustomRule==NULL)
//	{
//		m_pDlgCustomRule=new CDlgCustomRule;
//	}
//
//	//��������
//	if (m_pDlgCustomRule->m_hWnd==NULL)
//	{
//		//������Դ
//		AfxSetResourceHandle(GetModuleHandle(m_GameServiceAttrib.szServerDLLName));
//
//		//��������
//		m_pDlgCustomRule->Create(IDD_CUSTOM_RULE,pParentWnd);
//
//		//��ԭ��Դ
//		AfxSetResourceHandle(GetModuleHandle(NULL));
//	}
//
//	//���ñ���
//	m_pDlgCustomRule->SetCustomRule(pcbCustomRule,wCustonSize);
//
//	//��ʾ����
//	m_pDlgCustomRule->SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);
//
//	return m_pDlgCustomRule->GetSafeHwnd();
//}

//////////////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	static CGameServiceManager GameServiceManager;
	return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////////////
