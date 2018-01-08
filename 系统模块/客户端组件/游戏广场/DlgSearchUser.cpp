#include "StdAfx.h"
#include "Resource.h"
#include "DlgSearchUser.h"
#include "PlatformEvent.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//自定子项
#define UD_USER_STATION				(UD_CUSTOM+0)						//用户位置

//时间定义
#define IDI_SEARCH_TIME_OUT			100									//超时标识
#define TIME_SEARCH_TIME_OUT		15000L								//超时时间

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CDlgSearchUser * CDlgSearchUser::m_pDlgSearchUser=NULL;					//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSearchUser, CSkinDialog)

	//系统消息
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonOKControlName = TEXT("ButtonOK");
const TCHAR* const szButtonEnterControlName = TEXT("ButtonEnter");

//编辑控件
const TCHAR* const szEditUINickNameControlName = TEXT("EditUINickName");
const TCHAR* const szEditUIGameIDControlName = TEXT("EditUIGameID");

//标签控件
const TCHAR* const szLabelAccountsControlName = TEXT("LabelAccounts");
const TCHAR* const szLabelGameIDControlName = TEXT("LabelGameID");
const TCHAR* const szLabelPositionControlName = TEXT("LabelPosition");

//扩展按钮
const TCHAR* const szComboGameIDControlName = TEXT("ComboGameID");
const TCHAR* const szComboNickNameControlName = TEXT("ComboNickName");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSearchUser::CDlgSearchUser() : CSkinDialog(IDD_DLG_POPUP)
{
	//设置任务
	m_MissionManager.InsertMissionItem(this);

	//设置变量
	m_dwGameID=0L;
	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//信息变量
	ZeroMemory(&m_UserRemoteInfo,sizeof(m_UserRemoteInfo));

	//设置变量
	ASSERT(m_pDlgSearchUser==NULL);
	if (m_pDlgSearchUser==NULL) m_pDlgSearchUser=this;

	return;
}

//析构函数
CDlgSearchUser::~CDlgSearchUser()
{
	//设置变量
	ASSERT(m_pDlgSearchUser==this);
	if (m_pDlgSearchUser==this) m_pDlgSearchUser=NULL;

	return;
}

//消息过虑
BOOL CDlgSearchUser::PreTranslateMessage(MSG * pMsg)
{
	//发送聊天
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//变量定义
		CEditUI * pEditControl = NULL;

		//设置焦点
		pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUINickNameControlName));
		if(pEditControl && pEditControl->IsVisible())
		{
			pEditControl->SetFocus();
			pEditControl->SetText(TEXT(""));
		}

		//设置焦点
		pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUIGameIDControlName));
		if(pEditControl && pEditControl->IsVisible())
		{
			pEditControl->SetFocus();
			pEditControl->SetText(TEXT(""));
		}

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgSearchUser::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("用户查找"));

	//设置窗口
	SetWindowPos(NULL,0,0,441,245,SWP_NOZORDER|SWP_NOMOVE);	

	//加载好友
	LoadFriendUserInfo();

	return FALSE;
}

