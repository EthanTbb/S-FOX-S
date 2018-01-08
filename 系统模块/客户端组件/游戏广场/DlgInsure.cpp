#include "Stdafx.h"
#include "Resource.h"
#include "DlgInsure.h"
#include "DlgInsureOpen.h"
#include "GlobalUnits.h"
#include "InsureCommon.h"
#include "DlgPresentProof.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//�����ؼ�
const TCHAR* const szTabLayoutInsureName = TEXT("TabLayoutInsure");
const TCHAR* const szContainerWealthName = TEXT("InsureItemWealth");
const TCHAR* const szContainerAccessName = TEXT("InsureItemAccess");
const TCHAR* const szContainerPresentName = TEXT("InsureItemPresent");
const TCHAR* const szContainerRecordName = TEXT("InsureItemRecord");

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonDefaultControlName = TEXT("ButtonDefault");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szButtonPresentControlName = TEXT("ButtonPresent");

const TCHAR* const szButtonGetBaseEnsureControlName = TEXT("ButtonGetBaseEnsure");
const TCHAR* const szButtonGetScoreControlName = TEXT("ButtonGetScore");
const TCHAR* const szButtonRechargeControlName = TEXT("ButtonRecharge");
const TCHAR* const szButtonExchargeControlName1 = TEXT("ButtonExcharge1");
const TCHAR* const szButtonExchargeControlName2 = TEXT("ButtonExcharge2");

const TCHAR* const szWebBrowserRecordControlName = TEXT("WebBrowserRecord");
//////////////////////////////////////////////////////////////////////////////////
//�ҵĲƸ�

//��ť�ؼ�
const TCHAR* const szButtonBaseEnsureControlName = TEXT("ButtonBaseEnsure");
const TCHAR* const szButtonRechargCenterControlName = TEXT("ButtonRechargCenter");
const TCHAR* const szButtonMyBagControlName = TEXT("ButtonMyBag");
const TCHAR* const szButtonExchangeControlName = TEXT("ButtonExchange");
const TCHAR* const szButtonGameShopControlName = TEXT("ButtonGameShop");
const TCHAR* const szButtonRechargeRecrodControlName = TEXT("ButtonRechargeRecrod");
const TCHAR* const szButtonSetControlName = TEXT("ButtonSet");

//�༭�ؼ�
const TCHAR* const szEditGameScoreControlName = TEXT("EditGameScore");
const TCHAR* const szEditInsureGameScoreControlName = TEXT("EditInsureGameScore");
const TCHAR* const szEditGameBeanControlName = TEXT("EditGameBean");
const TCHAR* const szEditGameIngotControlName = TEXT("EditGameIngot");
const TCHAR* const szEditGlamourValueControlName = TEXT("EditGlamourValue");

//��ǩ�ؼ�
const TCHAR* const szContainerSetTipControlName = TEXT("ContainerSetTip");
const TCHAR* const szlableUnSetTipControlName = TEXT("lableUnSetTip");
const TCHAR* const szLabelInOutControlName = TEXT("LabelInOut");

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ�Ҵ�ȡ

//���ֿؼ�
const TCHAR* const szNumberUserScoreControlName = TEXT("NumberUserScore");
const TCHAR* const szNumberAcessUserInsrueControlName = TEXT("NumberAcessUserInsrue");
const TCHAR* const szNumberGameScoreControlName = TEXT("NumberGameScore");
const TCHAR* const szNumberInsureGameScoreControlName = TEXT("NumberInsureGameScore");
const TCHAR* const szNumberGameBeanControlName = TEXT("NumberGameBean");
const TCHAR* const szNumberGameIngotControlName = TEXT("NumberGameIngot");
const TCHAR* const szNumberGlamourValueControlName = TEXT("NumberGlamourValue");

//��ť�ؼ�
const TCHAR* const szButtonW1ControlName = TEXT("ButtonW1");
const TCHAR* const szButtonW5ControlName = TEXT("ButtonW5");
const TCHAR* const szButtonW10ControlName = TEXT("ButtonW10");
const TCHAR* const szButtonW50ControlName = TEXT("ButtonW50");
const TCHAR* const szButtonW100ControlName = TEXT("ButtonW100");
const TCHAR* const szButtonW1000ControlName = TEXT("ButtonW1000");
const TCHAR* const szButtonW10000ControlName = TEXT("ButtonW10000");
const TCHAR* const szButtonAllInControlName = TEXT("ButtonAllIn");
const TCHAR* const szButtonAllOutControlName = TEXT("ButtonAllOut");
const TCHAR* const szButtonInControlName = TEXT("ButtonIn");
const TCHAR* const szButtonOutControlName = TEXT("ButtonOut");

//�༭�ؼ�
const TCHAR* const szEditInOutControlName = TEXT("EditInOut");
const TCHAR* const szEditPassWordControlName = TEXT("EditPassWord");
//////////////////////////////////////////////////////////////////////////////////
//��Ϸ������

//��ǩ�ؼ�
const TCHAR* const szLabelPresentIDControlName = TEXT("LabelPresentID");
const TCHAR* const szLabelPresentChineseNumeralControlName = TEXT("LabelPresentChineseNumeral");
//���ֿؼ�
const TCHAR* const szumberPresentUserInsrueControlName = TEXT("NumberPresentUserInsrue");

//��ѡ�ؼ�
const TCHAR* const szRadioButtonCtrlEnterControlName = TEXT("RadioButtonCtrlEnter");
const TCHAR* const szRadioButtonPresentIDControlName = TEXT("RadioButtonPresentID");
const TCHAR* const szRadioButtonPresentNickControlName = TEXT("RadioButtonPresentNick");

//�༭�ؼ�
const TCHAR* const szEditPresentIDControlName = TEXT("EditPresentID");
const TCHAR* const szEditPresentInsurePassWordControlName = TEXT("EditPresentInsurePassWord");
const TCHAR* const szEditPresentAmountControlName = TEXT("EditPresentAmount");
const TCHAR* const szEditPresentBakControlName = TEXT("EditPresentBak");

//VIP�ؼ�
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVipAward");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelMemberTaxControlName = TEXT("LabelMemberTax");
const TCHAR* const szLabelMemberTaxFreeControlName = TEXT("LabelMemberTaxFree");

