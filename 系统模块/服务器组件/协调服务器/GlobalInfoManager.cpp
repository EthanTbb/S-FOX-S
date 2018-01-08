#include "StdAfx.h"
#include "GlobalInfoManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUserItem::CGlobalUserItem()
{
	//�û�����
	m_dwUserID=0L;
	m_dwGameID=0L;
	m_szNickName[0]=0;

	//�ȼ���Ϣ
	m_cbMemberOrder=0;
	m_cbMasterOrder=0;

	//��������
	m_pNextUserItemPtr=NULL;

	return;
}

//��������
CGlobalUserItem::~CGlobalUserItem()
{
}

void CGlobalUserItem::UpdateStatus(const WORD wTableID, const WORD wChairID, const BYTE cbUserStatus)
{
	m_UserInfo.wTableID = wTableID;
	m_UserInfo.wChairID = wChairID;
	m_UserInfo.cbUserStatus = cbUserStatus;
}

//ö�ٷ���
CGlobalServerItem * CGlobalUserItem::EnumServerItem(WORD wIndex)
{
	if (wIndex>=m_GlobalServerItemArray.GetCount()) return NULL;
	return m_GlobalServerItemArray[wIndex];
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalPlazaItem::CGlobalPlazaItem()
{
	//���ñ���
	m_wIndex=0;
	ZeroMemory(&m_GamePlaza,sizeof(m_GamePlaza));

	//��������
	m_pNextPlazaItemPtr=NULL;

	return;
}

//��������
CGlobalPlazaItem::~CGlobalPlazaItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalChatItem::CGlobalChatItem()
{
	//���ñ���
	m_wIndex=0;
	ZeroMemory(&m_ChatServer,sizeof(m_ChatServer));

	//��������
	m_pNextChatServerPtr=NULL;

	return;
}

//��������
CGlobalChatItem::~CGlobalChatItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalServerItem::CGlobalServerItem()
{
	//���ñ���
	m_wIndex=0;
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//��������
	m_pNextServerItemPtr=NULL;

	//��������
	m_MapUserID.InitHashTable(PRIME_SERVER_USER);

	return;
}

//��������
CGlobalServerItem::~CGlobalServerItem()
{
}

//Ѱ���û�
CGlobalUserItem * CGlobalServerItem::SearchUserItem(DWORD dwUserID)
{
	//�����û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalInfoManager::CGlobalInfoManager()
{
	//�洢����
	m_pGlobalUserItem=NULL;
	m_pGlobalPlazaItem=NULL;
	m_pGlobalServerItem=NULL;
	m_pGlobalChatItem = NULL;

	//��������
	m_MapPlazaID.InitHashTable(PRIME_SERVER);
	m_MapServerID.InitHashTable(PRIME_SERVER);
	m_MapUserID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapGameID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapNickName.InitHashTable(PRIME_PLATFORM_USER);
	m_MapChatID.InitHashTable(PRIME_SERVER);
	//˽�˷�
	m_MapPersonalTableInfo.InitHashTable(MAX_SERVER);
	m_ServerTableCountArray.RemoveAll();
	return;
}

//��������
CGlobalInfoManager::~CGlobalInfoManager()
{
	//ɾ���û�
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		SafeDelete(pGlobalUserItem);
	}
	m_MapUserID.RemoveAll();

	//ɾ���㳡
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		SafeDelete(pGlobalPlazaItem);
	}
	m_MapPlazaID.RemoveAll();

	//ɾ������
	WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		SafeDelete(pGlobalServerItem);
	}
	m_MapServerID.RemoveAll();

	//ɾ������
	WORD wChatID=0;
	CGlobalChatItem * pGlobalChatItem=NULL;
	Position=m_MapChatID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapChatID.GetNextAssoc(Position,wChatID,pGlobalChatItem);
		SafeDelete(pGlobalChatItem);
	}
	m_MapChatID.RemoveAll();

	//ɾ���û�
	while (m_pGlobalUserItem!=NULL)
	{
		pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		SafeDelete(pGlobalUserItem);
	}

	//ɾ���㳡
	while (m_pGlobalPlazaItem!=NULL)
	{
		pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		SafeDelete(pGlobalPlazaItem);
	}

	//ɾ������
	while (m_pGlobalServerItem!=NULL)
	{
		pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		SafeDelete(pGlobalServerItem);
	}

	//ɾ������
	while (m_pGlobalChatItem!=NULL)
	{
		pGlobalChatItem=m_pGlobalChatItem;
		m_pGlobalChatItem=m_pGlobalChatItem->m_pNextChatServerPtr;
		SafeDelete(pGlobalChatItem);
	}

	return;
}

