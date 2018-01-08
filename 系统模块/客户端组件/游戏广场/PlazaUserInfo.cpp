#include "StdAfx.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "PlazaUserInfo.h"
#include "GamePlaza.h"
#include "DlgInformation.h"
#include <time.h>
//////////////////////////////////////////////////////////////////////////////////
//用户控件
const TCHAR* const szPlazaHallAvtatrControlName = TEXT("PlazaHallAvtatr");
const TCHAR* const szDoubleBuffControlName		= TEXT("DoubleBuff");
const TCHAR* const szAmuletBuffControlName		= TEXT("AmuletBuff");
const TCHAR* const szGuardKickBuffControlName	= TEXT("GuardKickBuff");

//按钮控件
const TCHAR* const szButtonIndividualControlName = TEXT("ButtonIndividual");
const TCHAR* const szButtonCopyUserIDControlName = TEXT("ButtonCopyUserID");
const TCHAR* const szButtonRechargeControlName = TEXT("ButtonRecharge");
const TCHAR* const szButtonGetScoreControlName = TEXT("ButtonGetScore");
const TCHAR* const szButtonExchargeControlName = TEXT("ButtonExcharge");
const TCHAR* const szButtonVipControlName = TEXT("ButtonVip");
//标签控件
const TCHAR* const szLabelNickNameControlName = TEXT("LabelNickName");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");


//进度控件
const TCHAR* const szProgressExperienceControlName = TEXT("ProgressExperience");

//数字控件
const TCHAR* const szNumberUserIDControlName = TEXT("NumberUserID");
const TCHAR* const szNumberUserLevelControlName = TEXT("NumberUserLevel");
const TCHAR* const szNumberGameBeanControlName = TEXT("NumberGameBean");
const TCHAR* const szNumberGameScoreControlName = TEXT("NumberGameScore");
const TCHAR* const szNumberGameIngotControlName = TEXT("NumberGameIngot");

//////////////////////////////////////////////////////////////////////////////////
//快速导航

//按钮控件
const TCHAR* const szButtonMyTaskControlName = TEXT("ButtonMyTask");
const TCHAR* const szButtonMyFriendControlName = TEXT("ButtonMyFriend");
const TCHAR* const szButtonBaseEnsureControlName = TEXT("ButtonBaseEnsure");
const TCHAR* const szButtonCheckInControlName = TEXT("ButtonCheckIn");
//////////////////////////////////////////////////////////////////////////////////

//实名提示
const TCHAR* const szRealTipFlagControlName = TEXT("RealTipFlag");
const TCHAR* const szRealTipContainerControlName = TEXT("RealTipContainer");
const TCHAR* const szRealTipButtonEnterControlName = TEXT("RealTipButtonEnter");
const TCHAR* const szRealTipButtonCloseControlName = TEXT("RealTipButtonClose");
const TCHAR* const szRealTipNumberAwardControlName = TEXT("RealTipNumberAward");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaUserInfo::CPlazaUserInfo(CPaintManagerUI*	pPaintManager):m_pPaintManager(pPaintManager)
,m_pUserDoubleBuff(NULL)
,m_pUserAmuletBuff(NULL)
,m_pUserGuardKickBuff(NULL)
{
	//设置控件
	m_MissionManager.InsertMissionItem(this);
	//设置变量
	ZeroMemory(&m_GrowLevelParameter,sizeof(m_GrowLevelParameter));		
	m_wFaceID = -1;
	m_pPlazaHallAvtatr = NULL;
	m_bQueryLevel = false;
	m_bQueryPropBuff = false;
	m_bQueryIndividual = false;
	m_bQuerySendPresent = false;
	m_nRealNum = 0;
}

//析构函数
CPlazaUserInfo::~CPlazaUserInfo()
{

}

void CPlazaUserInfo::ClearBuff()
{
	InitControlUI();
	m_vecPropertyBuff.clear();
	CPlatformFrame* pPlatformFrame =  CPlatformFrame::GetInstance();
	::KillTimer(pPlatformFrame->GetSafeHwnd(), IDI_CHECK_USER_BUFF);
}