const TCHAR* const szTextTransferTaxControlName = TEXT("TextTransferTax");
const TCHAR* const szTextTakeTaxControlName = TEXT("TextTakeTax");
/////////////////////////////////////////////////////////////////////////////////
void SetEditNum(CContainerUI* pContainer, LPCTSTR pszEditName, SCORE  dwNum)
{
	TCHAR szBuffer[64]=TEXT("");
	wsprintf(szBuffer, TEXT("%I64d"), dwNum);
	CEditUI* pEditUI = (CEditUI*)(pContainer->FindControl(pszEditName));
	if(pEditUI != NULL) pEditUI->SetText(szBuffer);
}

void SetNumber(CContainerUI* pContainer, LPCTSTR szControlName, const double  lNum)
{
	CNumberUI* pNumberUI = (CNumberUI*)(pContainer->FindControl(szControlName));
	if(pNumberUI != NULL) pNumberUI->SetNumber(lNum);
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsure::CDlgInsure() : CFGuiDialog(IDD_DLG_POPUP)
,m_pItemWealth(NULL)
,m_pItemAccess(NULL)
,m_pItemPresent(NULL)
,m_pContainerWealth(NULL)
,m_pContainerAccess(NULL)
,m_pContainerPresent(NULL)
,m_pContainerRecord(NULL)
,m_bEnableMission(false)
,m_bSaveMission(false)
,m_bTakeMission(false)
,m_bQueryMission(false)
,m_bTransferMission(false)
,m_bIsTransfer(false)
,m_bQueryRebateMission(false)
{
	cbRebateEnabled = 0;
	lRebateIngot = 0;
	lRebateLoveLiness =0;
	//��������
	m_MissionManager.InsertMissionItem(this);
	ZeroMemory(&m_UserInsureInfo,sizeof(m_UserInsureInfo));
}

//��������
CDlgInsure::~CDlgInsure()
{
	if(m_pItemWealth)
	{
		delete m_pItemWealth;
		m_pItemWealth = NULL;
	}

	if(m_pItemAccess)
	{
		delete m_pItemAccess;
		m_pItemAccess = NULL;
	}

	if(m_pItemPresent)
	{
		delete m_pItemPresent;
		m_pItemPresent = NULL;
	}
}

//��������
BOOL CDlgInsure::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�ҵ�����"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//�ƶ�����
	MoveWindow(rc);

	//��ѯ������Ϣ
	/*if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled==TRUE)*/ //QueryUserInsureInfo();
	PerformQueryRebate();

	return TRUE;
}

//ȷ������
VOID CDlgInsure::OnOK()
{

	//return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgInsure::OnCancel()
{

	return __super::OnCancel();
}

//��ʼ�ؼ�
VOID CDlgInsure::InitControlUI()
{
	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/UserService1.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRecordControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

	InitUI();
}
//��Ϣ����
void CDlgInsure::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (msg.sType == TEXT("click"))
	{
		if((pControlUI->GetName() == szButtonCloseControlName))
		{
			return OnCancel();
		}
		else if((pControlUI->GetName() == szButtonCancleControlName))
		{
			return OnCancel();
		}
		else if((pControlUI->GetName() == szButtonComfirmControlName))
		{
			return OnOK();
		}
		else if((pControlUI->GetName() == szButtonDefaultControlName))
		{
			return OnDefault();
		}
		//��ȡ��Ϸ��
		else if(pControlUI->GetName()== szButtonGetScoreControlName)
		{
			//��ȡ��ǩ
			CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabLayoutInsureName));
			COptionUI *pOptionUI = static_cast<COptionUI *>(GetControlByName(TEXT("OptionInsureAccess")));
			if(pTabLayoutUI == NULL || pOptionUI == NULL) return;

			pTabLayoutUI->SelectItem(0);
			pOptionUI->Selected(true);
		}
	}
	else if(lstrcmp(msg.sType, TEXT("selectchanged")) == 0)
	{
		//��ȡ��ǩ
		CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabLayoutInsureName));
		COptionUI *pOptionUI = static_cast<COptionUI *>(GetControlByName(TEXT("OptionInsureWealth")));
		if(pTabLayoutUI == NULL) return;
		
		bool bShow = false;
		if(pControlUI->GetName() == TEXT("OptionInsureWealth"))
		{
			pTabLayoutUI->SelectItem(0);
			return;
		}
		else if(pControlUI->GetName() == TEXT("OptionInsureAccess"))
		{
			if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled == FALSE)
			{
				bShow = true;
			}
			else
			{
				pTabLayoutUI->SelectItem(1);
			}
			
		}
		else if(pControlUI->GetName() == TEXT("OptionInsurePresent"))
		{
			if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled == FALSE)
			{
				bShow = true;
			}
			else
			{
				pTabLayoutUI->SelectItem(2);
			}
		}
		else if(pControlUI->GetName() == TEXT("OptionInsureRecord"))
		{
			if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled == FALSE)
			{
				bShow = true;
			}
			else
			{
				CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRecordControlName));
				if( pActiveXUI1 ) 
				{
					pActiveXUI1->Refresh();
				}
				pTabLayoutUI->SelectItem(3);
			}
			
		}

		if(bShow)
		{
			pOptionUI->Selected(true);
			pTabLayoutUI->SelectItem(0);

			//��ʾ��Ϣ
			CDlgInformation Information(this);
			if(IDOK ==Information.ShowMessageBox(TEXT("����ʹ�ã����ȿ�ͨ���У�"),MB_OKCANCEL|MB_ICONINFORMATION))
			{
				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_PERSONER,2L);
				return OnCancel();
			}
		}

		return;

	}

	if(m_pItemWealth) m_pItemWealth->Notify(msg);
	if(m_pItemAccess) m_pItemAccess->Notify(msg);
	if(m_pItemPresent) m_pItemPresent->Notify(msg);
	return;
}

LPCTSTR CDlgInsure::GetSkinFile()
{
	return TEXT("DlgInsure.xml"); 
}


