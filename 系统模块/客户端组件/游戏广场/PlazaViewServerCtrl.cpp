#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlazaViewServer.h"
#include "PlazaViewContainer.h"
#include "PlazaViewServerCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//界面定义
#define PROPERTY_CY					81									//游戏道具
#define BILL_VIEW_CY				81									//广告视图

//分割参数
#define INCISE_SCALE				45/100								//界面分割

//控件标识
#define IDC_PROPERTY				300									//游戏道具
#define IDC_CHAT_CONTROL			301									//聊天控制
#define IDC_WEB_BILL_PUBLICIZE		302									//浏览控件

//UserList距离
#define USERLIST_DISTANCETOP		16									//上端距离
#define USERLIST_DISTANCELEFT		5									//左端距离
#define USERLIST_DISTANCERIGHT		5									//右端距离
#define USERLIST_DISTANCEBOTTOM		6									//下端距离

//Chat距离
#define CHAT_DISTANCETOP		6									//上端距离
#define CHAT_DISTANCELEFT		5									//左端距离
#define CHAT_DISTANCERIGHT		5									//右端距离
#define CHAT_DISTANCEBOTTOM		3									//下端距离
//////////////////////////////////////////////////////////////////////////////////
//菜单命令

//颜色菜单
#define MAX_CHAT_COLOR				16									//最大数目
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//快捷短语
#define MAX_SHORT_COUNT				16									//最大数目
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//选择短语

//////////////////////////////////////////////////////////////////////////////////
//控件定义

//编辑控件
const TCHAR * szEditChatControlName=TEXT("EditChat");

//按钮控件
const TCHAR * szButtonChat1ControlName=TEXT("ButtonChat1");
const TCHAR * szButtonChat2ControlName=TEXT("ButtonChat2");
const TCHAR * szButtonChat3ControlName=TEXT("ButtonChat3");
const TCHAR * szButtonChat4ControlName=TEXT("ButtonChat4");
const TCHAR * szButtonSendChatControlName=TEXT("ButtonSendChat");
const TCHAR * szButtonChatShortControlName=TEXT("ButtonChatShort");
const TCHAR * szContainerChatInputControlName=TEXT("ContainerChatInput");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewServerCtrl, CFGuiDialog)

	//系统消息
	ON_WM_SIZE()

	//命令消息
	ON_COMMAND(IDM_LIMIT_USER_ROOM_CHAT, OnLimitUserRoomChat)
	ON_COMMAND(IDM_ALLOW_USER_ROOM_CHAT, OnAllowUserRoomChat)
	ON_COMMAND(IDM_LIMIT_USER_GAME_CHAT, OnLimitUserGameChat)
	ON_COMMAND(IDM_ALLOW_USER_GAME_CHAT, OnAllowUserGameChat)
	ON_COMMAND(IDM_LIMIT_USER_WHISP_CHAT, OnLimitUserWhispChat)
	ON_COMMAND(IDM_ALLOW_USER_WHISP_CHAT, OnAllowUserWhispChat)

	//控件消息
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_CONTROL, OnNMRclickUserList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST_CONTROL, OnNMDblclkUserList) 
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST_CONTROL, OnNMClickUserList)

	//聊天命令
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_SHORT, (IDM_SELECT_CHAT_SHORT+MAX_SHORT_COUNT), OnSelectChatShort)

	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CPlazaViewServerCtrl::CPlazaViewServerCtrl() : CFGuiDialog(IDD_GAME_SERVER_CONTROL)
{
	//界面变量
	m_bCreateFlag=false;
	m_wChairCount=0;
	m_wServerType=0;
	m_dwServerRule=0;
	m_dwUserRight=0L;
	m_dwTrumpetCount=0;
	m_dwTyphonCount=0;

	//聊天变量
	m_dwChatTime=0L;

	//控件变量
	m_pExpressionControl=NULL;
	m_pTrumpetItem=NULL;	

	//接口变量
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pISelectedUserItem=NULL;
	m_pIGameLevelParser=NULL;
	m_pIPlazaUserManager=NULL;	

	//聊天资源
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

	//公告栏资源
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

//析构函数
CPlazaViewServerCtrl::~CPlazaViewServerCtrl()
{
	//销毁窗口
	if(m_pTrumpetItem) m_pTrumpetItem->DestroyWindow();
	if(m_pExpressionControl) m_pExpressionControl->DestroyWindow();

	//释放对象
	SafeDelete(m_pTrumpetItem);	
	SafeDelete(m_pExpressionControl);

	return;
}

//接口查询
VOID * CPlazaViewServerCtrl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGamePropertySink,Guid,dwQueryVer);	
	QUERYINTERFACE(IGamePropertyUseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGamePropertySink,Guid,dwQueryVer);
	return NULL;
}

//初始控件
VOID CPlazaViewServerCtrl::InitControlUI()
{
	//设置控件
	CEditUI * pEditControl = static_cast<CEditUI *>(GetControlByName(szEditChatControlName));
	if(pEditControl)
	{
		/*pEditControl->SetBkColor(RGB(0,0,0));*/		
		pEditControl->SetMaxChar(LEN_USER_CHAT-1);
		//pEditControl->SetTextColor(ARGB(255,255,255,255));
	}

	//设置按钮
	CButtonUI* pButtonSendChat = static_cast<CButtonUI *>(GetControlByName(szButtonSendChatControlName));
	if(pButtonSendChat) pButtonSendChat->SetWantReturn();
}

