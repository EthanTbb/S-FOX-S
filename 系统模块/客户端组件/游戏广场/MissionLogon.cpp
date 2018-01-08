#include "StdAfx.h"
#include "GlobalUnits.h"
#include "MissionLogon.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "DLGMBCard.h"
#include <algorithm>
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//账号排序
bool SortAccounts(const tagAccountsInfo& lhs, const tagAccountsInfo& rhs)
{
	return lhs.tAcitveTime > rhs.tAcitveTime;
}

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CMissionLogon * CMissionLogon::m_pMissionLogon=NULL;					//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionLogon, CDlgStatus)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionLogon::CMissionLogon()
{
	//设置变量
	m_bRegister=false;
	m_bRemPassword=false;
	m_bLowVerValidate=true;

	//控件指针
	m_pDlgLogon=NULL;
	m_pDlgRegister=NULL;

	//设置组件
	SetStatusViewSink(this);

	//设置对象
	ASSERT(m_pMissionLogon==NULL);
	if (m_pMissionLogon==NULL) m_pMissionLogon=this;

	return;
}

//析构函数
CMissionLogon::~CMissionLogon()
{
	//删除对象
	SafeDelete(m_pDlgLogon);
	SafeDelete(m_pDlgRegister);

	//释放对象
	ASSERT(m_pMissionLogon==this);
	if (m_pMissionLogon==this) m_pMissionLogon=NULL;

	return;
}

//取消连接
VOID CMissionLogon::OnStatusCancel()
{
	//隐藏界面
	HideStatusWindow();

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,true);

	//重新登录
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return;
}

//连接事件
bool CMissionLogon::OnEventMissionLink(INT nErrorCode)
{
	//错误处理
	if (nErrorCode!=0L)
	{
		//关闭提示
		HideStatusWindow();

		//显示提示
		CDlgInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！"),MB_ICONERROR);

		//重新登录
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}
	}
	else
	{
		//发送数据
		if(m_bLowVerValidate)
			SendLogonPacket(MB_VALIDATE_FLAGS|LOW_VER_VALIDATE_FLAGS);
		else
			SendLogonPacket(MB_VALIDATE_FLAGS);
	}

	return true;
}

//关闭事件
bool CMissionLogon::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//关闭提示
		HideStatusWindow();

		//显示提示
		CDlgInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告或稍后再重试！"),MB_ICONERROR);

		//重新登录
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}
	}

	return true;
}

//读取事件
bool CMissionLogon::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_LOGON)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LOGON_SUCCESS:	//登录成功
			{
				return OnSocketSubLogonSuccess(pData,wDataSize);
			}
		case SUB_GP_LOGON_FAILURE:	//登录失败
			{
				return OnSocketSubLogonFailure(pData,wDataSize);
			}
		case SUB_GP_LOGON_FINISH:	//登录完成
			{
				return OnSocketSubLogonFinish(pData,wDataSize);
			}
		case SUB_GP_VALIDATE_MBCARD://登录失败
			{
				return OnSocketSubLogonValidateMBCard(pData,wDataSize);
			}
		case SUB_GP_VALIDATE_PASSPORT: //验证证件
			{
				return OnSocketSubLogonValidatePassPort(pData,wDataSize);
			}
		case SUB_GP_VERIFY_RESULT:	//验证结果
			{
				return OnSocketSubVerifyIndividualResult(pData,wDataSize);
			}
		case SUB_GP_UPDATE_NOTIFY:	//升级提示
			{
				return OnSocketSubUpdateNotify(pData,wDataSize);
			}
		case SUB_GP_GROWLEVEL_CONFIG: //等级配置
			{
				return OnSocketSubGrowLevelConfig(pData,wDataSize);
			}
		case SUB_GP_MEMBER_PARAMETER_RESULT://会员配置
			{
				return OnSocketSubMemberParameterResult(pData,wDataSize);
			}
		case SUB_GP_REAL_AUTH_CONFIG://认证配置 
			{
				return OnSocketSubRealAuthConfig(pData,wDataSize);
			}
		}
	}

	return false;
}

