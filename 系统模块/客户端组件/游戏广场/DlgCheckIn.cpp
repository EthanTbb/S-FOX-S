#include "StdAfx.h"
#include "Resource.h"
#include "Dlgcheckin.h"
#include "DlgMemberGift.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"
/////////////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName=TEXT("ButtonClose");
const TCHAR* const szButtonCheckInControlName=TEXT("ButtonCheckIn");
const TCHAR* const szNumberSignDataName=TEXT("NumberSignData");
const TCHAR* const szSignLayoutDay1Name=TEXT("SignLayoutDay1");
const TCHAR* const szSignLayoutDay2Name=TEXT("SignLayoutDay2");
const TCHAR* const szSignLayoutDay3Name=TEXT("SignLayoutDay3");
const TCHAR* const szSignLayoutDay4Name=TEXT("SignLayoutDay4");
const TCHAR* const szSignLayoutDay5Name=TEXT("SignLayoutDay5");
const TCHAR* const szSignLayoutDay6Name=TEXT("SignLayoutDay6");
const TCHAR* const szSignLayoutDay7Name=TEXT("SignLayoutDay7");


const TCHAR* const szButtonDayGiftControlName=TEXT("ButtonDayGift");
const TCHAR* const szButtonDayPresentControlName=TEXT("ButtonDayPresent");

const TCHAR* const szButtonVipPresentControlName=TEXT("ButtonVipPresent");

const TCHAR* const szNumberSignMoney1Name=TEXT("NumberSignMoney1");
const TCHAR* const szNumberSignMoney2Name=TEXT("NumberSignMoney2");
const TCHAR* const szNumberSignMoney3Name=TEXT("NumberSignMoney3");
const TCHAR* const szNumberSignMoney4Name=TEXT("NumberSignMoney4");
const TCHAR* const szNumberSignMoney5Name=TEXT("NumberSignMoney5");
const TCHAR* const szNumberSignMoney6Name=TEXT("NumberSignMoney6");
const TCHAR* const szNumberSignMoney7Name=TEXT("NumberSignMoney7");

const TCHAR* const szButtonDaySelect1Name=TEXT("ButtonDaySelect1");
const TCHAR* const szButtonDaySelect2Name=TEXT("ButtonDaySelect2");
const TCHAR* const szButtonDaySelect3Name=TEXT("ButtonDaySelect3");
const TCHAR* const szButtonDaySelect4Name=TEXT("ButtonDaySelect4");
const TCHAR* const szButtonDaySelect5Name=TEXT("ButtonDaySelect5");
const TCHAR* const szButtonDaySelect6Name=TEXT("ButtonDaySelect6");
const TCHAR* const szButtonDaySelect7Name=TEXT("ButtonDaySelect7");

const TCHAR* const szLabelDayUnSignIn1Name=TEXT("LabelDayUnSignIn1");
const TCHAR* const szLabelDayUnSignIn2Name=TEXT("LabelDayUnSignIn2");
const TCHAR* const szLabelDayUnSignIn3Name=TEXT("LabelDayUnSignIn3");
const TCHAR* const szLabelDayUnSignIn4Name=TEXT("LabelDayUnSignIn4");
const TCHAR* const szLabelDayUnSignIn5Name=TEXT("LabelDayUnSignIn5");
const TCHAR* const szLabelDayUnSignIn6Name=TEXT("LabelDayUnSignIn6");
const TCHAR* const szLabelDayUnSignIn7Name=TEXT("LabelDayUnSignIn7");

const TCHAR* const szLabelDaySignIn1Name=TEXT("LabelDaySignIn1");
const TCHAR* const szLabelDaySignIn2Name=TEXT("LabelDaySignIn2");
const TCHAR* const szLabelDaySignIn3Name=TEXT("LabelDaySignIn3");
const TCHAR* const szLabelDaySignIn4Name=TEXT("LabelDaySignIn4");
const TCHAR* const szLabelDaySignIn5Name=TEXT("LabelDaySignIn5");
const TCHAR* const szLabelDaySignIn6Name=TEXT("LabelDaySignIn6");
const TCHAR* const szLabelDaySignIn7Name=TEXT("LabelDaySignIn7");