//确定消息
VOID CDlgSearchUser::OnOK()
{	
	//变量定义
	CControlUI * pNickNameControl=NULL;
	CControlUI * pGameIDControl=NULL;

	//查找对象
	pNickNameControl=GetControlByName(szEditUINickNameControlName);
	pGameIDControl=GetControlByName(szEditUIGameIDControlName);
	if (pNickNameControl==NULL || pGameIDControl==NULL) return;

	//变量定义
	CString strSearchKey;
	bool bNickNameMode=true;

	//昵称查找
	if (lstrlen(pNickNameControl->GetText())>0)
	{
		bNickNameMode=true;
		strSearchKey = pNickNameControl->GetText();
	}

	//I D查找
	if (strSearchKey.IsEmpty()) bNickNameMode=false; 
	if (bNickNameMode==false) strSearchKey = pGameIDControl->GetText();

	//输入处理
	strSearchKey.TrimLeft();
	strSearchKey.TrimRight();
	strSearchKey.MakeLower();

	//远程判断
	if (GetActiveStatus()==true)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("查找操作正在进行中，请稍后再进行新的查找提交！"),MB_ICONQUESTION,30L);

		return;
	}

	//效验输入
	if ((strSearchKey.IsEmpty()==TRUE)&&(bNickNameMode==true))
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入完整的“用户昵称”或者其中的部分字符！"),MB_ICONSTOP);

		//设置焦点
		pNickNameControl->SetFocus();

		return;
	}

	//效验输入
	if ((_tstol(strSearchKey)==0L)&&(bNickNameMode==false))
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入您要查找的“游戏 I D”号码！"),MB_ICONSTOP);

		//设置焦点
		pGameIDControl->SetFocus();

		return;
	}	

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	if (bNickNameMode==true)
	{		
		m_dwGameID=0L;
		lstrcpyn(m_szNickName,strSearchKey,CountArray(m_szNickName));

		//查找自己则退出
		if (lstrcmp(pGlobalUserData->szNickName,m_szNickName)==0) return;
	}
	else
	{
		m_dwGameID=_tstol(strSearchKey);
		ZeroMemory(m_szNickName,sizeof(m_szNickName));

		//查找自己则退出
		if (pGlobalUserData->dwGameID==m_dwGameID) return;
	}

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==true) 
	{
		//设置时间
		SetTimer(IDI_SEARCH_TIME_OUT,TIME_SEARCH_TIME_OUT,NULL);
	}

	return;
}

//初始控件
void CDlgSearchUser::InitControlUI()
{
	__super::InitControlUI();

	//变量定义
	CEditUI * pEditControl=NULL;

	//用户昵称
	pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUINickNameControlName));
	if(pEditControl!=NULL) pEditControl->SetMaxChar(LEN_NICKNAME-1);

	//游戏ID
	pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUIGameIDControlName));
	if(pEditControl!=NULL) 
	{
		pEditControl->SetMaxChar(9);
		pEditControl->SetNumberOnly();
	}

	//进入按钮
	CButtonUI * pButtonControl = static_cast<CButtonUI *>(GetControlByName(szButtonEnterControlName));
	if(pButtonControl!=NULL) pButtonControl->SetVisible(false);
}

//消息提醒
void CDlgSearchUser::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonOKControlName) == 0) return OnOK();
		if(lstrcmp(pControlUI->GetName(), szButtonEnterControlName) == 0)
		{
			//变量定义
            tagTrackAccountsInfo TrackAccountsInfo;		
			TrackAccountsInfo.wKindID=m_UserRemoteInfo.wKindID;
			TrackAccountsInfo.wServerID=m_UserRemoteInfo.wServerID;
			TrackAccountsInfo.dwTargetID=m_UserRemoteInfo.dwUserID;

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_TRACK_ACCOUNTS,(LPARAM)&TrackAccountsInfo);

			return EndDialog(IDOK);
		}
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboGameIDControlName)==0)
		{
			//查找对象
			CControlUI * pEditGameID = GetControlByName(szEditUIGameIDControlName);
			if(pEditGameID!=NULL) pEditGameID->SetText(pControlUI->GetText());

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szComboNickNameControlName)==0)
		{
			//查找对象
			CControlUI * pEditNickName = GetControlByName(szEditUINickNameControlName);
			if(pEditNickName!=NULL) pEditNickName->SetText(pControlUI->GetText());

			return;
		}
	}
}

//房间关闭
VOID CDlgSearchUser::OnViewItemDelete(CPlazaViewServer * pServerViewItem)
{
	return;
}

//用户删除
VOID CDlgSearchUser::OnUserItemDelete(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem)
{
	return;
}

//用户更新
VOID CDlgSearchUser::OnUserItemUpdate(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem)
{
	return;
}

//绘画标题
VOID CDlgSearchUser::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//资源句柄
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//加载标题
	CPngImage ImageLogo;
	ImageLogo.LoadImage(hInstance,TEXT("IMAGE_SEARCH_TITLE"));

	//绘制标题
	ImageLogo.DrawImage(pDC,10,10);
}

//绘画消息
VOID CDlgSearchUser::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	////框架位置
	//INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	//INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	//INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	//INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	////绘画图片
	//CPngImage ImageSearchLogo;
	//ImageSearchLogo.LoadImage(AfxGetInstanceHandle(),TEXT("SEARCH_LOGO"));
	//ImageSearchLogo.DrawImage(pDC,rcListControl.left+6,rcListControl.top-ImageSearchLogo.GetHeight()-5);

	return;
}