//显示登录
VOID CMissionLogon::ShowLogon(BYTE cbShowFalg)
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//设置变量
	m_bRegister=false;

	//创建窗口
	if (m_pDlgLogon==NULL)
	{
		m_pDlgLogon=new CDlgLogon;
	}

	//创建窗口
	if (m_pDlgLogon->m_hWnd==NULL)
	{
		m_pDlgLogon->Create(IDD_DLG_POPUP,GetDesktopWindow());
	}

	//显示窗口
	if (GetActiveStatus()==false)
	{
		if(m_pDlgLogon->IsIconic())
		{
			m_pDlgLogon->ShowWindow(SW_RESTORE);
		}

		if(cbShowFalg!=0) 
		{
			m_pDlgLogon->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDlgLogon->ShowWindow(SW_HIDE);
		}
		m_pDlgLogon->SetForegroundWindow();
		m_pDlgLogon->CenterWindow();
	}

	//隐藏窗口
	if ((m_pDlgRegister!=NULL)&&(m_pDlgRegister->m_hWnd!=NULL))
	{
		m_pDlgRegister->ShowWindow(SW_HIDE);
	}

	return;
}

//显示注册
VOID CMissionLogon::ShowRegister(BYTE cbShowFalg)
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//设置变量
	m_bRegister=true;

	//创建窗口
	if (m_pDlgRegister==NULL)
	{
		m_pDlgRegister=new CDlgRegister;
	}
	
	//创建窗口
	if (m_pDlgRegister->m_hWnd==NULL)
	{
		m_pDlgRegister->Create(IDD_DLG_POPUP,GetDesktopWindow());
	}

	//显示窗口
	if(cbShowFalg!=0) 
	{
		m_pDlgRegister->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgRegister->ShowWindow(SW_HIDE);
	}
	m_pDlgRegister->SetForegroundWindow();
	m_pDlgRegister->CenterWindow();

	//隐藏窗口
	if ((m_pDlgLogon!=NULL)&&(m_pDlgLogon->m_hWnd!=NULL))
	{
		m_pDlgLogon->ShowWindow(SW_HIDE);
	}

	return;
}

//记录用户
VOID CMissionLogon::UpdateUserInfo()
{
	//最多保存MAX_ACCOUNTS数量的账号信息，超过把时间最早的删掉
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置标志
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal!=NULL) pParameterGlobal->m_bRemberPassword=m_bRemPassword;

	//构造信息
	TCHAR szUserID[16];
	_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

	//上次用户
	CWHRegKey RegUserInfo;
	RegUserInfo.OpenRegKey(REG_USER_INFO,true);
	RegUserInfo.WriteValue(TEXT("LastUserID"),pGlobalUserData->dwUserID);

	//变量定义
	TCHAR szGameID[16]=TEXT("");
	TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");

	//用户表项
	CWHRegKey RegUserItem;
	RegUserItem.Attach(RegUserInfo.CreateItemKey(szUserID));
	//游戏标识
	if (pGlobalUserData->dwGameID!=0L)
	{
		_sntprintf(szGameID,CountArray(szGameID),TEXT("%ld"),pGlobalUserData->dwGameID);
	}

	//保存密码
	if (m_bRemPassword==true)
	{
		if (m_bRegister==false)
		{
			LPCTSTR pszPassword=m_pDlgLogon->m_szPassword;
			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
		}
		else
		{
			LPCTSTR pszPassword=m_pDlgRegister->m_szLogonPass;
			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
		}
	}

	//辅助信息
	RegUserItem.WriteValue(TEXT("FaceID"),pGlobalUserData->wFaceID);
	RegUserItem.WriteValue(TEXT("AcitveTimeToken"),(DWORD)time(NULL));

	//帐号信息
	RegUserItem.WriteString(TEXT("GameID"),szGameID);
	RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
	RegUserItem.WriteString(TEXT("UserAccount"),pGlobalUserData->szAccounts);
	RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);

	
	//超过MAX_ACCOUNTS个按时间排序，删掉时间最小的
	DWORD dwRegIndex=0L;
	vector<tagAccountsInfo> vecTmpAccounts;
	do
	{
		//变量定义
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//读取键项
		TCHAR szKeyItemName[32]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//打开子键
		//CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//帐户信息
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//读取密码
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//解开密码
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		AccountsInfo.tAcitveTime = RegUserItem.GetValue(TEXT("AcitveTimeToken"), 0);

		//插入数据
		vecTmpAccounts.push_back(AccountsInfo);
	}while(true);

	if( vecTmpAccounts.size() > 5 )
	{
		std::sort(vecTmpAccounts.begin(), vecTmpAccounts.end(), SortAccounts);
		int nAccountCount= (int)vecTmpAccounts.size();
		for(int i = MAX_ACCOUNTS;  i < nAccountCount; i++)
		{
			//删除表项
			CString strUserID;
			strUserID.Format(TEXT("%ld"),vecTmpAccounts[i].dwUserID);
			RegUserInfo.RecurseDeleteKey(strUserID);
		}
	}
	return;
}

