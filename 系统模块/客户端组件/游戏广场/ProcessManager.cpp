#include "StdAfx.h"
#include "GlobalUnits.h"
#include "ProcessManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_PROCESS_DETECT			100									//���̼��

//ʱ���ʶ
#ifndef _DEBUG
#define TIME_DETECT_NORMAL			10000L								//������
#define TIME_DETECT_CREATE			90000L								//�������
#else
#define TIME_DETECT_NORMAL			5000L								//������
#define TIME_DETECT_CREATE			90000L								//�������
#endif 


//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CProcessManager, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CProcessManager::CProcessManager()
{
	//ͨѶ����
	m_hProcessControl=NULL;

	//�ں˱���
	m_pIProcessManagerSink=NULL;
	ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

	return;
}

//��������
CProcessManager::~CProcessManager()
{
	//�رս���
	CloseGameProcess(false);

	return;
}

//�ӿڲ�ѯ
VOID * CProcessManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IProcessManager,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessControlSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IProcessManager,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CProcessManager::OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)
{
	//Ч��״̬
	ASSERT(m_pIProcessManagerSink!=NULL);
	if (m_pIProcessManagerSink==NULL) return false;

	//����Ч��
	//ASSERT((m_hProcessControl==NULL)||(m_hProcessControl==hWndSend));
	if ((m_hProcessControl!=NULL)&&(m_hProcessControl!=hWndSend)) return false;

	//׼����Ϣ
	if (wMainCmdID==IPC_CMD_GF_CONTROL)
	{
		switch (wSubCmdID)
		{
		case IPC_SUB_GF_CLIENT_READY:	//׼������
			{
				//Ч��״̬
				ASSERT(m_hProcessControl==NULL);
				if (m_hProcessControl!=NULL) return false;

				//�¼�����
				m_hProcessControl=hWndSend;
				m_pIProcessManagerSink->OnGameProcessCreate(false);

				//�������
				SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);

				//���ؽ�����ɣ���������
				SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW)));

				//�������
				SetTimer(IDI_PROCESS_DETECT,TIME_DETECT_NORMAL,NULL);

				return true;
			}
		case IPC_SUB_GF_CLIENT_CLOSE:	//���̹ر�
			{
				//���ñ���
				m_hProcessControl=NULL;

				//�������
				DetachGameProcess(true);

				return true;
			}
		}
	}

	//��Ϣ����
	m_pIProcessManagerSink->OnProcessManagerData(wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//�رս���
bool CProcessManager::CloseGameProcess(bool bPerformSink)
{
	//ɾ��ʱ��
	if (m_hWnd!=NULL) KillTimer(IDI_PROCESS_DETECT);

	//�رս���
	if (m_ProcessInfo.hProcess!=NULL)
	{
		//�����ж�
		if ((m_hProcessControl!=NULL)&&(IsWindow(m_hProcessControl)!=FALSE))
		{
			//������Ϣ
			SendProcessCommand(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLOSE_PROCESS,NULL,0);

			//��������
			DWORD dwResult=WaitForSingleObject(m_ProcessInfo.hProcess,1000L);
			if (dwResult==WAIT_TIMEOUT) TerminateProcess(m_ProcessInfo.hProcess,0);
		}
		else
		{
			//��������
			DWORD dwResult=WaitForSingleObject(m_ProcessInfo.hProcess,0L);
			if (dwResult==WAIT_TIMEOUT) TerminateProcess(m_ProcessInfo.hProcess,0);
		}

		//�˳�����
		DWORD dwExitCode=0L;
		GetExitCodeProcess(m_ProcessInfo.hProcess,&dwExitCode);

		//�رվ��
		if (m_ProcessInfo.hThread!=NULL) CloseHandle(m_ProcessInfo.hThread);
		if (m_ProcessInfo.hProcess!=NULL) CloseHandle(m_ProcessInfo.hProcess);

		//���ñ���
		m_hProcessControl=NULL;
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

		//�¼�֪ͨ
		ASSERT(m_pIProcessManagerSink!=NULL);
		if (bPerformSink==true) m_pIProcessManagerSink->OnGameProcessClose(dwExitCode);
	}

	return true;
}

//�������
bool CProcessManager::DetachGameProcess(bool bPerformSink)
{
	//ɾ��ʱ��
	if (m_hWnd!=NULL) KillTimer(IDI_PROCESS_DETECT);

	//����֪ͨ
	if (m_ProcessInfo.hProcess!=NULL)
	{
		//�رվ��
		if (m_ProcessInfo.hThread!=NULL) CloseHandle(m_ProcessInfo.hThread);
		if (m_ProcessInfo.hProcess!=NULL) CloseHandle(m_ProcessInfo.hProcess);

		//���ñ���
		m_hProcessControl=NULL;
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

		//�¼�֪ͨ
		ASSERT(m_pIProcessManagerSink!=NULL);
		if (bPerformSink==true) m_pIProcessManagerSink->OnGameProcessClose(0L);
	}

	return true;
}

//��������
bool CProcessManager::CreateGameProcess(LPCTSTR pszProcessName)
{
	//Ч��״̬
	ASSERT(m_pIProcessManagerSink!=NULL);
	if (m_pIProcessManagerSink==NULL) return false;

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);
	}

	//�������
	if (m_ProcessControlModule.GetInterface()==NULL)
	{
		//�������
		if (m_ProcessControlModule.CreateInstance()==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ýӿ�
		if (m_ProcessControlModule->SetProcessControlSink(QUERY_ME_INTERFACE(IUnknownEx))==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//�������
		if (m_ProcessControlModule->InitializeLink()==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//�����ж�
	if ((m_ProcessInfo.hProcess!=NULL)&&(WaitForSingleObject(m_ProcessInfo.hProcess,0)==WAIT_TIMEOUT))
	{
		//�����ж�
		if ((m_hProcessControl!=NULL)&&(IsWindow(m_hProcessControl)!=FALSE))
		{
			//����֪ͨ
			m_pIProcessManagerSink->OnGameProcessCreate(true);

			//�������
			SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
		}

		return true;
	}

	//��������
	HWND hProcessTrade=m_ProcessControlModule->GetProcessControlWnd();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�������
	TCHAR szCommomLine[MAX_PATH];
	_sntprintf(szCommomLine,CountArray(szCommomLine),TEXT("%s /Transmittal:%I64d"),pszProcessName,(LONGLONG)(hProcessTrade));

	//��������
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//��������
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess=CreateProcess(NULL,szCommomLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_ProcessInfo);

	//������
	if (bSuccess==FALSE)
	{
		ASSERT(FALSE);
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));
		return false;
	}

	//�������
	SetTimer(IDI_PROCESS_DETECT,TIME_DETECT_CREATE,NULL);

	//���ؽ��̣����õȴ�
	SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_APPSTARTING)));

	return true;
}

