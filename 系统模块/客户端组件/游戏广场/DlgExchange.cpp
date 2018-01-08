#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "DlgExchange.h"

////////////////////////////////////////////////////////////////////////////////////////////
//控件定义

//按钮控件
const TCHAR* const szButtonConfirmExchangeControlName=TEXT("ButtonConfirmExchange");
const TCHAR* const szButtonConfirmPurchaseControlName=TEXT("ButtonConfirmPurchase");

//编辑控件
const TCHAR* const szEditIngotCountControlName=TEXT("EditIngotCount");
const TCHAR* const szEditPurchaseTimeControlName=TEXT("EditPurchaseTime");

//组合控件
const TCHAR* const szComboMemberTypeControlName=TEXT("ComboMemberType");

//文件控件
const TCHAR* const szTextMoreExchangeControlName=TEXT("TextMoreExchange");
const TCHAR* const szTextExchangeRateControlName=TEXT("TextExchangeRate");

////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgExchange::CDlgExchange(): CSkinDialog(IDD_DLG_POPUP)
{
	//标识变量
	m_bLableRound=false;

	//显示变量
	m_lExchangeScore=0;
	m_lConsumeBeans=0;
	m_lPresentScore=0;

	//参数变量
	m_dwExchangeRate=0L;
	m_wMemberCount=0;
	ZeroMemory(&m_MemberParameter,sizeof(m_MemberParameter));
}

//析构函数
CDlgExchange::~CDlgExchange()
{
}


//初始控件
void CDlgExchange::InitControlUI()
{
	__super::InitControlUI();

	//变量定义
	CEditUI * pEditControl = NULL;

	//设置控件
	pEditControl = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pEditControl)
	{
		pEditControl->SetNumberOnly();
		pEditControl->SetIntegerNumber();
		pEditControl->SetMaxChar(12);
	}

	//设置控件
	CEditUI * pEditPurchaseTime =  static_cast<CEditUI *>(GetControlByName(szEditPurchaseTimeControlName));
	if(pEditPurchaseTime!=NULL)
	{
		pEditPurchaseTime->SetNumberOnly();
		pEditPurchaseTime->SetMaxChar(3);
	}
}

//消息提醒
void CDlgExchange::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(),szButtonConfirmExchangeControlName) == 0)
		{
			return OnClickedConfirmExchange();
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonConfirmPurchaseControlName) == 0)
		{
			return OnClickedConfirmPurchase();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szTextMoreExchangeControlName)==0)
		{
			return OnTextLinkMoreExchange();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{	
		if(lstrcmp(pControlUI->GetName(),szEditIngotCountControlName)==0)
		{
			return OnEditIngotTextChange();
		} 
		else if(lstrcmp(pControlUI->GetName(),szEditPurchaseTimeControlName)==0)
		{
			return OnComboControlItemChange();
		}		
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboMemberTypeControlName) == 0)
		{
			return OnComboControlItemChange();
		}
	}
}


//创建函数
BOOL CDlgExchange::OnInitDialog()
{
	__super::OnInitDialog();

	//居中窗口
	CenterWindow(this);

	//加载参数
	PerformLoadExchangeParam();

	return FALSE;
}

//消息解释
BOOL CDlgExchange::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE) return TRUE;
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//绘画标题
VOID CDlgExchange::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//资源句柄
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//加载标题
	CPngImage ImageTitle;
	ImageTitle.LoadImage(hInstance,TEXT("IMAGE_EXCHANGE_TITLE"));

	//绘制标题
	ImageTitle.DrawImage(pDC,10,10);
}

