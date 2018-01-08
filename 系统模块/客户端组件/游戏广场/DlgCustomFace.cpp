#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomFace.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////

//编辑模式
#define MM_SYSTEM					0									//系统模式
#define MM_CUSTOM					1									//自定模式

//类型定义
#define FACE_SELECT_ITEM			0									//选择头像
#define FACE_CUSTOM_EDIT			1									//自定头像
#define FACE_CUSTOM_VIDEO			2									//自拍头像

//控件标识
#define IDC_FACE_CUSTOM_WND			100									//自定头像
#define IDC_FACE_SELECT_WND			101									//选择头像

//////////////////////////////////////////////////////////////////////////////////
//基础控件
const TCHAR* const szAvatarPreviewControlName = TEXT("AvatarPreview");

//按钮
const TCHAR* const szButtonOkControlName = TEXT("ButtonOk");
const TCHAR* const szButtonCancelControlName = TEXT("ButtonCancel");
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonLocalControlName = TEXT("ButtonLocal");
const TCHAR* const szButtonSelfieControlName = TEXT("ButtonSelfie");

//容器
const TCHAR* const szTileLayoutSymAvatarControlName = TEXT("TileLayoutSymAvatar");
const TCHAR* const szTabLayoutAvatarControlName = TEXT("TabLayoutAvatar");


BEGIN_MESSAGE_MAP(CDlgCustomFace, CFGuiDialog)
	//ON_NOTIFY(TCN_SELCHANGE,IDC_TAB_FACE,OnTcnSelAlterFaceMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCustomFace::CDlgCustomFace() : CFGuiDialog(IDD_DLG_COSTOM_FACE)
,m_pTileLayoutSysAvatar(NULL)
,m_pAvatarPreview(NULL)
{
	//设置变量
	m_pICustomFaceEvent=NULL;

	//功能按钮
	m_pbtOk=NULL;
	m_pbtCancel=NULL;

	//设置变量
	m_cbMode=0;
	m_wFaceID=INVALID_WORD;
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));
	ZeroMemory(&m_CustomFaceInfo96,sizeof(m_CustomFaceInfo96));

	//任务组件
	m_MissionManager.InsertMissionItem(this);

	//设置接口
	m_FaceItemCustomWnd.SetImageEditorEvent(this);
	//m_FaceItemSelectWnd.SetFaceItemSelectEvent(this);

	return;
}

//析构函数
CDlgCustomFace::~CDlgCustomFace()
{
}