//��������
VOID CGlobalInfoManager::ResetData()
{
	//ɾ���û�
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();

	//ɾ���û�
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		if (pGlobalUserItem!=NULL) FreeGlobalUserItem(pGlobalUserItem);
	}

	//ɾ���㳡
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();

	//ɾ���㳡
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		if (pGlobalPlazaItem!=NULL) FreeGlobalPlazaItem(pGlobalPlazaItem);
	}

	//ɾ������
	WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();

	//ɾ������
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		if (pGlobalServerItem!=NULL) FreeGlobalServerItem(pGlobalServerItem);
	}

	//ɾ������
	WORD wChatID=0;
	CGlobalChatItem * pGlobalChatItem=NULL;
	Position=m_MapChatID.GetStartPosition();

	//ɾ���㳡
	while (Position!=NULL)
	{
		m_MapChatID.GetNextAssoc(Position,wPlazaID,pGlobalChatItem);
		if (pGlobalChatItem!=NULL) FreeGlobalChatItem(pGlobalChatItem);
	}

	//ɾ������
	m_MapUserID.RemoveAll();
	m_MapGameID.RemoveAll();
	m_MapPlazaID.RemoveAll();
	m_MapServerID.RemoveAll();
	m_MapNickName.RemoveAll();
	m_MapChatID.RemoveAll();

	return;
}

//��������
tagPersonalTableInfo* CGlobalInfoManager::SearchTableByID(LPCTSTR lpszTableID)
{	
	//�������
	tagPersonalTableInfo* pPersonalTable = NULL;
	if(m_MapPersonalTableInfo.Lookup(lpszTableID, pPersonalTable) == TRUE)
		return pPersonalTable;
	return NULL;
}

tagPersonalTableInfo* CGlobalInfoManager::SearchTableByTableIDAndServerID(DWORD dwServerID, DWORD dwTableID)
{
	//�Ƴ�����
	POSITION Position = NULL;
	tagPersonalTableInfo* pPersonalTable = NULL;
	CString str;
	int iHostCreateRoomCount = 0;
	Position = m_MapPersonalTableInfo.GetStartPosition();
	while(Position != NULL)
	{
		m_MapPersonalTableInfo.GetNextAssoc(Position,str,pPersonalTable);
		if(pPersonalTable != NULL && pPersonalTable->dwServerID == dwServerID &&  pPersonalTable->dwTableID == dwTableID)
		{
			return pPersonalTable;
		}
	}

	return NULL;
}

//�������
bool CGlobalInfoManager::AddFreeServerTable(DWORD dwServerID)
{
	//���ҷ���
	int nSize = m_ServerTableCountArray.GetCount();
	CGlobalServerItem* pServerItem = SearchServerItem(dwServerID);
	if(pServerItem == NULL) return true;
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount->dwServerID == dwServerID)
		{
			if(pServerItem->m_GameServer.wTableCount < pServerTableCount->dwTableCount +1) return true;
			pServerTableCount->dwTableCount += 1;
			break;
		}
	}
	return true;
}

//�������
bool CGlobalInfoManager::AddServerTable(CString strServerID, tagPersonalTableInfo PersonalTable)
{
	//�������
	tagPersonalTableInfo* pPersonalTable = NULL;

	//��鷿������˽�˷������Ŀ�������������Ŀ���������Ŀ��������
	if (GetHostCreatePersonalRoomCount(PersonalTable.dwUserID) > MAX_CREATE_PERSONAL_ROOM)
	{
		return false;
	}

	//�������
	pPersonalTable = NULL;
	if(m_MapPersonalTableInfo.Lookup(strServerID, pPersonalTable) == FALSE)
	{
		pPersonalTable = new tagPersonalTableInfo;

		m_MapPersonalTableInfo[strServerID] = pPersonalTable;
		CopyMemory(pPersonalTable, &PersonalTable, sizeof(tagPersonalTableInfo));
	}
	else
	{
		return false;
	}

	return true;
}

