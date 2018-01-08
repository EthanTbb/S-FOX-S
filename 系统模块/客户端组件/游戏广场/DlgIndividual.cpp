#include "Stdafx.h"
#include "Resource.h"
#include "DlgIndividual.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "GlobalUnits.h"
#include "DlgCustomFace.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szEditUserIDControlName = TEXT("EditUserID");
const TCHAR* const szEditNickNameControlName = TEXT("EditNickName");
const TCHAR* const szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR* const szEditStandardLogonPWControlName = TEXT("EditStandardLogonPW");
const TCHAR* const szNumberUserLevelControlName = TEXT("NumberUserLevel");
const TCHAR* const szRadioMankindControlName = TEXT("RadioButtonMale");
const TCHAR* const szRadioFemaleControlName = TEXT("RadioButtonFemale");
const TCHAR* const szLabelVipControlName = TEXT("LabelVip");
const TCHAR* const szLableInsureTittleControlName = TEXT("LableInsureTittle");
const TCHAR* const szButtonStandardComfirmControlName = TEXT("ButtonStandardComfirm");
const TCHAR* const szRichSignControlName = TEXT("RichSign");
const TCHAR* const szEditSignControlName = TEXT("EditSign");
const TCHAR* const szLabelSignLimitTipControlName = TEXT("LabelSignLimitTip");
const TCHAR* const szButtonVipControlName = TEXT("ButtonVip");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
//��ϸ����
const TCHAR* const szEditMobilePhoneControlName = TEXT("EditMobilePhone");
const TCHAR* const szEditQQControlName = TEXT("EditQQ");
const TCHAR* const szEditEMailControlName = TEXT("EditEMail");
const TCHAR* const szEditSpreaderControlName = TEXT("EditSpreader");
const TCHAR* const szszEditAddressControlName = TEXT("szEditAddress");
const TCHAR* const szEditRealInfoPWControlName = TEXT("EditRealInfoPW");
const TCHAR* const szButtonRealInfoComfirmControlName = TEXT("ButtonRealInfoComfirm");
//ʵ����֤
const TCHAR* const szButtonRealAuthComfirmControlName = TEXT("ButtonRealAuthComfirm");
const TCHAR* const szEditRealAuthPWControlName = TEXT("EditRealAuthPW");
const TCHAR* const szEditRealNameControlName = TEXT("EditRealName");
const TCHAR* const szEditIDNumberControlName = TEXT("EditIDNumber");
const TCHAR* const szContainerRealAuthControlName = TEXT("ContainerRealAuth");
const TCHAR* const szContainerRealAuthTipPassControlName = TEXT("ContainerRealAuthTipPass");
const TCHAR* const szContainerRealAuthTipPassFlagControlName = TEXT("ContainerRealAuthTipPassFlag");
//�޸�����
const TCHAR* const szEditLogonPWOrgControlName = TEXT("EditLogonPWOrg");
const TCHAR* const szEditLogonPWNewControlName = TEXT("EditLogonPWNew");
const TCHAR* const szEditLogonPWComfirmControlName = TEXT("EditLogonPWComfirm");
const TCHAR* const szEditInsurePWOrgControlName = TEXT("EditInsurePWOrg");
const TCHAR* const szEditInsurePWNewControlName = TEXT("EditInsurePWNew");
const TCHAR* const szEditInsurePWComfirmControlName = TEXT("EditInsurePWComfirm");
const TCHAR* const szEditInsurePassControlName = TEXT("EditInsurePwd");
const TCHAR* const szEditInsurePass1ControlName = TEXT("EditInsurePwd2");
const TCHAR* const szEditInsureLogonControlName = TEXT("EditLogonPwd");
const TCHAR* const szPScaleLogonPWNewControlName = TEXT("PScaleLogonPWNew");
const TCHAR* const szPScaleInsurePWNewControlName = TEXT("PScaleInsurePWNew");
const TCHAR* const szPScaleInsurePWSetControlName = TEXT("PScaleInsurePWSet");
//��ͨ����
const TCHAR* const szButtonLogonPWModifyControlName = TEXT("ButtonLogonPWModify");
const TCHAR* const szButtonInsurePWModifyControlName = TEXT("ButtonInsurePWModify");
const TCHAR* const szButtonOpenControlName = TEXT("ButtonInsureOpen");
const TCHAR* const szButtonModifyAvatarControlName = TEXT("ButtonModifyAvatar");
//��ǩ�ؼ�
const TCHAR* const szOptionIndividualStandardControlName = TEXT("OptionIndividualStandard");
const TCHAR* const szOptionIndividualRealInfoControlName = TEXT("OptionIndividualRealInfo");
const TCHAR* const szOptionIndividualModifyControlName = TEXT("OptionIndividualModify");
const TCHAR* const szOptionIndividualRealAuthControlName = TEXT("OptionIndividualRealAuth");
const TCHAR* const szTabIndividualControlName = TEXT("TabLayoutIndividual");
const TCHAR* const szTabInsurePassControlName = TEXT("TabLayoutInsurePass");
const TCHAR* const szUserAvatarControlName = TEXT("UserAvatar");

//////////////////////////////////////////////////////////////////////////////////
//�����¼�
const BYTE byMissionNone					=		0	;			//û������
const BYTE byMissionIndividualLoad			=		1	;			//��ѯ����
const BYTE byMissionIndividualModify		=		2	;			//�޸�����
const BYTE byMissionIndividualAuth			=		3	;			//ʵ����֤
const BYTE byMissionLogonModify				=		4	;			//��¼����
const BYTE byMissionInsureModify			=		5	;			//��������
const BYTE byMissionInsureSet				=		6	;			//��������

//////////////////////////////////////////////////////////////////////////////////


//���캯��
CDlgIndividual::CDlgIndividual(): CFGuiDialog(IDD_DLG_POPUP)
{
	//��������
	m_byMissionType = byMissionNone;
	m_MissionManager.InsertMissionItem(this);

	m_cbSelectedIndex = 0;
}

//��������
CDlgIndividual::~CDlgIndividual()
{

}


