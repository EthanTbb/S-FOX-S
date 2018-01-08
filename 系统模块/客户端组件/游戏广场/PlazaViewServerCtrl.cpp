#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlazaViewServer.h"
#include "PlazaViewContainer.h"
#include "PlazaViewServerCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//���涨��
#define PROPERTY_CY					81									//��Ϸ����
#define BILL_VIEW_CY				81									//�����ͼ

//�ָ����
#define INCISE_SCALE				45/100								//����ָ�

//�ؼ���ʶ
#define IDC_PROPERTY				300									//��Ϸ����
#define IDC_CHAT_CONTROL			301									//�������
#define IDC_WEB_BILL_PUBLICIZE		302									//����ؼ�

//UserList����
#define USERLIST_DISTANCETOP		16									//�϶˾���
#define USERLIST_DISTANCELEFT		5									//��˾���
#define USERLIST_DISTANCERIGHT		5									//�Ҷ˾���
#define USERLIST_DISTANCEBOTTOM		6									//�¶˾���

//Chat����
#define CHAT_DISTANCETOP		6									//�϶˾���
#define CHAT_DISTANCELEFT		5									//��˾���
#define CHAT_DISTANCERIGHT		5									//�Ҷ˾���
#define CHAT_DISTANCEBOTTOM		3									//�¶˾���
//////////////////////////////////////////////////////////////////////////////////
//�˵�����

//��ɫ�˵�
#define MAX_CHAT_COLOR				16									//�����Ŀ
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//ѡ����ɫ

//��ݶ���
#define MAX_SHORT_COUNT				16									//�����Ŀ
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//ѡ�����

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//�༭�ؼ�
const TCHAR * szEditChatControlName=TEXT("EditChat");

//��ť�ؼ�
const TCHAR * szButtonChat1ControlName=TEXT("ButtonChat1");
const TCHAR * szButtonChat2ControlName=TEXT("ButtonChat2");
const TCHAR * szButtonChat3ControlName=TEXT("ButtonChat3");
const TCHAR * szButtonChat4ControlName=TEXT("ButtonChat4");
const TCHAR * szButtonSendChatControlName=TEXT("ButtonSendChat");
const TCHAR * szButtonChatShortControlName=TEXT("ButtonChatShort");
const TCHAR * szContainerChatInputControlName=TEXT("ContainerChatInput");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewServerCtrl, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_SIZE()

	//������Ϣ
	ON_COMMAND(IDM_LIMIT_USER_ROOM_CHAT, OnLimitUserRoomChat)
	ON_COMMAND(IDM_ALLOW_USER_ROOM_CHAT, OnAllowUserRoomChat)
	ON_COMMAND(IDM_LIMIT_USER_GAME_CHAT, OnLimitUserGameChat)
	ON_COMMAND(IDM_ALLOW_USER_GAME_CHAT, OnAllowUserGameChat)
	ON_COMMAND(IDM_LIMIT_USER_WHISP_CHAT, OnLimitUserWhispChat)
	ON_COMMAND(IDM_ALLOW_USER_WHISP_CHAT, OnAllowUserWhispChat)

	//�ؼ���Ϣ
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_CONTROL, OnNMRclickUserList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST_CONTROL, OnNMDblclkUserList) 
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST_CONTROL, OnNMClickUserList)

	//��������
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_SHORT, (IDM_SELECT_CHAT_SHORT+MAX_SHORT_COUNT), OnSelectChatShort)

	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CPlazaViewServerCtrl::CPlazaViewServerCtrl() : CFGuiDialog(IDD_GAME_SERVER_CONTROL)
{
	//�������
	m_bCreateFlag=false;
	m_wChairCount=0;
	m_wServerType=0;
	m_dwServerRule=0;
	m_dwUserRight=0L;
	m_dwTrumpetCount=0;
	m_dwTyphonCount=0;

	//�������
	m_dwChatTime=0L;

	//�ؼ�����
	m_pExpressionControl=NULL;
	m_pTrumpetItem=NULL;	

	//�ӿڱ���
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pISelectedUserItem=NULL;
	m_pIGameLevelParser=NULL;
	m_pIPlazaUserManager=NULL;	

	//������Դ
	tagEncircleResource	EncircleChat;
	EncircleChat.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_TL);
	EncircleChat.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_TM);
	EncircleChat.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_TR);
	EncircleChat.pszImageML=MAKEINTRESOURCE(IDB_CHAT_ML);
	EncircleChat.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_MR);
	EncircleChat.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_BL);
	EncircleChat.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_BM);
	EncircleChat.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_BR);
	m_ChatEncircle.InitEncircleResource(EncircleChat,AfxGetResourceHandle());	

	//��������Դ
	tagEncircleResource	EncircleUserList;
	EncircleUserList.pszImageTL=MAKEINTRESOURCE(IDB_USER_LIST_TL);
	EncircleUserList.pszImageTM=MAKEINTRESOURCE(IDB_USER_LIST_TM);
	EncircleUserList.pszImageTR=MAKEINTRESOURCE(IDB_USER_LIST_TR);
	EncircleUserList.pszImageML=MAKEINTRESOURCE(IDB_USER_LIST_ML);
	EncircleUserList.pszImageMR=MAKEINTRESOURCE(IDB_USER_LIST_MR);
	EncircleUserList.pszImageBL=MAKEINTRESOURCE(IDB_USER_LIST_BL);
	EncircleUserList.pszImageBM=MAKEINTRESOURCE(IDB_USER_LIST_BM);
	EncircleUserList.pszImageBR=MAKEINTRESOURCE(IDB_USER_LIST_BR);
	m_UserListEncircle.InitEncircleResource(EncircleUserList,AfxGetInstanceHandle());

	return;
}

//��������
CPlazaViewServerCtrl::~CPlazaViewServerCtrl()
{
	//���ٴ���
	if(m_pTrumpetItem) m_pTrumpetItem->DestroyWindow();
	if(m_pExpressionControl) m_pExpressionControl->DestroyWindow();

	//�ͷŶ���
	SafeDelete(m_pTrumpetItem);	
	SafeDelete(m_pExpressionControl);

	return;
}

