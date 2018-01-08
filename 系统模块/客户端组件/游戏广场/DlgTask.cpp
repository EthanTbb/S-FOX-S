#include "Stdafx.h"
#include "Resource.h"
#include "DlgTask.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "DlgTaskDetails.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");


const TCHAR* const szButtonViewReceivingControlName = TEXT("ButtonViewReceiving");
const TCHAR* const szButtonViewReceivedControlName = TEXT("ButtonViewReceived");
const TCHAR* const szButtonViewCompletedControlName = TEXT("ButtonViewCompleted");

//容器控件
const TCHAR* const szPageLayoutReceivingControlName = TEXT("PageLayoutReceiving");
const TCHAR* const szPageLayoutReceivedControlName = TEXT("PageLayoutReceived");
const TCHAR* const szPageLayoutCompletedControlName = TEXT("PageLayoutCompleted");

//VIP控件
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVipAward");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelAwardMemberControlName = TEXT("LabelAwardMember");

const TCHAR* const szLableReceivingTypeNormalControlName = TEXT("LableReceivingTypeNormal");
const TCHAR* const szLableReceivingTypeVipControlName = TEXT("LableReceivingTypeVip");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTask::CDlgTask() : CFGuiDialog(IDD_DLG_POPUP)
{
	//任务标识
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=false;
	m_bTaskGiveUp =false;

	//设置任务
	m_MissionManager.InsertMissionItem(this);
}

//析构函数
CDlgTask::~CDlgTask()
{
		//变量定义
	WORD wKey=0;
	POSITION Position;

	//删除参数	
	tagTaskParameter * pTaskParameter=NULL;
	Position=m_TaskParameterMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskParameterMap.GetNextAssoc(Position,wKey,pTaskParameter);
		SafeDelete(pTaskParameter);
	}
	m_TaskParameterMap.RemoveAll();

	//删除状态
	tagTaskStatus * pTaskStatus=NULL;
	Position=m_TaskStatusMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskStatusMap.GetNextAssoc(Position,wKey,pTaskStatus);
		SafeDelete(pTaskStatus);
	}
	m_TaskStatusMap.RemoveAll();
}

//创建函数
BOOL CDlgTask::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("我的任务"));

	//居中窗口
	CenterWindow(this);

	//加载任务
	PerformLoadTask();

	return TRUE;
}

//确定函数
VOID CDlgTask::OnOK()
{

	//return __super::OnOK();
}

//取消消息
VOID CDlgTask::OnCancel()
{
	return __super::OnCancel();
}

//初始控件
void CDlgTask::InitControlUI()
{
}
//消息提醒
void CDlgTask::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}

		else if(lstrcmp(pControlUI->GetName(), szButtonViewReceivingControlName)==0)
		{
			return ViewTaskReceiving(pControlUI);
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonViewReceivedControlName)==0)
		{
			return ViewTaskReceived(pControlUI);
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonViewCompletedControlName)==0)
		{
			return ViewTaskCompleted(pControlUI);
		}
	}	
	return;
}

LPCTSTR CDlgTask::GetSkinFile()
{
	return TEXT("DlgTask.xml"); 
}

//连接事件
bool CDlgTask::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//显示提示
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("无法连接到登录服务器，任务信息加载失败，请稍后再试！"),MB_ICONERROR);
	}
	else
	{
		//加载任务
		if(m_bLoadTask==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskLoadInfo TaskLoadInfo;
			TaskLoadInfo.dwUserID = pGlobalUserData->dwUserID;
			lstrcpyn(TaskLoadInfo.szPassword,pGlobalUserData->szPassword,CountArray(TaskLoadInfo.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_LOAD,&TaskLoadInfo,sizeof(TaskLoadInfo));

			return true;
		}

		//领取奖励
		if(m_bTakeTask==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskTake TaskTake;
			TaskTake.wTaskID = m_wTaskID;
			TaskTake.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskTake.szMachineID);
			lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_TAKE,&TaskTake,sizeof(TaskTake));

			return true;
		}

		//领取奖励
		if(m_bTaskReward==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskReward TaskReward;
			TaskReward.wTaskID = m_wTaskID;
			TaskReward.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskReward.szMachineID);
			lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_REWARD,&TaskReward,sizeof(TaskReward));

			return true;
		}

		//放弃任务
		if(m_bTaskGiveUp == true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskGiveUp TaskTake;
			TaskTake.wTaskID = m_wTaskID;
			TaskTake.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskTake.szMachineID);
			lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_GIVEUP,&TaskTake,sizeof(TaskTake));
		}
	}
	return true;
}