//消息提醒
VOID CPlazaViewServerCtrl::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(),szButtonSendChatControlName) == 0)
		{
			//获取信息
			CString strMessage;
			CControlUI * pEditChat=GetControlByName(szEditChatControlName);
			if(pEditChat) strMessage=pEditChat->GetText();

			//格式消息
			strMessage.TrimLeft();
			strMessage.TrimRight();

			//发送信息
			if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
			{
				CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
				SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
				pEditChat->SetText(TEXT(""));
			}

			//设置焦点
			pEditChat->SetFocus();

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChatShortControlName) == 0)
		{
			//构造菜单
			CSkinMenu ChatShortMenu;
			ChatShortMenu.CreateMenu();

			//工作目录
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//构造路径
			TCHAR szFileName[MAX_PATH]=TEXT("");
			TCHAR szFileName2[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseInfo.ini"),szDirectory,AfxGetApp()->m_pszExeName);
			_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\PhraseInfo.ini"),szDirectory);

			//判断文件
			TCHAR *pFileName = szFileName;
			CFile file;
			if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
			if(file.m_hFile!=INVALID_HANDLE_VALUE) file.Close();
			ASSERT(pFileName);

			//变量定义
			BYTE cbIndex=1;
			bool bSuccess=false;
			TCHAR szItemName[16]=TEXT(""),szShortcut[LEN_USER_CHAT]=TEXT("");

			//读取信息
			while (true)
			{
				//读取信息
				_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),cbIndex);
				GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

				//结果判断
				if (szShortcut[0]!=0)
				{
					//设置变量
					cbIndex++;
					bSuccess=true;

					//加入菜单
					ChatShortMenu.AppendMenu(IDM_SELECT_CHAT_SHORT+cbIndex-1,szShortcut);
				}

				//结束判断
				if ((szShortcut[0]==0)||(cbIndex>=MAX_SHORT_COUNT))
				{
					break;
				}
			}

			//弹出菜单
			if (bSuccess==true)
			{
				//创建表情			
				CControlUI * pButtonChatShort=GetControlByName(szButtonChatShortControlName);
				if(pButtonChatShort==NULL) return;

				//变量定义
				CRect rcButton=pButtonChatShort->GetPos();
				ClientToScreen(&rcButton);
                
				//弹出菜单
				ChatShortMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);
			}

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat1ControlName) == 0)
		{
			//创建表情			
			CControlUI * pChatButton1=GetControlByName(szButtonChat1ControlName);
			if(pChatButton1==NULL) return;

			//变量定义
			CRect rcButton=pChatButton1->GetPos();
			ClientToScreen(&rcButton);

			//创建窗口
			if (m_pExpressionControl==NULL)
			{
				m_pExpressionControl=new CExpressionControl;
			}

			//显示窗口
			m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat2ControlName) == 0)
		{
			//获取对象
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//变量定义
			INT nWidth=72,nHeight=16;
			COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
				RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

			//构造颜色
			CImage ImageColor[CountArray(crColor)];
			for (INT i=0;i<CountArray(ImageColor);i++)
			{
				//创建位图
				ImageColor[i].Create(nWidth,nHeight,16);

				//获取设备
				CImageDC ImageDC(ImageColor[i]);
				CDC * pDC=CDC::FromHandle(ImageDC);

				//绘画位图
				pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
				pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
				pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
			}

			//创建菜单
			CSkinMenu ColorMenu;
			ColorMenu.CreateMenu();

			//构造菜单
			ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("选择其他颜色"));

			//颜色选项
			ColorMenu.AppendSeparator();
			for (INT i=0;i<CountArray(ImageColor);i++) 
			{
				ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
				if (pParameterGlobal->m_crChatTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
			}

			//查找对象
			CControlUI * pChatButton2=GetControlByName(szButtonChat2ControlName);
			if(pChatButton2==NULL) return;

			//弹出菜单
			CRect rcButton=pChatButton2->GetPos();
			ClientToScreen(&rcButton);
			ColorMenu.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat3ControlName) == 0)
		{
			//创建菜单
			CSkinMenu MenuMessage;
			MenuMessage.CreateMenu();

			//构造菜单
			MenuMessage.AppendMenu(IDM_MESSAGE_SHOW_ALL,TEXT("显示所有信息"));
			MenuMessage.AppendMenu(IDM_MESSAGE_HIDE_DETEST,TEXT("屏蔽厌恶信息"));
			MenuMessage.AppendMenu(IDM_MESSAGE_ONLY_FRIEND,TEXT("只显示好友信息"));

			//获取对象
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//设置菜单
			BYTE cbMessageMode=pParameterGlobal->m_cbMessageMode;
			if (cbMessageMode==MESSAGE_MODE_ALL) MenuMessage.CheckMenuItem(IDM_MESSAGE_SHOW_ALL,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_DETEST) MenuMessage.CheckMenuItem(IDM_MESSAGE_HIDE_DETEST,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_FRIEND) MenuMessage.CheckMenuItem(IDM_MESSAGE_ONLY_FRIEND,MF_BYCOMMAND|MF_CHECKED);

			//查找对象
			CControlUI * pChatButton3=GetControlByName(szButtonChat3ControlName);
			if(pChatButton3==NULL) return;

			//弹出菜单
			CRect rcButton=pChatButton3->GetPos();
			ClientToScreen(&rcButton);
			MenuMessage.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return;
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonChat4ControlName) == 0)
		{
			//变量定义
			CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
			CGamePropertyBase * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
			CGamePropertyBase * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

			//有效判断
			if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon==NULL)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("当前喇叭功能未启用！"),MB_ICONINFORMATION,30L);
				return;
			}

			//创建窗口
			if (m_pTrumpetItem==NULL)
			{
				m_pTrumpetItem=new CDlgTrumpet;
				m_pTrumpetItem->SetGameTrumpetSink(QUERY_ME_INTERFACE(IUnknownEx));
			}
			//设置喇叭数
			m_pTrumpetItem->SetTrumpet(m_dwTrumpetCount);
			m_pTrumpetItem->SetTyphon(m_dwTyphonCount);

			//显示窗口
			m_pTrumpetItem->ShowTrumpetWindow(AfxGetMainWnd(),m_pIMySelfUserItem);

			return;
		}
	}
}

//开始绘画
void CPlazaViewServerCtrl::OnBeginPaintWindow(HDC hDC)
{
	//获取对象
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//用户列表
	CRect rcListArea;
	rcListArea.left=0;
	rcListArea.top=0;
	rcListArea.right=rcClient.Width();
	rcListArea.bottom=rcClient.Height()*INCISE_SCALE;		
	m_UserListEncircle.DrawEncircleFrame(pDC,rcListArea);

	//绘画聊天
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

//使用道具
bool CPlazaViewServerCtrl::OnEventUseProperty(WORD wPropertyIndex)
{
	//发送喇叭
	if(wPropertyIndex==PROPERTY_ID_TRUMPET || wPropertyIndex==PROPERTY_ID_TYPHON)
	{
		//构造结果
		CMD_GR_C_SendTrumpet SendTrumpet;
		SendTrumpet.cbRequestArea=PT_ISSUE_AREA_SERVER;
		SendTrumpet.wPropertyIndex=wPropertyIndex;
		SendTrumpet.TrumpetColor=m_pTrumpetItem->GetTrumpetColor();
		m_pTrumpetItem->GetTrumpetContent(SendTrumpet.szTrumpetContent);

		//发送消息
		m_pITCPSocket->SendData(MDM_GR_PROPERTY,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet));
	}

	return true;
}