//绘画消息
VOID CDlgExchange::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//当前元宝
	TCHAR szCurrIngot[16]=TEXT("");
	CRect rcCurrIngot(93,108,158,121);
	SwitchScoreFormat(pGlobalUserData->lUserIngot,3,szCurrIngot,CountArray(szCurrIngot));
	pDC->DrawText(szCurrIngot,lstrlen(szCurrIngot),rcCurrIngot,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//兑换游戏币
	TCHAR szExchangeScore[32]=TEXT("");
	CRect rcExchangeScore(93,172,210,185);
	SwitchScoreFormat(m_lExchangeScore,3,szExchangeScore,CountArray(szExchangeScore));
	pDC->DrawText(szExchangeScore,lstrlen(szExchangeScore),rcExchangeScore,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//消耗游戏豆
	TCHAR szConsumeBeans[32]=TEXT("");
	CRect rcConsumeBeans(324,171,404,185);
	SwitchScoreFormat(m_lConsumeBeans,3,szConsumeBeans,CountArray(szConsumeBeans));
	pDC->DrawText(szConsumeBeans,lstrlen(szConsumeBeans),rcConsumeBeans,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//赠送游戏币
	TCHAR szPresentScore[32]=TEXT("");
	CRect rcPresentScore(324,203,404,217);
	SwitchScoreFormat(m_lPresentScore,3,szPresentScore,CountArray(szPresentScore));
	pDC->DrawText(szPresentScore,lstrlen(szPresentScore),rcPresentScore,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

//开始任务
VOID CDlgExchange::OnMissionStart()
{
	//变量定义
	CControlUI * pControlUI = NULL; 
	
	//确认兑换
	pControlUI=GetControlByName(szButtonConfirmExchangeControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(false); 

	//确认购买
	pControlUI=GetControlByName(szButtonConfirmPurchaseControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(false);
}

//终止任务
VOID CDlgExchange::OnMissionConclude()
{
	//变量定义
	CControlUI * pControlUI = NULL; 
	
	//确认兑换
	pControlUI=GetControlByName(szButtonConfirmExchangeControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(true); 

	//确认购买
	pControlUI=GetControlByName(szButtonConfirmPurchaseControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(true);
}

//确认兑换
VOID CDlgExchange::OnClickedConfirmExchange()
{
	//查找控件
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pControlUI==NULL) return;
	
	//输入限制
	CString strInputIngot(pControlUI->GetText());	
	SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());

	//输入判断
	if(lExchangeIngot==0)
	{
		CInformation Information;
		Information.ShowMessageBox(TEXT("请输入兑换元宝数量！"));

		//设置焦点
		pControlUI->SetFocus();

		return;
	}

	//执行兑换
	return PerformExchangeScore();
}

//确认购买
VOID CDlgExchange::OnClickedConfirmPurchase()
{
	//查找控件
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditPurchaseTimeControlName));
	if(pControlUI==NULL) return;
	
	//输入限制
	SCORE lPurchaseTime = GetUserControlNumber(pControlUI->GetText());

	//输入判断
	if(lPurchaseTime==0)
	{
		CInformation Information;
		Information.ShowMessageBox(TEXT("请输入购买会员的有效时间！"));

		//设置焦点
		pControlUI->SetFocus();

		return;
	}

	//执行兑换
	return PerformPurchaseMember();
}

//更多兑换
VOID CDlgExchange::OnTextLinkMoreExchange()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szNavigation[256]=TEXT("");
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sShop/Index.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
}

//文本变更
VOID CDlgExchange::OnEditIngotTextChange()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//查找控件
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pControlUI==NULL) return;
	
	//输入限制
	SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());
	if(lExchangeIngot>pGlobalUserData->lUserIngot)
	{
		//设置变量
		lExchangeIngot=pGlobalUserData->lUserIngot;		

		//调整输入
		TCHAR szUserIngot[32]=TEXT("");
		_sntprintf(szUserIngot,CountArray(szUserIngot),TEXT("%I64d"),lExchangeIngot);
		pControlUI->SetText(szUserIngot);
	}	

	//兑换金币
	m_lExchangeScore = lExchangeIngot*m_dwExchangeRate;

	//刷新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
}

