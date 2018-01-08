#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "DlgSearchUser.h"
#include "PlatformFrame.h"
#include "DlgHelp.h"
#include "DlgRecharge.h"
#include "DlgShop.h"
#include "DlgFriend.h"
#include "DlgAddGame.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//��Ļ����
#define LESS_LIST_CX				285									//�б���
#define SPLITTER_WIDTH				8									//��ֿ��

//��Ļ����
#define LESS_SCREEN_CY				740									//��С�߶�
#define LESS_SCREEN_CX				1024								//��С���

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_SERVER_LIST				300									//��Ϸ�б�
#define IDC_WEB_PUBLICIZE			301									//����ؼ�
#define IDC_SKIN_SPLITTER			302									//��ֿؼ�
#define IDC_SYSTEM_TRAY_ICON		303									//����ͼ��
#define IDC_PLAZA_VIEW_GAME_TYPE	304									//���Ϳؼ�
#define IDC_PLAZA_VIEW_GAME			305									//��Ϸ��ͼ
#define IDC_PLAZA_GAME_CONTAINER	306									//��Ϸ����
#define IDC_PLAZA_SERVER_CONTAINER	307									//��������

//�ؼ���ʶ
#define IDC_PLAZA_VIEW_AFFICHE		310									//�������
#define IDC_PLAZA_VIEW_USER_INFO	311									//�û���Ϣ
#define IDC_PLAZA_VIEW_SERVER_CTRL	312									//�������	

//////////////////////////////////////////////////////////////////////////////////

//������Ŀ
#define MAX_SERVER_COUNT			5									//������Ŀ

//ʱ���ʶ
#define IDI_FLASH_TRAY_ICON			10									//����ͼ��
#define IDI_SHOW_GAME_AD			156									//��ʾ�

//ʱ���ʶ
#define IDI_UPDATA_ONLINE			16									//��������
#define TIME_UPDATA_ONLINE			10000								//��������

//////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define WM_TASK_BAR					0x0313								//�������Ҽ�
#define WM_INSTALL_CANCEL_EVENT		8888								//ȡ����װ


//////////////////////////////////////////////////////////////////////////////////

//�ؼ�����

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonResoreControlName = TEXT("ButtonResore");
const TCHAR* const szButtonMaxControlName = TEXT("ButtonMax");
const TCHAR* const szButtonMinControlName = TEXT("ButtonMin");
const TCHAR* const szButtonNavigation1ControlName = TEXT("ButtonNavigation1");
const TCHAR* const szButtonNavigation2ControlName = TEXT("ButtonNavigation2");
const TCHAR* const szButtonNavigation3ControlName = TEXT("ButtonNavigation3");
const TCHAR* const szButtonNavigation4ControlName = TEXT("ButtonNavigation4");

const TCHAR* const szButtonButtonMenu1ControlName = TEXT("ButtonMenu1");
const TCHAR* const szButtonButtonMenu2ControlName = TEXT("ButtonMenu2");
const TCHAR* const szButtonButtonMenu3ControlName = TEXT("ButtonMenu3");
const TCHAR* const szButtonButtonMenu4ControlName = TEXT("ButtonMenu4");
const TCHAR* const szButtonButtonMenu5ControlName = TEXT("ButtonMenu5");

//��ҳ�ؼ�
const TCHAR* const szWebBrowserNoticeControlName = TEXT("WebBrowserNotice");
const TCHAR* const szWebBrowserUserRankControlName = TEXT("WebBrowserUserRank");
const TCHAR* const szWebBrowserBottomAdControlName = TEXT("WebBrowserBottomAd");
const TCHAR* const szWebBrowserMarqueeControlName = TEXT("WebBrowserMarquee");
//����˵�

//��ť�ؼ�
const TCHAR* const szButtonMainControlName = TEXT("ButtonMain");
const TCHAR* const szButtonMySpreadControlName = TEXT("ButtonMySpread");
const TCHAR* const szButtonLockMachineControlName = TEXT("ButtonLockMachine");
const TCHAR* const szButtonHlepControlName = TEXT("ButtonHlep");
const TCHAR* const szButtonMunuControlName = TEXT("ButtonMunu");

//�˵��ؼ�
const TCHAR* const szMenuSettingControlName = TEXT("MenuSetting");
const TCHAR* const szMenuOnlineControlName = TEXT("MenuOnline");
const TCHAR* const szMenuContactControlName = TEXT("MenuContact");
const TCHAR* const szMenuSwitchControlName = TEXT("MenuSwitch");
const TCHAR* const szMenuReStoreControlName = TEXT("MenuReStore");
const TCHAR* const szMenuMinControlName = TEXT("MenuMin");
const TCHAR* const szMenuMaxControlName = TEXT("MenuMax");
const TCHAR* const szMenuExitControlName = TEXT("MenuExit");
const TCHAR* const szMenuShowMainControlName = TEXT("MenuShowMain");