//显示背包
bool CPlazaViewServerCtrl::PerformShowBag()
{
	return CGlobalUnits::GetInstance()->PerformShowBag();
}

//显示商城
bool CPlazaViewServerCtrl::PerformShowShop()
{
	return CGlobalUnits::GetInstance()->PerformShowShop();
}


//列表配置
VOID CPlazaViewServerCtrl::SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//设置列表
	m_UserListControl.SetColumnDescribe(ColumnItem,cbColumnCount);
}

//显示菜单
VOID CPlazaViewServerCtrl::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//判断状态
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true) return;

	//设置变量
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.Add(pIClientUserItem);

	//构造菜单
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//变量定义
	TCHAR szMenuString[256]=TEXT("");
	bool bMeUserItem=(pIClientUserItem==m_pIMySelfUserItem);

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//变量定义
	LPCTSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//用户信息
	WORD wTableID=pUserInfo->wTableID;
	WORD wChairID=pUserInfo->wChairID;
	BYTE cbUserStatus=pUserInfo->cbUserStatus;

	//玩家信息
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//插入名片
	CImage ImageUserCard;
	pUserItemElement->ConstructNameCard(pIClientUserItem,m_pIGameLevelParser,ImageUserCard);
	if (ImageUserCard.IsNull()==false) UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,ImageUserCard,MF_GRAYED);

	//插入分割
	UserInfoMenu.AppendSeparator();

	//常规菜单
	if (bMeUserItem==false)
	{
		UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("发送私聊消息 ..."));
		UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);
	}
	UserInfoMenu.AppendMenu(IDM_COPY_USER_NICKNAME,TEXT("复制昵称"));

	//设置交谈
	if ((bMeUserItem==false)&&(pIChatTargetItem!=m_pIMySelfUserItem)&&(pIChatTargetItem!=pIClientUserItem))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("与 [ %s ] 交谈"),pIClientUserItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_SET_CHAT_USER,szMenuString);
	}

	//取消交谈
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("取消与 [ %s ] 交谈"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//操作菜单
	if ((bMeUserItem==false)&&(cbMeUserStatus!=US_PLAYING)&&(m_wServerType&GAME_GENRE_MATCH)==0)
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//旁观游戏
		bool bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("旁观 [ %s ] 游戏"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_LOOKON_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//一起游戏
		bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_SIT)||(cbUserStatus==US_READY)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("坐到 [ %s ] 的游戏桌"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_PLAY_GAME_TOGETHER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//邀请游戏
		bEnableMenu=((wMeTableID!=INVALID_TABLE)&&(cbMeUserStatus!=US_LOOKON)&&(cbUserStatus!=US_PLAYING)&&(cbUserStatus!=US_OFFLINE));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("邀请 [ %s ] 一起玩游戏"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_INVITE_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);
	}

	//好友管理
	if (bMeUserItem==false&&(m_wServerType&GAME_GENRE_MATCH)==0)
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//加入菜单
		BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
		UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("设置为好友"),(cbCompanion==CP_FRIEND)?MF_CHECKED:0);
		UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("设置为厌恶"),(cbCompanion==CP_DETEST)?MF_CHECKED:0);
	}

	//管理菜单
	if ((bMeUserItem==false)&&(m_pIMySelfUserItem->GetMasterOrder()!=0))
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//管理菜单
		bool bPlaying = (pIClientUserItem->GetUserStatus()==US_PLAYING);
		UserInfoMenu.AppendMenu(IDM_KILL_USER,TEXT("踢用户下线..."),(CMasterRight::CanKillUser(m_dwMasterRight)==false && !bPlaying)?MF_GRAYED:MF_ENABLED);
		
		//玩家管理
		UserInfoMenu.AppendSeparator();
		UINT nMenuFlags=(CMasterRight::CanLimitRoomChat(m_dwMasterRight)==false)?MF_GRAYED:MF_ENABLED;
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_ROOM_CHAT,TEXT("禁止该用户大厅聊天"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_ROOM_CHAT,TEXT("允许该用户大厅聊天"),nMenuFlags);

		nMenuFlags=(CMasterRight::CanLimitGameChat(m_dwMasterRight)==false)?MF_GRAYED:MF_ENABLED;
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_GAME_CHAT,TEXT("禁止该用户游戏聊天"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_GAME_CHAT,TEXT("允许该用户游戏聊天"),nMenuFlags);

		nMenuFlags=(CMasterRight::CanLimitWisper(m_dwMasterRight)==false)?MF_GRAYED:MF_ENABLED;
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_WHISP_CHAT,TEXT("禁止该用户私聊"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_WHISP_CHAT,TEXT("允许该用户私聊"),nMenuFlags);
	}

	//功能菜单
	if((bMeUserItem==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0) && (CUserRight::CanKillOutUser(m_dwUserRight)==true)&&(m_wServerType&GAME_GENRE_MATCH)==0)
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//功能菜单
		TCHAR szText[32]=TEXT("");
		_sntprintf(szText,CountArray(szText),TEXT("踢 [%s] 下线"),pIClientUserItem->GetNickName());
		bool bPlaying = (pIClientUserItem->GetUserStatus()==US_PLAYING);		
		bool bUnEnableMenu =  (m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder()) && !bPlaying;
		UserInfoMenu.AppendMenu(IDM_KILL_USER,szText,bUnEnableMenu?MF_GRAYED:MF_ENABLED);
	}

	////插入分割
	//UserInfoMenu.AppendSeparator();

	//UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("查找在线用户 ..."));

	////插入分割
	//UserInfoMenu.AppendSeparator();

	//玩家位置
	if (wTableID!=INVALID_TABLE)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("正在 [ %d ] 号游戏桌"),wTableID+1);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//积分信息
	DWORD dwPlayCount=pIClientUserItem->GetUserPlayCount();
	FLOAT fWinRate=pIClientUserItem->GetUserWinRate();
	FLOAT fUserFleeRate=pIClientUserItem->GetUserFleeRate();
	FLOAT fUserDrawRate=pIClientUserItem->GetUserDrawRate();
	_sntprintf(szMenuString,CountArray(szMenuString),TEXT("成绩：") SCORE_STRING TEXT(" 胜率：%.2f%% 逃跑率：%.2f%%"),
		pUserInfo->lScore,fWinRate,fUserFleeRate);
	UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

	//社团信息
	if (pUserInfo->szGroupName[0]!=0)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("游戏社团：%s"),pUserInfo->szGroupName);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//备注信息
	if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("备注信息：%s"),pszUserNote);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}
	//使用道具
	if(bMeUserItem == false)
	{
		UserInfoMenu.AppendMenu(IDM_USE_PROPERTY, TEXT("使用道具"), 0);
	}

	//弹出菜单
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//显示菜单
VOID CPlazaViewServerCtrl::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount)
{
	//效验参数
	ASSERT((pIClientUserItem!=NULL)&&(wItemCount>0));
	if ((pIClientUserItem==NULL)||(wItemCount==0)) return;

	//判断状态
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true) return;

	//设置变量
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.SetSize(wItemCount);
	CopyMemory(m_MenuUserItemArray.GetData(),pIClientUserItem,wItemCount*sizeof(IClientUserItem *));

	//构造菜单
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//变量定义
	TCHAR szMenuString[256]=TEXT("");
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//玩家信息
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//发送私聊
	UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("发送群聊消息 ..."),MF_GRAYED);
	UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);

	//聊天对象
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("取消与 [ %s ] 交谈"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//插入分割
	UserInfoMenu.AppendSeparator();

	//加入菜单
	UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("设置为好友"));
	UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("设置为厌恶"));

	//插入分割
	UserInfoMenu.AppendSeparator();

	//功能菜单
	UserInfoMenu.AppendMenu(IDM_USER_INFO_MANAGER,TEXT("用户信息管理 ..."));
	//UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("查找在线用户 ..."));

	//弹出菜单
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//用户进入
VOID CPlazaViewServerCtrl::OnEventUserEnter(IClientUserItem * pIClientUserItem)
{
	//设置自己
	if (m_pIMySelfUserItem==NULL)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();

		//自己判断
		if (pGlobalUserInfo->GetGlobalUserData()->dwUserID==pIClientUserItem->GetUserID())
		{
			//设置变量
			m_pIMySelfUserItem=pIClientUserItem;

			//设置界面
			m_UserListControl.SetMySelfUserID(m_pIMySelfUserItem->GetUserID());
			m_UserListControl.SetServerRule(m_dwServerRule);
		}
	}

	//插入用户
	m_UserListControl.InsertDataItem(pIClientUserItem);

	//变量定义
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//提示信息
	if(pParameterGlobal->m_bNotifyUserInOut==true)
	{
		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}
	return;
}

