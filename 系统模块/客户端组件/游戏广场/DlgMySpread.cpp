#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "DlgMySpread.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonCopyControlName = TEXT("ButtonCopy");
const TCHAR* const szTextSpreadControlName = TEXT("TextSpread");
const TCHAR* const szEditInputControlName = TEXT("EditInput");
//////////////////////////////////////////////////////////////////////////////////
//�����¼�
const BYTE  byMissionNone   = 0;
const BYTE  byMissionLoad   = 1;
//////////////////////////////////////////////////////////////////////////////////

CDlgMySpread::CDlgMySpread(): CFGuiDialog(IDD_DLG_POPUP)
{
	m_byMissionType = byMissionNone;
	m_MissionManager.InsertMissionItem(this);
	ZeroMemory(m_szSpreadLink,sizeof(m_szSpreadLink));
	ZeroMemory(&m_UserSpreadInfo,sizeof(m_UserSpreadInfo));
}

CDlgMySpread::~CDlgMySpread()
{
}

BOOL CDlgMySpread::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("�ƹ㴰��"));

	//���д���
	CenterWindow(this);

	//��������
	LoadDataFromMemory();

	//ˢ�½���
	UpdateDataToUI();

	//���ؽ���
	PerformLoadSpreadReward();

	return TRUE;
}

VOID CDlgMySpread::OnOK()
{

}

VOID CDlgMySpread::OnCancel()
{
	__super::OnCancel();
}

//��ʼ�ؼ�
void CDlgMySpread::InitControlUI()
{
}

//Ƥ������
LPCTSTR CDlgMySpread::GetSkinFile()
{
	return TEXT("DlgSpread.xml");
}

//��Ϣ����
void CDlgMySpread::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			OnCancel();
			return;
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonCopyControlName)==0)
		{
			OnNotifyCopy(msg);
			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if (lstrcmp(pControlUI->GetName(), szTextSpreadControlName)==0)
		{
			OnNotifyLink(msg);
			return;
		}
	}
}

void CDlgMySpread::OnNotifyCopy(TNotifyUI & msg)
{
	//���ü�����
	if (CWHService::SetClipboardString(m_szSpreadLink))
	{
		CToastUI *pToast = (CToastUI *)(GetControlByName(_T("ntoast")));
		if(pToast!=NULL)
		{
			pToast->Show(500);
		}
	}
}

void CDlgMySpread::OnNotifyLink(TNotifyUI & msg)
{
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//�����ַ
	TCHAR szSpreadInfo[256]=TEXT("");
	int index = (int)msg.wParam;
	if (index ==0)
	{
		_sntprintf(szSpreadInfo,CountArray(szSpreadInfo),TEXT("%s/%sMember/SpreadInfo.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	}
	else if (index ==1)
	{
		_sntprintf(szSpreadInfo,CountArray(szSpreadInfo),TEXT("%s/%sMember/SpreadBalance.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
	}
	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szSpreadInfo,NULL,NULL,SW_NORMAL);
}

//��ʼ����
VOID CDlgMySpread::OnMissionStart()
{
}

//��ֹ����
VOID CDlgMySpread::OnMissionConclude()
{

}

//�����¼�
bool CDlgMySpread::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		OnMissionLinkError(nErrorCode);
		return true;
	}

	OnMissionStart();
	switch(m_byMissionType)
	{
	case byMissionLoad:
		{
			OnMissionLinkLoad();
			break;
		}
	default:
		return true;
	}
	return true;
}

//�ر��¼�
bool CDlgMySpread::OnEventMissionShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		OnMissionShutError(cbShutReason);
		return true;
	}
	return true;
}

//��ȡ�¼�
bool CDlgMySpread::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_SPREAD_INFO:	//���ؽ���
			{
				OnMissionReadLoad(pData,wDataSize);
				break;
			}
		}
	}

	OnMissionConclude();
	return true;
}



bool CDlgMySpread::ConcludeMission()
{
	m_byMissionType = byMissionNone;
	m_MissionManager.ConcludeMissionItem(this,false);
	return true;

}

bool CDlgMySpread::AvtiveMission(BYTE byMissionType)
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

void CDlgMySpread::OnMissionLinkError(INT nErrorCode)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("���������еķ��������޷��ɹ����ӷ���������������վά�����棡"),MB_ICONERROR);
}

void CDlgMySpread::OnMissionShutError(BYTE cbShutReason)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("���ڵ�ǰ����������æ����������ʧ�ܣ����Ժ������ԣ�"),MB_ICONERROR);

}

void CDlgMySpread::OnMissionLinkLoad()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GP_UserSpreadQuery UserSpreadQuery;
	UserSpreadQuery.dwUserID=pGlobalUserData->dwUserID;

	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SPREAD_QUERY,&UserSpreadQuery,sizeof(UserSpreadQuery));

}

void CDlgMySpread::OnMissionReadLoad(VOID * pData, WORD wDataSize)
{
	//�ر�����
	ConcludeMission();

	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_UserSpreadInfo));
	if(wDataSize!=sizeof(CMD_GP_UserSpreadInfo)) return;

	//��ȡ����
	CMD_GP_UserSpreadInfo * pUserSpreadInfo= (CMD_GP_UserSpreadInfo *)pData;

	//���ñ���
	m_UserSpreadInfo.dwSpreadCount=pUserSpreadInfo->dwSpreadCount;
	m_UserSpreadInfo.lSpreadReward=pUserSpreadInfo->lSpreadReward;

	UpdateDataToUI();

}

void CDlgMySpread::LoadDataFromUI()
{

}

void CDlgMySpread::LoadDataFromMemory()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
	CString strPlatformLink(CGlobalWebLink::GetInstance()->GetPlatformLink());
	int nStartIndex = strPlatformLink.Find(':');
	int nEndIndex = strPlatformLink.Find('.');
	if(nStartIndex != -1 && nEndIndex != -1)
	{
		strPlatformLink.Replace(strPlatformLink.Mid(nStartIndex+1,nEndIndex-nStartIndex-1),TEXT("//%d"));
	}
	_sntprintf(m_szSpreadLink,CountArray(m_szSpreadLink),strPlatformLink,pGlobalUserData->dwGameID);
}

void CDlgMySpread::UpdateDataToUI()
{
	//��������
	TCHAR szSpreadText[256]=TEXT("");
	_sntprintf(szSpreadText,CountArray(szSpreadText),TEXT("�����Ƽ� {a name='PlayerNumLink' linkcolor='#FFFFCC00' linkhovercolor='#FFFFFF66'}%d{/a} ��ң�Ŀǰ����ȡ�ƹ㽱�� {a name='GoldNum' linkcolor='#FFFFCC00' linkhovercolor='#FFFFFF66'}%d{/a} ��Ϸ��"),m_UserSpreadInfo.dwSpreadCount,m_UserSpreadInfo.lSpreadReward);

	CTextUI * pTextSpread = (CTextUI *)GetControlByName(szTextSpreadControlName);
	if (pTextSpread)
	{
		pTextSpread->SetText(szSpreadText);
	}

	CEditUI * pEditLink = (CEditUI *)GetControlByName(szEditInputControlName);
	if (pEditLink!=NULL)
	{
		pEditLink->SetText(m_szSpreadLink);
		pEditLink->SetEnabled(false);
	}

}

void CDlgMySpread::UpdateDataToMemory()
{

}

//���ؽ���
void CDlgMySpread::PerformLoadSpreadReward()
{
	AvtiveMission(byMissionLoad);
}
////////////////////////////////////////////////////////////////////////////////////////////