bool CDlgInsure::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//�¼�����
		OnMissionConclude();

		//������ʾ
		if (m_bEnableMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����ͨ����ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bSaveMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ�Ҵ���ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bTakeMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ��ȡ��ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���������ϻ�ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bTransferMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ������ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bQueryRebateMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����ѯ����ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}
		return true;
	}

	//��ͨ����
	if (m_bEnableMission==true)
	{
		//��������
		CMD_GP_UserEnableInsure UserEnableInsure;
		ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserEnableInsure.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(UserEnableInsure.szLogonPass,m_szLogonPass,CountArray(UserEnableInsure.szLogonPass));
		lstrcpyn(UserEnableInsure.szInsurePass,m_szInsurePass,CountArray(UserEnableInsure.szInsurePass));		
		CWHService::GetMachineIDEx(UserEnableInsure.szMachineID);

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_ENABLE_INSURE,&UserEnableInsure,sizeof(UserEnableInsure));

		return true;
	}

	//��������
	if (m_bSaveMission==true)
	{
		//��������
		CMD_GP_UserSaveScore UserSaveScore;
		ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserSaveScore.lSaveScore=m_lScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserSaveScore.szMachineID);

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		return true;
	}

	//ȡ������
	if (m_bTakeMission==true)
	{
		//��������
		CMD_GP_UserTakeScore UserTakeScore;
		ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserTakeScore.lTakeScore=m_lScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserTakeScore.szMachineID);
		lstrcpyn(UserTakeScore.szPassword,m_szInsurePass,CountArray(UserTakeScore.szPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		return true;
	}

	//ת������
	if (m_bTransferMission==true)
	{
		//��������
		CMD_GP_QueryUserInfoRequest QueryUserInfoRequest;
		ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

		//���ñ���
		QueryUserInfoRequest.cbByNickName=m_cbByNickName;
		lstrcpyn(QueryUserInfoRequest.szAccounts,m_szAccounts,CountArray(QueryUserInfoRequest.szAccounts));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));
	}

	//��ѯ����
	if (m_bQueryMission==true)
	{
		//��������
		CMD_GP_QueryInsureInfo QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//���ñ���
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;
		lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserInfo->GetGlobalUserData()->szPassword,CountArray(QueryInsureInfo.szPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	//��ѯ����
	if (m_bQueryRebateMission==true)
	{
		//��������
		CMD_GP_QueryTransferRebate QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//���ñ���
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;
		lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserInfo->GetGlobalUserData()->szPassword,CountArray(QueryInsureInfo.szPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_TRANSFER_REBATE,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	return true;
}


bool CDlgInsure::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		////��������
		//if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		//{
		//	return true;
		//}

		//������ʾ
		if (m_bEnableMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����ͨ����ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//��������
		if (m_bSaveMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ���Ϸ�Ҵ���ʧ�ܣ�"),MB_ICONERROR,30);
		}

		//ȡ������
		if (m_bTakeMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ���Ϸ��ȡ��ʧ�ܣ�"),MB_ICONERROR,30);
		}

		//��ѯ����
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ��������ϻ�ȡʧ�ܣ�"),MB_ICONERROR,30);
		}

		//������ʾ
		if (m_bQueryRebateMission==true)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����ѯ����ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}
	}

	//�¼�����
	OnMissionConclude();
	return true;
}


bool CDlgInsure::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_QUERY_TRANSFER_REBATE_RESULT:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_QueryTransferRebateResult));
				if (wDataSize<sizeof(CMD_GP_QueryTransferRebateResult)) return false;

				//��������
				CMD_GP_QueryTransferRebateResult * pUserInsureInfo=(CMD_GP_QueryTransferRebateResult *)pData;

				cbRebateEnabled = pUserInsureInfo->cbRebateEnabled;
				lRebateIngot =pUserInsureInfo->lIngot;
				lRebateLoveLiness = pUserInsureInfo->lLoveLiness;
				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				QueryUserInsureInfo();

				return true;
			}
		case SUB_GP_USER_INSURE_INFO:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//��������
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//���ñ���
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);
				
				UpdateUI();

				ShowPresent();

				

				return true;
			}
		case SUB_GP_USER_INSURE_ENABLE_RESULT:	//��ͨ���
			{
				//Ч�����
				ASSERT(wDataSize<=sizeof(CMD_GP_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GP_UserInsureEnableResult)) return false;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��������
				CMD_GP_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GP_UserInsureEnableResult *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//���ñ���
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//��ʾ��Ϣ
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				//��ѯ����
				if(pGlobalUserData->cbInsureEnabled==TRUE) QueryUserInsureInfo();

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//���гɹ�
			{
				//Ч�����
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);
				
				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//���ñ���
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));
				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);	

				//���ý���
				if(m_pItemAccess) m_pItemAccess->SetEditFocus();

				////���ý���
				//if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				//{
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				//}			

				//���½���
				UpdateUI();

				if (m_bIsTransfer)
				{
					CDlgPresentProof dlg;
					tagTransferScoreInfo TransferScoreInfo;
					TransferScoreInfo.lTransferScore = m_lScore;
					TransferScoreInfo.dwTargetID = m_dwTargetUserID;
					lstrcpyn(TransferScoreInfo.szTargetAccounts,m_szAccounts,CountArray(TransferScoreInfo.szTargetAccounts));
					lstrcpyn(TransferScoreInfo.szTransRemark,m_szTransRemark,CountArray(TransferScoreInfo.szTransRemark));
					dlg.SetTransferScoreInfo(TransferScoreInfo);
					dlg.DoModal();
					m_bIsTransfer = false;
				}
				else
				{
					//��ʾ��Ϣ
					if (pUserInsureSuccess->szDescribeString[0]!=0)
					{
						CDlgInformation Information(this);
						Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
					}	
				}			

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���ý���
				//if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass!= NULL)
				//{
				//	m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
				//	m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				//}

				////���ý���
				//if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				//{
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				//}

				m_bIsTransfer = false;

				//��ʾ��Ϣ
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_QUERY_USER_INFO_RESULT:	//�û���Ϣ
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GP_UserTransferUserInfo)) return false;

				//��������
				CMD_GP_UserTransferUserInfo * pTransferUserInfo=(CMD_GP_UserTransferUserInfo *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);

				//��������
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder? pMemberParameter->dwMemberInsure:m_UserInsureInfo.wRevenueTransfer;

				//������Ϣ
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				//CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("������Ϸ����ȡ%d��������,��ȷ��Ҫ��[%s], ID:%d ����%s ��Ϸ����?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				lstrcpyn(m_szAccounts,pTransferUserInfo->szAccounts,CountArray(m_szAccounts));
				//��ʾ��Ϣ
				CDlgInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//��������
					CMD_GP_UserTransferScore UserTransferScore;
					ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

					//��������
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//���ñ���
					m_dwTargetUserID = pTransferUserInfo->dwTargetGameID;
					UserTransferScore.lTransferScore=m_lScore;
					UserTransferScore.dwUserID=pGlobalUserData->dwUserID;
					CWHService::GetMachineIDEx(UserTransferScore.szMachineID);					
					lstrcpyn(UserTransferScore.szPassword,m_szInsurePass,CountArray(UserTransferScore.szPassword));
					lstrcpyn(UserTransferScore.szTransRemark,m_szTransRemark,CountArray(UserTransferScore.szTransRemark));
					lstrcpyn(UserTransferScore.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferScore.szAccounts));

					m_bIsTransfer = true;
					//��������
					m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_SCORE,&UserTransferScore,sizeof(UserTransferScore));
				}

				return true;
			}
		}
	}

	return true;
}