const TCHAR* const szLabelCheckInSeal1Name=TEXT("LabelCheckInSeal1");
const TCHAR* const szLabelCheckInSeal2Name=TEXT("LabelCheckInSeal2");
const TCHAR* const szLabelCheckInSeal3Name=TEXT("LabelCheckInSeal3");
const TCHAR* const szLabelCheckInSeal4Name=TEXT("LabelCheckInSeal4");
const TCHAR* const szLabelCheckInSeal5Name=TEXT("LabelCheckInSeal5");
const TCHAR* const szLabelCheckInSeal6Name=TEXT("LabelCheckInSeal6");
const TCHAR* const szLabelCheckInSeal7Name=TEXT("LabelCheckInSeal7");

const TCHAR* const szLabeCardShadow1Name=TEXT("LabeCardShadow1");
const TCHAR* const szLabeCardShadow2Name=TEXT("LabeCardShadow2");
const TCHAR* const szLabeCardShadow3Name=TEXT("LabeCardShadow3");
const TCHAR* const szLabeCardShadow4Name=TEXT("LabeCardShadow4");
const TCHAR* const szLabeCardShadow5Name=TEXT("LabeCardShadow5");
const TCHAR* const szLabeCardShadow6Name=TEXT("LabeCardShadow6");
const TCHAR* const szLabeCardShadow7Name=TEXT("LabeCardShadow7");
/////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCheckIn, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgCheckIn::CDlgCheckIn() : CFGuiDialog(IDD_DLG_POPUP)
{
	//ǩ������
	m_bTodayChecked=false;
	m_wDateIndex=0;
	ZeroMemory(m_lRewardGold,sizeof(m_lRewardGold));
	ZeroMemory(m_GiftSub,sizeof(m_GiftSub));
	m_wGiftSubCount = 0;
	
	//�����ʶ
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo = false;

	m_bButtonEnableMemberDayPresent = false;
	m_bButtonEnableMemberDayGift = false;
	//��������
	m_MissionManager.InsertMissionItem(this);
}

//��������
CDlgCheckIn::~CDlgCheckIn()
{
}

LPCTSTR CDlgCheckIn::GetSkinFile()
{
	return TEXT("DlgCheckIn.xml");
}

//��ʼ�ؼ�
void CDlgCheckIn::InitControlUI()
{
	//���ûس�
	CButtonUI * pControlUI = static_cast<CButtonUI *>(GetControlByName(szButtonCheckInControlName));
	if(pControlUI!=NULL) pControlUI->SetWantReturn();

	//UpdateCheckIn();
}

//��Ϣ����
void CDlgCheckIn::Notify(TNotifyUI &  msg)
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
		else if(lstrcmp(pControlUI->GetName(), szButtonCheckInControlName)==0) 
		{
			return OnButtonCheckInClicked(); 
		}		
		else if(lstrcmp(pControlUI->GetName(), szButtonVipPresentControlName)==0) 
		{
			return OnButtonGiftClicked();
			//return PerformMemberDayGift(); 
		}		
		else if(lstrcmp(pControlUI->GetName(), szButtonDayPresentControlName)==0) 
		{

			return PerformMemberDayPresent(); 
		}	
	}
}

//��������
BOOL CDlgCheckIn::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("ÿ��ǩ��"));

	//���д���
	CenterWindow(this);

	UpdateMember();

	//ִ������
	PerformQueryCheckInfo();

	
	return TRUE;
}

//���ǩ��
VOID CDlgCheckIn::OnButtonCheckInClicked()
{
	return PerformCheckInDone();
}

