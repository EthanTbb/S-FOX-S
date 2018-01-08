#include "StdAfx.h"
#include "Resource.h"
#include "Dlgcheckin.h"
#include "DlgMemberGift.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"
/////////////////////////////////////////////////////////////////////////////////////////
//控件定义

//按钮控件
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

	//系统消息
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgCheckIn::CDlgCheckIn() : CFGuiDialog(IDD_DLG_POPUP)
{
	//签到配置
	m_bTodayChecked=false;
	m_wDateIndex=0;
	ZeroMemory(m_lRewardGold,sizeof(m_lRewardGold));
	ZeroMemory(m_GiftSub,sizeof(m_GiftSub));
	m_wGiftSubCount = 0;
	
	//任务标识
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo = false;

	m_bButtonEnableMemberDayPresent = false;
	m_bButtonEnableMemberDayGift = false;
	//设置任务
	m_MissionManager.InsertMissionItem(this);
}

//析构函数
CDlgCheckIn::~CDlgCheckIn()
{
}

LPCTSTR CDlgCheckIn::GetSkinFile()
{
	return TEXT("DlgCheckIn.xml");
}

//初始控件
void CDlgCheckIn::InitControlUI()
{
	//设置回车
	CButtonUI * pControlUI = static_cast<CButtonUI *>(GetControlByName(szButtonCheckInControlName));
	if(pControlUI!=NULL) pControlUI->SetWantReturn();

	//UpdateCheckIn();
}

//消息提醒
void CDlgCheckIn::Notify(TNotifyUI &  msg)
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

//创建函数
BOOL CDlgCheckIn::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("每天签到"));

	//居中窗口
	CenterWindow(this);

	UpdateMember();

	//执行任务
	PerformQueryCheckInfo();

	
	return TRUE;
}

//点击签到
VOID CDlgCheckIn::OnButtonCheckInClicked()
{
	return PerformCheckInDone();
}

//执行查询
VOID CDlgCheckIn::PerformQueryCheckInfo()
{
	//设置变量
	m_bCheckInDone=false;
	m_bQueryCheckInfo=true;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//执行签到
VOID CDlgCheckIn::PerformCheckInDone()
{
	//状态判断
	if(m_bTodayChecked==true) 
	{
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("您今天已经签到了，请明天再来！"));
		return;
	}

	//设置变量
	m_bCheckInDone=true;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//开始任务
	OnMissionStart();
}

VOID CDlgCheckIn::PerformMemberDayGift()
{
	//设置变量
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = true;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//开始任务
	OnMissionStart();
}

VOID CDlgCheckIn::PerformMemberDayPresent()
{
	//设置变量
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = true;
	m_bMemberQueryInfo =false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//开始任务
	OnMissionStart();
}