//�ӿڲ�ѯ
VOID * CPlazaViewServerCtrl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGamePropertySink,Guid,dwQueryVer);	
	QUERYINTERFACE(IGamePropertyUseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGamePropertySink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ؼ�
VOID CPlazaViewServerCtrl::InitControlUI()
{
	//���ÿؼ�
	CEditUI * pEditControl = static_cast<CEditUI *>(GetControlByName(szEditChatControlName));
	if(pEditControl)
	{
		/*pEditControl->SetBkColor(RGB(0,0,0));*/		
		pEditControl->SetMaxChar(LEN_USER_CHAT-1);
		//pEditControl->SetTextColor(ARGB(255,255,255,255));
	}

	//���ð�ť
	CButtonUI* pButtonSendChat = static_cast<CButtonUI *>(GetControlByName(szButtonSendChatControlName));
	if(pButtonSendChat) pButtonSendChat->SetWantReturn();
}

//��Ϣ����
VOID CPlazaViewServerCtrl::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(),szButtonSendChatControlName) == 0)
		{
			//��ȡ��Ϣ
			CString strMessage;
			CControlUI * pEditChat=GetControlByName(szEditChatControlName);
			if(pEditChat) strMessage=pEditChat->GetText();

			//��ʽ��Ϣ
			strMessage.TrimLeft();
			strMessage.TrimRight();

			//������Ϣ
			if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
			{
				CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
				SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
				pEditChat->SetText(TEXT(""));
			}

			//���ý���
			pEditChat->SetFocus();

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChatShortControlName) == 0)
		{
			//����˵�
			CSkinMenu ChatShortMenu;
			ChatShortMenu.CreateMenu();

			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			TCHAR szFileName2[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseInfo.ini"),szDirectory,AfxGetApp()->m_pszExeName);
			_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\PhraseInfo.ini"),szDirectory);

			//�ж��ļ�
			TCHAR *pFileName = szFileName;
			CFile file;
			if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
			if(file.m_hFile!=INVALID_HANDLE_VALUE) file.Close();
			ASSERT(pFileName);

			//��������
			BYTE cbIndex=1;
			bool bSuccess=false;
			TCHAR szItemName[16]=TEXT(""),szShortcut[LEN_USER_CHAT]=TEXT("");

			//��ȡ��Ϣ
			while (true)
			{
				//��ȡ��Ϣ
				_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),cbIndex);
				GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

				//����ж�
				if (szShortcut[0]!=0)
				{
					//���ñ���
					cbIndex++;
					bSuccess=true;

					//����˵�
					ChatShortMenu.AppendMenu(IDM_SELECT_CHAT_SHORT+cbIndex-1,szShortcut);
				}

				//�����ж�
				if ((szShortcut[0]==0)||(cbIndex>=MAX_SHORT_COUNT))
				{
					break;
				}
			}

			//�����˵�
			if (bSuccess==true)
			{
				//��������			
				CControlUI * pButtonChatShort=GetControlByName(szButtonChatShortControlName);
				if(pButtonChatShort==NULL) return;

				//��������
				CRect rcButton=pButtonChatShort->GetPos();
				ClientToScreen(&rcButton);
                
				//�����˵�
				ChatShortMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);
			}

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat1ControlName) == 0)
		{
			//��������			
			CControlUI * pChatButton1=GetControlByName(szButtonChat1ControlName);
			if(pChatButton1==NULL) return;

			//��������
			CRect rcButton=pChatButton1->GetPos();
			ClientToScreen(&rcButton);

			//��������
			if (m_pExpressionControl==NULL)
			{
				m_pExpressionControl=new CExpressionControl;
			}

			//��ʾ����
			m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat2ControlName) == 0)
		{
			//��ȡ����
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//��������
			INT nWidth=72,nHeight=16;
			COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
				RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

			//������ɫ
			CImage ImageColor[CountArray(crColor)];
			for (INT i=0;i<CountArray(ImageColor);i++)
			{
				//����λͼ
				ImageColor[i].Create(nWidth,nHeight,16);

				//��ȡ�豸
				CImageDC ImageDC(ImageColor[i]);
				CDC * pDC=CDC::FromHandle(ImageDC);

				//�滭λͼ
				pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
				pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
				pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
			}

			//�����˵�
			CSkinMenu ColorMenu;
			ColorMenu.CreateMenu();

			//����˵�
			ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("ѡ��������ɫ"));

			//��ɫѡ��
			ColorMenu.AppendSeparator();
			for (INT i=0;i<CountArray(ImageColor);i++) 
			{
				ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
				if (pParameterGlobal->m_crChatTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
			}

			//���Ҷ���
			CControlUI * pChatButton2=GetControlByName(szButtonChat2ControlName);
			if(pChatButton2==NULL) return;

			//�����˵�
			CRect rcButton=pChatButton2->GetPos();
			ClientToScreen(&rcButton);
			ColorMenu.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat3ControlName) == 0)
		{
			//�����˵�
			CSkinMenu MenuMessage;
			MenuMessage.CreateMenu();

			//����˵�
			MenuMessage.AppendMenu(IDM_MESSAGE_SHOW_ALL,TEXT("��ʾ������Ϣ"));
			MenuMessage.AppendMenu(IDM_MESSAGE_HIDE_DETEST,TEXT("���������Ϣ"));
			MenuMessage.AppendMenu(IDM_MESSAGE_ONLY_FRIEND,TEXT("ֻ��ʾ������Ϣ"));

			//��ȡ����
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//���ò˵�
			BYTE cbMessageMode=pParameterGlobal->m_cbMessageMode;
			if (cbMessageMode==MESSAGE_MODE_ALL) MenuMessage.CheckMenuItem(IDM_MESSAGE_SHOW_ALL,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_DETEST) MenuMessage.CheckMenuItem(IDM_MESSAGE_HIDE_DETEST,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_FRIEND) MenuMessage.CheckMenuItem(IDM_MESSAGE_ONLY_FRIEND,MF_BYCOMMAND|MF_CHECKED);

			//���Ҷ���
			CControlUI * pChatButton3=GetControlByName(szButtonChat3ControlName);
			if(pChatButton3==NULL) return;

			//�����˵�
			CRect rcButton=pChatButton3->GetPos();
			ClientToScreen(&rcButton);
			MenuMessage.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat4ControlName) == 0)
		{
			//��������
			CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
			CGamePropertyBase * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
			CGamePropertyBase * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

			//��Ч�ж�
			if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon==NULL)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("��ǰ���ȹ���δ���ã�"),MB_ICONINFORMATION,30L);
				return;
			}

			//��������
			if (m_pTrumpetItem==NULL)
			{
				m_pTrumpetItem=new CDlgTrumpet;
				m_pTrumpetItem->SetGameTrumpetSink(QUERY_ME_INTERFACE(IUnknownEx));
			}
			//����������
			m_pTrumpetItem->SetTrumpet(m_dwTrumpetCount);
			m_pTrumpetItem->SetTyphon(m_dwTyphonCount);

			//��ʾ����
			m_pTrumpetItem->ShowTrumpetWindow(AfxGetMainWnd(),m_pIMySelfUserItem);

			return;
		}
	}
}