//���ýӿ�
bool CProcessManager::SetProcessManagerSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessManagerSink)!=NULL);
		m_pIProcessManagerSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessManagerSink);

		//�ɹ��ж�
		if (m_pIProcessManagerSink==NULL) return false;
	}
	else m_pIProcessManagerSink=NULL;

	return true;
}

//��������
bool CProcessManager::SendProcessData(WORD wMainCmdID, WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendData(m_hProcessControl,wMainCmdID,wSubCmdID);
	}

	return true;
}

//��������
bool CProcessManager::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendData(m_hProcessControl,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//ִ������
bool CProcessManager::SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//ִ������
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendCommand(m_hProcessControl,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//������Ϣ
bool CProcessManager::SendServerInfo(IClientUserItem * pIMyselfUserItem, DWORD dwUserRight, DWORD dwMasterRight, DWORD dwServerRule, WORD wServerType, tagGameServer & GameServer
										,WORD wAVServerPort,DWORD dwAVServerAddr)
{
	//������Ϣ
	IPC_GF_ServerInfo ServerInfoData;
	ZeroMemory(&ServerInfoData,sizeof(ServerInfoData));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�û���Ϣ
	ServerInfoData.dwUserRight=dwUserRight;
	ServerInfoData.dwMasterRight=dwMasterRight;
	ServerInfoData.dwUserID=pIMyselfUserItem->GetUserID();
	ServerInfoData.wTableID=pIMyselfUserItem->GetTableID();
	ServerInfoData.wChairID=pIMyselfUserItem->GetChairID();

	//������Ϣ
	ServerInfoData.wServerType=wServerType;
	ServerInfoData.dwServerRule=dwServerRule;
	ServerInfoData.wKindID=GameServer.wKindID;
	ServerInfoData.wServerID=GameServer.wServerID;
	lstrcpyn(ServerInfoData.szServerName,GameServer.szServerName,CountArray(ServerInfoData.szServerName));

	//��Ƶ����
	ServerInfoData.wAVServerPort=wAVServerPort;
	ServerInfoData.dwAVServerAddr=dwAVServerAddr;

	//��������
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_SERVER_INFO,&ServerInfoData,sizeof(ServerInfoData));

	return true;
}

//������Ϣ
bool CProcessManager::SendMatchInfo(tagGameMatch& GameMatch)
{
	IPC_GF_MatchInfo MatchInfo;
	ZeroMemory(&MatchInfo,sizeof(MatchInfo));
	CopyMemory(&(MatchInfo.GameMatch),&GameMatch,sizeof(GameMatch));

	//��������
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_MATCH_INFO,&MatchInfo,sizeof(MatchInfo));

	return true;
}