//子项变更
VOID CDlgExchange::OnComboControlItemChange()
{
	//变量定义
	CComboUI * pComboMemberType=(CComboUI *)GetControlByName(szComboMemberTypeControlName);;
	if(pComboMemberType==NULL) return;

	//获取选择
	INT nCurrentSel=pComboMemberType->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagMemberParameter * pMemberParameter=(tagMemberParameter *)pComboMemberType->GetItemAt(nCurrentSel)->GetUserData();

	//变量定义
	CEditUI * pEditPurchaseTime=(CEditUI *)GetControlByName(szEditPurchaseTimeControlName);;
	if(pEditPurchaseTime==NULL) return;

	//获取选择
	SCORE lPurchaseTime = GetUserControlNumber(pEditPurchaseTime->GetText());

	//设置变量
	m_lConsumeBeans = pMemberParameter->lMemberPrice*lPurchaseTime;
	m_lPresentScore = pMemberParameter->lPresentScore*lPurchaseTime;

	//刷新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
}

//兑换游戏币
VOID CDlgExchange::PerformExchangeScore()
{	
}

//购买会员
VOID CDlgExchange::PerformPurchaseMember()
{	
}

//加载低保
VOID CDlgExchange::PerformLoadExchangeParam()
{	
}

//获取数字
SCORE CDlgExchange::GetUserControlNumber(LPCTSTR pszNumber)
{
	//变量定义
	WORD wIndex=0;
	SCORE lNumber=0L;

	//获取数值
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszNumber,CountArray(szBuffer));

	//构造数值
	while (szBuffer[wIndex]!=0)
	{
		//变量定义
		WORD wCurrentIndex=wIndex++;

		//构造数值
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lNumber=lNumber*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lNumber;
}

//转换字符
VOID CDlgExchange::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//插入逗号
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}
////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgExchangePlaza::CDlgExchangePlaza()
{
	//控制变量
	m_bExchangeScore=false;
	m_bPurchaseMember=false;
	m_bLoadExchangeParam=false;


	//设置任务
	m_MissionManager.InsertMissionItem(this);
}

//析构函数
CDlgExchangePlaza::~CDlgExchangePlaza()
{
}

//连接事件
bool CDlgExchangePlaza::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//显示提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("无法连接到登录服务器，任务信息加载失败，请稍后再试！"),MB_ICONERROR);
	}
	else
	{
		//兑换游戏币
		if(m_bExchangeScore==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//查找控件
			CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
			if(pControlUI==NULL) return true;
			
			//输入限制
			CString strInputIngot(pControlUI->GetText());	
			SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());

			//构造结构
			CMD_GP_ExchangeScoreByIngot ExchangeScore;
			ZeroMemory(&ExchangeScore,sizeof(ExchangeScore));

			//设置结构
			ExchangeScore.dwUserID=pGlobalUserData->dwUserID;
			ExchangeScore.lExchangeIngot=lExchangeIngot;
			CWHService::GetMachineIDEx(ExchangeScore.szMachineID);

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_SCORE_BYINGOT,&ExchangeScore,sizeof(ExchangeScore));

			return true;
		}

		//购买会员
		if(m_bPurchaseMember==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CComboUI * pComboMemberType=(CComboUI *)GetControlByName(szComboMemberTypeControlName);;
			if(pComboMemberType==NULL) return true;

			//获取选择
			INT nCurrentSel=pComboMemberType->GetCurSel();
			if(nCurrentSel==LB_ERR) return true;
			tagMemberParameter * pMemberParameter=(tagMemberParameter *)pComboMemberType->GetItemAt(nCurrentSel)->GetUserData();

			//变量定义
			CEditUI * pEditPurchaseTime=(CEditUI *)GetControlByName(szEditPurchaseTimeControlName);;
			if(pEditPurchaseTime==NULL) return true;

			//获取选择
			WORD dwPurchaseTime = (WORD)GetUserControlNumber(pEditPurchaseTime->GetText());

			//构造结构
			CMD_GP_PurchaseMember PurchaseMember;
			ZeroMemory(&PurchaseMember,sizeof(PurchaseMember));

			//设置变量
			PurchaseMember.dwUserID=pGlobalUserData->dwUserID;
			PurchaseMember.cbMemberOrder=pMemberParameter->cbMemberOrder;
			PurchaseMember.wPurchaseTime=dwPurchaseTime;
			CWHService::GetMachineIDEx(PurchaseMember.szMachineID);

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_PURCHASE_MEMBER,&PurchaseMember,sizeof(PurchaseMember));

			return true;
		}

		//加载参数
		if(m_bLoadExchangeParam==true)
		{
			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_QUERY,NULL,0);

			return true;
		}	
	}

	return true;
}