//��ʼ�滭
void CPlazaViewServerCtrl::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ����
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�û��б�
	CRect rcListArea;
	rcListArea.left=0;
	rcListArea.top=0;
	rcListArea.right=rcClient.Width();
	rcListArea.bottom=rcClient.Height()*INCISE_SCALE;		
	m_UserListEncircle.DrawEncircleFrame(pDC,rcListArea);

	//�滭����
	CRect rcChatArea;
	rcChatArea.left=0;
	rcChatArea.top=rcListArea.bottom;
	rcChatArea.right=rcClient.Width();
	rcChatArea.bottom=rcClient.Height();	
	m_ChatEncircle.DrawEncircleFrame(pDC,rcChatArea);	

	return;
}


int CPlazaViewServerCtrl::GetTrumpetCount()
{
	return m_dwTrumpetCount;
}

VOID CPlazaViewServerCtrl::SetTrumpetCount(const DWORD dwCount)
{
	m_dwTrumpetCount = dwCount;
	if(m_pTrumpetItem) m_pTrumpetItem->SetTrumpet(dwCount);
}

int CPlazaViewServerCtrl::GetTyphonCount()
{
	return m_dwTyphonCount;
}

VOID CPlazaViewServerCtrl::SetTyphonCount(const DWORD dwCount)
{
	m_dwTyphonCount = dwCount;
	if(m_pTrumpetItem) m_pTrumpetItem->SetTyphon(dwCount);
}

//ʹ�õ���
bool CPlazaViewServerCtrl::OnEventUseProperty(WORD wPropertyIndex)
{
	//��������
	if(wPropertyIndex==PROPERTY_ID_TRUMPET || wPropertyIndex==PROPERTY_ID_TYPHON)
	{
		//������
		CMD_GR_C_SendTrumpet SendTrumpet;
		SendTrumpet.cbRequestArea=PT_ISSUE_AREA_SERVER;
		SendTrumpet.wPropertyIndex=wPropertyIndex;
		SendTrumpet.TrumpetColor=m_pTrumpetItem->GetTrumpetColor();
		m_pTrumpetItem->GetTrumpetContent(SendTrumpet.szTrumpetContent);

		//������Ϣ
		m_pITCPSocket->SendData(MDM_GR_PROPERTY,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet));
	}

	return true;
}

//��ʾ����
bool CPlazaViewServerCtrl::PerformShowBag()
{
	return CGlobalUnits::GetInstance()->PerformShowBag();
}

//��ʾ�̳�
bool CPlazaViewServerCtrl::PerformShowShop()
{
	return CGlobalUnits::GetInstance()->PerformShowShop();
}


//�б�����
VOID CPlazaViewServerCtrl::SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//�����б�
	m_UserListControl.SetColumnDescribe(ColumnItem,cbColumnCount);
}

