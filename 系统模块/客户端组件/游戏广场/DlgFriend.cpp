#include "Stdafx.h"
#include "Resource.h"
#include "DlgFriend.h"
#include "PlatformFrame.h"
#include "DlgInformation.h"

//时间定义
#define IDI_SEARCH_TIME_OUT			100									//超时标识
#define TIME_SEARCH_TIME_OUT		15000L								//超时时间

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");
const TCHAR* const szButtonJionGameControlName = TEXT("ButtonJionGame");
const TCHAR* const szButtonWatchGameControlName = TEXT("ButtonWatchGame");
const TCHAR* const szButtonChatControlName = TEXT("ButtonChat");
const TCHAR* const szButtonMenuControlName = TEXT("ButtonMenu");

//好友列表
const TCHAR* const szFriendListContorlName = TEXT("FriendList");
//头像
const TCHAR* const szContainerFriendAvatarContorlName = TEXT("FriendAvatar");

//标签控件
const TCHAR* const szLabelFriendLevelContorlName = TEXT("LabelFriendLevel");
const TCHAR* const szLabelFriendNickNameContorlName = TEXT("LabelFriendNickName");
const TCHAR* const szLabelFriendInGameContorlName = TEXT("LabelFriendInGame");
const TCHAR* const szLabelFriendInRoomContorlName = TEXT("LabelFriendInRoom");
const TCHAR* const szLabelFriendInTableContorlName = TEXT("LabelFriendInTable");
const TCHAR* const szLabelFriendGameIDContorlName = TEXT("LabelFriendGameID");
const TCHAR* const szLabelFriendOnlineCountContorlName = TEXT("LabelFriendOnlineCount");
const TCHAR* const szLabelAllFriendCountContorlName = TEXT("LabelAllFriendCount");
const TCHAR* const szLableFriendAllContorlName = TEXT("LableFriendAll");
const TCHAR* const szLableFriendOnlineContorlName = TEXT("LableFriendOnline");
//容器控件
const TCHAR* const szVerticalLayoutRightViewContorlName = TEXT("VerRightView");

//菜单控件
const TCHAR* const szMenuFriendAllContorlName = TEXT("MenuFriendAll");
const TCHAR* const szMenuMenuFriendOnlineContorlName = TEXT("MenuFriendOnline");


const DWORD dwVipNickNameTextColor = 0xFFFF0000;
const DWORD dwOnLineNickNameTextColor = 0xFFFFCC99;
const DWORD dwOffLineNickNameTextColor = 0xFF999999;
const DWORD dwOnlineTextColor = 0xFF99CC66;
const DWORD dwOfflineTextColor = 0xFF999999;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgFriend::CDlgFriend() : CFGuiDialog(IDD_DLG_POPUP)
,m_pListFriend(NULL)
,m_pButtonJionGame(NULL)
,m_pButtonWatchGame(NULL)
,m_pButtonChat(NULL)
,m_pFriendAvatar(NULL)
,m_pLabelFriendLevel(NULL)
,m_pLabelFriendNickName(NULL)
,m_pLabelFriendInGame(NULL)
,m_pLabelFriendInRoom(NULL)
,m_pLabelFriendInTable(NULL)
,m_pLabelFriendGameID(NULL)
{
	::ZeroMemory(&m_CurSelectUserInfo, sizeof(m_CurSelectUserInfo));
	//设置任务
	m_MissionManager.InsertMissionItem(this);
}

//析构函数
CDlgFriend::~CDlgFriend()
{
}

