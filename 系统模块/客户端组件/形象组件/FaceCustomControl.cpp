#include "StdAfx.h"
#include "Resource.h"
#include "CameraControl.h"
#include "FaceCustomControl.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_BT_TURN_LEFT			100									//旋转按钮
#define IDC_BT_TURN_RIGHT			101									//旋转按钮
#define IDC_BT_SHOW_VLINE			102									//控制按钮
#define IDC_BT_LOAD_IMAGE			103									//加载图片
#define IDC_BT_LOAD_CAMERA			104									//加载摄像

//控件标识
#define IDC_IMAGE_EDIT_CTRL			200									//编辑控件

//颜色定义
#define CR_BACK_GROUND				RGB(189,173,139)					//背景颜色

//////////////////////////////////////////////////////////////////////////////////
const TCHAR* const szTabLayoutCustomControlName = TEXT("TabLayoutCustom");
const TCHAR* const szButtonLocalControlName = TEXT("ButtonLocal"); 
const TCHAR* const szButtonSelfieControlName = TEXT("ButtonSelfie"); 
const TCHAR* const szButtonCWControlName = TEXT("ButtonCW");
const TCHAR* const szButtonCCWControlName = TEXT("ButtonCCW");
const TCHAR* const szButtonCornerControlName = TEXT("ButtonCorner");
const TCHAR* const szButtonReturnControlName = TEXT("ButtonReturn");


BEGIN_MESSAGE_MAP(CFaceItemCustomWnd, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFaceItemCustomWnd::CFaceItemCustomWnd():m_pTabLayoutCustom(NULL)
{
	//界面控件
	m_pbtTurnLeft=NULL;
	m_pbtTurnRight=NULL;
	m_pbtShowVLine=NULL;
	m_pbtLoadImage=NULL;
	m_pbtLoadCamera=NULL;
}

//析构函数
CFaceItemCustomWnd::~CFaceItemCustomWnd()
{
}

//加载判断
bool CFaceItemCustomWnd::IsNull()
{
	return m_ImageEditorControl.IsNull();
}

//重置图片
VOID CFaceItemCustomWnd::DestoryImage()
{
	//状态判断
	if (m_ImageEditorControl.IsNull()==false)
	{
		//设置控件
		m_pbtTurnLeft->SetVisible(false);
		m_pbtTurnRight->SetVisible(false);
		m_pbtShowVLine->SetVisible(false);

		//重置图片
		m_ImageEditorControl.DestoryImage();
		m_ImageEditorControl.ShowWindow(SW_HIDE);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	}

	return;
}

//初始控件
VOID CFaceItemCustomWnd::InitControlUI()
{
	__super::InitControlUI();

	m_pTabLayoutCustom = (CTabLayoutUI*)GetControlByName(szTabLayoutCustomControlName);

	//左转按钮
	m_pbtTurnLeft = (CButtonUI*)GetControlByName(szButtonCWControlName);
	if(m_pbtTurnLeft)m_pbtTurnLeft->SetVisible(false);

	//右转按钮
	m_pbtTurnRight = (CButtonUI*)GetControlByName(szButtonCCWControlName);
	if(m_pbtTurnRight)m_pbtTurnRight->SetVisible(false);

	//虚线按钮
	m_pbtShowVLine = (CButtonUI*)GetControlByName(szButtonCornerControlName);
	if(m_pbtShowVLine) m_pbtShowVLine->SetVisible(false);

	//加载图片
	m_pbtLoadImage = (CButtonUI*)GetControlByName(szButtonLocalControlName);

	//开启摄像
	m_pbtLoadCamera = (CButtonUI*)GetControlByName(szButtonSelfieControlName);

	m_bLoadCustom = false;
}

//消息提醒
VOID CFaceItemCustomWnd::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (pControlUI==m_pbtTurnLeft)
		{
			return OnBnClickedTurnLeft();
		}
		else if (pControlUI==m_pbtTurnRight)
		{
			return OnBnClickedTurnRight();
		}
		else if (pControlUI==m_pbtShowVLine)
		{
			return OnBnClickedShowVLine();
		}
		else if (pControlUI==m_pbtLoadImage)
		{
			return OnBnClickedLoadImage();
		}
		else if (pControlUI==m_pbtLoadCamera)
		{
			return OnBnClickedLoadCamera();
		}
		else if( pControlUI->GetName() == szButtonReturnControlName )
		{
			if(m_pTabLayoutCustom) m_pTabLayoutCustom->SelectItem(0);
			//设置控件
			m_ImageEditorControl.DestoryImage();
			m_ImageEditorControl.ShowWindow(SW_HIDE);
		}
	}
}