//连接事件
bool CDlgSearchUser::OnEventMissionLink(INT nErrorCode)
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
		KillTimer(IDI_SEARCH_TIME_OUT);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("连接服务器超时，远程查找操作失败！"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//变量定义
	CMD_GP_C_SearchCorrespond SearchCorrespond;
	ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

	//构造数据
	SearchCorrespond.dwGameID=m_dwGameID;
	lstrcpyn(SearchCorrespond.szNickName,m_szNickName,CountArray(SearchCorrespond.szNickName));

	//发送数据
	m_MissionManager.SendData(MDM_GP_REMOTE_SERVICE,SUB_GP_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

	return true;
}

//关闭事件
bool CDlgSearchUser::OnEventMissionShut(BYTE cbShutReason)
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
		KillTimer(IDI_SEARCH_TIME_OUT);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，远程查找操作失败！"),MB_ICONERROR,30L);
	}
	else
	{
		//删除时间
		KillTimer(IDI_SEARCH_TIME_OUT);
	}
	
	return true;
}

//读取事件
bool CDlgSearchUser::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_REMOTE_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_S_SEARCH_CORRESPOND:	//查找结果
			{
				//变量定义
				CMD_GP_S_SearchCorrespond * pSearchCorrespond=(CMD_GP_S_SearchCorrespond *)pData;

				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GP_S_SearchCorrespond));
				ASSERT(wDataSize>=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
				ASSERT(wDataSize==(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

				//效验参数
				if (wDataSize>sizeof(CMD_GP_S_SearchCorrespond)) return false;
				if (wDataSize<(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
				if (wDataSize!=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

				//失败处理
				KillTimer(IDI_SEARCH_TIME_OUT);

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//获取对象
				CControlUI * pControlAccounts=GetControlByName(szLabelAccountsControlName);
				if(pControlAccounts==NULL) return true;

				CControlUI * pControlGameID=GetControlByName(szLabelGameIDControlName);
				if(pControlGameID==NULL) return true;

				CControlUI * pControlPosition=GetControlByName(szLabelPositionControlName);
				if(pControlPosition==NULL) return true;

				CControlUI * pControlBTEnter = GetControlByName(szButtonEnterControlName);
				if(pControlBTEnter==NULL) return true;

				//设置控件
				pControlAccounts->SetText(TEXT(""));
				pControlGameID->SetText(TEXT(""));
				pControlPosition->SetText(TEXT(""));
				pControlBTEnter->SetVisible(false);

				//清除数据
				ZeroMemory(&m_UserRemoteInfo,sizeof(m_UserRemoteInfo));

				//插入列表
				for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
				{
					//查找类型
					CServerListData * pServerListData=CServerListData::GetInstance();
					CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(pSearchCorrespond->UserRemoteInfo[i].wKindID);

					//插入列表
					if (pGameKindItem!=NULL)
					{
						tagUserRemoteInfo * pUserRemoteInfo = &pSearchCorrespond->UserRemoteInfo[i];
						if(pUserRemoteInfo!=NULL)
						{
							//拷贝数据
							CopyMemory(&m_UserRemoteInfo,pUserRemoteInfo,sizeof(m_UserRemoteInfo));

							//游戏ID
							TCHAR szGameID[16]=TEXT("");
							_sntprintf(szGameID,CountArray(szGameID),TEXT("%d"),pUserRemoteInfo->dwGameID);

							//游戏房间
							TCHAR szGameServer[36]=TEXT("");
							_sntprintf(szGameServer,CountArray(szGameServer),TEXT("%s房间"),pUserRemoteInfo->szGameServer);

							//设置控件
							pControlAccounts->SetText(pUserRemoteInfo->szNickName);
							pControlGameID->SetText(szGameID);
							pControlPosition->SetText(szGameServer);
							pControlBTEnter->SetVisible(true);

							break;
						}
					}
				}

				//结果判断
				if (pSearchCorrespond->wUserCount==0L)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("您所查询的用户不存在或者没有进入房间，请更改查询条件再试！"),MB_ICONQUESTION,30L);
				}

				return true;
			}
		}
	}

	return true;
}

