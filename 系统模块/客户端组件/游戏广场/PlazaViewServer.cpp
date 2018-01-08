#include "StdAfx.h"
#include "GamePlaza.h"
#include "PlazaViewServer.h"

//���ݿؼ�
#include "GlobalUnits.h"
#include "PlatformEvent.h"

//����ؼ�
#include "PlatformFrame.h"

//�Ի�����
#include "DlgSearchUser.h"
#include "DlgServerManager.h"
#include "DlgTablePassword.h"
#include "DlgInputPassword.h"
#include "DlgCollocateRoom.h"
#include "DlgIssueMessage.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//��������
#define PROPERTY_CY					89									//��Ϸ����
#define PUBLICIZE_CY				90									//��Ϸ���
#define INCISE_SCALE				47/100								//����ָ�
#define RIGHT_VIEW_WIDTH			260									//����ͼ���

//������С
#define FALSH_MOVIE_CX				300									//�������
#define FALSH_MOVIE_CY				300									//�����߶�

//��������
#define SPLITTER_CY					3									//��ָ߶�
#define SPLITTER_CX					8									//��ֿ��
#define TABLE_BUTTON_SX				5									//��ť��϶
#define	TABLE_BUTTON_CX				330									//��ť����

//�ؼ���ʶ
#define IDC_CHAT_CONTROL			100									//�������
#define IDC_SERVER_TABLE			101									//���ӿؼ�
#define IDC_GAME_PROPERTY			102									//��Ϸ����
#define IDC_WEB_PUBLICIZE			103									//����ؼ�

//ʱ�Ӷ���
#define IDI_GUIDE_TASK				200									//��������
#define TIME_GUIDE_TASK				3000								//��������

//�����ж�
#define CLOSE_NETWORK_DOWN          88                                  //�����ж�

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR * szButtonQuitServerControlName=TEXT("ButtonClose");
const TCHAR * szButtonTableButton1ControlName=TEXT("ButtonTable1");
const TCHAR * szButtonTableButton2ControlName=TEXT("ButtonTable2");
const TCHAR * szButtonTableButton3ControlName=TEXT("ButtonTable3");
const TCHAR * szButtonTableButton4ControlName=TEXT("ButtonTable4");
const TCHAR * szButtonTableButton5ControlName=TEXT("ButtonTable5");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewServer, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_NCDESTROY()

	////��������
	ON_COMMAND(IDM_MANAGER_SERVER, OnManageServer)
	ON_COMMAND(IDM_MANAGER_USER, OnManageUser)
	ON_COMMAND(IDM_MANAGER_MESSAGE, OnManageSystemMsg)

	//�Զ���Ϣ
	ON_MESSAGE(WM_ADD_FRIEND, OnWhisperAddFriend)
	ON_MESSAGE(WM_SET_DETEST, OnWhisperSetDetest)

	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaViewServer::CPlazaViewServer()
{
	//�������
	m_bCreateFlag=false;

	//��������
	m_dwChatTime=0L;
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;
	m_wInvitedTableID=INVALID_TABLE;
	m_bUserInRoom=false;

	//�û�����
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//��Ϸ����
	m_pITableViewFrame=NULL;

	//��������
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;
	m_szServerPassword[0]=0;

	//���ò���
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	//�ؼ�����
	m_pDlgSearchTable=NULL;
	m_pDlgInsureServer=NULL;
	m_pDlgServerManager=NULL;
	m_pDlgExchangeServer=NULL;
	m_pPlazaViewServerCtrl=NULL;
	m_pDlgTaskServerCenter=NULL;
	m_pDlgCheckIn =NULL;
	m_pDlgBaseEnsure = NULL;
	m_pDlgShopServer = NULL;
	m_pDlgBagServer = NULL;
	m_pDlgPropertyUse = NULL;
	m_pDlgCheckIn=NULL;

	m_bPlaying = false;

	//����ؼ�
	m_pDlgManageUser=NULL;

	//�������
	m_DlgStatus.SetStatusViewSink(this);

	//������Ϣ
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));


	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//������Դ
	tagEncircleResource	EncircleTable;
	EncircleTable.pszImageTL=MAKEINTRESOURCE(IDB_TABLE_TL);
	EncircleTable.pszImageTM=MAKEINTRESOURCE(IDB_TABLE_TM);
	EncircleTable.pszImageTR=MAKEINTRESOURCE(IDB_TABLE_TR);
	EncircleTable.pszImageML=MAKEINTRESOURCE(IDB_TABLE_ML);
	EncircleTable.pszImageMR=MAKEINTRESOURCE(IDB_TABLE_MR);
	EncircleTable.pszImageBL=MAKEINTRESOURCE(IDB_TABLE_BL);
	EncircleTable.pszImageBM=MAKEINTRESOURCE(IDB_TABLE_BM);
	EncircleTable.pszImageBR=MAKEINTRESOURCE(IDB_TABLE_BR);
	m_TableEncircle.InitEncircleResource(EncircleTable,AfxGetInstanceHandle());

	return;
}

//��������
CPlazaViewServer::~CPlazaViewServer()
{
	if (m_ImageGameName.IsNull()==false) m_ImageGameName.DestroyImage();

	return;
}

//�ӿڲ�ѯ
VOID * CPlazaViewServer::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IFlashControlSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertyUseSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableViewFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}

//��Ϣ����
void CPlazaViewServer::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonQuitServerControlName)==0) 
		{
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton1ControlName)==0)
		{
			return OnTableButton1Clicked();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton2ControlName)==0)
		{
			return OnTableButton2Clicked();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton3ControlName)==0)
		{
			return OnTableButton3Clicked();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton4ControlName)==0)
		{
			return OnTableButton4Clicked();
		}	
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton5ControlName)==0)
		{
			return OnTableButton5Clicked();
		}
	}

	return;
}

//��ʼ�滭
void CPlazaViewServer::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ����
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	CRect rcTable;
	rcTable.top=0;
	rcTable.left=0;
	rcTable.right=rcClient.right;
	rcTable.bottom=rcClient.Height();
	m_TableEncircle.DrawEncircleFrame(pDC,rcTable);

	//��ȡ����
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(13,45,75));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//λ����Ϣ
	CRect rcTextTitle(20,10,120,23);
	TCHAR szTextTitle[64]=TEXT("����λ�ã�");

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(154,109,43));
	pDC->DrawText(szTextTitle,lstrlen(szTextTitle),&rcTextTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

	//������Ϣ
	pDC->SetTextColor(RGB(219,183,131));
	GetWindowText(szTextTitle,CountArray(szTextTitle));

	//�滭����
	rcTextTitle.SetRect(80,10,rcClient.Width()-TABLE_BUTTON_CX-8,22);
	pDC->DrawText(szTextTitle,lstrlen(szTextTitle),&rcTextTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);
}

//�ؼ���
VOID CPlazaViewServer::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	return;
}

//��������
BOOL CPlazaViewServer::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();



	//�����ؼ�
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IFlashControlSink);
	m_FlashControl.CreateControl(this,CRect(0,0,FALSH_MOVIE_CX,FALSH_MOVIE_CY),pIUnknownEx);
	m_FlashControl.ShowControl(SW_HIDE);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//��ȡ����
	ASSERT(CPlatformFrame::GetInstance()!=NULL);
	m_pPlazaViewServerCtrl = CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl();	

	//�������
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("�ȼ������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("�û������������ʧ�ܣ�");

	//�������
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));

	//�������
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("�û������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("�û������������ʧ�ܣ�");

	//���ñ���
	m_bCreateFlag=true;

	//���ý���
	InitServerViewCtrl(CSkinRenderManager::GetInstance());

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	return TRUE;
}

//��Ϣ����
BOOL CPlazaViewServer::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	//������Ϣ
	if (pMsg->message==WM_KEYDOWN)
	{
		//ȡ����ť
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//�س���ť
		if (pMsg->wParam==VK_RETURN)
		{
			PostMessage(WM_COMMAND,IDC_BT_SEND_CHAT,0);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlazaViewServer::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//��ť����
	/*switch (nCommandID)
	{*/
	//case IDC_BT_TABLE_BUTTON_4:		//��Ϸ����
	//	{
	//		//�������
	//		CCollocateRoom DlgCollocate;
	//		DlgCollocate.InitCollocate(GetParameterGame(),GetParameterServer());

	//		//��ʾ����
	//		if (DlgCollocate.DoModal()==IDOK)
	//		{
	//			//���͹���
	//			SendUserRulePacket();
	//		}
	//		return TRUE;
	//	}
	
	//�˵�����
	switch (nCommandID)
	{
	case IDM_GAME_RULE:				//��Ϸ����
		{
			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szBrowser[MAX_PATH]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameRule.aspx?GameID=%ld&KindID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wGameID,m_GameServer.wKindID);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_GAME_CHART:			//��Ϸ����
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szBrowser[256]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/Rank/GameChart.aspx?GameID=%ld&KindID=%ld&ServerID=%ld&UserID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wGameID,m_GameServer.wKindID,m_GameServer.wServerID,pGlobalUserData->dwUserID);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_DELETE_SERVER_ITEM:	//�رշ���
		{
			if(IsPlayingMySelf() && lParam!=CLOSE_NETWORK_DOWN)
			{
				//��ʾ��Ϣ
				CDlgInformation Information(this);
				INT nRes=0;
				if (m_wServerType!=GAME_GENRE_MATCH)
					nRes=Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
				else
					nRes=Information.ShowMessageBox(TEXT("�����ڱ����У�ǿ���˳����ᱻϵͳ�йܣ���Ӯ�Ը���ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
				if (nRes!=IDYES)
				{
					return TRUE;
				}
				OnGameProcessClose(0);
				m_pIMySelfUserItem->GetUserInfo()->cbUserStatus=US_FREE;
			}

			//�ж�����
			if (m_TCPSocketModule.GetInterface()!=NULL)
			{
				m_TCPSocketModule->CloseSocket();
				m_ServiceStatus=ServiceStatus_NetworkDown;
			}

			m_bPlaying =false;

			//�رշ���
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->DeleteServerViewItem();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//���˫��
VOID CPlazaViewServer::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//�������
VOID CPlazaViewServer::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	//��������
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;

	//��ȡ�û�
	IClientUserItem * pTableUserItem=NULL;
	if (wChairID!=INDEX_ENTER_CHAIR) pTableUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

	//�����λ
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//��Ϣ����
	if (pTableUserItem!=NULL)
	{
		if (pTableUserItem==m_pIMySelfUserItem)
		{
			//״̬����
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//��������
			PerformStandUpAction(wTableID,wChairID);
		}
		else
		{
			//״̬����
			WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
			WORD wMeChairID=m_pIMySelfUserItem->GetChairID();
			if ((wMeTableID==wTableID)&&(wMeChairID==wChairID)) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//�Թ۶���
			PerformLookonAction(wTableID,wChairID);
		}
	}
	else
	{
		//�������
		if (wChairID==INDEX_ENTER_CHAIR)
		{
			//��������
			WORD wUserTableID=m_pIMySelfUserItem->GetTableID();
			BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

			//�������
			if ((wTableID==wUserTableID)&&(cbUserStatus!=US_LOOKON))
			{
				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
				return;
			}
		}

		//���¶���
		PerformSitDownAction(wTableID,wChairID,true);
	}

	return;
}

//�Ҽ�����
VOID CPlazaViewServer::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	//���λ��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

		//�û��˵�
		if (pIClientUserItem!=NULL)
		{
			TrackUserItemMenu(pIClientUserItem);
		}

		return;
	}

	return;
}

//���Ӱ�ť
VOID CPlazaViewServer::OnHitTableButton(BYTE cbButton)
{
	//��ť����
	switch (cbButton)
	{
	case BT_JOIN_GAME:		//������Ϸ
	case BT_JOIN_MATCH:		//�������
	case BT_GROUP_MATCH:	//�������
		{
			//������Ϸ
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
			}

			break;
		}
	case BT_QUIT_MATCH:	//�˳���Ϸ
		{
			//�˳���Ϸ
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH);
			}
		}
	case BT_REVIVE_MATH:
		{
			//����ṹ
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				//��������
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_USER_REVIVE,NULL,0);
			}
		}
	}
}

//�����¼�
bool CPlazaViewServer::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������ʾ
	if (nErrorCode!=0L)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("�ܱ�Ǹ����Ϸ��������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONSTOP);

		return true;
	}

	//���͵�¼
	SendLogonPacket(m_szServerPassword);

	//����״̬
	SetServiceStatus(ServiceStatus_Validate);

	return true;
}

