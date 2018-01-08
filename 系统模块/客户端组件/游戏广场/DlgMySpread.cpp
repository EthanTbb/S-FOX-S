#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "DlgMySpread.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonCopyControlName = TEXT("ButtonCopy");
const TCHAR* const szTextSpreadControlName = TEXT("TextSpread");
const TCHAR* const szEditInputControlName = TEXT("EditInput");
//////////////////////////////////////////////////////////////////////////////////
//网络事件
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
	//设置标题
	SetWindowText(TEXT("推广窗口"));

	//居中窗口
	CenterWindow(this);

	//加载数据
	LoadDataFromMemory();

	//刷新界面
	UpdateDataToUI();

	//加载奖励
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

//初始控件
void CDlgMySpread::InitControlUI()
{
}

//皮肤名称
LPCTSTR CDlgMySpread::GetSkinFile()
{
	return TEXT("DlgSpread.xml");
}

//消息提醒
void CDlgMySpread::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
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
	//设置剪贴板
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
	//构造地址
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
	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szSpreadInfo,NULL,NULL,SW_NORMAL);
}

//开始任务
VOID CDlgMySpread::OnMissionStart()
{
}

//终止任务
VOID CDlgMySpread::OnMissionConclude()
{

}

//连接事件
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

//关闭事件
bool CDlgMySpread::OnEventMissionShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		OnMissionShutError(cbShutReason);
		return true;
	}
	return true;
}

//读取事件
bool CDlgMySpread::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_SPREAD_INFO:	//加载奖励
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
	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)
	{
		//错误提示
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("服务器连接失败，请检查网络配置以及网络连接状况！"),MB_ICONERROR);
		return false;
	}
	return true;
}

void CDlgMySpread::OnMissionLinkError(INT nErrorCode)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！"),MB_ICONERROR);
}

void CDlgMySpread::OnMissionShutError(BYTE cbShutReason)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("由于当前服务器处理繁忙，操作处理失败，请稍后再重试！"),MB_ICONERROR);

}

void CDlgMySpread::OnMissionLinkLoad()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GP_UserSpreadQuery UserSpreadQuery;
	UserSpreadQuery.dwUserID=pGlobalUserData->dwUserID;

	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SPREAD_QUERY,&UserSpreadQuery,sizeof(UserSpreadQuery));

}

void CDlgMySpread::OnMissionReadLoad(VOID * pData, WORD wDataSize)
{
	//关闭连接
	ConcludeMission();

	//参数校验
	ASSERT(wDataSize==sizeof(CMD_GP_UserSpreadInfo));
	if(wDataSize!=sizeof(CMD_GP_UserSpreadInfo)) return;

	//提取数据
	CMD_GP_UserSpreadInfo * pUserSpreadInfo= (CMD_GP_UserSpreadInfo *)pData;

	//设置变量
	m_UserSpreadInfo.dwSpreadCount=pUserSpreadInfo->dwSpreadCount;
	m_UserSpreadInfo.lSpreadReward=pUserSpreadInfo->lSpreadReward;

	UpdateDataToUI();

}

void CDlgMySpread::LoadDataFromUI()
{

}

void CDlgMySpread::LoadDataFromMemory()
{
	//获取链接
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
	//变量定义
	TCHAR szSpreadText[256]=TEXT("");
	_sntprintf(szSpreadText,CountArray(szSpreadText),TEXT("您已推荐 {a name='PlayerNumLink' linkcolor='#FFFFCC00' linkhovercolor='#FFFFFF66'}%d{/a} 玩家，目前可领取推广奖励 {a name='GoldNum' linkcolor='#FFFFCC00' linkhovercolor='#FFFFFF66'}%d{/a} 游戏币"),m_UserSpreadInfo.dwSpreadCount,m_UserSpreadInfo.lSpreadReward);

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

//加载奖励
void CDlgMySpread::PerformLoadSpreadReward()
{
	AvtiveMission(byMissionLoad);
}
////////////////////////////////////////////////////////////////////////////////////////////
