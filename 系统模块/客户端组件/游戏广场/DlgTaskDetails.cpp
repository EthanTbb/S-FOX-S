#include "Stdafx.h"
#include "Resource.h"
#include "DlgTaskDetails.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonRecevingControlName = TEXT("ButtonReceving");
const TCHAR* const szButtonGiveUpControlName = TEXT("ButtonGiveUp");
const TCHAR* const szButtonGetControlName = TEXT("ButtonGet");

//标签控件
const TCHAR* const szLabelNameControlName = TEXT("LabelName");
const TCHAR* const szLabelProgressControlName = TEXT("LabelProgress");
const TCHAR* const szLabelTimeControlName = TEXT("LabelTime");
const TCHAR* const szLabelSummaryControlName = TEXT("LabelSummary");
const TCHAR* const szLabelAwardControlName = TEXT("LabelAward");

//VIP控件
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVipAward");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelAwardMemberControlName = TEXT("LabelAwardMember");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTaskDetails ::CDlgTaskDetails () : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pTaskParameter = NULL;
	m_pTaskStatus =NULL;
}

//析构函数
CDlgTaskDetails ::~CDlgTaskDetails ()
{
}

//创建函数
BOOL CDlgTaskDetails ::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("游戏帮助"));

	//居中窗口
	CenterWindow(this);

	UpdateDetails();

	return TRUE;
}

//确定函数
VOID CDlgTaskDetails ::OnOK()
{

	return __super::OnOK();
}

//取消消息
VOID CDlgTaskDetails ::OnCancel()
{
	return __super::OnCancel();
}

//消息提醒
void CDlgTaskDetails ::Notify(TNotifyUI &  msg)
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
		else if(lstrcmp(pControlUI->GetName(), szButtonRecevingControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGiveUpControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGetControlName)==0)
		{
			return OnOK();
		}
	}
	
	return;
}

LPCTSTR CDlgTaskDetails::GetSkinFile()
{
	return TEXT("DlgTaskDetails.xml"); 
}

void CDlgTaskDetails::SetTaskParameter(tagTaskParameter * pTaskParameter,tagTaskStatus * pTaskStatus)
{
	m_pTaskParameter = pTaskParameter;
	m_pTaskStatus = pTaskStatus;
}

void CDlgTaskDetails::UpdateDetails()
{
	if(NULL == m_pTaskParameter)
	{
		return;
	}

	//设置名字
	CControlUI * pControl =	GetControlByName(szLabelNameControlName);
	if(pControl) pControl->SetText(m_pTaskParameter->szTaskName);

	//设置进度
	TCHAR szTaskProgress[32]=TEXT("");
	_sntprintf(szTaskProgress,CountArray(szTaskProgress),TEXT("%d/%d"),(m_pTaskStatus?m_pTaskStatus->wTaskProgress:0),m_pTaskParameter->wTaskObject);
	pControl =	GetControlByName(szLabelProgressControlName);
	if(pControl) pControl->SetText(szTaskProgress);

	//设置时间
	TCHAR szTaskTimeLimit[128]=TEXT("");
	WORD wHour = (WORD)(m_pTaskParameter->dwTimeLimit/3600);
	WORD wMinute = (WORD)((m_pTaskParameter->dwTimeLimit-wHour*3600)/60);
	WORD wSecond = (WORD)(m_pTaskParameter->dwTimeLimit-wHour*3600-wMinute*60);
	_sntprintf(szTaskTimeLimit,CountArray(szTaskTimeLimit),TEXT("%02d时:%02d分:%02d秒"),wHour,wMinute,wSecond);
	pControl =	GetControlByName(szLabelTimeControlName);
	if(pControl) pControl->SetText(szTaskTimeLimit);


	//设置说明
	pControl = GetControlByName(szLabelSummaryControlName);
	if(pControl) pControl->SetText(m_pTaskParameter->szTaskDescribe);

	//设置奖励
	TCHAR szTaskReward[64]=TEXT("");
	if(m_pTaskParameter->lStandardAwardGold > 0 && m_pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币+%I64d元宝"),m_pTaskParameter->lStandardAwardGold,m_pTaskParameter->lStandardAwardMedal);
	}
	else if( m_pTaskParameter->lStandardAwardGold > 0 && m_pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d游戏币"),m_pTaskParameter->lStandardAwardGold);
	}
	else if(m_pTaskParameter->lStandardAwardGold <= 0 && m_pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d元宝"),m_pTaskParameter->lStandardAwardMedal);
	}
	else if(m_pTaskParameter->lStandardAwardGold <= 0 && m_pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("无"));
	}
	pControl = GetControlByName(szLabelAwardControlName);
	if(pControl) pControl->SetText(szTaskReward);

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

		CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
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
		CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
		if(pContainerVip !=NULL)  pContainerVip->SetVisible(false);
	}


	pControl = GetControlByName(szButtonRecevingControlName);
	if(pControl) pControl->SetVisible(false);
	pControl = GetControlByName(szButtonGiveUpControlName);
	if(pControl) pControl->SetVisible(false);
	pControl = GetControlByName(szButtonGetControlName);
	if(pControl) pControl->SetVisible(false);

	if(NULL != m_pTaskStatus)
	{
		if(m_pTaskStatus->cbTaskStatus==TASK_STATUS_UNFINISH)
		{
			pControl = GetControlByName(szButtonGiveUpControlName);
			if(pControl) pControl->SetVisible(true);
		}
		else if(m_pTaskStatus->cbTaskStatus==TASK_STATUS_SUCCESS)
		{
			pControl = GetControlByName(szButtonGetControlName);
			if(pControl) pControl->SetVisible(true);
		}
	}
	else
	{
		pControl = GetControlByName(szButtonRecevingControlName);
		if(pControl) pControl->SetVisible(true);
	}	
}
//////////////////////////////////////////////////////////////////////////////////