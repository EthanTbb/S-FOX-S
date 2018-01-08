#include "Stdafx.h"
#include "Resource.h"
#include "CameraControl.h"

//////////////////////////////////////////////////////////////////////////////////

#define IDC_CAMERA_WND				100									//窗口标识


//////////////////////////////////////////////////////////////////////////////////
const TCHAR* const szButtonCloseContorlName = TEXT("ButtonClose");
const TCHAR* const szButtonSaveContorlName = TEXT("ButtonSave");
const TCHAR* const szButtonRepeatControlName = TEXT("ButtonRepeat");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");
const TCHAR* const szLabelCameraTipControlName = TEXT("LabelCameraTip");

//构造函数
CCameraControl::CCameraControl(CWnd * pParent) : CFGuiDialog(IDD_DLG_CAMERA, pParent)
{
	//设置变量
	m_bInit=false;
	m_hWndCamera=NULL;

	//界面控件
	m_pLabelText=NULL;
	m_pbtOk=NULL;
	m_pbtRepeat=NULL;
	m_pbtCameraDict=NULL;

	//设置变量
	ZeroMemory(&m_CapStatus,sizeof(m_CapStatus));
	ZeroMemory(&m_CapTureparms,sizeof(m_CapTureparms));
	ZeroMemory(&m_CapDrivercaps,sizeof(m_CapDrivercaps));

	return;
}

//析构函数
CCameraControl::~CCameraControl()
{
}

//控件绑定
VOID CCameraControl::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	//其他控件
	DDX_Control(pDX, IDC_CAMERA_CONTROL, m_CameraControl);
}

//消息解释
BOOL CCameraControl::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CCameraControl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("视频拍摄"));

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//获取大小
	CRect rcClient;
	m_CameraControl.GetClientRect(&rcClient);

	//创建窗口
	DWORD dwStyle=WS_CHILD|WS_BORDER;
	m_hWndCamera=capCreateCaptureWindow(NULL,dwStyle,0,0,rcClient.Width(),rcClient.Height(),m_CameraControl,IDC_CAMERA_WND);

	//连接设备
	if (capDriverConnect(m_hWndCamera,0)==TRUE)
	{
		//获取性能
		capDriverGetCaps(m_hWndCamera,&m_CapDrivercaps,sizeof(m_CapDrivercaps));

		//结果判断
		if (m_CapDrivercaps.fCaptureInitialized==TRUE)
		{
			//设置变量
			m_bInit=true;

			//设置属性
			capPreview(m_hWndCamera,TRUE);
			capPreviewRate(m_hWndCamera,30);

			//显示窗口
			::ShowWindow(m_hWndCamera,SW_SHOW);

			//获取状态
			capGetStatus(m_hWndCamera,&m_CapStatus,sizeof(m_CapStatus));
		}
	}

	//设置控件
	if (m_bInit==true)
	{
		m_pbtCameraDict->SetEnabled(true);
		m_pLabelText->SetText(TEXT("摄像头初始化成功"));
	}
	else
	{
		//设置信息
		m_pLabelText->SetText(TEXT("没有安装摄像头或者摄像头初始化失败"));
	}

	return TRUE; 
}

//初始控件
void CCameraControl::InitControlUI()
{
	__super::InitControlUI();

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	m_pLabelText = (CLabelUI*)GetControlByName(szLabelCameraTipControlName);

	//保存按钮
	m_pbtOk = (CButtonUI*)GetControlByName(szButtonSaveContorlName);

	//重拍按钮
	m_pbtRepeat = (CButtonUI*)GetControlByName(szButtonRepeatControlName);

	//拍照按钮
	m_pbtCameraDict = (CButtonUI*)GetControlByName(szButtonTakeControlName);
}

//消息提醒
void CCameraControl::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (pControlUI==m_pbtOk)
		{
			return OnOK();
		}
		else if( pControlUI->GetName() == szButtonCloseContorlName)
		{
			return OnCancel();
		}
		else if (pControlUI==m_pbtRepeat)
		{
			return OnBnClickedRepeat();
		}
		else if (pControlUI==m_pbtCameraDict)
		{
			return OnBnClickedCameraDict();
		}
	}
}

//重拍按钮
VOID CCameraControl::OnBnClickedRepeat()
{
	//设置窗口
	capPreview(m_hWndCamera,TRUE);

	//设置按钮
	m_pbtOk->SetEnabled(false);
	m_pbtRepeat->SetEnabled(false);
	m_pbtCameraDict->SetEnabled(true);

	return;
}

//拍照按钮
VOID CCameraControl::OnBnClickedCameraDict()
{
	//截获图像
	capGrabFrame(m_hWndCamera);
	capEditCopy(m_hWndCamera);

	//拷贝图像
	if (OpenClipboard()==FALSE)
	{
		ASSERT(FALSE);
		return;
	}

	//销毁图像
	if (m_ImageResult.IsNull()==false)
	{
		m_ImageResult.Destroy();
	}

	//拷贝数据
	HBITMAP hImageResult=(HBITMAP)CopyImage(GetClipboardData(CF_BITMAP),IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	//结果处理
	if (hImageResult!=NULL)
	{
		//附加数据
		m_ImageResult.Attach(hImageResult);

		//清空剪切
		EmptyClipboard();
		CloseClipboard();

		//设置按钮
		m_pbtOk->SetEnabled(true);
		m_pbtRepeat->SetEnabled(true);
		m_pbtCameraDict->SetEnabled(false);

		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
