#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgLockMachine.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName  = TEXT("ButtonClose");
const TCHAR* const szButtonLockControlName   = TEXT("ButtonLock");
const TCHAR* const szButtonUnlockControlName = TEXT("ButtonUnlock");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szEditPassWordControlName = TEXT("EditPassWord");

//////////////////////////////////////////////////////////////////////////////////
//网络事件
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
	//设置标题
	SetWindowText(TEXT("锁机设置"));

	//居中窗口
	CenterWindow(this);

	//加载数据
	LoadDataFromMemory();

	//刷新界面
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

//关闭事件
bool CDlgLockMachine::OnEventMissionShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		OnMissionShutError(cbShutReason);
		return true;
	}
	return true;
}

//读取事件
bool CDlgLockMachine::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:	//操作失败
			{
				OnMissionReadLockFailure(pData,wDataSize);
				break;
			}
		case SUB_GP_OPERATE_SUCCESS:	//操作成功
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

void CDlgLockMachine::OnMissionLinkError(INT nErrorCode)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！"),MB_ICONERROR);
}

void CDlgLockMachine::OnMissionShutError(BYTE cbShutReason)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("由于当前服务器处理繁忙，操作处理失败，请稍后再重试！"),MB_ICONERROR);
}


void CDlgLockMachine::OnMissionLinkLock()
{
	OnSendData();
}

void CDlgLockMachine::OnMissionReadLockSuccess(VOID * pData, WORD wDataSize)
{
	//关闭连接
	ConcludeMission();
	//效验数据
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) 
	{
		return ;
	}


	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	pGlobalUserData->cbMoorMachine=(m_byMachine==TRUE)?TRUE:FALSE;

	//发送事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_MOOR_MACHINE,0L);

	//显示消息
	if (pOperateSuccess->szDescribeString[0]!=0)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
	}

	//关闭窗口
	__super::OnCancel();
}

void CDlgLockMachine::OnMissionReadLockFailure(VOID * pData, WORD wDataSize)
{
	//关闭连接
	ConcludeMission();

	//效验参数
	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) 
	{
		return ;
	}

	//显示消息
	if (pOperateFailure->szDescribeString[0]!=0)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
	}

	//设置控件
	CEditUI * pEditPassword = (CEditUI *)GetControlByName(szEditPassWordControlName);
	if (pEditPassword !=NULL)
	{
		pEditPassword->SetFocus();
	}
}

void CDlgLockMachine::LoadDataFromUI()
{
	//获取信息
	CEditUI * pEditPassword = (CEditUI *)GetControlByName(szEditPassWordControlName);
	if (pEditPassword !=NULL)
	{
		lstrcpyn(m_szPassword,pEditPassword->GetText(),CountArray(m_szPassword));
	}

}

void CDlgLockMachine::LoadDataFromMemory()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置按钮
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
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	pGlobalUserData->cbMoorMachine = m_byMachine;
}

bool CDlgLockMachine::EfficacyData()
{
	//密码判断
	if (m_szPassword[0]==0)
	{
		//错误提示
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("密码不能为空，请输入银行密码进行绑定或者解除绑定！"),MB_ICONERROR);

		//获取信息
		CEditUI * pEditPassword = (CEditUI *)GetControlByName(szEditPassWordControlName);
		if (pEditPassword !=NULL)
		{
			//设置焦点
			pEditPassword->SetFocus();
		}

		return false;
	}
	return true;
}

void CDlgLockMachine::OnMachineLock(BYTE byLock)
{
	//加载数据
	LoadDataFromUI();

	//验证数据
	if(!EfficacyData())
	{
		return;
	}
	//激活会话
	AvtiveMission(byLock);
}

void CDlgLockMachine::OnSendData()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	CMD_GP_ModifyMachine ModifyMachine;
	ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));
	//构造数据
	ModifyMachine.cbBind=m_byMachine;
	ModifyMachine.dwUserID=pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(ModifyMachine.szMachineID);
	CWHEncrypt::MD5Encrypt(m_szPassword,ModifyMachine.szPassword);
	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_MACHINE,&ModifyMachine,sizeof(ModifyMachine));

}
//////////////////////////////////////////////////////////////////////////////////