//��������
BOOL CDlgIndividual::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("��������"));

	//���д���
	CenterWindow();

	//���½���
	UpdateUI();

	AvtiveMission(byMissionIndividualLoad);


	return TRUE;
}

//ȷ������
VOID CDlgIndividual::OnOK()
{

	//return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgIndividual::OnCancel()
{
	return __super::OnCancel();
}

void CDlgIndividual::InitControlUI()
{
	//��ȡ�ؼ�
	CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabIndividualControlName));
	COptionUI* pOptionUI[4];
	pOptionUI[0] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualStandardControlName));
	pOptionUI[1] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualRealInfoControlName));
	pOptionUI[2] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualModifyControlName));
	pOptionUI[3] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualRealAuthControlName));
	

	//����ҳ��
	if(pTabLayoutUI != NULL && pOptionUI[m_cbSelectedIndex] != NULL)
	{
		pTabLayoutUI->SelectItem(m_cbSelectedIndex);
		pOptionUI[m_cbSelectedIndex]->Selected(true);

	}

	if(m_cbSelectedIndex == 2)
	{
		//������������
		if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled==FALSE)
		{
			pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabInsurePassControlName));
			if(pTabLayoutUI != NULL) pTabLayoutUI->SelectItem(1);
		}
	}

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	if(pGlobalUserData->CustomFaceInfo.dwDataSize != 0)
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID,m_CustomFaceInfo96);
	}

	UpdateUserFace();

	if (CGlobalUnits::GetInstance()->GetAuthRealParameter()->dwAuthentDisable)
	{
		CControlUI *pControl = GetControlByName(szOptionIndividualRealAuthControlName);
		if (pControl!=NULL)
		{
			pControl->SetVisible(false);
		}
	}
}

//��Ϣ����
void CDlgIndividual::Notify(TNotifyUI &  msg)
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
		//vip����
		else if(lstrcmp(pControlUI->GetName(), szButtonVipControlName)==0)
		{
			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szVipLinkInfo[256]=TEXT("");
			_sntprintf(szVipLinkInfo,CountArray(szVipLinkInfo),TEXT("%s/%sUserService/UserService5.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szVipLinkInfo,NULL,NULL,SW_NORMAL);
		}
		//�˺�����
		if(pControlUI->GetName() == szButtonStandardComfirmControlName)
		{
			if(EfficacyStandard())
			{
				AvtiveMission(byMissionIndividualModify);
			}

		}
		//ʵ������
		if(pControlUI->GetName() == szButtonRealInfoComfirmControlName )
		{
			if(EfficacyExtendInfo())
			{
				AvtiveMission(byMissionIndividualModify);
			}
		}
		else if(pControlUI->GetName() == szButtonRealAuthComfirmControlName )
		{
			if(EfficacyReal())
			{
				AvtiveMission(byMissionIndividualAuth);
			}
		}
		//�޸ĵ�¼����
		else if(pControlUI->GetName() == szButtonLogonPWModifyControlName)
		{
			if(EfficacyModifyLogonPassward())
			{
				AvtiveMission(byMissionLogonModify);
			}

		}
		//�޸���������
		else if(pControlUI->GetName() == szButtonInsurePWModifyControlName)
		{
			if(EfficacyModifyInsurePassward())
			{
				AvtiveMission(byMissionInsureModify);
			}

		}
		//������������
		else if(pControlUI->GetName() == szButtonOpenControlName)
		{
			if(EfficacyOpenInsurePassward())
			{
				AvtiveMission(byMissionInsureSet);
			}
		}
		//����ͷ��
		else if(pControlUI->GetName() == szButtonModifyAvatarControlName)
		{
			CDlgCustomFace DlgCustomFace;
			DlgCustomFace.SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(CPlatformFrame::GetInstance(),IUnknownEx));
			//��ʾ����
			DlgCustomFace.DoModal();

			//�û���Ϣ
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			if(pGlobalUserData->CustomFaceInfo.dwDataSize != 0)
			{
				CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
				pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID,m_CustomFaceInfo96);
			}
			UpdateUserFace();
		}

	}
	else if(lstrcmp(msg.sType, TEXT("selectchanged")) == 0)
	{
		if(pControlUI->GetName() == szOptionIndividualStandardControlName)
		{
			m_cbSelectedIndex = 0;
			return;
		}
		else if(pControlUI->GetName() == szOptionIndividualRealInfoControlName)
		{
			m_cbSelectedIndex = 1;
			AvtiveMission(byMissionIndividualLoad);

			return;
		}
		else if(pControlUI->GetName() == szOptionIndividualModifyControlName)
		{
			m_cbSelectedIndex = 2;
			//������������
			if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled==FALSE)
			{
				CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabInsurePassControlName));
				if(pTabLayoutUI != NULL) pTabLayoutUI->SelectItem(1);
			}

			return;
		}
		else if(pControlUI->GetName() == szOptionIndividualRealAuthControlName)
		{
			m_cbSelectedIndex = 3;
			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) == 0)
	{	
		if(lstrcmp(pControlUI->GetName(), szEditLogonPWNewControlName) == 0)
		{
			//����ǿ��
			SetPassLevel(szPScaleLogonPWNewControlName, szEditLogonPWNewControlName);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szEditInsurePWNewControlName) == 0)
		{
			//����ǿ��
			SetPassLevel(szPScaleInsurePWNewControlName, szEditInsurePWNewControlName);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(),szEditInsurePass1ControlName) == 0)
		{
			//����ǿ��
			SetPassLevel(szPScaleInsurePWSetControlName, szEditInsurePass1ControlName);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(),szEditSignControlName) == 0)
		{
			//�û�ǩ��
			CEditUI * pEdit = static_cast<CEditUI *>(GetControlByName(szEditSignControlName));
			if (pEdit)
			{
				int nMaxCharNum = 0;
				int nCurrCharNum = 0;
				nCurrCharNum = (int)(_tcslen(pEdit->GetText()));
				nMaxCharNum = pEdit->GetMaxChar();

				TCHAR szText[128]={0};	
				_sntprintf(szText, CountArray(szText), TEXT("(����������%d��)"), nMaxCharNum - nCurrCharNum);

				CLabelUI *pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelSignLimitTipControlName));
				if(pLabelUI)
				{
					pLabelUI->SetText(szText);

				}
			}

			return;
		}
	}
	
	return;
}