//执行登录
bool CMissionLogon::PerformLogonMission(bool bRemPassword)
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return false;

	//获取地址
	TCHAR szCustomServer[LEN_SERVER]=TEXT("");
	if (m_pDlgLogon!=NULL) lstrcpyn(szCustomServer,m_pDlgLogon->m_szLogonServer,CountArray(szCustomServer));

	//设置地址
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);

	//发起连接
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		//错误提示
		CDlgInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("服务器连接失败，请检查网络配置以及网络连接状况！"),MB_ICONERROR);

		//重新登录
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}

		return false;
	}

	//设置变量
	m_bRemPassword=bRemPassword;

	//写入地址
	CGlobalServer GlobalServer;
	GlobalServer.SetLastServerName(szCustomServer);

	//显示进度
	if(m_bRegister==false || (m_pDlgRegister && m_pDlgRegister->m_bVerifyIndividual==false))
	{
		ShowStatusWindow(TEXT("正在连接服务器，请耐心稍候片刻..."));
	}

	return true;
}

//登录成功
bool CMissionLogon::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
	if (wDataSize<sizeof(CMD_GP_LogonSuccess)) return false;

	//消息处理
	CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//保存信息
	pGlobalUserData->wFaceID=pLogonSuccess->wFaceID;
	pGlobalUserData->cbGender=pLogonSuccess->cbGender;
	pGlobalUserData->dwGameID=pLogonSuccess->dwGameID;
	pGlobalUserData->dwUserID=pLogonSuccess->dwUserID;
	pGlobalUserData->dwCustomID=pLogonSuccess->dwCustomID;	
	pGlobalUserData->dwExperience=pLogonSuccess->dwExperience;
	pGlobalUserData->lLoveLiness=pLogonSuccess->lLoveLiness;
	pGlobalUserData->cbMoorMachine=pLogonSuccess->cbMoorMachine;

	//用户成绩
	pGlobalUserData->lUserScore=pLogonSuccess->lUserScore;
	pGlobalUserData->lUserInsure=pLogonSuccess->lUserInsure;
	pGlobalUserData->lUserIngot=pLogonSuccess->lUserIngot;
	pGlobalUserData->dUserBeans=pLogonSuccess->dUserBeans;

	//登录信息
	pGlobalUserData->dwLogonTickCount=GetTickCount();

	//配置信息
	pGlobalUserData->cbInsureEnabled=pLogonSuccess->cbInsureEnabled;

	//显示配置	
	pParameterGlobal->m_bShowServerStatus=(pLogonSuccess->cbShowServerStatus!=0);

	//帐号信息
	lstrcpyn(pGlobalUserData->szAccounts,pLogonSuccess->szAccounts,CountArray(pGlobalUserData->szAccounts));
	lstrcpyn(pGlobalUserData->szNickName,pLogonSuccess->szNickName,CountArray(pGlobalUserData->szNickName));
	lstrcpyn(pGlobalUserData->szDynamicPass,pLogonSuccess->szDynamicPass,CountArray(pGlobalUserData->szDynamicPass));	

	//扩展信息
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
	while (true)
	{
		//提取数据
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		//提取数据
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_MEMBER_INFO:	//会员信息
			{
				ASSERT(DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo));
				if (DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo))
				{
					DTP_GP_MemberInfo * pMemberInfo=(DTP_GP_MemberInfo *)pDataBuffer;
					pGlobalUserData->cbMemberOrder=pMemberInfo->cbMemberOrder;
					pGlobalUserData->MemberOverDate=pMemberInfo->MemberOverDate;
				}

				break;
			}
		case DTP_GP_UNDER_WRITE:	//个性签名
			{
				ASSERT(DataDescribe.wDataSize<=sizeof(pGlobalUserData->szUnderWrite));
				if (DataDescribe.wDataSize<=sizeof(pGlobalUserData->szUnderWrite))
				{
					pGlobalUserData->szUnderWrite[DataDescribe.wDataSize/sizeof(TCHAR)-1L]=0;
					CopyMemory(pGlobalUserData->szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
				}

				break;
			}
		}
	}

	//本地加载
	if (pLogonSuccess->dwCustomID!=0)
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pLogonSuccess->dwUserID,pLogonSuccess->dwCustomID,pGlobalUserData->CustomFaceInfo);
	}

	CUserInformation* pUserInformation = CUserInformation::GetInstance();
	pUserInformation->LoadInformation();

	//设置提示
	ShowStatusWindow(TEXT("正在获取游戏列表..."));

	return true;
}