//�ر��¼�
bool CPlazaViewServer::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�ر�״̬
	m_DlgStatus.HideStatusWindow();

	//�ر���Ϸ
	m_ProcessManager.CloseGameProcess(false);

	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//��ȡָ��
			IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			ASSERT(pIStringMessage!=NULL);

			//��ʾ��Ϣ
			pIStringMessage->InsertSystemString(TEXT("����Ϸ�������������ж���"));

			//�ر���ʾ
			CDlgInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("����Ϸ�������������Ѿ��жϣ��Ƿ�رյ�ǰ��Ϸ���䣿"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//�رշ���
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//�ر���ʾ
			CDlgInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("���������ԭ�򣬷�������ʧ�ܣ����Ժ����³��ԣ�"),MB_ICONERROR);

			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//����״̬
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//��ȡ�¼�
bool CPlazaViewServer::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	bool br=false;
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:		//��¼��Ϣ
		{
			br=OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_CONFIG:		//������Ϣ
		{
			br=OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_USER:		//�û���Ϣ
		{
			br=OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_STATUS:		//״̬��Ϣ
		{
			br=OnSocketMainStatus(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_INSURE:		//������Ϣ
		{
			br=OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_BASEENSURE:		//�ͱ���Ϣ
		{
			br=OnSocketMainBaseEnsure(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_MEMBER:
		{
			br=OnSocketMainMember(Command.wSubCmdID,pData,wDataSize);
			return true;
		}
	case MDM_GR_CHECKIN:		//ǩ����Ϣ
		{
			br=OnSocketMainCheckIn(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_TASK:		//������Ϣ
		{
			br=OnSocketMainTask(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_EXCHANGE:	//�һ���Ϣ
		{
			br=OnSocketMainExchange(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_PROPERTY:	//������Ϣ
		{
			br=OnSocketMainProperty(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_MANAGE:		//������Ϣ
		{
			br=OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_CM_SYSTEM:		//ϵͳ��Ϣ
		{
			br=OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GF_GAME:		//��Ϸ��Ϣ
	case MDM_GF_FRAME:		//�����Ϣ
		{
			br=OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_MATCH:		//������Ϣ
		{
			br=OnSocketMainMatch(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	}

	return true;
}

//�������
VOID CPlazaViewServer::OnEventFlashFinish()
{
	m_FlashControl.ShowControl(SW_HIDE);
}

//�������
bool CPlazaViewServer::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//Ч�����
	ASSERT((pszNickName!=NULL)&&(wItemCount>0));
	if ((pszNickName==NULL)||(wItemCount==0)) return false;

	//ִ�й���
	return PerformBuyProperty(PT_ISSUE_AREA_SERVER,pszNickName,wItemCount,wPropertyIndex);
}

bool CPlazaViewServer::OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage)
{
	//�����׳�
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
	{
		TCHAR szMessage[128]=TEXT("�����׳��������������");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	//����û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByNickName(pszNickName);
	if(pIClientUserItem==NULL)
	{
		TCHAR szMessage[128]=TEXT("��ָ����ʹ�ö����ڱ������л򲻴��ڣ�");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	return true;
}

//ʹ�õ���
bool CPlazaViewServer::OnEventUseProperty(WORD wPropertyIndex)
{
	////��������
	//if(wPropertyIndex==PROPERTY_ID_TRUMPET || wPropertyIndex==PROPERTY_ID_TYPHON)
	//{
	//	//������
	//	CMD_GR_C_SendTrumpet SendTrumpet;
	//	SendTrumpet.cbRequestArea=PT_ISSUE_AREA_SERVER;
	//	SendTrumpet.wPropertyIndex=wPropertyIndex;
	//	SendTrumpet.TrumpetColor=m_pTrumpetItem->GetTrumpetColor();
	//	m_pTrumpetItem->GetTrumpetContent(SendTrumpet.szTrumpetContent);

	//	//������Ϣ
	//	m_TCPSocketModule->SendData(MDM_GR_PROPERTY,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet));
	//}

	return true;
}

//���̴���
bool CPlazaViewServer::OnGameProcessError()
{
	OnGameProcessClose(0);

	return true;
}

//���̹ر�
bool CPlazaViewServer::OnGameProcessClose(DWORD dwExitCode)
{
	//Ч��״̬
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//�˳���Ϸ
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	m_pITableViewFrame->SetTableStatus(false);

	//��ԭ����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage(WM_COMMAND,IDM_RESTORE,0);

	return true;
}

//��������
bool CPlazaViewServer::OnGameProcessCreate(bool bAlreadyExist)
{
	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//��������
	if (bAlreadyExist==false)
	{
		//��ȡ�ȼ�
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//�ȼ�����
		WORD wLevelCount;
		tagGrowLevelConfig GrowLevelItem[60];
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		wLevelCount=pUserItemElement->GetExperienceInfo(GrowLevelItem,CountArray(GrowLevelItem));

		//��������
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
		m_ProcessManager.SendGrowLevelConfig(GrowLevelItem,wLevelCount);
		m_ProcessManager.SendGamePropertyTrumpet(m_pPlazaViewServerCtrl->m_dwTrumpetCount,m_pPlazaViewServerCtrl->m_dwTyphonCount);
		m_ProcessManager.SendGamePropertyTypeInfo();
		m_ProcessManager.SendGamePropertyRelatInfo();
		m_ProcessManager.SendGamePropertyInfo();
	}

	//������Ϣ
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_GameServer,m_wAVServerPort,m_dwAVServerAddr);
	
	//������Ϣ
	CServerListData * pServerListData = CServerListData::GetInstance();
	CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(m_GameServer.wServerID);
	if (pGameServerItem!=NULL) m_ProcessManager.SendMatchInfo(pGameServerItem->m_GameMatch);

	//�����û�
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//��������
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//�������
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//�Թ��û�
		wEnumIndex=0;
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//�������
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);

	return true;
}

//��������
bool CPlazaViewServer::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//�û�����
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_USER_ATTRIB))
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
		if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;

		//Ѱ���û�
		IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pData;
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserAttrib->dwUserID);

		//�û��ж�
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return true;

		//��������
		ASSERT(CUserInformation::GetInstance()!=NULL);
		CUserInformation * pUserInformation=CUserInformation::GetInstance();

		//���ù�ϵ
		m_PlazaUserManagerModule->UpdateUserItemAttrib(pIClientUserItem,&pUserAttrib->UserAttrib);
		pUserInformation->InsertCompanionInfo(pIClientUserItem,pUserAttrib->UserAttrib.cbCompanion);

		return true;
	}

	//�߳��û�
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_KICK_USER))
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(IPC_GF_KickUser));
		if (wDataSize!=sizeof(IPC_GF_KickUser)) return false;

		//Ѱ���û�
		IPC_GF_KickUser * pKickUser=(IPC_GF_KickUser *)pData;
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pKickUser->dwTargetUserID);

		//�û��ж�
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return true;

		//������Ϸ
		if((m_pIMySelfUserItem->GetMasterOrder()==0) && (m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR))
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("�ܱ�Ǹ��������Ϸ�������ˣ�"),MB_ICONQUESTION);
			return true;
		}

		//��������
		CMD_GR_KickUser KickUser;
		KickUser.dwTargetUserID=pKickUser->dwTargetUserID;

		//��������
		m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_KICK_USER,&KickUser,sizeof(KickUser));

		return true;
	}

	//���ٻ�λ
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_QUICK_TRANSPOS))
	{
		//��������
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		BYTE cbActionHitAutoJoin=pParameterGlobal->m_cbActionHitAutoJoin;

		//ִ�ж���
		pParameterGlobal->m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;

		//��������
		SendMessage(WM_COMMAND,IDC_BT_TABLE_BUTTON_1,0L);

		//�ָ�����
		pParameterGlobal->m_cbActionHitAutoJoin=cbActionHitAutoJoin;

		return true;
	}

	if ((wMainCmdID==IPC_CMD_GF_MATCH_INFO)&&(wSubCmdID==IPC_SUB_GF_EXIT_MATCH))
	{
		//��������
		m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH);

		return true;
	}

	//������Ϣ
	if ((wMainCmdID==IPC_CMD_GF_PROPERTY_INFO))
	{
		//�������
		if(wSubCmdID==IPC_SUB_GF_BUY_PROPERTY)
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_BuyProperty));
			if (wDataSize!=sizeof(IPC_GF_BuyProperty)) return false;

			//��ȡ����
			IPC_GF_BuyProperty * pBuyProperty=(IPC_GF_BuyProperty *)pData;
			ASSERT(pBuyProperty!=NULL);
			if(pBuyProperty==NULL) return false;

			//ִ�й���
			PerformBuyProperty(PT_ISSUE_AREA_GAME,pBuyProperty->szNickName,pBuyProperty->wItemCount,pBuyProperty->wPropertyIndex);
		}

		//���ȵ���
		if(wSubCmdID==IPC_SUB_GR_PROPERTY_TRUMPET)
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GR_PropertyTrumpet));
			if (wDataSize!=sizeof(IPC_GR_PropertyTrumpet)) return false;

			//��ȡ����
			IPC_GR_PropertyTrumpet * pPropertyTrumpet=(IPC_GR_PropertyTrumpet *)pData;
			ASSERT(pPropertyTrumpet!=NULL);
			if(pPropertyTrumpet==NULL) return false;

			//����ṹ
			CMD_GR_C_SendTrumpet SendTrumpet;
			SendTrumpet.cbRequestArea=PT_ISSUE_AREA_GAME;
			SendTrumpet.wPropertyIndex=pPropertyTrumpet->wPropertyIndex;
			SendTrumpet.TrumpetColor=pPropertyTrumpet->TrumpetColor;
			CopyMemory(SendTrumpet.szTrumpetContent,pPropertyTrumpet->szTrumpetContent,sizeof(SendTrumpet.szTrumpetContent));

			//������Ϣ
			m_TCPSocketModule->SendData(MDM_GR_PROPERTY,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet));
		}

        return true;                
	}

	//������Ϣ
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//Ч������
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//��������
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//��������
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			if(pCommand->wMainCmdID==MDM_GR_INSURE && pCommand->wSubCmdID==SUB_GR_QUERY_INSURE_INFO)
			{
				//��������
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//��������
				CMD_GR_C_QueryInsureInfoRequest *pQueryInsureInfo=(CMD_GR_C_QueryInsureInfoRequest *)pSocketPackage->cbBuffer;
				pQueryInsureInfo->cbActivityGame=TRUE;
				lstrcpyn(pQueryInsureInfo->szInsurePass,pGlobalUserData->szPassword,CountArray(pQueryInsureInfo->szInsurePass));
			}
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�û�����
VOID CPlazaViewServer::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//�ж��Լ�
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//��������
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//�û�����
	m_pPlazaViewServerCtrl->OnEventUserEnter(pIClientUserItem);

	return;
}

//�û�ɾ��
VOID CPlazaViewServer::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//��������
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//�뿪����
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//�뿪֪ͨ
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserLeave(pIClientUserItem);
	}

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemDelete(pIClientUserItem,this);

	//�û�����
	m_pPlazaViewServerCtrl->OnEventUserLeave(pIClientUserItem);

	return;
}

//�û�����
VOID CPlazaViewServer::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//�û�����
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//��������
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		m_pITableViewFrame->UpdateTableView(pUserInfo->wTableID);
	}

	//������Ϸ
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(m_pIMySelfUserItem->GetTableID()==pUserInfo->wTableID))
	{
		m_ProcessManager.SendCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,pCustomFaceInfo);
	}

	return;
}

//�û�����
VOID CPlazaViewServer::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//�û�����
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//����֪ͨ
	if (pIClientUserItem==m_pIMySelfUserItem)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		if(m_wServerType==GAME_GENRE_GOLD)
		{
			pGlobalUserData->lUserScore=pIClientUserItem->GetUserScore();
		}
		
		//���²Ƹ�
		pGlobalUserData->lLoveLiness=pUserInfo->lLoveLiness;
		pGlobalUserData->lUserIngot=pIClientUserItem->GetUserIngot();
		pGlobalUserData->lUserInsure=pIClientUserItem->GetUserInsure();
		pGlobalUserData->dUserBeans=pUserInfo->dBeans;

		//�¼�֪ͨ
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��Ϸ֪ͨ
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//��������
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//���ñ���
		UserScore.lScore=pUserInfo->lScore;
		UserScore.lGrade=pUserInfo->lGrade;
		UserScore.lIngot=pUserInfo->lIngot;
		UserScore.lInsure=pUserInfo->lInsure;
		UserScore.dBeans=pUserInfo->dBeans;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwExperience=pUserInfo->dwExperience;
		UserScore.lLoveLiness=pUserInfo->lLoveLiness;
		UserScore.lIntegralCount=pUserInfo->lIntegralCount;

		//��������
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//�û�����
VOID CPlazaViewServer::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	//�û�����
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//�����뿪
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//���Ӽ���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//����жϣ���������
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//������
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pIClientUserItem->GetUserCompanion()==CP_DETEST)
				{
					CMD_GR_UserRepulseSit UserRepulseSit;
					UserRepulseSit.wTableID=wNowTableID;
					UserRepulseSit.wChairID=wNowChairID;
					UserRepulseSit.dwUserID=pMeUserInfo->dwUserID;
					UserRepulseSit.dwRepulseUserID=pUserInfo->dwUserID;
					m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_REPULSE_SIT,&UserRepulseSit,sizeof(UserRepulseSit));
				}
			}
		}
		m_pITableViewFrame->SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//����״̬
	if ((m_pITableViewFrame->GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		m_pITableViewFrame->UpdateTableView(wNowTableID);
	}

	//�뿪֪ͨ
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//���봦��
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//�Լ��ж�
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			//���ñ���
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;

			//��������
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//״̬�ı�
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//��Ϸ֪ͨ
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//�û�����
VOID CPlazaViewServer::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib)
{
	//��������
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wUserTableID=pIClientUserItem->GetTableID();

	//�û�����
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//֪ͨ��Ϸ
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//��������
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//����֪ͨ
		m_ProcessManager.SendUserAttrib(pIClientUserItem->GetUserID(),UserAttrib);
	}

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	return;
}

//ȡ������
VOID CPlazaViewServer::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//��ʾ�˵�
VOID CPlazaViewServer::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	m_pPlazaViewServerCtrl->TrackUserItemMenu(pIClientUserItem);
}

//��ʾ�˵�
VOID CPlazaViewServer::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	m_pPlazaViewServerCtrl->TrackUserItemMenu(pIClientUserItem,wUserCount);
}


VOID CPlazaViewServer::ShowIndividual()
{
	CDlgInformation Information;
	Information.ShowMessageBox(TEXT("ϵͳ��ʾ"),TEXT("�ܱ�Ǹ���÷����ֹ������������"),MB_ICONERROR);
}

VOID CPlazaViewServer::ShowBaseEnsure()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgBaseEnsureServer DlgBaseEnsureServer;
	DlgBaseEnsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//���ñ���
	m_pDlgBaseEnsure=&DlgBaseEnsureServer;

	//��ʾ����
	DlgBaseEnsureServer.DoModal();

	//���ñ���
	m_pDlgBaseEnsure=NULL;

}


VOID CPlazaViewServer::ShowCheckIn()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgCheckInServer DlgCheckInServer;
	DlgCheckInServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//���ñ���
	m_pDlgCheckIn=&DlgCheckInServer;

	//��ʾ����
	DlgCheckInServer.DoModal();

	//���ñ���
	m_pDlgCheckIn=NULL;

}