//�û���Ϣ
bool CProcessManager::SendUserItem(IClientUserItem * pIClientUserItem)
{
	//���ͻ���
	BYTE cbBuffer[IPC_PACKET];
	ZeroMemory(cbBuffer,sizeof(cbBuffer));

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//��������
	IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)cbBuffer;
	tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(IPC_GF_UserInfo),sizeof(cbBuffer)-sizeof(IPC_GF_UserInfo));

	//�û���ϵ
	pIPCUserInfo->cbCompanion=pIClientUserItem->GetUserCompanion();

	//�û�����
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;
	pUserInfoHead->dwAgentID=pUserInfo->dwAgentID;
	//�û�����
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//�û�״̬
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//�û�����
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lGrade=pUserInfo->lGrade;
	pUserInfoHead->lIngot=pUserInfo->lIngot;
	pUserInfoHead->lInsure=pUserInfo->lInsure;
	pUserInfoHead->dBeans=pUserInfo->dBeans;
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;	
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;
	pUserInfoHead->lLoveLiness=pUserInfo->lLoveLiness;
	pUserInfoHead->lIntegralCount=pUserInfo->lIntegralCount;

	//��չ����
	pUserInfoHead->bIsAndroid=pIClientUserItem->IsAndroidUser();

	//������Ϣ
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);
	SendPacket.AddPacket(pIClientUserItem->GetUserNoteInfo(),DTP_GR_USER_NOTE);
	SendPacket.AddPacket(pUserInfo->szQQ,DTP_GR_QQ);
	SendPacket.AddPacket(pUserInfo->szMobilePhone,DTP_GR_MOBILE_PHONE);

	//�Զ�ͷ��
	if ((pUserInfo->dwCustomID!=0L)&&(pCustomFaceInfo->dwDataSize!=0L))
	{
		SendPacket.AddPacket(pCustomFaceInfo,sizeof(tagCustomFaceInfo),DTP_GR_CUSTOM_FACE);
	}

	//��������
	WORD wSendSize=sizeof(IPC_GF_UserInfo)+SendPacket.GetDataSize();
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_ENTER,cbBuffer,wSendSize);

	return true;
}

//�û�����
bool CProcessManager::SendUserScore(DWORD dwUserID, tagUserScore & UserScore)
{
	//�û�״̬
	IPC_GF_UserScore UserScoreData;
	ZeroMemory(&UserScoreData,sizeof(UserScoreData));

	//������Ϣ
	UserScoreData.dwUserID=dwUserID;
	UserScoreData.UserScore=UserScore;

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_SCORE,&UserScoreData,sizeof(UserScoreData));

	return true;
}

//�û�״̬
bool CProcessManager::SendUserStatus(DWORD dwUserID, tagUserStatus & UserStatus)
{
	//�û�״̬
	IPC_GF_UserStatus UserStatusData;
	ZeroMemory(&UserStatusData,sizeof(UserStatusData));

	//������Ϣ
	UserStatusData.dwUserID=dwUserID;
	UserStatusData.UserStatus=UserStatus;

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_STATUS,&UserStatusData,sizeof(UserStatusData));

	return true;
}