//关闭事件
bool CDlgTask::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//读取事件
bool CDlgTask::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令过滤
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//任务信息
	if(Command.wSubCmdID==SUB_GP_TASK_INFO)
	{
		//提取数据
		CMD_GP_TaskInfo * pTaskInfo = (CMD_GP_TaskInfo *)pData;

		//参数校验
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//添加任务
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;			
		}
		
		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		LoadDataFromMemory();
		UpdateUI();

		//任务终止
		OnMissionConclude();



		return true;
	}

    //任务列表
	if(Command.wSubCmdID==SUB_GP_TASK_LIST)
	{
		//参数校验
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//变量定义
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//任务数量
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//解析数据
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//添加任务
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//偏移指针
			pDataBuffer += wParameterSize;			
		}

		return true;
	}

	//任务结果
	if(Command.wSubCmdID==SUB_GP_TASK_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_TaskResult));
		if(wDataSize>sizeof(CMD_GP_TaskResult)) return false;

		//提取数据
		CMD_GP_TaskResult * pTaskResult = (CMD_GP_TaskResult *)pData;

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);



		//执行成功
		if(pTaskResult->bSuccessed==true)
		{
			//放弃任务
			if(pTaskResult->wCommandID==SUB_GP_TASK_GIVEUP)
			{
				//设置状态	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//添加任务
				DeleteMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();

			}
			//领取任务
			if(pTaskResult->wCommandID==SUB_GP_TASK_TAKE)
			{
				//设置状态	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//添加任务
				AddMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();

			}

			//领取奖励
			if(pTaskResult->wCommandID==SUB_GP_TASK_REWARD)
			{
				//查询对象
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);
				pTaskStatus->cbTaskStatus = TASK_STATUS_REWARD;

				//更新状态
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					LoadDataFromMemory();
					UpdateUI();
				}

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//更新财富
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}
		}

		//消息提示
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(pTaskResult->szNotifyContent);
		}

		//任务终止
		OnMissionConclude();


		return true;
	}

	return true;
}
//开始任务
VOID CDlgTask::OnMissionStart()
{

}

//终止任务
VOID CDlgTask::OnMissionConclude()
{


}