//�Ƴ���������
bool CGlobalInfoManager::RemoveFreeServerTable(DWORD dwServerID)
{
	//���ҷ���
	int nSize = m_ServerTableCountArray.GetCount();
	CGlobalServerItem* pServerItem = SearchServerItem(dwServerID);
	if(pServerItem == NULL) return true;
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount->dwServerID == dwServerID)
		{
			if(pServerTableCount->dwTableCount < 1) return true;
			pServerTableCount->dwTableCount -= 1;
			break;
		}
	}
	return true;
}

//�Ƴ�����
bool CGlobalInfoManager::RemoveServerTable(DWORD dwServerID, DWORD dwTableID)
{
	//�Ƴ�����
	POSITION Position = NULL;
	tagPersonalTableInfo* pPersonalTable = NULL;
	CString str;

	Position = m_MapPersonalTableInfo.GetStartPosition();
	while(Position != NULL)
	{
		m_MapPersonalTableInfo.GetNextAssoc(Position,str,pPersonalTable);
		if(pPersonalTable != NULL && pPersonalTable->dwServerID == dwServerID && pPersonalTable->dwTableID == dwTableID)
		{
			//������ɢ�����������ɢ���Ӽ��� ˽�˷�
			//ͳ��ͬһ����������ɢ�ķ�������
			int nOneHostDissumeCount = 0;
			for (int i = 0; i < m_VecDissumePersonalTableInfo.size(); i++)
			{
				if (m_VecDissumePersonalTableInfo[i].dwUserID == pPersonalTable->dwUserID)
				{
					nOneHostDissumeCount++;
				}
			}

			//ɾ�����ȱ���ɢ��Լս��
			if (nOneHostDissumeCount > MAX_CREATE_PERSONAL_ROOM)
			{
				int nOneHostDissumeCount = 0;
				for (int i = 0; i < m_VecDissumePersonalTableInfo.size(); i++)
				{
					if (m_VecDissumePersonalTableInfo[i].dwUserID == pPersonalTable->dwUserID)
					{
						nOneHostDissumeCount = i;
						m_VecDissumePersonalTableInfo.erase(m_VecDissumePersonalTableInfo.begin() + i);
						break;
					}
				}

			}
			m_VecDissumePersonalTableInfo.push_back(*pPersonalTable);

			SafeDelete(pPersonalTable);
			m_MapPersonalTableInfo.RemoveKey(str);
			return true;
		}
	}
	return false;
}


//�Ƴ�����
bool CGlobalInfoManager::RemoveServerTable(DWORD dwServerID)
{
	//�Ƴ�����
	POSITION Position = NULL;
	tagPersonalTableInfo* pPersonalTable = NULL;
	CString str;

	Position = m_MapPersonalTableInfo.GetStartPosition();
	while(Position != NULL)
	{
		m_MapPersonalTableInfo.GetNextAssoc(Position,str,pPersonalTable);
		if (Position == NULL)
		{
			break;
		}
		if(pPersonalTable != NULL && pPersonalTable->dwServerID == dwServerID)
		{

			SafeDelete(pPersonalTable);
			m_MapPersonalTableInfo.RemoveKey(str);

		}
	}
	return true;
}


//��ȡ�������������з���
VOID CGlobalInfoManager::GetHostCreatePersonalRoom(tagHostCreatRoomInfo & HostCreatRoomInfo)
{
	TCHAR szInfo[260] = {0};
	//�Ƴ�����
	POSITION Position = NULL;
	tagPersonalTableInfo* pPersonalTable = NULL;
	CString str;
	int iHostCreateRoomCount = 0;
	Position = m_MapPersonalTableInfo.GetStartPosition();
	while(Position != NULL)
	{
		m_MapPersonalTableInfo.GetNextAssoc(Position,str,pPersonalTable);
		if(pPersonalTable != NULL && pPersonalTable->dwUserID == HostCreatRoomInfo.dwUserID && pPersonalTable->dwKindID == HostCreatRoomInfo.dwKindID)
		{
			lstrcpyn(HostCreatRoomInfo.szRoomID[iHostCreateRoomCount],  pPersonalTable->szRoomID, CountArray(pPersonalTable->szRoomID));
			iHostCreateRoomCount++;
			//������󷿼���Ŀ����
			if(iHostCreateRoomCount >= MAX_CREATE_PERSONAL_ROOM)
			{
				break;
			}
		}

	}

	//�����Ѿ���ɢ������
	for (int i = m_VecDissumePersonalTableInfo.size() - 1; i >= 0 ; i--)
	{
		if (m_VecDissumePersonalTableInfo[i].dwUserID == HostCreatRoomInfo.dwUserID && m_VecDissumePersonalTableInfo[i].dwKindID == HostCreatRoomInfo.dwKindID)
		{
			lstrcpyn(HostCreatRoomInfo.szRoomID[iHostCreateRoomCount],  m_VecDissumePersonalTableInfo[i].szRoomID, CountArray(m_VecDissumePersonalTableInfo[i].szRoomID));
			iHostCreateRoomCount++;
			if(iHostCreateRoomCount >= MAX_CREATE_PERSONAL_ROOM)
			{
				break;
			}
		}
	}
}