//�м䵼��
const TCHAR* const szButtonMidRechargeControlName = TEXT("ButtonMidRecharge");
const TCHAR* const szButtonMidShopControlName = TEXT("ButtonMidShop");
const TCHAR* const szButtonMidBagControlName = TEXT("ButtonMidBag");
const TCHAR* const szButtonMidBankControlName = TEXT("ButtonMidBank");

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//��̬����

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//���ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFGuiFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//�Զ���Ϣ
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TASK_BAR, OnMessageTaskBar)	
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSTALL_CANCEL_EVENT, OnMessageInstallCancelEvent)

	//�Զ���Ϣ
	ON_MESSAGE(WM_INSUREPLAZA_EVENT, OnMessageInsurePlazaEvent)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformFrame::CPlatformFrame()
:m_PlazaHallList(&m_PaintManager)
,m_PlazaUserInfo(&m_PaintManager)
,m_PlazaHallGame(&m_PaintManager)
{
	//�������
	m_bMaxShow=false;
	m_bRectify=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//״̬����
	m_bWhisperIcon=false;
	m_bAutoMenuEnable=FALSE;

	//�������
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);

	//ƽ̨����
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//���ýӿ�
	CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	pCustomFaceManager->SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	m_pDlgEnquire=NULL;
	return;
}

//��������
CPlatformFrame::~CPlatformFrame()
{
	//ƽ̨����
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//�ӿڲ�ѯ
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//����ʧ��
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->wFaceID=wFaceID;

		//��������
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//ͷ������
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��ǰ����
	if (m_PlazaViewServer.m_hWnd!=NULL)
	{
		m_PlazaViewServer.UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	return;
}


//��ȡ֪ͨ
VOID CPlatformFrame::OnGameItemFinish()
{
	//��ʾ�б�
	//m_PlazaViewGame.ShowKindItemView(m_PlazaViewGameType.GetCurrActiveIndex(),FALSE);

	m_PlazaHallList.ShowPlazaHallList();
	m_PlazaHallGame.ShowPlazaHallGame();

	return;
}

//���֪ͨ
VOID CPlatformFrame::OnGameMatchFinish()
{
	////��ʾ����
	//if(m_PlazaViewGame.GetShowItemMode()==VIEW_MODE_MATCH)
	//{
	//	m_PlazaViewGame.ShowMatchItemView(FALSE);
	//}

	return;
}

//��ȡ֪ͨ
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
	////��ʾ����
	//if(wKindID!=INVALID_WORD)
	//{
	//	m_PlazaViewGame.ShowServerItemView(wKindID,TRUE);
	//}

	return;
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//����ʱ��
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			//��������
			//m_PlazaViewGameType.InsertGameType((CGameTypeItem *)pGameListItem);

			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			break;
		}
	}

	return;
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//����ʱ��
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	switch(pGameListItem->GetItemGenre())
	{		
	case ItemGenre_Type:	//��������
		{
			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			////��ȡ����
			//CGameKindItem * pGameKindItem = (CGameKindItem *)pGameListItem;
			//m_PlazaViewGame.UpdateKindViewItem(pGameKindItem->m_GameKind.wKindID);

			break;
		}
	case ItemGenre_Server:	//��������
		{
			break;
		}
	}

	return;
}

//ɾ��֪ͨ
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

//��Ϣ����
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//�û���¼
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//�л��ʺ�
		{
			//�л�ѯ��
			if (m_PlazaViewServer.m_hWnd!=NULL)
			{
				CDlgInformation Information(this);
				if (Information.ShowMessageBox(TEXT("��Ϸ���伴���رգ�ȷ��Ҫ���л��ʺš��� "),MB_YESNO)!=IDYES) return TRUE;
			}

			//ȫ������
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//������ͼ
			//m_PlazaViewGameType.ResetGameTypeView();

			//��ȡ����
			CServerListData * pServerListData = CServerListData::GetInstance();

			//�������
			POSITION Position=NULL;
			do
			{
				//��ȡ����
				CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(Position);
				if(pGameServerItem!=NULL)
				{
					pGameServerItem->m_bSignuped=false;
					ZeroMemory(&pGameServerItem->m_GameMatch,sizeof(pGameServerItem->m_GameMatch));
				}

			} while (Position!=NULL);

			//�������
			do
			{
				//��ȡ����
				CAgentGameKindItem * pAgentGameKindItem=pServerListData->EmunAgentGameKindItem(Position);
				if(pAgentGameKindItem!=NULL)
				{
					pServerListData->DeleteAgentGameKindItem(pAgentGameKindItem->m_AgentGameKind.wKindID);
				}

			} while (Position!=NULL);

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	case IDM_CLOSE:		//�ر�
		{
			//Ͷ����Ϣ
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case IDM_RESTORE:   //��ԭ
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}
			return TRUE;
		}
	case IDM_MINIMIZE:  //��С��
		{
			if(IsWindowVisible())
			{
				ShowWindow(SW_MINIMIZE);
			}
			return TRUE;
		}
	case IDM_MAXIMIZE:  //���
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}
			MaxSizeWindow();
			return TRUE;
		}
	case IDM_SHOW_LOGON: //��ʾ��¼
		{
			m_MissionLogon.ShowLogon(SW_RESTORE);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

void CPlatformFrame::InitControlUI()
{
	m_PlazaUserInfo.InitControlUI();
	m_PlazaHallList.InitControlUI();

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Notice.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserNoticeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/UserRank.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI2 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserUserRankControlName));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->Navigate(szBillUrl);
	}

	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/PlatformBottom.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI3 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserBottomAdControlName));
	if( pActiveXUI3 ) 
	{
		pActiveXUI3->Navigate(szBillUrl);
	}

	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/SystemMessage.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI4 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserMarqueeControlName));
	if( pActiveXUI4 ) 
	{
		pActiveXUI4->Navigate(szBillUrl);
	}
}