//关闭事件
bool CDlgExchangePlaza::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//读取事件
bool CDlgExchangePlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令过滤
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//兑换参数
	if(Command.wSubCmdID==SUB_GP_EXCHANGE_PARAMETER)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_ExchangeParameter));
		if(wDataSize>sizeof(CMD_GP_ExchangeParameter)) return false;

		//提取数据
		CMD_GP_ExchangeParameter * pExchangeParameter= (CMD_GP_ExchangeParameter *)pData;

		//设置变量
		m_dwExchangeRate = pExchangeParameter->dwExchangeRate;
		m_wMemberCount = pExchangeParameter->wMemberCount;
		CopyMemory(&m_MemberParameter,pExchangeParameter->MemberParameter,sizeof(tagMemberParameter)*m_wMemberCount);
		
		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();

		//变量定义
		CListLabelElementUI * pListLabelElement=NULL;

		//查找控件	
		CComboUI * pComboMemberType =  static_cast<CComboUI *>(GetControlByName(szComboMemberTypeControlName));
		if(pComboMemberType!=NULL)
		{
			//开通时间
			for(INT nIndex=0;nIndex<m_wMemberCount;nIndex++)
			{
				//创建对象
				pListLabelElement = new CListLabelElementUI;
				if(pListLabelElement==NULL) throw TEXT("内存不足！");

				//设置子项
				pListLabelElement->SetAttribute(TEXT("text"),m_MemberParameter[nIndex].szMemberName);
				pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
				pListLabelElement->SetManager(&m_PaintManager,pComboMemberType,true);
				if(pComboMemberType->Add(pListLabelElement)==true)
				{
					pListLabelElement->SetUserData(&m_MemberParameter[nIndex]);
				}
			}

			//默认选择
			if ((pComboMemberType->GetCount()>0)&&(pComboMemberType->GetCurSel()==LB_ERR))
			{
				pComboMemberType->SelectItem(0);
			}		
		}	

		//查找控件	
		CControlUI * pControlUI=  GetControlByName(szTextExchangeRateControlName);
		if(pControlUI!=NULL)
		{
			//变量定义
			TCHAR szExchangeRate[128]=TEXT("");
			_sntprintf(szExchangeRate,CountArray(szExchangeRate),TEXT("{a}{f 2}<u><c #00FF0000>1元宝=%d游戏币{/c}</u>{/f}{/a}"),m_dwExchangeRate);
			pControlUI->SetText(szExchangeRate);
		}

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//购买结果
	if(Command.wSubCmdID==SUB_GP_PURCHASE_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_PurchaseResult));
		if(wDataSize>sizeof(CMD_GP_PurchaseResult)) return false;

		//提取数据
		CMD_GP_PurchaseResult * pPurchaseResult= (CMD_GP_PurchaseResult *)pData;

		//购买成功
		if(pPurchaseResult->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			pGlobalUserData->lUserScore=pPurchaseResult->lCurrScore;
			pGlobalUserData->dUserBeans=pPurchaseResult->dCurrBeans;
			pGlobalUserData->cbMemberOrder=pPurchaseResult->cbMemberOrder;			

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//消息提示
		if(pPurchaseResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pPurchaseResult->szNotifyContent);
		}

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//兑换结果
	if(Command.wSubCmdID==SUB_GP_EXCHANGE_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_ExchangeResult));
		if(wDataSize>sizeof(CMD_GP_ExchangeResult)) return false;

		//提取数据
		CMD_GP_ExchangeResult * pExchangeResult= (CMD_GP_ExchangeResult *)pData;

		//兑换成功
		if(pExchangeResult->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			pGlobalUserData->lUserScore=pExchangeResult->lCurrScore;
			pGlobalUserData->lUserIngot=pExchangeResult->lCurrIngot;

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//消息提示
		if(pExchangeResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pExchangeResult->szNotifyContent);
		}
		
		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	return true;
}