//��ʼ����
VOID CDlgInsure::OnMissionStart()
{
	return;
}

//��ֹ����
VOID CDlgInsure::OnMissionConclude()
{
	return;
}

VOID CDlgInsure::PerformQueryInfo()
{
	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}
}

VOID CDlgInsure::PerformQueryRebate()
{
	//���ñ���
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bTransferMission=false;
	m_bEnableMission=false;
	m_bQueryRebateMission=true;

	//�¼�����
	OnMissionStart();

	//ִ�в�ѯ
	PerformQueryInfo();

}


VOID CDlgInsure::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//���ñ���
	m_bEnableMission=true;
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bTransferMission=false;
	m_bQueryRebateMission=false;
	//���ñ���
	lstrcpyn(m_szLogonPass,pszLogonPass,CountArray(m_szLogonPass));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));	

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

VOID CDlgInsure::QueryUserInsureInfo()
{
	//���ñ���
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=true;
	m_bTransferMission=false;
	m_bEnableMission=false;
    m_bQueryRebateMission=false;
	//�¼�����
	OnMissionStart();

	//ִ�в�ѯ
	PerformQueryInfo();

	return;
}

//�����¼�
VOID CDlgInsure::PerformSaveScore(SCORE lSaveScore)
{
	//���ñ���	
	m_bSaveMission=true;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;
	m_bQueryRebateMission=false;

	//���ñ���
	m_lScore=lSaveScore;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//ȡ���¼�
VOID CDlgInsure::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//���ñ���	
	m_bTakeMission=true;
	m_bSaveMission=false;	
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;
	m_bQueryRebateMission=false;

	//���ñ���
	m_lScore=lTakeScore;
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

VOID CDlgInsure::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//���ñ���
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=true;
	m_bQueryRebateMission=false;

	//���ñ���
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

tagUserInsureInfo* CDlgInsure::GetUserInsureInfo()
{
	return &m_UserInsureInfo;
}

VOID CDlgInsure::InitData()
{

}

VOID CDlgInsure::LoadDataFromMemory()
{


}

VOID CDlgInsure::UpdateData()
{


}

VOID CDlgInsure::LoadDataFromUI()
{

}

VOID CDlgInsure::InitUI()
{
	m_pContainerWealth = (CContainerUI*)GetControlByName(szContainerWealthName);
	m_pContainerAccess = (CContainerUI*)GetControlByName(szContainerAccessName);
	m_pContainerPresent = (CContainerUI*)GetControlByName(szContainerPresentName);
	m_pContainerRecord = (CContainerUI*)GetControlByName(szContainerRecordName);
	
	ASSERT(m_pContainerWealth != NULL);
	ASSERT(m_pContainerAccess != NULL);
	ASSERT(m_pContainerPresent != NULL);
	ASSERT(m_pContainerRecord != NULL);
	
	if(m_pItemWealth==NULL) m_pItemWealth = new CInsureItemWealth(this);
	if(m_pItemAccess==NULL) m_pItemAccess = new CInsureItemAccess(this);
	if(m_pItemPresent==NULL) m_pItemPresent = new CInsureItemPresent(this);

	if(m_pItemWealth) m_pItemWealth->InitUI();
	if(m_pItemAccess) m_pItemAccess->InitUI();
	if(m_pItemPresent) m_pItemPresent->InitUI();

	UpdateUI();

	//����
	if (m_pContainerPresent)
	{
		m_pContainerPresent->SetVisible(false);
	}

	COptionUI *pOptionUI = static_cast<COptionUI *>(GetControlByName(TEXT("OptionInsurePresent")));
	if (pOptionUI)
	{
		pOptionUI->SetVisible(false);
	}
}

VOID CDlgInsure::UpdateUI()
{
	if(m_pItemWealth) m_pItemWealth->UpdateUI();
	if(m_pItemAccess) m_pItemAccess->UpdateUI();
	if(m_pItemPresent) m_pItemPresent->UpdateUI();	
}

VOID CDlgInsure::OnDefault()
{

}

VOID CDlgInsure::DoApply()
{
}

void CDlgInsure::ShowPresent()
{
	//����
	if (m_pContainerPresent)
	{
		m_pContainerPresent->SetVisible(m_UserInsureInfo.cbEnjoinTransfer ==0);
	}

	COptionUI *pOptionUI = static_cast<COptionUI *>(GetControlByName(TEXT("OptionInsurePresent")));
	if (pOptionUI)
	{
		pOptionUI->SetVisible(m_UserInsureInfo.cbEnjoinTransfer == 0);
	}
}

CContainerUI* CDlgInsure::GetWealthContainer()
{
	return m_pContainerWealth;
}

CContainerUI* CDlgInsure::GetAccessContainer()
{
	return m_pContainerAccess;
}

CContainerUI* CDlgInsure::GetPresentContainer()
{
	return m_pContainerPresent;
}

CContainerUI* CDlgInsure::GetRecordContainer()
{
	return m_pContainerRecord;
}



//////////////////////////////////////////////////////////////////////////////////
CInsureItemWealth::CInsureItemWealth(CDlgInsure* pDlgInsureMain):m_pDlgInsureMain(pDlgInsureMain)
{
	m_pContainerUI = pDlgInsureMain->GetWealthContainer();
}
CInsureItemWealth::~CInsureItemWealth()
{
}

VOID CInsureItemWealth::InitData()
{

}

VOID CInsureItemWealth::InitUI()
{
	
}

VOID CInsureItemWealth::UpdateUI()
{
	//��ȡ��Ϣ
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//��������
	TCHAR szBuffer[64]=TEXT("");

	TCHAR szServiceFee[64]=TEXT("");

	//Я����Ϸ��
	SetNumber(m_pContainerUI, szNumberGameScoreControlName, (double)pGlobalUserData->lUserScore);

	//������Ϸ��
	SetNumber(m_pContainerUI, szNumberInsureGameScoreControlName, (double)pGlobalUserData->lUserInsure);

	//��Ϸ��
	SetNumber(m_pContainerUI, szNumberGameBeanControlName, pGlobalUserData->dUserBeans);

	//Ԫ��
	SetNumber(m_pContainerUI, szNumberGameIngotControlName, (double)pGlobalUserData->lUserIngot);

	//����ֵ
	SetNumber(m_pContainerUI, szNumberGlamourValueControlName, (double)pGlobalUserData->lLoveLiness);

	//�Ƿ���ʾ������������
	bool bVisible = pGlobalUserData->cbInsureEnabled==FALSE;
	CContainerUI* pSetTip = static_cast<CContainerUI*>(m_pContainerUI->FindControl(szContainerSetTipControlName));
	if(pSetTip) pSetTip->SetVisible(bVisible);

	
}

void CInsureItemWealth::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	//����¼�
	if (msg.sType == TEXT("click"))
	{
		//��ͨ����
		if(pControlUI->GetName()== szButtonSetControlName)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_PERSONER,2L);

			m_pDlgInsureMain->OnCancel();

		}
		//��ȡ�ͱ�
		else if(pControlUI->GetName()== szButtonBaseEnsureControlName
				|| pControlUI->GetName()== szButtonGetBaseEnsureControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BASE_ENSURE,0L);

			m_pDlgInsureMain->OnCancel();

		}
		//��ֵ����
		else if(pControlUI->GetName()== szButtonRechargCenterControlName
				|| pControlUI->GetName()== szButtonRechargeControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//�ҵı���
		else if(pControlUI->GetName()== szButtonMyBagControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//Ԫ���һ�
		else if(pControlUI->GetName()== szButtonExchangeControlName
				|| pControlUI->GetName()== szButtonExchargeControlName1
				|| pControlUI->GetName()== szButtonExchargeControlName2)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_SHOP,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//��Ϸ�̳�
		else if(pControlUI->GetName()== szButtonGameShopControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_SHOP,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//��ֵ��¼
		else if(pControlUI->GetName()== szButtonRechargeRecrodControlName)
		{
			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szLinkInfo[256]=TEXT("");
			_sntprintf(szLinkInfo,CountArray(szLinkInfo),TEXT("%s/%sUserService/UserService3.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szLinkInfo,NULL,NULL,SW_NORMAL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
CInsureItemAccess::CInsureItemAccess(CDlgInsure* pDlgInsureMain):m_pDlgInsureMain(pDlgInsureMain)
,m_pEditInOut(NULL)
,m_LabelInOut(NULL)
,m_InOutScore(0)
{
	m_pContainerUI = pDlgInsureMain->GetAccessContainer();
}
CInsureItemAccess::~CInsureItemAccess()
{
}

VOID CInsureItemAccess::InitData()
{

}

VOID CInsureItemAccess::LoadDataFromUI()
{
	CNumberConvert nc;
	m_InOutScore = nc.GetUserControlScore(m_pEditInOut->GetText());
}

VOID CInsureItemAccess::InitUI()
{
	m_pEditInOut = static_cast<CEditUI*>(m_pContainerUI->FindControl(szEditInOutControlName));
	m_pEditPassWord = static_cast<CEditUI*>(m_pContainerUI->FindControl(szEditPassWordControlName));
	m_LabelInOut = static_cast<CLabelUI*>(m_pContainerUI->FindControl(szLabelInOutControlName));
}


VOID CInsureItemAccess::ShowCapital()
{
	LoadDataFromUI(); 
	TCHAR szBuffer[256]={0};
	TCHAR szSource[256]={0};
	lstrcpyn(szSource,m_pEditInOut->GetText(),256);
	CString str(szSource);
	str.Replace(TEXT(","),TEXT(""));
	CNumberConvert nc;
	nc.NumberConvert(str.GetBuffer(),szBuffer,str.GetLength());
	if(m_LabelInOut != NULL) m_LabelInOut->SetText(szBuffer);
}


VOID CInsureItemAccess::UpdateUI()
{
	//��ȡ��Ϣ
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//Я����Ϸ��
	CNumberUI* pNumberUserScore = static_cast<CNumberUI*>(m_pContainerUI->FindControl(szNumberUserScoreControlName));
	if(pNumberUserScore != NULL) 
	{
		float fUserScore = (float)pGlobalUserData->lUserScore;
		pNumberUserScore->SetNumber(fUserScore);
	}
	
	//��Ϸ���
	CNumberUI* pUserInsrueScore = static_cast<CNumberUI*>(m_pContainerUI->FindControl(szNumberAcessUserInsrueControlName));
	if(pUserInsrueScore != NULL) 
	{
		float fUserInsure = (float)pGlobalUserData->lUserInsure;
		pUserInsrueScore->SetNumber(fUserInsure);
	}

	//��ʾ��Ϣ
	TCHAR szNormalTip[128]=TEXT("");
	_sntprintf(szNormalTip,CountArray(szNormalTip),TEXT("��ʾ��������Ϸ���������ѣ�ȡ�����۳�%d��������ѡ�������������������롣"),m_pDlgInsureMain->m_UserInsureInfo.wRevenueTake);
	CControlUI * pNormalTip = m_pContainerUI->FindControl(szTextTakeTaxControlName);
	if (pNormalTip)
	{
		pNormalTip->SetText(szNormalTip);
	}

}

void CInsureItemAccess::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	//����¼�
	if (msg.sType == TEXT("click"))
	{
		if(pControlUI->GetName() == szButtonInControlName )
		{
			OnBnClickedSaveScore();
		}
		else if( pControlUI->GetName() == szButtonOutControlName )
		{
			OnBnClickedTakeScore();
		}
		else
		{
			OnBnClickAccess(msg);
		}
	}
	else if( msg.sType == TEXT("textchanged") )
	{
		if( pControlUI->GetName() == szEditInOutControlName )
		{
			ShowCapital();
		}
	}
}

void CInsureItemAccess::OnBnClickAccess(TNotifyUI& msg)
{

	const TCHAR* const szAccessButtonName[7]={szButtonW1ControlName, szButtonW5ControlName, szButtonW10ControlName, 
										  szButtonW50ControlName, szButtonW100ControlName, szButtonW1000ControlName, 
										  szButtonW10000ControlName};
	const DWORD dwAccessScore[7]={10000, 5*10000, 10*10000, 50*10000, 100*10000, 1000*10000, 10000*10000};

	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	CNumberConvert nc;
	m_InOutScore = nc.GetUserControlScore(m_pEditInOut->GetText());
	SCORE nAccessScore = m_InOutScore;
	for(int i = 0; i < CountArray(szAccessButtonName); i++)
	{
		if( msg.pSender->GetName() == szAccessButtonName[i] ){
			nAccessScore += dwAccessScore[i];
			SetEditNum(m_pContainerUI, szEditInOutControlName, nAccessScore);
			ShowCapital();
			return;
		}
	}
	
	if( msg.pSender->GetName() == szButtonAllInControlName){
		SetEditNum(m_pContainerUI, szEditInOutControlName, pGlobalUserData->lUserScore);
	}
	else if( msg.pSender->GetName() == szButtonAllOutControlName){
		SetEditNum(m_pContainerUI, szEditInOutControlName, pGlobalUserData->lUserInsure);
	}

	ShowCapital();
}

VOID CInsureItemAccess::OnBnClickedSaveScore()
{
	//��ȡUI����
	LoadDataFromUI();

	//��������
	SCORE lSaveScore = m_InOutScore;
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	//�����ж�
	if (lSaveScore<=0L)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("�������Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditInOut->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lSaveScore > pGlobalUserData->lUserScore)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("����Я����Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditInOut->SetFocus();

		return;
	}

	//ִ�д��
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformSaveScore(lSaveScore);
}

VOID CInsureItemAccess::OnBnClickedTakeScore()
{
	//��ȡUI����
	LoadDataFromUI();

	//��ȡ����
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	SCORE lTakeScore= m_InOutScore;

	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditPassWord->GetText(),CountArray(szPassword));

	//��������
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//�����ж�
	if (lTakeScore<=0L)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("ȡ������Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditInOut->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTakeScore > pGlobalUserData->lUserInsure)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("��������Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditInOut->SetFocus();

		return;
	}

	//�����ж�
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("�������벻��Ϊ�գ������������������룡"),MB_ICONERROR,30);

		//���ý���
		m_pEditPassWord->SetFocus();

		return;
	}

	//�¼�֪ͨ
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTakeScore(lTakeScore,szInsurePass);

	return;
}

void CInsureItemAccess::SetEditFocus()
{
	if(m_pEditPassWord){
		m_pEditPassWord->SetFocus();
		m_pEditPassWord->SetText(TEXT(""));
	}
	if(m_LabelInOut) m_LabelInOut->SetText(TEXT(""));
}

//////////////////////////////////////////////////////////////////
//������ͣ�������ҳ�棩
CInsureItemPresent::CInsureItemPresent(CDlgInsure* pDlgInsureMain):m_pDlgInsureMain(pDlgInsureMain)
,m_pRadioByNickName(NULL)
,m_pRadioByUserID(NULL)
,m_pEditAccount(NULL)
,m_pEditInsurePass(NULL)
,m_pEditAmount(NULL)
,m_pEditRemarks(NULL)
,m_pLabelTarget(NULL)
,m_pLabelChineseNumeral(NULL)
{
	m_pContainerUI = pDlgInsureMain->GetPresentContainer();
}

CInsureItemPresent::~CInsureItemPresent()
{
}

VOID CInsureItemPresent::InitData()
{

}

VOID CInsureItemPresent::InitUI()
{
	m_pRadioByNickName = static_cast<CRadioButtonUI*>(m_pContainerUI->FindControl(szRadioButtonPresentNickControlName));
	m_pRadioByUserID = static_cast<CRadioButtonUI*>(m_pContainerUI->FindControl(szRadioButtonPresentIDControlName));
	m_pEditAccount = static_cast<CEditUI*>(m_pContainerUI->FindControl(szEditPresentIDControlName));
	m_pEditInsurePass = static_cast<CEditUI*>(m_pContainerUI->FindControl(szEditPresentInsurePassWordControlName));
	m_pEditAmount = static_cast<CEditUI*>(m_pContainerUI->FindControl(szEditPresentAmountControlName));
	m_pEditRemarks = static_cast<CEditUI*>(m_pContainerUI->FindControl(szEditPresentBakControlName));
	m_pLabelTarget = static_cast<CLabelUI*>(m_pContainerUI->FindControl(szLabelPresentIDControlName));
	m_pLabelChineseNumeral = static_cast<CLabelUI*>(m_pContainerUI->FindControl(szLabelPresentChineseNumeralControlName));

	m_pRadioByUserID->SetCheck(true);
}

VOID CInsureItemPresent::UpdateUI()
{
	//��ȡ��Ϣ
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//��Ϸ���
	CNumberUI* pUserInsrueScore = static_cast<CNumberUI*>(m_pContainerUI->FindControl(szumberPresentUserInsrueControlName));
	if(pUserInsrueScore != NULL) 
	{
		float fUserInsure = (float)pGlobalUserData->lUserInsure;
		pUserInsrueScore->SetNumber(fUserInsure);
	}

	//�������
	CControlUI * pRebate = m_pContainerUI->FindControl(_T("VerticalLayoutRebate"));
	if (pRebate)
	{
		pRebate->SetVisible(m_pDlgInsureMain->cbRebateEnabled);
	}
	if (m_pDlgInsureMain->cbRebateEnabled)
	{
		{
			TCHAR szNormalTip[128]=TEXT("");
			_sntprintf(szNormalTip,CountArray(szNormalTip),TEXT("%d"),m_pDlgInsureMain->lRebateIngot);
			CControlUI * pRebateIngot = m_pContainerUI->FindControl(_T("LabelRebateIngot"));
			if (pRebateIngot)
			{
				pRebateIngot->SetText(szNormalTip);
			}
		}

		{
			TCHAR szNormalTip[128]=TEXT("");
			_sntprintf(szNormalTip,CountArray(szNormalTip),TEXT("%d"),m_pDlgInsureMain->lRebateLoveLiness);
			CControlUI * pRebateLoveLiness = m_pContainerUI->FindControl(_T("LabelRebateLoveLiness"));
			if (pRebateLoveLiness)
			{
				pRebateLoveLiness->SetText(szNormalTip);
			}
		}
	}


	//��ͨ��ʾ
	TCHAR szNormalTip[128]=TEXT("");
	_sntprintf(szNormalTip,CountArray(szNormalTip),TEXT("��ʾ����ͨ�����Ϸ��������۳�%d��������ѡ�"),m_pDlgInsureMain->m_UserInsureInfo.wRevenueTransfer);
	CControlUI * pNormalTip = m_pContainerUI->FindControl(szTextTransferTaxControlName);
	if (pNormalTip)
	{
		pNormalTip->SetText(szNormalTip);
	}

	//VIP��ʾ
	TCHAR szTaskRewardTotal[64]=TEXT("");
	if(pGlobalUserData->cbMemberOrder > 0)
	{
		//��Ա����
		const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
		if(pMemberParameter != NULL)
		{
			_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("�۳�%d�������ѡ�"),pMemberParameter->dwMemberInsure);
		}

		CControlUI * pContainerVip = m_pContainerUI->FindControl(szContainerVipAwardControlName);
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

			if(pMemberParameter->dwMemberInsure>0)
			{
				CControlUI * pControl4 = pContainerVip->FindControl(szLabelMemberTaxControlName);
				if(pControl4 !=NULL) 
				{
					pControl4->SetVisible(true);
					pControl4->SetText(szTaskRewardTotal);
				}
				CControlUI * pControl5 = pContainerVip->FindControl(szLabelMemberTaxFreeControlName);
				if(pControl5 !=NULL) 
				{
					pControl5->SetVisible(false);
				}
			}
			else
			{
				CControlUI * pControl4 = pContainerVip->FindControl(szLabelMemberTaxControlName);
				if(pControl4 !=NULL) 
				{
					pControl4->SetVisible(false);
				}
				CControlUI * pControl5 = pContainerVip->FindControl(szLabelMemberTaxFreeControlName);
				if(pControl5 !=NULL) 
				{
					pControl5->SetVisible(true);
				}
			}

		}

	}
	else
	{
		CControlUI * pContainerVip = m_pContainerUI->FindControl(szContainerVipAwardControlName);
		if(pContainerVip !=NULL)  pContainerVip->SetVisible(false);
	}

}