//登录失败
bool CMissionLogon::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GP_LogonFailure * pLogonFailure=(CMD_GP_LogonFailure *)pData;

	//效验数据
	ASSERT(wDataSize>=(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//关闭处理
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//进度界面
	HideStatusWindow();

	//显示消息
	LPCTSTR pszDescribeString(pLogonFailure->szDescribeString);
	if (lstrlen(pszDescribeString)>0) 
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(pszDescribeString,MB_ICONERROR,60);
	}

	//重新登录
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return true;
}

//登录完成
bool CMissionLogon::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_LogonFinish));
	if (wDataSize!=sizeof(CMD_GP_LogonFinish)) return false;

	//变量定义
	CMD_GP_LogonFinish * pLogonFinish=(CMD_GP_LogonFinish *)pData;

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//时间配置
	pParameterGlobal->m_wIntermitTime=pLogonFinish->wIntermitTime;
	pParameterGlobal->m_wOnLineCountTime=pLogonFinish->wOnLineCountTime;

	//更新信息
	UpdateUserInfo();


	//删除控件
	SafeDelete(m_pDlgLogon);
	SafeDelete(m_pDlgRegister);

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//打开主页
	ASSERT(CPlatformFrame::GetInstance()!=NULL);
	CPlatformFrame::GetInstance()->WebBrowse(pGlobalWebLink->GetPlatformLink(),false);

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//发送事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGON,0L);

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//网络加载自定义头像
	if ((pGlobalUserData->dwCustomID!=0)&&(pGlobalUserData->CustomFaceInfo.dwDataSize==0L))
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID);
	}

	//关闭窗口
	HideStatusWindow();
	CPlatformFrame::GetInstance()->OnWkeURLHidden();
	return true;
}

//升级提示
bool CMissionLogon::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GP_UpdateNotify)) return false;

	//设置变量
	CMD_GP_UpdateNotify * pUpdateNotify=(CMD_GP_UpdateNotify *)pData;

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//隐藏窗口
	HideStatusWindow();

	//提示消息
	if ((pUpdateNotify->cbMustUpdate==TRUE)||(pUpdateNotify->cbAdviceUpdate==TRUE))
	{
		//构造提示
		if (pUpdateNotify->cbMustUpdate==FALSE)
		{
			//构造提示
			LPCTSTR pszString=TEXT("游戏大厅已升级，您现在的版本还能继续使用，建议您立即下载更新！");

			//退出判断
			CDlgInformation Information(CPlatformFrame::GetInstance());
			if (Information.ShowMessageBox(pszString,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES)
			{
				m_bLowVerValidate=false;
				PerformLogonMission(m_bRemPassword);
				return true;
			}
		}
	}

	//下载大厅
	//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	//if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("游戏大厅"),0,0);

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//目标目录
	TCHAR szTargetPath[MAX_PATH]=TEXT("");
	_sntprintf(szTargetPath,CountArray(szTargetPath),TEXT("%s\\Update.exe"),szDirectory);

	//启动升级
	ShellExecute(NULL,TEXT("open"),szTargetPath,TEXT(""),NULL,true);

	CPaintManagerUI::InvokAllCloseCallBack();

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	pGlobalUserData->dwUserID = 0L;

	//关闭窗口
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return true;
}

//等级配置
bool CMissionLogon::OnSocketSubGrowLevelConfig(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize<=sizeof(CMD_GP_GrowLevelConfig));				
	if (wDataSize>sizeof(CMD_GP_GrowLevelConfig)) return false;

	//提取数据
	CMD_GP_GrowLevelConfig * pGrowLevelConfig=(CMD_GP_GrowLevelConfig*)pData;

    //配置等级
	ASSERT(CUserItemElement::GetInstance());
	CUserItemElement::GetInstance()->SetGrowLevelConfig(pGrowLevelConfig->wLevelCount,pGrowLevelConfig->GrowLevelItem);

	return true;
}

//会员配置
bool CMissionLogon::OnSocketSubMemberParameterResult(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize<=sizeof(CMD_GP_MemberParameterResult));				
	if (wDataSize>sizeof(CMD_GP_MemberParameterResult)) return false;

	//提取数据
	CMD_GP_MemberParameterResult * pMemberParameterResult=(CMD_GP_MemberParameterResult*)pData;

	//配置会员
	ASSERT(CUserItemElement::GetInstance());
	CUserItemElement::GetInstance()->SetMemberParameter(pMemberParameterResult->MemberParameter, pMemberParameterResult->wMemberCount);

	return true;
}