//��Ϣ����
void CPlatformFrame::Notify(TNotifyUI &  msg)
{	
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if ((lstrcmp(msg.sType, TEXT("click")) == 0))
	{
		if(lstrcmp(pControlUI->GetName(), _T("ButtonEdit"))!=0)
		{
			if(lstrcmp(pControlUI->GetName(), _T("ItemGameDelete"))!=0)
			{
				m_PlazaHallGame.CloseDeleteState();
			}
			
		}
		
	}
	else if ((lstrcmp(msg.sType, TEXT("itemactivate")) == 0) ||  (lstrcmp(msg.sType, TEXT("itemclick")) == 0))
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("NodeElementUI")) == 0)
		{
			m_PlazaHallGame.CloseDeleteState();
		}
	}
	//�û����
	if(m_PlazaUserInfo.Notify(msg))
	{
		return;
	}
	if(m_PlazaHallList.Notify(msg))
	{
		return;
	}
	if (m_PlazaHallGame.Notify(msg))
	{
		return;
	}



	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//����˵�
		if(lstrcmp(pControlUI->GetName(), szButtonMainControlName)==0)
		{
						//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sNavigation%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),1);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);	

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMySpreadControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowMySpread();

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonLockMachineControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowLockMachine();

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonHlepControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowHelp();	

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMunuControlName)==0)
		{
			CMenuWnd* pMenu = new CMenuWnd();

			int x = pControlUI->GetX();
			int y = pControlUI->GetY();
			int w=  pControlUI->GetWidth();
			int h=  pControlUI->GetHeight();

			CPoint point;
			point.x = x + w/2;
			point.y = y + h;

			point.x -= 50;
			

			//pControlUI = new
			
			::ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("PlazaMenu.xml"),point, &m_PaintManager, NULL);
		}

		//��ֵ����
		else if(lstrcmp(pControlUI->GetName(), szButtonMidRechargeControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowRecharge();

			return;
		}
		//��Ϸ�̳�
		else if(lstrcmp(pControlUI->GetName(), szButtonMidShopControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowShop();

			return;
		}
		//�ҵı���
		else if(lstrcmp(pControlUI->GetName(), szButtonMidBagControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowBag();

			return;
		}
		//�ҵ�����
		else if(lstrcmp(pControlUI->GetName(), szButtonMidBankControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowInsure();

			return;
		}


		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) 
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMaxControlName)==0)
		{
			//���ڿ���
			if (m_bMaxShow==false) 
			{
				//��󻯴���
				MaxSizeWindow();				
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonResoreControlName)==0)
		{
			//���ڿ���
			if (m_bMaxShow==true)
			{
				//��ԭ����
				RestoreWindow();

				//���ÿؼ�
				pControlUI->SetVisible(false);
				pControlUI = GetControlByName(szButtonMaxControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible();
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonNavigation1ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation2ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation3ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation4ControlName)==0)
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sNavigation%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), _T("ButtonQuit"))==0  )
		{
			//���Ҷ���
			CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallMain"));
			if(pCnt!=NULL)
			{
				pCnt->SetVisible();
			}
			//���Ҷ���
			pCnt = (CContainerUI * )GetControlByName(_T("PlazaRoomMain"));
			if(pCnt!=NULL)
			{
				pCnt->SetVisible(false);
			}
			return;
		}
	}
			//���Ҷ���

}

LPCTSTR CPlatformFrame::GetSkinFile()
{
	return TEXT("PlatformFrame.xml");
}

//�����ؼ�
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;



	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//��������
	if (IsEnteredServerItem())
	{
		if(m_PlazaViewServer.m_bPlaying)
		{
            DeferWindowPos(hDwp,m_PlazaViewContainer,NULL,15,164,nWidth-30,nHeight-177,uFlags);
		}
		else
		{
			DeferWindowPos(hDwp,m_PlazaViewContainer,NULL,15,164,0,0,uFlags);
		}
	}
	else
	{
		DeferWindowPos(hDwp,m_PlazaViewContainer,NULL,15,164,0,0,uFlags);
	}
	

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();		

	//���½���
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//��������
CRect CPlatformFrame::MapSelectedTypeRect()
{
	//��������
	CRect rcSourceType,rcDestType;

	////��ȡ����
	//m_PlazaViewGame.GetWindowRect(&rcDestType);
	//rcSourceType = m_PlazaViewGameType.GetSelectedTypeRect();	

	////��������
	//rcDestType.left = rcSourceType.left;
	//rcDestType.right= rcSourceType.right;
	//rcDestType.bottom = rcDestType.top + rcSourceType.Height();

	////ת������
	//ScreenToClient(&rcDestType);

	return rcDestType;
}