//��ʾ����
VOID CPlazaViewServer::ShowInsureView()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//�ǽ�ҷ���
	if(m_wServerType!=GAME_GENRE_GOLD)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("ϵͳ��ʾ"),TEXT("�ܱ�Ǹ���÷����ֹ��������"),MB_ICONERROR);

		return;
	}

	//��������
	CDlgInsureServerNew DlgInsureServer;
	DlgInsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//���ñ���
	m_pDlgInsureServer=&DlgInsureServer;

	//��ʾ����
	DlgInsureServer.DoModal();

	//���ñ���
	m_pDlgInsureServer=NULL;

	return;
}

//��ʾ����
VOID CPlazaViewServer::ShowTask()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgTaskServer TaskServerCenter;
	TaskServerCenter.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//���ñ���
	m_pDlgTaskServerCenter=&TaskServerCenter;

	//��ʾ����
	TaskServerCenter.DoModal();

	//���ñ���
	m_pDlgTaskServerCenter=NULL;
}

//��ʾ�һ�
VOID CPlazaViewServer::ShowExchange()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgExchangeServer DlgExchangeServer;
	DlgExchangeServer.Initialization(m_TCPSocketModule.GetInterface());
	
	//���ñ���
	m_pDlgExchangeServer=&DlgExchangeServer;

	//��ʾ����
	DlgExchangeServer.DoModal();

	//���ñ���
	m_pDlgExchangeServer=NULL;
}

//��ʾ�̳�
VOID CPlazaViewServer::ShowShop()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//�ǽ�ҷ���
	if(m_wServerType!=GAME_GENRE_GOLD)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("ϵͳ��ʾ"),TEXT("�ܱ�Ǹ���÷����ֹ�����̳�"),MB_ICONERROR);

		return;
	}

	//��������
	CDlgShopServer DlgShopServer;
	DlgShopServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//���ñ���
	m_pDlgShopServer=&DlgShopServer;

	//��ʾ����
	DlgShopServer.DoModal();

	//���ñ���
	m_pDlgShopServer=NULL;
	return;
}

VOID CPlazaViewServer::ShowBag()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgBagServer DlgBagServer;
	DlgBagServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//���ñ���
	m_pDlgBagServer=&DlgBagServer;

	//��ʾ����
	DlgBagServer.DoModal();

	//���ñ���
	m_pDlgBagServer=NULL;
}

VOID CPlazaViewServer::ShowPropertyUse(IClientUserItem* pClientUserItem)
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgPropertyUseServer DlgPropertyUseServer;
	DlgPropertyUseServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem, pClientUserItem);

	//���ñ���
	m_pDlgPropertyUse=&DlgPropertyUseServer;
	//��ʾ����
	INT_PTR res = DlgPropertyUseServer.DoModal();
	m_pDlgPropertyUse = NULL;

	//����
	if(res == IDOK)
	{
		ShowShop();
	}
}

bool CPlazaViewServer::QuerySendPresentMisson()
{
	if(m_pDlgBagServer != NULL)
	{
		return m_pDlgBagServer->QuerySendPresentMisson();
	}
	return false;
}

//�ϰ��¼�
VOID CPlazaViewServer::NotifyBossCome(bool bBossCome)
{
	return;
}

//��������
VOID CPlazaViewServer::SearchGameTable(tagSearchTable & SearchTable)
{
	//��������
	WORD wNullCount=0;
	WORD wChairUser=m_pITableViewFrame->GetChairCount();
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();

	//������Ϸ
	if (CServerRule::IsAllowDynamicJoin(m_dwServerRule)==true)
	{
		if(m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
		{
			SearchTable.wResultTableID=0;
			SearchTable.wResultChairID=INDEX_ENTER_CHAIR;

			return;
		}
	}

	//��������
	for (WORD i=0;i<m_pITableViewFrame->GetTableCount();i++)
	{
		//��������
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%m_pITableViewFrame->GetTableCount();

		//��������
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//��ȡ����
			ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(SearchTable.wResultTableID);

			//��������
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//Ѱ�ҿ�λ��
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//�ж���Ŀ
			if (wNullCount>0)
			{
				//Ч�����
				CString strDescribe;
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}
		
				//��Ŀ�ж�
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return;
			}
		}
	}

	//��������
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return;
}

//����ͷ��
VOID CPlazaViewServer::UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//�����û�
	ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule.GetInterface()->SearchUserByUserID(dwUserID);

	//�����û�
	if (pIClientUserItem!=NULL)
	{
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		m_PlazaUserManagerModule->UpdateUserCustomFace(pIClientUserItem,dwCustomID,CustomFaceInfo);
	}

	return;
}

//��ѯ��Ϸ�ͻ���
bool CPlazaViewServer::IsGameClientReady()
{
	return m_ProcessManager.IsProcessReady();
}

//�Լ�״̬
bool CPlazaViewServer::IsPlayingMySelf()
{
	return ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetUserStatus()==US_PLAYING));
}

//�����û�
IClientUserItem * CPlazaViewServer::SearchUserByUserID (DWORD dwUserID)
{
	//��ȡ�û�
	IClientUserItem * pTargetUserItem=NULL;
	pTargetUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID);
	
	return pTargetUserItem;
}

//����������
bool CPlazaViewServer::SitDownInviteTable(WORD wTableID, bool bInRoom)
{
	//���ñ���
	m_wInvitedTableID=wTableID;
	m_bUserInRoom=bInRoom;

	if (bInRoom==true)
	{
		//�ҿ���λ
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_pITableViewFrame->GetNullChairCount(m_wInvitedTableID,wChairID);

		if (wNullCount==0) 
		{
			CDlgInformation Information2(this);
			Information2.ShowMessageBox(TEXT("����Ϸ���Ѿ�û�п�λ���ˣ�"),MB_ICONINFORMATION);
			return true;
		}

		//������������
		bool bSitDown=PerformSitDownAction(m_wInvitedTableID,wChairID,false);
		m_wInvitedTableID=INVALID_TABLE;
	}

	return true;
}

//�û�����
bool CPlazaViewServer::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//�û�����
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_USER_GAME_DATA:		//�û���Ϸ����
		{
			return OnSocketSubUserItemGameData(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//����ʧ��
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return OnSocketSubUserChat(pData,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:	//�û�����
		{
			return OnSocketSubExpression(pData,wDataSize);
		}
	case SUB_GR_WISPER_CHAT:		//�û�˽��
		{
			return OnSocketSubWisperUserChat(pData,wDataSize);
		}
	case SUB_GR_WISPER_EXPRESSION:	//˽�ı���
		{
			return OnSocketSubWisperExpression(pData,wDataSize);
		}
	case SUB_GR_USER_INVITE:	   //�������
		{
			return OnSocketSubUserInvite(pData,wDataSize);
		}
	case SUB_GR_USER_WAIT_DISTRIBUTE: //�ȴ�����
		{
			return OnSocketSubWaitDistribute(pData,wDataSize);
		}
	}

	return true;
}

//��¼����
bool CPlazaViewServer::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//��¼�ɹ�
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//��¼ʧ��
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//��¼���
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//������ʾ
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//���ô���
bool CPlazaViewServer::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//�б�����
		{
			//��������
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//Ч�����
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//������Ϣ
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			//�����б�
			m_pPlazaViewServerCtrl->SetColumnDescribe(pConfigColumn->ColumnItem,pConfigColumn->cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//��Ϣ����
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//��������
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;

          	//�������
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			m_wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��������
			bool bSuccess=m_pITableViewFrame->ConfigTableFrame(wTableCount,wChairCount,m_dwServerRule,m_wServerType,m_GameServer.wServerID,szResDirectory);

			//������
			if (bSuccess==false)
			{
				//������ʾ
				m_DlgStatus.HideStatusWindow();

				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				CDlgInformation Information(AfxGetMainWnd());
				INT nResult=Information.ShowMessageBox(TEXT("��Ϸ��Դ�ļ�����ʧ�ܣ��Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CPlatformFrame::GetInstance()->DownLoadGame(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
					CPlatformFrame::GetInstance()->ReturnHallMain();
					//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					//pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//�������
		{
			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��Ϸ�ȼ�
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//������
			if (bSuccess==false)
			{
				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				CDlgInformation Information(this);
				INT nResult=Information.ShowMessageBox(TEXT("��Ϸ�ȼ����ö�ȡʧ�ܻ��߸�ʽ�����Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CPlatformFrame::GetInstance()->DownLoadGame(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
					//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					//pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			//���óɹ�
			if(bSuccess==true)
			{
				m_pPlazaViewServerCtrl->SetServerInfo(m_wChairCount,m_wServerType,m_dwServerRule,m_dwUserRight,m_dwMasterRight);

				//��������
				CGamePropertyListData *pGamePropertyListData = CGamePropertyListData::GetInstance();
				CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
				POSITION Position=NULL;
				CGamePropertyItem * pGamePropertyItem=NULL;
				while (true)
				{
					pGamePropertyItem=pGamePropertyListData->EmunGamePropertyItem(Position);
					if (pGamePropertyItem==NULL) break;
					pGamePropertyManager->CreatePropertyItem(pGamePropertyItem->m_PropertyItem);
					if (Position==NULL) break;
				}			

			}
			return true;
		}
	case SUB_GR_CONFIG_USER_RIGHT:	//�������Ȩ��
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_ConfigUserRight));
			if (wDataSize<sizeof(CMD_GR_ConfigUserRight)) return false;

			//��Ϣ����
			CMD_GR_ConfigUserRight * pConfigUserRight=(CMD_GR_ConfigUserRight *)pData;

			//���Ȩ��
			m_dwUserRight=pConfigUserRight->dwUserRight;

			//����˽�Ĵ���
			for( INT_PTR i = 0; i < m_DlgWhisperItemArray.GetCount(); i++ )
			{
				CDlgWhisper *pdlg = m_DlgWhisperItemArray[i];
				if( pdlg ) pdlg->SetUserRight(m_dwUserRight);
			}

			//֪ͨ�ͻ���
			if(IsGameClientReady())
			{
				//���캯��
				IPC_GF_UserRight ur = {0};
				ur.dwUserRight = m_dwUserRight;

				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_USER_RIGHT,&ur,sizeof(ur) );
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//״̬����
bool CPlazaViewServer::OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//������Ϣ
		{
			//��������
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

			//Ч������
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==wDataSize);
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=wDataSize) return false;

			//��Ϣ����
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				BYTE cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
				BYTE cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
				m_pITableViewFrame->SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableInfo->TableStatusArray[i].lCellScore);
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//����״̬
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_TableStatus));
			if (wDataSize!=sizeof(CMD_GR_TableStatus)) return false;

			//��������
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;

			//��������
			BYTE cbTableLock=pTableStatus->TableStatus.cbTableLock;
			BYTE cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
			m_pITableViewFrame->SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableStatus->TableStatus.lCellScore);

			//��������
			if(cbPlayStatus==TRUE && m_pIMySelfUserItem->GetTableID()==pTableStatus->wTableID && CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
			{
				m_pITableViewFrame->SetTableStatus(false);
			}

			return true;
		}
	}

	return true;
}

//���д���
bool CPlazaViewServer::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//��Ϸ����
	BYTE *pActivityGame = (BYTE*)pData;
	if(*pActivityGame==TRUE)
	{
		return OnSocketMainGameFrame(MDM_GR_INSURE,wSubCmdID,pData,wDataSize);
	}

	//��Ϣ����
	ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}