//用户离开
VOID CPlazaViewServerCtrl::OnEventUserLeave(IClientUserItem * pIClientUserItem)
{
	//变量定义
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();

	//删除用户
	m_UserListControl.DeleteDataItem(pIClientUserItem);
	m_ChatControl.DeleteUserItem(pIClientUserItem);

	//菜单对象
	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	{
		//获取用户
		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
		if (pIChatUserItem->GetUserID()==pIClientUserItem->GetUserID()) m_MenuUserItemArray.RemoveAt(i);
	}	
	
	//变量定义
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//提示信息
	if(pParameterGlobal->m_bNotifyUserInOut==true)
	{
		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
	}

	return;
}

//用户状态
VOID CPlazaViewServerCtrl::OnEventUserUpdate(IClientUserItem * pIClientUserItem)
{
	//更新状态
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	return;
}
//购买道具
bool CPlazaViewServerCtrl::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//效验参数
	ASSERT((pszNickName!=NULL)&&(wItemCount>0));
	if ((pszNickName==NULL)||(wItemCount==0)) return false;

	//查找对象
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

	//执行购买
	return pPlazaViewServer->PerformBuyProperty(PT_ISSUE_AREA_SERVER,pszNickName,wItemCount,wPropertyIndex);
}

//用户选择
VOID CPlazaViewServerCtrl::OnChangeChatTarget(IClientUserItem * pIClientUserItem)
{
	//设置变量
	if (pIClientUserItem!=NULL)
	{
		//设置界面
		if (m_ChatControl.m_hWnd==NULL)
		{
			//创建窗口
			CRect rcCreate(0,0,0,0);
			m_ChatControl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT_CONTROL);

			//调整窗口
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}
	else
	{
		//设置界面
		if (m_ChatControl.m_hWnd!=NULL)
		{
			//销毁窗口
			m_ChatControl.DestroyWindow();

			//调整界面
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//设置焦点
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();
}

//表情事件
VOID CPlazaViewServerCtrl::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	//设置焦点
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	//发送表情
	if (EfficacyUserChat(NULL,pExpression->GetIndex())==true)
	{
		SendExpressionPacket(m_ChatControl.GetChatTargetUserID(),pExpression->GetIndex());
	}
}


bool CPlazaViewServerCtrl::OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage)
{
	//防作弊场
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
	{
		TCHAR szMessage[128]=TEXT("防作弊场不允许赠送礼物！");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	//检查用户
	IClientUserItem * pIClientUserItem=m_pIPlazaUserManager->SearchUserByNickName(pszNickName);
	if(pIClientUserItem==NULL)
	{
		TCHAR szMessage[128]=TEXT("您指定的使用对象不在本房间中或不存在！");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	return true;
}

//道具喇叭
VOID CPlazaViewServerCtrl::OnEventPropertyTrumpet()
{
	//更新界面
	if(m_pTrumpetItem!=NULL) m_pTrumpetItem->UpdateControlSurface();
}

//进入事件
VOID CPlazaViewServerCtrl::SetServerInfo(WORD wChairCount,WORD wServerType,DWORD dwServerRule,DWORD dwUserRight,DWORD dwMasterRight)
{
	//设置变量
	m_wChairCount = wChairCount;
	m_wServerType = wServerType;
	m_dwServerRule = dwServerRule;
	m_dwUserRight = dwUserRight;
	m_dwMasterRight = dwMasterRight;	

	//变量定义
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
	CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

	//房间验证
	ASSERT(pPlazaViewServer!=NULL);
	if(pPlazaViewServer==NULL) return;

	//查询接口
	m_pITCPSocket = pPlazaViewServer->GetTCPSocket();
	m_pIPlazaUserManager=pPlazaViewServer->GetPlazaUserManager();
	m_pIGameLevelParser=pPlazaViewServer->GetGameLevelParser();

	//设置接口
	m_UserListControl.SetServerRule(m_dwServerRule);
	m_UserListControl.SetGameLevelParser(m_pIGameLevelParser);

	//设置控件
	m_UserListControl.DeleteAllItems();

	//调整界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//控件绑定
VOID CPlazaViewServerCtrl::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//常规控件
	DDX_Control(pDX, IDC_USER_LIST_CONTROL, m_UserListControl);

	//聊天控件
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);

	return;
}

//配置函数
BOOL CPlazaViewServerCtrl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_bCreateFlag=true;

	//设置窗口
	CSize SizeWindow(m_PaintManager.GetInitSize());
	SetWindowPos(NULL, 0, 0, SizeWindow.cx, SizeWindow.cy, SWP_NOZORDER|SWP_NOMOVE);

	//初始化控件
	InitSilderControl();

	//变量定义
	HINSTANCE hInstance= AfxGetResourceHandle();

	//聊天控件
	m_ChatControl.SetChatControlSink(this);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(50,30,17));

	//设置控件
	m_ChatMessage.SetBackgroundColor(FALSE,RGB(50,30,17));

	return TRUE;
}