//图像事件
VOID CDlgCustomFace::OnEventFaceSelect(WORD wFaceID)
{
	//设置变量
	m_wFaceID=wFaceID;

	//设置模式
	m_cbMode=MM_SYSTEM;

	//设置控件
	//m_FaceItemCustomWnd.DestoryImage();

	//更新界面
	//RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//图像事件
VOID CDlgCustomFace::OnEventImageSelect(bool bLoadImage, bool bOperateing)
{
	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//控件绑定
VOID CDlgCustomFace::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//创建函数
BOOL CDlgCustomFace::OnInitDialog()
{
	__super::OnInitDialog();

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置标题
	SetWindowText(TEXT("更换头像"));

	//创建窗口
	CRect rcCreate(0,0,0,0);
	m_FaceItemCustomWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_FACE_CUSTOM_WND);

	CTabLayoutUI* pTabLayoutAvatar = (CTabLayoutUI*)GetControlByName(szTabLayoutAvatarControlName);
	if(pTabLayoutAvatar != NULL){
		CRect rcCtrl = pTabLayoutAvatar->GetPos();
		//调整位置
		m_FaceItemCustomWnd.SetWindowPos(NULL,rcCtrl.left,rcCtrl.top,rcCtrl.Width(),rcCtrl.Height(),SWP_NOZORDER);
		m_FaceItemCustomWnd.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//初始控件
void CDlgCustomFace::InitControlUI()
{
	__super::InitControlUI();

	//变量定义
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	CLabelUI * pLabelControl=NULL;

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//确定按钮
	m_pbtOk = (CButtonUI*)GetControlByName(szButtonOkControlName);

	//取消按钮
	m_pbtCancel = (CButtonUI*)GetControlByName(szButtonCancelControlName);

	m_pTileLayoutSysAvatar = (CTileLayoutUI*)GetControlByName(szTileLayoutSymAvatarControlName);
	m_pAvatarPreview = (CImageUI*)GetControlByName(szAvatarPreviewControlName);

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	m_wFaceID=pGlobalUserData->wFaceID;
	CopyMemory(&m_CustomFaceInfo,&pGlobalUserData->CustomFaceInfo,sizeof(m_CustomFaceInfo));

	LoadSysAvatar();

	//加载头像
	if(pGlobalUserData->CustomFaceInfo.dwDataSize != 0)
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID,m_CustomFaceInfo96);
	}

	if(pGlobalUserData->CustomFaceInfo.dwDataSize != NULL)
	{
		UpdateAvatarPreview(m_CustomFaceInfo96.dwCustomFace, BIGFACE_CX*BIGFACE_CY);
	}
	else
	{
		UpdateAvatarPreview(pGlobalUserData->wFaceID);
	}
}

//消息提醒
void CDlgCustomFace::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//确认
		if (pControlUI==m_pbtOk)
		{
			return OnOK();
		}
		//取消
		else if (pControlUI==m_pbtCancel)
		{
			return OnCancel();
		}
		//关闭
		else if( pControlUI->GetName() == szButtonCloseControlName)
		{
			return OnCancel();
		}
	}
	//选择头像
	else if( msg.sType == TEXT("selectchanged") )
	{
		if(pControlUI->GetParent() != NULL && m_pTileLayoutSysAvatar != NULL)
		{	
			int iIndex = m_pTileLayoutSysAvatar->GetItemIndex(pControlUI->GetParent());
			if(iIndex != -1)
			{
				m_wFaceID = iIndex ;
				UpdateAvatarPreview(m_wFaceID);
			}
		}
	}
	//模式切换
	else if( msg.sType == TEXT("tabselect") )
	{
		if( pControlUI->GetName() == szTabLayoutAvatarControlName )
		{
			m_cbMode = (BYTE)msg.wParam;
			m_FaceItemCustomWnd.ShowWindow( (m_cbMode==MM_CUSTOM)?SW_SHOW:SW_HIDE );
			if(m_cbMode == MM_CUSTOM)
			{
				//设置焦点
				m_FaceItemCustomWnd.SetFocus();
				m_FaceItemCustomWnd.SetActiveWindow();
			}
		}
	}
}

//绘画标题
VOID CDlgCustomFace::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//资源句柄
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//加载标题
	CPngImage ImageTitle;
	ImageTitle.LoadImage(hInstance,TEXT("CHANGE_FACE_TITLE"));

	//绘制标题
	ImageTitle.DrawImage(pDC,10,10);
}

//绘画函数
void CDlgCustomFace::OnEndPaintWindow(HDC hDC)
{
	if( m_cbMode == MM_CUSTOM && ::IsWindow(m_FaceItemCustomWnd.GetSafeHwnd()) && !m_FaceItemCustomWnd.IsNull() )
	{
		CRect rcCtrl(0,0,BIGFACE_CX,BIGFACE_CY);
		if(m_pAvatarPreview != NULL)
		{
			rcCtrl = m_pAvatarPreview->GetPos();
		}
		CDC* pDC = CDC::FromHandle(hDC);
		m_FaceItemCustomWnd.DrawEditImage(pDC,rcCtrl.left,rcCtrl.top,rcCtrl.Width(),rcCtrl.Height());
	}
	return;
}