//连接事件
bool CPlazaUserInfo::OnEventMissionLink(INT nErrorCode)
{
	//错误判断
	if (nErrorCode!=0L) return true;

	//变量定义
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	if(m_bQueryLevel)
	{
		//变量定义
		CMD_GP_GrowLevelQueryInfo GrowLevelQueryInfo;
		ZeroMemory(&GrowLevelQueryInfo,sizeof(GrowLevelQueryInfo));

		//设置数据
		GrowLevelQueryInfo.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(GrowLevelQueryInfo.szMachineID);
		lstrcpyn(GrowLevelQueryInfo.szPassword,pGlobalUserData->szPassword,CountArray(GrowLevelQueryInfo.szPassword));

		//发送数据
		pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GROWLEVEL_QUERY,&GrowLevelQueryInfo,sizeof(GrowLevelQueryInfo));
		m_bQueryLevel = false;
	}
	else if(m_bQueryPropBuff)
	{
		CMD_GP_C_UserPropertyBuff UserPropertyBuff;
		UserPropertyBuff.dwUserID = pGlobalUserData->dwUserID;
		pMissionManager->SendData(MDM_GP_PROPERTY,SUB_GP_PROPERTY_BUFF,&UserPropertyBuff,sizeof(UserPropertyBuff));
		m_bQueryPropBuff = false;
	}
	else if (m_bQueryIndividual)
	{
		//变量定义
		CMD_GP_QueryIndividual QueryIndividual;
		ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		QueryIndividual.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(QueryIndividual.szPassword,pGlobalUserData->szPassword,CountArray(QueryIndividual.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));
		m_bQueryIndividual = false;

	}
	else if (m_bQuerySendPresent)
	{
		//变量定义
		CMD_GR_C_QuerySendPresent QuerySendPresent={0};

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		QuerySendPresent.dwUserID=pGlobalUserData->dwUserID;

		//发送数据
		m_MissionManager.SendData(MDM_GP_PROPERTY,SUB_GP_QUERY_SEND_PRESENT,&QuerySendPresent,sizeof(QuerySendPresent));
		m_bQuerySendPresent = false;
	}

	return true;
}