//时间消息
VOID CDlgSearchUser::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_SEARCH_TIME_OUT:	//超时时间
		{
			//删除时间
			KillTimer(IDI_SEARCH_TIME_OUT);

			//关闭连接
			m_MissionManager.ConcludeMissionItem(this,false);

			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("服务器超时没有响应，查找操作失败，请稍后重试！"),MB_OK|MB_ICONSTOP,30L);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//
////远程查找
//VOID CDlgSearchUser::OnBnClickedSearchRemote()
//{
//	//获取状态
//	UINT nCurrentCheck=m_SearchRemote.GetCheck();
//
//	//会员判断
//	if (nCurrentCheck==BST_UNCHECKED)
//	{
//		//变量定义
//		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
//		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
//
//		//会员判断
//		if (pGlobalUserData->cbMemberOrder==0)
//		{
//			//提示消息
//			CInformation Information(this);
//			INT_PTR nResult=Information.ShowMessageBox(TEXT("您不是会员，无法使用远程查找功能，需要立即成功会员吗？"),MB_ICONQUESTION|MB_YESNO);
//
//			//会员连接
//			if (nResult==IDYES)
//			{
//				//更新窗口
//				UpdateWindow();
//
//				//构造地址
//				TCHAR szLinkUrl[128]=TEXT("");
//				_sntprintf(szLinkUrl,CountArray(szLinkUrl),TEXT("%s/PayIndex.aspx"),szPlatformLink);
//
//				//连接地址
//				ShellExecute(NULL,TEXT("OPEN"),szLinkUrl,NULL,NULL,SW_NORMAL);
//			}
//
//			return;
//		}
//	}
//
//	//设置焦点
//	if (m_edGameID.IsWindowVisible()==TRUE) m_edGameID.SetFocus();
//	if (m_edNickName.IsWindowVisible()==TRUE) m_edNickName.SetFocus();
//
//	//设置控件
//	m_ComSearchArea.EnableWindow((nCurrentCheck==BST_CHECKED)?TRUE:FALSE);
//	m_SearchRemote.SetCheck((nCurrentCheck==BST_UNCHECKED)?BST_CHECKED:BST_UNCHECKED);
//
//	return;

//加载好友
VOID CDlgSearchUser::LoadFriendUserInfo()
{
	//查找对象
	CComboUI * pComboGameID = static_cast<CComboUI *>(GetControlByName(szComboGameIDControlName));
	if(pComboGameID==NULL) return;

	//查找对象
	CComboUI * pComboNickName = static_cast<CComboUI *>(GetControlByName(szComboNickNameControlName));
	if(pComboNickName==NULL) return;

	//获取对象
	ASSERT(CUserInformation::GetInstance());
	CUserInformation * pUserInformation = CUserInformation::GetInstance();

	//变量定义
	TCHAR szGameID[16]=TEXT("");
	CListLabelElementUI * pListLabelElement=NULL;			

    //枚举好友
	WORD wIndex=0;
	tagCompanionInfo * pCompanionInfo=NULL;
	while(pCompanionInfo=pUserInformation->EmunCompanionInfo(wIndex++))
	{
		//判断关系
		if(pCompanionInfo->cbCompanion!=CP_FRIEND) continue;

		try
		{
			//添加昵称
			pListLabelElement = new CListLabelElementUI;
			if(pListLabelElement==NULL) throw TEXT("内存不足！");

			//设置子项
			pListLabelElement->SetAttribute(TEXT("text"),pCompanionInfo->szNickName);
			pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
			pListLabelElement->SetManager(&m_PaintManager,pComboNickName,true);
			pComboNickName->Add(pListLabelElement);

			//格式字符
			_sntprintf(szGameID,CountArray(szGameID),TEXT("%d"),pCompanionInfo->dwGameID);

			//添加标识
			pListLabelElement = new CListLabelElementUI;
			if(pListLabelElement==NULL) throw TEXT("内存不足！");

			//设置子项
			pListLabelElement->SetAttribute(TEXT("text"),szGameID);
			pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
			pListLabelElement->SetManager(&m_PaintManager,pComboGameID,true);
			pComboGameID->Add(pListLabelElement);
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