void CDlgFriend::InitControlUI()
{
	m_pListFriend = (CListExUI*)GetControlByName(szFriendListContorlName);
	if(m_pListFriend != NULL) m_pListFriend->RemoveAll();

	m_pButtonJionGame = (CButtonUI*)GetControlByName(szButtonJionGameControlName);
	m_pButtonWatchGame = (CButtonUI*)GetControlByName(szButtonWatchGameControlName);
	m_pButtonChat = (CButtonUI*)GetControlByName(szButtonChatControlName);

	m_pFriendAvatar = (CControlUI*)GetControlByName(szContainerFriendAvatarContorlName);
	m_pLabelFriendLevel = (CLabelUI*)GetControlByName(szLabelFriendLevelContorlName);
	m_pLabelFriendNickName = (CLabelUI*)GetControlByName(szLabelFriendNickNameContorlName);
	m_pLabelFriendInGame = (CLabelUI*)GetControlByName(szLabelFriendInGameContorlName);
	m_pLabelFriendInRoom = (CLabelUI*)GetControlByName(szLabelFriendInRoomContorlName);
	m_pLabelFriendInTable = (CLabelUI*)GetControlByName(szLabelFriendInTableContorlName);
	m_pLabelFriendGameID = (CLabelUI*)GetControlByName(szLabelFriendGameIDContorlName);

	CContainerUI* pContainerUI = (CContainerUI*)GetControlByName(szVerticalLayoutRightViewContorlName);
	if(pContainerUI) pContainerUI->SetVisible(false);

	LoadFriendUserInfo();
	
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	bool bEnable = pPlatformFrame->IsEnteredServerItem();
	if(m_pButtonJionGame) m_pButtonJionGame->SetEnabled( bEnable );
	if(m_pButtonWatchGame) m_pButtonWatchGame->SetEnabled( bEnable );
	if(m_pButtonChat) m_pButtonChat->SetEnabled( bEnable );
	
	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==true) 
	{
		//设置时间
		::SetTimer(GetSafeHwnd(), IDI_SEARCH_TIME_OUT,TIME_SEARCH_TIME_OUT,NULL);
	}
}

//确定函数
VOID CDlgFriend::OnOK()
{

}

//取消消息
VOID CDlgFriend::OnCancel()
{
	EndDialog(IDCANCEL);
	//Close();
}

//消息提醒
void CDlgFriend::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if ( msg.sType == TEXT("click") )
	{
		if( pControlUI->GetName() == szButtonCloseControlName )
		{
			return OnCancel();
		}
		else if( pControlUI->GetName() == szButtonMenuControlName )
		{
			CMenuWnd* pMenu = new CMenuWnd();
			int x = pControlUI->GetX();
			int y = pControlUI->GetY();
			int w =  pControlUI->GetWidth();
			CPoint point;
			point.x = x + w + 2;
			point.y = y - 5;
			::ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("DlgFriendMenu.xml"),point, &m_PaintManager, NULL);
			
		}
		else if( pControlUI->GetName() == szButtonChatControlName ) //发起私聊
		{
			IClientUserItem* pClientUserItem = NULL;
			if( (pClientUserItem = CanChat(m_CurSelectUserInfo.dwUserID)) != NULL )
			{
				Chat(pClientUserItem);
			}
		}
		else if( pControlUI->GetName() == szButtonJionGameControlName ) //加入游戏
		{
			WORD wTableID = 0;
			WORD wChairID = 0;
			if( CanJoinGame(true, wTableID, wChairID) )
			{
				JoinGame(wTableID, wChairID);
			}
		}
		else if( pControlUI->GetName() == szButtonWatchGameControlName ) //旁观游戏
		{
			WORD wTableID = 0;
			WORD wChairID = 0;
			if( CanLookOnGame(true, wTableID, wChairID) )
			{
				LookOnGame(wTableID, wChairID);
			}
		}
		
	}
	else if( msg.sType == TEXT("itemselect") )
	{
		if(pControlUI->GetName() == szFriendListContorlName)
		{
			int nIndex = (int)msg.wParam;
			CNodeElementUI* pNodeElementUI = static_cast<CNodeElementUI*>(m_pListFriend->GetItemAt(nIndex));
			tagCompanionInfo* pCompanionInfo = (tagCompanionInfo*)pNodeElementUI->GetTag();
			int nFriendCount = (int)(m_vecFriendInfo.size());
			for(int i = 0; i < nFriendCount; i++)
			{
				if(m_vecFriendInfo[i].dwGameID == pCompanionInfo->dwGameID)
				{
					memcpy(&m_CurSelectUserInfo, &m_vecFriendInfo[i], sizeof(m_CurSelectUserInfo));
					UpdateFriendInfo(m_vecFriendInfo[i]);
					return;
				}
			}

			tagUserRemoteInfo tmpUserInfo;
			ZeroMemory(&tmpUserInfo, sizeof(tmpUserInfo));
			tmpUserInfo.dwUserID = pCompanionInfo->dwUserID;
			tmpUserInfo.dwGameID = pCompanionInfo->dwGameID;
			tmpUserInfo.wTableID = INVALID_CHAIR;
			tmpUserInfo.wFaceID = pCompanionInfo->cbFaceID;
			_tcscpy(tmpUserInfo.szNickName, pCompanionInfo->szNickName);
			memcpy(&m_CurSelectUserInfo, &tmpUserInfo, sizeof(m_CurSelectUserInfo));
			UpdateFriendInfo(tmpUserInfo);
		}
	}
	return;
}