//绘画消息
VOID CFaceItemCustomWnd::OnBeginPaintWindow(HDC hDC)
{
	return;
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),CR_BACK_GROUND);

	//加载图片
	CPngImage ImageFaceEditBack;
	ImageFaceEditBack.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("FACE_EDIT_BACK"));

	//绘画背景
	ImageFaceEditBack.DrawImage(pDC,30,70);

	//获取对象
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(pSkinResourceManager->GetDefaultFont());

	//绘画文字
	if (m_ImageEditorControl.IsWindowVisible()==FALSE)
	{
		pDC->TextOut(60,150,TEXT("您可以选择以下一种方式设置头像:"));
		pDC->TextOut(60,180,TEXT("本地照片：选择一张本地图片编辑后上传为头像。"));
		pDC->TextOut(60,205,TEXT("自拍头像：通过摄像头拍照编辑后上传为头像。"));
	}

	return;
}

//设置接口
VOID CFaceItemCustomWnd::SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent)
{
	//设置接口
	m_ImageEditorControl.SetImageEditorEvent(pIImageEditorEvent);

	return;
}

//获取图片
VOID CFaceItemCustomWnd::DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//获取图片
	m_ImageEditorControl.DrawEditImage(pDC,nXPos,nYPos,nWidth,nHeight);

	return;
}

//位置信息
VOID CFaceItemCustomWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//移动控件
	if( ::IsWindow( m_ImageEditorControl.GetSafeHwnd() ) )
	{
		SIZE szEditor={451, 276};
		m_ImageEditorControl.SetWindowPos(NULL,(cx-szEditor.cx)/2,12,szEditor.cx,szEditor.cy,SWP_NOZORDER);
	}
	return;
}

//建立消息
INT CFaceItemCustomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//变量定义
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=GetModuleHandle(AVATAR_CONTROL_DLL_NAME);

	//创建控件
	m_ImageEditorControl.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_IMAGE_EDIT_CTRL);

	return 0;
}

//旋转图像
VOID CFaceItemCustomWnd::OnBnClickedTurnLeft()
{
	//选择图片
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_LEFT);

	return;
}

//旋转图像
VOID CFaceItemCustomWnd::OnBnClickedTurnRight()
{
	//选择图片
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_RIGHT);

	return;
}

//显示虚线
VOID CFaceItemCustomWnd::OnBnClickedShowVLine()
{
	//设置虚线
	bool bShowVLine=m_ImageEditorControl.GetVLineStatus();
	m_ImageEditorControl.SetVLineStatus((bShowVLine==false));

	return;
}

//加载图像
VOID CFaceItemCustomWnd::OnBnClickedLoadImage()
{
	//变量定义
	CFileDialog FileDialog(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,TEXT("图像文件（*.BMP *.PNG *.JPG）|*.BMP;*.PNG;*JPG||"),this);

	//打开文件
	if (FileDialog.DoModal()==IDOK)
	{
		//加载图片
		if (m_ImageEditorControl.LoadEditImage(FileDialog.GetPathName())==false)
		{
			//设置按钮
			m_pbtTurnLeft->SetVisible(false);
			m_pbtTurnRight->SetVisible(false);
			m_pbtShowVLine->SetVisible(false);

			//设置控件
			m_ImageEditorControl.ShowWindow(SW_HIDE);

			//提示信息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("图片加载失败，请选择其他图片再试！"),MB_ICONERROR,0);
		}
		else
		{
			//设置按钮
			m_pbtTurnLeft->SetVisible(true);
			m_pbtTurnRight->SetVisible(true);
			m_pbtShowVLine->SetVisible(true);
			if(m_pTabLayoutCustom) m_pTabLayoutCustom->SelectItem(1);
			//设置控件
			m_ImageEditorControl.ShowWindow(SW_SHOW);

			m_bLoadCustom = true;
		}
	}

	return;
}

//加载图像
VOID CFaceItemCustomWnd::OnBnClickedLoadCamera()
{
	//变量定义
	CCameraControl CameraControl(this);

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(AVATAR_CONTROL_DLL_NAME));

	//捕获图片
	if (CameraControl.DoModal()==IDOK)
	{
		//加载图片
		m_ImageEditorControl.ShowWindow(SW_SHOW);

		//加载图片
		ASSERT(CameraControl.m_ImageResult.IsNull()==false);
		bool bSuccess=m_ImageEditorControl.LoadEditImage(CameraControl.m_ImageResult);

		//设置按钮
		m_pbtTurnLeft->SetEnabled(bSuccess);
		m_pbtTurnRight->SetEnabled(bSuccess);
		m_pbtShowVLine->SetEnabled(bSuccess);

		m_bLoadCustom = true;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