//��������
VOID CPlatformFrame::ShowTaskNotifyer(LPCTSTR pszTaskName)
{


	return;
}

//���ҳ��
bool CPlatformFrame::WebBrowse(LPCTSTR pszURL, bool bAutoFullView)
{
	//���ҳ��
	//CPlazaViewGame::GetInstance()->WebBrowse(pszURL);

	//�������
	//m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewGame);

	//�������
	if (bAutoFullView==true)
	{
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		//if (pParameterGlobal->m_bFullScreenBrowse==true) ControlServerList(ServerListControl_Hide,false);
	}

	return true;
}

bool CPlatformFrame::DownLoadGame( LPCTSTR pszClientName, WORD wKindID, WORD wServerID )
{
	return m_PlazaHallGame.DownLoadGame(pszClientName,wKindID,wServerID);
}


//��ȡ˽��
bool CPlatformFrame::ShowWhisperItem()
{
	//��ȡ��Ϣ
	if (m_DlgWhisperItemArray.GetCount()>0L)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[0];

		//��ʾ����
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//�ö�����
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();
		pDlgWhisper->SetForegroundWindow();

		//ɾ������
		m_DlgWhisperItemArray.RemoveAt(0L);

		//ɾ��ͼ��
		if (m_DlgWhisperItemArray.GetCount()==0L)
		{
			//���ñ���
			m_bWhisperIcon=false;

			//ɾ��ʱ��
			KillTimer(IDI_FLASH_TRAY_ICON);

			//����ͼ��
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		}

		return true;
	}

	return false;
}

//����˽��
bool CPlatformFrame::InsertWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//����Ч��
	ASSERT((pDlgWhisper!=NULL)&&(pDlgWhisper->m_hWnd!=NULL));
	if ((pDlgWhisper==NULL)||(pDlgWhisper->m_hWnd==NULL)) return false;

	//״̬�ж�
	if (pDlgWhisper->IsWindowVisible()) return false;

	//��������
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper) return false;
	}

	//����˽��
	m_DlgWhisperItemArray.Add(pDlgWhisper);

	//����ͼ��
	if (m_DlgWhisperItemArray.GetCount()==1L)
	{
		//���ñ���
		m_bWhisperIcon=true;

		//����ʱ��
		SetTimer(IDI_FLASH_TRAY_ICON,500,NULL);

		//����ͼ��
		m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)));
	}

	return true;
}

//ɾ��˽��
bool CPlatformFrame::RemoveWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//��������
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper)
		{
			//ɾ������
			m_DlgWhisperItemArray.RemoveAt(i);

			//ɾ��ͼ��
			if (m_DlgWhisperItemArray.GetCount()==0L)
			{
				//���ñ���
				m_bWhisperIcon=false;

				//ɾ��ʱ��
				KillTimer(IDI_FLASH_TRAY_ICON);

				//����ͼ��
				m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
			}

			return true;
		}
	}

	return false;
}

//�����
bool CPlatformFrame::ActiveServerViewItem()
{
	//�����ж�
	ASSERT(m_PlazaViewServer.m_hWnd!=NULL);
	if (m_PlazaViewServer.m_hWnd==NULL) return false;

	//��ס����
	LockWindowUpdate();

	//������ͼ
	m_PlazaViewContainer.RemoveViewItem(VIA_Left);
	m_PlazaViewContainer.RemoveViewItem(VIA_Top);	
	m_PlazaViewContainer.InsertViewItem(&m_PlazaViewServer,VIA_Center);	
	m_PlazaViewContainer.InsertViewItem(&m_PlazaViewServerCtrl,VIA_Right);		

	//��������
	UnlockWindowUpdate();

	CRect rc;
	GetWindowRect(rc);
	RectifyControl(rc.right-rc.left,rc.bottom-rc.top);
	//��������
	m_PlazaViewContainer.RectifyViewItem();
	m_PlazaViewContainer.SetActiveWindow();

	//���Ҷ���
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaRoomMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}
	//���Ҷ���
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}

	return true;
}