LPCTSTR CDlgIndividual::GetSkinFile()
{
	return TEXT("DlgIndividual.xml");
}

//�����¼�
bool CDlgIndividual::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		OnMissionLinkError(nErrorCode);
		return true;
	}

	switch(m_byMissionType)
	{
	case byMissionIndividualLoad:
		{
			OnMissionLinkIndividualLoad();
			break;
		}
	case byMissionIndividualModify:
		{
			OnMissionLinkIndividualModify();
			break;
		}
	case byMissionIndividualAuth:
		{
			OnMissionLinkIndividualAuth();
			break;
		}
	case byMissionLogonModify:
		{
			OnMissionLinkLogonModify();
			break;
		}
	case byMissionInsureSet:
		{
			OnMissionLinkInsureSet();
			break;
		}
	case byMissionInsureModify:
		{
			OnMissionLinkInsureModify();
			break;
		}
	default:
		return true;
	}
	return true;
}

//�ر��¼�
bool CDlgIndividual::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//������ʾ
		if (m_byMissionType == byMissionIndividualLoad)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���û����ϻ�ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_byMissionType == byMissionIndividualModify)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���û������޸�ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_byMissionType == byMissionInsureModify || m_byMissionType == byMissionLogonModify)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ�������޸�ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}
	}

	return true;
}

//��ȡ�¼�
bool CDlgIndividual::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:		//����ʧ��
			{
				//Ч�����
				CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				//OnMissionConclude();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��ʾ��Ϣ
				if (pOperateFailure->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_INDIVIDUAL_RESULT:
			{
				//��������
				CMD_GP_IndividuaResult * pIndividualResult=(CMD_GP_IndividuaResult *)pData;

				//Ч������
				ASSERT(wDataSize>=(sizeof(CMD_GP_IndividuaResult)-sizeof(pIndividualResult->szNotifyContent)));
				if (wDataSize<(sizeof(CMD_GP_IndividuaResult)-sizeof(pIndividualResult->szNotifyContent))) return false;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//���ñ���					
				pGlobalUserData->lUserScore = pIndividualResult->lCurrScore;

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) 
				{
					pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
					pPlatformEvent->SendPlatformEvent(EVENT_USER_HIDE_REAL_TIP,0L);
				}

				//��������
				UpdateData();

				//���½���
				UpdateUI();

				//��ʾ��Ϣ
				if (pIndividualResult->szNotifyContent[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pIndividualResult->szNotifyContent,MB_ICONINFORMATION,60);
				}

				//���´���

				return true;
			}
		case SUB_GP_OPERATE_SUCCESS:	//�����ɹ�
			{
				//��������
				CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;

				//Ч������
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

				//��������
				UpdateData();

				//���½���
				UpdateUI();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��ʾ��Ϣ
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		case SUB_GP_USER_INDIVIDUAL:	//������Ϣ
			{
				//Ч�����
				ASSERT(wDataSize>=sizeof(CMD_GP_UserIndividual));
				if (wDataSize<sizeof(CMD_GP_UserIndividual)) return false;

				//��������
				CMD_GP_UserIndividual * pUserIndividual=(CMD_GP_UserIndividual *)pData;

				//��������
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

				//���ñ���
				pIndividualUserData->dwUserID=pUserIndividual->dwUserID;

				
				//��������
				VOID * pDataBuffer=NULL;
				tagDataDescribe DataDescribe;
				CRecvPacketHelper RecvPacket(pUserIndividual+1,wDataSize-sizeof(CMD_GP_UserIndividual));

				//��չ��Ϣ
				while (true)
				{
					pDataBuffer=RecvPacket.GetData(DataDescribe);
					if (DataDescribe.wDataDescribe==DTP_NULL) break;
					switch (DataDescribe.wDataDescribe)
					{
					case DTP_GP_UI_USER_NOTE:		//�û���ע
						{
							lstrcpyn(pIndividualUserData->szUserNote,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szUserNote));
							break;
						}
					case DTP_GP_UI_COMPELLATION:	//��ʵ����
						{
							lstrcpyn(pIndividualUserData->szCompellation,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szCompellation));
							break;
						}
					case DTP_GP_UI_PASSPORTID:	//���֤
						{
							lstrcpyn(pIndividualUserData->szPassPortID,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szPassPortID));
							break;
						}
					case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
						{
							lstrcpyn(pIndividualUserData->szSeatPhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szSeatPhone));
							break;
						}
					case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
						{
							lstrcpyn(pIndividualUserData->szMobilePhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szMobilePhone));
							break;
						}
					case DTP_GP_UI_QQ:				//Q Q ����
						{
							lstrcpyn(pIndividualUserData->szQQ,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szQQ));
							break;
						}
					case DTP_GP_UI_EMAIL:			//�����ʼ�
						{
							lstrcpyn(pIndividualUserData->szEMail,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szEMail));
							break;
						}
					case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
						{
							lstrcpyn(pIndividualUserData->szDwellingPlace,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szDwellingPlace));
							break;
						}
					}
				}

				//�ɹ�����
				//OnMissionConclude();

				//��������
				//UpdateIndividualInfo();
				
				UpdateUI();
				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

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

				//������������
				if(pGlobalUserData->cbInsureEnabled==TRUE)
				{
					CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabInsurePassControlName));
					if(pTabLayoutUI != NULL) pTabLayoutUI->SelectItem(0);
				}

				//��ʾ��Ϣ
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}
				else
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(TEXT("�����������óɹ���"),MB_ICONINFORMATION,60);
				}

				return true;
			}

		}
	}

	return true;
}


//��ѯ����
void CDlgIndividual::OnMissionLinkIndividualLoad()
{
	//��������
	CMD_GP_QueryIndividual QueryIndividual;
	ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ���
	QueryIndividual.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(QueryIndividual.szPassword,pGlobalUserData->szPassword,CountArray(QueryIndividual.szPassword));

	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));

	return;
}