//��ȡ���������ķ��������
INT CGlobalInfoManager::GetHostCreatePersonalRoomCount(DWORD dwUserID)
{
	tagPersonalTableInfo* pPersonalTable = NULL;
	POSITION Position = NULL;
	int iHostCreateRoomCount = 0;
	CString str;
	Position = m_MapPersonalTableInfo.GetStartPosition();
	while(Position != NULL)
	{
		m_MapPersonalTableInfo.GetNextAssoc(Position,str,pPersonalTable);
		if(pPersonalTable != NULL && pPersonalTable->dwUserID == dwUserID )
		{
			iHostCreateRoomCount++;
		}
	}
	return  iHostCreateRoomCount;
}

//��ȡ����
DWORD CGlobalInfoManager::GetFreeServer(DWORD dwUserID, DWORD dwKindID, BYTE cbIsJoinGame)
{

	//��������
	POSITION Position;
	bool bExit = false;
	CString strServerID;

	//�������������Ϸ
	if (cbIsJoinGame)
	{
		tagPersonalTableInfo* pPersonalTableInfo;
		//�����û�
		Position=m_MapPersonalTableInfo.GetStartPosition();
		while(Position != NULL)
		{
			m_MapPersonalTableInfo.GetNextAssoc(Position,strServerID,pPersonalTableInfo);
			if(pPersonalTableInfo != NULL)
			{
				if(pPersonalTableInfo->dwUserID == dwUserID)
				{
					bExit = true;
					return 0;
				}
			}
		}
	}


	//���ҷ���
	int nSize = m_ServerTableCountArray.GetCount();
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount->dwKindID == dwKindID && pServerTableCount->dwTableCount > 0)
		{
			return pServerTableCount->dwServerID;
		}
	}
	return 0;
}

//���ɷ���ID
VOID CGlobalInfoManager::RandServerID(LPTSTR pszServerID, WORD wMaxCount)
{
	//�������
	TCHAR szScource[11] = TEXT("0123456789");
	TCHAR* lpszTmp = new TCHAR[wMaxCount+1];

	ZeroMemory(lpszTmp, sizeof(TCHAR)*(wMaxCount+1));

	bool bExit = true;
	while(bExit)
	{
		//����ID
		for(int i = 0; i < wMaxCount; ++i)
		{
			lpszTmp[i] = szScource[rand()%10];
		}

		//����ID
		tagPersonalTableInfo* pPersonalTable = NULL;
		if(m_MapPersonalTableInfo.Lookup(lpszTmp, pPersonalTable) == TRUE)
			bExit = true;
		else
			bExit = false;
	}

	//��ֹ�ַ�
	lpszTmp[wMaxCount]=0;

	//�ַ�ת��
	CW2CT strSrcData(lpszTmp);
	lstrcpyn(pszServerID,strSrcData,wMaxCount+1);

	//�ͷ��ڴ�
	SafeDeleteArray(lpszTmp);

	return;
}