//消息过虑
BOOL CPlazaViewServerCtrl::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	//按键消息
	if (pMsg->message==WM_KEYDOWN)
	{
		//取消按钮
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlazaViewServerCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDM_MESSAGE_SHOW_ALL:		//显示所有
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_ALL;

			return TRUE;
		}
	case IDM_MESSAGE_HIDE_DETEST:	//屏蔽厌恶
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_DETEST;

			return TRUE;
		}
	case IDM_MESSAGE_ONLY_FRIEND:	//显示好友
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_FRIEND;

			return TRUE;
		}
	case IDM_SET_CHAT_USER:			//交谈对象
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//设置聊天
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem!=NULL) m_ChatControl.SetChatTargetUser(pIClientUserItem);

			return TRUE;
		}
	case IDM_CANCEL_CHAT_USER:		//取消交谈
		{
			//设置聊天
			m_ChatControl.SetChatTargetUser(NULL);

			return TRUE;
		}
	case IDM_CREATE_ISER_WISPER:	//发送私聊
		{
			//变量定义
			CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
			CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

			//创建私聊
			WORD wUserCount=(WORD)m_MenuUserItemArray.GetCount();
			pPlazaViewServer->WhisperConversation(m_MenuUserItemArray.GetData(),wUserCount);

			return TRUE;
		}
	case IDM_COPY_USER_NICKNAME:	//拷贝昵称
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//拷贝字符
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			CWHService::SetClipboardString(pIClientUserItem->GetNickName());

			//设置字符
			CControlUI * pEditChat=GetControlByName(szEditChatControlName);
			if(pEditChat)
			{
				pEditChat->SetText(pIClientUserItem->GetNickName());
				pEditChat->SetFocus();
			}

			return TRUE;
		}
	
	case IDM_SEE_USER_LOCATION:		//查看位置
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//隐藏信息
			bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMasterOrder()==0)) return FALSE;

			//获取用户
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//变量定义
			CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
			CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
			ITableViewFrame * pITableViewFrame = pPlazaViewServer->GetTableViewFrame();

			//获取属性
			WORD wTableID=pIClientUserItem->GetTableID();
			WORD wChairID=pIClientUserItem->GetChairID();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

			//定位位置
			if (wTableID!=INVALID_TABLE)
			{
				//设置可视
				/*ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
				CWndViewItemCtrl::GetInstance()->ActiveViewItem(this);*/

				//设置可视
				pITableViewFrame->VisibleTable(wTableID);

				//闪动椅子
				if (cbUserStatus==US_LOOKON)
				{
					pITableViewFrame->FlashGameTable(wTableID);
				}
				else
				{
					pITableViewFrame->FlashGameChair(wTableID,wChairID);
				}
			}

			//变量定义
			LVFINDINFO LVFindInfo;
			ZeroMemory(&LVFindInfo,sizeof(LVFindInfo));

			//设置变量
			LVFindInfo.flags=LVFI_PARAM;
			LVFindInfo.lParam=(LPARAM)pIClientUserItem;

			//定位列表
			INT nItem=m_UserListControl.FindItem(&LVFindInfo);

			//设置列表
			if (nItem!=-1L)
			{
				m_UserListControl.EnsureVisible(nItem,FALSE);
				m_UserListControl.SetItemState(nItem,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
			}

			return TRUE;
		}
	case IDM_KILL_USER:             //踢用户下线
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//获取用户
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//隐藏信息
			bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMasterOrder()==0)&&
				(m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder())) return FALSE;

			//获取属性
			WORD wTableID=pIClientUserItem->GetTableID();
			WORD wChairID=pIClientUserItem->GetChairID();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
			if(cbUserStatus==US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("该玩家已开始游戏，不能踢！"),MB_ICONINFORMATION);
				return TRUE;
			}

			//变量定义
			CMD_GR_KickUser KickUser;
			KickUser.dwTargetUserID=pIClientUserItem->GetUserID();

			//发送数据
			if(m_pIMySelfUserItem->GetMasterOrder()!=0)
			{
				m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_KILL_USER,&KickUser,sizeof(KickUser));
			}
			else
			{
				//变量定义
				CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
				CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
				ITableViewFrame * pITableViewFrame = pPlazaViewServer->GetTableViewFrame();

				//百人游戏
				if(pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("很抱歉，百人游戏不许踢人！"),MB_ICONQUESTION);
					return TRUE;
				}

				m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_KICK_USER,&KickUser,sizeof(KickUser));
			}

			return TRUE;
		}

	case IDM_LOOKON_USER:           //旁观游戏
		{
			//获取对象
			CPlazaViewContainer * pPlazaViewContainer=CPlazaViewContainer::GetInstance();
			CPlazaViewServer * pPlazaViewServer=(CPlazaViewServer *)pPlazaViewContainer->GetViewItemByArea(VIA_Center);

			//状态判断
			if (pPlazaViewServer==NULL) return TRUE;			
			if (pPlazaViewServer->GetServiceStatus()!=ServiceStatus_ServiceIng) return TRUE;

			//百人游戏
			if (m_pIMySelfUserItem->GetMasterOrder()==0 && m_wChairCount>=MAX_CHAIR)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("百人游戏不允许旁观！"),MB_ICONINFORMATION);
				return TRUE;
			}

			//过虑消息
			if (m_pIMySelfUserItem->GetUserStatus()==US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("正在进行游戏，请结束游戏后再进行旁观！"),MB_ICONINFORMATION);
				return TRUE;
			}

			//寻找玩家
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem==NULL) return TRUE;

			//玩家状态判断
			if(pIClientUserItem->GetUserStatus()!=US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("该玩家已不在游戏中！"),MB_ICONINFORMATION);
				return TRUE;
			}

			//旁观动作
			pPlazaViewServer->PerformLookonAction(pIClientUserItem->GetTableID(),pIClientUserItem->GetChairID());

			return TRUE;
		}
	case IDM_PLAY_GAME_TOGETHER:    //一起游戏
		{
			//获取对象
			CPlazaViewContainer * pPlazaViewContainer=CPlazaViewContainer::GetInstance();
			CPlazaViewServer * pPlazaViewServer=(CPlazaViewServer *)pPlazaViewContainer->GetViewItemByArea(VIA_Center);

			//状态判断
			if (pPlazaViewServer==NULL) return TRUE;
			if (pPlazaViewServer->GetServiceStatus()!=ServiceStatus_ServiceIng) return TRUE;

			//过虑消息
			if (m_pIMySelfUserItem->GetUserStatus()==US_PLAYING) return TRUE;

			//寻找玩家
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem==NULL) return TRUE;

			//玩家状态判断
			if(pIClientUserItem->GetTableID()==INVALID_TABLE)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("该玩家已不在桌子上！"),MB_ICONINFORMATION);
				return TRUE;
			}

			//获取空位
			WORD wChairID=INVALID_CHAIR;
			ITableViewFrame * pITableViewFrame=pPlazaViewServer->GetTableViewFrame();
			WORD wNullCount=pITableViewFrame->GetNullChairCount(pIClientUserItem->GetTableID(),wChairID);
			if (wNullCount==0) 
			{
				CInformation Information2(this);
				Information2.ShowMessageBox(TEXT("此游戏桌已经没有空位置了！"),MB_ICONINFORMATION);
				return true;
			}

			//坐下动作
			pPlazaViewServer->PerformSitDownAction(pIClientUserItem->GetTableID(),wChairID,false);

			return TRUE;
		}
	case IDM_INVITE_USER:           //邀请游戏
		{
			//状态判断
			//if (m_ServiceStatus!=ServiceStatus_ServiceIng) return TRUE;

			//过虑消息
			if (m_pIMySelfUserItem->GetUserStatus()==US_PLAYING) return TRUE;
			if (m_pIMySelfUserItem->GetTableID()==INVALID_TABLE) return TRUE;

			//寻找玩家
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem==NULL) return TRUE;

			//发送命令
			CMD_GR_UserInviteReq UserInviteReq;
			UserInviteReq.wTableID=m_pIMySelfUserItem->GetTableID();
			UserInviteReq.dwUserID=pIClientUserItem->GetUserID();
			m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_INVITE_REQ,&UserInviteReq,sizeof(UserInviteReq));

			//提示消息
			m_ChatMessage.InsertSystemString(TEXT("成功发送用户邀请命令"));

			return TRUE;
		}
	case IDM_SET_FRIEND_USER:		//设为好友
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//变量定义
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

			//变量定义
			tagUserAttrib UserAttrib;
			UserAttrib.cbCompanion=cbDestCompanion;

			//变量定义
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//设置关系
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);
			m_pIPlazaUserManager->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

			return TRUE;
		}
	case IDM_SET_DETEST_USER:		//设为厌恶
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//变量定义
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

			//变量定义
			tagUserAttrib UserAttrib;
			UserAttrib.cbCompanion=cbDestCompanion;

			//变量定义
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//设置关系
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);
			m_pIPlazaUserManager->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

			return TRUE;
		}
	case IDM_USE_PROPERTY:		//使用道具
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;
			//变量定义
			IClientUserItem* pIClientUserItem=m_MenuUserItemArray[0];

			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowPropertyUse(pIClientUserItem);
			return TRUE;
		}
	//case IDM_KICK_USER:             //踢出用户
	//	{
	//		//效验状态
	//		ASSERT(m_MenuUserItemArray.GetCount()>0);
	//		if (m_MenuUserItemArray.GetCount()==0) return TRUE;

	//		//变量定义
	//		IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//		//隐藏信息
	//		bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_pIClientKernel->GetServerAttribute()->dwServerRule);
	//		if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder())) return FALSE;

	//		//获取属性
	//		BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	//		if(cbUserStatus==US_PLAYING)
	//		{
	//			CInformation Information(this);
	//			Information.ShowMessageBox(TEXT("该玩家已开始游戏，不能踢！"),MB_ICONINFORMATION);
	//			return TRUE;
	//		}
	//		
	//		//变量定义
	//		IPC_GF_KickUser KickUser;
	//		KickUser.dwTargetUserID=pIClientUserItem->GetUserID();

	//		//发送数据
	//		m_pIClientKernel->SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_KICK_USER,&KickUser,sizeof(KickUser));
	//	}
	}

	return __super::OnCommand(wParam,lParam);
}

