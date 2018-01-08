#include "Stdafx.h"
#include "Resource.h"
#include "DlgTask.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "DlgTaskDetails.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");


const TCHAR* const szButtonViewReceivingControlName = TEXT("ButtonViewReceiving");
const TCHAR* const szButtonViewReceivedControlName = TEXT("ButtonViewReceived");
const TCHAR* const szButtonViewCompletedControlName = TEXT("ButtonViewCompleted");

//�����ؼ�
const TCHAR* const szPageLayoutReceivingControlName = TEXT("PageLayoutReceiving");
const TCHAR* const szPageLayoutReceivedControlName = TEXT("PageLayoutReceived");
const TCHAR* const szPageLayoutCompletedControlName = TEXT("PageLayoutCompleted");

//VIP�ؼ�
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVipAward");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelAwardMemberControlName = TEXT("LabelAwardMember");

const TCHAR* const szLableReceivingTypeNormalControlName = TEXT("LableReceivingTypeNormal");
const TCHAR* const szLableReceivingTypeVipControlName = TEXT("LableReceivingTypeVip");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTask::CDlgTask() : CFGuiDialog(IDD_DLG_POPUP)
{
	//�����ʶ
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=false;
	m_bTaskGiveUp =false;

	//��������
	m_MissionManager.InsertMissionItem(this);
}

//��������
CDlgTask::~CDlgTask()
{
		//��������
	WORD wKey=0;
	POSITION Position;

	//ɾ������	
	tagTaskParameter * pTaskParameter=NULL;
	Position=m_TaskParameterMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskParameterMap.GetNextAssoc(Position,wKey,pTaskParameter);
		SafeDelete(pTaskParameter);
	}
	m_TaskParameterMap.RemoveAll();

	//ɾ��״̬
	tagTaskStatus * pTaskStatus=NULL;
	Position=m_TaskStatusMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskStatusMap.GetNextAssoc(Position,wKey,pTaskStatus);
		SafeDelete(pTaskStatus);
	}
	m_TaskStatusMap.RemoveAll();
}

//��������
BOOL CDlgTask::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�ҵ�����"));

	//���д���
	CenterWindow(this);

	//��������
	PerformLoadTask();

	return TRUE;
}

//ȷ������
VOID CDlgTask::OnOK()
{

	//return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgTask::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ�ؼ�
void CDlgTask::InitControlUI()
{
}
//��Ϣ����
void CDlgTask::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
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

//�����¼�
bool CDlgTask::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//��ʾ��ʾ
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("�޷����ӵ���¼��������������Ϣ����ʧ�ܣ����Ժ����ԣ�"),MB_ICONERROR);
	}
	else
	{
		//��������
		if(m_bLoadTask==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskLoadInfo TaskLoadInfo;
			TaskLoadInfo.dwUserID = pGlobalUserData->dwUserID;
			lstrcpyn(TaskLoadInfo.szPassword,pGlobalUserData->szPassword,CountArray(TaskLoadInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_LOAD,&TaskLoadInfo,sizeof(TaskLoadInfo));

			return true;
		}

		//��ȡ����
		if(m_bTakeTask==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskTake TaskTake;
			TaskTake.wTaskID = m_wTaskID;
			TaskTake.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskTake.szMachineID);
			lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_TAKE,&TaskTake,sizeof(TaskTake));

			return true;
		}

		//��ȡ����
		if(m_bTaskReward==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskReward TaskReward;
			TaskReward.wTaskID = m_wTaskID;
			TaskReward.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskReward.szMachineID);
			lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_REWARD,&TaskReward,sizeof(TaskReward));

			return true;
		}

		//��������
		if(m_bTaskGiveUp == true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskGiveUp TaskTake;
			TaskTake.wTaskID = m_wTaskID;
			TaskTake.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskTake.szMachineID);
			lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_GIVEUP,&TaskTake,sizeof(TaskTake));
		}
	}
	return true;
}