bool CPlazaViewServer::OnSocketMainBaseEnsure( WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//��Ϣ����
	ASSERT(m_pDlgBaseEnsure!=NULL);
	if (m_pDlgBaseEnsure==NULL) return true;
	if (m_pDlgBaseEnsure->OnServerBaseEnsureMessage(MDM_GR_BASEENSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}


bool CPlazaViewServer::OnSocketMainMember( WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//��Ϣ����
	ASSERT(m_pDlgCheckIn!=NULL);
	if (m_pDlgCheckIn==NULL) return true;
	if (m_pDlgCheckIn->OnServerCheckInMessage(MDM_GR_MEMBER,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}
	return true;
}


bool CPlazaViewServer::OnSocketMainCheckIn( WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//��Ϣ����
	ASSERT(m_pDlgCheckIn!=NULL);
	if (m_pDlgCheckIn==NULL) return true;
	if (m_pDlgCheckIn->OnServerCheckInMessage(MDM_GR_CHECKIN,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//������
bool CPlazaViewServer::OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//������ʾ
	////��Ϣ����
	//if(wSubCmdID==SUB_GR_TASK_FINISH)
	//{
	//	//����У��
	//	ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskFinish));
	//	if(wDataSize>sizeof(CMD_GR_S_TaskFinish)) return true;

	//	//��ȡ����
	//	CMD_GR_S_TaskFinish * pTaskFinish = (CMD_GR_S_TaskFinish *)pData;

	//	//��ʾ��ʾ��
	//	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	//	pPlatformFrame->ShowTaskNotifyer(pTaskFinish->szTaskName);

	//	return true;
	//}
    
	//��Ϣ����
	/*ASSERT(m_pDlgTaskServerCenter!=NULL);*/
	if (m_pDlgTaskServerCenter==NULL) return true;
	if (m_pDlgTaskServerCenter->OnTaskMessage(MDM_GR_TASK,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//�һ�����
bool CPlazaViewServer::OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;
   
	//��Ϣ����
	ASSERT(m_pDlgExchangeServer!=NULL);
	if (m_pDlgExchangeServer==NULL) return true;
	if (m_pDlgExchangeServer->OnExchangeMessage(MDM_GR_EXCHANGE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//���ߴ���
bool CPlazaViewServer::OnSocketMainProperty(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{										
	case SUB_GR_QUERY_PROPERTY_RESULT:		//��ѯ����
	case SUB_GR_GAME_PROPERTY_BUY_RESULT:	//�������
	case SUB_GR_QUERY_PROPERTY_RESULT_FINISH:	//��ѯ����
	case SUB_GR_GAME_PROPERTY_FAILURE:
		{
			if(m_pDlgShopServer) return m_pDlgShopServer->OnServerShopMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
		}
	case SUB_GR_PROPERTY_USE_RESULT:		//ʹ�õ���
		{
			return OnSocketSubPropertyUse(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_BACKPACK_RESULT:	//��ұ���
		{
			return OnSocketSubUserBackpack(pData,wDataSize);
		}
	case SUB_GR_QUERY_SEND_PRESENT_RESULT:	//��ѯ����
		{
			return	OnSocketSubQuerySendPresent(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_PRESENT_RESULT:	//���͵���
		{
			return OnSocketSubPropertyPresent(pData,wDataSize);
		}
	case SUB_GR_GET_SEND_PRESENT_RESULT:	//��ȡ����
		{
			return OnSocketSubGetSendPresent(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_SUCCESS:			//���߳ɹ�
		{
			return OnSocketSubPropertySuccess(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_FAILURE:			//����ʧ��
		{
			return OnSocketSubPropertyFailure(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_EFFECT:			//����ЧӦ
		{
			return OnSocketSubPropertyEffect(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_MESSAGE:			//������Ϣ
		{
			return OnSocketPropertyMessage(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_TRUMPET:			//������Ϣ
		{
			return OnSocketPropertyTrumpet(pData,wDataSize);
		}
	case SUB_GR_USER_PROP_BUFF:				//����״̬
		{
			return OnSocketSubUserPropBuff(pData,wDataSize);
		}
	case SUB_GR_USER_TRUMPET:			//��������
		{
			return OnSocketSubUserTrumpet(pData,wDataSize);
		}
	}
	return true;
}

//������
bool CPlazaViewServer::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//��ǰ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//��Ϣ����
			if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerType(m_wServerType);
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			}

			return true;
		}
	}

	return true;
}

//ϵͳ����
bool CPlazaViewServer::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//������Ϣ
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CPlazaViewServer::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;
	if(!IsGameClientReady()) return true;

	//��������
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//��������
bool CPlazaViewServer::OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_MATCH_FEE:		//���ò�ѯ
		{
			//����Ч��
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Fee));
			if(wDataSize>sizeof(CMD_GR_Match_Fee)) return false;

			//��ȡ����
			CMD_GR_Match_Fee * pMatchFee = (CMD_GR_Match_Fee *)pData;
			if(pMatchFee->cbSignupMode==SIGNUP_MODE_SIGNUP_FEE)
			{
				//����۷�
				if (pMatchFee->cbDeductArea==DEDUCT_AREA_SERVER)
				{
					if (pMatchFee->lSignupFee>0)
					{
						INT nResult=ShowInformation(pMatchFee->szNotifyContent,MB_ICONINFORMATION|MB_YESNO,30L);
						if (nResult==IDYES) 
						{
							BYTE cbSignupMode=SIGNUP_MODE_SIGNUP_FEE;
							m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&cbSignupMode,sizeof(cbSignupMode));
						}
					}
					else if (pMatchFee->lSignupFee ==0)
					{
						BYTE cbSignupMode=SIGNUP_MODE_SIGNUP_FEE;
						m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&cbSignupMode,sizeof(cbSignupMode));
					}
				}

				//��ҳ�۷�
				if (pMatchFee->cbDeductArea==DEDUCT_AREA_WEBSITE)
				{
					//��������
					//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					////��������
					//TCHAR szSignupLink[MAX_PATH];
					//_sntprintf(szSignupLink,CountArray(szSignupLink),TEXT("http://web.fox2046.com:81/MatchCenter/MatchXZ.aspx?UserID=%d&Password=%s&ServerID=%d&MatchID=%d&MatchNo=%I64d"),pGlobalUserData->dwUserID,pGlobalUserData->szPassword,m_GameServer.wServerID,pMatchFee->dwMatchID,pMatchFee->lMatchNO);

					////��������
					//CDlgSignupBrowse SingupBrowse;
					//SingupBrowse.SetInitSize(CSize(720,568));
					//SingupBrowse.SetBrowseInfo(TEXT("��������"),szSignupLink);
					//SingupBrowse.DoModal();

					////�����ɹ�
					//if (SingupBrowse.GetSignupStatus()==1)
					//{
					//	BYTE cbSignupMode=SIGNUP_MODE_SIGNUP_FEE;
					//	m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&cbSignupMode,sizeof(cbSignupMode));
					//}
				}
			}

			return true;
		}
	case SUB_GR_MATCH_SAFECARD:		//���տ�
		{
			//����Ч��
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_SafeCard));
			if(wDataSize>sizeof(CMD_GR_Match_SafeCard)) return false;

			//��ȡ����
			CMD_GR_Match_SafeCard * pMatchSafeCard= (CMD_GR_Match_SafeCard *)pData;

			//���ѹ���
			INT nResult=ShowInformation(CW2CT(pMatchSafeCard->szNotifyContent),MB_ICONINFORMATION|MB_YESNO,30L);
			
			//����ṹ
			CMD_GR_Match_BuySafeCard MatchBuySafeCard;
			ZeroMemory(&MatchBuySafeCard,sizeof(MatchBuySafeCard));

			//���ñ�ʶ
			MatchBuySafeCard.bBuySafeCard=nResult==IDYES;

			//��������
			m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_BUY_SAFECARD,&MatchBuySafeCard,sizeof(MatchBuySafeCard));

			return true;
		}
	case SUB_GR_MATCH_REVIVE:		//��������
		{
			//����Ч��
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Revive));
			if(wDataSize>sizeof(CMD_GR_Match_Revive)) return false;

			//��ȡ����
			CMD_GR_Match_Revive * pMatchRevive= (CMD_GR_Match_Revive *)pData;

			//���ѹ���
			INT nResult=ShowInformation(CW2CT(pMatchRevive->szNotifyContent),MB_ICONINFORMATION|MB_YESNO,30L);
			if (nResult==IDYES) 
			{
				//����ṹ
				CMD_GR_Match_UserRevive MatchUserRevive;
				ZeroMemory(&MatchUserRevive,sizeof(MatchUserRevive));

				//���ñ���
				MatchUserRevive.bUseSafeCard=false;

				//��������
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_USER_REVIVE,&MatchUserRevive,sizeof(MatchUserRevive));
			}

			return true;
		}
	case SUB_GR_MATCH_GOLDUPDATE:	//��Ҹ���
		{
			//����У��
			ASSERT(sizeof(CMD_GR_MatchGoldUpdate)==wDataSize);
			if(sizeof(CMD_GR_MatchGoldUpdate)!=wDataSize) return false;

			//��ȡ����
			CMD_GR_MatchGoldUpdate * pMatchGoldUpdate=(CMD_GR_MatchGoldUpdate*)pData;

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//���ñ���
			pGlobalUserData->lUserScore=pMatchGoldUpdate->lCurrGold;

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

			return true;
		}
	case SUB_GR_MATCH_ELIMINATE:	//������̭
		{
			return true;
		}
	}

	return m_pITableViewFrame->OnEventViewData(wSubCmdID,pData,wDataSize);
}


//�û�����
bool CPlazaViewServer::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//��������
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//��Ϣ����
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	if(bMySelfUserItem) 		m_bPlaying = true;

	//��ȡ��Ϣ
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//�û�����
		UserInfo.wFaceID=pUserInfoHead->wFaceID;
		UserInfo.dwGameID=pUserInfoHead->dwGameID;
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
		UserInfo.dwCustomID=pUserInfoHead->dwCustomID;
		UserInfo.dwAgentID=pUserInfoHead->dwAgentID;

		//�û�״̬
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//�û�����
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//�û�����
		UserInfo.lScore=pUserInfoHead->lScore;
		UserInfo.lGrade=pUserInfoHead->lGrade;
		UserInfo.lInsure=pUserInfoHead->lInsure;
		UserInfo.lIngot=pUserInfoHead->lIngot;
		UserInfo.dBeans=pUserInfoHead->dBeans;
		UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
		UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
		UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
		UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;		
		UserInfo.dwExperience=pUserInfoHead->dwExperience;
		UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;
		UserInfo.lIntegralCount=pUserInfoHead->lIntegralCount;

		//��������
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//��չ��Ϣ
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//�û��ǳ�
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
					{
						CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
					}
					break;
				}
			case DTP_GR_GROUP_NAME:		//�û�����
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
					{
						CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
					}
					break;
				}
			case DTP_GR_UNDER_WRITE:	//����ǩ��
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
					{
						CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
					}
					break;
				}
			case DTP_GR_QQ:	//QQ����
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szQQ));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szQQ))
					{
						CopyMemory(UserInfo.szQQ,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szQQ[CountArray(UserInfo.szQQ)-1]=0;
					}
					break;
				}
			case DTP_GR_MOBILE_PHONE:	//�ֻ�����
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szMobilePhone));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szMobilePhone))
					{
						CopyMemory(UserInfo.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szMobilePhone[CountArray(UserInfo.szMobilePhone)-1]=0;
					}
					break;
				}
			}
		}

		//�Զ�ͷ��
		if (pUserInfoHead->dwCustomID!=0L)
		{
			//����ͷ��
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

			//����ͷ��
			if (bSuccess==false)
			{
				pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
			}
		}
	}
	else
	{
		//�û���Ϣ
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		lstrcpyn(UserInfo.szNickName,TEXT("��Ϸ���"),CountArray(UserInfo.szNickName));

		//�û�״̬
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//�û�����
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//�����û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo,pUserInfoHead->bIsAndroid);

	//��ȡ����
	CServerListData * pServerListData=CServerListData::GetInstance();

	//��������
	pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������ʾ
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
		if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
		{
			//��ȡָ��
			IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			ASSERT(pIStringMessage!=NULL);

			//��ʾ��Ϣ
			CString strDescribe;
			strDescribe.Format(TEXT("���ĺ��� [%s] �����ˣ�"),pIClientUserItem->GetNickName());
			pIStringMessage->InsertSystemString(strDescribe);
		}
	}

	return true;
}

//�û�����
bool CPlazaViewServer::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//Ѱ���û�
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(m_pIMySelfUserItem->GetMasterOrder()>0));

	//�����û�
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	}

	return true;
}

//�û�״̬
bool CPlazaViewServer::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//Ѱ���û�
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//����״̬
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//ɾ���û�
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);

		//��ȡ����
		CServerListData * pServerListData=CServerListData::GetInstance();

		//��������
		pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());
	}
	else
	{
		//�����û�
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//�û���Ϸ����
bool CPlazaViewServer::OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_UserGameData * pUserGameData=(CMD_GR_UserGameData *)pData;
	WORD wHeadSize=sizeof(CMD_GR_UserGameData)-sizeof(pUserGameData->szUserGameData);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData)))) return false;

	//Ѱ���û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserGameData->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	return true;
}

//��¼���
bool CPlazaViewServer::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GR_LogonFinish));
	if(wDataSize!=sizeof(CMD_GR_LogonFinish)) return false;

	//��ȡ����
	CMD_GR_LogonFinish * pLogonFinish = (CMD_GR_LogonFinish *)pData;

	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//����״̬
	SetServiceStatus(ServiceStatus_ServiceIng);

	//�����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	if (pPlatformFrame!=NULL) pPlatformFrame->ActiveServerViewItem();

	//�û�����
	CControlUI * pButtonControl = GetControlByName(szButtonTableButton4ControlName);
	if(pButtonControl!=NULL)
	{
		pButtonControl->SetEnabled(false);		
		if (CServerRule::IsForfendGameRule(m_dwServerRule)==false && m_wChairCount < MAX_CHAIR)
		{
			//���͹���
			SendUserRulePacket();

			//ʹ������
			pButtonControl->SetEnabled(true);		
		}
	}

	//�����ж�
	pButtonControl = GetControlByName(szButtonTableButton3ControlName);
	if(pButtonControl!=NULL) 
	{
		pButtonControl->SetEnabled(false);
		if (CServerRule::IsForfendLockTable(m_dwServerRule)==false && m_wChairCount < MAX_CHAIR &&
			CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==false)
		{
			 pButtonControl->SetEnabled(true);
		}
	}

	//���Ұ�ť
	pButtonControl = GetControlByName(szButtonTableButton2ControlName);
	if(pButtonControl!=NULL)
	{	
		pButtonControl->SetEnabled(true);
		if(CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true || 
			m_wServerType==GAME_GENRE_MATCH )
		{
			pButtonControl->SetEnabled(false);
		}
	}

	//���밴ť
	pButtonControl = GetControlByName(szButtonTableButton1ControlName);
	if(pButtonControl!=NULL)
	{	
		pButtonControl->SetEnabled(true);
		if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true ||
			m_wServerType==GAME_GENRE_MATCH )
		{
			pButtonControl->SetEnabled(false);
		}
	}

	//�����ж�
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//��������	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//������
		if (bSuccess==false)
		{
			//�ж�����
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;
		}		
	}

	////������ʾ
	////��������
	//if(pLogonFinish->bGuideTask==true)
	//{
	//	SetTimer(IDI_GUIDE_TASK,1000L+rand()%TIME_GUIDE_TASK,NULL);
	//}

	//׷�����
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	DWORD dwTrackUserID = pGlobalUnits->GetTrackUserID();
	if(pGlobalUnits!=NULL && dwTrackUserID!=0)
	{
		//�����û�
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwTrackUserID);
		if(pIClientUserItem && pIClientUserItem->GetTableID()!=INVALID_TABLE && pIClientUserItem->GetChairID()!=INVALID_CHAIR)
		{
			//��������
			tagSearchTable SearchTable;
			ZeroMemory(&SearchTable,sizeof(SearchTable));

			//��������
			SearchTable.bAllNull=true;
			SearchTable.bNotFull=true;
			SearchTable.bOneNull=true;
			SearchTable.bFilterPass=true;
			SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

			//�������
			SearchTable.wResultChairID=INVALID_CHAIR;
			SearchTable.wResultTableID=pIClientUserItem->GetTableID();			
			SearchTable.wStartTableID=SearchTable.wResultTableID;

			//��������
			SearchGameTable(SearchTable);
			if(SearchTable.wResultChairID!=INVALID_CHAIR && SearchTable.wResultTableID==SearchTable.wStartTableID  && CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==false)
			{
				//��������
				WORD wChairID=SearchTable.wResultChairID;
				m_pITableViewFrame->VisibleTable(SearchTable.wResultTableID);
				m_pITableViewFrame->FlashGameChair(SearchTable.wResultTableID,wChairID);

				//ִ������
				PerformSitDownAction(SearchTable.wResultTableID,wChairID,true);
			}
			else
			{
				//ִ���Թ�
				PerformLookonAction(SearchTable.wStartTableID,pIClientUserItem->GetChairID());
			}			

			return true;
		}
	}

	//��ѯ����
	CMD_GR_C_QuerySendPresent QuerySendPresent={0};
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	QuerySendPresent.dwUserID=pGlobalUserData->dwUserID;

	//��������

	m_TCPSocketModule->SendData(MDM_GR_PROPERTY, SUB_GR_QUERY_SEND_PRESENT, &QuerySendPresent, sizeof(QuerySendPresent));

	return true;
}