//加载任务
VOID CDlgTask::PerformLoadTask()
{
	//设置变量
	m_bLoadTask=true;
	m_bTakeTask=false;
	m_bTaskReward=false;
	m_bTaskGiveUp =false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//领取任务
VOID CDlgTask::PerformTakeTask()
{
	//设置变量
	m_bLoadTask=false;
	m_bTakeTask=true;
	m_bTaskReward=false;
	m_bTaskGiveUp =false;

	//设置变量
	m_wTaskID=m_pTaskParameter->wTaskID;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}


//领取奖励
VOID CDlgTask::PerformRewardTask()
{
	//设置变量
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=true;
	m_bTaskGiveUp =false;

	//设置变量
	m_wTaskID=m_pTaskStatus->wTaskID;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}


//放弃任务
VOID CDlgTask::PerformGiveUpTask()
{
	//设置变量
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=false;
	m_bTaskGiveUp =true;

	//设置变量
	m_wTaskID=m_pTaskStatus->wTaskID;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//添加任务
BOOL CDlgTask::AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize)
{
	//参数校验
	ASSERT(pTaskParameterData!=NULL && wParameterSize>0);
	if(pTaskParameterData==NULL || wParameterSize==0) return FALSE;

		//变量定义
	tagTaskParameter * pTaskParameter=NULL;

	//申请资源
	try
	{
		pTaskParameter = new tagTaskParameter;
		if(pTaskParameter==NULL) throw(TEXT("内存不足！"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//拷贝数据
	CopyMemory(pTaskParameter,pTaskParameterData,wParameterSize);
	m_TaskParameterMap[pTaskParameter->wTaskID]=pTaskParameter;

	return true;
}

BOOL CDlgTask::DeleteMineTaskItem( tagTaskStatus & TaskStatus )
{

	//变量定义
	tagTaskStatus * pTaskStatus=NULL;

	if(m_TaskStatusMap.Lookup(TaskStatus.wTaskID,   pTaskStatus))   
	{   
		m_TaskStatusMap.RemoveKey(TaskStatus.wTaskID);   
		delete   pTaskStatus;
		pTaskStatus=NULL;   
	}

	return TRUE;
}

//添加任务
BOOL CDlgTask::AddMineTaskItem(tagTaskStatus & TaskStatus)
{
	//变量定义
	tagTaskStatus * pTaskStatus=NULL;

	//申请资源
	try
	{
		pTaskStatus = new tagTaskStatus;
		if(pTaskStatus==NULL) throw(TEXT("内存不足！"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//拷贝数据
	CopyMemory(pTaskStatus,&TaskStatus,sizeof(tagTaskStatus));
	m_TaskStatusMap[pTaskStatus->wTaskID]=pTaskStatus;

	return TRUE;
}

VOID CDlgTask::LoadDataFromMemory()
{
	m_vecReceiving.clear();
	m_vecReceived.clear();
	m_vecCompleted.clear();
	m_vecFail.clear();
	m_vecAward.clear();
	POSITION   pos   =   m_TaskParameterMap.GetStartPosition();   
	while(pos   !=   NULL)   
	{
		WORD  nTaskID = 0;   
		tagTaskParameter * pTaskParameter = NULL; 
		m_TaskParameterMap.GetNextAssoc(pos,nTaskID,pTaskParameter);  

		//查找参数
		tagTaskStatus * pTaskStatus=NULL;
		if(m_TaskStatusMap.Lookup(nTaskID,pTaskStatus)==TRUE)
		{
			if(pTaskStatus->cbTaskStatus==TASK_STATUS_UNFINISH)
			{
				m_vecReceived.push_back(pTaskParameter);
			}
			else if(pTaskStatus->cbTaskStatus==TASK_STATUS_SUCCESS)
			{
				m_vecCompleted.push_back(pTaskParameter);
			}
			else if(pTaskStatus->cbTaskStatus==TASK_STATUS_FAILED)
			{
				m_vecFail.push_back(pTaskParameter);
			}
			else if(pTaskStatus->cbTaskStatus == TASK_STATUS_REWARD)
			{
                m_vecAward.push_back(pTaskParameter);
			}
		}
		else
		{
			m_vecReceiving.push_back(pTaskParameter);
		}
	}

}
VOID CDlgTask::AddTask(CPageLayoutUI * page,tagTaskParameter * pTaskParameter)
{
	//新增任务
	CDialogBuilder builderPage;
	CContainerUI* itme =NULL;
	//查找参数
	tagTaskStatus * pTaskStatus=NULL;
	if(m_TaskStatusMap.Lookup(pTaskParameter->wTaskID,pTaskStatus)==TRUE)
	{
		if(pTaskStatus->cbTaskStatus==TASK_STATUS_UNFINISH)
		{
			itme = static_cast<CContainerUI*>(builderPage.Create(_T("DlgTaskItemReceived.xml"), (UINT)0));
		}
		else if(pTaskStatus->cbTaskStatus==TASK_STATUS_SUCCESS)
		{
			itme = static_cast<CContainerUI*>(builderPage.Create( _T("DlgTaskItemCompleted.xml"), (UINT)0));
		}
		else if(pTaskStatus->cbTaskStatus == TASK_STATUS_FAILED)
		{
			itme = static_cast<CContainerUI*>(builderPage.Create( _T("DlgTaskItemFail.xml"), (UINT)0));
		}
		else if(pTaskStatus->cbTaskStatus == TASK_STATUS_REWARD)
		{
			itme = static_cast<CContainerUI*>(builderPage.Create( _T("DlgTaskItemAwarded.xml"), (UINT)0));
		}
	}
	else
	{
		itme = static_cast<CContainerUI*>(builderPage.Create( _T("DlgTaskItemReceiving.xml"), (UINT)0));
	}

	if(itme==NULL)
	{
		return;

	}

	page->Add(itme);
	itme->SetTag((UINT_PTR)pTaskParameter);

	//设置名字
	CControlUI * pControl =	itme->FindControl(_TEXT("LableName"));
	if(pControl !=NULL) 	pControl->SetText(pTaskParameter->szTaskName);

	//设置摘要
	CControlUI * pControl2 = itme->FindControl(_TEXT("TextSummary"));
	if(pControl2 !=NULL)  pControl2->SetText(pTaskParameter->szTaskDescribe);

	//设置奖励
	CControlUI * pControl3 = itme->FindControl(_TEXT("TextAward"));
	TCHAR szTaskReward[64]=TEXT("");
	if(pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币+%I64d元宝"),pTaskParameter->lStandardAwardGold,pTaskParameter->lStandardAwardMedal);
	}
	else if( pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币"),pTaskParameter->lStandardAwardGold);
	}
	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d元宝"),pTaskParameter->lStandardAwardMedal);
	}
	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("无"));
	}
	if(pControl3 !=NULL)  pControl3->SetText(szTaskReward);


	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	TCHAR szTaskRewardTotal[64]=TEXT("");
	if(pGlobalUserData->cbMemberOrder > 0)
	{
		//会员任务
		const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
		if(pMemberParameter != NULL)
		{
			_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("奖励+%d%%"),pMemberParameter->dwMemberTask);
		}

		CControlUI * pContainerVip = itme->FindControl(szContainerVipAwardControlName);
		if(pContainerVip!=NULL)
		{
			//会员等级
			for (int i=0;i<6;i++)
			{
				CStdString strLevel;
				strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
				CControlUI * pLabelVipUI = pContainerVip->FindControl(strLevel);
				if(pLabelVipUI)
				{
					if (i==pGlobalUserData->cbMemberOrder)
					{
						pLabelVipUI->SetVisible(true);
					}
					else
					{
						pLabelVipUI->SetVisible(false);
					}
				}
			}

			CControlUI * pControl4 = pContainerVip->FindControl(szLabelAwardMemberControlName);
			if(pControl4 !=NULL) 
			{
				pControl4->SetVisible(true);
				pControl4->SetText(szTaskRewardTotal);
			}
		}

	}
	else
	{
		CControlUI * pContainerVip = itme->FindControl(szContainerVipAwardControlName);
		if(pContainerVip !=NULL)  pContainerVip->SetVisible(false);
	}


	CControlUI * pLableReceivingTypeNormal = itme->FindControl(szLableReceivingTypeNormalControlName);
	if(pLableReceivingTypeNormal !=NULL)  pLableReceivingTypeNormal->SetVisible(pTaskParameter->cbPlayerType&0x1);

	CControlUI * pLableszLableReceivingTypeVip = itme->FindControl(szLableReceivingTypeVipControlName);
	if(pLableszLableReceivingTypeVip !=NULL)  pLableszLableReceivingTypeVip->SetVisible(pTaskParameter->cbPlayerType&0x2 && !(pTaskParameter->cbPlayerType&0x1));




	////用户信息
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//TCHAR szTaskReward[64]=TEXT("");
	//if(pGlobalUserData->cbMemberOrder > 0)
	//{
	//	
	//	if(pTaskParameter->lMemberAwardGold > 0 && pTaskParameter->lMemberAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币+%I64d元宝"),pTaskParameter->lMemberAwardGold,pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if( pTaskParameter->lMemberAwardGold > 0 && pTaskParameter->lMemberAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币"),pTaskParameter->lMemberAwardGold);
	//	}
	//	else if(pTaskParameter->lMemberAwardGold <= 0 && pTaskParameter->lMemberAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d元宝"),pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if(pTaskParameter->lMemberAwardGold <= 0 && pTaskParameter->lMemberAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("无"));
	//	}
	//}
	//else
	//{
	//	if(pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币+%I64d元宝"),pTaskParameter->lMemberAwardGold,pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if( pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币"),pTaskParameter->lMemberAwardGold);
	//	}
	//	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d元宝"),pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("无"));
	//	}

	//}
	//pControl3->SetText(szTaskReward);
}

//查看任务
VOID CDlgTask::ViewTaskReceiving(CControlUI * pControlUI)
{
	tagTaskParameter * pTaskParameter = (tagTaskParameter *)(pControlUI->GetParent()->GetTag());
	if(pTaskParameter!=NULL)
	{
		//设置变量
		m_pTaskParameter=pTaskParameter;

		//查找对象
		m_pTaskStatus=NULL;
		m_TaskStatusMap.Lookup(m_pTaskParameter->wTaskID,m_pTaskStatus);


		CDlgTaskDetails dlg;
		dlg.SetTaskParameter(m_pTaskParameter,m_pTaskStatus);
		INT_PTR nResult=dlg.DoModal();
		if(IDOK == nResult)
		{
			PerformTakeTask();
		}
	}
}

//查看任务
VOID CDlgTask::ViewTaskReceived(CControlUI * pControlUI)
{
	tagTaskParameter * pTaskParameter = (tagTaskParameter *)(pControlUI->GetParent()->GetTag());
	if(pTaskParameter!=NULL)
	{
		//设置变量
		m_pTaskParameter=pTaskParameter;

		//查找对象
		m_pTaskStatus=NULL;
		m_TaskStatusMap.Lookup(m_pTaskParameter->wTaskID,m_pTaskStatus);


		CDlgTaskDetails dlg;
		dlg.SetTaskParameter(m_pTaskParameter,m_pTaskStatus);
		INT_PTR nResult=dlg.DoModal();
		if(IDOK == nResult)
		{

			PerformGiveUpTask();
		}
	}
}

//查看任务
VOID CDlgTask::ViewTaskCompleted(CControlUI * pControlUI)
{
	tagTaskParameter * pTaskParameter = (tagTaskParameter *)(pControlUI->GetParent()->GetTag());
	if(pTaskParameter!=NULL)
	{
		//设置变量
		m_pTaskParameter=pTaskParameter;

		//查找对象
		m_pTaskStatus=NULL;
		m_TaskStatusMap.Lookup(m_pTaskParameter->wTaskID,m_pTaskStatus);


		CDlgTaskDetails dlg;
		dlg.SetTaskParameter(m_pTaskParameter,m_pTaskStatus);
		INT_PTR nResult=dlg.DoModal();
		if(IDOK == nResult)
		{
			PerformRewardTask();
		}
	}
}

//刷新UI
VOID CDlgTask::UpdateUI()
{
	CPageLayoutUI * pPageLayoutReceiving = (CPageLayoutUI * )GetControlByName(szPageLayoutReceivingControlName);
	if (pPageLayoutReceiving !=NULL)
	{
		pPageLayoutReceiving->RemoveAll();
		int n = (int)m_vecReceiving.size();
		for(int i=0;i<n;i++)
		{
			AddTask(pPageLayoutReceiving,m_vecReceiving[i]);
		}
	}
	CPageLayoutUI * pPageLayoutReceived = (CPageLayoutUI * )GetControlByName(szPageLayoutReceivedControlName);
	if (pPageLayoutReceived !=NULL)
	{
		pPageLayoutReceived->RemoveAll();
		int n = (int)m_vecReceived.size();
		for(int i=0;i<n;i++)
		{
			AddTask(pPageLayoutReceived,m_vecReceived[i]);
		}
	}

	CPageLayoutUI * pPageLayoutCompleted = (CPageLayoutUI * )GetControlByName(szPageLayoutCompletedControlName);
	if (pPageLayoutCompleted !=NULL)
	{
		pPageLayoutCompleted->RemoveAll();
		int n = (int)m_vecCompleted.size();
		for(int i=0;i<n;i++)
		{
			AddTask(pPageLayoutCompleted,m_vecCompleted[i]);
		}

		n = (int)m_vecFail.size();
		for(int i=0;i<n;i++)
		{
			AddTask(pPageLayoutCompleted,m_vecFail[i]);
		}

		n = (int)m_vecAward.size();
		for(int i=0;i<n;i++)
		{
			AddTask(pPageLayoutCompleted,m_vecAward[i]);
		}
	}

}



//////////////////////////////////////////////////////////////////////////////////
CDlgTaskServer::CDlgTaskServer()
{
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
}
CDlgTaskServer::~CDlgTaskServer()
{

}

VOID CDlgTaskServer::PerformLoadTask()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_C_LoadTaskInfo LoadTaskInfo;
	LoadTaskInfo.dwUserID = pGlobalUserData->dwUserID;
	lstrcpyn(LoadTaskInfo.szPassword,pGlobalUserData->szPassword,CountArray(LoadTaskInfo.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_LOAD_INFO,&LoadTaskInfo,sizeof(LoadTaskInfo));

	//开始任务
	OnMissionStart();
}

VOID CDlgTaskServer::PerformTakeTask()
{
	//设置变量
	m_wTaskID=m_pTaskParameter->wTaskID;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_C_TakeTask TaskTake;
	TaskTake.wTaskID = m_wTaskID;
	TaskTake.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskTake.szMachineID);
	lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_TAKE,&TaskTake,sizeof(TaskTake));

	//开始任务
	OnMissionStart();
}


VOID CDlgTaskServer::PerformGiveUpTask()
{
	//设置变量
	m_wTaskID=m_pTaskParameter->wTaskID;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_C_TakeGiveUp TaskTake;
	TaskTake.wTaskID = m_wTaskID;
	TaskTake.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskTake.szMachineID);
	lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_GIVEUP,&TaskTake,sizeof(TaskTake));

	//开始任务
	OnMissionStart();
}