LRESULT CDlgFriend::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		CStdString *strMenuName = (CStdString*)wParam;

		CStdString* strPost = new CStdString();
		strPost = strMenuName;		

		if ( *strMenuName == szMenuFriendAllContorlName ) 
		{
			CControlUI* pControl = (CControlUI*)GetControlByName(szLableFriendAllContorlName);
			if(pControl) pControl->SetVisible(true);
			 pControl = (CControlUI*)GetControlByName(szLableFriendOnlineContorlName);
			if(pControl) pControl->SetVisible(false);

			//显示全部好友
			for(int i = 0; m_pListFriend && i < m_pListFriend->GetCount(); i++)
			{
				m_pListFriend->GetItemAt(i)->SetVisible(true);
			}
		}
		else if ( *strMenuName == szMenuMenuFriendOnlineContorlName ) 
		{
			CControlUI* pControl = (CControlUI*)GetControlByName(szLableFriendAllContorlName);
			if(pControl) pControl->SetVisible(false);
			 pControl = (CControlUI*)GetControlByName(szLableFriendOnlineContorlName);
			if(pControl) pControl->SetVisible(true);

			//显示在线好友
			for(int i = 0; m_pListFriend && i < m_pListFriend->GetCount(); i++)
			{
				CNodeElementUI* pNodeElementUI = static_cast<CNodeElementUI*>(m_pListFriend->GetItemAt(i));
				tagCompanionInfo* pCompanionInfo = (tagCompanionInfo*)pNodeElementUI->GetTag();
				bool bOnLine = IsOnline(pCompanionInfo->dwGameID);
				pNodeElementUI->SetVisible(bOnLine);
			}
		}
	}
	return 1L;
}

LPCTSTR CDlgFriend::GetSkinFile()
{
	return TEXT("DlgFriend.xml"); 
}

LPCTSTR CDlgFriend::GetWindowClassName() const
{
	return TEXT("WHFriendClass");
}



//连接事件
bool CDlgFriend::OnEventMissionLink(INT nErrorCode)
{
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//删除时间
		::KillTimer(GetSafeHwnd(), IDI_SEARCH_TIME_OUT);

		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("连接服务器超时，远程查找操作失败！"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//枚举好友
	CUserInformation * pUserInformation = CUserInformation::GetInstance();
	WORD wIndex=0;
	tagCompanionInfo * pCompanionInfo=NULL;
	DWORD cBuffer[512]={0};
	CMD_GP_C_SearchAllCorrespond* pSearchCorrespond = (CMD_GP_C_SearchAllCorrespond*)cBuffer;

	int nCount = 0;
	while(pCompanionInfo=pUserInformation->EmunCompanionInfo(wIndex++))
	{
		//判断关系
		if(pCompanionInfo->cbCompanion!=CP_FRIEND) continue;
		pSearchCorrespond->dwGameID[nCount++] = pCompanionInfo->dwGameID;
	}
	//构造数据
	pSearchCorrespond->dwCount = nCount; 

	WORD wSize = 0;
	wSize =(WORD)( sizeof(CMD_GP_C_SearchAllCorrespond)+sizeof(DWORD)*(nCount-1));

	//发送数据
	m_MissionManager.SendData(MDM_GP_REMOTE_SERVICE,SUB_GP_C_SEARCH_ALLCORRESPOND, pSearchCorrespond, wSize);

	return true;
}

//关闭事件
bool CDlgFriend::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//删除时间
		::KillTimer(GetSafeHwnd(), IDI_SEARCH_TIME_OUT);

		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，远程查找操作失败！"),MB_ICONERROR,30L);
	}
	else
	{
		//删除时间
		::KillTimer(GetSafeHwnd(), IDI_SEARCH_TIME_OUT);
	}
	return true;
}