void CInsureItemPresent::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	//����¼�
	if (msg.sType == TEXT("click"))
	{
		//ȷ������
		if(pControlUI->GetName()== szButtonPresentControlName)
		{
			OnBnClickedTransferScore();
		}
		else if(pControlUI->GetName()== szRadioButtonPresentIDControlName)
		{
			OnBnCheckedTransferByUserID();
		}
		else if(pControlUI->GetName()== szRadioButtonPresentNickControlName)
		{
			OnBnCheckedTransferByNickName();
		}
	}
	else if( msg.sType == TEXT("textchanged") )
	{
		if( pControlUI->GetName() == szEditPresentAmountControlName )
		{
			CNumberConvert nc;
			SCORE PresentAmount = nc.GetUserControlScore(m_pEditAmount->GetText());
			TCHAR szBuffer[256]={0};
			TCHAR szSource[256]={0};
			lstrcpyn(szSource,m_pEditAmount->GetText(),256);
			CString str(szSource);
			str.Replace(TEXT(","),TEXT(""));
			nc.NumberConvert(str.GetBuffer(),szBuffer,str.GetLength());
			//SwitchScoreString(PresentAmount, szBuffer,CountArray(szBuffer));
			if(m_pLabelChineseNumeral != NULL) m_pLabelChineseNumeral->SetText(szBuffer);
		}
	}
}