//兑换游戏币
VOID CDlgExchangePlaza::PerformExchangeScore()
{
	//设置变量
	m_bExchangeScore=true;
	m_bPurchaseMember=false;
	m_bLoadExchangeParam=false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//购买会员
VOID CDlgExchangePlaza::PerformPurchaseMember()
{
	//设置变量
	m_bExchangeScore=false;
	m_bPurchaseMember=true;
	m_bLoadExchangeParam=false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//加载参数
VOID CDlgExchangePlaza::PerformLoadExchangeParam()
{
	//设置变量
	m_bExchangeScore=false;
	m_bPurchaseMember=false;
	m_bLoadExchangeParam=true;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}
////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgExchangeServer::CDlgExchangeServer()
{
	//设置变量
	m_pITCPSocket=NULL;
}

//析构函数
CDlgExchangeServer::~CDlgExchangeServer()
{
}

//兑换游戏币
VOID CDlgExchangeServer::PerformExchangeScore()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//查找控件
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pControlUI==NULL) return;
	
	//输入限制
	CString strInputIngot(pControlUI->GetText());	
	SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());

	//构造结构
	CMD_GR_ExchangeScoreByIngot ExchangeScore;
	ZeroMemory(&ExchangeScore,sizeof(ExchangeScore));

	//设置结构
	ExchangeScore.dwUserID=pGlobalUserData->dwUserID;
	ExchangeScore.lExchangeIngot=lExchangeIngot;
	CWHService::GetMachineIDEx(ExchangeScore.szMachineID);

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_EXCHANGE,SUB_GR_EXCHANGE_SCORE_BYINGOT,&ExchangeScore,sizeof(ExchangeScore));
}

//购买会员
VOID CDlgExchangeServer::PerformPurchaseMember()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CComboUI * pComboMemberType=(CComboUI *)GetControlByName(szComboMemberTypeControlName);;
	if(pComboMemberType==NULL) return;

	//获取选择
	INT nCurrentSel=pComboMemberType->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagMemberParameter * pMemberParameter=(tagMemberParameter *)pComboMemberType->GetItemAt(nCurrentSel)->GetUserData();

	//变量定义
	CEditUI * pEditPurchaseTime=(CEditUI *)GetControlByName(szEditPurchaseTimeControlName);;
	if(pEditPurchaseTime==NULL) return;

	//获取选择
	WORD dwPurchaseTime = (WORD)GetUserControlNumber(pEditPurchaseTime->GetText());

	//构造结构
	CMD_GR_PurchaseMember PurchaseMember;
	ZeroMemory(&PurchaseMember,sizeof(PurchaseMember));

	//设置变量
	PurchaseMember.dwUserID=pGlobalUserData->dwUserID;
	PurchaseMember.cbMemberOrder=pMemberParameter->cbMemberOrder;
	PurchaseMember.wPurchaseTime=dwPurchaseTime;
	CWHService::GetMachineIDEx(PurchaseMember.szMachineID);

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_EXCHANGE,SUB_GR_PURCHASE_MEMBER,&PurchaseMember,sizeof(PurchaseMember));
}

//加载参数
VOID CDlgExchangeServer::PerformLoadExchangeParam()
{
	//发送数据
	m_pITCPSocket->SendData(MDM_GR_EXCHANGE,SUB_GR_EXCHANGE_LOAD_INFO,NULL,0);
}

//配置任务
VOID CDlgExchangeServer::Initialization(ITCPSocket * pITCPSocket)
{
	//发送变量
	m_pITCPSocket=pITCPSocket;
}