//ִ�в�ѯ
VOID CDlgCheckIn::PerformQueryCheckInfo()
{
	//���ñ���
	m_bCheckInDone=false;
	m_bQueryCheckInfo=true;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//ִ��ǩ��
VOID CDlgCheckIn::PerformCheckInDone()
{
	//״̬�ж�
	if(m_bTodayChecked==true) 
	{
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("�������Ѿ�ǩ���ˣ�������������"));
		return;
	}

	//���ñ���
	m_bCheckInDone=true;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//��ʼ����
	OnMissionStart();
}

VOID CDlgCheckIn::PerformMemberDayGift()
{
	//���ñ���
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = true;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//��ʼ����
	OnMissionStart();
}

VOID CDlgCheckIn::PerformMemberDayPresent()
{
	//���ñ���
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = true;
	m_bMemberQueryInfo =false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//��ʼ����
	OnMissionStart();
}


VOID CDlgCheckIn::PerformMemberQueryInfo()
{
	//���ñ���
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =true;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//��ʼ����
	OnMissionStart();
}

VOID CDlgCheckIn::UpdateMember()
{
	//CControlUI * pContorl1 = (CControlUI*)GetControlByName(szButtonDayPresentControlName);
	//pContorl1->SetEnabled(m_bButtonEnableMemberDayPresent);

	//CControlUI * pContorl2 = (CControlUI*)GetControlByName(szButtonDayGiftControlName);
	//pContorl2->SetEnabled(m_bButtonEnableMemberDayGift);
}