//��¼�ɹ�
bool CPlazaViewServer::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//��Ϣ����
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//�������
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//����״̬
	SetServiceStatus(ServiceStatus_RecvInfo);

	//�ر���ʾ
	//m_DlgUpdateStatus.StartEnterServer(/*TEXT("���ڶ�ȡ������Ϣ...")*/);

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameRoom.aspx?KindID=%ld&ServerID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wKindID,m_GameServer.wServerID);

	////���ӹ��
	//m_PlatformPublicize.Navigate(szBillUrl);
	//m_PlatformPublicize.SetBoradColor(RGB(0,0,0));
	ASSERT(m_ImageGameName.IsNull()==true);
	if (m_ImageGameName.IsNull()==false) m_ImageGameName.DestroyImage();

	//����·��
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��ȡ�ļ�
	TCHAR szGameName[64]=TEXT("");
	CString strProcessName(m_GameKind.szProcessName);
	INT nPos=strProcessName.Find(TEXT("."));
	_sntprintf(szGameName,CountArray(szGameName),TEXT("%s"),strProcessName.Left(nPos));

	//����·��
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\GameKindLogo\\Room_%s.png"),szDirectory,szGameName);

	//����ͼƬ
	m_ImageGameName.LoadImage(szFilePath);

	return true;
}

//��¼ʧ��
bool CPlazaViewServer::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//Ч�����
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//�رմ���
	m_TCPSocketModule->CloseSocket();

	//����״̬
	SetServiceStatus(ServiceStatus_NetworkDown);

	//������Ϣ
	if(pLogonFailure->lErrorCode == LOGON_FAIL_SERVER_INVALIDATION)
	{
		//��������
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		//if (pPlatformFrame!=NULL) pPlatformFrame->ListGameServerInvalidation(m_GameKind.wKindID, m_GameServer.wServerID);
	}

	//��ʾ��Ϣ
	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
	if (lstrlen(pszDescribeString)>0)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(pszDescribeString,MB_ICONERROR,60);
	}

	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//������ʾ
bool CPlazaViewServer::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//��������
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//��������
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//�رմ���
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//���ش���
		//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		//if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("��Ϸ�㳡"),0,0);

		//������ʾ
		TCHAR szDescribe[512]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�����Ѿ����£���Ҫ���´�����"));

		//��ʾ��Ϣ
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION,0);

		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//Ŀ��Ŀ¼
		TCHAR szTargetPath[MAX_PATH]=TEXT("");
		_sntprintf(szTargetPath,CountArray(szTargetPath),TEXT("%s\\Update.exe"),szDirectory);

		//��������
		ShellExecute(NULL,TEXT("open"),szTargetPath,TEXT(""),NULL,true);

		CPaintManagerUI::InvokAllCloseCallBack();

		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		pGlobalUserData->dwUserID = 0L;

		//�رմ���
		AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

		return true;
	}

	//��Ϸ����
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//�رմ���
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}



		////������ʾ
		//if (pUpdateNotify->cbMustUpdateClient==FALSE)
		//{
			//������ʾ
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"),m_GameKind.szKindName);

			//��ʾ��Ϣ
			CDlgInformation Information(this);
			INT nResult = Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0);
		//}

			if (nResult==IDYES)
			{
				//ֱ������
				CPlatformFrame::GetInstance()->DownLoadGame(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
				CPlatformFrame::GetInstance()->ReturnHallMain();
			}
			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	}

	return true;
}

//����ʧ��
bool CPlazaViewServer::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//Ч�����
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//��������
	IClientUserItem * pITableUserItem=NULL;
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wMeChairID=m_pIMySelfUserItem->GetChairID();

	//���滹ԭ
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_TABLE)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pITableUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//��ʾ��Ϣ
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//�û�����
bool CPlazaViewServer::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��ȡָ��
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//��ʾ��Ϣ
	if (pUserChat->dwTargetUserID!=0L)
	{
		//��ȡ�û�
		IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;

		//��ʾ��Ϣ
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}
	else 
	{
		//��ʾ��Ϣ
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}

	return true;
}

//�û�����
bool CPlazaViewServer::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	//��������
	CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��ȡָ��
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//������Ϣ
	if (pUserExpression->dwTargetUserID==0L)
	{
		//��������
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//�������
		if (pExpressionItem!=NULL)
		{
			//��ȡ·��
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//������Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			pIStringMessage->InsertExpression(pszSendUser,szImagePath);
		}
	}

	//˽����Ϣ
	if (pUserExpression->dwTargetUserID!=0L)
	{
		//��������
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//�������
		if (pExpressionItem!=NULL)
		{
			//��ȡ·��
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//��ȡ�û�
			IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);
			if (pIRecvUserItem==NULL) return true;

			//��ʾ��Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
			pIStringMessage->InsertExpression(pszSendUser,pszRecvUser,szImagePath);
		}
	}

	return true;
}

//�û�˽��
bool CPlazaViewServer::OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//�����û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//�����û�
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	//��������
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,pWisperChat->dwTargetUserID,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventWisperChat(pISendUserItem,pWisperChat->szChatString,pWisperChat->dwChatColor);
	}

	return true;
}

//˽�ı���
bool CPlazaViewServer::OnSocketSubWisperExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	//��������
	CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//�����û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//�����û�
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	//��������
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,pWisperExpression->dwTargetUserID,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventExpression(pISendUserItem,pWisperExpression->wItemIndex);
	}

	return true;
}

//���߳ɹ�
bool CPlazaViewServer::OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertySuccess));
	if (wDataSize!=sizeof(CMD_GR_S_PropertySuccess)) return false;

	//��������
	CMD_GR_S_PropertySuccess * pPropertySuccess=(CMD_GR_S_PropertySuccess *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//��ȡ�û�
	IClientUserItem * pTargetUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwTargetUserID);
	if (pTargetUserItem==NULL) return true;

	//ʹ�÷�Χ
	if(pPropertySuccess->wKind==1&& IsGameClientReady())
	{
		//���ñ���
		IPC_GR_PropertySuccess  PropertySuccess;  
		PropertySuccess.wPropertyIndex=pPropertySuccess->wPropertyIndex;
		PropertySuccess.wPropertyCount=pPropertySuccess->wItemCount;
		PropertySuccess.dwSourceUserID=pPropertySuccess->dwSourceUserID;
		PropertySuccess.dwTargetUserID=pPropertySuccess->dwTargetUserID;
		CopyMemory(PropertySuccess.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertySuccess.szSourceNickName));

		//��������
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));

		if(bSendSuccess==true) return true;
	}
	
	//��ȡ����
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//Ѱ�ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	//���ߴ���
	if (pPropertySuccess->wKind==1)
	{
		//ͼƬ��Ϣ
		tagPropertyImage PropertyImage={0};
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//���Ŷ���
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//����·��
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			try
			{
				//���Ŷ���
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("�����ļ�����ʧ��");
				}

				//�����ؼ�
				CRect rcWindow;
				m_pITableViewFrame->GetViewWnd()->GetWindowRect(&rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//���Ŷ���
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
		}
	}

	//������ʾ
	if(pPropertySuccess->wKind != 1) //�������͵ĵ��߲���ʾ
	{
		//��������
		TCHAR  szMessage[256]=TEXT("");
		
		//��ȡ��ʾ
		if(pPropertySuccess->dwSourceUserID==m_pIMySelfUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("��ϲ��,���߹���ɹ�!"));
		else
			pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),m_pIMySelfUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

		//������Ϣ
		CDlgInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

		//��������
		//if(m_pTrumpetItem!=NULL)  m_pTrumpetItem->UpdateControlSurface();
	}

	return true;
}

//����ʧ��
bool CPlazaViewServer::OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_PropertyFailure * pPropertyFailure=(CMD_GR_PropertyFailure *)pData;
	if(pPropertyFailure==NULL) return false;

	//ʹ�÷�Χ
	if(pPropertyFailure->wRequestArea==PT_ISSUE_AREA_GAME && IsGameClientReady())
	{
		//���ñ���
		IPC_GR_PropertyFailure  PropertyFailure;  
		PropertyFailure.lErrorCode=pPropertyFailure->lErrorCode;
		CopyMemory(PropertyFailure.szDescribeString,pPropertyFailure->szDescribeString,sizeof(PropertyFailure.szDescribeString));

		//��������
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_FAILURE,&PropertyFailure,sizeof(PropertyFailure));

		if(bSendSuccess==true) return true; 
	}

	//������Ϣ
	CDlgInformation Information;
	Information.ShowMessageBox(pPropertyFailure->szDescribeString,MB_ICONINFORMATION);

	return true;
}

//����ЧӦ
bool CPlazaViewServer::OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyEffect));
	if(wDataSize!=sizeof(CMD_GR_S_PropertyEffect))  return false;

	//��������
	CMD_GR_S_PropertyEffect * pPropertyEffect=(CMD_GR_S_PropertyEffect *)pData;
	if(pPropertyEffect==NULL) return false;

	//�����û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyEffect->wUserID);
	if(pISendUserItem==NULL) return true;

	//���ñ���
	tagUserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	if(pPropertyEffect->wPropKind == PROP_KIND_VIP)
	{
		pUserInfo->cbMemberOrder=pPropertyEffect->cbMemberOrder;
	}
	else if( pPropertyEffect->wPropKind == PROP_KIND_ESCAPE_CLEAR)
	{
		pUserInfo->dwFleeCount = pPropertyEffect->dwFleeCount;
	}
	else if( pPropertyEffect->wPropKind == PROP_KIND_NEGATIVE_SCORE_CLEAR)
	{
		pUserInfo->lScore = pPropertyEffect->lScore;
	}
	//������Ϣ
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pISendUserItem);
    
    return true;
}

//�������
bool CPlazaViewServer::OnSocketSubUserInvite(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserInvite));
	if (wDataSize!=sizeof(CMD_GR_UserInvite)) return false;

	//��Ϣ����
	CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)pData;
	ASSERT(pUserInvite->wTableID<m_pITableViewFrame->GetTableCount());
	if (m_pITableViewFrame->GetPlayFlag(pUserInvite->wTableID)==true) return true;

	//Ѱ���û�
	IClientUserItem * pIUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserInvite->dwUserID);
	if (pIUserItem==NULL) return true;
	if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�û�����
	if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE) return true;
	if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	//��ʾ��Ϣ
	TCHAR szMessage[256]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ����������� [ %ld ] ��Ϸ��������Ϸ���Ƿ�ͬ�⣿"),pIUserItem->GetNickName(),pUserInvite->wTableID+1);
	//��ʾ��Ϣ
	CDlgInformation Information(this);
	if (Information.ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO)==IDYES)
	{
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_pITableViewFrame->GetNullChairCount(pUserInvite->wTableID,wChairID);
		if (wNullCount==0) 
		{
			CDlgInformation Information2(this);
			Information2.ShowMessageBox(TEXT("����Ϸ���Ѿ�û�п�λ���ˣ�"),MB_ICONINFORMATION);
			return true;
		}
		PerformSitDownAction(pUserInvite->wTableID,wChairID,false);
	}

	return true;
}

//������Ϣ
bool CPlazaViewServer::OnSocketPropertyMessage(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyMessage));
	if (wDataSize!=sizeof(CMD_GR_S_PropertyMessage)) return false;

	//У������
	CMD_GR_S_PropertyMessage * pPropertyMessage = (CMD_GR_S_PropertyMessage *)pData;
	if(pPropertyMessage==NULL) return false;

    //��ȡ����
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//Ѱ�ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
	if(pGamePropertyItem==NULL) return true;

	//�����ж�
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY) return true;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//��ȡ�û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwTargerUserID);
	if (pIRecvUserItem==NULL) return true;

	//��ȡ��Ϣ
	TCHAR szNotifyMessage[128]=TEXT("");	
	pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),pPropertyMessage->wPropertyCount,szNotifyMessage);

	//��ȡָ��
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//������Ϣ
	pIStringMessage->InsertPromptString(szNotifyMessage);

	if(IsGameClientReady())
	{
		//����ṹ
		IPC_GR_PropertyMessage  PropertyMessage;
		PropertyMessage.wPropertyIndex=pPropertyMessage->wPropertyIndex;
		PropertyMessage.wPropertyCount=pPropertyMessage->wPropertyCount;
		CopyMemory(PropertyMessage.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertyMessage.szSourceNickName));
		CopyMemory(PropertyMessage.szTargerNickName,pIRecvUserItem->GetNickName(),sizeof(PropertyMessage.szTargerNickName));

		//��������
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	}

	return true;
}

