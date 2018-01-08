#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//系统文件
#include "Mmsystem.h"

//控件文件
#include "DlgCheckIn.h"
#include "DlgExChange.h"
#include "DlgMySpread.h"
#include "DlgTaskCenter.h"
#include "DlgSearchUser.h"
#include "DlgBaseEnsure.h"
#include "DlgLockMachine.h"
#include "DlgRecharge.h"
#include "DlgShop.h"
#include "DlgBag.h"
#include "DlgFriend.h"
#include "DlgSetup.h"
#include "DlgContact.h"
#include "DlgFeedBack.h"
#include "DlgHelp.h"
#include "DlgTask.h"
#include "DlgInsure.h"
#include "DlgIndividual.h"
#include "DlgRealTip.h"
#include "DlgBagRecord.h"
#include "DlgGame.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalUnits::CGlobalUnits()
{
	//设置变量
	//m_pDlgDownLoad=NULL;
	m_pDlgBagPlaza=NULL;
	m_pDlgSetup=NULL;

	//版本信息
	m_dwPlazaVersion=VERSION_PLAZA;
	m_dwFrameVersion=VERSION_FRAME;

	//追踪变量
	m_dwTrackUserID=0;
	m_wTrackServerID=0;

	//设置对象
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	ZeroMemory(&m_AuthRealParameter, sizeof(m_AuthRealParameter));
	

	return;
}

//析构函数
CGlobalUnits::~CGlobalUnits()
{
	//释放对象
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	////释放对象
	//if(m_pDlgDownLoad!=NULL) m_pDlgDownLoad->DestroyWindow();
	//SafeDelete(m_pDlgDownLoad);

	return;
}

//记录甜饼
bool CGlobalUnits::WriteUserCookie()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//记录甜饼
	if (pParameterGlobal->m_bWriteCookie==true)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//构造变量
		TCHAR szUserID[32]=TEXT("");
		_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

		//用户资料
		CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),szUserID);
		CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),pGlobalUserData->szAccounts);
		CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),pGlobalUserData->szPassword);
	}

	return true;
}

//删除甜饼
bool CGlobalUnits::DeleteUserCookie()
{
	//删除信息
	CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),TEXT(""));

	return true;
}

//播放声音
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//播放判断
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//播放声音
	PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);

	return true;
}

//播放声音
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//播放判断
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//加载资源
	HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
	if (hResource==NULL) return false;

	//锁定资源
	HGLOBAL hGlobal=LoadResource(hInstance,hResource);
	if (hGlobal==NULL) return false;

	//播放声音
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

	//清理资源
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return true;
}

//获取标识
DWORD CGlobalUnits::GetTrackUserID()
{
	//变量定义
	DWORD dwTrackUserID=m_dwTrackUserID;

	//设置变量
	m_dwTrackUserID=0;

	return dwTrackUserID;
}

//获取房间
WORD CGlobalUnits::GetTrackServerID()
{
	//变量定义
	WORD wTrackServerID=m_wTrackServerID;

	//设置变量
	m_wTrackServerID=0;

	return wTrackServerID;
}

////下载游戏
//bool CGlobalUnits::DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
//{
//	//创建对象
//	if (m_pDlgDownLoad==NULL)
//	{
//		m_pDlgDownLoad=new CDlgDownLoad;
//	}
//
//	//创建窗口
//	if (m_pDlgDownLoad->m_hWnd==NULL)
//	{
//		m_pDlgDownLoad->Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
//	}
//
//	//下载游戏
//	m_pDlgDownLoad->DownLoadClient(pszClientName,wKindID,wServerID);
//
//	return true;
//}

bool CGlobalUnits::QuerySendPresentMisson()
{
	//获取指针
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() )
	{
		return pPlatformFrame->GetPlazaViewServer()->QuerySendPresentMisson();
	}
	else
	{
		if(m_pDlgBagPlaza != NULL)
		{
			return m_pDlgBagPlaza->QuerySendPresentMisson();
		}
	}
	return false;
}

//显示个人中心
VOID CGlobalUnits::PerformShowPersonerCenter(BYTE cbSelectedIndex/* = 0*/)
{
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowIndividual();
	}
	else
	{
		CDlgIndividual dlg;
		dlg.SetTabSelectedIndex(cbSelectedIndex);
		dlg.DoModal();
	}
}

//显示银行
VOID CGlobalUnits::PerformShowInsure()
{
	//获取房间
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowInsureView();
	}
	else
	{
		CDlgInsure insure;
		insure.DoModal();
	}

	return;
}