VOID CDlgCheckIn::UpdateCheckIn()
{
	//ǩ������
	CNumberUI * pNumberUI = (CNumberUI*)GetControlByName(szNumberSignDataName);
	if(pNumberUI) 
	{
		float fDataIndex = (float)m_wDateIndex;
		pNumberUI->SetNumber(fDataIndex);
	}

	//���ý��
	for (int i=0;i<7;i++)
	{
		CStdString strControlName;
		strControlName.SmallFormat(TEXT("NumberSignMoney%d"),i+1);
		CNumberUI * pNumberUI = (CNumberUI*)GetControlByName(strControlName);
		if (pNumberUI) 
		{
			float fRewardGold = (float)m_lRewardGold[i];
			pNumberUI->SetNumber(fRewardGold);
		}
	}

	//���ñ���
	int nDateIndexed = m_wDateIndex+1;
	for (int i=0;i<7;i++)
	{
		int CurrIndex = i +1;
		bool bCurrState = CurrIndex<nDateIndexed;
		CStdString strControlName;
		CControlUI *pControlUI=NULL;
		//ǩ������
		strControlName.SmallFormat(TEXT("LabelDaySignIn%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName);
		if (pControlUI) 
		{
			pControlUI->SetVisible(bCurrState);
		}

		//δǩ����
		strControlName.SmallFormat(TEXT("LabelDayUnSignIn%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName); 
		if (pControlUI) 
		{
			pControlUI->SetVisible(!bCurrState);
		}

		//ǩ������
		strControlName.SmallFormat(TEXT("LabelCheckInSeal%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName); 
		if (pControlUI) 
		{
			pControlUI->SetVisible(bCurrState);
		}

		//δǩ��Ӱ
		strControlName.SmallFormat(TEXT("LabeCardShadow%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName); 
		if (pControlUI) 
		{
			pControlUI->SetVisible(!bCurrState);
		}
	}


	CButtonUI * pButtonUI = (CButtonUI*)GetControlByName(szButtonCheckInControlName);
	if(pButtonUI != NULL) pButtonUI->SetEnabled(!m_bTodayChecked);

	pButtonUI = (CButtonUI*)GetControlByName(szButtonVipPresentControlName);
	if(pButtonUI != NULL) pButtonUI->SetEnabled(true);

}

VOID CDlgCheckIn::SetSignLayout(LPCTSTR szLayoutName, const int nDay, const bool bSign, const int nSignGold)
{
	CContainerUI * pContainerUI = (CContainerUI*)GetControlByName(szLayoutName);
	if(pContainerUI != NULL)
	{
		COptionUI* pOptionCheckUI = static_cast<COptionUI*>(pContainerUI->GetItemAt(0));
		if(pOptionCheckUI != NULL){
			pOptionCheckUI->Selected(bSign);
			pOptionCheckUI->SetVisible(true);
		}

		CNumberUI* pNumberGlodUI = static_cast<CNumberUI*>(pContainerUI->GetItemAt(1));
		if(pNumberGlodUI != NULL){
			pNumberGlodUI->SetNumber(nSignGold);
			pNumberGlodUI->SetVisible(!bSign);
		}

		CNumberUI* pNumberCardDataUI = static_cast<CNumberUI*>(pContainerUI->GetItemAt(2));
		if(pNumberCardDataUI != NULL){
			pNumberCardDataUI->SetNumber(nDay);
			pNumberCardDataUI->SetVisible(!bSign);
		}

		CNumberUI* pNumberGlodUI2 = static_cast<CNumberUI*>(pContainerUI->GetItemAt(3));
		if(pNumberGlodUI2 != NULL){
			pNumberGlodUI2->SetNumber(nSignGold);
			pNumberGlodUI2->SetVisible(bSign);
		}


		CControlUI* pControlUI = static_cast<CControlUI*>(pContainerUI->GetItemAt(4));
		if(pControlUI != NULL){
			pControlUI->SetVisible(bSign);
		}
	}
}

//��ʼ����
VOID CDlgCheckIn::OnMissionStart()
{

}

//��ֹ����
VOID CDlgCheckIn::OnMissionConclude()
{

}

//�����¼�
bool CDlgCheckIn::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//��ʾ��ʾ
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("�޷����ӵ���¼��������ǩ����Ϣ����ʧ�ܣ����Ժ����ԣ�"),MB_ICONERROR);
	}
	else
	{
		//��ѯǩ��
		if(m_bQueryCheckInfo==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_CheckInQueryInfo CheckInQueryInfo;
			CheckInQueryInfo.dwUserID = pGlobalUserData->dwUserID;
			lstrcpyn(CheckInQueryInfo.szPassword,pGlobalUserData->szPassword,CountArray(CheckInQueryInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_QUERY,&CheckInQueryInfo,sizeof(CheckInQueryInfo));

			return true;
		}

		//ִ��ǩ��
		if(m_bCheckInDone==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_CheckInDone CheckInDone;
			CheckInDone.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(CheckInDone.szMachineID);
			lstrcpyn(CheckInDone.szPassword,pGlobalUserData->szPassword,CountArray(CheckInDone.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_DONE,&CheckInDone,sizeof(CheckInDone));

			return true;
		}
		
		//��Ա���
		if(m_bMemberDayGift == true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_MemberDayGift memberInfo;
			memberInfo.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(memberInfo.szMachineID);
			lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MEMBER_DAY_GIFT,&memberInfo,sizeof(memberInfo));

		}

		//��Ա����
		if(m_bMemberDayPresent == true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_MemberDayPresent memberInfo;
			memberInfo.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(memberInfo.szMachineID);
			lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MEMBER_DAY_PRESENT,&memberInfo,sizeof(memberInfo));

		}

		//��Ա��ѯ
		if(m_bMemberQueryInfo == true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_MemberQueryInfo memberInfo;
			memberInfo.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(memberInfo.szMachineID);
			lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MEMBER_QUERY_INFO,&memberInfo,sizeof(memberInfo));

		}
	}

	return true;
}

//�ر��¼�
bool CDlgCheckIn::OnEventMissionShut(BYTE cbShutReason)
{
	//������ֹ
	OnMissionConclude();

	return true;
}

//��ȡ�¼�
bool CDlgCheckIn::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�������
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

    //ǩ����Ϣ
	if(Command.wSubCmdID==SUB_GP_CHECKIN_INFO)
	{
		//����У��
		ASSERT(sizeof(CMD_GP_CheckInInfo)==wDataSize);
		if(sizeof(CMD_GP_CheckInInfo)!=wDataSize) return false;

		//��ȡ����
		CMD_GP_CheckInInfo * pCheckInfo = (CMD_GP_CheckInInfo *)pData;

		//���ñ���
		m_wDateIndex=pCheckInfo->wSeriesDate;
		m_bTodayChecked=pCheckInfo->bTodayChecked;
		CopyMemory(m_lRewardGold,pCheckInfo->lRewardGold,sizeof(m_lRewardGold));

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();

		//���½���
		UpdateCheckIn();

		//��ѯ��Ա
		PerformMemberQueryInfo();

		return true;
	}

	//ǩ�����
	if(Command.wSubCmdID==SUB_GP_CHECKIN_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_CheckInResult));
		if(wDataSize>sizeof(CMD_GP_CheckInResult)) return false;

		//��ȡ����
		CMD_GP_CheckInResult * pCheckInResult = (CMD_GP_CheckInResult *)pData;

		if(pCheckInResult->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//���ñ���			
			m_bTodayChecked=true;			
			pGlobalUserData->lUserScore = pCheckInResult->lScore;
			
			m_wDateIndex ++;
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);


			UpdateCheckIn();
		}

		//��Ϣ��ʾ
		if(pCheckInResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(TEXT("��ʾ"),pCheckInResult->szNotifyContent);
		}

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();


		return true;
	}
	//��Ա��ѯ
	if(Command.wSubCmdID==SUB_GP_MEMBER_QUERY_INFO_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_MemberQueryInfoResult));
		if(wDataSize>sizeof(CMD_GP_MemberQueryInfoResult)) return false;

		//��ȡ����
		CMD_GP_MemberQueryInfoResult * pMemberInfo = (CMD_GP_MemberQueryInfoResult *)pData;


		m_bButtonEnableMemberDayPresent = pMemberInfo->bPresent;
		m_bButtonEnableMemberDayGift	= pMemberInfo->bGift;
		m_wGiftSubCount = pMemberInfo->GiftSubCount;
		CopyMemory(m_GiftSub,pMemberInfo->GiftSub,sizeof(tagGiftPropertyInfo)*m_wGiftSubCount);

		UpdateMember();

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);


	}

	if(Command.wSubCmdID==SUB_GP_MEMBER_DAY_PRESENT_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_MemberDayPresentResult));
		if(wDataSize>sizeof(CMD_GP_MemberDayPresentResult)) return false;

		//��ȡ����
		CMD_GP_MemberDayPresentResult * pMemberInfo = (CMD_GP_MemberDayPresentResult *)pData;

		if(pMemberInfo->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();		
			pGlobalUserData->lUserScore = pMemberInfo->lGameScore;

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

			m_bButtonEnableMemberDayPresent = false;
			UpdateMember();
		}

		////��Ϣ��ʾ
		//if(pMemberInfo->szNotifyContent[0]!=0)
		//{
		//	CDlgInformation Information;
		//	Information.ShowMessageBox(pMemberInfo->szNotifyContent);
		//}
		CDlgInformation Information;
		Information.ShowMessageBox(_T("��ϲ������ȡ�ɹ�!"));
		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();
	}

	if(Command.wSubCmdID==SUB_GP_MEMBER_DAY_GIFT_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_MemberDayGiftResult));
		if(wDataSize>sizeof(CMD_GP_MemberDayGiftResult)) return false;

		//��ȡ����
		CMD_GP_MemberDayGiftResult * pMemberInfo = (CMD_GP_MemberDayGiftResult *)pData;

		if(pMemberInfo->bSuccessed==true)
		{
			m_bButtonEnableMemberDayGift = false;
			UpdateMember();
		}

		////��Ϣ��ʾ
		//if(pMemberInfo->szNotifyContent[0]!=0)
		//{
		//	CDlgInformation Information;
		//	Information.ShowMessageBox(pMemberInfo->szNotifyContent);
		//}

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();
		if ( m_bButtonEnableMemberDayPresent)
		{
			PerformMemberDayPresent();
		}
		else if(pMemberInfo->bSuccessed==true)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(_T("��ϲ������ȡ�ɹ�!"));
		}
	}
	return true;
}