//�޸�����
void CDlgIndividual::OnMissionLinkIndividualModify()
{
	//��������
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	ZeroMemory(cbBuffer,sizeof(cbBuffer));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//��������
	CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_ModifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_ModifyIndividual));

	//���ñ���
	pModifyIndividual->dwUserID=pGlobalUserData->dwUserID;

	
	//��ȡ����
	TCHAR szItemText[128] = {0};
	if(m_cbSelectedIndex == 0)
		GetControlItemText(szEditStandardLogonPWControlName,szItemText,CountArray(szItemText));
	else if(m_cbSelectedIndex == 1)
		GetControlItemText(szEditRealInfoPWControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		//��������
		TCHAR szPassword[128] = {0};
		CWHEncrypt::MD5Encrypt(szItemText,szPassword);
		lstrcpyn(pModifyIndividual->szPassword,szPassword,CountArray(pModifyIndividual->szPassword));
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	if (m_cbSelectedIndex ==0)
	{
		//�û��ǳ�
		GetControlItemText(szEditNickNameControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0)
		{
			SendPacket.AddPacket(szItemText,DTP_GP_UI_NICKNAME);
			ZeroMemory(szItemText,sizeof(szItemText));
		}

		//�û��Ա�
		CRadioButtonUI * pManButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioMankindControlName));
		CRadioButtonUI * pFemaleButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioFemaleControlName));
		if (pManButtonUI && pManButtonUI->GetCheck() ) pModifyIndividual->cbGender=GENDER_MANKIND;
		if (pFemaleButtonUI && pFemaleButtonUI->GetCheck()) pModifyIndividual->cbGender=GENDER_FEMALE;


		//�û�ǩ��
		GetControlItemText(szEditSignControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0)
		{
			SendPacket.AddPacket(szItemText,DTP_GP_UI_UNDER_WRITE);
			ZeroMemory(szItemText,sizeof(szItemText));
		}
	}
	else
	{
		//�û��ǳ�
		SendPacket.AddPacket(pGlobalUserData->szNickName,DTP_GP_UI_NICKNAME);

		//�û��Ա�
		pModifyIndividual->cbGender= pGlobalUserData->cbGender;

		//�û�ǩ��
		SendPacket.AddPacket(pGlobalUserData->szUnderWrite,DTP_GP_UI_UNDER_WRITE);
	}

	if (m_cbSelectedIndex ==1)
	{
		//�Ƽ���ʶ
		if (pGlobalUserData->szSpreader[0] ==0)
		{
			GetControlItemText(szEditSpreaderControlName,szItemText,CountArray(szItemText));
			//ת����ֵ
			DWORD dwSpreaderID = _ttoi(szItemText);
			_sntprintf(szItemText,CountArray(szItemText),TEXT("%d"),dwSpreaderID);
			if (szItemText[0]!=0) 
			{
				SendPacket.AddPacket(szItemText,DTP_GP_UI_SPREADER);
				ZeroMemory(szItemText,sizeof(szItemText));
			}
		}

		//�ֻ�����
		GetControlItemText(szEditMobilePhoneControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_MOBILE_PHONE);
		ZeroMemory(szItemText,sizeof(szItemText));


		//Q Q ����
		GetControlItemText(szEditQQControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_QQ);
		ZeroMemory(szItemText,sizeof(szItemText));

		//�����ʼ�
		GetControlItemText(szEditEMailControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_EMAIL);
		ZeroMemory(szItemText,sizeof(szItemText));

		//��ϸ��ַ
		GetControlItemText(szszEditAddressControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_DWELLING_PLACE);
		ZeroMemory(szItemText,sizeof(szItemText));
	}
	else
	{
		//�̶�����
		SendPacket.AddPacket(pIndividualUserData->szSeatPhone,DTP_GP_UI_SEAT_PHONE);

		//�ֻ�����
		SendPacket.AddPacket(pIndividualUserData->szMobilePhone,DTP_GP_UI_MOBILE_PHONE);

		//Q Q ����
		SendPacket.AddPacket(pIndividualUserData->szQQ,DTP_GP_UI_QQ);

		//�����ʼ�
		SendPacket.AddPacket(pIndividualUserData->szEMail,DTP_GP_UI_EMAIL);

		//��ϸ��ַ
		SendPacket.AddPacket(pIndividualUserData->szDwellingPlace,DTP_GP_UI_DWELLING_PLACE);
	}

	//��������
	WORD wSendSize=sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INDIVIDUAL,cbBuffer,wSendSize);
	return;
}

void CDlgIndividual::OnMissionLinkIndividualAuth()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//��ȡ�û�
	CMD_GP_RealAuth CmdParameter;
	ZeroMemory(&CmdParameter,sizeof(CmdParameter));
	CmdParameter.dwUserID=pGlobalUserData->dwUserID;

	//��ȡ����
	TCHAR szItemText[128] = {0};
	GetControlItemText(szEditRealAuthPWControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		TCHAR szPassword[128] = {0};
		CWHEncrypt::MD5Encrypt(szItemText,szPassword);
		lstrcpyn(CmdParameter.szPassword,szPassword,CountArray(CmdParameter.szPassword));
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	//��ȡ����
	GetControlItemText(szEditRealNameControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		lstrcpyn(CmdParameter.szCompellation,szItemText,CountArray(CmdParameter.szCompellation));
		
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	//��ȡ���
	GetControlItemText(szEditIDNumberControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		lstrcpyn(CmdParameter.szPassPortID,szItemText,CountArray(CmdParameter.szPassPortID));
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_REAL_AUTH_QUERY,&CmdParameter,sizeof(CmdParameter));
}

//��¼����
void CDlgIndividual::OnMissionLinkLogonModify()
{
	//��������
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ȡ����
	CEditUI * pEditPass1Control = (CEditUI *)GetControlByName(szEditLogonPWOrgControlName);
	if(pEditPass1Control==NULL) return; 
	lstrcpyn(szSrcPassword,pEditPass1Control->GetText(),CountArray(szSrcPassword));

	//��ȡ����
	CEditUI * pEditPass2Control = (CEditUI *)GetControlByName(szEditLogonPWNewControlName);
	if(pEditPass2Control==NULL) return; 
	lstrcpyn(szDesPassword[0],pEditPass2Control->GetText(),CountArray(szDesPassword[0]));

	//��ȡ����
	CEditUI * pEditPass3Control = (CEditUI *)GetControlByName(szEditLogonPWComfirmControlName);
	if(pEditPass3Control==NULL) return; 
	lstrcpyn(szDesPassword[1],pEditPass3Control->GetText(),CountArray(szDesPassword[1]));

	//��������
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szEncryptPassword);

	//��������
	CMD_GP_ModifyLogonPass ModifyLogonPass;
	ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

	//��������
	TCHAR szSrcPasswordMD5[LEN_PASSWORD]=TEXT("");
	TCHAR szDesPasswordMD5[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szSrcPasswordMD5);
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szDesPasswordMD5);

	//��������
	ModifyLogonPass.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(ModifyLogonPass.szScrPassword,szSrcPasswordMD5,CountArray(ModifyLogonPass.szScrPassword));
	lstrcpyn(ModifyLogonPass.szDesPassword,szDesPasswordMD5,CountArray(ModifyLogonPass.szDesPassword));

	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_LOGON_PASS,&ModifyLogonPass,sizeof(ModifyLogonPass));

	return;
}

