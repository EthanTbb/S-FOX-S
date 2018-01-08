#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "Dlgbaseensure.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");

const TCHAR* const szButtonCheckInControlName = TEXT("ButtonCheckIn");
const TCHAR* const szButtonSpreadControlName = TEXT("ButtonSpread");
const TCHAR* const szButtonTaskControlName = TEXT("ButtonTask");
const TCHAR* const szButtonRechargeControlName = TEXT("ButtonRecharge");
const TCHAR* const szButtonVipControlName = TEXT("ButtonVip");

//��ǩ�ؼ�
const TCHAR* const szLabelLessTipControlName = TEXT("LabelLessTip");
const TCHAR* const szLabelConditionControlName = TEXT("LabelCondition");
const TCHAR* const szLabelTimesControlName = TEXT("LabelTimes");
const TCHAR* const szLabelValueControlName = TEXT("LabelValue");
const TCHAR* const szLabelDisEnableTipControlName = TEXT("LabelDisEnableTip");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgBaseEnsure::CDlgBaseEnsure(): CFGuiDialog(IDD_DLG_POPUP)
{
	//���Ʊ���
	m_bLoadBaseEnsure=false;
	m_bTakeBaseEnsure=false;

	//���ñ���
	ZeroMemory(&m_BaseEnsureParameter,sizeof(m_BaseEnsureParameter));

	//��������
	m_MissionManager.InsertMissionItem(this);
}

//��������
CDlgBaseEnsure::~CDlgBaseEnsure()
{
}


//��ʼ�ؼ�
void CDlgBaseEnsure::InitControlUI()
{
	__super::InitControlUI();
}

//��Ϣ����
void CDlgBaseEnsure::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTakeControlName)==0)
		{
			//��ȡ�ͱ�
			return PerformTakeBaseEnsure();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonCheckInControlName)==0)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_CHECKIN,0L);

			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonSpreadControlName)==0)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_MY_SPREAD,0L);
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTaskControlName)==0)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_TASKVIEW,0L);
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonRechargeControlName)==0)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonVipControlName)==0)
		{
			//�����¼�
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

//��������
BOOL CDlgBaseEnsure::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�ҵĵͱ�"));

	//���д���
	CenterWindow(this);

	//���صͱ�
	PerformLoadBaseEnsure();

	return FALSE;
}


//�����¼�
bool CDlgBaseEnsure::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//��ʾ��ʾ
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("�޷����ӵ���¼��������������Ϣ����ʧ�ܣ����Ժ����ԣ�"),MB_ICONERROR);
	}
	else
	{
		//���صͱ�
		if(m_bLoadBaseEnsure==true)
		{
			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_LOAD,NULL,0);

			return true;
		}

		//��ȡ�ͱ�
		if(m_bTakeBaseEnsure==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_BaseEnsureTake BaseEnsureTake;
			BaseEnsureTake.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(BaseEnsureTake.szMachineID);
			lstrcpyn(BaseEnsureTake.szPassword,pGlobalUserData->szPassword,CountArray(BaseEnsureTake.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_TAKE,&BaseEnsureTake,sizeof(BaseEnsureTake));

			return true;
		}
	}

	return true;
}

//�ر��¼�
bool CDlgBaseEnsure::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//��ȡ�¼�
bool CDlgBaseEnsure::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�������
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//�ͱ�����
	if(Command.wSubCmdID==SUB_GP_BASEENSURE_PARAMETER)
	{
		//����У��
		ASSERT(wDataSize==sizeof(CMD_GP_BaseEnsureParamter));
		if(wDataSize!=sizeof(CMD_GP_BaseEnsureParamter)) return false;

		//��ȡ����
		CMD_GP_BaseEnsureParamter * pBaseEnsureParamter= (CMD_GP_BaseEnsureParamter *)pData;

		//���ñ���
		m_BaseEnsureParameter.cbTakeTimes=pBaseEnsureParamter->cbTakeTimes;
		m_BaseEnsureParameter.lScoreAmount=pBaseEnsureParamter->lScoreAmount;
		m_BaseEnsureParameter.lScoreCondition=pBaseEnsureParamter->lScoreCondition;
		
		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();

		UpdateUI();


		return true;
	}

	//�ͱ����
	if(Command.wSubCmdID==SUB_GP_BASEENSURE_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_BaseEnsureResult));
		if(wDataSize>sizeof(CMD_GP_BaseEnsureResult)) return false;

		//��ȡ����
		CMD_GP_BaseEnsureResult * pBaseEnsureResult = (CMD_GP_BaseEnsureResult *)pData;

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//ִ�гɹ�
		if(pBaseEnsureResult->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//������Ϸ��
			pGlobalUserData->lUserScore=pBaseEnsureResult->lGameScore;

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//������ֹ
		OnMissionConclude();

		//��Ϣ��ʾ
		if(pBaseEnsureResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(pBaseEnsureResult->szNotifyContent);
		}

		return true;
	}

	return true;
}