//显示任务
VOID CGlobalUnits::PerformShowTaskView()
{
	//获取房间
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowTask();
	}
	else
	{
		//变量定义
		CDlgTask task;
		task.DoModal();
	}
	return;
}

//显示推广
VOID CGlobalUnits::PerformShowMySpread()
{
	CDlgMySpread DlgMySpread;
	DlgMySpread.DoModal();
}

//显示兑换
VOID CGlobalUnits::PerformShowExchange()
{
	//获取房间
	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatformFrame->GetPlazaViewServer();

	//变量定义
	if(IsWindow(pPlazaViewServer->GetSafeHwnd())==FALSE)
	{
		//变量定义
		CDlgExchangePlaza DlgExchangePlaza;
		DlgExchangePlaza.DoModal();
	}
	else
	{
		pPlazaViewServer->ShowExchange();
	}
}

//显示签到
VOID CGlobalUnits::PerformShowCheckIn()
{

	//获取房间
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowCheckIn();
	}
	else
	{
		//变量定义
		CDlgCheckIn DlgCheckIn;
		DlgCheckIn.DoModal();
	}


}

//显示低保
VOID CGlobalUnits::PerformShowBaseEnsure()
{

	//获取房间
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowBaseEnsure();
	}
	else
	{
		//变量定义
		CDlgBaseEnsure DlgBaseEnsure;
		DlgBaseEnsure.DoModal();
	}


}

//锁定机器
VOID CGlobalUnits::PerformShowLockMachine()
{
	CDlgLockMachine DlgLockMachine;
	DlgLockMachine.DoModal();
}

//查找用户
VOID CGlobalUnits::PerformShowSearchUser()
{
	CDlgSearchUser SearchUser;
	SearchUser.DoModal();
}

//充值中心
VOID CGlobalUnits::PerformShowRecharge()
{
	CDlgRecharge recharge;
	recharge.DoModal();	
}

//游戏商场
bool CGlobalUnits::PerformShowShop()
{
	//获取指针
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() )
	{
		pPlatformFrame->GetPlazaViewServer()->ShowShop();
	}
	else
	{
		CDlgShopPlaza shop;
		shop.DoModal();
	}
	return true;
}

//我的背包
bool CGlobalUnits::PerformShowBag()
{
	//获取指针
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() )
	{
		pPlatformFrame->GetPlazaViewServer()->ShowBag();
	}
	else
	{
		CDlgBagPlaza dlgBagPlaza;
		m_pDlgBagPlaza = &dlgBagPlaza;
		dlgBagPlaza.DoModal();
		m_pDlgBagPlaza = NULL;
	}
	return true;
}

bool CGlobalUnits::PerformShowBagRecord()
{
	CDlgBagRecord dlgBag;
	dlgBag.DoModal();

	return true;
}

//道具使用
VOID CGlobalUnits::PerformShowPropertyUse(IClientUserItem* pClientUserItem)
{
	//获取指针
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() )
	{
		pPlatformFrame->GetPlazaViewServer()->ShowPropertyUse(pClientUserItem);
	}
}

//我的好友
VOID CGlobalUnits::PerformShowFriend()
{
	CDlgFriend friends;
	HWND hWndPranet = CPlatformFrame::GetInstance()->GetSafeHwnd();
	friends.DoModal();
}

//系统设置
VOID CGlobalUnits::PerformShowSetup()
{
	if(m_pDlgSetup == NULL)
	{
		CDlgSetup setup;
		m_pDlgSetup = &setup;
		setup.DoModal();
		m_pDlgSetup = NULL;
	}
}

//联系客服
VOID CGlobalUnits::PerformShowContact()
{
	CDlgContact contact;
	contact.DoModal();
}

//反馈信息
VOID CGlobalUnits::PerformShowFeedBack()
{
	CDlgFeedBack feedback;
	feedback.DoModal();
}

//帮助中心
VOID CGlobalUnits::PerformShowHelp()
{
	CDlgHelp help;
	help.DoModal();
}

VOID CGlobalUnits::PerformShowGameAd()
{
	CDlgGame game;
	game.DoModal();
}

VOID CGlobalUnits::PerformShowRealTip()
{
	CDlgRealTip realTip;
	realTip.DoModal();
}

VOID CGlobalUnits::SetAuthRealParameter(const tagAuthRealParameter* pInfo)
{
	memcpy(&m_AuthRealParameter, pInfo, sizeof(m_AuthRealParameter));
}
const tagAuthRealParameter* CGlobalUnits::GetAuthRealParameter()
{
	return &m_AuthRealParameter;
}

//////////////////////////////////////////////////////////////////////////////////
