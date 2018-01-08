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
//容器控件
const TCHAR* const szTabLayoutInsureName = TEXT("TabLayoutInsure");
const TCHAR* const szContainerWealthName = TEXT("InsureItemWealth");
const TCHAR* const szContainerAccessName = TEXT("InsureItemAccess");
const TCHAR* const szContainerPresentName = TEXT("InsureItemPresent");
const TCHAR* const szContainerRecordName = TEXT("InsureItemRecord");

//按钮控件
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
//我的财富

//按钮控件
const TCHAR* const szButtonBaseEnsureControlName = TEXT("ButtonBaseEnsure");
const TCHAR* const szButtonRechargCenterControlName = TEXT("ButtonRechargCenter");
const TCHAR* const szButtonMyBagControlName = TEXT("ButtonMyBag");
const TCHAR* const szButtonExchangeControlName = TEXT("ButtonExchange");
const TCHAR* const szButtonGameShopControlName = TEXT("ButtonGameShop");
const TCHAR* const szButtonRechargeRecrodControlName = TEXT("ButtonRechargeRecrod");
const TCHAR* const szButtonSetControlName = TEXT("ButtonSet");

//编辑控件
const TCHAR* const szEditGameScoreControlName = TEXT("EditGameScore");
const TCHAR* const szEditInsureGameScoreControlName = TEXT("EditInsureGameScore");
const TCHAR* const szEditGameBeanControlName = TEXT("EditGameBean");
const TCHAR* const szEditGameIngotControlName = TEXT("EditGameIngot");
const TCHAR* const szEditGlamourValueControlName = TEXT("EditGlamourValue");

//标签控件
const TCHAR* const szContainerSetTipControlName = TEXT("ContainerSetTip");
const TCHAR* const szlableUnSetTipControlName = TEXT("lableUnSetTip");
const TCHAR* const szLabelInOutControlName = TEXT("LabelInOut");

//////////////////////////////////////////////////////////////////////////////////
//游戏币存取

//数字控件
const TCHAR* const szNumberUserScoreControlName = TEXT("NumberUserScore");
const TCHAR* const szNumberAcessUserInsrueControlName = TEXT("NumberAcessUserInsrue");
const TCHAR* const szNumberGameScoreControlName = TEXT("NumberGameScore");
const TCHAR* const szNumberInsureGameScoreControlName = TEXT("NumberInsureGameScore");
const TCHAR* const szNumberGameBeanControlName = TEXT("NumberGameBean");
const TCHAR* const szNumberGameIngotControlName = TEXT("NumberGameIngot");
const TCHAR* const szNumberGlamourValueControlName = TEXT("NumberGlamourValue");

//按钮控件
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

//编辑控件
const TCHAR* const szEditInOutControlName = TEXT("EditInOut");
const TCHAR* const szEditPassWordControlName = TEXT("EditPassWord");
//////////////////////////////////////////////////////////////////////////////////
//游戏币赠送

//标签控件
const TCHAR* const szLabelPresentIDControlName = TEXT("LabelPresentID");
const TCHAR* const szLabelPresentChineseNumeralControlName = TEXT("LabelPresentChineseNumeral");
//数字控件
const TCHAR* const szumberPresentUserInsrueControlName = TEXT("NumberPresentUserInsrue");

//单选控件
const TCHAR* const szRadioButtonCtrlEnterControlName = TEXT("RadioButtonCtrlEnter");
const TCHAR* const szRadioButtonPresentIDControlName = TEXT("RadioButtonPresentID");
const TCHAR* const szRadioButtonPresentNickControlName = TEXT("RadioButtonPresentNick");

//编辑控件
const TCHAR* const szEditPresentIDControlName = TEXT("EditPresentID");
const TCHAR* const szEditPresentInsurePassWordControlName = TEXT("EditPresentInsurePassWord");
const TCHAR* const szEditPresentAmountControlName = TEXT("EditPresentAmount");
const TCHAR* const szEditPresentBakControlName = TEXT("EditPresentBak");

//VIP控件
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

//构造函数
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
	//设置任务
	m_MissionManager.InsertMissionItem(this);
	ZeroMemory(&m_UserInsureInfo,sizeof(m_UserInsureInfo));
}