//ɾ������
bool CPlatformFrame::DeleteServerViewItem()
{
	//�����ж�
	if (m_PlazaViewServer.m_hWnd==NULL) return false;

	//��ס����
	LockWindowUpdate();

	//���ٷ���
	m_PlazaViewServer.DestroyWindow();	
	m_PlazaViewServerCtrl.DestroyWindow();

	//��Ϸ��ͼ
	//m_PlazaViewContainer.InsertViewItem(&m_PlazaViewGameType,VIA_Top);	
	//m_PlazaViewContainer.InsertViewItem(&m_PlazaViewGame,VIA_Center);	
	//m_PlazaViewContainer.InsertViewItem(&m_PlazaViewAffiche,VIA_Right);	

	m_PlazaViewContainer.RemoveViewItem(VIA_Center);
	m_PlazaViewContainer.RemoveViewItem(VIA_Right);
	//��������
	UnlockWindowUpdate();

	CRect rc;
	GetWindowRect(rc);
	RectifyControl(rc.right-rc.left,rc.bottom-rc.top);



	//��������
	m_PlazaViewContainer.RectifyViewItem();

	//���Ҷ���
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}
	//���Ҷ���
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaRoomMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}
	//ɾ��״̬
	m_PlazaHallGame.CloseDeleteState();

	return true;
}

//���뷿��
bool CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//Ч�����
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��¼�ж�
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//��������
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//��ǰ�ж�
	if (m_PlazaViewServer.m_hWnd!=NULL)
	{
		////��ʾ��Ϣ
		//TCHAR szBuffer[256]=TEXT("");
		//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ڽ��� [ %s ] ��Ϸ�����У����Ժ�ȴ�Ƭ��..."),m_PlazaViewServer.GetServerName());

		////��ʾ��Ϣ
		//CDlgInformation Information(this);
		//Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return false;
	}

	//ȡ��ʵ��
	m_PlazaUserInfo.CancleRealTip();



	//��������
	try
	{
		//���÷���
		m_PlazaViewServer.Create(IDD_DLG_CHILD,this);
		m_PlazaViewServer.InitServerViewItem(pGameServerItem,m_wAVServerPort,m_dwAVServerAddr);

		//���ÿؼ�
		m_PlazaViewServerCtrl.Create(IDD_GAME_RIGHT_CONTROL,this);
	}
	catch (LPCTSTR pszString)
	{
		//���ٴ���
		if(m_PlazaViewServer.m_hWnd) m_PlazaViewServer.DestroyWindow();
		if(m_PlazaViewServerCtrl.m_hWnd) m_PlazaViewServerCtrl.DestroyWindow();

		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return false;
	}

	return true;
}

//��ȡ����
CControlUI * CPlatformFrame::GetPlazaControlByName(LPCTSTR pszControlName)
{
	return GetControlByName(pszControlName);
}


void CPlatformFrame::ReturnHallMain()
{
	m_PlazaHallGame.ReturnHallMain();
}

void CPlatformFrame::ShowWindowMaskLayer(bool bShow /*=true*/)
{
	CControlUI* pActiveXUI1 = GetControlByName(_T("Lableshadow"));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->SetVisible(bShow);
	}
}

//��ԭ����
bool CPlatformFrame::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ñ���
		m_bMaxShow=false;
		m_bRectify=false;

		//��󻯰�ť
		CControlUI * pControlUI = GetControlByName(szButtonResoreControlName); 
		if(pControlUI!=NULL) pControlUI->SetVisible(false);

		//��ԭ��ť
		pControlUI = GetControlByName(szButtonMaxControlName);
		if(pControlUI!=NULL) pControlUI->SetVisible();

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��󴰿�
bool CPlatformFrame::MaxSizeWindow(bool bRecordPosition)
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//���ñ���
		m_bMaxShow=true;
		m_bRectify=false;

		//Ĭ��λ��
		if(bRecordPosition==true) GetWindowRect(&m_rcNormalSize);

		//��󻯰�ť
		CControlUI * pControlUI = GetControlByName(szButtonMaxControlName); 
		if(pControlUI!=NULL) pControlUI->SetVisible(false);

		//��ԭ��ť
		pControlUI = GetControlByName(szButtonResoreControlName);
		if(pControlUI!=NULL) pControlUI->SetVisible();

		//��ȡλ��
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//�ر���Ϣ
VOID CPlatformFrame::OnClose()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�ر���ʾ
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//��ʾ����
		INT_PTR nResult = IDCANCEL;
		if( m_pDlgEnquire == NULL )
		{
			CDlgEnquire DlgEnquire;
			m_pDlgEnquire=&DlgEnquire;
			nResult=DlgEnquire.DoModal();
			m_pDlgEnquire = NULL;
		}
		
		//�����
		switch (nResult)
		{
		case IDCANCEL:				//ȡ������
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//�˳�����
			{
				//�رշ���
				if(m_PlazaViewServer.m_hWnd!=NULL)
				{
					m_PlazaViewServer.SendMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
				}

				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//�л��ʺ�
			{
				//Ͷ����Ϣ
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);

				return;
			}
		}
	}

	//ע���ȼ�
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//ʱ����Ϣ
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//����ͼ��
		{
			//���ñ���
			m_bWhisperIcon=!m_bWhisperIcon;

			//����ͼ��
			UINT uIconID=(m_bWhisperIcon==true)?IDI_WHISPER:IDI_NULL;
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIconID)));

			return;
		}
	case IDI_CHECK_USER_BUFF:		//������״̬
	case IDI_CHECK_SEND_PRESENT:	//������ޱ�����	
		{
			m_PlazaUserInfo.OnTimer(nIDEvent);
			return;
		}
	case IDI_SHOW_GAME_AD:
		{
			KillTimer(nIDEvent);
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowGameAd();
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);
	return;
}