VOID CInsureItemPresent::OnBnClickedTransferScore()
{
	//��ȡ��Ϣ
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//�����ж�
	if (pGlobalUserData->lUserInsure < m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite)
	{
		//��ʾ��Ϣ
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("��������Ϸ�ҵ���Ŀ�������ת������ %I64d���޷�����ת�ʲ�����"),m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite);
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		return;
	}

	//ת�ʽ��
	CNumberConvert nc;
	SCORE lTransferScore= nc.GetUserControlScore(m_pEditAmount->GetText());

	//ת�ʱ�ע
	TCHAR szTransRemark[LEN_TRANS_REMARK]=TEXT("");
	lstrcpyn(szTransRemark,m_pEditRemarks->GetText(),CountArray(szTransRemark));

	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditInsurePass->GetText(),CountArray(szPassword));

	//��������
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//��ȡ�ǳ�
	CString strAccounts = m_pEditAccount->GetText();

	//��ʽ�ǳ�
	strAccounts.TrimLeft();
	strAccounts.TrimRight();

	//�ǳ��ж�
	if (m_pRadioByNickName->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("������Ҫ���͵�����ǳƣ�"),MB_ICONERROR,30);

		//���ý���
		m_pEditAccount->SetFocus();

		return;
	}

	//ID���ж�
	if(m_pRadioByUserID->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("������Ҫ���͵����ID��"),MB_ICONERROR,30);

		//���ý���
		m_pEditAccount->SetFocus();

		return;
	}

	//�����ж�
	if (lTransferScore<=0L)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("���͵���Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTransferScore > m_pDlgInsureMain->GetUserInsureInfo()->lUserInsure)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("��������Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTransferScore<m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite)
	{
		//��ʾ��Ϣ
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("������ת�ʵ���Ϸ����Ŀ�������� %I64d���޷�����ת�ʲ�����"),m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite);
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//�����ж�
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("�������벻��Ϊ�գ������������������룡"),MB_ICONERROR,30);

		//���ý���
		m_pEditInsurePass->SetFocus();

		return;
	}

	//�¼�֪ͨ
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTransferScore(strAccounts,lTransferScore,szInsurePass,szTransRemark,m_pRadioByUserID->GetCheck()?FALSE:TRUE);

	//���ý���
	m_pEditInsurePass->SetFocus();
	m_pEditInsurePass->SetText(TEXT(""));

	return;
}