VOID CDlgCheckIn::PerformMemberQueryInfo()
{
	//设置变量
	m_bCheckInDone=false;
	m_bQueryCheckInfo=false;
	m_bMemberDayGift = false;
	m_bMemberDayPresent = false;
	m_bMemberQueryInfo =true;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  return;

	//开始任务
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
	//签到天数
	CNumberUI * pNumberUI = (CNumberUI*)GetControlByName(szNumberSignDataName);
	if(pNumberUI) 
	{
		float fDataIndex = (float)m_wDateIndex;
		pNumberUI->SetNumber(fDataIndex);
	}

	//设置金币
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

	//设置宝箱
	int nDateIndexed = m_wDateIndex+1;
	for (int i=0;i<7;i++)
	{
		int CurrIndex = i +1;
		bool bCurrState = CurrIndex<nDateIndexed;
		CStdString strControlName;
		CControlUI *pControlUI=NULL;
		//签到宝箱
		strControlName.SmallFormat(TEXT("LabelDaySignIn%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName);
		if (pControlUI) 
		{
			pControlUI->SetVisible(bCurrState);
		}

		//未签宝箱
		strControlName.SmallFormat(TEXT("LabelDayUnSignIn%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName); 
		if (pControlUI) 
		{
			pControlUI->SetVisible(!bCurrState);
		}

		//签到盖章
		strControlName.SmallFormat(TEXT("LabelCheckInSeal%d"),CurrIndex);
		pControlUI = (CControlUI*)GetControlByName(strControlName); 
		if (pControlUI) 
		{
			pControlUI->SetVisible(bCurrState);
		}

		//未签阴影
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

//开始任务
VOID CDlgCheckIn::OnMissionStart()
{

}

//终止任务
VOID CDlgCheckIn::OnMissionConclude()
{

}

//连接事件
bool CDlgCheckIn::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//显示提示
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("无法连接到登录服务器，签到信息加载失败，请稍后再试！"),MB_ICONERROR);
	}
	else
	{
		//查询签到
		if(m_bQueryCheckInfo==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_CheckInQueryInfo CheckInQueryInfo;
			CheckInQueryInfo.dwUserID = pGlobalUserData->dwUserID;
			lstrcpyn(CheckInQueryInfo.szPassword,pGlobalUserData->szPassword,CountArray(CheckInQueryInfo.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_QUERY,&CheckInQueryInfo,sizeof(CheckInQueryInfo));

			return true;
		}

		//执行签到
		if(m_bCheckInDone==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_CheckInDone CheckInDone;
			CheckInDone.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(CheckInDone.szMachineID);
			lstrcpyn(CheckInDone.szPassword,pGlobalUserData->szPassword,CountArray(CheckInDone.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_DONE,&CheckInDone,sizeof(CheckInDone));

			return true;
		}
		
		//会员礼包
		if(m_bMemberDayGift == true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_MemberDayGift memberInfo;
			memberInfo.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(memberInfo.szMachineID);
			lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MEMBER_DAY_GIFT,&memberInfo,sizeof(memberInfo));

		}

		//会员赠送
		if(m_bMemberDayPresent == true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_MemberDayPresent memberInfo;
			memberInfo.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(memberInfo.szMachineID);
			lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MEMBER_DAY_PRESENT,&memberInfo,sizeof(memberInfo));

		}

		//会员查询
		if(m_bMemberQueryInfo == true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_MemberQueryInfo memberInfo;
			memberInfo.dwUserID = pGlobalUserData->dwUserID;
			CWHService::GetMachineIDEx(memberInfo.szMachineID);
			lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MEMBER_QUERY_INFO,&memberInfo,sizeof(memberInfo));

		}
	}

	return true;
}

//关闭事件
bool CDlgCheckIn::OnEventMissionShut(BYTE cbShutReason)
{
	//任务终止
	OnMissionConclude();

	return true;
}

//读取事件
bool CDlgCheckIn::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令过滤
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

    //签到信息
	if(Command.wSubCmdID==SUB_GP_CHECKIN_INFO)
	{
		//参数校验
		ASSERT(sizeof(CMD_GP_CheckInInfo)==wDataSize);
		if(sizeof(CMD_GP_CheckInInfo)!=wDataSize) return false;

		//提取数据
		CMD_GP_CheckInInfo * pCheckInfo = (CMD_GP_CheckInInfo *)pData;

		//设置变量
		m_wDateIndex=pCheckInfo->wSeriesDate;
		m_bTodayChecked=pCheckInfo->bTodayChecked;
		CopyMemory(m_lRewardGold,pCheckInfo->lRewardGold,sizeof(m_lRewardGold));

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();

		//更新界面
		UpdateCheckIn();

		//查询会员
		PerformMemberQueryInfo();

		return true;
	}

	//签到结果
	if(Command.wSubCmdID==SUB_GP_CHECKIN_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_CheckInResult));
		if(wDataSize>sizeof(CMD_GP_CheckInResult)) return false;

		//提取数据
		CMD_GP_CheckInResult * pCheckInResult = (CMD_GP_CheckInResult *)pData;

		if(pCheckInResult->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//设置变量			
			m_bTodayChecked=true;			
			pGlobalUserData->lUserScore = pCheckInResult->lScore;
			
			m_wDateIndex ++;
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);


			UpdateCheckIn();
		}

		//消息提示
		if(pCheckInResult->szNotifyContent[0]!=0)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(TEXT("提示"),pCheckInResult->szNotifyContent);
		}

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();


		return true;
	}
	//会员查询
	if(Command.wSubCmdID==SUB_GP_MEMBER_QUERY_INFO_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_MemberQueryInfoResult));
		if(wDataSize>sizeof(CMD_GP_MemberQueryInfoResult)) return false;

		//提取数据
		CMD_GP_MemberQueryInfoResult * pMemberInfo = (CMD_GP_MemberQueryInfoResult *)pData;


		m_bButtonEnableMemberDayPresent = pMemberInfo->bPresent;
		m_bButtonEnableMemberDayGift	= pMemberInfo->bGift;
		m_wGiftSubCount = pMemberInfo->GiftSubCount;
		CopyMemory(m_GiftSub,pMemberInfo->GiftSub,sizeof(tagGiftPropertyInfo)*m_wGiftSubCount);

		UpdateMember();

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);


	}

	if(Command.wSubCmdID==SUB_GP_MEMBER_DAY_PRESENT_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_MemberDayPresentResult));
		if(wDataSize>sizeof(CMD_GP_MemberDayPresentResult)) return false;

		//提取数据
		CMD_GP_MemberDayPresentResult * pMemberInfo = (CMD_GP_MemberDayPresentResult *)pData;

		if(pMemberInfo->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();		
			pGlobalUserData->lUserScore = pMemberInfo->lGameScore;

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

			m_bButtonEnableMemberDayPresent = false;
			UpdateMember();
		}

		////消息提示
		//if(pMemberInfo->szNotifyContent[0]!=0)
		//{
		//	CDlgInformation Information;
		//	Information.ShowMessageBox(pMemberInfo->szNotifyContent);
		//}
		CDlgInformation Information;
		Information.ShowMessageBox(_T("恭喜您，领取成功!"));
		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();
	}

	if(Command.wSubCmdID==SUB_GP_MEMBER_DAY_GIFT_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_MemberDayGiftResult));
		if(wDataSize>sizeof(CMD_GP_MemberDayGiftResult)) return false;

		//提取数据
		CMD_GP_MemberDayGiftResult * pMemberInfo = (CMD_GP_MemberDayGiftResult *)pData;

		if(pMemberInfo->bSuccessed==true)
		{
			m_bButtonEnableMemberDayGift = false;
			UpdateMember();
		}

		////消息提示
		//if(pMemberInfo->szNotifyContent[0]!=0)
		//{
		//	CDlgInformation Information;
		//	Information.ShowMessageBox(pMemberInfo->szNotifyContent);
		//}

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();
		if ( m_bButtonEnableMemberDayPresent)
		{
			PerformMemberDayPresent();
		}
		else if(pMemberInfo->bSuccessed==true)
		{
			CDlgInformation Information;
			Information.ShowMessageBox(_T("恭喜您，领取成功!"));
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
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_CheckInQueryInfo CheckInQueryInfo;
	CheckInQueryInfo.dwUserID = pGlobalUserData->dwUserID;
	lstrcpyn(CheckInQueryInfo.szPassword,pGlobalUserData->szPassword,CountArray(CheckInQueryInfo.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_CHECKIN,SUB_GR_CHECKIN_QUERY,&CheckInQueryInfo,sizeof(CheckInQueryInfo));
}

VOID CDlgCheckInServer::PerformCheckInDone()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_CheckInDone CheckInDone;
	CheckInDone.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(CheckInDone.szMachineID);
	lstrcpyn(CheckInDone.szPassword,pGlobalUserData->szPassword,CountArray(CheckInDone.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_CHECKIN,SUB_GR_CHECKIN_DONE,&CheckInDone,sizeof(CheckInDone));

}

VOID CDlgCheckInServer::PerformMemberDayGift()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_MemberDayGift memberInfo;
	memberInfo.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(memberInfo.szMachineID);
	lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_MEMBER,SUB_GR_MEMBER_DAY_GIFT,&memberInfo,sizeof(memberInfo));

}

