#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "Dlgbaseensure.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");

const TCHAR* const szButtonCheckInControlName = TEXT("ButtonCheckIn");
const TCHAR* const szButtonSpreadControlName = TEXT("ButtonSpread");
const TCHAR* const szButtonTaskControlName = TEXT("ButtonTask");
const TCHAR* const szButtonRechargeControlName = TEXT("ButtonRecharge");
const TCHAR* const szButtonVipControlName = TEXT("ButtonVip");

//标签控件
const TCHAR* const szLabelLessTipControlName = TEXT("LabelLessTip");
const TCHAR* const szLabelConditionControlName = TEXT("LabelCondition");
const TCHAR* const szLabelTimesControlName = TEXT("LabelTimes");
const TCHAR* const szLabelValueControlName = TEXT("LabelValue");
const TCHAR* const szLabelDisEnableTipControlName = TEXT("LabelDisEnableTip");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBaseEnsure::CDlgBaseEnsure(): CFGuiDialog(IDD_DLG_POPUP)
{
	//控制变量
	m_bLoadBaseEnsure=false;
	m_bTakeBaseEnsure=false;

	//配置变量
	ZeroMemory(&m_BaseEnsureParameter,sizeof(m_BaseEnsureParameter));

	//设置任务
	m_MissionManager.InsertMissionItem(this);
}

//析构函数
CDlgBaseEnsure::~CDlgBaseEnsure()
{
}


//初始控件
void CDlgBaseEnsure::InitControlUI()
{
	__super::InitControlUI();
}

//消息提醒
void CDlgBaseEnsure::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTakeControlName)==0)
		{
			//领取低保
			return PerformTakeBaseEnsure();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonCheckInControlName)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_CHECKIN,0L);

			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonSpreadControlName)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_MY_SPREAD,0L);
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTaskControlName)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_TASKVIEW,0L);
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonRechargeControlName)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonVipControlName)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_CHECKIN,0L);
			return OnCancel();
		}
	}
}

LPCTSTR CDlgBaseEnsure::GetSkinFile()
{
	return TEXT("DlgBaseEnsure.xml"); 
}

//创建函数
BOOL CDlgBaseEnsure::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("我的低保"));

	//居中窗口
	CenterWindow(this);

	//加载低保
	PerformLoadBaseEnsure();

	return FALSE;
}


//连接事件
bool CDlgBaseEnsure::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//显示提示
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("无法连接到登录服务器，任务信息加载失败，请稍后再试！"),MB_ICONERROR);
	}
	else
	{
		//加载低保
		if(m_bLoadBaseEnsure==true)
		{
			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_LOAD,NULL,0);

			return true;
		}

		//领取低保
		if(m_bTakeBaseEnsure==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_BaseEnsureTake BaseEnsureTake;
			BaseEnsureTake.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(BaseEnsureTake.szMachineID);
			lstrcpyn(BaseEnsureTake.szPassword,pGlobalUserData->szPassword,CountArray(BaseEnsureTake.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_TAKE,&BaseEnsureTake,sizeof(BaseEnsureTake));

			return true;
		}
	}

	return true;
}

//关闭事件
bool CDlgBaseEnsure::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//读取事件
bool CDlgBaseEnsure::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令过滤
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//低保参数
	if(Command.wSubCmdID==SUB_GP_BASEENSURE_PARAMETER)
	{
		//参数校验
		ASSERT(wDataSize==sizeof(CMD_GP_BaseEnsureParamter));
		if(wDataSize!=sizeof(CMD_GP_BaseEnsureParamter)) return false;

		//提取数据
		CMD_GP_BaseEnsureParamter * pBaseEnsureParamter= (CMD_GP_BaseEnsureParamter *)pData;

		//设置变量
		m_BaseEnsureParameter.cbTakeTimes=pBaseEnsureParamter->cbTakeTimes;
		m_BaseEnsureParameter.lScoreAmount=pBaseEnsureParamter->lScoreAmount;
		m_BaseEnsureParameter.lScoreCondition=pBaseEnsureParamter->lScoreCondition;
		
		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();

		UpdateUI();


		return true;
	}

	//低保结果
	if(Command.wSubCmdID==SUB_GP_BASEENSURE_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_BaseEnsureResult));
		if(wDataSize>sizeof(CMD_GP_BaseEnsureResult)) return false;

		//提取数据
		CMD_GP_BaseEnsureResult * pBaseEnsureResult = (CMD_GP_BaseEnsureResult *)pData;

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//执行成功
		if(pBaseEnsureResult->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//设置游戏币
			pGlobalUserData->lUserScore=pBaseEnsureResult->lGameScore;

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//任务终止
		OnMissionConclude();

		//消息提示
		if(pBaseEnsureResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(pBaseEnsureResult->szNotifyContent);
		}

		return true;
	}

	return true;
}

//开始任务
VOID CDlgBaseEnsure::OnMissionStart()
{
	//设置控件
	CControlUI * pControl =	GetControlByName(szButtonTakeControlName);
	if(pControl != NULL)
	{
		pControl->SetEnabled(false);
	}
}