//�ر��¼�
bool CDlgTask::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//��ȡ�¼�
bool CDlgTask::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�������
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//������Ϣ
	if(Command.wSubCmdID==SUB_GP_TASK_INFO)
	{
		//��ȡ����
		CMD_GP_TaskInfo * pTaskInfo = (CMD_GP_TaskInfo *)pData;

		//����У��
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//�������
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;			
		}
		
		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		LoadDataFromMemory();
		UpdateUI();

		//������ֹ
		OnMissionConclude();



		return true;
	}

    //�����б�
	if(Command.wSubCmdID==SUB_GP_TASK_LIST)
	{
		//����У��
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//��������
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//��������
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//��������
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//�������
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//ƫ��ָ��
			pDataBuffer += wParameterSize;			
		}

		return true;
	}

	//������
	if(Command.wSubCmdID==SUB_GP_TASK_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_TaskResult));
		if(wDataSize>sizeof(CMD_GP_TaskResult)) return false;

		//��ȡ����
		CMD_GP_TaskResult * pTaskResult = (CMD_GP_TaskResult *)pData;

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);



		//ִ�гɹ�
		if(pTaskResult->bSuccessed==true)
		{
			//��������
			if(pTaskResult->wCommandID==SUB_GP_TASK_GIVEUP)
			{
				//����״̬	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//�������
				DeleteMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();

			}
			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GP_TASK_TAKE)
			{
				//����״̬	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//�������
				AddMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();

			}

			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GP_TASK_REWARD)
			{
				//��ѯ����
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);
				pTaskStatus->cbTaskStatus = TASK_STATUS_REWARD;

				//����״̬
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					LoadDataFromMemory();
					UpdateUI();
				}

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//���²Ƹ�
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}
		}

		//��Ϣ��ʾ
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(pTaskResult->szNotifyContent);
		}

		//������ֹ
		OnMissionConclude();


		return true;
	}

	return true;
}
//��ʼ����
VOID CDlgTask::OnMissionStart()
{

}

//��ֹ����
VOID CDlgTask::OnMissionConclude()
{


}