//��������
void CDlgIndividual::OnMissionLinkInsureModify()
{
	//��������
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ȡ����
	CEditUI * pEditPass1Control = (CEditUI *)GetControlByName(szEditInsurePWOrgControlName);
	if(pEditPass1Control==NULL) return; 
	lstrcpyn(szSrcPassword,pEditPass1Control->GetText(),CountArray(szSrcPassword));

	//��ȡ����
	CEditUI * pEditPass2Control = (CEditUI *)GetControlByName(szEditInsurePWNewControlName);
	if(pEditPass2Control==NULL) return; 
	lstrcpyn(szDesPassword[0],pEditPass2Control->GetText(),CountArray(szDesPassword[0]));

	//��ȡ����
	CEditUI * pEditPass3Control = (CEditUI *)GetControlByName(szEditInsurePWComfirmControlName);
	if(pEditPass3Control==NULL) return; 
	lstrcpyn(szDesPassword[1],pEditPass3Control->GetText(),CountArray(szDesPassword[1]));

	//��������
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szEncryptPassword);

	//��������
	CMD_GP_ModifyInsurePass ModifyInsurePass;
	ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

	//��������
	TCHAR szSrcPasswordMD5[LEN_PASSWORD]=TEXT("");
	TCHAR szDesPasswordMD5[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szSrcPasswordMD5);
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szDesPasswordMD5);

	//��������
	ModifyInsurePass.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(ModifyInsurePass.szScrPassword,szSrcPasswordMD5,CountArray(ModifyInsurePass.szScrPassword));
	lstrcpyn(ModifyInsurePass.szDesPassword,szDesPasswordMD5,CountArray(ModifyInsurePass.szDesPassword));

	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&ModifyInsurePass,sizeof(ModifyInsurePass));
	return;
}

//��������
void CDlgIndividual::OnMissionLinkInsureSet()
{
	//������Ϣ
	TCHAR szLogonPass[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass1[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass2[LEN_PASSWORD]=TEXT("");

	//��ȡ����
	GetControlItemText(szEditInsurePassControlName,szInsurePass1,CountArray(szInsurePass1));
	GetControlItemText(szEditInsurePass1ControlName,szInsurePass2,CountArray(szInsurePass2));
	GetControlItemText(szEditInsureLogonControlName,szLogonPass,CountArray(szLogonPass));

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//�������
	TCHAR szEnciryptInsurePass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szInsurePass1,szEnciryptInsurePass);

	//�������
	TCHAR szEnciryptLogonPass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szLogonPass,szEnciryptLogonPass);

	//��������
	CMD_GP_UserEnableInsure UserEnableInsure;
	ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ���
	UserEnableInsure.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(UserEnableInsure.szLogonPass,szEnciryptLogonPass,CountArray(UserEnableInsure.szLogonPass));
	lstrcpyn(UserEnableInsure.szInsurePass,szEnciryptInsurePass,CountArray(UserEnableInsure.szInsurePass));		
	CWHService::GetMachineIDEx(UserEnableInsure.szMachineID);

	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_ENABLE_INSURE,&UserEnableInsure,sizeof(UserEnableInsure));

	return;
}

bool CDlgIndividual::ConcludeMission()
{
	m_byMissionType = byMissionNone;
	m_MissionManager.ConcludeMissionItem(this,false);
	return true;
}

//��ʼ����
bool CDlgIndividual::AvtiveMission( BYTE byMissionType )
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

void CDlgIndividual::OnMissionLinkError(INT nErrorCode)
{
	switch(m_byMissionType)
	{
	case byMissionIndividualLoad:
		{
			CDlgInformation Information(this); 
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���û����ϻ�ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
			break;
		}
	case byMissionIndividualModify:
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���û������޸�ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
			break;
		}
	case byMissionInsureModify:
	case byMissionLogonModify:
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ�������޸�ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
			break;
		}
	default:
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("���������еķ��������޷��ɹ����ӷ���������������վά�����棡"),MB_ICONERROR);
		}
	}
}

void CDlgIndividual::OnMissionShutError(BYTE cbShutReason)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("���ڵ�ǰ����������æ����������ʧ�ܣ����Ժ������ԣ�"),MB_ICONERROR);
}