//析构函数
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

//创建函数
BOOL CDlgInsure::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("我的银行"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//移动窗口
	MoveWindow(rc);

	//查询银行信息
	/*if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled==TRUE)*/ //QueryUserInsureInfo();
	PerformQueryRebate();

	return TRUE;
}

//确定函数
VOID CDlgInsure::OnOK()
{

	//return __super::OnOK();
}

//取消消息
VOID CDlgInsure::OnCancel()
{

	return __super::OnCancel();
}

//初始控件
VOID CDlgInsure::InitControlUI()
{
	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/UserService1.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRecordControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

	InitUI();
}
//消息提醒
void CDlgInsure::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
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
		//存取游戏币
		else if(pControlUI->GetName()== szButtonGetScoreControlName)
		{
			//获取标签
			CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabLayoutInsureName));
			COptionUI *pOptionUI = static_cast<COptionUI *>(GetControlByName(TEXT("OptionInsureAccess")));
			if(pTabLayoutUI == NULL || pOptionUI == NULL) return;

			pTabLayoutUI->SelectItem(0);
			pOptionUI->Selected(true);
		}
	}
	else if(lstrcmp(msg.sType, TEXT("selectchanged")) == 0)
	{
		//获取标签
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

			//提示消息
			CDlgInformation Information(this);
			if(IDOK ==Information.ShowMessageBox(TEXT("初次使用，请先开通银行！"),MB_OKCANCEL|MB_ICONINFORMATION))
			{
				//发送事件
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
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//事件处理
		OnMissionConclude();

		//错误提示
		if (m_bEnableMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，开通银行失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bSaveMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币存入失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bTakeMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币取出失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bQueryMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，银行资料获取失败！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bTransferMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币赠送失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bQueryRebateMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，查询返利失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}
		return true;
	}

	//开通任务
	if (m_bEnableMission==true)
	{
		//变量定义
		CMD_GP_UserEnableInsure UserEnableInsure;
		ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserEnableInsure.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(UserEnableInsure.szLogonPass,m_szLogonPass,CountArray(UserEnableInsure.szLogonPass));
		lstrcpyn(UserEnableInsure.szInsurePass,m_szInsurePass,CountArray(UserEnableInsure.szInsurePass));		
		CWHService::GetMachineIDEx(UserEnableInsure.szMachineID);

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_ENABLE_INSURE,&UserEnableInsure,sizeof(UserEnableInsure));

		return true;
	}

	//存入任务
	if (m_bSaveMission==true)
	{
		//变量定义
		CMD_GP_UserSaveScore UserSaveScore;
		ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserSaveScore.lSaveScore=m_lScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserSaveScore.szMachineID);

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		return true;
	}

	//取出任务
	if (m_bTakeMission==true)
	{
		//变量定义
		CMD_GP_UserTakeScore UserTakeScore;
		ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserTakeScore.lTakeScore=m_lScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserTakeScore.szMachineID);
		lstrcpyn(UserTakeScore.szPassword,m_szInsurePass,CountArray(UserTakeScore.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		return true;
	}

	//转帐任务
	if (m_bTransferMission==true)
	{
		//变量定义
		CMD_GP_QueryUserInfoRequest QueryUserInfoRequest;
		ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

		//设置变量
		QueryUserInfoRequest.cbByNickName=m_cbByNickName;
		lstrcpyn(QueryUserInfoRequest.szAccounts,m_szAccounts,CountArray(QueryUserInfoRequest.szAccounts));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));
	}

	//查询任务
	if (m_bQueryMission==true)
	{
		//变量定义
		CMD_GP_QueryInsureInfo QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//设置变量
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;
		lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserInfo->GetGlobalUserData()->szPassword,CountArray(QueryInsureInfo.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	//查询返利
	if (m_bQueryRebateMission==true)
	{
		//变量定义
		CMD_GP_QueryTransferRebate QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//设置变量
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;
		lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserInfo->GetGlobalUserData()->szPassword,CountArray(QueryInsureInfo.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_TRANSFER_REBATE,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	return true;
}


bool CDlgInsure::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		////重试任务
		//if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		//{
		//	return true;
		//}

		//错误提示
		if (m_bEnableMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，开通银行失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//存入任务
		if (m_bSaveMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，游戏币存入失败！"),MB_ICONERROR,30);
		}

		//取出任务
		if (m_bTakeMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，游戏币取出失败！"),MB_ICONERROR,30);
		}

		//查询任务
		if (m_bQueryMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，银行资料获取失败！"),MB_ICONERROR,30);
		}

		//错误提示
		if (m_bQueryRebateMission==true)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，查询返利失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}
	}

	//事件处理
	OnMissionConclude();
	return true;
}


bool CDlgInsure::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_QUERY_TRANSFER_REBATE_RESULT:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_QueryTransferRebateResult));
				if (wDataSize<sizeof(CMD_GP_QueryTransferRebateResult)) return false;

				//变量定义
				CMD_GP_QueryTransferRebateResult * pUserInsureInfo=(CMD_GP_QueryTransferRebateResult *)pData;

				cbRebateEnabled = pUserInsureInfo->cbRebateEnabled;
				lRebateIngot =pUserInsureInfo->lIngot;
				lRebateLoveLiness = pUserInsureInfo->lLoveLiness;
				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				QueryUserInsureInfo();

				return true;
			}
		case SUB_GP_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//变量定义
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//设置变量
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);
				
				UpdateUI();

				ShowPresent();

				

				return true;
			}
		case SUB_GP_USER_INSURE_ENABLE_RESULT:	//开通结果
			{
				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GP_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GP_UserInsureEnableResult)) return false;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//变量定义
				CMD_GP_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GP_UserInsureEnableResult *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//显示消息
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				//查询银行
				if(pGlobalUserData->cbInsureEnabled==TRUE) QueryUserInsureInfo();

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);
				
				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));
				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);	

				//设置焦点
				if(m_pItemAccess) m_pItemAccess->SetEditFocus();

				////设置焦点
				//if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				//{
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				//}			

				//更新界面
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
					//显示消息
					if (pUserInsureSuccess->szDescribeString[0]!=0)
					{
						CDlgInformation Information(this);
						Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
					}	
				}			

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//设置焦点
				//if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass!= NULL)
				//{
				//	m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
				//	m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				//}

				////设置焦点
				//if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				//{
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
				//	m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				//}

				m_bIsTransfer = false;

				//显示消息
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_QUERY_USER_INFO_RESULT:	//用户信息
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GP_UserTransferUserInfo)) return false;

				//变量定义
				CMD_GP_UserTransferUserInfo * pTransferUserInfo=(CMD_GP_UserTransferUserInfo *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);

				//变量定义
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder? pMemberParameter->dwMemberInsure:m_UserInsureInfo.wRevenueTransfer;

				//构造消息
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				//CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("赠送游戏币收取%d‰手续费,您确定要给[%s], ID:%d 赠送%s 游戏币吗?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				lstrcpyn(m_szAccounts,pTransferUserInfo->szAccounts,CountArray(m_szAccounts));
				//提示消息
				CDlgInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//变量定义
					CMD_GP_UserTransferScore UserTransferScore;
					ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

					//变量定义
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//设置变量
					m_dwTargetUserID = pTransferUserInfo->dwTargetGameID;
					UserTransferScore.lTransferScore=m_lScore;
					UserTransferScore.dwUserID=pGlobalUserData->dwUserID;
					CWHService::GetMachineIDEx(UserTransferScore.szMachineID);					
					lstrcpyn(UserTransferScore.szPassword,m_szInsurePass,CountArray(UserTransferScore.szPassword));
					lstrcpyn(UserTransferScore.szTransRemark,m_szTransRemark,CountArray(UserTransferScore.szTransRemark));
					lstrcpyn(UserTransferScore.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferScore.szAccounts));

					m_bIsTransfer = true;
					//发送数据
					m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_SCORE,&UserTransferScore,sizeof(UserTransferScore));
				}

				return true;
			}
		}
	}

	return true;
}