//�û�����
bool CProcessManager::SendUserAttrib(DWORD dwUserID, tagUserAttrib & UserAttrib)
{
	//�û�״̬
	IPC_GF_UserAttrib UserAttribData;
	ZeroMemory(&UserAttribData,sizeof(UserAttribData));

	//������Ϣ
	UserAttribData.dwUserID=dwUserID;
	UserAttribData.UserAttrib=UserAttrib;

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_ATTRIB,&UserAttribData,sizeof(UserAttribData));

	return true;
}

//�û�ͷ��
bool CProcessManager::SendCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo * pCustomFaceInfo)
{
	//��������
	IPC_GF_CustomFace CustomFaceData;
	ZeroMemory(&CustomFaceData,sizeof(CustomFaceData));

	//������Ϣ
	CustomFaceData.dwUserID=dwUserID;
	CustomFaceData.dwCustomID=dwCustomID;
	CopyMemory(&CustomFaceData.CustomFaceInfo,pCustomFaceInfo,sizeof(CustomFaceData.CustomFaceInfo));

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_CUSTOM_FACE,&CustomFaceData,sizeof(CustomFaceData));

	return true;
}

//�ȼ���Ϣ
bool CProcessManager::SendLevelInfo(tagLevelItem LevelItem[], BYTE cbLevelCount)
{
	//��������
	IPC_GF_LevelInfo LevelInfoData;
	ZeroMemory(&LevelInfoData,sizeof(LevelInfoData));

	//������Ϣ
	LevelInfoData.cbItemCount=__min(cbLevelCount,CountArray(LevelInfoData.LevelItem));
	CopyMemory(LevelInfoData.LevelItem,LevelItem,sizeof(tagLevelItem)*LevelInfoData.cbItemCount);

	//��������
	WORD wHeadSize=sizeof(LevelInfoData)-sizeof(LevelInfoData.LevelItem);
	WORD wDataSize=sizeof(LevelInfoData.LevelItem[0])*LevelInfoData.cbItemCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LEVEL_INFO,&LevelInfoData,wHeadSize+wDataSize);

	return true;
}

//�б���Ϣ
bool CProcessManager::SendColumnInfo(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//��������
	IPC_GF_ColumnInfo ColumnInfoData;
	ZeroMemory(&ColumnInfoData,sizeof(ColumnInfoData));

	//������Ϣ
	ColumnInfoData.cbColumnCount=__min(cbColumnCount,CountArray(ColumnInfoData.ColumnItem));
	CopyMemory(ColumnInfoData.ColumnItem,ColumnItem,sizeof(tagColumnItem)*ColumnInfoData.cbColumnCount);

	//��������
	WORD wHeadSize=sizeof(ColumnInfoData)-sizeof(ColumnInfoData.ColumnItem);
	WORD wDataSize=sizeof(ColumnInfoData.ColumnItem[0])*ColumnInfoData.cbColumnCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_COLUMN_INFO,&ColumnInfoData,wHeadSize+wDataSize);

	return true;
}

//������Ϣ
bool CProcessManager::SendGamePropertyTypeInfo()
{

	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[IPC_PACKET];
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	POSITION Position=NULL;
	CGamePropertyTypeItem * pGamePropertyTypeItem=NULL;
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();
	DWORD dwCount = pGamePropertyListData->GetGamePropertyTypeCount();

	//ö������
	for (DWORD i=0;i<dwCount;i++)
	{
		//��������
		if ((wSendSize+sizeof(tagPropertyTypeItem))>sizeof(cbDataBuffer))
		{
			SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LIST_PROPERTY_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePropertyTypeItem=pGamePropertyListData->EmunGamePropertyTypeItem(Position);
		if (pGamePropertyTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGamePropertyTypeItem->m_PropertyTypeItem,sizeof(tagPropertyTypeItem));
		wSendSize+=sizeof(tagPropertyTypeItem);
	}

	//����ʣ��
	if (wSendSize>0) SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LIST_PROPERTY_TYPE,cbDataBuffer,wSendSize);


	return true;
}