//λ����Ϣ
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//����λ��
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//������Ϣ
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION|WS_BORDER, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);	

	//��������
	CRect rcCreate(0,0,0,0);

	//��������
	m_PlazaViewContainer.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_PLAZA_GAME_CONTAINER);

	//��Ϸ�б�
	CServerListData::GetInstance()->SetServerListDataSink(this);

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;

	//�ƶ�����
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	////��ʾ����
	//SetWindowRgn(NULL,TRUE);

	//��¼ϵͳ
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//ע���ȼ�
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey)==false)
	{
		//CDlgInformation Information(this);
		//Information.ShowMessageBox(TEXT("ϵͳ�ȼ���ͻ���ϰ��ȼ�ע��ʧ�ܣ�"));
	}

	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey)==false)
	{
		//CDlgInformation Information(this);
		//Information.ShowMessageBox(TEXT("ϵͳ�ȼ���ͻ����ȡ��Ϣ�ȼ�ע��ʧ�ܣ�"));
	}
	//��������
	m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	if(pParameterGlobal->m_bFullScreenBrowse)
	{
		MaxSizeWindow();
	}
	else
	{
		RestoreWindow();
	}

	//��������
	SetTimer(IDI_UPDATA_ONLINE, TIME_UPDATA_ONLINE, NULL);

	//��������
	CSkinDialog::SetWndFont(this,NULL);

	SetWindowText(szProduct);

	return 0;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>GetCaptionRect().Height()) return;

	//���ڿ���
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//���øı�
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//���ڿ���
	if (m_bMaxShow==true)
	{
		m_bMaxShow=false;
		MaxSizeWindow(false);
	}
	else
	{
		m_bMaxShow=true;
		RestoreWindow();
	}

	return;
}

//�ȼ���Ϣ
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//�ϰ��ȼ�
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//��¼�ж�
			if (pGlobalUserData->dwUserID!=0L)
			{
				//��������
				bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

				//���ش���
				if (bBossCome==false)
				{
					//��ԭ����
					ShowWindow(SW_RESTORE);
					ShowWindow(SW_SHOW);

					//�ö�����
					SetActiveWindow();
					BringWindowToTop();
					SetForegroundWindow();

					//��ʾͼ��
					m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//���ش���
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//����ͼ��
					m_SystemTrayIcon.HideTrayIcon();
				}

				////������Ϣ
				//for (INT_PTR i=0;i<m_PlazaViewServerArray.GetCount();i++)
				//{
				//	ASSERT(m_PlazaViewServerArray[i]!=NULL);
				//	m_PlazaViewServerArray[i]->NotifyBossCome(bBossCome);
				//}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//˽���ȼ�
		{
			//��ȡ��Ϣ
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 0;
		}
	}

	return 0;
}