//��ʾ�˵�
VOID CPlazaViewServerCtrl::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//�ж�״̬
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true) return;

	//���ñ���
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.Add(pIClientUserItem);

	//����˵�
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//��������
	TCHAR szMenuString[256]=TEXT("");
	bool bMeUserItem=(pIClientUserItem==m_pIMySelfUserItem);

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//��������
	LPCTSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//�û���Ϣ
	WORD wTableID=pUserInfo->wTableID;
	WORD wChairID=pUserInfo->wChairID;
	BYTE cbUserStatus=pUserInfo->cbUserStatus;

	//�����Ϣ
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//������Ƭ
	CImage ImageUserCard;
	pUserItemElement->ConstructNameCard(pIClientUserItem,m_pIGameLevelParser,ImageUserCard);
	if (ImageUserCard.IsNull()==false) UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,ImageUserCard,MF_GRAYED);

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//����˵�
	if (bMeUserItem==false)
	{
		UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("����˽����Ϣ ..."));
		UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);
	}
	UserInfoMenu.AppendMenu(IDM_COPY_USER_NICKNAME,TEXT("�����ǳ�"));

	//���ý�̸
	if ((bMeUserItem==false)&&(pIChatTargetItem!=m_pIMySelfUserItem)&&(pIChatTargetItem!=pIClientUserItem))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�� [ %s ] ��̸"),pIClientUserItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_SET_CHAT_USER,szMenuString);
	}

	//ȡ����̸
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ȡ���� [ %s ] ��̸"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//�����˵�
	if ((bMeUserItem==false)&&(cbMeUserStatus!=US_PLAYING)&&(m_wServerType&GAME_GENRE_MATCH)==0)
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//�Թ���Ϸ
		bool bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�Թ� [ %s ] ��Ϸ"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_LOOKON_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//һ����Ϸ
		bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_SIT)||(cbUserStatus==US_READY)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %s ] ����Ϸ��"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_PLAY_GAME_TOGETHER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//������Ϸ
		bEnableMenu=((wMeTableID!=INVALID_TABLE)&&(cbMeUserStatus!=US_LOOKON)&&(cbUserStatus!=US_PLAYING)&&(cbUserStatus!=US_OFFLINE));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %s ] һ������Ϸ"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_INVITE_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);
	}

	//���ѹ���
	if (bMeUserItem==false&&(m_wServerType&GAME_GENRE_MATCH)==0)
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//����˵�
		BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
		UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("����Ϊ����"),(cbCompanion==CP_FRIEND)?MF_CHECKED:0);
		UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("����Ϊ���"),(cbCompanion==CP_DETEST)?MF_CHECKED:0);
	}

	//����˵�
	if ((bMeUserItem==false)&&(m_pIMySelfUserItem->GetMasterOrder()!=0))
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//����˵�
		bool bPlaying = (pIClientUserItem->GetUserStatus()==US_PLAYING);
		UserInfoMenu.AppendMenu(IDM_KILL_USER,TEXT("���û�����..."),(CMasterRight::CanKillUser(m_dwMasterRight)==false && !bPlaying)?MF_GRAYED:MF_ENABLED);
		
		//��ҹ���
		UserInfoMenu.AppendSeparator();
		UINT nMenuFlags=(CMasterRight::CanLimitRoomChat(m_dwMasterRight)==false)?MF_GRAYED:MF_ENABLED;
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_ROOM_CHAT,TEXT("��ֹ���û���������"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_ROOM_CHAT,TEXT("������û���������"),nMenuFlags);

		nMenuFlags=(CMasterRight::CanLimitGameChat(m_dwMasterRight)==false)?MF_GRAYED:MF_ENABLED;
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_GAME_CHAT,TEXT("��ֹ���û���Ϸ����"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_GAME_CHAT,TEXT("������û���Ϸ����"),nMenuFlags);

		nMenuFlags=(CMasterRight::CanLimitWisper(m_dwMasterRight)==false)?MF_GRAYED:MF_ENABLED;
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_WHISP_CHAT,TEXT("��ֹ���û�˽��"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_WHISP_CHAT,TEXT("������û�˽��"),nMenuFlags);
	}

	//���ܲ˵�
	if((bMeUserItem==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0) && (CUserRight::CanKillOutUser(m_dwUserRight)==true)&&(m_wServerType&GAME_GENRE_MATCH)==0)
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//���ܲ˵�
		TCHAR szText[32]=TEXT("");
		_sntprintf(szText,CountArray(szText),TEXT("�� [%s] ����"),pIClientUserItem->GetNickName());
		bool bPlaying = (pIClientUserItem->GetUserStatus()==US_PLAYING);		
		bool bUnEnableMenu =  (m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder()) && !bPlaying;
		UserInfoMenu.AppendMenu(IDM_KILL_USER,szText,bUnEnableMenu?MF_GRAYED:MF_ENABLED);
	}

	////����ָ�
	//UserInfoMenu.AppendSeparator();

	//UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("���������û� ..."));

	////����ָ�
	//UserInfoMenu.AppendSeparator();

	//���λ��
	if (wTableID!=INVALID_TABLE)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %d ] ����Ϸ��"),wTableID+1);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//������Ϣ
	DWORD dwPlayCount=pIClientUserItem->GetUserPlayCount();
	FLOAT fWinRate=pIClientUserItem->GetUserWinRate();
	FLOAT fUserFleeRate=pIClientUserItem->GetUserFleeRate();
	FLOAT fUserDrawRate=pIClientUserItem->GetUserDrawRate();
	_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�ɼ���") SCORE_STRING TEXT(" ʤ�ʣ�%.2f%% �����ʣ�%.2f%%"),
		pUserInfo->lScore,fWinRate,fUserFleeRate);
	UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

	//������Ϣ
	if (pUserInfo->szGroupName[0]!=0)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��Ϸ���ţ�%s"),pUserInfo->szGroupName);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//��ע��Ϣ
	if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��ע��Ϣ��%s"),pszUserNote);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}
	//ʹ�õ���
	if(bMeUserItem == false)
	{
		UserInfoMenu.AppendMenu(IDM_USE_PROPERTY, TEXT("ʹ�õ���"), 0);
	}

	//�����˵�
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//��ʾ�˵�
VOID CPlazaViewServerCtrl::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount)
{
	//Ч�����
	ASSERT((pIClientUserItem!=NULL)&&(wItemCount>0));
	if ((pIClientUserItem==NULL)||(wItemCount==0)) return;

	//�ж�״̬
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true) return;

	//���ñ���
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.SetSize(wItemCount);
	CopyMemory(m_MenuUserItemArray.GetData(),pIClientUserItem,wItemCount*sizeof(IClientUserItem *));

	//����˵�
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//��������
	TCHAR szMenuString[256]=TEXT("");
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//�����Ϣ
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//����˽��
	UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("����Ⱥ����Ϣ ..."),MF_GRAYED);
	UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);

	//�������
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ȡ���� [ %s ] ��̸"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//����˵�
	UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("����Ϊ����"));
	UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("����Ϊ���"));

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//���ܲ˵�
	UserInfoMenu.AppendMenu(IDM_USER_INFO_MANAGER,TEXT("�û���Ϣ���� ..."));
	//UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("���������û� ..."));

	//�����˵�
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//�û�����
VOID CPlazaViewServerCtrl::OnEventUserEnter(IClientUserItem * pIClientUserItem)
{
	//�����Լ�
	if (m_pIMySelfUserItem==NULL)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();

		//�Լ��ж�
		if (pGlobalUserInfo->GetGlobalUserData()->dwUserID==pIClientUserItem->GetUserID())
		{
			//���ñ���
			m_pIMySelfUserItem=pIClientUserItem;

			//���ý���
			m_UserListControl.SetMySelfUserID(m_pIMySelfUserItem->GetUserID());
			m_UserListControl.SetServerRule(m_dwServerRule);
		}
	}

	//�����û�
	m_UserListControl.InsertDataItem(pIClientUserItem);

	//��������
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��ʾ��Ϣ
	if(pParameterGlobal->m_bNotifyUserInOut==true)
	{
		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}
	return;
}

//�û��뿪
VOID CPlazaViewServerCtrl::OnEventUserLeave(IClientUserItem * pIClientUserItem)
{
	//��������
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();

	//ɾ���û�
	m_UserListControl.DeleteDataItem(pIClientUserItem);
	m_ChatControl.DeleteUserItem(pIClientUserItem);

	//�˵�����
	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	{
		//��ȡ�û�
		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
		if (pIChatUserItem->GetUserID()==pIClientUserItem->GetUserID()) m_MenuUserItemArray.RemoveAt(i);
	}	
	
	//��������
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��ʾ��Ϣ
	if(pParameterGlobal->m_bNotifyUserInOut==true)
	{
		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
	}

	return;
}

//�û�״̬
VOID CPlazaViewServerCtrl::OnEventUserUpdate(IClientUserItem * pIClientUserItem)
{
	//����״̬
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	return;
}
//�������
bool CPlazaViewServerCtrl::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//Ч�����
	ASSERT((pszNickName!=NULL)&&(wItemCount>0));
	if ((pszNickName==NULL)||(wItemCount==0)) return false;

	//���Ҷ���
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

	//ִ�й���
	return pPlazaViewServer->PerformBuyProperty(PT_ISSUE_AREA_SERVER,pszNickName,wItemCount,wPropertyIndex);
}