//ɾ���û�
bool CGlobalInfoManager::DeleteUserItem(DWORD dwUserID, WORD wServerID)
{
	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(dwUserID,pGlobalUserItem)==FALSE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	CGlobalServerItem * pGlobalServerItem=NULL;
	INT_PTR nServerCount=pGlobalUserItem->m_GlobalServerItemArray.GetCount();

	//�˳�����
	for (INT_PTR i=0;i<nServerCount;i++)
	{
		//��ȡ����
		pGlobalServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

		//�����ж�
		if (pGlobalServerItem->GetServerID()==wServerID)
		{
			//ɾ������
			pGlobalServerItem->m_MapUserID.RemoveKey(dwUserID);
			pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);

			//�ͷ��û�
			if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0L) 
			{
				//�ͷ�����
				m_MapUserID.RemoveKey(dwUserID);
				m_MapGameID.RemoveKey(pGlobalUserItem->GetGameID());
				m_MapNickName.RemoveKey(pGlobalUserItem->GetNickName());

				//�ͷŶ���
				FreeGlobalUserItem(pGlobalUserItem);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�����û�
bool CGlobalInfoManager::ActiveUserItem(tagGlobalUserInfo & GlobalUserInfo, WORD wServerID)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(GlobalUserInfo.dwUserID,pGlobalUserItem)==FALSE)
	{
		//�����û�
		pGlobalUserItem=CreateGlobalUserItem();
		if (pGlobalUserItem==NULL) 
		{
			ASSERT(FALSE);
			return false;
		}

		//�����ǳ�
		TCHAR szNickName[LEN_NICKNAME]=TEXT("");
		lstrcpyn(szNickName,GlobalUserInfo.szNickName,CountArray(szNickName));

		//�����û�
		pGlobalUserItem->m_dwUserID=GlobalUserInfo.dwUserID;
		pGlobalUserItem->m_dwGameID=GlobalUserInfo.dwGameID;
		lstrcpyn(pGlobalUserItem->m_szNickName,GlobalUserInfo.szNickName,CountArray(pGlobalUserItem->m_szNickName));

		//������Ϣ
		pGlobalUserItem->m_cbGender=GlobalUserInfo.cbGender;
		pGlobalUserItem->m_cbMemberOrder=GlobalUserInfo.cbMemberOrder;
		pGlobalUserItem->m_cbMasterOrder=GlobalUserInfo.cbMasterOrder;
		memcpy(&pGlobalUserItem->m_UserInfo, &GlobalUserInfo.userInfo, sizeof(tagUserInfo));

		//�ǳ�����
		m_MapNickName[szNickName]=pGlobalUserItem;

		//��������
		m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;
		m_MapGameID[GlobalUserInfo.dwGameID]=pGlobalUserItem;
	}
	else
	{
		//�ظ��ж�
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			if (pGlobalUserItem->m_GlobalServerItemArray[i]->GetServerID()==wServerID)
			{
				ASSERT(FALSE);
				return false;
			}
		}
	}

	//���ù���
	pGlobalUserItem->m_GlobalServerItemArray.Add(pGlobalServerItem);
	pGlobalServerItem->m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;

	return true;
}

//ɾ���㳡
bool CGlobalInfoManager::DeletePlazaItem(WORD wPlazaID)
{
	//Ѱ�ҹ㳡
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//�ͷŹ㳡
	m_MapPlazaID.RemoveKey(wPlazaID);
	FreeGlobalPlazaItem(pGlobalPlazaItem);

	return true;
}

//����㳡
bool CGlobalInfoManager::ActivePlazaItem(WORD wBindIndex, tagGamePlaza & GamePlaza)
{
	//Ѱ�ҹ㳡
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(GamePlaza.wPlazaID,pGlobalPlazaItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//�����㳡
	pGlobalPlazaItem=CreateGlobalPlazaItem();
	if (pGlobalPlazaItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ù㳡
	pGlobalPlazaItem->m_wIndex=wBindIndex;
	pGlobalPlazaItem->m_GamePlaza=GamePlaza;

	//��������
	m_MapPlazaID[GamePlaza.wPlazaID]=pGlobalPlazaItem;

	return true;
}

//ɾ������
bool CGlobalInfoManager::DeleteServerItem(WORD wServerID)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	DWORD dwUserKey=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=pGlobalServerItem->m_MapUserID.GetStartPosition();

	//ɾ���û�
	while (Position!=NULL)
	{
		//��ȡ����
		pGlobalServerItem->m_MapUserID.GetNextAssoc(Position,dwUserKey,pGlobalUserItem);

		//�������
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			//��ȡ����
			CGlobalServerItem * pTempServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

			//�����ж�
			if (pTempServerItem->GetServerID()==wServerID)
			{
				pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);
				break;
			}
		}

		//�ͷ��û�
		if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0)
		{
			m_MapUserID.RemoveKey(dwUserKey);
			FreeGlobalUserItem(pGlobalUserItem);
		}
	}

	//�ͷŷ���
	m_MapServerID.RemoveKey(wServerID);
	FreeGlobalServerItem(pGlobalServerItem);

	//���ҷ���
	int nSize = m_ServerTableCountArray.GetCount();
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount->dwServerID == wServerID)
		{
			SafeDelete(pServerTableCount);
			m_ServerTableCountArray.RemoveAt(i);
			break;
		}
	}

	//ɾ���û���Ϣ
	CString strServerID;
	tagPersonalTableInfo* pPersonalTableInfo = NULL;
	Position = m_MapPersonalTableInfo.GetStartPosition();
	while(Position != NULL)
	{
		m_MapPersonalTableInfo.GetNextAssoc(Position,strServerID,pPersonalTableInfo);

		if(pPersonalTableInfo != NULL && pPersonalTableInfo->dwServerID == wServerID)
		{

			SafeDelete(pPersonalTableInfo);
			m_MapPersonalTableInfo.RemoveKey(strServerID);
		}
		pPersonalTableInfo = NULL;
	}

	//ɾ���û���Ϣ
	RemoveServerTable(wServerID);
	return true;
}