VOID CDlgCheckIn::OnButtonGiftClicked()
{
	CDlgMemberGift dlg;
	dlg.SetEnableReceive(m_bButtonEnableMemberDayGift ||m_bButtonEnableMemberDayPresent);
	if(IDOK == dlg.DoModal())
	{
		if(m_bButtonEnableMemberDayGift )
		{
			PerformMemberDayGift();
		}
		else if (m_bButtonEnableMemberDayPresent)
		{
			PerformMemberDayPresent();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////////////////

CDlgCheckInServer::CDlgCheckInServer()
{
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
}

CDlgCheckInServer::~CDlgCheckInServer()
{

}

VOID CDlgCheckInServer::PerformQueryCheckInfo()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_CheckInQueryInfo CheckInQueryInfo;
	CheckInQueryInfo.dwUserID = pGlobalUserData->dwUserID;
	lstrcpyn(CheckInQueryInfo.szPassword,pGlobalUserData->szPassword,CountArray(CheckInQueryInfo.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_CHECKIN,SUB_GR_CHECKIN_QUERY,&CheckInQueryInfo,sizeof(CheckInQueryInfo));
}

VOID CDlgCheckInServer::PerformCheckInDone()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_CheckInDone CheckInDone;
	CheckInDone.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(CheckInDone.szMachineID);
	lstrcpyn(CheckInDone.szPassword,pGlobalUserData->szPassword,CountArray(CheckInDone.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_CHECKIN,SUB_GR_CHECKIN_DONE,&CheckInDone,sizeof(CheckInDone));

}

VOID CDlgCheckInServer::PerformMemberDayGift()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_MemberDayGift memberInfo;
	memberInfo.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(memberInfo.szMachineID);
	lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_MEMBER,SUB_GR_MEMBER_DAY_GIFT,&memberInfo,sizeof(memberInfo));

}

VOID CDlgCheckInServer::PerformMemberDayPresent()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_MemberDayPresent memberInfo;
	memberInfo.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(memberInfo.szMachineID);
	lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_MEMBER,SUB_GR_MEMBER_DAY_PRESENT,&memberInfo,sizeof(memberInfo));
}