//�û�ѡ��
VOID CPlazaViewServerCtrl::OnChangeChatTarget(IClientUserItem * pIClientUserItem)
{
	//���ñ���
	if (pIClientUserItem!=NULL)
	{
		//���ý���
		if (m_ChatControl.m_hWnd==NULL)
		{
			//��������
			CRect rcCreate(0,0,0,0);
			m_ChatControl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT_CONTROL);

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}
	else
	{
		//���ý���
		if (m_ChatControl.m_hWnd!=NULL)
		{
			//���ٴ���
			m_ChatControl.DestroyWindow();

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//���ý���
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();
}

//�����¼�
VOID CPlazaViewServerCtrl::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	//���ý���
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	//���ͱ���
	if (EfficacyUserChat(NULL,pExpression->GetIndex())==true)
	{
		SendExpressionPacket(m_ChatControl.GetChatTargetUserID(),pExpression->GetIndex());
	}
}


bool CPlazaViewServerCtrl::OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage)
{
	//�����׳�
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
	{
		TCHAR szMessage[128]=TEXT("�����׳��������������");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	//����û�
	IClientUserItem * pIClientUserItem=m_pIPlazaUserManager->SearchUserByNickName(pszNickName);
	if(pIClientUserItem==NULL)
	{
		TCHAR szMessage[128]=TEXT("��ָ����ʹ�ö����ڱ������л򲻴��ڣ�");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	return true;
}

//��������
VOID CPlazaViewServerCtrl::OnEventPropertyTrumpet()
{
	//���½���
	if(m_pTrumpetItem!=NULL) m_pTrumpetItem->UpdateControlSurface();
}

//�����¼�
VOID CPlazaViewServerCtrl::SetServerInfo(WORD wChairCount,WORD wServerType,DWORD dwServerRule,DWORD dwUserRight,DWORD dwMasterRight)
{
	//���ñ���
	m_wChairCount = wChairCount;
	m_wServerType = wServerType;
	m_dwServerRule = dwServerRule;
	m_dwUserRight = dwUserRight;
	m_dwMasterRight = dwMasterRight;	

	//��������
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

	//������֤
	ASSERT(pPlazaViewServer!=NULL);
	if(pPlazaViewServer==NULL) return;

	//��ѯ�ӿ�
	m_pITCPSocket = pPlazaViewServer->GetTCPSocket();
	m_pIPlazaUserManager=pPlazaViewServer->GetPlazaUserManager();
	m_pIGameLevelParser=pPlazaViewServer->GetGameLevelParser();

	//���ýӿ�
	m_UserListControl.SetServerRule(m_dwServerRule);
	m_UserListControl.SetGameLevelParser(m_pIGameLevelParser);

	//���ÿؼ�
	m_UserListControl.DeleteAllItems();

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//�ؼ���
VOID CPlazaViewServerCtrl::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//����ؼ�
	DDX_Control(pDX, IDC_USER_LIST_CONTROL, m_UserListControl);

	//����ؼ�
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);

	return;
}

//���ú���
BOOL CPlazaViewServerCtrl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_bCreateFlag=true;

	//���ô���
	CSize SizeWindow(m_PaintManager.GetInitSize());
	SetWindowPos(NULL, 0, 0, SizeWindow.cx, SizeWindow.cy, SWP_NOZORDER|SWP_NOMOVE);

	//��ʼ���ؼ�
	InitSilderControl();

	//��������
	HINSTANCE hInstance= AfxGetResourceHandle();

	//����ؼ�
	m_ChatControl.SetChatControlSink(this);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(50,30,17));

	//���ÿؼ�
	m_ChatMessage.SetBackgroundColor(FALSE,RGB(50,30,17));

	return TRUE;
}