//���½���
void CDlgIndividual::UpdateUI()
{
	//��ȡ����
	CUserItemElement * pUserItemElement = CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//�û��ǳ�
	SetControlItemText(szEditNickNameControlName,pGlobalUserData->szNickName);

	//�û��ʺ�
	SetControlItemText(szEditAccountsControlName,pGlobalUserData->szAccounts);
	
	//�û�����	
	TCHAR szText[128]={0};	
	_sntprintf(szText, CountArray(szText), TEXT("%d"), pGlobalUserData->dwGameID);
	SetControlItemText(szEditUserIDControlName,szText);

	//�û��ȼ�
	CNumberUI* pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberUserLevelControlName));
	if(pNumberUI != NULL)
	{
		pNumberUI->SetNumber(pUserItemElement->GetExperienceLevel(pGlobalUserData->dwExperience));
	}

	//��Ա�ȼ�
	for (int i=0;i<6;i++)
	{
		CStdString strLevel;
		strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
		CControlUI* pLabelUI = GetControlByName(strLevel);
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

	//�û��Ա�
	CRadioButtonUI* pRadioFemale = static_cast<CRadioButtonUI *>(GetControlByName(szRadioFemaleControlName));
	if (pRadioFemale != NULL)
	{
		pRadioFemale->SetCheck(pGlobalUserData->cbGender == GENDER_FEMALE);
	}
	CRadioButtonUI* pRadioMale = static_cast<CRadioButtonUI *>(GetControlByName(szRadioMankindControlName));
	if(pRadioMale != NULL)
	{
        pRadioMale->SetCheck(pGlobalUserData->cbGender == GENDER_MANKIND);
	}

	//�û�ǩ��
	SetControlItemText(szEditSignControlName,pGlobalUserData->szUnderWrite);

	//ǩ����ʾ
	CEditUI * pEdit = static_cast<CEditUI *>(GetControlByName(szEditSignControlName));
	if (pEdit)
	{
		int nMaxCharNum = 0;
		int nCurrCharNum = 0;
		nCurrCharNum = (int)(_tcslen(pEdit->GetText()));
		nMaxCharNum = pEdit->GetMaxChar();

		TCHAR szText[128]={0};	
		_sntprintf(szText, CountArray(szText), TEXT("(����������%d��)"), nMaxCharNum - nCurrCharNum);

		CLabelUI *pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelSignLimitTipControlName));
		if(pLabelUI)
		{
			pLabelUI->SetText(szText);

		}
	}

	//�ֻ�����
	TCHAR szMobilePhone[LEN_MOBILE_PHONE]=TEXT("");
	pUserItemElement->ProtectMobilePhone(pIndividualUserData->szMobilePhone,szMobilePhone,CountArray(szMobilePhone),'*');
	SetControlItemText(szEditMobilePhoneControlName,szMobilePhone);

	//QQ����
	SetControlItemText(szEditQQControlName,pIndividualUserData->szQQ);

	//�����ʼ�
	SetControlItemText(szEditEMailControlName,pIndividualUserData->szEMail);

	//��ϸ��ַ
	SetControlItemText(szszEditAddressControlName,pIndividualUserData->szDwellingPlace);


	//��ʵ����
	if (pIndividualUserData->szCompellation[0] != 0)
	{
		TCHAR szCompellation[LEN_COMPELLATION]=TEXT("");
		pUserItemElement->ProtectCompellation(pIndividualUserData->szCompellation,szCompellation,CountArray(szCompellation),'*');
		SetControlItemText(szEditRealNameControlName,szCompellation);
		CControlUI * pControlUI = GetControlByName(szEditRealNameControlName);
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
		}
	}

	//��ݱ�ʶ
	if(pIndividualUserData->szPassPortID[0] != 0)
	{
		TCHAR szPassPortID[LEN_PASS_PORT_ID]=TEXT("");
		pUserItemElement->ProtectPassPortID(pIndividualUserData->szPassPortID,szPassPortID,CountArray(szPassPortID),'*');
		SetControlItemText(szEditIDNumberControlName,szPassPortID);
		CControlUI *pControlUI = GetControlByName(szEditIDNumberControlName);
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
		}

		pControlUI = GetControlByName(szContainerRealAuthTipPassControlName);
		if (pControlUI)
		{
			pControlUI->SetVisible(true);
		}
		pControlUI = GetControlByName(szContainerRealAuthTipPassFlagControlName);
		if (pControlUI)
		{
			pControlUI->SetVisible(true);
		}
		pControlUI = GetControlByName(szContainerRealAuthControlName);
		if (pControlUI)
		{
			pControlUI->SetVisible(false);
		}
	}

	//�ƹ��ʶ
	if (pGlobalUserData->szSpreader[0]!= 0)
	{
		SetControlItemText(szEditSpreaderControlName,pGlobalUserData->szSpreader);
		CControlUI * pControlUI = GetControlByName(szEditSpreaderControlName);
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
		}
	}
}