//终止任务
VOID CDlgBaseEnsure::OnMissionConclude()
{
	//设置名字
	CControlUI * pControl =	GetControlByName(szButtonTakeControlName);
	if(pControl != NULL)
	{
		//获取对象
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		//游戏币判断
		if(pGlobalUserData->lUserScore+pGlobalUserData->lUserInsure<m_BaseEnsureParameter.lScoreCondition)
		{
			pControl->SetEnabled();
			CControlUI * pControlLable =	GetControlByName(szLabelLessTipControlName);
			if(pControlLable != NULL)
			{
				pControlLable->SetVisible(true);
			}
			CControlUI * pControlLable2 =	GetControlByName(szLabelDisEnableTipControlName);
			if(pControlLable2 != NULL)
			{
				pControlLable2->SetVisible(false);
			}
		}
		else
		{
			CControlUI * pControlLable =	GetControlByName(szLabelLessTipControlName);
			if(pControlLable != NULL)
			{
				pControlLable->SetVisible(false);
			}
			CControlUI * pControlLable2 =	GetControlByName(szLabelDisEnableTipControlName);
			if(pControlLable2 != NULL)
			{
				pControlLable2->SetVisible(true);
			}
		}
	}
}

//加载低保
VOID CDlgBaseEnsure::PerformLoadBaseEnsure()
{
	//设置变量
	m_bLoadBaseEnsure=true;
	m_bTakeBaseEnsure=false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//领取低保
VOID CDlgBaseEnsure::PerformTakeBaseEnsure()
{
	//设置变量
	m_bLoadBaseEnsure=false;
	m_bTakeBaseEnsure=true;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

void CDlgBaseEnsure::UpdateUI()
{
	CControlUI * pControl = NULL;
	//领取条件
	TCHAR szTakeCondition[128]=TEXT("");
	_sntprintf(szTakeCondition,CountArray(szTakeCondition),TEXT("游戏币低于 %I64d "),m_BaseEnsureParameter.lScoreCondition);
	pControl =	GetControlByName(szLabelConditionControlName);
	if(pControl != NULL)
	{
		pControl->SetText(szTakeCondition);
		pControl = NULL;
	}

	//领取次数
	TCHAR szTakeTimes[128]=TEXT("");
	_sntprintf(szTakeTimes,CountArray(szTakeTimes),TEXT("%d 次/天"),m_BaseEnsureParameter.cbTakeTimes);
	pControl =	GetControlByName(szLabelTimesControlName);
	if(pControl != NULL)
	{
		pControl->SetText(szTakeTimes);
		pControl = NULL;
	}

	//领取金额
	TCHAR szTakeValue[128]=TEXT("");
	_sntprintf(szTakeValue,CountArray(szTakeValue),TEXT("%I64d 游戏币"),m_BaseEnsureParameter.lScoreAmount);
	pControl =	GetControlByName(szLabelValueControlName);
	if(pControl != NULL)
	{
		pControl->SetText(szTakeValue);
		pControl = NULL;
	}

}

VOID CDlgBaseEnsure::OnOK()
{

}

VOID CDlgBaseEnsure::OnCancel()
{
	return __super::OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////

CDlgBaseEnsureServer::CDlgBaseEnsureServer()
{
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
}

CDlgBaseEnsureServer::~CDlgBaseEnsureServer()
{

}

VOID CDlgBaseEnsureServer::PerformLoadBaseEnsure()
{
	//开始任务
	OnMissionStart();
	//发送数据
	m_pITCPSocket->SendData(MDM_GR_BASEENSURE,SUB_GR_BASEENSURE_LOAD,NULL,0);

}

VOID CDlgBaseEnsureServer::PerformTakeBaseEnsure()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_BaseEnsureTake BaseEnsureTake;
	BaseEnsureTake.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(BaseEnsureTake.szMachineID);
	lstrcpyn(BaseEnsureTake.szPassword,pGlobalUserData->szPassword,CountArray(BaseEnsureTake.szPassword));

	//开始任务
	OnMissionStart();

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_BASEENSURE,SUB_GR_BASEENSURE_TAKE,&BaseEnsureTake,sizeof(BaseEnsureTake));

}

VOID CDlgBaseEnsureServer::Initialization( ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem )
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

bool CDlgBaseEnsureServer::OnServerBaseEnsureMessage( WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//消息判断
	if (wMainCmdID==MDM_GR_BASEENSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_BASEENSURE_PARAMETER:		
			{
				//参数校验
				ASSERT(wDataSize==sizeof(CMD_GR_BaseEnsureParamter));
				if(wDataSize!=sizeof(CMD_GR_BaseEnsureParamter)) return false;

				//提取数据
				CMD_GR_BaseEnsureParamter * pBaseEnsureParamter= (CMD_GR_BaseEnsureParamter *)pData;

				//设置变量
				m_BaseEnsureParameter.cbTakeTimes=pBaseEnsureParamter->cbTakeTimes;
				m_BaseEnsureParameter.lScoreAmount=pBaseEnsureParamter->lScoreAmount;
				m_BaseEnsureParameter.lScoreCondition=pBaseEnsureParamter->lScoreCondition;

				//任务终止
				OnMissionConclude();

				UpdateUI();

				return true;
			}
		case SUB_GR_BASEENSURE_RESULT:		
			{
				//参数校验
				ASSERT(wDataSize<=sizeof(CMD_GR_BaseEnsureResult));
				if(wDataSize>sizeof(CMD_GR_BaseEnsureResult)) return false;

				//提取数据
				CMD_GR_BaseEnsureResult * pBaseEnsureResult = (CMD_GR_BaseEnsureResult *)pData;

				//执行成功
				if(pBaseEnsureResult->bSuccessed==true)
				{
					//获取对象
					CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

					//设置游戏币
					pGlobalUserData->lUserScore=pBaseEnsureResult->lGameScore;

					//发送事件
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
				}

				//任务终止
				OnMissionConclude();

				//消息提示
				if(pBaseEnsureResult->szNotifyContent[0]!=0)
				{
					CDlgInformation Information;
					Information.ShowMessageBox(pBaseEnsureResult->szNotifyContent);
				}

				return true;
			}
		}
	}
	return true;
}