//��Ϣ����
BOOL CPlazaViewServerCtrl::PreTranslateMessage(MSG * pMsg)
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
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlazaViewServerCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
	case IDM_MESSAGE_SHOW_ALL:		//��ʾ����
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_ALL;

			return TRUE;
		}
	case IDM_MESSAGE_HIDE_DETEST:	//�������
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_DETEST;

			return TRUE;
		}
	case IDM_MESSAGE_ONLY_FRIEND:	//��ʾ����
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_FRIEND;

			return TRUE;
		}
	case IDM_SET_CHAT_USER:			//��̸����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem!=NULL) m_ChatControl.SetChatTargetUser(pIClientUserItem);

			return TRUE;
		}
	case IDM_CANCEL_CHAT_USER:		//ȡ����̸
		{
			//��������
			m_ChatControl.SetChatTargetUser(NULL);

			return TRUE;
		}
	case IDM_CREATE_ISER_WISPER:	//����˽��
		{
			//��������
			CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
			CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

			//����˽��
			WORD wUserCount=(WORD)m_MenuUserItemArray.GetCount();
			pPlazaViewServer->WhisperConversation(m_MenuUserItemArray.GetData(),wUserCount);

			return TRUE;
		}
	case IDM_COPY_USER_NICKNAME:	//�����ǳ�
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//�����ַ�
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			CWHService::SetClipboardString(pIClientUserItem->GetNickName());

			//�����ַ�
			CControlUI * pEditChat=GetControlByName(szEditChatControlName);
			if(pEditChat)
			{
				pEditChat->SetText(pIClientUserItem->GetNickName());
				pEditChat->SetFocus();
			}

			return TRUE;
		}
	
	case IDM_SEE_USER_LOCATION:		//�鿴λ��
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//������Ϣ
			bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMasterOrder()==0)) return FALSE;

			//��ȡ�û�
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//��������
			CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
			CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
			ITableViewFrame * pITableViewFrame = pPlazaViewServer->GetTableViewFrame();

			//��ȡ����
			WORD wTableID=pIClientUserItem->GetTableID();
			WORD wChairID=pIClientUserItem->GetChairID();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

			//��λλ��
			if (wTableID!=INVALID_TABLE)
			{
				//���ÿ���
				/*ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
				CWndViewItemCtrl::GetInstance()->ActiveViewItem(this);*/

				//���ÿ���
				pITableViewFrame->VisibleTable(wTableID);

				//��������
				if (cbUserStatus==US_LOOKON)
				{
					pITableViewFrame->FlashGameTable(wTableID);
				}
				else
				{
					pITableViewFrame->FlashGameChair(wTableID,wChairID);
				}
			}

			//��������
			LVFINDINFO LVFindInfo;
			ZeroMemory(&LVFindInfo,sizeof(LVFindInfo));

			//���ñ���
			LVFindInfo.flags=LVFI_PARAM;
			LVFindInfo.lParam=(LPARAM)pIClientUserItem;

			//��λ�б�
			INT nItem=m_UserListControl.FindItem(&LVFindInfo);

			//�����б�
			if (nItem!=-1L)
			{
				m_UserListControl.EnsureVisible(nItem,FALSE);
				m_UserListControl.SetItemState(nItem,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
			}

			return TRUE;
		}
	case IDM_KILL_USER:             //���û�����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��ȡ�û�
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//������Ϣ
			bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMasterOrder()==0)&&
				(m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder())) return FALSE;

			//��ȡ����
			WORD wTableID=pIClientUserItem->GetTableID();
			WORD wChairID=pIClientUserItem->GetChairID();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
			if(cbUserStatus==US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("������ѿ�ʼ��Ϸ�������ߣ�"),MB_ICONINFORMATION);
				return TRUE;
			}

			//��������
			CMD_GR_KickUser KickUser;
			KickUser.dwTargetUserID=pIClientUserItem->GetUserID();

			//��������
			if(m_pIMySelfUserItem->GetMasterOrder()!=0)
			{
				m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_KILL_USER,&KickUser,sizeof(KickUser));
			}
			else
			{
				//��������
				CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
				CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
				ITableViewFrame * pITableViewFrame = pPlazaViewServer->GetTableViewFrame();

				//������Ϸ
				if(pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("�ܱ�Ǹ��������Ϸ�������ˣ�"),MB_ICONQUESTION);
					return TRUE;
				}

				m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_KICK_USER,&KickUser,sizeof(KickUser));
			}

			return TRUE;
		}

	case IDM_LOOKON_USER:           //�Թ���Ϸ
		{
			//��ȡ����
			CPlazaViewContainer * pPlazaViewContainer=CPlazaViewContainer::GetInstance();
			CPlazaViewServer * pPlazaViewServer=(CPlazaViewServer *)pPlazaViewContainer->GetViewItemByArea(VIA_Center);

			//״̬�ж�
			if (pPlazaViewServer==NULL) return TRUE;			
			if (pPlazaViewServer->GetServiceStatus()!=ServiceStatus_ServiceIng) return TRUE;

			//������Ϸ
			if (m_pIMySelfUserItem->GetMasterOrder()==0 && m_wChairCount>=MAX_CHAIR)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("������Ϸ�������Թۣ�"),MB_ICONINFORMATION);
				return TRUE;
			}

			//������Ϣ
			if (m_pIMySelfUserItem->GetUserStatus()==US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("���ڽ�����Ϸ���������Ϸ���ٽ����Թۣ�"),MB_ICONINFORMATION);
				return TRUE;
			}

			//Ѱ�����
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem==NULL) return TRUE;

			//���״̬�ж�
			if(pIClientUserItem->GetUserStatus()!=US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("������Ѳ�����Ϸ�У�"),MB_ICONINFORMATION);
				return TRUE;
			}

			//�Թ۶���
			pPlazaViewServer->PerformLookonAction(pIClientUserItem->GetTableID(),pIClientUserItem->GetChairID());

			return TRUE;
		}
	case IDM_PLAY_GAME_TOGETHER:    //һ����Ϸ
		{
			//��ȡ����
			CPlazaViewContainer * pPlazaViewContainer=CPlazaViewContainer::GetInstance();
			CPlazaViewServer * pPlazaViewServer=(CPlazaViewServer *)pPlazaViewContainer->GetViewItemByArea(VIA_Center);

			//״̬�ж�
			if (pPlazaViewServer==NULL) return TRUE;
			if (pPlazaViewServer->GetServiceStatus()!=ServiceStatus_ServiceIng) return TRUE;

			//������Ϣ
			if (m_pIMySelfUserItem->GetUserStatus()==US_PLAYING) return TRUE;

			//Ѱ�����
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem==NULL) return TRUE;

			//���״̬�ж�
			if(pIClientUserItem->GetTableID()==INVALID_TABLE)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("������Ѳ��������ϣ�"),MB_ICONINFORMATION);
				return TRUE;
			}

			//��ȡ��λ
			WORD wChairID=INVALID_CHAIR;
			ITableViewFrame * pITableViewFrame=pPlazaViewServer->GetTableViewFrame();
			WORD wNullCount=pITableViewFrame->GetNullChairCount(pIClientUserItem->GetTableID(),wChairID);
			if (wNullCount==0) 
			{
				CInformation Information2(this);
				Information2.ShowMessageBox(TEXT("����Ϸ���Ѿ�û�п�λ���ˣ�"),MB_ICONINFORMATION);
				return true;
			}

			//���¶���
			pPlazaViewServer->PerformSitDownAction(pIClientUserItem->GetTableID(),wChairID,false);

			return TRUE;
		}
	case IDM_INVITE_USER:           //������Ϸ
		{
			//״̬�ж�
			//if (m_ServiceStatus!=ServiceStatus_ServiceIng) return TRUE;

			//������Ϣ
			if (m_pIMySelfUserItem->GetUserStatus()==US_PLAYING) return TRUE;
			if (m_pIMySelfUserItem->GetTableID()==INVALID_TABLE) return TRUE;

			//Ѱ�����
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem==NULL) return TRUE;

			//��������
			CMD_GR_UserInviteReq UserInviteReq;
			UserInviteReq.wTableID=m_pIMySelfUserItem->GetTableID();
			UserInviteReq.dwUserID=pIClientUserItem->GetUserID();
			m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_INVITE_REQ,&UserInviteReq,sizeof(UserInviteReq));

			//��ʾ��Ϣ
			m_ChatMessage.InsertSystemString(TEXT("�ɹ������û���������"));

			return TRUE;
		}
	case IDM_SET_FRIEND_USER:		//��Ϊ����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

			//��������
			tagUserAttrib UserAttrib;
			UserAttrib.cbCompanion=cbDestCompanion;

			//��������
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//���ù�ϵ
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);
			m_pIPlazaUserManager->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

			return TRUE;
		}
	case IDM_SET_DETEST_USER:		//��Ϊ���
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

			//��������
			tagUserAttrib UserAttrib;
			UserAttrib.cbCompanion=cbDestCompanion;

			//��������
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//���ù�ϵ
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);
			m_pIPlazaUserManager->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

			return TRUE;
		}
	case IDM_USE_PROPERTY:		//ʹ�õ���
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;
			//��������
			IClientUserItem* pIClientUserItem=m_MenuUserItemArray[0];

			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowPropertyUse(pIClientUserItem);
			return TRUE;
		}
	//case IDM_KICK_USER:             //�߳��û�
	//	{
	//		//Ч��״̬
	//		ASSERT(m_MenuUserItemArray.GetCount()>0);
	//		if (m_MenuUserItemArray.GetCount()==0) return TRUE;

	//		//��������
	//		IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//		//������Ϣ
	//		bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_pIClientKernel->GetServerAttribute()->dwServerRule);
	//		if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder())) return FALSE;

	//		//��ȡ����
	//		BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	//		if(cbUserStatus==US_PLAYING)
	//		{
	//			CInformation Information(this);
	//			Information.ShowMessageBox(TEXT("������ѿ�ʼ��Ϸ�������ߣ�"),MB_ICONINFORMATION);
	//			return TRUE;
	//		}
	//		
	//		//��������
	//		IPC_GF_KickUser KickUser;
	//		KickUser.dwTargetUserID=pIClientUserItem->GetUserID();

	//		//��������
	//		m_pIClientKernel->SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_KICK_USER,&KickUser,sizeof(KickUser));
	//	}
	}

	return __super::OnCommand(wParam,lParam);
}