bool CMissionLogon::OnSocketSubRealAuthConfig(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize<=sizeof(CMD_GP_RealAuthParameter));				
	if (wDataSize>sizeof(CMD_GP_RealAuthParameter)) return false;

	CMD_GP_RealAuthParameter* pParameter = (CMD_GP_RealAuthParameter*)pData;

	tagAuthRealParameter CmdParameter;
	CmdParameter.dwAuthRealAward = pParameter->dwAuthRealAward;
	CmdParameter.dwAuthentDisable = pParameter->dwAuthentDisable;
	//设置数据
	CGlobalUnits::GetInstance()->SetAuthRealParameter(&CmdParameter);

	return true;
}

//发送信息
bool CMissionLogon::SendLogonPacket(BYTE cbValidateFlags)
{
	//变量定义
	WORD wPacketSize,wSubCmdID;
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//构造数据
	if (m_bRegister==false)
	{
		wPacketSize=m_pDlgLogon->ConstructLogonPacket(cbBuffer,sizeof(cbBuffer), cbValidateFlags);
		if(m_pDlgLogon->m_cbLogonMode==LOGON_BY_VISITOR)
			wSubCmdID=SUB_GP_LOGON_VISITOR;
		else
			wSubCmdID=(m_pDlgLogon->m_cbLogonMode==LOGON_BY_GAME_ID)?SUB_GP_LOGON_GAMEID:SUB_GP_LOGON_ACCOUNTS;
	}
	else
	{
		wPacketSize=m_pDlgRegister->ConstructRegisterPacket(cbBuffer,sizeof(cbBuffer), cbValidateFlags);
		wSubCmdID=m_pDlgRegister->m_bVerifyIndividual?SUB_GP_VERIFY_INDIVIDUAL:SUB_GP_REGISTER_ACCOUNTS;
	}

	//提示信息
	if(m_bRegister==false || m_pDlgRegister->m_bVerifyIndividual==false)
	{
		ShowStatusWindow(TEXT("正在验证用户登录信息..."));
	}

	//发送数据
	ASSERT(GetMissionManager()!=NULL);
	GetMissionManager()->SendData(MDM_GP_LOGON,wSubCmdID,cbBuffer,wPacketSize);

	return true;
}

//登录失败
bool CMissionLogon::OnSocketSubLogonValidateMBCard(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_ValidateMBCard));
	if (wDataSize!=sizeof(CMD_GP_ValidateMBCard)) return false;

	//进度界面
	HideStatusWindow();

	//消息处理
	CMD_GP_ValidateMBCard * pValidateMBCard=(CMD_GP_ValidateMBCard *)pData;

	//消息处理
	CDLGMBCard DLGMBCard;
	DLGMBCard.SetMBCard(pValidateMBCard->uMBCardID);
	if (IDOK==DLGMBCard.DoModal())
	{
		SendLogonPacket(0);
	}
	else
	{
		//结束任务
		GetMissionManager()->ConcludeMissionItem(this, true);

		//重新登录
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}
	}

	return true;
}

//登录失败
bool  CMissionLogon::OnSocketSubLogonValidatePassPort(VOID * pData, WORD wDataSize)
{
	//进度界面
	HideStatusWindow();

	//验证证件
	if(m_pDlgLogon!=NULL && m_pDlgLogon->m_hWnd!=NULL)
	{
		if(m_pDlgLogon->ShowAccreditWindow()==true)
		{
			return SendLogonPacket(0);
		}
	}

	//结束任务
	GetMissionManager()->ConcludeMissionItem(this, true);

	//重新登录
	ShowLogon();

	return true;
}

//验证昵称
bool CMissionLogon::OnSocketSubVerifyIndividualResult(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize<=sizeof(CMD_GP_VerifyIndividualResult));
	if (wDataSize>sizeof(CMD_GP_VerifyIndividualResult)) return false;

	//提取数据
	CMD_GP_VerifyIndividualResult * pVerifyIndividualResult = (CMD_GP_VerifyIndividualResult *)pData;
	ASSERT(pVerifyIndividualResult!=NULL);

	//验证结果
	if(m_pDlgRegister!=NULL && m_pDlgRegister->m_hWnd!=NULL)
	{
		m_pDlgRegister->OnVerifypIndividualResult(pVerifyIndividualResult->wVerifyMask,pVerifyIndividualResult->bVerifyPassage,pVerifyIndividualResult->szErrorMsg);
	}

	//结束任务
	GetMissionManager()->ConcludeMissionItem(this, true);

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