//开始任务
VOID CDlgInsure::OnMissionStart()
{
	return;
}

//终止任务
VOID CDlgInsure::OnMissionConclude()
{
	return;
}

VOID CDlgInsure::PerformQueryInfo()
{
	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}
}

VOID CDlgInsure::PerformQueryRebate()
{
	//设置变量
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bTransferMission=false;
	m_bEnableMission=false;
	m_bQueryRebateMission=true;

	//事件处理
	OnMissionStart();

	//执行查询
	PerformQueryInfo();

}


VOID CDlgInsure::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//设置变量
	m_bEnableMission=true;
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bTransferMission=false;
	m_bQueryRebateMission=false;
	//设置变量
	lstrcpyn(m_szLogonPass,pszLogonPass,CountArray(m_szLogonPass));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));	

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

VOID CDlgInsure::QueryUserInsureInfo()
{
	//设置变量
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=true;
	m_bTransferMission=false;
	m_bEnableMission=false;
    m_bQueryRebateMission=false;
	//事件处理
	OnMissionStart();

	//执行查询
	PerformQueryInfo();

	return;
}

//存入事件
VOID CDlgInsure::PerformSaveScore(SCORE lSaveScore)
{
	//设置变量	
	m_bSaveMission=true;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;
	m_bQueryRebateMission=false;

	//设置变量
	m_lScore=lSaveScore;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//取出事件
VOID CDlgInsure::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//设置变量	
	m_bTakeMission=true;
	m_bSaveMission=false;	
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;
	m_bQueryRebateMission=false;

	//设置变量
	m_lScore=lTakeScore;
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

VOID CDlgInsure::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//设置变量
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=true;
	m_bQueryRebateMission=false;

	//设置变量
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//发起连接
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

	//隐藏
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
	//隐藏
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
	//获取信息
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//变量定义
	TCHAR szBuffer[64]=TEXT("");

	TCHAR szServiceFee[64]=TEXT("");

	//携带游戏币
	SetNumber(m_pContainerUI, szNumberGameScoreControlName, (double)pGlobalUserData->lUserScore);

	//银行游戏币
	SetNumber(m_pContainerUI, szNumberInsureGameScoreControlName, (double)pGlobalUserData->lUserInsure);

	//游戏豆
	SetNumber(m_pContainerUI, szNumberGameBeanControlName, pGlobalUserData->dUserBeans);

	//元宝
	SetNumber(m_pContainerUI, szNumberGameIngotControlName, (double)pGlobalUserData->lUserIngot);

	//魅力值
	SetNumber(m_pContainerUI, szNumberGlamourValueControlName, (double)pGlobalUserData->lLoveLiness);

	//是否显示设置银行密码
	bool bVisible = pGlobalUserData->cbInsureEnabled==FALSE;
	CContainerUI* pSetTip = static_cast<CContainerUI*>(m_pContainerUI->FindControl(szContainerSetTipControlName));
	if(pSetTip) pSetTip->SetVisible(bVisible);

	
}