//调整控件
VOID CPlazaViewServerCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//环绕信息
	tagEncircleInfo EncircleInfoList;
	m_UserListEncircle.GetEncircleInfo(EncircleInfoList);

	//用户列表
	CRect rcListArea;
	rcListArea.left=0;
	rcListArea.top=0;
	rcListArea.right=nWidth;
	rcListArea.bottom=nHeight*INCISE_SCALE;		

	//绘画聊天
	CRect rcChatArea;
	rcChatArea.left=0;
	rcChatArea.top=rcListArea.bottom;
	rcChatArea.right=nWidth;
	rcChatArea.bottom=nHeight;	

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//调整距离
	CRect rcListAreaModify;
	rcListAreaModify.left=0 - USERLIST_DISTANCELEFT;
	rcListAreaModify.top=0 - USERLIST_DISTANCETOP;
	rcListAreaModify.right=nWidth +USERLIST_DISTANCERIGHT;
	rcListAreaModify.bottom=nHeight*INCISE_SCALE + USERLIST_DISTANCEBOTTOM;		

	//用户列表
	m_UserListEncircle.DeferWindowPos(&m_UserListControl,hDwp,rcListAreaModify);
	
	//聊天控制
	if ((m_ChatControl.m_hWnd!=NULL)&&(m_ChatControl.GetChatTargetUserItem()!=NULL))
	{
		//获取位置
		CRect rcChatControl;
		m_ChatControl.GetWindowRect(&rcChatControl);

		//位置信息
		tagEncircleInfo EncircleInfo;
		m_ChatEncircle.GetEncircleInfo(EncircleInfo);

		//聊天控件
		rcChatArea.bottom-=rcChatControl.Height();
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatArea);

		//聊天控制
		DeferWindowPos(hDwp,m_ChatControl,NULL,rcChatArea.left+EncircleInfo.nLBorder,rcChatArea.bottom-EncircleInfo.nBBorder,
			rcChatArea.Width()-EncircleInfo.nLBorder-EncircleInfo.nRBorder,rcChatControl.Height(),uFlags);
	}
	else
	{
		//调整距离
		CRect rcChatAreaModify;
		rcChatAreaModify.left=rcChatArea.left - CHAT_DISTANCELEFT;
		rcChatAreaModify.top=rcChatArea.top - CHAT_DISTANCETOP;
		rcChatAreaModify.right=rcChatArea.right +CHAT_DISTANCERIGHT;
		rcChatAreaModify.bottom=rcChatArea.bottom + CHAT_DISTANCEBOTTOM;		

		//聊天控件
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatAreaModify);
	}

	//移动结束
	EndDeferWindowPos(hDwp);




	return;
}