//��������
void CDlgIndividual::UpdateData()
{

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();


	if(m_byMissionType == byMissionLogonModify)
	{
		//��������
		TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
		TCHAR szDesPasswordMD5[LEN_PASSWORD]=TEXT("");
		CEditUI* pEditUI = static_cast<CEditUI *>(GetControlByName(szEditLogonPWComfirmControlName));
		if(pEditUI != NULL)
		{
			lstrcpyn(szDesPassword,pEditUI->GetText(),CountArray(szDesPassword));
		}
		else
		{
			return;
		}
		CWHEncrypt::MD5Encrypt(szDesPassword,szDesPasswordMD5);
		lstrcpyn(pGlobalUserData->szPassword,szDesPasswordMD5,CountArray(pGlobalUserData->szPassword));

		//��¼����
		if (pParameterGlobal->m_bRemberPassword==true)
		{
			//������Ϣ
			TCHAR szKeyUserID[16];
			_sntprintf(szKeyUserID,CountArray(szKeyUserID),TEXT("%s\\%ld"),REG_USER_INFO,pGlobalUserData->dwUserID);

			//�ϴ��û�
			CWHRegKey RegUserInfo;
			RegUserInfo.OpenRegKey(REG_USER_INFO,true);

			//�û�����
			CWHRegKey RegUserItem;
			RegUserItem.OpenRegKey(szKeyUserID,true);

			//��������
			TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");
			CWHEncrypt::XorEncrypt(szDesPassword,szPassBuffer,CountArray(szPassBuffer));

			//�ʺ���Ϣ
			RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
			RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);
		}

		return;
	}

	TCHAR szItemText[128] = {0};

	if(m_cbSelectedIndex == 0)
	{
		//�û��ǳ�
		GetControlItemText(szEditNickNameControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pGlobalUserData->szNickName,szItemText,CountArray(pGlobalUserData->szNickName));
		ZeroMemory(szItemText,sizeof(szItemText));

		//�Ա�ѡ��
		CRadioButtonUI * pManButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioMankindControlName));
		CRadioButtonUI * pFemaleButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioFemaleControlName));
		if (pManButtonUI && pManButtonUI->GetCheck() ) pGlobalUserData->cbGender=GENDER_MANKIND;
		if (pFemaleButtonUI && pFemaleButtonUI->GetCheck()) pGlobalUserData->cbGender=GENDER_FEMALE;
		
		//�û�ǩ��
		GetControlItemText(szEditSignControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pGlobalUserData->szUnderWrite,szItemText,CountArray(pGlobalUserData->szUnderWrite));
		ZeroMemory(szItemText,sizeof(szItemText));

	}
	if(m_cbSelectedIndex == 1)
	{
		//�Ƽ���
		GetControlItemText(szEditSpreaderControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0) 
		{
			lstrcpyn(pGlobalUserData->szSpreader,szItemText,CountArray(pGlobalUserData->szSpreader));
			ZeroMemory(szItemText,sizeof(szItemText));
		}

		//�ֻ�����
		GetControlItemText(szEditMobilePhoneControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pIndividualUserData->szMobilePhone,szItemText,CountArray(pIndividualUserData->szMobilePhone));
		ZeroMemory(szItemText,sizeof(szItemText));

		//Q Q ����
		GetControlItemText(szEditQQControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pIndividualUserData->szQQ,szItemText,CountArray(pIndividualUserData->szQQ));
		ZeroMemory(szItemText,sizeof(szItemText));

		//�����ʼ�
		GetControlItemText(szEditEMailControlName,szItemText,CountArray(szItemText));		
		lstrcpyn(pIndividualUserData->szEMail,szItemText,CountArray(pIndividualUserData->szEMail));
		ZeroMemory(szItemText,sizeof(szItemText));

		//��ϸ��ַ
		GetControlItemText(szszEditAddressControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pIndividualUserData->szDwellingPlace,szItemText,CountArray(pIndividualUserData->szDwellingPlace));
		ZeroMemory(szItemText,sizeof(szItemText));
	}
	if(m_cbSelectedIndex == 3)
	{
		//��ʵ����
		GetControlItemText(szEditRealNameControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0) 
		{
			lstrcpyn(pIndividualUserData->szCompellation,szItemText,CountArray(pIndividualUserData->szCompellation));
			ZeroMemory(szItemText,sizeof(szItemText));
		}

		//���֤��
		GetControlItemText(szEditIDNumberControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0) 
		{
			lstrcpyn(pIndividualUserData->szPassPortID,szItemText,CountArray(pIndividualUserData->szPassPortID));
			ZeroMemory(szItemText,sizeof(szItemText));
		}
	}

	//�����¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

	return;
}
//�����ַ�
VOID CDlgIndividual::SetControlItemText(LPCTSTR pszControlName,LPCTSTR pszText)
{
	//����ʺ�
	CControlUI * pControlUI = GetControlByName(pszControlName);
	if(pControlUI) pControlUI->SetText(pszText);
}

//��ȡ�ַ�
VOID CDlgIndividual::GetControlItemText(LPCTSTR pszControlName, TCHAR szString[], WORD wMaxCount)
{
	//����ʺ�
	CControlUI * pControlUI = GetControlByName(pszControlName);
	if(pControlUI==NULL) return;

	//��ȡ�ַ�
	CString strString(pControlUI->GetText());	

	////�����ַ�
	//strString.TrimLeft();
	//strString.TrimRight();

	//���ý��
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//���ý���

VOID CDlgIndividual::SetControlFocus(LPCTSTR pszControlName)
{
	//����ʺ�
	CControlUI * pControlUI = GetControlByName(pszControlName);
	if(pControlUI==NULL) return;
	pControlUI->SetFocus();
	return;
}

//����ǿ��
VOID CDlgIndividual::SetPassLevel(LPCTSTR szPowerScale, LPCTSTR szPassControl)
{
	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//��ȡ����
	CControlUI * pEditLogonPass=GetControlByName(szPassControl);
	if(pEditLogonPass!=NULL) lstrcpyn(szPassword, pEditLogonPass->GetText(), CountArray(szPassword));

	//�ȼ��ж�
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//���´���
	CPowerScaleUI * pPowerScaleUI = (CPowerScaleUI *)GetControlByName(szPowerScale);
	pPowerScaleUI->SetLevelState(cbPasswordLevel);

	return;
}

//����ҳ��
void CDlgIndividual::SetTabSelectedIndex(BYTE cbIndex/* = 0*/)
{
	m_cbSelectedIndex = cbIndex;
	return;
}

void CDlgIndividual::UpdateUserFace()
{
	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	CImageUI* pUserAvatar = static_cast<CImageUI*>(GetControlByName(szUserAvatarControlName));
	if(pUserAvatar != NULL)
	{
		//����ͷ��
		if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
		{
			pUserAvatar->SetCustomImage(m_CustomFaceInfo96.dwCustomFace, BIGFACE_CX*BIGFACE_CY);
		}
		else
		{
			TCHAR szBuffer[64]={0};
			WORD wFaceID = pGlobalUserData->wFaceID;
			wsprintf(szBuffer, TEXT("file=\'AVATAR_96_96\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
			pUserAvatar->SetImage(szBuffer);
		}
	}
}

bool CDlgIndividual::EfficacyStandard()
{
	//�û���Ϣ
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	TCHAR szItemText[128] = {0};
	TCHAR szDescribe[128]=TEXT("");
	//�ǳ��ж�
	GetControlItemText(szEditNickNameControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyNickName(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditNickNameControlName);
		return false;
	}

	//ǩ��
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditSignControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyIllegal(szItemText)==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("ǩ�������Ƿ��ַ��������������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditSignControlName);
		return false;
	}


	//�����ж�
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditStandardLogonPWControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyPassword(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditStandardLogonPWControlName);

		return false;
	}

	//��������
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szItemText,szEncryptPassword);
	//�����ж�
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��¼����������������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditStandardLogonPWControlName);

		return false;
	}

	return true;

}

