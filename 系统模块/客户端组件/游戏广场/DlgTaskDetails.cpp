#include "Stdafx.h"
#include "Resource.h"
#include "DlgTaskDetails.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonRecevingControlName = TEXT("ButtonReceving");
const TCHAR* const szButtonGiveUpControlName = TEXT("ButtonGiveUp");
const TCHAR* const szButtonGetControlName = TEXT("ButtonGet");

//��ǩ�ؼ�
const TCHAR* const szLabelNameControlName = TEXT("LabelName");
const TCHAR* const szLabelProgressControlName = TEXT("LabelProgress");
const TCHAR* const szLabelTimeControlName = TEXT("LabelTime");
const TCHAR* const szLabelSummaryControlName = TEXT("LabelSummary");
const TCHAR* const szLabelAwardControlName = TEXT("LabelAward");

//VIP�ؼ�
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVipAward");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelAwardMemberControlName = TEXT("LabelAwardMember");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTaskDetails ::CDlgTaskDetails () : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pTaskParameter = NULL;
	m_pTaskStatus =NULL;
}

//��������
CDlgTaskDetails ::~CDlgTaskDetails ()
{
}

//��������
BOOL CDlgTaskDetails ::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("��Ϸ����"));

	//���д���
	CenterWindow(this);

	UpdateDetails();

	return TRUE;
}

//ȷ������
VOID CDlgTaskDetails ::OnOK()
{

	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgTaskDetails ::OnCancel()
{
	return __super::OnCancel();
}

//��Ϣ����
void CDlgTaskDetails ::Notify(TNotifyUI &  msg)
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

	//��������
	CControlUI * pControl =	GetControlByName(szLabelNameControlName);
	if(pControl) pControl->SetText(m_pTaskParameter->szTaskName);

	//���ý���
	TCHAR szTaskProgress[32]=TEXT("");
	_sntprintf(szTaskProgress,CountArray(szTaskProgress),TEXT("%d/%d"),(m_pTaskStatus?m_pTaskStatus->wTaskProgress:0),m_pTaskParameter->wTaskObject);
	pControl =	GetControlByName(szLabelProgressControlName);
	if(pControl) pControl->SetText(szTaskProgress);

	//����ʱ��
	TCHAR szTaskTimeLimit[128]=TEXT("");
	WORD wHour = (WORD)(m_pTaskParameter->dwTimeLimit/3600);
	WORD wMinute = (WORD)((m_pTaskParameter->dwTimeLimit-wHour*3600)/60);
	WORD wSecond = (WORD)(m_pTaskParameter->dwTimeLimit-wHour*3600-wMinute*60);
	_sntprintf(szTaskTimeLimit,CountArray(szTaskTimeLimit),TEXT("%02dʱ:%02d��:%02d��"),wHour,wMinute,wSecond);
	pControl =	GetControlByName(szLabelTimeControlName);
	if(pControl) pControl->SetText(szTaskTimeLimit);


	//����˵��
	pControl = GetControlByName(szLabelSummaryControlName);
	if(pControl) pControl->SetText(m_pTaskParameter->szTaskDescribe);

	//���ý���
	TCHAR szTaskReward[64]=TEXT("");
	if(m_pTaskParameter->lStandardAwardGold > 0 && m_pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��+%I64dԪ��"),m_pTaskParameter->lStandardAwardGold,m_pTaskParameter->lStandardAwardMedal);
	}
	else if( m_pTaskParameter->lStandardAwardGold > 0 && m_pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64d��Ϸ��"),m_pTaskParameter->lStandardAwardGold);
	}
	else if(m_pTaskParameter->lStandardAwardGold <= 0 && m_pTaskParameter->lStandardAwardMedal > 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("%I64dԪ��"),m_pTaskParameter->lStandardAwardMedal);
	}
	else if(m_pTaskParameter->lStandardAwardGold <= 0 && m_pTaskParameter->lStandardAwardMedal <= 0)
	{
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��"));
	}
	pControl = GetControlByName(szLabelAwardControlName);
	if(pControl) pControl->SetText(szTaskReward);

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

		CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
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