//设置列表
VOID CPlazaViewServerCtrl::InitSilderControl()
{
	//构造变量
	tagColorUserList ColorUserList;
	ZeroMemory(&ColorUserList,sizeof(ColorUserList));

	//颜色定义
	ColorUserList.crSelectTX=COLOR_SELECT_TX;
	ColorUserList.crSelectBK=COLOR_SELECT_BK;
	ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//颜色定义
	ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	ColorUserList.crMasterTX=COLOR_MASTER_TX;
	ColorUserList.crMasterBK=COLOR_MASTER_BK;
	ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	ColorUserList.crMemberBK=COLOR_MEMBER_BK;
	ColorUserList.crAndroidTX=COLOR_ANDROID_TX;
	ColorUserList.crAndroidBK=COLOR_ANDROID_BK;

	//设置控件
	m_UserListControl.SetColorUserList(ColorUserList);
}


//聊天效验
bool CPlazaViewServerCtrl::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	//状态判断
	if (m_pIMySelfUserItem==NULL) return false;

	//长度比较
	if(pszChatString != NULL)
	{
		CString strChat=pszChatString;
		if(strChat.GetLength() >= (LEN_USER_CHAT/2))
		{
			m_ChatMessage.InsertString(TEXT("抱歉，您输入的聊天内容过长，请缩短后再发送！\r\n"),COLOR_WARN);
			return false;
		}
	}

	//变量定义
	BYTE cbMemberOrder=m_pIMySelfUserItem->GetMemberOrder();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//房间配置
	if (CServerRule::IsForfendGameChat(m_dwServerRule)&&(cbMasterOrder==0))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，当前此游戏房间禁止用户房间聊天！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanRoomChat(m_dwUserRight)==false)
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"));

		return false;
	}

	//速度判断
	DWORD dwCurrentTime=(DWORD)time(NULL);
	if ((cbMasterOrder==0)&&((dwCurrentTime-m_dwChatTime)<=TIME_USER_CHAT))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("您的说话速度太快了，请坐下来喝杯茶休息下吧！"));

		return false;
	}

	//设置变量
	m_dwChatTime=dwCurrentTime;
	m_strChatString=pszChatString;

	return true;
}

//发送聊天
bool CPlazaViewServerCtrl::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//构造信息
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=crColor;	
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);
	UserChat.dwSendUserID=m_pISelectedUserItem?m_pISelectedUserItem->GetUserID():m_pIMySelfUserItem->GetUserID();

	//发送命令
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//发送表情
bool CPlazaViewServerCtrl::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//变量定义
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造信息
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;
	UserExpression.dwSendUserID=m_pISelectedUserItem?m_pISelectedUserItem->GetUserID():m_pIMySelfUserItem->GetUserID();

	//发送命令
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}