VOID CDlgCheckInServer::PerformMemberQueryInfo()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_MemberQueryInfo memberInfo;
	memberInfo.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(memberInfo.szMachineID);
	lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_MEMBER,SUB_GR_MEMBER_QUERY_INFO,&memberInfo,sizeof(memberInfo));
}

VOID CDlgCheckInServer::Initialization( ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem )
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

bool CDlgCheckInServer::OnServerCheckInMessage( WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//��Ϣ�ж�
	if (wMainCmdID==MDM_GR_CHECKIN)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_CHECKIN_INFO:		
			{
				//����У��
				ASSERT(sizeof(CMD_GR_CheckInInfo)==wDataSize);
				if(sizeof(CMD_GR_CheckInInfo)!=wDataSize) return false;

				//��ȡ����
				CMD_GR_CheckInInfo * pCheckInfo = (CMD_GR_CheckInInfo *)pData;

				//���ñ���
				m_wDateIndex=pCheckInfo->wSeriesDate;
				m_bTodayChecked=pCheckInfo->bTodayChecked;
				CopyMemory(m_lRewardGold,pCheckInfo->lRewardGold,sizeof(m_lRewardGold));

				//������ֹ
				OnMissionConclude();

				//���½���
				UpdateCheckIn();

				PerformMemberQueryInfo();

				return true;
			}
		case SUB_GR_CHECKIN_RESULT:		
			{
				//����У��
				ASSERT(wDataSize<=sizeof(CMD_GR_CheckInResult));
				if(wDataSize>sizeof(CMD_GR_CheckInResult)) return false;

				//��ȡ����
				CMD_GR_CheckInResult * pCheckInResult = (CMD_GR_CheckInResult *)pData;

				if(pCheckInResult->bSuccessed==true)
				{
					//��ȡ����
					CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

					//���ñ���			
					m_bTodayChecked=true;			
					pGlobalUserData->lUserScore = pCheckInResult->lScore;

					m_wDateIndex ++;
					//�����¼�
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

					UpdateCheckIn();

				}

				//��Ϣ��ʾ
				if(pCheckInResult->szNotifyContent[0]!=0)
				{
					CDlgInformation Information;
					Information.ShowMessageBox(pCheckInResult->szNotifyContent);
				}

				//������ֹ
				OnMissionConclude();
				return true;

			}
		}
	}
	if (wMainCmdID==MDM_GR_MEMBER)
	{
		switch (wSubCmdID)
		{
		case SUB_GP_MEMBER_QUERY_INFO_RESULT:
			{
				//����У��
				ASSERT(wDataSize<=sizeof(CMD_GR_MemberQueryInfoResult));
				if(wDataSize>sizeof(CMD_GR_MemberQueryInfoResult)) return false;

				//��ȡ����
				CMD_GR_MemberQueryInfoResult * pMemberInfo = (CMD_GR_MemberQueryInfoResult *)pData;


				m_bButtonEnableMemberDayPresent = pMemberInfo->bPresent;
				m_bButtonEnableMemberDayGift	= pMemberInfo->bGift;
				m_wGiftSubCount = pMemberInfo->GiftSubCount;
				CopyMemory(m_GiftSub,pMemberInfo->GiftSub,sizeof(tagGiftPropertyInfo)*m_wGiftSubCount);

				UpdateMember();
				return true;
			}
		case SUB_GP_MEMBER_DAY_PRESENT_RESULT:
			{
				//����У��
				ASSERT(wDataSize<=sizeof(CMD_GR_MemberDayPresentResult));
				if(wDataSize>sizeof(CMD_GR_MemberDayPresentResult)) return false;

				//��ȡ����
				CMD_GR_MemberDayPresentResult * pMemberInfo = (CMD_GR_MemberDayPresentResult *)pData;

				if(pMemberInfo->bSuccessed==true)
				{
					//��ȡ����
					CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();		
					pGlobalUserData->lUserScore = pMemberInfo->lGameScore;

					//�����¼�
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

					m_bButtonEnableMemberDayPresent = false;
					UpdateMember();

					CDlgInformation Information;
					Information.ShowMessageBox(_T("��ϲ������ȡ�ɹ�!"));
				}
				else
				{
					//��Ϣ��ʾ
					if(pMemberInfo->szNotifyContent[0]!=0)
					{
						CDlgInformation Information;
						Information.ShowMessageBox(pMemberInfo->szNotifyContent);
					}
				}





				return true;
			}
		case SUB_GP_MEMBER_DAY_GIFT_RESULT:
			{
				//����У��
				ASSERT(wDataSize<=sizeof(CMD_GR_MemberDayGiftResult));
				if(wDataSize>sizeof(CMD_GR_MemberDayGiftResult)) return false;

				//��ȡ����
				CMD_GR_MemberDayGiftResult * pMemberInfo = (CMD_GR_MemberDayGiftResult *)pData;

				if(pMemberInfo->bSuccessed==true)
				{
					m_bButtonEnableMemberDayGift = false;
					UpdateMember();				
				}

				if (m_bButtonEnableMemberDayPresent)
				{
					PerformMemberDayPresent();
				}
				else if(pMemberInfo->bSuccessed==true)
				{
					CDlgInformation Information;
					Information.ShowMessageBox(_T("��ϲ������ȡ�ɹ�!"));
				}

				////��Ϣ��ʾ
				//if(pMemberInfo->szNotifyContent[0]!=0)
				//{
				//	CDlgInformation Information;
				//	Information.ShowMessageBox(pMemberInfo->szNotifyContent);
				//}

				return true;
			}
		}
	}
	return true;
}