//关闭事件
bool CPlazaUserInfo::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//读取事件
bool CPlazaUserInfo::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GROWLEVEL_PARAMETER:	//等级参数
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_GrowLevelParameter));				
				if (wDataSize!=sizeof(CMD_GP_GrowLevelParameter)) return false;

                //提取数据
				CMD_GP_GrowLevelParameter * pGrowLevelParameter=(CMD_GP_GrowLevelParameter *)pData;

				//设置变量
				m_GrowLevelParameter.wCurrLevelID=pGrowLevelParameter->wCurrLevelID;				
				m_GrowLevelParameter.dwExperience=pGrowLevelParameter->dwExperience;
				m_GrowLevelParameter.dwUpgradeExperience=pGrowLevelParameter->dwUpgradeExperience;
				m_GrowLevelParameter.lUpgradeRewardGold=pGrowLevelParameter->lUpgradeRewardGold;
				m_GrowLevelParameter.lUpgradeRewardIngot=pGrowLevelParameter->lUpgradeRewardIngot;

				//关闭连接
				CMissionManager * pMissionManager=GetMissionManager();
				if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

				//更新等级
				UpdateLevelUI();

				//查询道具BUFF
				QueryPropBuff();
				//激活链接
				AvtiveMission();
				return true;
			}
		case SUB_GP_GROWLEVEL_UPGRADE:		//等级升级
			{
				//变量定义
				CMD_GP_GrowLevelUpgrade * pGrowLevelUpgrade=(CMD_GP_GrowLevelUpgrade *)pData;

				//效验数据
				ASSERT(wDataSize>=CountStringBuffer(pGrowLevelUpgrade->szNotifyContent));
				if (wDataSize<CountStringBuffer(pGrowLevelUpgrade->szNotifyContent)) return false;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//更新财富
				pGlobalUserData->lUserScore=pGrowLevelUpgrade->lCurrScore;
				pGlobalUserData->lUserIngot=pGrowLevelUpgrade->lCurrIngot;				

				//显示消息
				if (pGrowLevelUpgrade->szNotifyContent[0]!=0)
				{
					CDlgInformation Information;
					Information.ShowMessageBox(pGrowLevelUpgrade->szNotifyContent,MB_ICONINFORMATION,60);
				}	

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

				return true;
			}
		case SUB_GP_USER_INDIVIDUAL:	//个人信息
			{
				//效验参数
				ASSERT(wDataSize>=sizeof(CMD_GP_UserIndividual));
				if (wDataSize<sizeof(CMD_GP_UserIndividual)) return false;

				//变量定义
				CMD_GP_UserIndividual * pUserIndividual=(CMD_GP_UserIndividual *)pData;

				//变量定义
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pIndividualUserData->dwUserID=pUserIndividual->dwUserID;


				//变量定义
				VOID * pDataBuffer=NULL;
				tagDataDescribe DataDescribe;
				CRecvPacketHelper RecvPacket(pUserIndividual+1,wDataSize-sizeof(CMD_GP_UserIndividual));

				//扩展信息
				while (true)
				{
					pDataBuffer=RecvPacket.GetData(DataDescribe);
					if (DataDescribe.wDataDescribe==DTP_NULL) break;
					switch (DataDescribe.wDataDescribe)
					{
					case DTP_GP_UI_USER_NOTE:		//用户备注
						{
							lstrcpyn(pIndividualUserData->szUserNote,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szUserNote));
							break;
						}
					case DTP_GP_UI_COMPELLATION:	//真实名字
						{
							lstrcpyn(pIndividualUserData->szCompellation,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szCompellation));
							break;
						}
					case DTP_GP_UI_PASSPORTID:	//身份证
						{
							lstrcpyn(pIndividualUserData->szPassPortID,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szPassPortID));
							break;
						}
					case DTP_GP_UI_SEAT_PHONE:		//固定电话
						{
							lstrcpyn(pIndividualUserData->szSeatPhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szSeatPhone));
							break;
						}
					case DTP_GP_UI_MOBILE_PHONE:	//移动电话
						{
							lstrcpyn(pIndividualUserData->szMobilePhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szMobilePhone));
							break;
						}
					case DTP_GP_UI_QQ:				//Q Q 号码
						{
							lstrcpyn(pIndividualUserData->szQQ,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szQQ));
							break;
						}
					case DTP_GP_UI_EMAIL:			//电子邮件
						{
							lstrcpyn(pIndividualUserData->szEMail,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szEMail));
							break;
						}
					case DTP_GP_UI_DWELLING_PLACE:	//联系地址
						{
							lstrcpyn(pIndividualUserData->szDwellingPlace,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szDwellingPlace));
							break;
						}
					case DTP_GP_UI_SPREADER:	//推荐标识
						{
							lstrcpyn(pGlobalUserData->szSpreader,(LPCTSTR)pDataBuffer,CountArray(pGlobalUserData->szSpreader));
							break;
						}
					}
				}

				//关闭连接
				CMissionManager * pMissionManager=GetMissionManager();
				if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

				//实名判断
				if(pIndividualUserData->szCompellation[0] ==0 || pIndividualUserData->szPassPortID[0] == 0)
				{
					////获取实名
					//GetRealTipNum();
					//SetRealTipAward();
					//if (m_nRealNum < 3)
					//{
					//	ShowRealTip();
					//}
					//else
					//{
					//	ShowRealTip();
					//	CancleRealTip();
					//}
					//m_nRealNum++;
					//WriteRealTipNum();

					SetRealTipAward();


					//发送事件
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_REAL_TIP,0);
					
				}
				//else
				//{
				//	HideRealTip();
				//}
				
				//激活查询赠送
				AvtiveQuerySendPresentMission();

				return true;
			}
		}
	}

	//道具消息
	if(Command.wMainCmdID==MDM_GP_PROPERTY)
	{
		if(Command.wSubCmdID==SUB_GP_PROPERTY_BUFF_RESULT)	//道具BUFF
		{
			CMD_GP_S_UserPropertyBuff*  pUserPropertyBuff = (CMD_GP_S_UserPropertyBuff *)pData;

			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
			m_vecPropertyBuff.clear();

			if(pUserPropertyBuff->dwUserID == pGlobalUserData->dwUserID)
			{
				for(int i = 0; i < pUserPropertyBuff->cbBuffCount; i++)
				{
					m_vecPropertyBuff.push_back(pUserPropertyBuff->PropertyBuff[i]);
				}
			}


			//更新Buff图标
			if( !m_vecPropertyBuff.empty() )
			{
				UpdateBuffUI();
				CPlatformFrame* pPlatformFrame =  CPlatformFrame::GetInstance();
				::SetTimer(pPlatformFrame->GetSafeHwnd(), IDI_CHECK_USER_BUFF, 1000, NULL);
			}

			//关闭连接
			CMissionManager * pMissionManager=GetMissionManager();
			if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

			//查询个人信息
			QueryIndividual();
			//激活链接
			AvtiveMission();

			return true;
		}
		else if(Command.wSubCmdID==SUB_GP_QUERY_SEND_PRESENT_RESULT) //查询赠送
		{
			CMD_GP_S_QuerySendPresent* pQuerySendPresentResult = (CMD_GP_S_QuerySendPresent*)pData;
			if(pQuerySendPresentResult != NULL )
			{
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				pGlobalUserData->dwPresentCount = pQuerySendPresentResult->wPresentCount;

				if( pQuerySendPresentResult->wPresentCount > 0 )
				{
					vector<SendPresent> vecSendPresent;
					for(int i = 0; i < pQuerySendPresentResult->wPresentCount; i++)
					{
						vecSendPresent.push_back(pQuerySendPresentResult->Present[i]);
					}
					CDlgInformation Information;
					CStdString strNotifyContent;
					strNotifyContent.Format(TEXT("有人赠送%d个道具给你"), vecSendPresent.size());
					Information.ShowMessageBox(strNotifyContent.GetData(),MB_ICONINFORMATION);

					CGlobalUnits::GetInstance()->QuerySendPresentMisson();
				}
			}
			return true;
		}
	}

	return true;
}