VOID CDlgCheckInServer::PerformMemberDayPresent()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_MemberDayPresent memberInfo;
	memberInfo.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(memberInfo.szMachineID);
	lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_MEMBER,SUB_GR_MEMBER_DAY_PRESENT,&memberInfo,sizeof(memberInfo));
}

VOID CDlgCheckInServer::PerformMemberQueryInfo()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_MemberQueryInfo memberInfo;
	memberInfo.dwUserID = pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(memberInfo.szMachineID);
	lstrcpyn(memberInfo.szPassword,pGlobalUserData->szPassword,CountArray(memberInfo.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_MEMBER,SUB_GR_MEMBER_QUERY_INFO,&memberInfo,sizeof(memberInfo));
}

VOID CDlgCheckInServer::Initialization( ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem )
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

bool CDlgCheckInServer::OnServerCheckInMessage( WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//消息判断
	if (wMainCmdID==MDM_GR_CHECKIN)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_CHECKIN_INFO:		
			{
				//参数校验
				ASSERT(sizeof(CMD_GR_CheckInInfo)==wDataSize);
				if(sizeof(CMD_GR_CheckInInfo)!=wDataSize) return false;

				//提取数据
				CMD_GR_CheckInInfo * pCheckInfo = (CMD_GR_CheckInInfo *)pData;

				//设置变量
				m_wDateIndex=pCheckInfo->wSeriesDate;
				m_bTodayChecked=pCheckInfo->bTodayChecked;
				CopyMemory(m_lRewardGold,pCheckInfo->lRewardGold,sizeof(m_lRewardGold));

				//任务终止
				OnMissionConclude();

				//更新界面
				UpdateCheckIn();

				PerformMemberQueryInfo();

				return true;
			}
		case SUB_GR_CHECKIN_RESULT:		
			{
				//参数校验
				ASSERT(wDataSize<=sizeof(CMD_GR_CheckInResult));
				if(wDataSize>sizeof(CMD_GR_CheckInResult)) return false;

				//提取数据
				CMD_GR_CheckInResult * pCheckInResult = (CMD_GR_CheckInResult *)pData;

				if(pCheckInResult->bSuccessed==true)
				{
					//获取对象
					CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

					//设置变量			
					m_bTodayChecked=true;			
					pGlobalUserData->lUserScore = pCheckInResult->lScore;

					m_wDateIndex ++;
					//发送事件
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

					UpdateCheckIn();

				}

				//消息提示
				if(pCheckInResult->szNotifyContent[0]!=0)
				{
					CDlgInformation Information;
					Information.ShowMessageBox(pCheckInResult->szNotifyContent);
				}

				//任务终止
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
				//参数校验
				ASSERT(wDataSize<=sizeof(CMD_GR_MemberQueryInfoResult));
				if(wDataSize>sizeof(CMD_GR_MemberQueryInfoResult)) return false;

				//提取数据
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
				//参数校验
				ASSERT(wDataSize<=sizeof(CMD_GR_MemberDayPresentResult));
				if(wDataSize>sizeof(CMD_GR_MemberDayPresentResult)) return false;

				//提取数据
				CMD_GR_MemberDayPresentResult * pMemberInfo = (CMD_GR_MemberDayPresentResult *)pData;

				if(pMemberInfo->bSuccessed==true)
				{
					//获取对象
					CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();		
					pGlobalUserData->lUserScore = pMemberInfo->lGameScore;

					//发送事件
					CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
					if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

					m_bButtonEnableMemberDayPresent = false;
					UpdateMember();

					CDlgInformation Information;
					Information.ShowMessageBox(_T("恭喜您，领取成功!"));
				}
				else
				{
					//消息提示
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
				//参数校验
				ASSERT(wDataSize<=sizeof(CMD_GR_MemberDayGiftResult));
				if(wDataSize>sizeof(CMD_GR_MemberDayGiftResult)) return false;

				//提取数据
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
					Information.ShowMessageBox(_T("恭喜您，领取成功!"));
				}

				////消息提示
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