//连接事件
bool CDlgCustomFace::OnEventMissionLink(INT nErrorCode)
{
	//错误判断
	if (nErrorCode==0L)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//发送消息 
		switch (m_cbMode)
		{
		case MM_SYSTEM:		//系统模式
			{
				//变量定义
				CMD_GP_SystemFaceInfo SystemFaceInfo;
				ZeroMemory(&SystemFaceInfo,sizeof(SystemFaceInfo));

				//设置变量
				SystemFaceInfo.wFaceID=m_wFaceID;
				SystemFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineIDEx(SystemFaceInfo.szMachineID);
				CopyMemory(SystemFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(SystemFaceInfo.szPassword));

				//发送数据
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(SystemFaceInfo));

				break;
			}
		case MM_CUSTOM:		//自定模式
			{
				//变量定义
				CMD_GP_CustomFaceInfo CustomFaceInfo;
				ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

				//设置变量
				CustomFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineIDEx(CustomFaceInfo.szMachineID);
				CopyMemory(CustomFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(CustomFaceInfo.szPassword));
				CopyMemory(CustomFaceInfo.dwCustomFace,m_CustomFaceInfo.dwCustomFace,sizeof(CustomFaceInfo.dwCustomFace));

				//发送数据
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_CUSTOM_FACE_INFO,&CustomFaceInfo,sizeof(CustomFaceInfo));

				break;
			}
		}

		return true;
	}
	else
	{
		//显示控件
		EnableControl(true);

		//显示提示
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("无法连接服务器，头像修改失败，请检查网络是否畅通！"),MB_ICONERROR);
	}

	return true;
}

//关闭事件
bool CDlgCustomFace::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//显示控件
		EnableControl(true);

		//显示提示
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("由于当前服务器处理繁忙，上传自定义头像失败，请稍后再重试！"),MB_ICONERROR);
	}

	return true;
}

//读取事件
bool CDlgCustomFace::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//操作成功
	if ((Command.wMainCmdID==MDM_GP_USER_SERVICE)&&(Command.wSubCmdID==SUB_GP_USER_FACE_INFO))
	{
		//效验参数
		ASSERT(wDataSize==sizeof(CMD_GP_UserFaceInfo));
		if (wDataSize!=sizeof(CMD_GP_UserFaceInfo)) return false;

		//消息处理
		CMD_GP_UserFaceInfo * pUserFaceInfo=(CMD_GP_UserFaceInfo *)pData;

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//保存数据
		if (pUserFaceInfo->dwCustomID==0)
		{
			//事件通知
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventSystemFace(pGlobalUserData->dwUserID,pUserFaceInfo->wFaceID);
		}
		else
		{
			//保存数据
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo.dwCustomFace);
			pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo96.dwCustomFace, BIGFACE_CX*BIGFACE_CY);

			//事件通知
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo);
		}

		//关闭窗口
		EndDialog(IDOK);

		return true;
	}

	return true;
}