//消息提醒
bool CPlazaUserInfo::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//个人中心
		if(lstrcmp(pControlUI->GetName(),szButtonIndividualControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowPersonerCenter();	
			return true;
		}
		//vip链接
		else if(lstrcmp(pControlUI->GetName(), szButtonVipControlName)==0)
		{
			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szVipLinkInfo[256]=TEXT("");
			_sntprintf(szVipLinkInfo,CountArray(szVipLinkInfo),TEXT("%s/%sUserService/UserService5.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szVipLinkInfo,NULL,NULL,SW_NORMAL);
		}
		//复制ID
		else if(lstrcmp(pControlUI->GetName(), szButtonCopyUserIDControlName)==0)
		{
			//用户ID
			CNumberUI * pControlUI = (CNumberUI *)GetControlByName(szNumberUserIDControlName);
			if(pControlUI)
			{
				//变量定义
				TCHAR szCopyText[256]=TEXT("");

				//格式字符串
				_sntprintf(szCopyText,CountArray(szCopyText),TEXT("%.0f"),pControlUI->GetNumber());
				
				//设置剪贴板
				if (CWHService::SetClipboardString(szCopyText))
				{
					CToastUI *pToast = (CToastUI *)(GetControlByName(_T("ntoast")));
					if(pToast!=NULL)
					{
						pToast->Show(500);
					}
				}

			}
			return true;
		}
		//充值游戏豆
		else if(lstrcmp(pControlUI->GetName(), szButtonRechargeControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowRecharge();	

			return true;
		}
		//获取金币
		else if(lstrcmp(pControlUI->GetName(), szButtonGetScoreControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowInsure();			

			return true;
		}
		//兑换元宝
		else if(lstrcmp(pControlUI->GetName(), szButtonExchargeControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowShop();				
			return true;
		}
		//我的好友
		else if(lstrcmp(pControlUI->GetName(), szButtonMyFriendControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowFriend();
			return true;
		}
		//显示任务
		else if(lstrcmp(pControlUI->GetName(), szButtonMyTaskControlName)==0)
		{
			
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowTaskView();

			return true;
		}
		//领取低保
		else if(lstrcmp(pControlUI->GetName(), szButtonBaseEnsureControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowBaseEnsure();

			return true;
		}
		//每日签到
		else if(lstrcmp(pControlUI->GetName(), szButtonCheckInControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowCheckIn();

			return true;
		}
		//每日签到
		else if(lstrcmp(pControlUI->GetName(), szRealTipButtonCloseControlName)==0) 
		{
			CancleRealTip();

			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szRealTipButtonEnterControlName)==0) 
		{
			CancleRealTip();
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_PERSONER,3L);

			return true;
		}

	}

	return false;
}

//初始控件
bool CPlazaUserInfo::InitControlUI()
{
	m_pPlazaHallAvtatr = (CImageUI*)GetControlByName(szPlazaHallAvtatrControlName);
	m_pUserDoubleBuff = (CControlUI*)GetControlByName(szDoubleBuffControlName);
	m_pUserAmuletBuff = (CControlUI*)GetControlByName(szAmuletBuffControlName);
	m_pUserGuardKickBuff = (CControlUI*)GetControlByName(szGuardKickBuffControlName);

	if(m_pUserDoubleBuff)
	{
		m_sDoubleBuffImage = m_pUserDoubleBuff->GetBkImage();
		m_pUserDoubleBuff->SetBkImage(TEXT(""));
		m_pUserDoubleBuff->SetVisible(false);
	}
	if(m_pUserAmuletBuff)
	{
		m_sAmuletBuffImage = m_pUserAmuletBuff->GetBkImage();
		m_pUserAmuletBuff->SetBkImage(TEXT(""));
		m_pUserAmuletBuff->SetVisible(false);
	}
	if(m_pUserGuardKickBuff)
	{
		m_sGuardKickBuffImage = m_pUserGuardKickBuff->GetBkImage();
		m_pUserGuardKickBuff->SetBkImage(TEXT(""));
		m_pUserGuardKickBuff->SetVisible(false);
	}

	return true;
}

bool CPlazaUserInfo::IsNotify(TNotifyUI &  msg)
{
	return true;
}

//获取对象
CControlUI * CPlazaUserInfo::GetControlByName(LPCTSTR pszControlName)
{

	return m_pPaintManager->FindControl(pszControlName);
}

//事件消息
bool CPlazaUserInfo::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录成功
		{
			//更新信息
			UpdateUI();
			//查询等级
			QueryLevel();
			//激活链接
			AvtiveMission();
			
			//5分钟查询一次
			CPlatformFrame* pPlatformFrame =  CPlatformFrame::GetInstance();
			::SetTimer(pPlatformFrame->GetSafeHwnd(), IDI_CHECK_SEND_PRESENT, 5*60*1000, NULL);

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			CPlatformFrame* pPlatformFrame =  CPlatformFrame::GetInstance();
			::KillTimer(pPlatformFrame->GetSafeHwnd(), IDI_CHECK_SEND_PRESENT);
			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//资料更新
		{
			//更新信息
			UpdateUI();

			//刷新等级
			UpdateLevel();

			return 0L;
		}
	case EVENT_USER_MOOR_MACHINE:	//绑定机器
		{
			return 0L;
		}
	}

	return 0L;
}

VOID CPlazaUserInfo::OnTimer(UINT_PTR nIDEvent)
{
	if(IDI_CHECK_USER_BUFF == nIDEvent)			UpdateBuffUI();
	else if(IDI_CHECK_SEND_PRESENT == nIDEvent)	AvtiveQuerySendPresentMission();
}

VOID CPlazaUserInfo::UpdateLevelUI()
{
	//计算进度
	DWORD dwMaxExperience=m_GrowLevelParameter.dwUpgradeExperience;
	dwMaxExperience=__max(dwMaxExperience,m_GrowLevelParameter.dwExperience);
	dwMaxExperience=__max(dwMaxExperience,1);

	WORD wProgress = (WORD)((m_GrowLevelParameter.dwExperience/(double)dwMaxExperience)*100);

	//进度条
	CProgressUI * pProgress = (CProgressUI *)(GetControlByName(szProgressExperienceControlName));
	if(pProgress)
	{
		pProgress->SetValue(wProgress);
	}

	//等级
	CNumberUI * pControlUI = (CNumberUI *)GetControlByName(szNumberUserLevelControlName);
	if(pControlUI)
	{
		float fLevel = (float)(m_GrowLevelParameter.wCurrLevelID);
		pControlUI->SetNumber(fLevel);
	}

}

VOID CPlazaUserInfo::UpdateBuffData(const tagPropertyBuff& Buff)
{
	int nBuffSize = (int)(m_vecPropertyBuff.size());
	for(int i = 0; i < nBuffSize; i++)
	{
		if(m_vecPropertyBuff[i].dwKind == Buff.dwKind && 
			m_vecPropertyBuff[i].dwScoreMultiple == Buff.dwScoreMultiple )
		{
			//更新时长
			m_vecPropertyBuff[i].UseResultsValidTime = Buff.UseResultsValidTime;
			return;
		}
	}
	m_vecPropertyBuff.push_back(Buff);
}

VOID CPlazaUserInfo::AvtiveMission()
{
	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (GetActiveStatus()==true) pMissionManager->ConcludeMissionItem(this,false);

	//设置地址
	LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
	if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

	//激活任务
	pMissionManager->AvtiveMissionItem(this,false);
}

VOID CPlazaUserInfo::UpdateUI()
{

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//用户昵称
	CControlUI * pLabelUI = GetControlByName(szLabelNickNameControlName);
	if(pLabelUI)
	{
		pLabelUI->SetText(pGlobalUserData->szNickName);
	}

	//会员等级
	for (int i=0;i<6;i++)
	{
		CStdString strLevel;
		strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
		pLabelUI = GetControlByName(strLevel);
		if(pLabelUI)
		{
			if (i==pGlobalUserData->cbMemberOrder)
			{
				pLabelUI->SetVisible(true);
			}
			else
			{
				pLabelUI->SetVisible(false);
			}
		}
	}

	//用户ID
	CNumberUI * pControlUI = (CNumberUI *)GetControlByName(szNumberUserIDControlName);
	if(pControlUI)
	{
		float fGameID = (float)(pGlobalUserData->dwGameID);
		pControlUI->SetNumber(fGameID);
	}

	//用户游戏豆
	pControlUI = (CNumberUI *)GetControlByName(szNumberGameBeanControlName);
	if(pControlUI)
	{
		float fUserBeans = (float)(pGlobalUserData->dUserBeans);
		pControlUI->SetNumber(fUserBeans);
	}

	//用户游戏币
	pControlUI = (CNumberUI *)GetControlByName(szNumberGameScoreControlName);
	if(pControlUI)
	{
		float fUserScore = (float)(pGlobalUserData->lUserScore);
		pControlUI->SetNumber(fUserScore);
	}

	//用户元宝
	pControlUI = (CNumberUI *)(GetControlByName(szNumberGameIngotControlName));
	if(pControlUI)
	{
		float fUserIngot = (float)(pGlobalUserData->lUserIngot);
		pControlUI->SetNumber(fUserIngot);
	}

	//更新头像
	if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
	{
		UpdateUserFace(pGlobalUserData->CustomFaceInfo.dwCustomFace);
	}
	else
	{
		UpdateUserFace(pGlobalUserData->wFaceID);
	}
	
}

VOID CPlazaUserInfo::QueryLevel()
{
	m_bQueryLevel = true;
	m_bQueryPropBuff = false;
	m_bQueryIndividual = false;
	m_bQuerySendPresent = false;
}

VOID CPlazaUserInfo::QueryPropBuff()
{
	m_bQueryPropBuff = true;
	m_bQueryIndividual = false;
	m_bQuerySendPresent = false;
	m_bQueryLevel = false;
}

VOID CPlazaUserInfo::QueryIndividual()
{
	m_bQueryIndividual = true;
	m_bQuerySendPresent = false;
	m_bQueryPropBuff = false;
	m_bQueryLevel = false;
}

VOID CPlazaUserInfo::QuerySendPresent()
{
	m_bQuerySendPresent = true;
	m_bQueryIndividual = false;
	m_bQueryPropBuff = false;
	m_bQueryLevel = false;
}

VOID CPlazaUserInfo::AvtiveQuerySendPresentMission()
{
	QuerySendPresent();
	//激活链接
	AvtiveMission();
}

VOID CPlazaUserInfo::UpdateLevel()
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//当前经验
	if(pGlobalUserData->dwExperience>=m_GrowLevelParameter.dwUpgradeExperience)
	{
		//查询等级
		QueryLevel();
	}
}

VOID CPlazaUserInfo::UpdateBuffUI()
{
	if(m_pUserDoubleBuff) m_pUserDoubleBuff->SetVisible(false);
	if(m_pUserAmuletBuff) m_pUserAmuletBuff->SetVisible(false);
	if(m_pUserGuardKickBuff) m_pUserGuardKickBuff->SetVisible(false);


	bool bValidBuff = false;
	int nBuffSize = (int)m_vecPropertyBuff.size();
	for(int i = 0; i < nBuffSize; i++)
	{
		if(m_vecPropertyBuff[i].dwKind == PROP_KIND_DOOUBLE)
		{
			bValidBuff = VisibleBuff(m_vecPropertyBuff[i], m_pUserDoubleBuff, m_sDoubleBuffImage.GetData());
		}
		else if(m_vecPropertyBuff[i].dwKind == PROP_KIND_DEFENSE)
		{
			bValidBuff = VisibleBuff(m_vecPropertyBuff[i], m_pUserAmuletBuff, m_sAmuletBuffImage.GetData());
		}
		else if(m_vecPropertyBuff[i].dwKind == PROP_KIND_DEFENSE_TICK)
		{
			bValidBuff = VisibleBuff(m_vecPropertyBuff[i], m_pUserGuardKickBuff, m_sGuardKickBuffImage.GetData());
		}
	}

	if( !bValidBuff )
	{
		CPlatformFrame* pPlatformFrame =  CPlatformFrame::GetInstance();
		::KillTimer(pPlatformFrame->GetSafeHwnd(), IDI_CHECK_USER_BUFF);
	}
}

bool CPlazaUserInfo::VisibleBuff(const tagPropertyBuff& buff, CControlUI* pControl, LPCTSTR lpImage)
{
	if(pControl == NULL) return false;

	if( buff.tUseTime + buff.UseResultsValidTime > DWORD(time(NULL)) )
	{
		if( buff.tUseTime + buff.UseResultsValidTime - time(NULL) <= 8 )
		{
			CStdString sImage = pControl->GetBkImage();
			if(sImage.IsEmpty()) pControl->SetBkImage(lpImage);
			else pControl->SetBkImage(TEXT(""));
		}
		else
		{
			pControl->SetBkImage(lpImage);
		}
		pControl->SetVisible(true);
		return true;
	}
	else
	{
		pControl->SetVisible(false);
	}
	return false;
}

VOID CPlazaUserInfo::UpdateUserFace(const WORD wFaceID)
{
	if(m_wFaceID == wFaceID) return;

	m_wFaceID = wFaceID;

	if(m_pPlazaHallAvtatr != NULL)
	{
		TCHAR szBuffer[64]={0};
		wsprintf(szBuffer, TEXT("file=\'AVATAR_48_48\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
		//wsprintf(szBuffer, TEXT("AVATAR_48_48\\Avatar%d.png"), wFaceID);
		m_pPlazaHallAvtatr->SetImage(szBuffer);
	}
}

VOID CPlazaUserInfo::UpdateUserFace(DWORD dwCustomFace[48*48])
{
	if(m_pPlazaHallAvtatr != NULL)
	{
		m_pPlazaHallAvtatr->SetCustomImage(dwCustomFace);
	}
}

//获取头像区域
RECT CPlazaUserInfo::GetUserFacePos()
{
	RECT rc = {0,0,0,0};
	if(m_pPlazaHallAvtatr != NULL)
	{
		rc = m_pPlazaHallAvtatr->GetPos();
	}
	return rc;
}




void CPlazaUserInfo::SetRealTipAward()
{

	CNumberUI * pNum = (CNumberUI *)(GetControlByName(szRealTipNumberAwardControlName));
	if(pNum)
	{
		float fAward =(float)(CGlobalUnits::GetInstance()->GetAuthRealParameter()->dwAuthRealAward);
		pNum->SetNumber(fAward);
	}
}

void CPlazaUserInfo::ShowRealTip()
{
	if (!CGlobalUnits::GetInstance()->GetAuthRealParameter()->dwAuthentDisable)
	{
		CControlUI * pLabelUI = GetControlByName(szRealTipFlagControlName);
		if(pLabelUI)
		{
			pLabelUI->SetVisible(true);
		}
		pLabelUI = GetControlByName(szRealTipContainerControlName);
		if(pLabelUI)
		{
			pLabelUI->SetVisible(true);
		}
	}

}

void CPlazaUserInfo::CancleRealTip()
{

	CControlUI * pLabelUI;
	pLabelUI = GetControlByName(szRealTipContainerControlName);
	if(pLabelUI)
	{
		pLabelUI->SetVisible(false);
	}
}

void CPlazaUserInfo::HideRealTip()
{
	CControlUI * pLabelUI = GetControlByName(szRealTipFlagControlName);
	if(pLabelUI)
	{
		pLabelUI->SetVisible(false);
	}
	pLabelUI = GetControlByName(szRealTipContainerControlName);
	if(pLabelUI)
	{
		pLabelUI->SetVisible(false);
	}
}


//////////////////////////////////////////////////////////////////////////////////