//������Ϣ
LRESULT CPlatformFrame::OnMessageTaskBar(WPARAM wParam, LPARAM lParam)
{
	//��ʾ�˵�
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	MousePoint.y-=10;

	CMenuWnd* pMenu = new CMenuWnd();
	pMenu->Init(NULL, _T("PlazaMenuTray.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);

	return 0L;
}

//ͼ����Ϣ
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//��굥��
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}

			//�����
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();

			return 1;
		}
	case WM_LBUTTONDBLCLK:		//���˫��
		{
			//��ȡ��Ϣ
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}			

			return 1;
		}
	case WM_RBUTTONUP:		//��굥��
	case WM_RBUTTONDBLCLK:		//���˫��
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			//�û�δ��½
			if(pGlobalUserData->dwUserID == 0)
			{
				//��ʾ�˵�
				CPoint MousePoint;
				GetCursorPos(&MousePoint);
				MousePoint.y-=10;

				CMenuWnd* pMenu = new CMenuWnd();
				pMenu->Init(NULL, _T("PlazaMenuTrayLogon.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);
			}
			else
			{
				//��ʾ�˵�
				CPoint MousePoint;
				GetCursorPos(&MousePoint);
				MousePoint.y-=10;

				CMenuWnd* pMenu = new CMenuWnd();
				pMenu->Init(NULL, _T("PlazaMenuTray.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);
			}			

			return 1;
		}
	}

	return 0;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//����ģ��
	if(m_PlazaUserInfo.OnMessagePlatformEvent(wParam, lParam))
	{
		return 0L;
	}
	//����ģ��
	if(m_PlazaHallGame.OnMessagePlatformEvent(wParam, lParam))
	{
		return 0L;
	}
	//����ģ��
	if (m_PlazaHallList.OnMessagePlatformEvent(wParam,lParam))
	{
		return 0L;
	}

	//�¼�����
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼���
		{
			//��ʾ����
			ShowWindow(SW_SHOW);

			//ǰ�ô���
			SetForegroundWindow();

			//�û����
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->WriteUserCookie();

			//��ȡ����
			TCHAR szPath[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szPath, sizeof(szPath));
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PlazaConfig.INI"),szPath);

			TCHAR	szGameAd[MAX_PATH];
			ZeroMemory(szGameAd,CountArray(szGameAd));
			GetPrivateProfileString(TEXT("GameAd"),TEXT("DisEnable"),TEXT(""),szGameAd,CountArray(szGameAd),szFileName);
			int nGameAd = _ttoi(szGameAd);
			if (nGameAd==0)
			{
				WORD wTodayShowDate = 0;
				CWHRegKey RegOptionItem;
				RegOptionItem.OpenRegKey(TEXT(""),true);
				wTodayShowDate=(WORD)RegOptionItem.GetValue(TEXT("TodayShowDate"),wTodayShowDate);

				//��ȡʱ��
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime); 
				WORD wDay=SystemTime.wDay;	
				WORD wMonth=SystemTime.wMonth;
				WORD wYear=SystemTime.wYear-1980;

				WORD wCurrDate = wYear<<16|wMonth<<8|wDay;

				if (wTodayShowDate != wCurrDate)
				{
					//�����ڸ�
					ShowWindowMaskLayer();

					//��ʾ���
					SetTimer(IDI_SHOW_GAME_AD,800,NULL);
				}


			}

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//���ش���
			ShowWindow(SW_HIDE);

			//�رշ���			
			if(m_PlazaViewServer.m_hWnd!=NULL)
			{
				m_PlazaViewServer.SendMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
			}
			m_PlazaUserInfo.ClearBuff();

			//��ʾ��¼
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//�������
		{
			//��ȡ����
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//�����б�
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//���뷿��
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	case EVENT_USER_SHOW_REAL_TIP:  //��ʾʵ��
		{
			//ASSERT(CGlobalUnits::GetInstance()!=NULL);
			//CGlobalUnits::GetInstance()->PerformShowRealTip();
			m_PlazaUserInfo.ShowRealTip();
			return 0L;
		}
	case EVENT_USER_HIDE_REAL_TIP:
		{
			m_PlazaUserInfo.HideRealTip();
			return 0L;
		}
	case EVENT_USER_SHOW_CHECKIN:	//��ʾǩ��
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowCheckIn();

			return 0L;
		}
	case EVENT_USER_SHOW_MY_SPREAD:	//��ʾ�ƹ�
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowMySpread();

			return 0L;
		}
	case EVENT_USER_SHOW_TASKVIEW:	//��ʾ����
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowTaskView();

			return 0L;
		}
	case EVENT_USER_SHOW_RECHARGE: //��ʾ��ֵ
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowRecharge();
			return 0L;
		}
	case EVENT_USER_SHOW_PERSONER: //��������
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowPersonerCenter((BYTE)lParam);
			return 0L;
		}
	case EVENT_USER_SHOW_MY_INSURE:
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowInsure();
			return 0L;
		}
	case EVENT_USER_SHOW_BAG_RECORD:
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowBagRecord();
			return 0L;
		}
	case EVENT_USER_SHOW_BASE_ENSURE: //��ʾ�ͱ�
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowBaseEnsure();
			return 0L;
		}
	case EVENT_USER_SHOW_BAG: //��ʾ����
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowBag();
			return 0L;
		}
	case EVENT_USER_SHOW_EXCHANGE: //��ʾ�һ�
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowExchange();
			return 0L;
		}
	case EVENT_USER_SHOW_SHOP: //��ʾ�̳�
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowShop();
			return 0L;
		}
	case EVENT_USER_SHOW_UPDATE_BUFF: //����BUFF
		{
			tagPropertyBuff * pPropertyBuff= (tagPropertyBuff*)lParam;
			m_PlazaUserInfo.UpdateBuffData(*pPropertyBuff);
			m_PlazaUserInfo.UpdateBuffUI();
			KillTimer(IDI_CHECK_USER_BUFF);
			SetTimer(IDI_CHECK_USER_BUFF, 1000, NULL);
			return 0L;
		}
	case EVENT_USER_CLOSE_GAME_AD:
		{
			ShowWindowMaskLayer(false);
			return 0L;
		}
	case EVENT_USER_TRACK_ACCOUNTS:	//׷�ٶ���
		{
			//��ȡ����
			tagTrackAccountsInfo * pTrackAccountsInfo= (tagTrackAccountsInfo *)lParam;
			if(pTrackAccountsInfo!=NULL)
			{
				//��ȡ����
				ASSERT(CServerListData::GetInstance());
				CServerListData * pServerListData = CServerListData::GetInstance();
				if(pServerListData!=NULL)
				{
					//���ҷ���
					CGameServerItem * pGameServerItem = pServerListData->SearchGameServer(pTrackAccountsInfo->wServerID);
					if(pGameServerItem!=NULL)
					{
						//���ñ�ʶ
						ASSERT(CGlobalUnits::GetInstance()!=NULL);
						CGlobalUnits::GetInstance()->SetTrackUserID(pTrackAccountsInfo->dwTargetID);
						
						//�رշ���
						if(m_PlazaViewServer.m_hWnd!=NULL)
						{
							m_PlazaViewServer.SendMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
						}

						//���뷿��
						if (m_PlazaViewServer.m_hWnd==NULL) 
						{
							//���뷿��
							EntranceServerItem(pGameServerItem);
						}

						return 0L;
					}
				}

				////��������
				//CGameKindItem * pGameKindItem = pServerListData->SearchGameKind(pTrackAccountsInfo->wKindID);
    //            if(pGameKindItem != NULL)
				//{
				//	//���ñ�ʶ
				//	ASSERT(CGlobalUnits::GetInstance()!=NULL);
				//	CGlobalUnits::GetInstance()->SetTrackUserID(pTrackAccountsInfo->dwTargetID);
				//	CGlobalUnits::GetInstance()->SetTrackServerID(pTrackAccountsInfo->wServerID);

				//	//��������
				//	tagGameKindInfo GameKindInfo;
				//	GameKindInfo.pGameKindItem=pGameKindItem;
				//	m_PlazaViewGame.OnViewPageKindItemClicked(&GameKindInfo);

				//	return 0L;
				//}
			}
			
			//ʧ����ʾ
			CDlgInformation Information;
			Information.ShowMessageBox(TEXT("��ʾ"),TEXT("û���ҵ���������ڵķ��䣡"));

			return 0L;
		}
	}

	return 0L;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam)
{
	//��ʾ��¼
	m_MissionLogon.ShowLogon();

	return 0L;
}