//�����
bool CGlobalInfoManager::ActiveServerItem(WORD wBindIndex, tagGameServer & GameServer)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(GameServer.wServerID,pGlobalServerItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	pGlobalServerItem=CreateGlobalServerItem();
	if (pGlobalServerItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//���÷���
	pGlobalServerItem->m_wIndex=wBindIndex;
	pGlobalServerItem->m_GameServer=GameServer;

	//��������
	m_MapServerID[GameServer.wServerID]=pGlobalServerItem;

	//����˽�˷���
	DWORD dwServerID = pGlobalServerItem->GetServerID();
	DWORD dwTableCount = pGlobalServerItem->GetTabelCount();

	//���ҷ���
	int nSize = m_ServerTableCountArray.GetCount();
	bool bExit = false;
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount != NULL && pServerTableCount->dwServerID == dwServerID)
		{
			bExit = true;
			break;
		}
	}

	if(bExit == false && pGlobalServerItem->m_GameServer.wServerType == GAME_GENRE_PERSONAL)
	{
		tagServerTableCount* pServerTableCount = new tagServerTableCount;
		pServerTableCount->dwKindID = pGlobalServerItem->GetKindID();
		pServerTableCount->dwServerID = dwServerID;
		pServerTableCount->dwTableCount = dwTableCount;

		m_ServerTableCountArray.Add(pServerTableCount);
	}

	return true;
}