//�����ǳ�
VOID CInsureItemPresent::OnBnCheckedTransferByNickName()
{
	//���ÿؼ�
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetNumberOnly(false);
	m_pLabelTarget->SetText(TEXT("�����ǳƣ�"));
}

//����ID
VOID CInsureItemPresent::OnBnCheckedTransferByUserID()
{
	//���ÿؼ�
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetNumberOnly(true);
	m_pLabelTarget->SetText(TEXT("����I D��"));
}


//���캯��
CDlgInsureServerNew::CDlgInsureServerNew()
{
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;

	return;
}

//��������
CDlgInsureServerNew::~CDlgInsureServerNew()
{
}

//��ѯ�¼�
VOID CDlgInsureServerNew::PerformQueryInfo()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//���ñ���
	QueryInsureInfo.cbActivityGame=FALSE;
	lstrcpyn(QueryInsureInfo.szInsurePass,pGlobalUserData->szPassword,CountArray(QueryInsureInfo.szInsurePass));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return;
}

//��ͨ�¼�
VOID CDlgInsureServerNew::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CMD_GR_C_EnableInsureRequest EnableInsureResult;
	ZeroMemory(&EnableInsureResult,sizeof(EnableInsureResult));

	//���ñ���
	EnableInsureResult.cbActivityGame=FALSE;
	EnableInsureResult.dwUserID=pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(EnableInsureResult.szMachineID);
	lstrcpyn(EnableInsureResult.szLogonPass,pszLogonPass,CountArray(EnableInsureResult.szLogonPass));
	lstrcpyn(EnableInsureResult.szInsurePass,pszInsurePass,CountArray(EnableInsureResult.szInsurePass));		

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_ENABLE_INSURE_REQUEST,&EnableInsureResult,sizeof(EnableInsureResult));

	return;
}