//��������
bool CPlazaViewServer::OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_SendTrumpet));
	if (wDataSize!=sizeof(CMD_GR_S_SendTrumpet)) return false;

	//��������
	CMD_GR_S_SendTrumpet * pSendTrumpet=(CMD_GR_S_SendTrumpet *)pData;

	//��ȡָ��
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

    //������Ϣ
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
		pIStringMessage->InsertUserTrumpet(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//������Ϣ
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
        pIStringMessage->InsertUserTyphon(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//���°���
	if(pSendTrumpet->dwSendUserID==m_pIMySelfUserItem->GetUserID())
	{
		//���߰���
		tagPropertyPackage * pPropertyPackage=m_pIMySelfUserItem->GetPropertyPackage();

		//���°���
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET) --pPropertyPackage->wTrumpetCount;
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON) --pPropertyPackage->wTyphonCount;
		//if(m_pTrumpetItem!=NULL) m_pTrumpetItem->UpdateControlSurface();
	}

	if(IsGameClientReady())
	{
		//����ṹ
		IPC_GR_PropertyTrumpet  PropertyTrumpet;
		PropertyTrumpet.dwSendUserID=pSendTrumpet->dwSendUserID;
		PropertyTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
		PropertyTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
		CopyMemory(PropertyTrumpet.szSendNickName,pSendTrumpet->szSendNickName,sizeof(PropertyTrumpet.szSendNickName));
		CopyMemory(PropertyTrumpet.szTrumpetContent,pSendTrumpet->szTrumpetContent,sizeof(PropertyTrumpet.szTrumpetContent));

		//��������
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));
	}

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	if(pParameterGlobal->m_bAllowSound)
	{
		//��������
		HINSTANCE hInstance = AfxGetInstanceHandle();
		bool bPlaying=false;

		//��ȡ����
		CD3DSound * pD3DSound=CD3DSound::GetInstance();
		if(pD3DSound)
		{
			bPlaying = pD3DSound->PlaySound(hInstance, (pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"), TEXT("WAVE"), false);
		}

		//ϵͳ����
		if(!bPlaying)
		{
			HRSRC hResour=FindResource(hInstance,(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"),TEXT("WAVE"));
			if (hResour!=NULL)
			{
				HGLOBAL hGlobal=LoadResource(hInstance,hResour);
				if (hGlobal!=NULL) 
				{
					//��������
					LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
					BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

					//������Դ
					UnlockResource(hGlobal);
				}
				FreeResource(hGlobal);
			}
		}
	}

    return true;
}

//�ȴ�����
bool CPlazaViewServer::OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize)
{
	//��������
	m_pITableViewFrame->SetTableStatus(true);

	//��Ϸ֪ͨ
	OnSocketMainGameFrame(MDM_GF_FRAME,SUB_GR_USER_WAIT_DISTRIBUTE,pData,wDataSize);

	return true;
}

bool CPlazaViewServer::OnSocketSubUserPropBuff(VOID *pData, WORD wDataSize)
{
	CMD_GR_S_PropertyBuff* pPropertyBuff = (CMD_GR_S_PropertyBuff*)pData;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();

	TCHAR szString[128]={0};
	for(int i = 0; i < pPropertyBuff->cbBuffCount; i++)
	{
		//�����¼�
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_SHOW_UPDATE_BUFF, (LPARAM)&pPropertyBuff->PropertyBuff[i]);

		tString tBuffInfo;
		CStdString sTmpString;
		
		DWORD nTotalMins =(DWORD)((pPropertyBuff->PropertyBuff[i].tUseTime + pPropertyBuff->PropertyBuff[i].UseResultsValidTime)-time(NULL)) / 60;
		
		DWORD nDays = nTotalMins/(24*60);
		DWORD nHours = (nTotalMins-(nDays*24*60))/60;
		DWORD nMins = nTotalMins%60;

		if(nDays > 0)
		{
			sTmpString.Format(TEXT("%d��"), nDays);
			tBuffInfo += sTmpString.GetData();
		}
		if(nHours > 0)
		{
			sTmpString.Format(TEXT("%dСʱ"), nHours);
			tBuffInfo += sTmpString.GetData();
		}
		if(nMins > 0)
		{
			sTmpString.Format(TEXT("%d����"), nMins);
			tBuffInfo += sTmpString.GetData();
		}

		wsprintf(szString, TEXT("%s���[%s]����Ч������ʣ��%s"), pGlobalUserData->szNickName, pPropertyBuff->PropertyBuff[i].szName, tBuffInfo.c_str());
		//����ϵͳ��Ϣ
		if(pIStringMessage) pIStringMessage->InsertSystemString(szString);
	}
	
	return true;
}

bool CPlazaViewServer::OnSocketSubUserBackpack(VOID *pData, WORD wDataSize)
{
	//������Ϣ����
	if( m_pDlgBagServer != NULL )
	{
		return m_pDlgBagServer->OnServerBagMessage(MDM_GR_USER, SUB_GR_PROPERTY_BACKPACK_RESULT, pData, wDataSize);
	}
	
	//����˵�ʹ�õ���
	if( m_pDlgPropertyUse != NULL )
	{
		return m_pDlgPropertyUse->OnServerPropertyUseMessage(MDM_GR_USER, SUB_GR_PROPERTY_BACKPACK_RESULT, pData, wDataSize);
	}
	return false;
}

bool CPlazaViewServer::OnSocketSubPropertyUse(VOID *pData, WORD wDataSize)
{
	WORD wSubCmdID = SUB_GR_PROPERTY_USE_RESULT;

	//�̳�
	if(m_pDlgShopServer != NULL)
	{
		m_pDlgShopServer->OnServerShopMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
	}

	//������Ϣ
	if( m_pDlgBagServer != NULL )
	{
		m_pDlgBagServer->OnServerBagMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
	}
	
	//ʹ�õ���
	if( m_pDlgPropertyUse != NULL )
	{
		m_pDlgPropertyUse->OnServerPropertyUseMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
	}
	
	CMD_GR_S_PropertyUse* pPropertyUse = (CMD_GR_S_PropertyUse*)pData;
	if(pPropertyUse->dwUserID == m_pIMySelfUserItem->GetUserID() )
	{
		if(pPropertyUse->dwPropID == PROPERTY_ID_TRUMPET || pPropertyUse->dwPropID == PROPERTY_ID_TYPHON)
		{
			if(pPropertyUse->dwPropID == PROPERTY_ID_TRUMPET)
			{
				CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTrumpetCount(pPropertyUse->dwRemainderPropCount);
			}
			else if(pPropertyUse->dwPropID == PROPERTY_ID_TYPHON)
			{
				CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTyphonCount(pPropertyUse->dwRemainderPropCount);
			}

			if (IsGameClientReady())
			{
				//���ñ���
				IPC_GF_Trumpet_Number UserTrumpet;
				UserTrumpet.dwTrumpetCount = pPropertyUse->dwRemainderPropCount;
				UserTrumpet.dwTyphonCount = pPropertyUse->dwRemainderPropCount;

				//��������
				bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_USER_TRUMPET,&UserTrumpet,sizeof(UserTrumpet));

				if(bSendSuccess==true) return true;
			}

		}
		UINT uType =  (pPropertyUse->dwHandleCode==0)?(MB_OK|MB_ICONINFORMATION) : (MB_OK|MB_ICONSTOP);
		//��ʾ��Ϣ
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(pPropertyUse->szNotifyContent,uType,30);
		
	}

	return true;
}

bool CPlazaViewServer::OnSocketSubQuerySendPresent(VOID *pData, WORD wDataSize)
{
	CMD_GR_S_QuerySendPresent* pQuerySendPresentResult = (CMD_GR_S_QuerySendPresent*)pData;
	if(pQuerySendPresentResult != NULL )
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		pGlobalUserData->dwPresentCount = pQuerySendPresentResult->wPresentCount;

		if( pQuerySendPresentResult->wPresentCount > 0 )
		{
			vector<SendPresent> vecSendPresent;
			for(int i = 0; i < pQuerySendPresentResult->wPresentCount; i++)
			{
				vecSendPresent.push_back(pQuerySendPresentResult->Present[i]);
			}
			CDlgInformation Information;
			CStdString strNotifyContent;
			strNotifyContent.Format(TEXT("��������%d�����߸���"), vecSendPresent.size());
			Information.ShowMessageBox(strNotifyContent.GetData(),MB_ICONINFORMATION);

			CGlobalUnits::GetInstance()->QuerySendPresentMisson();
		}
	}
	return true;
}

bool CPlazaViewServer::OnSocketSubPropertyPresent(VOID *pData, WORD wDataSize)
{
	if( m_pDlgBagServer != NULL )
	{
		return m_pDlgBagServer->OnServerBagMessage(MDM_GR_PROPERTY, SUB_GR_PROPERTY_PRESENT_RESULT, pData, wDataSize);
	}
	return false;
}

bool CPlazaViewServer::OnSocketSubGetSendPresent(VOID *pData, WORD wDataSize)
{
	if( m_pDlgBagServer != NULL )
	{
		return m_pDlgBagServer->OnServerBagMessage(MDM_GR_PROPERTY, SUB_GR_GET_SEND_PRESENT_RESULT, pData, wDataSize);
	}
	return false;
}

bool CPlazaViewServer::OnSocketSubUserTrumpet(VOID *pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_GR_S_UserTrumpet));

	CMD_GR_S_UserTrumpet* pUserTrumpet = (CMD_GR_S_UserTrumpet*)pData;

	CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTrumpetCount(pUserTrumpet->dwTrumpetCount);
	CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTyphonCount(pUserTrumpet->dwTyphonCount);

	if (IsGameClientReady())
	{
		//���ñ���
		IPC_GF_Trumpet_Number UserTrumpet;
		UserTrumpet.dwTrumpetCount = pUserTrumpet->dwTrumpetCount;
		UserTrumpet.dwTyphonCount = pUserTrumpet->dwTyphonCount;

		//��������
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_USER_TRUMPET,&UserTrumpet,sizeof(UserTrumpet));

		if(bSendSuccess==true) return true;
	}

	return true;
}

//ϵͳ��Ϣ
bool CPlazaViewServer::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	WORD wType = pSystemMessage->wType;

	//�رմ���
	if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//��ȡָ��
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//��ʾ��Ϣ
	if (wType&SMT_CHAT) 
	{
		pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//�ر�����
	if (wType&SMT_CLOSE_INSURE)
	{
		if(m_pDlgInsureServer != NULL) 
		{
			m_pDlgInsureServer->EndDialog(IDOK);
			m_pDlgInsureServer->DestroyWindow();
		}
	}

	//�ر���Ϸ
	if (wType&SMT_CLOSE_GAME)
	{
		m_ProcessManager.CloseGameProcess(false);
	}

	//������Ϣ
	if (wType&SMT_EJECT)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//�رշ���
	if (wType&SMT_CLOSE_ROOM)
	{
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	}

	return true;
}

//������Ϣ
bool CPlazaViewServer::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//�رմ���
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//������Ϣ
	CDlgInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//��������
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//��ȡ����
	while (wExcursion<wDataSize)
	{
		//��������
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//Ч�����
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//Ч�����
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//��������
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//�������
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E ���
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//�������
					if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//���ض���
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E ����
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//����ģ��
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//����ƫ��
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//�رշ���
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//�����ؼ�
VOID CPlazaViewServer::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//������Ϣ
	tagEncircleInfo EncircleInfoTable;
	m_TableEncircle.GetEncircleInfo(EncircleInfoTable);

	//��������
	CRect rcTableArea(0,0,nWidth,nHeight);

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//���ƿؼ�
	m_TableEncircle.DeferWindowPos(m_pITableViewFrame->GetViewWnd(),hDwp,rcTableArea);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	//�����ؼ�
	CRect rcWindow;
	m_pITableViewFrame->GetViewWnd()->GetWindowRect(&rcWindow);
	m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);
	


	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//���÷���
VOID CPlazaViewServer::InitServerViewItem(CGameServerItem * pGameServerItem,WORD wAVServerPort,DWORD dwAVServerAddr)
{
	//����״̬
	SetServiceStatus(ServiceStatus_Entering);

	//��������
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	CopyMemory(&m_GameKind,&pGameKindItem->m_GameKind,sizeof(m_GameKind));
	CopyMemory(&m_GameServer,&pGameServerItem->m_GameServer,sizeof(m_GameServer));
	m_wAVServerPort=wAVServerPort;
	m_dwAVServerAddr=dwAVServerAddr;
	m_szServerPassword[0]=0;

	//�ر��ж�
	ASSERT(m_GameServer.wServerID!=0);
	if (m_GameServer.wServerID==0) throw TEXT("�ܱ�Ǹ������Ϸ�����Ѿ��ر��ˣ�������������룡");

	//������֤
	if((m_GameServer.wServerKind&SERVER_GENRE_PASSWD)!=0)
	{
		//��������
		CDlgInputPassword DlgInputPasswd;
		if(DlgInputPasswd.DoModal()==TRUE)
		{
			DlgInputPasswd.GetServerPasswd(m_szServerPassword);
		}
		else
		{
			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

			return;
		}		
	}

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�������
	if(CreateViewFrame()==false) throw TEXT("������ͼ��ܴ���ʧ�ܣ�");

	//�������
	if (m_TCPSocketModule.CreateInstance()==false) throw TEXT("�޷��������������������Ϸ�������ʧ�ܣ�");

	//��������
	m_pParameterGame=pParameterGlobal->GetParameterGame(&m_GameKind);
	m_pParameterServer=pParameterGlobal->GetParameterServer(&m_GameServer);

	//��������
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//��������
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("��Ϸ��������ʧ�ܣ�����ϵͳ�������ò����Ƿ���ȷ��");
		return;
	}

	//������ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("���ڽ�����Ϸ���䣬���Ժ�..."));

	//�������
	TCHAR szServerTitle[128]=TEXT("");
	_sntprintf(szServerTitle,CountArray(szServerTitle),TEXT("��Ϸ���� > %s > %s"),m_GameKind.szKindName,m_GameServer.szServerName);

	//���ñ���
	SetWindowText(szServerTitle);

	return;
}

//���ý���
VOID CPlazaViewServer::InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager)
{
	return;
}

