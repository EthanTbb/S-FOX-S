#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//ϵͳ�ļ�
#include "Mmsystem.h"

//�ؼ��ļ�
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
//��̬����

CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUnits::CGlobalUnits()
{
	//���ñ���
	//m_pDlgDownLoad=NULL;
	m_pDlgBagPlaza=NULL;
	m_pDlgSetup=NULL;

	//�汾��Ϣ
	m_dwPlazaVersion=VERSION_PLAZA;
	m_dwFrameVersion=VERSION_FRAME;

	//׷�ٱ���
	m_dwTrackUserID=0;
	m_wTrackServerID=0;

	//���ö���
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	ZeroMemory(&m_AuthRealParameter, sizeof(m_AuthRealParameter));
	

	return;
}

//��������
CGlobalUnits::~CGlobalUnits()
{
	//�ͷŶ���
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	////�ͷŶ���
	//if(m_pDlgDownLoad!=NULL) m_pDlgDownLoad->DestroyWindow();
	//SafeDelete(m_pDlgDownLoad);

	return;
}

//��¼���
bool CGlobalUnits::WriteUserCookie()
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��¼���
	if (pParameterGlobal->m_bWriteCookie==true)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�������
		TCHAR szUserID[32]=TEXT("");
		_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

		//�û�����
		CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),szUserID);
		CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),pGlobalUserData->szAccounts);
		CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),pGlobalUserData->szPassword);
	}

	return true;
}

//ɾ�����
bool CGlobalUnits::DeleteUserCookie()
{
	//ɾ����Ϣ
	CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),TEXT(""));

	return true;
}

//��������
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�����ж�
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//��������
	PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);

	return true;
}

//��������
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�����ж�
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//������Դ
	HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
	if (hResource==NULL) return false;

	//������Դ
	HGLOBAL hGlobal=LoadResource(hInstance,hResource);
	if (hGlobal==NULL) return false;

	//��������
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

	//������Դ
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return true;
}

//��ȡ��ʶ
DWORD CGlobalUnits::GetTrackUserID()
{
	//��������
	DWORD dwTrackUserID=m_dwTrackUserID;

	//���ñ���
	m_dwTrackUserID=0;

	return dwTrackUserID;
}

//��ȡ����
WORD CGlobalUnits::GetTrackServerID()
{
	//��������
	WORD wTrackServerID=m_wTrackServerID;

	//���ñ���
	m_wTrackServerID=0;

	return wTrackServerID;
}

////������Ϸ
//bool CGlobalUnits::DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
//{
//	//��������
//	if (m_pDlgDownLoad==NULL)
//	{
//		m_pDlgDownLoad=new CDlgDownLoad;
//	}
//
//	//��������
//	if (m_pDlgDownLoad->m_hWnd==NULL)
//	{
//		m_pDlgDownLoad->Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
//	}
//
//	//������Ϸ
//	m_pDlgDownLoad->DownLoadClient(pszClientName,wKindID,wServerID);
//
//	return true;
//}

bool CGlobalUnits::QuerySendPresentMisson()
{
	//��ȡָ��
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

//��ʾ��������
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

//��ʾ����
VOID CGlobalUnits::PerformShowInsure()
{
	//��ȡ����
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

//��ʾ����
VOID CGlobalUnits::PerformShowTaskView()
{
	//��ȡ����
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowTask();
	}
	else
	{
		//��������
		CDlgTask task;
		task.DoModal();
	}
	return;
}

//��ʾ�ƹ�
VOID CGlobalUnits::PerformShowMySpread()
{
	CDlgMySpread DlgMySpread;
	DlgMySpread.DoModal();
}

//��ʾ�һ�
VOID CGlobalUnits::PerformShowExchange()
{
	//��ȡ����
	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatformFrame->GetPlazaViewServer();

	//��������
	if(IsWindow(pPlazaViewServer->GetSafeHwnd())==FALSE)
	{
		//��������
		CDlgExchangePlaza DlgExchangePlaza;
		DlgExchangePlaza.DoModal();
	}
	else
	{
		pPlazaViewServer->ShowExchange();
	}
}

//��ʾǩ��
VOID CGlobalUnits::PerformShowCheckIn()
{

	//��ȡ����
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowCheckIn();
	}
	else
	{
		//��������
		CDlgCheckIn DlgCheckIn;
		DlgCheckIn.DoModal();
	}


}

//��ʾ�ͱ�
VOID CGlobalUnits::PerformShowBaseEnsure()
{

	//��ȡ����
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if(pPlatformFrame->IsEnteredServerItem()==true)
	{
		pPlatformFrame->GetPlazaViewServer()->ShowBaseEnsure();
	}
	else
	{
		//��������
		CDlgBaseEnsure DlgBaseEnsure;
		DlgBaseEnsure.DoModal();
	}


}

//��������
VOID CGlobalUnits::PerformShowLockMachine()
{
	CDlgLockMachine DlgLockMachine;
	DlgLockMachine.DoModal();
}

//�����û�
VOID CGlobalUnits::PerformShowSearchUser()
{
	CDlgSearchUser SearchUser;
	SearchUser.DoModal();
}

//��ֵ����
VOID CGlobalUnits::PerformShowRecharge()
{
	CDlgRecharge recharge;
	recharge.DoModal();	
}

//��Ϸ�̳�
bool CGlobalUnits::PerformShowShop()
{
	//��ȡָ��
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

//�ҵı���
bool CGlobalUnits::PerformShowBag()
{
	//��ȡָ��
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

//����ʹ��
VOID CGlobalUnits::PerformShowPropertyUse(IClientUserItem* pClientUserItem)
{
	//��ȡָ��
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() )
	{
		pPlatformFrame->GetPlazaViewServer()->ShowPropertyUse(pClientUserItem);
	}
}

//�ҵĺ���
VOID CGlobalUnits::PerformShowFriend()
{
	CDlgFriend friends;
	HWND hWndPranet = CPlatformFrame::GetInstance()->GetSafeHwnd();
	friends.DoModal();
}

//ϵͳ����
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

//��ϵ�ͷ�
VOID CGlobalUnits::PerformShowContact()
{
	CDlgContact contact;
	contact.DoModal();
}

//������Ϣ
VOID CGlobalUnits::PerformShowFeedBack()
{
	CDlgFeedBack feedback;
	feedback.DoModal();
}

//��������
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