//�����ؼ�
VOID CPlazaViewServerCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//������Ϣ
	tagEncircleInfo EncircleInfoList;
	m_UserListEncircle.GetEncircleInfo(EncircleInfoList);

	//�û��б�
	CRect rcListArea;
	rcListArea.left=0;
	rcListArea.top=0;
	rcListArea.right=nWidth;
	rcListArea.bottom=nHeight*INCISE_SCALE;		

	//�滭����
	CRect rcChatArea;
	rcChatArea.left=0;
	rcChatArea.top=rcListArea.bottom;
	rcChatArea.right=nWidth;
	rcChatArea.bottom=nHeight;	

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//��������
	CRect rcListAreaModify;
	rcListAreaModify.left=0 - USERLIST_DISTANCELEFT;
	rcListAreaModify.top=0 - USERLIST_DISTANCETOP;
	rcListAreaModify.right=nWidth +USERLIST_DISTANCERIGHT;
	rcListAreaModify.bottom=nHeight*INCISE_SCALE + USERLIST_DISTANCEBOTTOM;		

	//�û��б�
	m_UserListEncircle.DeferWindowPos(&m_UserListControl,hDwp,rcListAreaModify);
	
	//�������
	if ((m_ChatControl.m_hWnd!=NULL)&&(m_ChatControl.GetChatTargetUserItem()!=NULL))
	{
		//��ȡλ��
		CRect rcChatControl;
		m_ChatControl.GetWindowRect(&rcChatControl);

		//λ����Ϣ
		tagEncircleInfo EncircleInfo;
		m_ChatEncircle.GetEncircleInfo(EncircleInfo);

		//����ؼ�
		rcChatArea.bottom-=rcChatControl.Height();
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatArea);

		//�������
		DeferWindowPos(hDwp,m_ChatControl,NULL,rcChatArea.left+EncircleInfo.nLBorder,rcChatArea.bottom-EncircleInfo.nBBorder,
			rcChatArea.Width()-EncircleInfo.nLBorder-EncircleInfo.nRBorder,rcChatControl.Height(),uFlags);
	}
	else
	{
		//��������
		CRect rcChatAreaModify;
		rcChatAreaModify.left=rcChatArea.left - CHAT_DISTANCELEFT;
		rcChatAreaModify.top=rcChatArea.top - CHAT_DISTANCETOP;
		rcChatAreaModify.right=rcChatArea.right +CHAT_DISTANCERIGHT;
		rcChatAreaModify.bottom=rcChatArea.bottom + CHAT_DISTANCEBOTTOM;		

		//����ؼ�
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatAreaModify);
	}

	//�ƶ�����
	EndDeferWindowPos(hDwp);




	return;
}

//�����б�
VOID CPlazaViewServerCtrl::InitSilderControl()
{
	//�������
	tagColorUserList ColorUserList;
	ZeroMemory(&ColorUserList,sizeof(ColorUserList));

	//��ɫ����
	ColorUserList.crSelectTX=COLOR_SELECT_TX;
	ColorUserList.crSelectBK=COLOR_SELECT_BK;
	ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//��ɫ����
	ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	ColorUserList.crMasterTX=COLOR_MASTER_TX;
	ColorUserList.crMasterBK=COLOR_MASTER_BK;
	ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	ColorUserList.crMemberBK=COLOR_MEMBER_BK;
	ColorUserList.crAndroidTX=COLOR_ANDROID_TX;
	ColorUserList.crAndroidBK=COLOR_ANDROID_BK;

	//���ÿؼ�
	m_UserListControl.SetColorUserList(ColorUserList);
}


//����Ч��
bool CPlazaViewServerCtrl::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	//״̬�ж�
	if (m_pIMySelfUserItem==NULL) return false;

	//���ȱȽ�
	if(pszChatString != NULL)
	{
		CString strChat=pszChatString;
		if(strChat.GetLength() >= (LEN_USER_CHAT/2))
		{
			m_ChatMessage.InsertString(TEXT("��Ǹ����������������ݹ����������̺��ٷ��ͣ�\r\n"),COLOR_WARN);
			return false;
		}
	}

	//��������
	BYTE cbMemberOrder=m_pIMySelfUserItem->GetMemberOrder();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//��������
	if (CServerRule::IsForfendGameChat(m_dwServerRule)&&(cbMasterOrder==0))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û��������죡"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanRoomChat(m_dwUserRight)==false)
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("��Ǹ����û�д������Ե�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"));

		return false;
	}

	//�ٶ��ж�
	DWORD dwCurrentTime=(DWORD)time(NULL);
	if ((cbMasterOrder==0)&&((dwCurrentTime-m_dwChatTime)<=TIME_USER_CHAT))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("����˵���ٶ�̫���ˣ����������ȱ�����Ϣ�°ɣ�"));

		return false;
	}

	//���ñ���
	m_dwChatTime=dwCurrentTime;
	m_strChatString=pszChatString;

	return true;
}

//��������
bool CPlazaViewServerCtrl::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//������Ϣ
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//��������
	UserChat.dwChatColor=crColor;	
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);
	UserChat.dwSendUserID=m_pISelectedUserItem?m_pISelectedUserItem->GetUserID():m_pIMySelfUserItem->GetUserID();

	//��������
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//���ͱ���
bool CPlazaViewServerCtrl::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//��������
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//������Ϣ
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;
	UserExpression.dwSendUserID=m_pISelectedUserItem?m_pISelectedUserItem->GetUserID():m_pIMySelfUserItem->GetUserID();

	//��������
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}