void CPlatformFrame::SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr)
{
	m_wAVServerPort=wAVServerPort;
	m_dwAVServerAddr=dwAVServerAddr;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam)
{
	tagInsurePlazaEvent * pInsureEvent = (tagInsurePlazaEvent*)wParam;
	if(pInsureEvent==NULL) return 0L;

	//��������
	if(pInsureEvent->wCommandID==INSURE_COMMAND_UPDATE)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//����
		pGlobalUserData->lUserScore = pInsureEvent->lUserScore;
		pGlobalUserData->lUserInsure = pInsureEvent->lUserInsure;

		//������Ϣ
		m_PlazaUserInfo.UpdateUI();
	}

	//�һ�����
	if(pInsureEvent->wCommandID==INSURE_COMMAND_EXCHANGE)
	{
		//��ʾ�һ�
		ASSERT(CGlobalUnits::GetInstance());
		CGlobalUnits::GetInstance()->PerformShowExchange();		
	}

	//�ͷŶ���
	if(pInsureEvent->bHeapData==true) SafeDelete(pInsureEvent);

	return 0;
}



//�Զ�����Ϣ
LRESULT CPlatformFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		CStdString *strMenuName = (CStdString*)wParam;	

		if ( *strMenuName == szMenuSettingControlName) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowSetup();	
	
		}
		else if( *strMenuName == szMenuOnlineControlName )
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowFeedBack();		
		}
		else if( *strMenuName == szMenuContactControlName )
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowContact();			
		}
		else if( *strMenuName == szMenuSwitchControlName )
		{
			CPaintManagerUI::InvokAllCloseCallBack();
			//Ͷ����Ϣ
			PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);
		}
		else if( *strMenuName == szMenuReStoreControlName )
		{
			//Ͷ����Ϣ
			PostMessage(WM_COMMAND,IDM_RESTORE,0);
		}
		else if( *strMenuName == szMenuMinControlName )
		{
			//Ͷ����Ϣ
			PostMessage(WM_COMMAND,IDM_MINIMIZE,0);
		}
		else if( *strMenuName == szMenuMaxControlName )
		{
			//Ͷ����Ϣ
			PostMessage(WM_COMMAND,IDM_MAXIMIZE,0);

		}
		else if( *strMenuName == szMenuExitControlName )
		{
			CPaintManagerUI::InvokAllCloseCallBack();

			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			pGlobalUserData->dwUserID = 0L;

			//�رմ���
			AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
		}
		else if( *strMenuName == szMenuShowMainControlName )
		{
			//Ͷ����Ϣ
			PostMessage(WM_COMMAND,IDM_SHOW_LOGON,0);
		}
		delete strMenuName;
	}
	//else if(lParam == WM_RBUTTONUP)
	//{
	//	//��ʾ�˵�
	//	CPoint MousePoint;
	//	GetCursorPos(&MousePoint);

	//	CMenuWnd* pMenu = new CMenuWnd();
	//	pMenu->Init(NULL, _T("PlazaMenuTray.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);
	//}

	return 1L;
}

void CPlatformFrame::OnWkeURLHidden()
{
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserNoticeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->RunJS(TEXT("document.body.style.overflow='hidden'"));
	}

	CWkeBrowserUI* pActiveXUI2 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserUserRankControlName));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->RunJS(TEXT("document.body.style.overflow='hidden'"));
	}
}


//////////////////////////////////////////////////////////////////////////////////