//�����¼�
VOID CDlgInsureServerNew::PerformSaveScore(SCORE lSaveScore)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	ZeroMemory(&SaveScoreRequest,sizeof(SaveScoreRequest));

	//���ñ���
	SaveScoreRequest.lSaveScore=lSaveScore;

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//ȡ���¼�
VOID CDlgInsureServerNew::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//���ñ���
	TakeScoreRequest.lTakeScore=lTakeScore;
	lstrcpyn(TakeScoreRequest.szInsurePass,pszInsurePass,CountArray(TakeScoreRequest.szInsurePass));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	return;
}

//ת���¼�
VOID CDlgInsureServerNew::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//���ñ���
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//��������
	CMD_GR_C_QueryUserInfoRequest QueryUserInfoRequest;	
	ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	//���ñ���
	QueryUserInfoRequest.cbByNickName=m_cbByNickName;
	lstrcpyn(QueryUserInfoRequest.szAccounts,pszAccounts,CountArray(QueryUserInfoRequest.szAccounts));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	return;
}

//��������
VOID CDlgInsureServerNew::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}

//������Ϣ
bool CDlgInsureServerNew::OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��Ϣ�ж�
	if (wMainCmdID==MDM_GR_INSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_USER_INSURE_INFO:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

				//��������
				CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;

				//���ñ���
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				UpdateUI();

				ShowPresent();

				return true;
			}
		case SUB_GR_USER_INSURE_ENABLE_RESULT:	//��ͨ���
			{
				//Ч�����
				ASSERT(wDataSize<=sizeof(CMD_GR_S_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GR_S_UserInsureEnableResult)) return false;

				//��������
				CMD_GR_S_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GR_S_UserInsureEnableResult *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//���ñ���
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//��ʾ��Ϣ
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				////��������
				//RectifyInsureStep();

				return true;
			}
		case SUB_GR_USER_INSURE_SUCCESS:	//���гɹ�
			{
				//Ч�����
				CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//���ñ���
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));
				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);	

				//���ý���
				if(m_pItemAccess) m_pItemAccess->SetEditFocus();	

				//���½���
				UpdateUI();

				if (m_bIsTransfer)
				{
					CDlgPresentProof dlg;
					tagTransferScoreInfo TransferScoreInfo;
					TransferScoreInfo.lTransferScore = m_lScore;
					TransferScoreInfo.dwTargetID = m_dwTargetUserID;
					lstrcpyn(TransferScoreInfo.szTargetAccounts,m_szAccounts,CountArray(TransferScoreInfo.szTargetAccounts));
					lstrcpyn(TransferScoreInfo.szTransRemark,m_szTransRemark,CountArray(TransferScoreInfo.szTransRemark));
					dlg.SetTransferScoreInfo(TransferScoreInfo);
					dlg.DoModal();
					m_bIsTransfer = false;
				}
				else
				{
					//��ʾ��Ϣ
					if (pUserInsureSuccess->szDescribeString[0]!=0)
					{
						CDlgInformation Information(this);
						Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
					}	
				}

			

				return true;
			}
		case SUB_GR_USER_INSURE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GR_S_UserInsureFailure * pUserInsureFailure=(CMD_GR_S_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				////���ÿؼ�
				//if (m_DlgInsureOperater.m_bInitInfo==true)
				//{
				//	//���ý���
				//	if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass != NULL)
				//	{
				//		m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
				//		m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				//	}

				//	//���ý���
				//	if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				//	{
				//		m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
				//		m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				//	}
				//}

				m_bIsTransfer = false;

				//��ʾ��Ϣ
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GR_USER_TRANSFER_USER_INFO:  //�û���Ϣ
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserTransferUserInfo)) return false;

				//��������
				CMD_GR_S_UserTransferUserInfo * pTransferUserInfo=(CMD_GR_S_UserTransferUserInfo *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);

				//��������
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder?pMemberParameter->dwMemberInsure:m_UserInsureInfo.wRevenueTransfer;


				//������Ϣ
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				//CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("������Ϸ����ȡ%d��������,��ȷ��Ҫ��[%s], ID:%d ����%s ��Ϸ����?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				lstrcpyn(m_szAccounts,pTransferUserInfo->szAccounts,CountArray(m_szAccounts));
				//��ʾ��Ϣ
				CDlgInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//��������
					CMD_GP_C_TransferScoreRequest TransferScoreRequest;
					ZeroMemory(&TransferScoreRequest,sizeof(TransferScoreRequest));

					//���ñ���
					m_dwTargetUserID = pTransferUserInfo->dwTargetGameID;
					TransferScoreRequest.lTransferScore=m_lScore;					
					lstrcpyn(TransferScoreRequest.szInsurePass,m_szInsurePass,CountArray(TransferScoreRequest.szInsurePass));
					lstrcpyn(TransferScoreRequest.szTransRemark,m_szTransRemark,CountArray(TransferScoreRequest.szTransRemark));
					lstrcpyn(TransferScoreRequest.szAccounts,pTransferUserInfo->szAccounts,CountArray(TransferScoreRequest.szAccounts));

					m_bIsTransfer = true;
					//��������
					m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TRANSFER_SCORE_REQUEST,&TransferScoreRequest,sizeof(TransferScoreRequest));
				}
				return true;
			}
		}
	}

	return true;
}

VOID CDlgInsureServerNew::QueryUserInsureInfo()
{
	PerformQueryInfo();
}