bool CDlgIndividual::EfficacyExtendInfo()
{
	//�û���Ϣ
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	TCHAR szItemText[128] = {0};
	TCHAR szDescribe[128]=TEXT("");

	//�ֻ�����
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditMobilePhoneControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyMobilePhone(szItemText,szDescribe,CountArray(szDescribe))==false)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

			//���ý���
			SetControlFocus(szEditRealNameControlName);
			return false;
		}
	}


	//QQ����
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditQQControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyQQ(szItemText,szDescribe,CountArray(szDescribe))==false)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

			//���ý���
			SetControlFocus(szEditQQControlName);
			return false;
		}
	}

	//��������
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditEMailControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyEmail(szItemText,szDescribe,CountArray(szDescribe))==false)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

			//���ý���
			SetControlFocus(szEditEMailControlName);
			return false;
		}
	}

	//��ϵ��ַ
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szszEditAddressControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyIllegal(szItemText)==false)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��ϵ�����Ƿ��ַ��������������룡"),MB_ICONERROR,0);

			//���ý���
			SetControlFocus(szszEditAddressControlName);
			return false;
		}
	}

	//�����ж�
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditRealInfoPWControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyPassword(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditRealInfoPWControlName);

		return false;
	}

	//��������
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szItemText,szEncryptPassword);
	//�����ж�
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��¼����������������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditRealInfoPWControlName);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyReal()
{
	//�û���Ϣ
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//��������
	TCHAR szItemText[128] = {0};
	TCHAR szDescribe[128]=TEXT("");

	//��ʵ����
	GetControlItemText(szEditRealNameControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyCompellation(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditRealNameControlName);
		return false;
	}

	//���֤��
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditIDNumberControlName,szItemText,CountArray(szItemText));
	//���֤��
	if (pUserItemElement->EfficacyPassPortID(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditIDNumberControlName);
		return false;
	}

	//�����ж�
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditRealAuthPWControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyPassword(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditRealAuthPWControlName);

		return false;
	}

	//��������
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szItemText,szEncryptPassword);
	//�����ж�
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��¼����������������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditRealInfoPWControlName);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyModifyLogonPassward()
{
	//�û���Ϣ
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//��������
	TCHAR szDesPassword[3][LEN_PASSWORD];
	TCHAR szDescribe[128]=TEXT("");

	//��ȡ����
	GetControlItemText(szEditLogonPWOrgControlName,szDesPassword[0],CountArray(szDesPassword[0]));
	GetControlItemText(szEditLogonPWNewControlName,szDesPassword[1],CountArray(szDesPassword[0]));
	GetControlItemText(szEditLogonPWComfirmControlName,szDesPassword[2],CountArray(szDesPassword[0]));

	////��������֤1
	//if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	//{
	//	//��ʾ��Ϣ
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

	//	//���ý���
	//	SetControlFocus(szEditLogonPWOrgControlName);

	//	return false;
	//}

	//��������֤2
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szEncryptPassword);
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("������������������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditLogonPWOrgControlName);

		return false;
	}

	//��������֤
	if (pUserItemElement->EfficacyPassword(szDesPassword[1],szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditLogonPWNewControlName);

		return false;
	}

	//ȷ��������֤
	if (lstrcmp(szDesPassword[1],szDesPassword[2])!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("ȷ�������������벻һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditLogonPWComfirmControlName);

		return false;
	}

	//һ���ж�
	if (lstrcmp(szDesPassword[1],szDesPassword[0])==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�������������һ���������޸ģ�"),MB_ICONINFORMATION,0);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyModifyInsurePassward()
{
	//�û���Ϣ
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//��������
	TCHAR szDesPassword[3][LEN_PASSWORD];
	TCHAR szDescribe[128]=TEXT("");

	//��ȡ����
	GetControlItemText(szEditInsurePWOrgControlName,szDesPassword[0],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePWNewControlName,szDesPassword[1],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePWComfirmControlName,szDesPassword[2],CountArray(szDesPassword[0]));

	////��������֤2
	//TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	//CWHEncrypt::MD5Encrypt(szDesPassword[0],szEncryptPassword);
	//if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	//{
	//	//��ʾ��Ϣ
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(TEXT("������������������룡"),MB_ICONERROR,0);

	//	//���ý���
	//	SetControlFocus(szEditInsurePWOrgControlName);

	//	return false;
	//}

	//��������֤
	if (pUserItemElement->EfficacyPassword(szDesPassword[1],szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditInsurePWNewControlName);

		return false;
	}

	//ȷ��������֤
	if (lstrcmp(szDesPassword[1],szDesPassword[2])!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("ȷ�������������벻һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditInsurePWComfirmControlName);

		return false;
	}

	//һ���ж�
	if (lstrcmp(szDesPassword[1],szDesPassword[0])==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�������������һ���������޸ģ�"),MB_ICONINFORMATION,0);

		return false;
	}

	//��֤��ͬ
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szDesPassword[1],szEncryptPassword);
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�����벻�����¼������ͬ��"),MB_ICONINFORMATION,0);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyOpenInsurePassward()
{
	//�û���Ϣ
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//��������
	TCHAR szDesPassword[3][LEN_PASSWORD];
	TCHAR szDescribe[128]=TEXT("");

	//��ȡ����
	GetControlItemText(szEditInsureLogonControlName,szDesPassword[0],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePassControlName,szDesPassword[1],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePass1ControlName,szDesPassword[2],CountArray(szDesPassword[0]));



	//��������֤
	if (pUserItemElement->EfficacyPassword(szDesPassword[1],szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditInsurePassControlName);

		return false;
	}

	//ȷ��������֤
	if (lstrcmp(szDesPassword[1],szDesPassword[2])!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("ȷ�������������벻һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditInsurePass1ControlName);

		return false;
	}

	//��¼������֤
	if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditInsureLogonControlName);

		return false;
	}

	//��¼������֤
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szEncryptPassword);
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��¼����������������룡"),MB_ICONERROR,0);

		//���ý���
		SetControlFocus(szEditInsureLogonControlName);

		return false;
	}

	//��֤��ͬ
	if (lstrcmp(szDesPassword[1],szDesPassword[0])==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��ͨ�������벻�����¼������ͬ��"),MB_ICONINFORMATION,0);

		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////