//���߹�ϵ
bool CProcessManager::SendGamePropertyRelatInfo()
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[IPC_PACKET];
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	POSITION Position=NULL;
	CGamePropertyRelatItem * pGamePropertyRelatItem=NULL;
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();
	DWORD dwCount = pGamePropertyListData->GetGamePropertyRelatCount();

	//ö������
	for (DWORD i=0;i<dwCount;i++)
	{
		//��������
		if ((wSendSize+sizeof(tagPropertyRelatItem))>sizeof(cbDataBuffer))
		{
			SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LIST_PROPERTY_RELAT,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePropertyRelatItem=pGamePropertyListData->EmunGamePropertyRelatItem(Position);
		if (pGamePropertyRelatItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGamePropertyRelatItem->m_PropertyRelatItem,sizeof(tagPropertyRelatItem));
		wSendSize+=sizeof(tagPropertyRelatItem);
	}

	//����ʣ��
	if (wSendSize>0) SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LIST_PROPERTY_RELAT,cbDataBuffer,wSendSize);


	return true;
}

//���͵���
bool CProcessManager::SendGamePropertyInfo()
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[IPC_PACKET];
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	POSITION Position=NULL;
	CGamePropertyItem * pGamePropertyItem=NULL;
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();
	DWORD dwCount = pGamePropertyListData->GetGamePropertyCount();

	//ö������
	for (DWORD i=0;i<dwCount;i++)
	{
		//��������
		if ((wSendSize+sizeof(tagPropertyItem))>sizeof(cbDataBuffer))
		{
			SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LIST_PROPERTY,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePropertyItem=pGamePropertyListData->EmunGamePropertyItem(Position);
		if (pGamePropertyItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGamePropertyItem->m_PropertyItem,sizeof(tagPropertyItem));
		wSendSize+=sizeof(tagPropertyItem);
	}

	//����ʣ��
	if (wSendSize>0) SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LIST_PROPERTY,cbDataBuffer,wSendSize);


	return true;
}


//���͵���
bool CProcessManager::SendGamePropertyTrumpet(DWORD dwTrumpetCount,DWORD dwTyphonCount)
{
	IPC_GF_Trumpet_Number UserTrumpet;
	UserTrumpet.dwTrumpetCount = dwTrumpetCount;
	UserTrumpet.dwTyphonCount =  dwTyphonCount;

	//��������
	SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_USER_TRUMPET,&UserTrumpet,sizeof(UserTrumpet));


	return true;
}

//�ȼ�����
bool CProcessManager::SendGrowLevelConfig(tagGrowLevelConfig GrowLevelItem[],WORD wLevelCount)
{
	//��������
	IPC_GF_GrowLevelConfig GrowLevelConfig;
	ZeroMemory(&GrowLevelConfig,sizeof(GrowLevelConfig));

	//������Ϣ
	GrowLevelConfig.wLevelCount=__min(wLevelCount,CountArray(GrowLevelConfig.GrowLevelItem));
	CopyMemory(GrowLevelConfig.GrowLevelItem,GrowLevelItem,wLevelCount*sizeof(GrowLevelConfig.GrowLevelItem[0]));	

	//��������
	WORD wHeadSize=sizeof(GrowLevelConfig)-sizeof(GrowLevelConfig.GrowLevelItem);
	WORD wDataSize=sizeof(GrowLevelConfig.GrowLevelItem[0])*GrowLevelConfig.wLevelCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_GROWLEVEL_CONFIG,&GrowLevelConfig,wHeadSize+wDataSize);

	return true;
}

//��ѯ����
bool CProcessManager::IsProcessReady()
{
	if( m_ProcessInfo.hProcess == NULL || m_hProcessControl == NULL ||
		IsWindow(m_hProcessControl) == FALSE )
		return false;

	return true;
}

//ʱ����Ϣ
VOID CProcessManager::OnTimer(UINT_PTR nIDEvent)
{
	//���̼��
	if (nIDEvent==IDI_PROCESS_DETECT)
	{
		//״̬Ч��
		ASSERT(m_ProcessInfo.hProcess!=NULL);
		if (m_ProcessInfo.hProcess==NULL) return;

		//�����ж�
		if ((m_hProcessControl==NULL)||(IsWindow(m_hProcessControl)==FALSE))
		{
			//��ֹ����
			DetachGameProcess(false);

			//�¼�֪ͨ
			ASSERT(m_pIProcessManagerSink!=NULL);
			if (m_pIProcessManagerSink!=NULL) m_pIProcessManagerSink->OnGameProcessError();
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