//禁止用户大厅聊天
VOID CPlazaViewServerCtrl::OnLimitUserRoomChat()
{
	//效验状态
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//获取用户
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//变量定义
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//设置变量
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_CHAT;
	LimitUserChat.cbLimitValue = TRUE;

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//允许用户大厅聊天
VOID CPlazaViewServerCtrl::OnAllowUserRoomChat()
{
	//效验状态
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//获取用户
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//变量定义
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//设置变量
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_CHAT;
	LimitUserChat.cbLimitValue = FALSE;

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//禁止用户游戏聊天
VOID CPlazaViewServerCtrl::OnLimitUserGameChat()
{
	//效验状态
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//获取用户
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//变量定义
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//设置变量
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_GAME_CHAT;
	LimitUserChat.cbLimitValue = TRUE;

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//允许用户游戏聊天
VOID CPlazaViewServerCtrl::OnAllowUserGameChat()
{
	//效验状态
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//获取用户
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//变量定义
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//设置变量
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_GAME_CHAT;
	LimitUserChat.cbLimitValue = FALSE;

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//禁止用户私聊
VOID CPlazaViewServerCtrl::OnLimitUserWhispChat()
{
	//效验状态
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//获取用户
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//变量定义
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//设置变量
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_WISPER;
	LimitUserChat.cbLimitValue = TRUE;

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//允许用户私聊
VOID CPlazaViewServerCtrl::OnAllowUserWhispChat()
{
	//效验状态
	ASSERT(m_MenuUserItemArray.GetCount()>0);
	if (m_MenuUserItemArray.GetCount()==0) return;

	//获取用户
	IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

	//变量定义
	CMD_GR_LimitUserChat LimitUserChat = {0};

	//设置变量
	LimitUserChat.dwTargetUserID = pIClientUserItem->GetUserID();
	LimitUserChat.cbLimitFlags = OSF_ROOM_WISPER;
	LimitUserChat.cbLimitValue = FALSE;

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_LIMIT_USER_CHAT,&LimitUserChat,sizeof(LimitUserChat));

	return;
}

//更多颜色
VOID CPlazaViewServerCtrl::OnSelectMoreColor()
{
	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//设置颜色
	CColorDialog ColorDialog(pParameterGlobal->m_crChatTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crChatTX=ColorDialog.GetColor();

	//设置界面
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	return;
}

//选择颜色
VOID CPlazaViewServerCtrl::OnSelectChatColor(UINT nCommandID)
{
	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//选择颜色
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crChatTX=crColor[nIndex];

	//设置焦点
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	return;
}

//选择短语
VOID CPlazaViewServerCtrl::OnSelectChatShort(UINT nCommandID)
{
	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_SHORT;

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	TCHAR szFileName2[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseInfo.ini"),szDirectory,AfxGetApp()->m_pszExeName);
	_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\PhraseInfo.ini"),szDirectory);

	//判断文件
	TCHAR *pFileName = szFileName;
	CFile file;
	if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
	if(file.m_hFile!=INVALID_HANDLE_VALUE) file.Close();
	ASSERT(pFileName);

	//变量定义
	TCHAR szItemName[16]=TEXT("");
	TCHAR szShortcut[LEN_USER_CHAT]=TEXT("");

	//读取信息
	_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),nIndex);
	GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

	//发送消息
	if (szShortcut[0]!=0)
	{
		//获取信息
		CString strMessage=szShortcut;
		strMessage.TrimLeft();strMessage.TrimRight();

		//发送信息
		if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
		{
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
		}
	}

	//设置焦点
	CControlUI * pEditChat=GetControlByName(szEditChatControlName);
	if(pEditChat) pEditChat->SetFocus();

	return;
}

//销毁消息
VOID CPlazaViewServerCtrl::OnNcDestroy()
{
	__super::OnNcDestroy();

	//设置变量
	m_bCreateFlag=false;
	m_wChairCount=0;
	m_wServerType=0;
	m_dwServerRule=0;
	m_dwUserRight=0;
	m_dwMasterRight=0;

	//接口变量
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pISelectedUserItem=NULL;
	m_pIGameLevelParser=NULL;
	m_pIPlazaUserManager=NULL;

	//数组变量
	m_MenuUserItemArray.RemoveAll();

	//销毁窗口
	m_ToolTipCtrl.DestroyWindow();
	m_UserListControl.DestroyWindow();

	//销毁窗口
	if(m_pTrumpetItem) m_pTrumpetItem->DestroyWindow();
	if(m_pExpressionControl) m_pExpressionControl->DestroyWindow();

	//释放对象
	SafeDelete(m_pTrumpetItem);	
	SafeDelete(m_pExpressionControl);
}

//位置消息
VOID CPlazaViewServerCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}


//右键列表
VOID CPlazaViewServerCtrl::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//弹出菜单
	if (pListNotify->iItem!=-1)
	{
		//选择数目
		UINT nSelectCount=m_UserListControl.GetSelectedCount();

		//选择处理
		if (nSelectCount>1L)
		{
			//用户数组
			IClientUserItem * pIClientUserItem[MAX_WHISPER_USER];
			ZeroMemory(pIClientUserItem,sizeof(pIClientUserItem));

			//变量定义
			WORD wUserCount=0;
			POSITION nSelectPos=m_UserListControl.GetFirstSelectedItemPosition();

			//选择收集
			while ((nSelectPos!=NULL)&&(wUserCount<CountArray(pIClientUserItem)))
			{
				//获取选择
				INT nSelectItem=m_UserListControl.GetNextSelectedItem(nSelectPos);

				//插入用户
				DWORD_PTR lItemData=m_UserListControl.GetItemData(nSelectItem);
				if (lItemData!=NULL) pIClientUserItem[wUserCount++]=((IClientUserItem *)(lItemData));
			};

			//弹出菜单
			if (wUserCount>0) TrackUserItemMenu(pIClientUserItem,wUserCount);
		}
		else
		{
			//弹出菜单
			DWORD_PTR lItemData=m_UserListControl.GetItemData(pListNotify->iItem);
			if (lItemData!=NULL) TrackUserItemMenu((IClientUserItem *)(lItemData));
		}
	}

	return;
}

//双击列表
VOID CPlazaViewServerCtrl::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//消息处理
	if ((pListNotify->iItem!=-1)/*&&(pListNotify->lParam!=NULL)*/)
	{
		//获取变量
		DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
		IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;

		//执行动作
		if (pIClientUserItem!=NULL)
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//执行动作
			switch (pParameterGlobal->m_cbActionLeftDoubleList)
			{
			case ACTION_ORIENTATION:			//定位用户
				{
					//获取属性
					WORD wTableID=pIClientUserItem->GetTableID();
					WORD wChairID=pIClientUserItem->GetChairID();
					BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

					//定位位置
					if (wTableID!=INVALID_TABLE)
					{
						//变量定义
						CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
						CPlazaViewServer * pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();
						ITableViewFrame * pITableViewFrame = pPlazaViewServer->GetTableViewFrame();

						//设置可视
						pITableViewFrame->VisibleTable(wTableID);

						//闪动椅子
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
			case ACTION_SEND_WHISPER:			//发送私聊
				{
					//创建私聊
					if ((pIClientUserItem!=m_pIMySelfUserItem))
					{
						//变量定义
						CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();
						CPlazaViewServer* pPlazaViewServer = pPlatrformFrame->GetPlazaViewServer();

						IClientUserItem * pIClientUserItemArray[]={pIClientUserItem};
						pPlazaViewServer->WhisperConversation(pIClientUserItemArray,CountArray(pIClientUserItemArray));
					}

					break;
				}
			case ACTION_SHOW_USER_INFO:		//用户信息
				{
					//模拟右键
					OnNMRclickUserList(pNMHDR, pResult);
					break;
				}
			}
		}
	}

	return;
}

//单击列表
VOID  CPlazaViewServerCtrl::OnNMClickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//消息处理
	if (pListNotify->iItem!=-1)
	{
		//获取变量
		DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
		IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;

		//执行动作
		if (pIClientUserItem!=NULL)
		{
			//设置指针
			m_pISelectedUserItem=NULL;

			//机器判断
			if(pIClientUserItem->IsAndroidUser()==true && CMasterRight::CanManagerAndroid(m_dwMasterRight)==true) 
			{
				m_pISelectedUserItem=pIClientUserItem;			
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////