//读取事件
bool CDlgFriend::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_REMOTE_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_S_SEARCH_ALLCORRESPOND:
			{
				//变量定义
				CMD_GP_S_SearchAllCorrespond * pSearchCorrespond = (CMD_GP_S_SearchAllCorrespond *)pData;
				//ASSERT(wDataSize < sizeof(CMD_GP_S_SearchAllCorrespond));
				//失败处理
				::KillTimer(GetSafeHwnd(), IDI_SEARCH_TIME_OUT);

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);
				
				tagCompanionInfo* pCompanionInfo = NULL;
				const int nSel = m_pListFriend->GetCurSel();
				if(nSel != -1)
				{
					CNodeElementUI* pNodeElementUI = static_cast<CNodeElementUI*>(m_pListFriend->GetItemAt(nSel));
					pCompanionInfo = (tagCompanionInfo*)pNodeElementUI->GetTag();
				}
				

				//更新列表
				int nVipIndex = 0;
				int nOnlineIndex = 0;
				int nSerchCount= (int) pSearchCorrespond->dwCount;
				for (int i = 0; i < nSerchCount; i++)
				{
					m_vecFriendInfo.push_back(pSearchCorrespond->UserRemoteInfo[i]);
					
					if(pCompanionInfo && pCompanionInfo->dwUserID == pSearchCorrespond->UserRemoteInfo[i].dwUserID)
					{
						UpdateFriendInfo(pSearchCorrespond->UserRemoteInfo[i]);
					}

					CNodeElementUI* pNodeElement = UpdateFriendOnLine(pSearchCorrespond->UserRemoteInfo[i]);
					bool bVipUser = pSearchCorrespond->UserRemoteInfo[i].cbMemberOrder > 0;
					//排序先vip、在线、离线
					if(pNodeElement != NULL)
					{
						m_pListFriend->Remove(pNodeElement, false);
						if( bVipUser){
							nVipIndex++;
						}
						else{
							nOnlineIndex++;
						}
						int nIndex = bVipUser?nVipIndex:(nVipIndex+nOnlineIndex);
						m_pListFriend->AddAt(pNodeElement, nIndex-1);
					}
				}

				//在线好友数
				CLabelUI* pFriendOnLine = (CLabelUI*)GetControlByName(szLabelFriendOnlineCountContorlName);
				if(pFriendOnLine != NULL)
				{
					TCHAR szBuffer[32]={0};
					wsprintf(szBuffer, TEXT("%d"), pSearchCorrespond->dwCount);
					pFriendOnLine->SetText(szBuffer);
				}
				
				

				return true;
			}
		}
	}

	return true;
}	