VOID CDlgTaskServer::PerformRewardTask()
{
	//设置变量
	m_wTaskID=m_pTaskStatus->wTaskID;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GP_TaskReward TaskReward;
	TaskReward.wTaskID = m_wTaskID;
	TaskReward.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskReward.szMachineID);
	lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_REWARD,&TaskReward,sizeof(TaskReward));

	//开始任务
	OnMissionStart();
}

VOID CDlgTaskServer::Initialization( ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem )
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

bool CDlgTaskServer::OnTaskMessage( WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//任务信息
	if(wSubCmdID==SUB_GR_TASK_INFO)
	{
		//提取数据
		CMD_GR_S_TaskInfo * pTaskInfo = (CMD_GR_S_TaskInfo *)pData;

		//参数校验
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//添加任务
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;
		}

		LoadDataFromMemory();
		UpdateUI();

		//任务终止
		OnMissionConclude();


		return true;
	}

	//任务信息
	if(wSubCmdID==SUB_GR_TASK_LIST)
	{
		//参数校验
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//变量定义
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//任务数量
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//解析数据
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//添加任务
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//偏移指针
			pDataBuffer += wParameterSize;		
		}

		return true;
	}

	//执行结果
	if(wSubCmdID==SUB_GR_TASK_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskResult));
		if(wDataSize>sizeof(CMD_GR_S_TaskResult)) return false;

		//提取数据
		CMD_GR_S_TaskResult * pTaskResult = (CMD_GR_S_TaskResult *)pData;

		//执行成功
		if(pTaskResult->bSuccessed==true)
		{
			//放弃任务
			if(pTaskResult->wCommandID==SUB_GR_TASK_GIVEUP)
			{
				//设置状态	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//添加任务
				DeleteMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();

			}

			//领取任务
			if(pTaskResult->wCommandID==SUB_GR_TASK_TAKE)
			{
				//设置状态	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//添加任务
				AddMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();
			}

			//领取奖励
			if(pTaskResult->wCommandID==SUB_GR_TASK_REWARD)
			{
				//查询对象
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);

				//更新状态
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					LoadDataFromMemory();
					UpdateUI();
				}

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//更新财富
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}					
		}

		//消息提示
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(pTaskResult->szNotifyContent);
		}

		//任务终止
		OnMissionConclude();

		return true;
	}

	return true;
}