//˽�ĻỰ
VOID CPlazaViewServer::WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng)
	{
		return;
	}

	//�û�����
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//��������
	bool bSameTable=false;
	bool bMasterUser=(cbMasterOrder>0);

	//�����ж�
	if (bMasterUser==false)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetMasterOrder()>0)
			{
				bMasterUser=true;
				break;
			}
		}
	}

	//ͬ���ж�
	if (wTableID!=INVALID_TABLE)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetTableID()==wTableID)
			{
				bSameTable=true;
				break;
			}
		}
	}

	//Ȩ���ж�
	if (CUserRight::CanWisper(m_dwUserRight)==false)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�ܱ�Ǹ����û�з���˽����Ϣ��Ȩ�ޣ�"),MB_ICONQUESTION);
		return;
	}

	//˽��Ȩ��
	if (CServerRule::IsForfendWisperChat(m_dwServerRule)&&(bMasterUser==false))
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�ܱ�Ǹ������Ϸ���䲻�������˽�ģ�"),MB_ICONQUESTION);
		return;
	}

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CWHArray<DWORD> CUserIDArray;
	CUserIDArray.Add(pGlobalUserData->dwUserID);

	//�����û�
	for (WORD i=0;i<wUserCount;i++)
	{
		//�û�����
		if (pIClientUserItem[i]->GetUserID()==0L) continue;
		if (pIClientUserItem[i]==m_pIMySelfUserItem) continue;

		//��������
		CUserIDArray.Add(pIClientUserItem[i]->GetUserID());
	}

	//����˽��
	WORD wUserIDArray=(WORD)CUserIDArray.GetCount();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,m_pIMySelfUserItem->GetUserID(),CUserIDArray.GetData(),wUserIDArray,true);

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		//��ʾ����
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//�ö�����
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();

		//ɾ������
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		if (pPlatformFrame!=NULL) pPlatformFrame->RemoveWhisperItem(pDlgWhisper);
	}

	return;
}

//����˽��
CDlgWhisper * CPlazaViewServer::CreateWhisperWnd(DWORD dwConversationID,DWORD dwSendUserID,DWORD dwUserID[], WORD wUserCount, bool bCreate)
{
	//Ѱ���ִ�
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->CompareUserItem(dwUserID,wUserCount)==false)) continue;

		return pDlgWhisper;
	}

	//�����ж�
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->DisuseEstimate()==false)) continue;

		//��������
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//�������
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//��������
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();
		pDlgWhisper->CreateWhisperWnd(dwConversationID,dwSendUserID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	//��������
	if (bCreate==true)
	{
		//��������
		CDlgWhisper * pDlgWhisper=NULL;
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//Ѱ�ҹر�
		for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
		{
			//��ȡ����
			CDlgWhisper * pDlgWhisperTemp=m_DlgWhisperItemArray[i];
			if (pDlgWhisperTemp->GetSafeHwnd()==NULL) 
			{
				pDlgWhisper=pDlgWhisperTemp;
				break;
			}
		}

		//��������
		if (pDlgWhisper==NULL)
		{
			try
			{
				pDlgWhisper=new CDlgWhisper;
				if (pDlgWhisper==NULL) return NULL;
				m_DlgWhisperItemArray.Add(pDlgWhisper);
			}
			catch (...) { return NULL; }
		}

		//�������
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//���ýӿ�
		m_ChatHistoryManager.SetMyselftUserItem(m_pIMySelfUserItem);

		//�û���Ŀ
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();

		//���ô���
		pDlgWhisper->SetServerViewItem(this);
		pDlgWhisper->SetUserRight(m_dwUserRight);
		pDlgWhisper->SetMasterRight(m_dwMasterRight);
		pDlgWhisper->SetMySelfUserItem(m_pIMySelfUserItem);
		pDlgWhisper->SetChatHistoryManager(QUERY_OBJECT_INTERFACE(m_ChatHistoryManager,IChatHistoryManager));
		pDlgWhisper->SetTCPSocket(m_TCPSocketModule.GetInterface());
		pDlgWhisper->SetGameLevelParser(m_GameLevelParserModule.GetInterface());
		pDlgWhisper->SetUserOrderParser(theApp.m_UserOrderParserModule.GetInterface());
		pDlgWhisper->SetPlazaUserManager(m_PlazaUserManagerModule.GetInterface());

		//��������
		pDlgWhisper->CreateWhisperWnd(dwConversationID,dwSendUserID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	return NULL;
}

//���͵�¼
bool CPlazaViewServer::SendLogonPacket(LPCTSTR pszServerPasswd)
{
	//��������
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//��������
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��Ϸ�汾
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//������Ϣ
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//��¼��Ϣ
	LogonUserID.wKindID=m_GameServer.wKindID;
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;	
	lstrcpyn(LogonUserID.szServerPasswd,pszServerPasswd,CountArray(LogonUserID.szServerPasswd));
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szDynamicPass,CountArray(LogonUserID.szPassword));	

	//������ʶ
	CWHService::GetMachineIDEx(LogonUserID.szMachineID);

	//��������
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//��������
bool CPlazaViewServer::SendUserRulePacket()
{
	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GR_UserRule),sizeof(cbBuffer)-sizeof(CMD_GR_UserRule));

	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	pUserRule->cbRuleMask=0x00;

	//���Ƶ�ַ
	if (pParameterGlobal->m_bLimitSameIP==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_SAME_IP;
	}

	//����ʤ��
	if (m_pParameterGame->m_bLimitWinRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_WIN_RATE;
		pUserRule->wMinWinRate=m_pParameterGame->m_wMinWinRate;
	}

	//��������
	if (m_pParameterGame->m_bLimitFleeRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_FLEE_RATE;
		pUserRule->wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
	}

	//���ƻ���
	if (m_pParameterGame->m_bLimitGameScore==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_GAME_SCORE;
		pUserRule->lMaxGameScore=(LONG)m_pParameterGame->m_lMaxGameScore;
		pUserRule->lMinGameScore=(LONG)m_pParameterGame->m_lMinGameScore;
	}

	//Я������
	if ((CServerRule::IsForfendLockTable(m_dwServerRule)==false)&&(m_pParameterServer->m_bTakePassword==true))
	{
		SendPacket.AddPacket(m_pParameterServer->m_szPassword,DTP_GR_TABLE_PASSWORD);
	}

	//��������
	WORD wSendSize=sizeof(CMD_GR_UserRule)+SendPacket.GetDataSize();
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_RULE,cbBuffer,wSendSize);

	return true;
}

//�����Թ�
bool CPlazaViewServer::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//��������
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//��������
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//��������
bool CPlazaViewServer::SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword)
{
	//��������
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//��������
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;
	if ((lpszPassword!=NULL)&&(lpszPassword[0]!=0))
	{
		lstrcpyn(UserSitDown.szPassword,lpszPassword,CountArray(UserSitDown.szPassword));
	}
	//��������ʱ��
	if((m_wServerType&GAME_GENRE_MATCH)!=0&&m_ProcessManager.IsProcessReady())
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("���ȹر���Ϸ�ͻ��ˣ�"));
		return true;
	}
	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//��������
bool CPlazaViewServer::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//��������
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//��������
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//��������
bool CPlazaViewServer::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//������Ϣ
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//��������
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//��������
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//���ͱ���
bool CPlazaViewServer::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//��������
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//������Ϣ
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//ִ���Թ�
bool CPlazaViewServer::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanLookon(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû���Թ���Ϸ��Ȩ�ޣ�"));

		return false;
	}

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ñ���
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//���ý���
	m_pITableViewFrame->VisibleTable(wTableID);

	//��������
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//ִ������
bool CPlazaViewServer::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//���ý���
	m_pITableViewFrame->VisibleTable(wTableID);

	//��������
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//ִ������
bool CPlazaViewServer::PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass)
{
	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû�м�����Ϸ��Ȩ�ޣ�"));

		return false;
	}

	//����Ч��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		CString strDescribe;
		if ((wChairID!=INDEX_ENTER_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,strDescribe,MB_ICONINFORMATION,30L);

			return false;
		}
	}

	//�����ж�
	TCHAR szPassword[LEN_PASSWORD] = {0};
	if ((m_pIMySelfUserItem->GetMasterOrder()==0)&&(bEfficacyPass==true)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//��������
		bool bLocker=m_pITableViewFrame->GetLockerFlag(wTableID);

		//�����ж�
		if(bLocker)
		{
			//��������
			CDlgTablePassword DlgTablePassword;
			if (DlgTablePassword.DoModal()!=IDOK) return false;

			//��������
			lstrcpyn(szPassword,DlgTablePassword.m_szPassword,CountArray(szPassword));
		}
	}

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ý���
	if ((wChairID!=INDEX_ENTER_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		m_pITableViewFrame->VisibleTable(wTableID);
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,m_pIMySelfUserItem);
	}

	//���ñ���
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//��������
	SendSitDownPacket(wTableID,wChairID,szPassword);

	return true;
}

bool CPlazaViewServer::PerformBuyProperty(BYTE cbRequestArea,LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//��������
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//���ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);
	if(pGamePropertyItem==NULL) return false;

	//�����û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByNickName(pszNickName);

	//�û��ж�
	if (pIClientUserItem==NULL)
	{
		return false;
	}

	////��������
	//CMD_GR_C_PropertyBuy PropertyBuy;
	//ZeroMemory(&PropertyBuy,sizeof(PropertyBuy));

	////���ñ���
	//PropertyBuy.cbRequestArea=cbRequestArea;
	//PropertyBuy.wItemCount=wItemCount;
	//PropertyBuy.wPropertyIndex=wPropertyIndex;
	//PropertyBuy.dwTargetUserID=pIClientUserItem->GetUserID();
	//PropertyBuy.cbConsumeScore=FALSE;

	//������Ϣ
	//m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_PROPERTY_BUY,&PropertyBuy,sizeof(PropertyBuy));

	return true;
}

//����״̬
VOID CPlazaViewServer::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//���ñ���
	m_ServiceStatus=ServiceStatus;

	return;
}

//��ԴĿ¼
VOID CPlazaViewServer::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;

	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}

//���밴ť
VOID CPlazaViewServer::OnTableButton1Clicked()
{
	//��������
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)) return;

	//״̬�ж�
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),MB_ICONINFORMATION,30L);
		return;
	}

	//�����λ
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//ִ�ж���
	switch (pParameterGlobal->m_cbActionHitAutoJoin)
	{
	case ACTION_SEARCH_TABLE:		//����λ��
		{
			//��������
			tagSearchTable SearchTable;
			ZeroMemory(&SearchTable,sizeof(SearchTable));

			//��������
			SearchTable.bNotFull=true;
			SearchTable.bOneNull=true;
			SearchTable.bAllNull=false;
			SearchTable.bFilterPass=true;
			SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

			//�������
			SearchTable.wResultTableID=INVALID_TABLE;
			SearchTable.wResultChairID=INVALID_CHAIR;
			SearchTable.wStartTableID=m_wFindTableID+1;

			//��������
			SearchGameTable(SearchTable);
			m_wFindTableID=SearchTable.wResultTableID;

			//��������
			if (m_wFindTableID==INVALID_CHAIR)
			{
				//��������
				SearchTable.bAllNull=true;
				SearchTable.bNotFull=true;
				SearchTable.bOneNull=true;
				SearchTable.bFilterPass=true;
				SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

				//�������
				SearchTable.wResultTableID=INVALID_TABLE;
				SearchTable.wResultChairID=INVALID_CHAIR;
				SearchTable.wStartTableID=m_wFindTableID+1;

				//��������
				SearchGameTable(SearchTable);
				m_wFindTableID=SearchTable.wResultTableID;
			}

			//����ж�
			if (m_wFindTableID!=INVALID_CHAIR)
			{
				//Ч������
				ASSERT(SearchTable.wResultTableID!=INVALID_TABLE);
				ASSERT(SearchTable.wResultChairID!=INVALID_CHAIR);

				//��������
				WORD wChairID=SearchTable.wResultChairID;
				m_pITableViewFrame->VisibleTable(m_wFindTableID);
				m_pITableViewFrame->FlashGameChair(m_wFindTableID,wChairID);

				//�Զ�����
				CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
				if (pParameterGlobal->m_bAutoSitDown==true) PerformSitDownAction(m_wFindTableID,wChairID,true);
			}
			else
			{
				//��ʾ��Ϣ
				CDlgInformation Information(this);
				Information.ShowMessageBox(TEXT("��Ǹ��������ʱû�п��Լ������Ϸ�������Ժ��ٴγ��ԣ�"),MB_ICONINFORMATION,30L);
			}

			break;
		}
	case ACTION_SHOW_SEARCH_DLG:	//��������
		{
			//��������
			PostMessage(WM_COMMAND,IDC_BT_TABLE_BUTTON_2,0L);

			break;
		}
	}

	return;
}

//���Ұ�ť
VOID CPlazaViewServer::OnTableButton2Clicked()
{
	//��������
	if (m_pDlgSearchTable==NULL)
	{
		//��������
		try
		{
			m_pDlgSearchTable=new CDlgSearchTable;
			if (m_pDlgSearchTable==NULL) return;
		}
		catch (...)
		{
			ASSERT(FALSE);
			return;
		}

		//���ö���
		m_pDlgSearchTable->SetServerViewItem(this);
		m_pDlgSearchTable->SetTableViewFrame(m_pITableViewFrame);
	}

	//��������
	if (m_pDlgSearchTable->m_hWnd==NULL)
	{
		m_pDlgSearchTable->Create(IDD_DLG_POPUP,this);
	}

	//��ʾ����
	m_pDlgSearchTable->ShowWindow(SW_SHOW);
	m_pDlgSearchTable->SetForegroundWindow();

	return;
}

//���ܰ�ť
VOID CPlazaViewServer::OnTableButton3Clicked()
{
	////��������
	//CDlgTablePassword DlgTablePassword;

	////������ʾ
	//if(m_pParameterServer->m_bTakePassword)
	//{
	//	lstrcpyn(DlgTablePassword.m_szPassword,m_pParameterServer->m_szPassword,CountArray(DlgTablePassword.m_szPassword));
	//	DlgTablePassword.SetPromptString(TEXT("���������ý������룬��Ҫ���ģ����������룡��Ҫ��������������롣"));
	//}
	//else
	//{
	//	DlgTablePassword.SetPromptString(TEXT("���������ӽ������롣"));
	//}

	////��ʾ����
	//if (DlgTablePassword.DoModal()==IDOK)
	//{
	//	CString strTablePass = DlgTablePassword.m_szPassword;
	//	if(strTablePass.GetLength() > 0)
	//	{
	//		//��������
	//		m_pParameterServer->m_bTakePassword=true;
	//		lstrcpyn(m_pParameterServer->m_szPassword,DlgTablePassword.m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	//	}
	//	else
	//	{
	//		//��������
	//		m_pParameterServer->m_bTakePassword=false;
	//		ZeroMemory(m_pParameterServer->m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	//	}

	//	//���͹���
	//	SendUserRulePacket();
	//}

	return;
}