void CDlgFriend::LoadFriendUserInfo()
{
	ASSERT(m_pListFriend != NULL);
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//获取对象
	ASSERT(CUserInformation::GetInstance());
	CUserInformation * pUserInformation = CUserInformation::GetInstance();

	CDialogBuilder builder;
	CNodeElementUI* pNodeElementUI =NULL;
	//枚举好友
	WORD wIndex=0;
	int nCount = 0;
	tagCompanionInfo * pCompanionInfo=NULL;
	while(pCompanionInfo=pUserInformation->EmunCompanionInfo(wIndex++))
	{
		//判断关系
		if(pCompanionInfo->cbCompanion!=CP_FRIEND) continue;
		try
		{
			nCount++;

			//创建子项
			pNodeElementUI = static_cast<CNodeElementUI*>(builder.Create(_T("FriendListItem.xml"), (UINT)0, NULL, &m_PaintManager));
			if(pNodeElementUI==NULL) throw TEXT("内存不足！");

			//获取变量
			CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
			//设置子项 0,vip图标  1,昵称  2,在线
			pNodeElementUI->SetTag((UINT_PTR)pCompanionInfo);

			pNodeElementUI->GetItemAt(0)->SetText(TEXT(""));
			pNodeElementUI->GetItemAt(1)->SetText(pCompanionInfo->szNickName);
			static_cast<CLabelUI*>(pNodeElementUI->GetItemAt(1))->SetTextColor(dwOffLineNickNameTextColor);

			pNodeElementUI->GetItemAt(2)->SetText(TEXT("离线"));
			static_cast<CLabelUI*>(pNodeElementUI->GetItemAt(2))->SetTextColor(dwOfflineTextColor);
			
			m_pListFriend->Add(pNodeElementUI);

		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	//好友总数
	CLabelUI* m_pLabelAllFriendCount = (CLabelUI*)GetControlByName(szLabelAllFriendCountContorlName);
	if(m_pLabelAllFriendCount != NULL)
	{
		TCHAR szCount[16]={0};
		wsprintf(szCount, TEXT("%d"), nCount);
		m_pLabelAllFriendCount->SetText(szCount);
	}
}

void CDlgFriend::UpdateFriendInfo(const tagUserRemoteInfo& UserInfo)
{
	CContainerUI* pContainerUI = (CContainerUI*)GetControlByName(szVerticalLayoutRightViewContorlName);
	if(pContainerUI) pContainerUI->SetVisible(true);
	
	//#define US_NULL						0x00								//没有状态
	//#define US_FREE						0x01								//站立状态
	//#define US_SIT						0x02								//坐下状态
	//#define US_READY						0x03								//同意状态
	//#define US_LOOKON						0x04								//旁观状态
	//#define US_PLAYING					0x05								//游戏状态
	//#define US_OFFLINE					0x06								//断线状态
	
	//头像
	if(m_pFriendAvatar)
	{
		TCHAR szBuffer[128]={0};
		WORD wFaceID = UserInfo.wFaceID;
		/*wsprintf(szAvatar, TEXT("AVATAR_48_48\\Avatar%d.png"), UserInfo.wFaceID);*/
		wsprintf(szBuffer, TEXT("file=\'AVATAR_48_48\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
		m_pFriendAvatar->SetBkImage(szBuffer);
	}

	const int nVipLevel = UserInfo.cbMemberOrder;
	//vip
	tString tLevelName;
	TCHAR szLevel[64]={0};
	if(nVipLevel > 0)
	{
		wsprintf(szLevel, TEXT("{i DLG_BOX_FRIEND\\LABEL_VIP_15_15.png 5 %d}{x 3}"), nVipLevel-1);
		tLevelName += szLevel;
	}
	//级别和昵称 
	if(m_pLabelFriendNickName)
	{
		tLevelName += UserInfo.szNickName;
		m_pLabelFriendNickName->SetText(tLevelName.c_str());
	}

	//游戏ID
	TCHAR szGameID[16]=TEXT("");
	_sntprintf(szGameID,CountArray(szGameID),TEXT("ID：%d"),UserInfo.dwGameID);
	if(m_pLabelFriendGameID) m_pLabelFriendGameID->SetText(szGameID);

	//查找对象
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(UserInfo.wKindID);
	tString tKindName;
	if(pGameKindItem != NULL)
	{
		tKindName = pGameKindItem->m_GameKind.szKindName;
	}
	if(m_pLabelFriendInGame) m_pLabelFriendInGame->SetText(tKindName.c_str());

	//游戏房间
	TCHAR szGameServer[36]=TEXT("");
	if( _tcslen(UserInfo.szGameServer) > 0)
	{
		_sntprintf(szGameServer,CountArray(szGameServer),TEXT("%s房间"),UserInfo.szGameServer);
	}
	if(m_pLabelFriendInRoom) m_pLabelFriendInRoom->SetText(szGameServer);
	
	if(m_pLabelFriendInTable != NULL)
	{
		TCHAR szTable[32]={0};
		if(UserInfo.wTableID  != INVALID_CHAIR)
		{
			wsprintf(szTable, TEXT("第%d桌"), UserInfo.wTableID+1);
		}
		m_pLabelFriendInTable->SetText(szTable);
	}

	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	bool bEnteredServer = pPlatformFrame->IsEnteredServerItem();

	WORD wTableID = 0;
	WORD wChairID = 0;
	bool bCanJoinGame = bEnteredServer && CanJoinGame(false, wTableID, wChairID);
	bool bCanLookOnGame = bEnteredServer && CanLookOnGame(false, wTableID, wChairID);
	bool bCanChat = bEnteredServer && ( CanChat(m_CurSelectUserInfo.dwUserID) != NULL );

	if(m_pButtonJionGame) m_pButtonJionGame->SetEnabled(bCanJoinGame);
	if(m_pButtonWatchGame) m_pButtonWatchGame->SetEnabled(bCanLookOnGame);
	if(m_pButtonChat) m_pButtonChat->SetEnabled(bCanChat);

}

CNodeElementUI* CDlgFriend::UpdateFriendOnLine(const tagUserRemoteInfo& UserInfo)
{
	for(int i = 0; i < m_pListFriend->GetCount(); i++)
	{
		CNodeElementUI* pNodeElementUI = static_cast<CNodeElementUI*>(m_pListFriend->GetItemAt(i));
		tagCompanionInfo* pCompanionInfo = (tagCompanionInfo*)pNodeElementUI->GetTag();
		if(pCompanionInfo->dwGameID == UserInfo.dwGameID)
		{
			TCHAR szBuffer[64]={0};
			DWORD dwNickNameTextColor = dwOnLineNickNameTextColor;
			//vip
			if(UserInfo.cbMemberOrder > 0)
			{
				wsprintf(szBuffer, TEXT("{x 5}{i DLG_BOX_FRIEND\\LABEL_VIP_12_12.png 5 %d}"), UserInfo.cbMemberOrder-1);
				dwNickNameTextColor = dwVipNickNameTextColor;
			}
			static_cast<CLabelUI*>(pNodeElementUI->GetItemAt(0))->SetText(szBuffer);

			static_cast<CLabelUI*>(pNodeElementUI->GetItemAt(1))->SetTextColor(dwNickNameTextColor);

			//在线
			wsprintf(szBuffer, TEXT("在线"));
			static_cast<CLabelUI*>(pNodeElementUI->GetItemAt(2))->SetText(szBuffer);
			static_cast<CLabelUI*>(pNodeElementUI->GetItemAt(2))->SetTextColor(dwOnlineTextColor);
			return pNodeElementUI;
		}
	}
	return NULL;
}

bool CDlgFriend::IsOnline(const DWORD dwGameID)
{
	int nSize = (int)m_vecFriendInfo.size();
	for(int i = 0; i < nSize; i++)
	{
		if(m_vecFriendInfo[i].dwGameID == dwGameID)
		{
			return true;
		}
	}
	return false;
}

IClientUserItem* CDlgFriend::CanChat(const DWORD dwUserID)
{
	//变量定义
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
	if(pPlazaViewServer == NULL) return NULL;

	IClientUserItem* pUserItem = pPlazaViewServer->SearchUserByUserID(dwUserID);
	if(pUserItem != NULL)
	{
		return pUserItem;
	}
	return NULL;
}

bool CDlgFriend::Chat(IClientUserItem* pUserItem)
{
	if( pUserItem == NULL ) return false;

	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
	if(pPlazaViewServer == NULL) return false;
	
	CClientUserItemArray UserItemArray;
	UserItemArray.Add(pUserItem);
	WORD wUserCount=(WORD)UserItemArray.GetCount();
	//创建私聊
	pPlazaViewServer->WhisperConversation(UserItemArray.GetData(),wUserCount);

	return true;
}

bool CDlgFriend::CanJoinGame(bool bTips, WORD& dwTableID, WORD& dwChairID)
{
	//变量定义
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
	if(pPlazaViewServer == NULL) return false;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();


	IClientUserItem* pUserItem = pPlazaViewServer->SearchUserByUserID(m_CurSelectUserInfo.dwUserID);
	IClientUserItem* pMeUserItem = pPlazaViewServer->SearchUserByUserID(pGlobalUserData->dwUserID);

	if(pUserItem != NULL && pMeUserItem != NULL && m_CurSelectUserInfo.dwUserID != pGlobalUserData->dwUserID)
	{
		//变量定义
		tagUserInfo * pUserInfo=pUserItem->GetUserInfo();
		tagUserInfo * pMeUserInfo=pMeUserItem->GetUserInfo();

		//用户信息
		WORD wTableID=pUserInfo->wTableID;
		WORD wChairID=pUserInfo->wChairID;
		BYTE cbUserStatus=pUserInfo->cbUserStatus;

		//玩家信息
		WORD wMeTableID=pMeUserInfo->wTableID;
		WORD wMeChiarID=pMeUserInfo->wChairID;
		BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

		WORD wServerType = pPlazaViewServer->GetServerType();

		if ( (cbMeUserStatus!=US_PLAYING)&&(wServerType&GAME_GENRE_MATCH)==0 )
		{
			//玩家状态判断
			if(pUserItem->GetTableID()==INVALID_TABLE)
			{
				if(bTips)
				{
					CDlgInformation Information(NULL);
					Information.ShowMessageBox(TEXT("该玩家已不在桌子上！"),MB_ICONINFORMATION);
				}
				return false;
			}

			//获取空位
			WORD wChairID=INVALID_CHAIR;
			ITableViewFrame * pITableViewFrame=pPlazaViewServer->GetTableViewFrame();
			WORD wNullCount=pITableViewFrame->GetNullChairCount(pUserItem->GetTableID(),wChairID);
			if (wNullCount==0) 
			{
				if(bTips)
				{
					CDlgInformation Information2(NULL);
					Information2.ShowMessageBox(TEXT("此游戏桌已经没有空位置了！"),MB_ICONINFORMATION);
				}
				return false;
			}
			//坐下动作
			//pPlazaViewServer->PerformSitDownAction(pUserItem->GetTableID(),wChairID,false);
			dwTableID = pUserItem->GetTableID();
			dwChairID = wChairID;
			return true;
		}
	}
	return false;
}

bool CDlgFriend::JoinGame(const WORD dwTableID, const WORD dwChairID)
{
	//变量定义
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
	if(pPlazaViewServer) pPlazaViewServer->PerformSitDownAction(dwTableID,dwChairID,false);
	return true;
}


bool CDlgFriend::CanLookOnGame(bool bTips, WORD& dwTableID, WORD& dwChairID)
{
	//获取对象
	CPlazaViewContainer * pPlazaViewContainer=CPlazaViewContainer::GetInstance();
	CPlazaViewServer * pPlazaViewServer=(CPlazaViewServer *)pPlazaViewContainer->GetViewItemByArea(VIA_Center);

	//状态判断
	if (pPlazaViewServer==NULL) return false;			
	if (pPlazaViewServer->GetServiceStatus()!=ServiceStatus_ServiceIng) return false;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	IClientUserItem* pUserItem = pPlazaViewServer->SearchUserByUserID(m_CurSelectUserInfo.dwUserID);
	IClientUserItem* pMeUserItem = pPlazaViewServer->SearchUserByUserID(pGlobalUserData->dwUserID);

	if(pUserItem != NULL && pMeUserItem != NULL && m_CurSelectUserInfo.dwUserID != pGlobalUserData->dwUserID)
	{
		//变量定义
		tagUserInfo * pUserInfo=pUserItem->GetUserInfo();
		tagUserInfo * pMeUserInfo=pMeUserItem->GetUserInfo();
		//旁观游戏
		bool bEnableLookOn=((pUserInfo->wTableID!=INVALID_TABLE)&&((pUserInfo->cbUserStatus==US_PLAYING)||(pUserInfo->cbUserStatus==US_OFFLINE)));
		if( !bEnableLookOn ) return false;

		//过虑消息
		if (pMeUserItem->GetUserStatus()==US_PLAYING)
		{
			if(bTips)
			{
				CDlgInformation Information(NULL);
				Information.ShowMessageBox(TEXT("正在进行游戏，请结束游戏后再进行旁观！"),MB_ICONINFORMATION);
			}
			return false;
		}
		//玩家状态判断
		if(pUserItem->GetUserStatus()!=US_PLAYING)
		{
			if(bTips)
			{
				CDlgInformation Information(NULL);
				Information.ShowMessageBox(TEXT("该玩家已不在游戏中！"),MB_ICONINFORMATION);
			}
			return false;
		}
		//旁观动作
		//pPlazaViewServer->PerformLookonAction(pUserItem->GetTableID(),pUserItem->GetChairID());
		dwTableID = pUserItem->GetTableID();
		dwChairID = pUserItem->GetChairID();
		return true;
	}
	return false;
}

bool CDlgFriend::LookOnGame(const WORD dwTableID, const WORD dwChairID)
{
	//变量定义
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
	if(pPlazaViewServer) pPlazaViewServer->PerformLookonAction(dwTableID, dwChairID);
	return true;
}

BOOL CDlgFriend::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("我的好友"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////