//��ֹ�û���������
VOID CPlazaViewServerCtrl::OnLimitUserRoomChat()
{
	//Ч��״̬
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//��������
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//���ñ���
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_CHAT;
	LimitUserChat.cbLimitValue = TRUE;

	//������Ϣ
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//�����û���������
VOID CPlazaViewServerCtrl::OnAllowUserRoomChat()
{
	//Ч��״̬
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//��������
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//���ñ���
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_CHAT;
	LimitUserChat.cbLimitValue = FALSE;

	//������Ϣ
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//��ֹ�û���Ϸ����
VOID CPlazaViewServerCtrl::OnLimitUserGameChat()
{
	//Ч��״̬
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//��������
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//���ñ���
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_GAME_CHAT;
	LimitUserChat.cbLimitValue = TRUE;

	//������Ϣ
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//�����û���Ϸ����
VOID CPlazaViewServerCtrl::OnAllowUserGameChat()
{
	//Ч��״̬
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//��������
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//���ñ���
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_GAME_CHAT;
	LimitUserChat.cbLimitValue = FALSE;

	//������Ϣ
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//��ֹ�û�˽��
VOID CPlazaViewServerCtrl::OnLimitUserWhispChat()
{
	//Ч��״̬
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//��������
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//���ñ���
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_WISPER;
	LimitUserChat.cbLimitValue = TRUE;

	//������Ϣ
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//�����û�˽��
VOID CPlazaViewServerCtrl::OnAllowUserWhispChat()
{
	//Ч��״̬
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//��������
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//���ñ���
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_WISPER;
	LimitUserChat.cbLimitValue = FALSE;

	//������Ϣ
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//������ɫ
VOID CPlazaViewServerCtrl::OnSelectMoreColor()
{
	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������ɫ
	CColorDialog ColorDialog(pParameterGlobal->m_crChatTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crChatTX=ColorDialog.GetColor();

	//���ý���
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	return;
}

//ѡ����ɫ
VOID CPlazaViewServerCtrl::OnSelectChatColor(UINT nCommandID)
{
	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//ѡ����ɫ
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crChatTX=crColor[nIndex];

	//���ý���
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	return;
}

//ѡ�����
VOID CPlazaViewServerCtrl::OnSelectChatShort(UINT nCommandID)
{
	//��������
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_SHORT;

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	TCHAR szFileName2[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseInfo.ini"),szDirectory,AfxGetApp()->m_pszExeName);
	_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\PhraseInfo.ini"),szDirectory);

	//�ж��ļ�
	TCHAR *pFileName = szFileName;
	CFile file;
	if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
	if(file.m_hFile!=INVALID_HANDLE_VALUE) file.Close();
	ASSERT(pFileName);

	//��������
	TCHAR szItemName[16]=TEXT("");
	TCHAR szShortcut[LEN_USER_CHAT]=TEXT("");

	//��ȡ��Ϣ
	_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),nIndex);
	GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

	//������Ϣ
	if (szShortcut[0]!=0)
	{
		//��ȡ��Ϣ
		CString strMessage=szShortcut;
		strMessage.TrimLeft();strMessage.TrimRight();

		//������Ϣ
		if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
		{
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
		}
	}

	//���ý���
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	return;
}

//������Ϣ
VOID CPlazaViewServerCtrl::OnNcDestroy()
{
	__super::OnNcDestroy();

	//���ñ���
	m_bCreateFlag=false;
	m_wChairCount=0;
	m_wServerType=0;
	m_dwServerRule=0;
	m_dwUserRight=0;
	m_dwMasterRight=0;

	//�ӿڱ���
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pISelectedUserItem=NULL;
	m_pIGameLevelParser=NULL;
	m_pIPlazaUserManager=NULL;

	//�������
	m_MenuUserItemArray.RemoveAll();

	//���ٴ���
	m_ToolTipCtrl.DestroyWindow();
	m_UserListControl.DestroyWindow();

	//���ٴ���
	if(m_pTrumpetItem) m_pTrumpetItem->DestroyWindow();
	if(m_pExpressionControl) m_pExpressionControl->DestroyWindow();

	//�ͷŶ���
	SafeDelete(m_pTrumpetItem);	
	SafeDelete(m_pExpressionControl);
}

//λ����Ϣ
VOID CPlazaViewServerCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}


//�Ҽ��б�
VOID CPlazaViewServerCtrl::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//�����˵�
	if (pListNotify->iItem!=-1)
	{
		//ѡ����Ŀ
		UINT nSelectCount=m_UserListControl.GetSelectedCount();

		//ѡ����
		if (nSelectCount>1L)
		{
			//�û�����
			IClientUserItem * pIClientUserItem[MAX_WHISPER_USER];
			ZeroMemory(pIClientUserItem,sizeof(pIClientUserItem));

			//��������
			WORD wUserCount=0;
			POSITION nSelectPos=m_UserListControl.GetFirstSelectedItemPosition();

			//ѡ���ռ�
			while ((nSelectPos!=NULL)&&(wUserCount<CountArray(pIClientUserItem)))
			{
				//��ȡѡ��
				INT nSelectItem=m_UserListControl.GetNextSelectedItem(nSelectPos);

				//�����û�
				DWORD_PTR lItemData=m_UserListControl.GetItemData(nSelectItem);
				if (lItemData!=NULL) pIClientUserItem[wUserCount++]=((IClientUserItem *)(lItemData));
			};

			//�����˵�
			if (wUserCount>0) TrackUserItemMenu(pIClientUserItem,wUserCount);
		}
		else
		{
			//�����˵�
			DWORD_PTR lItemData=m_UserListControl.GetItemData(pListNotify->iItem);
			if (lItemData!=NULL) TrackUserItemMenu((IClientUserItem *)(lItemData));
		}
	}

	return;
}

//˫���б�
VOID CPlazaViewServerCtrl::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//��Ϣ����
	if ((pListNotify->iItem!=-1)/*&&(pListNotify->lParam!=NULL)*/)
	{
		//��ȡ����
		DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
		IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;

		//ִ�ж���
		if (pIClientUserItem!=NULL)
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//ִ�ж���
			switch (pParameterGlobal->m_cbActionLeftDoubleList)
			{
			case ACTION_ORIENTATION:			//��λ�û�
				{
					//��ȡ����
					WORD wTableID=pIClientUserItem->GetTableID();
					WORD wChairID=pIClientUserItem->GetChairID();
					BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

					//��λλ��
					if (wTableID!=INVALID_TABLE)
					{
						//��������
						CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
						CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
						ITableViewFrame * pITableViewFrame = pPlazaViewServer->GetTableViewFrame();

						//���ÿ���
						pITableViewFrame->VisibleTable(wTableID);

						//��������
						if (cbUserStatus==US_LOOKON)
						{
							pITableViewFrame->FlashGameTable(wTableID);
						}
						else
						{
							pITableViewFrame->FlashGameChair(wTableID,wChairID);
						}
					}

					break;
				}
			case ACTION_SEND_WHISPER:			//����˽��
				{
					//����˽��
					if ((pIClientUserItem!=m_pIMySelfUserItem))
					{
						//��������
						CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
						CPlazaViewServer* pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

						IClientUserItem * pIClientUserItemArray[]={pIClientUserItem};
						pPlazaViewServer->WhisperConversation(pIClientUserItemArray,CountArray(pIClientUserItemArray));
					}

					break;
				}
			case ACTION_SHOW_USER_INFO:		//�û���Ϣ
				{
					//ģ���Ҽ�
					OnNMRclickUserList(pNMHDR, pResult);
					break;
				}
			}
		}
	}

	return;
}

//�����б�
VOID  CPlazaViewServerCtrl::OnNMClickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//��Ϣ����
	if (pListNotify->iItem!=-1)
	{
		//��ȡ����
		DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
		IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;

		//ִ�ж���
		if (pIClientUserItem!=NULL)
		{
			//����ָ��
			m_pISelectedUserItem=NULL;

			//�����ж�
			if(pIClientUserItem->IsAndroidUser()==true && CMasterRight::CanManagerAndroid(m_dwMasterRight)==true) 
			{
				m_pISelectedUserItem=pIClientUserItem;			
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////