//���ð�ť
VOID CPlazaViewServer::OnTableButton4Clicked()
{
	//��������
	CDlgCollocateRoom DlgCollocateRoom;
	DlgCollocateRoom.InitCollocate(m_pParameterGame, m_pParameterServer);

	//��ʾ����
	if (DlgCollocateRoom.DoModal()==IDOK)
	{
		//���͹���
		SendUserRulePacket();
	}

	return;
}

//���ఴť
VOID CPlazaViewServer::OnTableButton5Clicked()
{
	//����˵�
	CSkinMenu FunctionMenu;
	FunctionMenu.CreateMenu();

	//����˵�
	FunctionMenu.AppendMenu(IDM_GAME_RULE,TEXT("�������"));
	FunctionMenu.AppendMenu(IDM_GAME_CHART,TEXT("��Ϸ���а�"));

	//����˵�
	if (m_pIMySelfUserItem->GetMasterOrder()>0)
	{
		//����ָ�
		FunctionMenu.AppendSeparator();

		//����˵�
		FunctionMenu.AppendMenu(IDM_MANAGER_USER,TEXT("��Ϸ��ҹ���..."),MF_DISABLED);
		FunctionMenu.AppendMenu(IDM_MANAGER_SERVER,TEXT("��Ϸ�������..."),MF_DISABLED);
		FunctionMenu.AppendMenu(IDM_MANAGER_MESSAGE,TEXT("ϵͳ��Ϣ����..."),MF_DISABLED);

		//���ò˵�
		if (CMasterRight::CanKillUser(m_dwMasterRight)==true)
		{
			FunctionMenu.EnableMenuItem(IDM_MANAGER_USER,MF_ENABLED);
		}

		//���ò˵�
		if (CMasterRight::CanManagerServer(m_dwMasterRight)==true)
		{
			FunctionMenu.EnableMenuItem(IDM_MANAGER_SERVER,MF_ENABLED);
		}

		//���ò˵�
		if (CMasterRight::CanIssueMessage(m_dwMasterRight)==true)
		{
			FunctionMenu.EnableMenuItem(IDM_MANAGER_MESSAGE,MF_ENABLED);
		}
	}

	//�����˵�	
	CControlUI * pButtonControl = GetControlByName(szButtonTableButton5ControlName);
	if(pButtonControl!=NULL)
	{
		CRect rcButton=pButtonControl->GetPos();
		ClientToScreen(&rcButton);
		FunctionMenu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);
	}

	return;
}

//������ͼ���
bool CPlazaViewServer::CreateViewFrame()
{
	try
	{
		//��������
		if(m_GameServer.wServerType&GAME_GENRE_MATCH)
		{
			CMatchViewFrame * pMatchViewFrame=new CMatchViewFrame;
			if(pMatchViewFrame!=NULL)
			{
				m_pITableViewFrame = QUERY_OBJECT_PTR_INTERFACE(pMatchViewFrame,ITableViewFrame);
			}
		}
		else
		{
			CTableViewFrame * pTableViewFrame=new CTableViewFrame;
			if(pTableViewFrame!=NULL)
			{
				m_pITableViewFrame = QUERY_OBJECT_PTR_INTERFACE(pTableViewFrame,ITableViewFrame);
			}
		}

		//��������
		m_pITableViewFrame->CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));

		return true;
	}
	catch(...)
	{
		ASSERT(FALSE);
	}

	return false;
}

//����Ч��
bool CPlazaViewServer::EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe)
{
	//Ч�����
	ASSERT(wTableID<m_pITableViewFrame->GetTableCount());
	ASSERT(wChairID<m_pITableViewFrame->GetChairCount());

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_pITableViewFrame->GetTableCount()) return false;
	if (wChairID>=m_pITableViewFrame->GetChairCount()) return false;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(wTableID);
	IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//��������
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=CServerRule::IsAllowDynamicJoin(m_dwServerRule);

	//��Ϸ״̬
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		strDescribe=TEXT("����Ϸ���Ѿ���ʼ��Ϸ�ˣ���ʱ���ܼ��룡");
		return false;
	}

	//�����ж�
	if ((bReqLookon==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0))
	{
		//�����ж�
		for (WORD i=0;i<m_pITableViewFrame->GetChairCount();i++)
		{
			//��ȡ�û�
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==NULL)||(pITableUserItem==m_pIMySelfUserItem)) continue;

			//������
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//������ʾ
					LPCTSTR pszNickName=pITableUserItem->GetNickName();
					strDescribe.Format(TEXT("�������˲��벻�ܻ�ӭ�������Ϸ��������������ӭ����� [ %s ] ��"),pszNickName);

					return false;
				}
			}

			//ʤ��Ч��
			if (m_pParameterGame->m_bLimitWinRate==true)
			{
				WORD wWinRate = (WORD)(pITableUserItem->GetUserWinRate()*100L);
				if (wWinRate < m_pParameterGame->m_wMinWinRate)
				{
					strDescribe.Format(TEXT("[ %s ] ��ʤ��̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//����Ч��
			if (m_pParameterGame->m_bLimitFleeRate)
			{
				WORD wFleeRate = (WORD)(pITableUserItem->GetUserFleeRate()*100L);
				if (wFleeRate > m_pParameterGame->m_wMaxFleeRate)
				{
					strDescribe.Format(TEXT("[ %s ] ������̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//����Ч��
			if (m_pParameterGame->m_bLimitGameScore)
			{
				//��߻���
				if (pITableUserItem->GetUserScore()>m_pParameterGame->m_lMaxGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}

				//��ͻ���
				if (pITableUserItem->GetUserScore()<m_pParameterGame->m_lMinGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}

//�������
VOID CPlazaViewServer::OnManageServer()
{
	//Ȩ���ж�
	if (CMasterRight::CanManagerServer(m_dwMasterRight)==false) return;

	//��������
	CDlgServerManager DlgServerManager;
	DlgServerManager.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//���ñ���
	m_pDlgServerManager=&DlgServerManager;

	//��ʾ����
	DlgServerManager.DoModal();

	//���ñ���
	m_pDlgServerManager=NULL;

	return;
}

//�û�����
VOID CPlazaViewServer::OnManageUser()
{
	//Ȩ���ж�
	if (CMasterRight::CanKillUser(m_dwMasterRight)==false) return;

	//��ʾ����
	if (m_pDlgManageUser==NULL) 
	{
		m_pDlgManageUser=new CDlgManageUser;
	}

	if( m_pDlgManageUser )
		m_pDlgManageUser->SetMasterRight(m_dwMasterRight);

	//���ڴ���
	if (m_pDlgManageUser->m_hWnd==NULL)
	{
		m_pDlgManageUser->Create(IDD_DLG_MANAGE_USER,this);
		m_pDlgManageUser->SetTCPSocket(QUERY_OBJECT_PTR_INTERFACE(m_TCPSocketModule.GetInterface(),IUnknownEx));
	}

	//��ʾ����
	m_pDlgManageUser->ShowWindow(SW_SHOW);
	m_pDlgManageUser->BringWindowToTop();

	return;
}

//ϵͳ��Ϣ����
VOID CPlazaViewServer::OnManageSystemMsg()
{
	//Ȩ���ж�
	if (CMasterRight::CanIssueMessage(m_dwMasterRight)==false) return;

	CDlgIssueMessage DlgIssueMess;
	if(IDOK == DlgIssueMess.DoModal())
	{
		//������Ϣ
		CMD_GR_SendMessage SendMessage = {};

		if( DlgIssueMess.m_bSendALLRoom )
			SendMessage.cbAllRoom = TRUE;
		if( DlgIssueMess.m_bSendRoom )
			SendMessage.cbRoom = TRUE;
		if( DlgIssueMess.m_bSendGame )
			SendMessage.cbGame = TRUE;
		if( DlgIssueMess.m_bLoop )
		{
			SendMessage.cbLoop = TRUE;
			SendMessage.dwTimeRate = DlgIssueMess.m_dwTimeRate;
			SendMessage.tConcludeTime = DlgIssueMess.tConcludeTime;
		}
		lstrcpyn(SendMessage.szSystemMessage,DlgIssueMess.m_strMessage,CountArray(SendMessage.szSystemMessage));
		SendMessage.wChatLength = lstrlen(SendMessage.szSystemMessage)+1;

		WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
		m_TCPSocketModule->SendData(MDM_GR_MANAGE,SUB_GR_SEND_MESSAGE,&SendMessage,wSendSize);
	}

	return;
}

//������Ϣ
VOID CPlazaViewServer::OnNcDestroy()
{
	__super::OnNcDestroy();

	//�������
	m_bCreateFlag=false;

	//�û�����
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//��������
	m_wServerType=0L;
	m_dwServerRule=0L;

	//������Ϣ
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//���ٿؼ�
	m_ToolTipCtrl.DestroyWindow();	

	//���ٴ���
	if(m_pITableViewFrame!=NULL) 
	{
		m_pITableViewFrame->GetViewWnd()->DestroyWindow();
	}
    


	//�����Ի���
	if(m_pDlgInsureServer!=NULL) m_pDlgInsureServer->EndDialog(IDOK);
	if(m_pDlgTaskServerCenter!=NULL) m_pDlgTaskServerCenter->EndDialog(IDOK);
	if(m_pDlgCheckIn!=NULL) m_pDlgCheckIn->EndDialog(IDOK);	
	if(m_pDlgBaseEnsure!=NULL) m_pDlgBaseEnsure->EndDialog(IDOK);
	if(m_pDlgBagServer!=NULL) m_pDlgBagServer->EndDialog();
	if(m_pDlgShopServer!=NULL) m_pDlgShopServer->EndDialog();

	//���ٴ���
	if(m_pDlgSearchTable && IsWindow(m_pDlgSearchTable->m_hWnd)) m_pDlgSearchTable->DestroyWindow();
	if(m_pDlgManageUser && IsWindow(m_pDlgManageUser->m_hWnd)) m_pDlgManageUser->DestroyWindow();

	//ɾ������
	SafeDelete(m_pDlgSearchTable);
	SafeDelete(m_pDlgManageUser);

	//�ͷŽӿ�
	SafeRelease(m_pITableViewFrame);

	//ɾ��˽��
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];

		//ɾ��˽��
		CPlatformFrame::GetInstance()->RemoveWhisperItem(pDlgWhisper);

		//�رմ���
		pDlgWhisper->DestroyWindow();

		//�����ڴ�
		SafeDelete(pDlgWhisper);
	}

	//�Ƴ�Ԫ��
	m_DlgWhisperItemArray.RemoveAll();
}

//ʱ����Ϣ
VOID CPlazaViewServer::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//��ʾ��
	if(nIDEvent==IDI_GUIDE_TASK)
	{
		//�رն�ʱ��
		KillTimer(nIDEvent);



		//��˸����
		CPlatformFrame::GetInstance()->FlashWindow(true);

		//�����ؼ�
		CRect rcClient;
		GetClientRect(&rcClient);		
		RectifyControl(rcClient.Width(),rcClient.Height());

		return;
	}
}


//λ����Ϣ
VOID CPlazaViewServer::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}


//��Ϊ����
LRESULT CPlazaViewServer::OnWhisperAddFriend(WPARAM wParam, LPARAM lPara)
{
	//��������
	IClientUserItem * pIClientUserItem=(IClientUserItem*)wParam;

	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if(pIClientUserItem==NULL) return 0;

	//��������
	ASSERT(CUserInformation::GetInstance()!=NULL);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
	BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

	//��������
	DWORD dwMeGroupID=m_pIMySelfUserItem->GetGroupID();
	bool bSameGroup=((dwMeGroupID!=0L)&&(pIClientUserItem->GetGroupID()==dwMeGroupID));

	//���ù�ϵ
	pIClientUserItem->SetUserCompanion(cbDestCompanion);
	pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

	//��������
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=cbDestCompanion;
	(GetPlazaUserManager())->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

	////�����б�
	//m_UserListControl.UpdateDataItem(pIClientUserItem);
	//if (cbDestCompanion==CP_FRIEND) m_UserListCustom.InsertUserItem(pIClientUserItem);
	//if ((bSameGroup==false)&&(cbDestCompanion!=CP_FRIEND)) m_UserListCustom.DeleteUserItem(pIClientUserItem);

	return 0;
}

//��Ϊ���
LRESULT CPlazaViewServer::OnWhisperSetDetest(WPARAM wParam, LPARAM lPara)
{
	//��������
	IClientUserItem * pIClientUserItem=(IClientUserItem*)wParam;

	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if(pIClientUserItem==NULL) return 0;

	//��������
	ASSERT(CUserInformation::GetInstance()!=NULL);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
	BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

	//��������
	DWORD dwMeGroupID=m_pIMySelfUserItem->GetGroupID();
	bool bSameGroup=((dwMeGroupID!=0L)&&(pIClientUserItem->GetGroupID()==dwMeGroupID));

	//���ù�ϵ
	pIClientUserItem->SetUserCompanion(cbDestCompanion);
	pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

	//��������
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=cbDestCompanion;
	(GetPlazaUserManager())->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

	////�����б�
	//m_UserListControl.UpdateDataItem(pIClientUserItem);
	//if (bSameGroup==true) m_UserListCustom.UpdateUserItem(pIClientUserItem);
	//if ((bSameGroup==false)&&(cbDestCompanion!=CP_FRIEND)) m_UserListCustom.DeleteUserItem(pIClientUserItem);

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////