//��������
VOID CDlgTask::PerformLoadTask()
{
	//���ñ���
	m_bLoadTask=true;
	m_bTakeTask=false;
	m_bTaskReward=false;
	m_bTaskGiveUp =false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//��ȡ����
VOID CDlgTask::PerformTakeTask()
{
	//���ñ���
	m_bLoadTask=false;
	m_bTakeTask=true;
	m_bTaskReward=false;
	m_bTaskGiveUp =false;

	//���ñ���
	m_wTaskID=m_pTaskParameter->wTaskID;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}


//��ȡ����
VOID CDlgTask::PerformRewardTask()
{
	//���ñ���
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=true;
	m_bTaskGiveUp =false;

	//���ñ���
	m_wTaskID=m_pTaskStatus->wTaskID;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}


//��������
VOID CDlgTask::PerformGiveUpTask()
{
	//���ñ���
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=false;
	m_bTaskGiveUp =true;

	//���ñ���
	m_wTaskID=m_pTaskStatus->wTaskID;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//�������
BOOL CDlgTask::AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize)
{
	//����У��
	ASSERT(pTaskParameterData!=NULL && wParameterSize>0);
	if(pTaskParameterData==NULL || wParameterSize==0) return FALSE;

		//��������
	tagTaskParameter * pTaskParameter=NULL;

	//������Դ
	try
	{
		pTaskParameter = new tagTaskParameter;
		if(pTaskParameter==NULL) throw(TEXT("�ڴ治�㣡"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//��������
	CopyMemory(pTaskParameter,pTaskParameterData,wParameterSize);
	m_TaskParameterMap[pTaskParameter->wTaskID]=pTaskParameter;

	return true;
}

BOOL CDlgTask::DeleteMineTaskItem( tagTaskStatus & TaskStatus )
{

	//��������
	tagTaskStatus * pTaskStatus=NULL;

	if(m_TaskStatusMap.Lookup(TaskStatus.wTaskID,   pTaskStatus))   
	{   
		m_TaskStatusMap.RemoveKey(TaskStatus.wTaskID);   
		delete   pTaskStatus;
		pTaskStatus=NULL;   
	}

	return TRUE;
}

//�������
BOOL CDlgTask::AddMineTaskItem(tagTaskStatus & TaskStatus)
{
	//��������
	tagTaskStatus * pTaskStatus=NULL;

	//������Դ
	try
	{
		pTaskStatus = new tagTaskStatus;
		if(pTaskStatus==NULL) throw(TEXT("�ڴ治�㣡"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//��������
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

		//���Ҳ���
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
	//��������
	CDialogBuilder builderPage;
	CContainerUI* itme =NULL;
	//���Ҳ���
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

	//��������
	CControlUI * pControl =	itme->FindControl(_TEXT("LableName"));
	if(pControl !=NULL) 	pControl->SetText(pTaskParameter->szTaskName);

	//����ժҪ
	CControlUI * pControl2 = itme->FindControl(_TEXT("TextSummary"));
	if(pControl2 !=NULL)  pControl2->SetText(pTaskParameter->szTaskDescribe);

	//���ý���
	CControlUI * pControl3 = itme->FindControl(_TEXT("TextAward"));
	TCHAR szTaskReward[64]=TEXT("");
	if(pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��+%I64dԪ��"),pTaskParameter->lStandardAwardGold,pTaskParameter->lStandardAwardMedal);
	}
	else if( pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��"),pTaskParameter->lStandardAwardGold);
	}
	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64dԪ��"),pTaskParameter->lStandardAwardMedal);
	}
	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��"));
	}
	if(pControl3 !=NULL)  pControl3->SetText(szTaskReward);


	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	TCHAR szTaskRewardTotal[64]=TEXT("");
	if(pGlobalUserData->cbMemberOrder > 0)
	{
		//��Ա����
		const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
		if(pMemberParameter != NULL)
		{
			_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("����+%d%%"),pMemberParameter->dwMemberTask);
		}

		CControlUI * pContainerVip = itme->FindControl(szContainerVipAwardControlName);
		if(pContainerVip!=NULL)
		{
			//��Ա�ȼ�
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




	////�û���Ϣ
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//TCHAR szTaskReward[64]=TEXT("");
	//if(pGlobalUserData->cbMemberOrder > 0)
	//{
	//	
	//	if(pTaskParameter->lMemberAwardGold > 0 && pTaskParameter->lMemberAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��+%I64dԪ��"),pTaskParameter->lMemberAwardGold,pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if( pTaskParameter->lMemberAwardGold > 0 && pTaskParameter->lMemberAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��"),pTaskParameter->lMemberAwardGold);
	//	}
	//	else if(pTaskParameter->lMemberAwardGold <= 0 && pTaskParameter->lMemberAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64dԪ��"),pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if(pTaskParameter->lMemberAwardGold <= 0 && pTaskParameter->lMemberAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��"));
	//	}
	//}
	//else
	//{
	//	if(pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��+%I64dԪ��"),pTaskParameter->lMemberAwardGold,pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if( pTaskParameter->lStandardAwardGold > 0 && pTaskParameter->lStandardAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��"),pTaskParameter->lMemberAwardGold);
	//	}
	//	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal > 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64dԪ��"),pTaskParameter->lMemberAwardMedal);
	//	}
	//	else if(pTaskParameter->lStandardAwardGold <= 0 && pTaskParameter->lStandardAwardMedal <= 0)
	//	{
	//		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��"));
	//	}

	//}
	//pControl3->SetText(szTaskReward);
}

//�鿴����
VOID CDlgTask::ViewTaskReceiving(CControlUI * pControlUI)
{
	tagTaskParameter * pTaskParameter = (tagTaskParameter *)(pControlUI->GetParent()->GetTag());
	if(pTaskParameter!=NULL)
	{
		//���ñ���
		m_pTaskParameter=pTaskParameter;

		//���Ҷ���
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

//�鿴����
VOID CDlgTask::ViewTaskReceived(CControlUI * pControlUI)
{
	tagTaskParameter * pTaskParameter = (tagTaskParameter *)(pControlUI->GetParent()->GetTag());
	if(pTaskParameter!=NULL)
	{
		//���ñ���
		m_pTaskParameter=pTaskParameter;

		//���Ҷ���
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

//�鿴����
VOID CDlgTask::ViewTaskCompleted(CControlUI * pControlUI)
{
	tagTaskParameter * pTaskParameter = (tagTaskParameter *)(pControlUI->GetParent()->GetTag());
	if(pTaskParameter!=NULL)
	{
		//���ñ���
		m_pTaskParameter=pTaskParameter;

		//���Ҷ���
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

//ˢ��UI
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
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_C_LoadTaskInfo LoadTaskInfo;
	LoadTaskInfo.dwUserID = pGlobalUserData->dwUserID;
	lstrcpyn(LoadTaskInfo.szPassword,pGlobalUserData->szPassword,CountArray(LoadTaskInfo.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_LOAD_INFO,&LoadTaskInfo,sizeof(LoadTaskInfo));

	//��ʼ����
	OnMissionStart();
}

VOID CDlgTaskServer::PerformTakeTask()
{
	//���ñ���
	m_wTaskID=m_pTaskParameter->wTaskID;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_C_TakeTask TaskTake;
	TaskTake.wTaskID = m_wTaskID;
	TaskTake.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskTake.szMachineID);
	lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_TAKE,&TaskTake,sizeof(TaskTake));

	//��ʼ����
	OnMissionStart();
}


VOID CDlgTaskServer::PerformGiveUpTask()
{
	//���ñ���
	m_wTaskID=m_pTaskParameter->wTaskID;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_C_TakeGiveUp TaskTake;
	TaskTake.wTaskID = m_wTaskID;
	TaskTake.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskTake.szMachineID);
	lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_GIVEUP,&TaskTake,sizeof(TaskTake));

	//��ʼ����
	OnMissionStart();
}

VOID CDlgTaskServer::PerformRewardTask()
{
	//���ñ���
	m_wTaskID=m_pTaskStatus->wTaskID;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GP_TaskReward TaskReward;
	TaskReward.wTaskID = m_wTaskID;
	TaskReward.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskReward.szMachineID);
	lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_REWARD,&TaskReward,sizeof(TaskReward));

	//��ʼ����
	OnMissionStart();
}

VOID CDlgTaskServer::Initialization( ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem )
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

bool CDlgTaskServer::OnTaskMessage( WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//������Ϣ
	if(wSubCmdID==SUB_GR_TASK_INFO)
	{
		//��ȡ����
		CMD_GR_S_TaskInfo * pTaskInfo = (CMD_GR_S_TaskInfo *)pData;

		//����У��
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//�������
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;
		}

		LoadDataFromMemory();
		UpdateUI();

		//������ֹ
		OnMissionConclude();


		return true;
	}

	//������Ϣ
	if(wSubCmdID==SUB_GR_TASK_LIST)
	{
		//����У��
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//��������
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//��������
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//��������
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//�������
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//ƫ��ָ��
			pDataBuffer += wParameterSize;		
		}

		return true;
	}

	//ִ�н��
	if(wSubCmdID==SUB_GR_TASK_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskResult));
		if(wDataSize>sizeof(CMD_GR_S_TaskResult)) return false;

		//��ȡ����
		CMD_GR_S_TaskResult * pTaskResult = (CMD_GR_S_TaskResult *)pData;

		//ִ�гɹ�
		if(pTaskResult->bSuccessed==true)
		{
			//��������
			if(pTaskResult->wCommandID==SUB_GR_TASK_GIVEUP)
			{
				//����״̬	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//�������
				DeleteMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();

			}

			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GR_TASK_TAKE)
			{
				//����״̬	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//�������
				AddMineTaskItem(TaskStatus);				

				LoadDataFromMemory();
				UpdateUI();
			}

			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GR_TASK_REWARD)
			{
				//��ѯ����
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);

				//����״̬
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					LoadDataFromMemory();
					UpdateUI();
				}

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//���²Ƹ�
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}					
		}

		//��Ϣ��ʾ
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(pTaskResult->szNotifyContent);
		}

		//������ֹ
		OnMissionConclude();

		return true;
	}

	return true;
}