//��ʼ����
VOID CDlgBaseEnsure::OnMissionStart()
{
	//���ÿؼ�
	CControlUI * pControl =	GetControlByName(szButtonTakeControlName);
	if(pControl != NULL)
	{
		pControl->SetEnabled(false);
	}
}

//��ֹ����
VOID CDlgBaseEnsure::OnMissionConclude()
{
	//��������
	CControlUI * pControl =	GetControlByName(szButtonTakeControlName);
	if(pControl != NULL)
	{
		//��ȡ����
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		//��Ϸ���ж�
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

//���صͱ�
VOID CDlgBaseEnsure::PerformLoadBaseEnsure()
{
	//���ñ���
	m_bLoadBaseEnsure=true;
	m_bTakeBaseEnsure=false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//��ȡ�ͱ�
VOID CDlgBaseEnsure::PerformTakeBaseEnsure()
{
	//���ñ���
	m_bLoadBaseEnsure=false;
	m_bTakeBaseEnsure=true;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

void CDlgBaseEnsure::UpdateUI()
{
	CControlUI * pControl = NULL;
	//��ȡ����
	TCHAR szTakeCondition[128]=TEXT("");
	_sntprintf(szTakeCondition,CountArray(szTakeCondition),TEXT("��Ϸ�ҵ��� %I64d "),m_BaseEnsureParameter.lScoreCondition);
	pControl =	GetControlByName(szLabelConditionControlName);
	if(pControl != NULL)
	{
		pControl->SetText(szTakeCondition);
		pControl = NULL;
	}

	//��ȡ����
	TCHAR szTakeTimes[128]=TEXT("");
	_sntprintf(szTakeTimes,CountArray(szTakeTimes),TEXT("%d ��/��"),m_BaseEnsureParameter.cbTakeTimes);
	pControl =	GetControlByName(szLabelTimesControlName);
	if(pControl != NULL)
	{
		pControl->SetText(szTakeTimes);
		pControl = NULL;
	}

	//��ȡ���
	TCHAR szTakeValue[128]=TEXT("");
	_sntprintf(szTakeValue,CountArray(szTakeValue),TEXT("%I64d ��Ϸ��"),m_BaseEnsureParameter.lScoreAmount);
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
	//��ʼ����
	OnMissionStart();
	//��������
	m_pITCPSocket->SendData(MDM_GR_BASEENSURE,SUB_GR_BASEENSURE_LOAD,NULL,0);

}

VOID CDlgBaseEnsureServer::PerformTakeBaseEnsure()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_BaseEnsureTake BaseEnsureTake;
	BaseEnsureTake.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(BaseEnsureTake.szMachineID);
	lstrcpyn(BaseEnsureTake.szPassword,pGlobalUserData->szPassword,CountArray(BaseEnsureTake.szPassword));

	//��ʼ����
	OnMissionStart();

	//��������
	m_pITCPSocket->SendData(MDM_GR_BASEENSURE,SUB_GR_BASEENSURE_TAKE,&BaseEnsureTake,sizeof(BaseEnsureTake));

}

VOID CDlgBaseEnsureServer::Initialization( ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem )
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

bool CDlgBaseEnsureServer::OnServerBaseEnsureMessage( WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//��Ϣ�ж�
	if (wMainCmdID==MDM_GR_BASEENSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_BASEENSURE_PARAMETER:		
			{
				//����У��
				ASSERT(wDataSize==sizeof(CMD_GR_BaseEnsureParamter));
				if(wDataSize!=sizeof(CMD_GR_BaseEnsureParamter)) return false;

				//��ȡ����
				CMD_GR_BaseEnsureParamter * pBaseEnsureParamter= (CMD_GR_BaseEnsureParamter *)pData;

				//���ñ���
				m_BaseEnsureParameter.cbTakeTimes=pBaseEnsureParamter->cbTakeTimes;
				m_BaseEnsureParameter.lScoreAmount=pBaseEnsureParamter->lScoreAmount;
				m_BaseEnsureParameter.lScoreCondition=pBaseEnsureParamter->lScoreCondition;

				//������ֹ
				OnMissionConclude();

				UpdateUI();

				return true;
			}
		case SUB_GR_BASEENSURE_RESULT:		
			{
				//����У��
				ASSERT(wDataSize<=sizeof(CMD_GR_BaseEnsureResult));
				if(wDataSize>sizeof(CMD_GR_BaseEnsureResult)) return false;

				//��ȡ����
				CMD_GR_BaseEnsureResult * pBaseEnsureResult = (CMD_GR_BaseEnsureResult *)pData;

				//ִ�гɹ�
				if(pBaseEnsureResult->bSuccessed==true)
				{
					//��ȡ����
					CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

					//������Ϸ��
					pGlobalUserData->lUserScore=pBaseEnsureResult->lGameScore;

					//�����¼�
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
				}

				//������ֹ
				OnMissionConclude();

				//��Ϣ��ʾ
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