void CInsureItemWealth::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	//点击事件
	if (msg.sType == TEXT("click"))
	{
		//开通银行
		if(pControlUI->GetName()== szButtonSetControlName)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_PERSONER,2L);

			m_pDlgInsureMain->OnCancel();

		}
		//领取低保
		else if(pControlUI->GetName()== szButtonBaseEnsureControlName
				|| pControlUI->GetName()== szButtonGetBaseEnsureControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BASE_ENSURE,0L);

			m_pDlgInsureMain->OnCancel();

		}
		//充值中心
		else if(pControlUI->GetName()== szButtonRechargCenterControlName
				|| pControlUI->GetName()== szButtonRechargeControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//我的背包
		else if(pControlUI->GetName()== szButtonMyBagControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//元宝兑换
		else if(pControlUI->GetName()== szButtonExchangeControlName
				|| pControlUI->GetName()== szButtonExchargeControlName1
				|| pControlUI->GetName()== szButtonExchargeControlName2)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_SHOP,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//游戏商城
		else if(pControlUI->GetName()== szButtonGameShopControlName)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_SHOP,0L);

			m_pDlgInsureMain->OnCancel();
		}
		//充值记录
		else if(pControlUI->GetName()== szButtonRechargeRecrodControlName)
		{
			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szLinkInfo[256]=TEXT("");
			_sntprintf(szLinkInfo,CountArray(szLinkInfo),TEXT("%s/%sUserService/UserService3.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

			//打开页面
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
	//获取信息
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//携带游戏币
	CNumberUI* pNumberUserScore = static_cast<CNumberUI*>(m_pContainerUI->FindControl(szNumberUserScoreControlName));
	if(pNumberUserScore != NULL) 
	{
		float fUserScore = (float)pGlobalUserData->lUserScore;
		pNumberUserScore->SetNumber(fUserScore);
	}
	
	//游戏存款
	CNumberUI* pUserInsrueScore = static_cast<CNumberUI*>(m_pContainerUI->FindControl(szNumberAcessUserInsrueControlName));
	if(pUserInsrueScore != NULL) 
	{
		float fUserInsure = (float)pGlobalUserData->lUserInsure;
		pUserInsrueScore->SetNumber(fUserInsure);
	}

	//提示信息
	TCHAR szNormalTip[128]=TEXT("");
	_sntprintf(szNormalTip,CountArray(szNormalTip),TEXT("提示：存入游戏币免手续费，取出将扣除%d‰的手续费。存款无需输入银行密码。"),m_pDlgInsureMain->m_UserInsureInfo.wRevenueTake);
	CControlUI * pNormalTip = m_pContainerUI->FindControl(szTextTakeTaxControlName);
	if (pNormalTip)
	{
		pNormalTip->SetText(szNormalTip);
	}

}

void CInsureItemAccess::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	//点击事件
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
	//获取UI数据
	LoadDataFromUI();

	//变量定义
	SCORE lSaveScore = m_InOutScore;
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	//数据判断
	if (lSaveScore<=0L)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("存入的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInOut->SetFocus();

		return;
	}

	//数目判断
	if (lSaveScore > pGlobalUserData->lUserScore)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("您所携带游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInOut->SetFocus();

		return;
	}

	//执行存款
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformSaveScore(lSaveScore);
}

VOID CInsureItemAccess::OnBnClickedTakeScore()
{
	//获取UI数据
	LoadDataFromUI();

	//获取变量
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	SCORE lTakeScore= m_InOutScore;

	//获取密码
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditPassWord->GetText(),CountArray(szPassword));

	//加密密码
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//数据判断
	if (lTakeScore<=0L)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("取出的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInOut->SetFocus();

		return;
	}

	//数目判断
	if (lTakeScore > pGlobalUserData->lUserInsure)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("您银行游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInOut->SetFocus();

		return;
	}

	//密码判断
	if (szPassword[0]==0)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("银行密码不能为空，请重新输入银行密码！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditPassWord->SetFocus();

		return;
	}

	//事件通知
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
//金币赠送（银行子页面）
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
	//获取信息
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//游戏存款
	CNumberUI* pUserInsrueScore = static_cast<CNumberUI*>(m_pContainerUI->FindControl(szumberPresentUserInsrueControlName));
	if(pUserInsrueScore != NULL) 
	{
		float fUserInsure = (float)pGlobalUserData->lUserInsure;
		pUserInsrueScore->SetNumber(fUserInsure);
	}

	//返利结果
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


	//普通提示
	TCHAR szNormalTip[128]=TEXT("");
	_sntprintf(szNormalTip,CountArray(szNormalTip),TEXT("提示：普通玩家游戏币赠送需扣除%d‰的手续费。"),m_pDlgInsureMain->m_UserInsureInfo.wRevenueTransfer);
	CControlUI * pNormalTip = m_pContainerUI->FindControl(szTextTransferTaxControlName);
	if (pNormalTip)
	{
		pNormalTip->SetText(szNormalTip);
	}

	//VIP提示
	TCHAR szTaskRewardTotal[64]=TEXT("");
	if(pGlobalUserData->cbMemberOrder > 0)
	{
		//会员任务
		const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
		if(pMemberParameter != NULL)
		{
			_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("扣除%d‰手续费。"),pMemberParameter->dwMemberInsure);
		}

		CControlUI * pContainerVip = m_pContainerUI->FindControl(szContainerVipAwardControlName);
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
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	//点击事件
	if (msg.sType == TEXT("click"))
	{
		//确认赠送
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
	//获取信息
	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();

	//条件判断
	if (pGlobalUserData->lUserInsure < m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite)
	{
		//提示消息
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("您银行游戏币的数目余额不足最低转帐条件 %I64d，无法进行转帐操作！"),m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite);
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		return;
	}

	//转帐金额
	CNumberConvert nc;
	SCORE lTransferScore= nc.GetUserControlScore(m_pEditAmount->GetText());

	//转帐备注
	TCHAR szTransRemark[LEN_TRANS_REMARK]=TEXT("");
	lstrcpyn(szTransRemark,m_pEditRemarks->GetText(),CountArray(szTransRemark));

	//获取密码
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditInsurePass->GetText(),CountArray(szPassword));

	//加密密码
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//获取昵称
	CString strAccounts = m_pEditAccount->GetText();

	//格式昵称
	strAccounts.TrimLeft();
	strAccounts.TrimRight();

	//昵称判断
	if (m_pRadioByNickName->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("请输入要赠送的玩家昵称！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAccount->SetFocus();

		return;
	}

	//ID号判断
	if(m_pRadioByUserID->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("请输入要赠送的玩家ID！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAccount->SetFocus();

		return;
	}

	//数据判断
	if (lTransferScore<=0L)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("赠送的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//数目判断
	if (lTransferScore > m_pDlgInsureMain->GetUserInsureInfo()->lUserInsure)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("您银行游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//数目判断
	if (lTransferScore<m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite)
	{
		//提示消息
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("从银行转帐的游戏币数目不能少于 %I64d，无法进行转帐操作！"),m_pDlgInsureMain->GetUserInsureInfo()->lTransferPrerequisite);
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//密码判断
	if (szPassword[0]==0)
	{
		//提示消息
		CDlgInformation Information(m_pDlgInsureMain);
		Information.ShowMessageBox(TEXT("银行密码不能为空，请重新输入银行密码！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInsurePass->SetFocus();

		return;
	}

	//事件通知
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTransferScore(strAccounts,lTransferScore,szInsurePass,szTransRemark,m_pRadioByUserID->GetCheck()?FALSE:TRUE);

	//设置焦点
	m_pEditInsurePass->SetFocus();
	m_pEditInsurePass->SetText(TEXT(""));

	return;
}


//依据昵称
VOID CInsureItemPresent::OnBnCheckedTransferByNickName()
{
	//设置控件
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetNumberOnly(false);
	m_pLabelTarget->SetText(TEXT("接收昵称："));
}

//依据ID
VOID CInsureItemPresent::OnBnCheckedTransferByUserID()
{
	//设置控件
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetNumberOnly(true);
	m_pLabelTarget->SetText(TEXT("接收I D："));
}


//构造函数
CDlgInsureServerNew::CDlgInsureServerNew()
{
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;

	return;
}

//析构函数
CDlgInsureServerNew::~CDlgInsureServerNew()
{
}

//查询事件
VOID CDlgInsureServerNew::PerformQueryInfo()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//设置变量
	QueryInsureInfo.cbActivityGame=FALSE;
	lstrcpyn(QueryInsureInfo.szInsurePass,pGlobalUserData->szPassword,CountArray(QueryInsureInfo.szInsurePass));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return;
}

//开通事件
VOID CDlgInsureServerNew::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GR_C_EnableInsureRequest EnableInsureResult;
	ZeroMemory(&EnableInsureResult,sizeof(EnableInsureResult));

	//设置变量
	EnableInsureResult.cbActivityGame=FALSE;
	EnableInsureResult.dwUserID=pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(EnableInsureResult.szMachineID);
	lstrcpyn(EnableInsureResult.szLogonPass,pszLogonPass,CountArray(EnableInsureResult.szLogonPass));
	lstrcpyn(EnableInsureResult.szInsurePass,pszInsurePass,CountArray(EnableInsureResult.szInsurePass));		

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_ENABLE_INSURE_REQUEST,&EnableInsureResult,sizeof(EnableInsureResult));

	return;
}

//存入事件
VOID CDlgInsureServerNew::PerformSaveScore(SCORE lSaveScore)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	ZeroMemory(&SaveScoreRequest,sizeof(SaveScoreRequest));

	//设置变量
	SaveScoreRequest.lSaveScore=lSaveScore;

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//取出事件
VOID CDlgInsureServerNew::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//设置变量
	TakeScoreRequest.lTakeScore=lTakeScore;
	lstrcpyn(TakeScoreRequest.szInsurePass,pszInsurePass,CountArray(TakeScoreRequest.szInsurePass));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	return;
}

//转帐事件
VOID CDlgInsureServerNew::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//设置变量
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//变量定义
	CMD_GR_C_QueryUserInfoRequest QueryUserInfoRequest;	
	ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	//设置变量
	QueryUserInfoRequest.cbByNickName=m_cbByNickName;
	lstrcpyn(QueryUserInfoRequest.szAccounts,pszAccounts,CountArray(QueryUserInfoRequest.szAccounts));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	return;
}