//确定函数
VOID CDlgCustomFace::OnOK()
{
	//连接判断
	bool bConnect=false;

	//自定模式
	if (m_cbMode==MM_CUSTOM && m_FaceItemCustomWnd.m_bLoadCustom)
	{		
		//创建缓冲
		CImage ImageCustomFace;
		ImageCustomFace.Create(FACE_CX,FACE_CY,32);

		//创建 DC
		CImageDC BufferDC(ImageCustomFace);
		m_FaceItemCustomWnd.DrawEditImage(CDC::FromHandle(BufferDC),0,0,FACE_CX,FACE_CY);

		//获取数据
		INT nImagePitch=ImageCustomFace.GetPitch();
		LPBYTE cbBitCustomFace=(LPBYTE)ImageCustomFace.GetBits();

		//创建区域
		for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
		{
			for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
			{
				//设置颜色
				DWORD dwImageTarget=nYImagePos*nImagePitch+nXImagePos*4L;
				m_CustomFaceInfo.dwCustomFace[nYImagePos*FACE_CX+nXImagePos]=*(COLORREF *)(cbBitCustomFace+dwImageTarget);
			}
		}

		//设置变量
		bConnect=true;
		m_CustomFaceInfo.dwDataSize=sizeof(m_CustomFaceInfo);


		//96*96
		//创建缓冲
		CImage ImageCustomFace96;
		ImageCustomFace96.Create(BIGFACE_CX,BIGFACE_CY,32);
		//创建 DC
		CImageDC BufferDC2(ImageCustomFace96);
		m_FaceItemCustomWnd.DrawEditImage(CDC::FromHandle(BufferDC2),0,0,BIGFACE_CX,BIGFACE_CY);

		//获取数据
		INT nImagePitch2=ImageCustomFace96.GetPitch();
		LPBYTE cbBitCustomFace2=(LPBYTE)ImageCustomFace96.GetBits();

		//创建区域
		for (INT nYImagePos=0;nYImagePos<BIGFACE_CY;nYImagePos++)
		{
			for (INT nXImagePos=0;nXImagePos<BIGFACE_CX;nXImagePos++)
			{
				//设置颜色
				DWORD dwImageTarget=nYImagePos*nImagePitch2+nXImagePos*4L;
				m_CustomFaceInfo96.dwCustomFace[nYImagePos*BIGFACE_CX+nXImagePos]=*(COLORREF *)(cbBitCustomFace2+dwImageTarget);
			}
		}

		m_CustomFaceInfo96.dwDataSize=sizeof(m_CustomFaceInfo96);

	}

	//系统模式
	if (m_cbMode==MM_SYSTEM)
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		bConnect=(pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID);
	}

	//是否更改
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	if (!m_FaceItemCustomWnd.m_bLoadCustom && pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID)
	{
		m_cbMode=MM_SYSTEM;
		bConnect=(pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID);
	}

	//激活任务
	if (bConnect==true)
	{
		//控件控制
		EnableControl(false);

		//激活任务
		m_MissionManager.AvtiveMissionItem(this,false);

		return;
	}

	__super::OnOK();
}

//设置接口
bool CDlgCustomFace::SetCustomFaceEvent(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent)!=NULL);
		m_pICustomFaceEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent);

		//成功判断
		if (m_pICustomFaceEvent==NULL) return false;
	}
	else m_pICustomFaceEvent=NULL;

	return true;
}

//控件控制
VOID CDlgCustomFace::EnableControl(bool bEnableControl)
{
	//设置控件
	m_pbtOk->SetEnabled((bEnableControl==true)?TRUE:FALSE);
	//m_TabControl.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	//m_FaceItemCustomWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	//m_FaceItemSelectWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);

	return;
}


bool CDlgCustomFace::UpdateAvatarPreview(const WORD wFaceID)
{
	if( wFaceID == INVALID_WORD ) return false;
	if( m_pAvatarPreview == NULL ) return false;
	TCHAR szBuffer[64]={0};
	//wsprintf(szBuffer, TEXT("AVATAR_96_96\\Avatar%d.png"), wFaceID);
	wsprintf(szBuffer, TEXT("file=\'AVATAR_96_96\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
	m_pAvatarPreview->SetImage(szBuffer);
	return true;
}

bool CDlgCustomFace::UpdateAvatarPreview(DWORD* dwCustomFace, DWORD dwSize)
{
	if( m_pAvatarPreview == NULL ) return false;
	m_pAvatarPreview->SetCustomImage(dwCustomFace, dwSize);
	return true;
}

bool CDlgCustomFace::LoadSysAvatar()
{
	if(m_pTileLayoutSysAvatar == NULL) return false;

	TCHAR szBuffer[64]={0};
	for(int i = 0; i < m_pTileLayoutSysAvatar->GetCount(); i++)
	{
		CContainerUI* pItemAvatar = static_cast<CContainerUI*>(m_pTileLayoutSysAvatar->GetItemAt(i));
		CControlUI* pAvatar = pItemAvatar->GetItemAt(0);
		WORD wFaceID = i;
		wsprintf(szBuffer, TEXT("file=\'AVATAR_48_48\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
		pAvatar->SetBkImage(szBuffer);
		if (m_wFaceID == i)
		{
			COptionUI* pSelect = (COptionUI*)pItemAvatar->GetItemAt(1);
			pSelect->Selected(true);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