//任务消息
bool CDlgExchangeServer::OnExchangeMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//命令过滤
	if(wMainCmdID!=MDM_GR_EXCHANGE) return true;

	//兑换参数
	if(wSubCmdID==SUB_GR_EXCHANGE_PARAM_INFO)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GR_ExchangeParameter));
		if(wDataSize>sizeof(CMD_GR_ExchangeParameter)) return false;

		//提取数据
		CMD_GR_ExchangeParameter * pExchangeParameter= (CMD_GR_ExchangeParameter *)pData;

		//设置变量
		m_dwExchangeRate = pExchangeParameter->dwExchangeRate;
		m_wMemberCount = pExchangeParameter->wMemberCount;
		CopyMemory(&m_MemberParameter,pExchangeParameter->MemberParameter,sizeof(tagMemberParameter)*m_wMemberCount);
		
		//任务终止
		OnMissionConclude();

		//变量定义
		CListLabelElementUI * pListLabelElement=NULL;

		//查找控件	
		CComboUI * pComboMemberType =  static_cast<CComboUI *>(GetControlByName(szComboMemberTypeControlName));
		if(pComboMemberType!=NULL)
		{
			//开通时间
			for(INT nIndex=0;nIndex<m_wMemberCount;nIndex++)
			{
				//创建对象
				try
				{
					pListLabelElement = new CListLabelElementUI;
					if(pListLabelElement==NULL) throw TEXT("内存不足！");

					//设置子项
					pListLabelElement->SetAttribute(TEXT("text"),m_MemberParameter[nIndex].szMemberName);
					pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
					pListLabelElement->SetManager(&m_PaintManager,pComboMemberType,true);
					if(pComboMemberType->Add(pListLabelElement)==true)
					{
						pListLabelElement->SetUserData(&m_MemberParameter[nIndex]);
					}
				}
				catch(...)
				{
					ASSERT(FALSE);
					break;
				}
			}

			//默认选择
			if ((pComboMemberType->GetCount()>0)&&(pComboMemberType->GetCurSel()==LB_ERR))
			{
				pComboMemberType->SelectItem(0);
			}		
		}	

		//查找控件	
		CControlUI * pControlUI=  GetControlByName(szTextExchangeRateControlName);
		if(pControlUI!=NULL)
		{
			//变量定义
			TCHAR szExchangeRate[128]=TEXT("");
			_sntprintf(szExchangeRate,CountArray(szExchangeRate),TEXT("{a}{f 2}<u><c #00FF0000>1元宝=%d游戏币{/c}</u>{/f}{/a}"),m_dwExchangeRate);
			pControlUI->SetText(szExchangeRate);
		}

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//购买结果
	if(wSubCmdID==SUB_GR_PURCHASE_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GR_PurchaseResult));
		if(wDataSize>sizeof(CMD_GR_PurchaseResult)) return false;

		//提取数据
		CMD_GR_PurchaseResult * pPurchaseResult= (CMD_GR_PurchaseResult *)pData;

		//购买成功
		if(pPurchaseResult->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			pGlobalUserData->lUserScore=pPurchaseResult->lCurrScore;
			pGlobalUserData->cbMemberOrder=pPurchaseResult->cbMemberOrder;			

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//消息提示
		if(pPurchaseResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pPurchaseResult->szNotifyContent);
		}

		//任务终止
		OnMissionConclude();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//兑换结果
	if(wSubCmdID==SUB_GR_EXCHANGE_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GR_ExchangeResult));
		if(wDataSize>sizeof(CMD_GR_ExchangeResult)) return false;

		//提取数据
		CMD_GR_ExchangeResult * pExchangeResult= (CMD_GR_ExchangeResult *)pData;

		//兑换成功
		if(pExchangeResult->bSuccessed==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			pGlobalUserData->lUserScore=pExchangeResult->lCurrScore;
			pGlobalUserData->lUserIngot=pExchangeResult->lCurrIngot;

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//消息提示
		if(pExchangeResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pExchangeResult->szNotifyContent);
		}
		
		//任务终止
		OnMissionConclude();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
