#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgLockMachine.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName  = TEXT("ButtonClose");
const TCHAR* const szButtonLockControlName   = TEXT("ButtonLock");
const TCHAR* const szButtonUnlockControlName = TEXT("ButtonUnlock");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szEditPassWordControlName = TEXT("EditPassWord");

//////////////////////////////////////////////////////////////////////////////////
//�����¼�
const BYTE  byMissionNone   = 0;
const BYTE  byMissionLock   = 1;
const BYTE  byMissionunLock = 2;
//////////////////////////////////////////////////////////////////////////////////

CDlgLockMachine::CDlgLockMachine() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_byMissionType = byMissionNone;
	m_MissionManager.InsertMissionItem(this);
	m_byMachine=0;
	
	ZeroMemory(m_szPassword,sizeof(m_szPassword));
}

CDlgLockMachine::~CDlgLockMachine()
{
}

BOOL CDlgLockMachine::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("��������"));

	//���д���
	CenterWindow(this);

	//��������
	LoadDataFromMemory();

	//ˢ�½���
	UpdateDataToUI();

	return TRUE;
}

VOID CDlgLockMachine::OnOK()
{
	if (m_byMachine)
	{
		OnMachineLock(byMissionunLock);
	}
	else
	{
		OnMachineLock(byMissionLock);
	}
}

VOID CDlgLockMachine::OnCancel()
{
	__super::OnCancel();
}

void CDlgLockMachine::InitControlUI()
{
}

LPCTSTR CDlgLockMachine::GetSkinFile()
{
	return TEXT("DlgLockMachine.xml");
}

void CDlgLockMachine::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			OnCancel();
			return;
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0)
		{
			OnCancel();
			return;
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonLockControlName)==0)
		{
			OnNotifyLock(msg);
			return;
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonUnlockControlName)==0)
		{
			OnNotifyUnLock(msg);
			return;
		}
	}
	return;
}

void CDlgLockMachine::OnNotifyLock(TNotifyUI & msg)
{
    OnMachineLock(byMissionLock);
}

void CDlgLockMachine::OnNotifyUnLock(TNotifyUI & msg)
{
	OnMachineLock(byMissionunLock);
}

VOID CDlgLockMachine::OnMissionStart()
{

}

VOID CDlgLockMachine::OnMissionConclude()
{

}

bool CDlgLockMachine::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		OnMissionLinkError(nErrorCode);
		return true;
	}
	OnMissionStart();
	switch(m_byMissionType)
	{
	case byMissionLock:
	case byMissionunLock:
		{
			OnMissionLinkLock();
			break;
		}
	default:
		return true;
	}
	return true;
}

//�ر��¼�
bool CDlgLockMachine::OnEventMissionShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		OnMissionShutError(cbShutReason);
		return true;
	}
	return true;
}

//��ȡ�¼�
bool CDlgLockMachine::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:	//����ʧ��
			{
				OnMissionReadLockFailure(pData,wDataSize);
				break;
			}
		case SUB_GP_OPERATE_SUCCESS:	//�����ɹ�
			{
				OnMissionReadLockSuccess(pData,wDataSize);
				break;
			}
		}
	}

	OnMissionConclude();
	return true;
}

bool CDlgLockMachine::ConcludeMission()
{
	m_byMissionType = byMissionNone;
	m_MissionManager.ConcludeMissionItem(this,false);
	return true;
}

bool CDlgLockMachine::AvtiveMission(BYTE byMissionType)
{
	m_byMissionType = byMissionType;
	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)
	{
		//������ʾ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("����������ʧ�ܣ��������������Լ���������״����"),MB_ICONERROR);
		return false;
	}
	return true;
}

void CDlgLockMachine::OnMissionLinkError(INT nErrorCode)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("���������еķ��������޷��ɹ����ӷ���������������վά�����棡"),MB_ICONERROR);
}

void CDlgLockMachine::OnMissionShutError(BYTE cbShutReason)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("���ڵ�ǰ����������æ����������ʧ�ܣ����Ժ������ԣ�"),MB_ICONERROR);
}


void CDlgLockMachine::OnMissionLinkLock()
{
	OnSendData();
}

void CDlgLockMachine::OnMissionReadLockSuccess(VOID * pData, WORD wDataSize)
{
	//�ر�����
	ConcludeMission();
	//Ч������
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) 
	{
		return ;
	}


	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ���
	pGlobalUserData->cbMoorMachine=(m_byMachine==TRUE)?TRUE:FALSE;

	//�����¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_MOOR_MACHINE,0L);

	//��ʾ��Ϣ
	if (pOperateSuccess->szDescribeString[0]!=0)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
	}

	//�رմ���
	__super::OnCancel();
}

void CDlgLockMachine::OnMissionReadLockFailure(VOID * pData, WORD wDataSize)
{
	//�ر�����
	ConcludeMission();

	//Ч�����
	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) 
	{
		return ;
	}

	//��ʾ��Ϣ
	if (pOperateFailure->szDescribeString[0]!=0)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
	}

	//���ÿؼ�
	CEditUI * pEditPassword = (CEditUI *)GetControlByName(szEditPassWordControlName);
	if (pEditPassword !=NULL)
	{
		pEditPassword->SetFocus();
	}
}

void CDlgLockMachine::LoadDataFromUI()
{
	//��ȡ��Ϣ
	CEditUI * pEditPassword = (CEditUI *)GetControlByName(szEditPassWordControlName);
	if (pEditPassword !=NULL)
	{
		lstrcpyn(m_szPassword,pEditPassword->GetText(),CountArray(m_szPassword));
	}

}

void CDlgLockMachine::LoadDataFromMemory()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ð�ť
	m_byMachine=(pGlobalUserData->cbMoorMachine==0)?TRUE:FALSE;
}

void CDlgLockMachine::UpdateDataToUI()
{

	CControlUI * pbtLock = GetControlByName(szButtonLockControlName);
	if (pbtLock!=NULL)
	{
		pbtLock->SetVisible(m_byMachine == 1);
	}
	CControlUI * pbtUnlock = GetControlByName(szButtonUnlockControlName);
	if (pbtUnlock!=NULL)
	{
		pbtUnlock->SetVisible(m_byMachine == 0);
	}
}

void CDlgLockMachine::UpdateDataToMemory()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	pGlobalUserData->cbMoorMachine = m_byMachine;
}

bool CDlgLockMachine::EfficacyData()
{
	//�����ж�
	if (m_szPassword[0]==0)
	{
		//������ʾ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("���벻��Ϊ�գ�����������������а󶨻��߽���󶨣�"),MB_ICONERROR);

		//��ȡ��Ϣ
		CEditUI * pEditPassword = (CEditUI *)GetControlByName(szEditPassWordControlName);
		if (pEditPassword !=NULL)
		{
			//���ý���
			pEditPassword->SetFocus();
		}

		return false;
	}
	return true;
}

void CDlgLockMachine::OnMachineLock(BYTE byLock)
{
	//��������
	LoadDataFromUI();

	//��֤����
	if(!EfficacyData())
	{
		return;
	}
	//����Ự
	AvtiveMission(byLock);
}

void CDlgLockMachine::OnSendData()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	CMD_GP_ModifyMachine ModifyMachine;
	ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));
	//��������
	ModifyMachine.cbBind=m_byMachine;
	ModifyMachine.dwUserID=pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(ModifyMachine.szMachineID);
	CWHEncrypt::MD5Encrypt(m_szPassword,ModifyMachine.szPassword);
	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_MACHINE,&ModifyMachine,sizeof(ModifyMachine));

}
//////////////////////////////////////////////////////////////////////////////////