//配置银行
VOID CDlgInsureServerNew::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}

//银行消息
bool CDlgInsureServerNew::OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//消息判断
	if (wMainCmdID==MDM_GR_INSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

				//变量定义
				CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;

				//设置变量
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				UpdateUI();

				ShowPresent();

				return true;
			}
		case SUB_GR_USER_INSURE_ENABLE_RESULT:	//开通结果
			{
				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GR_S_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GR_S_UserInsureEnableResult)) return false;

				//变量定义
				CMD_GR_S_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GR_S_UserInsureEnableResult *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//显示消息
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				////调整步骤
				//RectifyInsureStep();

				return true;
			}
		case SUB_GR_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//设置变量
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));
				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);	

				//设置焦点
				if(m_pItemAccess) m_pItemAccess->SetEditFocus();	

				//更新界面
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
					//显示消息
					if (pUserInsureSuccess->szDescribeString[0]!=0)
					{
						CDlgInformation Information(this);
						Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
					}	
				}

			

				return true;
			}
		case SUB_GR_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GR_S_UserInsureFailure * pUserInsureFailure=(CMD_GR_S_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				////设置控件
				//if (m_DlgInsureOperater.m_bInitInfo==true)
				//{
				//	//设置焦点
				//	if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass != NULL)
				//	{
				//		m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
				//		m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				//	}

				//	//设置焦点
				//	if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				//	{
				//		m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
				//		m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				//	}
				//}

				m_bIsTransfer = false;

				//显示消息
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GR_USER_TRANSFER_USER_INFO:  //用户信息
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserTransferUserInfo)) return false;

				//变量定义
				CMD_GR_S_UserTransferUserInfo * pTransferUserInfo=(CMD_GR_S_UserTransferUserInfo *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);

				//变量定义
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder?pMemberParameter->dwMemberInsure:m_UserInsureInfo.wRevenueTransfer;


				//构造消息
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				//CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("赠送游戏币收取%d‰手续费,您确定要给[%s], ID:%d 赠送%s 游戏币吗?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				lstrcpyn(m_szAccounts,pTransferUserInfo->szAccounts,CountArray(m_szAccounts));
				//提示消息
				CDlgInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//变量定义
					CMD_GP_C_TransferScoreRequest TransferScoreRequest;
					ZeroMemory(&TransferScoreRequest,sizeof(TransferScoreRequest));

					//设置变量
					m_dwTargetUserID = pTransferUserInfo->dwTargetGameID;
					TransferScoreRequest.lTransferScore=m_lScore;					
					lstrcpyn(TransferScoreRequest.szInsurePass,m_szInsurePass,CountArray(TransferScoreRequest.szInsurePass));
					lstrcpyn(TransferScoreRequest.szTransRemark,m_szTransRemark,CountArray(TransferScoreRequest.szTransRemark));
					lstrcpyn(TransferScoreRequest.szAccounts,pTransferUserInfo->szAccounts,CountArray(TransferScoreRequest.szAccounts));

					m_bIsTransfer = true;
					//发送数据
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