//ɾ������
bool CGlobalInfoManager::DeleteChatItem(WORD wChatID)
{
	//Ѱ�ҹ㳡
	CGlobalChatItem * pGlobalChatItem=NULL;
	if (m_MapChatID.Lookup(wChatID,pGlobalChatItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//�ͷŹ㳡
	m_MapChatID.RemoveKey(wChatID);
	FreeGlobalChatItem(pGlobalChatItem);

	return true;
}

//��������
bool CGlobalInfoManager::ActiveChatItem(WORD wBindIndex, tagChatServer & ChatServer)
{
	//Ѱ�ҷ���
	CGlobalChatItem * pGlobalChatItem=NULL;
	if (m_MapChatID.Lookup(ChatServer.wChatID,pGlobalChatItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	pGlobalChatItem=CreateGlobalChatItem();
	if (pGlobalChatItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//���÷���
	pGlobalChatItem->m_wIndex=wBindIndex;
	pGlobalChatItem->m_ChatServer=ChatServer;

	//��������
	m_MapChatID[ChatServer.wChatID]=pGlobalChatItem;

	return true;
}

//Ѱ�ҹ㳡
CGlobalPlazaItem * CGlobalInfoManager::SearchPlazaItem(WORD wPlazaID)
{
	//Ѱ�ҷ���
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//Ѱ�ҷ���
CGlobalServerItem * CGlobalInfoManager::SearchServerItem(WORD wServerID)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	m_MapServerID.Lookup(wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//Ѱ������
CGlobalChatItem * CGlobalInfoManager::SearchChatItem(WORD wChatID)
{
	//Ѱ�ҷ���
	CGlobalChatItem * pGlobalChatItem=NULL;
	m_MapChatID.Lookup(wChatID,pGlobalChatItem);

	return pGlobalChatItem;
}

//Ѱ���û�
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByUserID(DWORD dwUserID)
{
	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//Ѱ���û�
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByGameID(DWORD dwGameID)
{
	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapGameID.Lookup(dwGameID,pGlobalUserItem);

	return pGlobalUserItem;
}

//Ѱ���û�
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByNickName(LPCTSTR pszNickName)
{
	//Ѱ���û�
	VOID * pGlobalUserItem=NULL;
	m_MapNickName.Lookup(pszNickName,pGlobalUserItem);

	return (CGlobalUserItem *)pGlobalUserItem;
}

//ö���û�
CGlobalUserItem * CGlobalInfoManager::EnumUserItem(POSITION & Position)
{
	//��������
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapUserID.GetStartPosition();
	if (Position!=NULL)	m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//ö�ٹ㳡
CGlobalPlazaItem * CGlobalInfoManager::EnumPlazaItem(POSITION & Position)
{
	//��������
	WORD wPlazaID=0L;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapPlazaID.GetStartPosition();
	if (Position!=NULL)	m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//ö�ٷ���
CGlobalServerItem * CGlobalInfoManager::EnumServerItem(POSITION & Position)
{
	//��������
	WORD wServerID=0L;
	CGlobalServerItem * pGlobalServerItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapServerID.GetStartPosition();
	if (Position!=NULL)	m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//ö������
CGlobalChatItem * CGlobalInfoManager::EnumChatItem(POSITION & Position)
{
	//��������
	WORD wChatID=0L;
	CGlobalChatItem * pGlobalChatItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapChatID.GetStartPosition();
	if (Position!=NULL)	m_MapChatID.GetNextAssoc(Position,wChatID,pGlobalChatItem);

	return pGlobalChatItem;
}

//�����û�
CGlobalUserItem * CGlobalInfoManager::CreateGlobalUserItem()
{
	//ʹ�ô洢
	if (m_pGlobalUserItem!=NULL)
	{
		CGlobalUserItem * pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		pGlobalUserItem->m_pNextUserItemPtr=NULL;
		return pGlobalUserItem;
	}

	//��������
	try 
	{ 
		CGlobalUserItem * pGlobalUserItem=new CGlobalUserItem;
		return pGlobalUserItem;
	}
	catch (...) {}

	return NULL;
}

//�����㳡
CGlobalPlazaItem * CGlobalInfoManager::CreateGlobalPlazaItem()
{
	//ʹ�ô洢
	if (m_pGlobalPlazaItem!=NULL)
	{
		CGlobalPlazaItem * pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		pGlobalPlazaItem->m_pNextPlazaItemPtr=NULL;
		return pGlobalPlazaItem;
	}

	//��������
	try 
	{ 
		CGlobalPlazaItem * pGlobalPlazaItem=new CGlobalPlazaItem;
		return pGlobalPlazaItem;
	}
	catch (...) {}

	return NULL;
}

//��������
CGlobalServerItem * CGlobalInfoManager::CreateGlobalServerItem()
{
	//ʹ�ô洢
	if (m_pGlobalServerItem!=NULL)
	{
		CGlobalServerItem * pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		pGlobalServerItem->m_pNextServerItemPtr=NULL;
		return pGlobalServerItem;
	}

	//��������
	try 
	{ 
		CGlobalServerItem * pGlobalServerItem=new CGlobalServerItem;
		return pGlobalServerItem;
	}
	catch (...) {}

	return NULL;
}

//��������
CGlobalChatItem * CGlobalInfoManager::CreateGlobalChatItem()
{
	//ʹ�ô洢
	if (m_pGlobalChatItem!=NULL)
	{
		CGlobalChatItem * pGlobalChatItem=m_pGlobalChatItem;
		m_pGlobalChatItem=m_pGlobalChatItem->m_pNextChatServerPtr;
		pGlobalChatItem->m_pNextChatServerPtr=NULL;
		return pGlobalChatItem;
	}

	//��������
	try 
	{ 
		CGlobalChatItem * pGlobalChatItem=new CGlobalChatItem;
		return pGlobalChatItem;
	}
	catch (...) {}

	return NULL;
}

//�ͷ��û�
bool CGlobalInfoManager::FreeGlobalUserItem(CGlobalUserItem * pGlobalUserItem)
{
	//Ч�����
	ASSERT(pGlobalUserItem!=NULL);
	if (pGlobalUserItem==NULL) return false;

	//���ñ���
	pGlobalUserItem->m_dwUserID=0L;
	pGlobalUserItem->m_dwGameID=0L;
	pGlobalUserItem->m_szNickName[0]=0;
	ZeroMemory(&pGlobalUserItem->m_UserInfo, sizeof(tagUserInfo));
	pGlobalUserItem->m_GlobalServerItemArray.RemoveAll();

	//����洢
	pGlobalUserItem->m_pNextUserItemPtr=m_pGlobalUserItem;
	m_pGlobalUserItem=pGlobalUserItem;

	return true;
}

//�ͷŹ㳡
bool CGlobalInfoManager::FreeGlobalPlazaItem(CGlobalPlazaItem * pGlobalPlazaItem)
{
	//Ч�����
	ASSERT(pGlobalPlazaItem!=NULL);
	if (pGlobalPlazaItem==NULL) return false;

	//���ñ���
	pGlobalPlazaItem->m_wIndex=0;
	ZeroMemory(&pGlobalPlazaItem->m_GamePlaza,sizeof(pGlobalPlazaItem->m_GamePlaza));

	//����洢
	pGlobalPlazaItem->m_pNextPlazaItemPtr=m_pGlobalPlazaItem;
	m_pGlobalPlazaItem=pGlobalPlazaItem;

	return true;
}

//�ͷŷ���
bool CGlobalInfoManager::FreeGlobalServerItem(CGlobalServerItem * pGlobalServerItem)
{
	//Ч�����
	ASSERT(pGlobalServerItem!=NULL);
	if (pGlobalServerItem==NULL) return false;

	//��������
	pGlobalServerItem->m_MapUserID.RemoveAll();

	//���ñ���
	pGlobalServerItem->m_wIndex=0;
	ZeroMemory(&pGlobalServerItem->m_GameServer,sizeof(pGlobalServerItem->m_GameServer));

	//����洢
	pGlobalServerItem->m_pNextServerItemPtr=m_pGlobalServerItem;
	m_pGlobalServerItem=pGlobalServerItem;

	return true;
}

//�ͷ�����
bool CGlobalInfoManager::FreeGlobalChatItem(CGlobalChatItem * pGlobalChatItem)
{
	//Ч�����
	ASSERT(pGlobalChatItem!=NULL);
	if (pGlobalChatItem==NULL) return false;

	//���ñ���
	pGlobalChatItem->m_wIndex=0;
	ZeroMemory(&pGlobalChatItem->m_ChatServer,sizeof(pGlobalChatItem->m_ChatServer));

	//����洢
	pGlobalChatItem->m_pNextChatServerPtr=m_pGlobalChatItem;
	m_pGlobalChatItem=pGlobalChatItem;

	return true;
}

//���һ����Ϸ��ഴ��˽�˷������Ŀ
bool CGlobalInfoManager::AddPersonalMaxCreate(CMD_CS_S_RegisterPersonal RegisterPersonal)
{
	int iCount = m_vecPersonalRoomMaxCreate.size();

	//�������������������룬 ����������޸�
	bool bIsExist = false;
	for (int i = 0; i < iCount; i++)
	{
		if (m_vecPersonalRoomMaxCreate[i].dwKindID == RegisterPersonal.dwKindID)
		{
			m_vecPersonalRoomMaxCreate[i].dwMaxCreate = RegisterPersonal.dwMaxCreate;
			bIsExist = true;
			break;
		}
	}

	if (!bIsExist)
	{
		m_vecPersonalRoomMaxCreate.push_back(RegisterPersonal);
	}

	return true;
}

//�Ƿ�����ٴ�������
bool CGlobalInfoManager::CanCreatePersonalRoom(DWORD dwKindID,   DWORD dwUserID)
{
	//��ȡ���Դ�������������Ŀ
	int iCount = m_vecPersonalRoomMaxCreate.size();
	int iMaxCreateCount = 0;
	for (int i = 0; i < iCount; i++)
	{
		if (m_vecPersonalRoomMaxCreate[i].dwKindID == dwKindID)
		{
			iMaxCreateCount = m_vecPersonalRoomMaxCreate[i].dwMaxCreate;
			break;
		}
	}

	if (iMaxCreateCount == 0)
	{
		iMaxCreateCount = MAX_CREATE_COUNT;
	}

	//���Լս������Ŀ
	int iHaveCreate = GetHostCreatePersonalRoomCount(dwUserID);
	if (iHaveCreate < iMaxCreateCount)
	{
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
