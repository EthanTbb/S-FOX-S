#include "StdAfx.h"
#include "Resource.h"
#include "SkinDialog.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//间距大小
#define ROUND_XSPACE				3									//横向间距
#define ROUND_YSPACE				3									//纵向间距

//阴影定义
#define SHADOW_CX					4									//阴影宽度
#define SHADOW_CY					4									//阴影高度

//按钮标识
#define BST_CLOSE					0									//关闭按钮
#define BST_MAX_SIZE				1									//还原按钮
#define BST_MIN_SIZE				2									//缩小按钮

//按钮状态
#define BUT_EXIST					0x01								//按钮存在
#define BUT_NORMAL					0x02								//按钮正常
#define BUT_DISABLE					0x04								//按钮禁止

//////////////////////////////////////////////////////////////////////////////////
//过渡变量

//时间标识
#define IDI_TRANSITION				10									//过渡标识
#define TIME_TRANSITION				30									//过渡时间

//常量定义
#define MIN_TRANSITION_INDEX		0									//过渡索引
#define MAX_TRANSITION_INDEX		8									//过渡索引

//////////////////////////////////////////////////////////////////////////////////

//枚举结构
struct tagEnumChildInfo
{
	//CWnd *							pWndControl;						//控制窗口
	HWND							hWndControl;						//窗口句柄
	CWnd *							pWndLayered;						//分层窗口
	CRgn *							pRegionResult;						//结果区域
};

//////////////////////////////////////////////////////////////////////////////////

//枚举结构
struct tagEnumChildParam
{
	//CWnd *							pWndControl;						//控制窗口
	HWND							hWndControl;						//窗口句柄
	CFont *							pFont;						        //控制字体
};

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CSkinDialogAttribute				CSkinDialog::m_SkinAttribute;		//资源变量
CSkinLayeredMap						CSkinLayered::m_SkinLayeredMap;		//分层列表

//////////////////////////////////////////////////////////////////////////////////

//常量定义
const TCHAR * strWndClassName = TEXT("PerrySkinWnd");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinLayered, CWnd)
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinDialog, CFGuiDialog)
	
	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()

	//自定消息
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinDialogAttribute::CSkinDialogAttribute()
{
	//设置颜色
	m_crTitleText=RGB(0,0,0);
	m_crTitleFrame=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	m_crControlText=RGB(0,0,0);
	m_crLabelRound=RGB(0,0,0);

	//变量定义
	tagEncircleResource EncircleResource;
	HINSTANCE hResInstance=GetModuleHandle(SKIN_CONTROL_DLL_NAME);

	//环绕资源
	EncircleResource.pszImageTL=TEXT("SKIN_WND_TL");
	EncircleResource.pszImageTM=TEXT("SKIN_WND_TM");
	EncircleResource.pszImageTR=TEXT("SKIN_WND_TR");
	EncircleResource.pszImageML=TEXT("SKIN_WND_ML");
	EncircleResource.pszImageMR=TEXT("SKIN_WND_MR");
	EncircleResource.pszImageBL=TEXT("SKIN_WND_BL");
	EncircleResource.pszImageBM=TEXT("SKIN_WND_BM");
	EncircleResource.pszImageBR=TEXT("SKIN_WND_BR");
	m_PNGEncircleFrame.InitEncircleResource(EncircleResource,hResInstance);

	//获取大小
	CBitImage ImageButtonClose;
	ImageButtonClose.LoadFromResource(hResInstance,IDB_SKIN_WND_CLOSE);
	m_SizeButton.SetSize(ImageButtonClose.GetWidth()/5,ImageButtonClose.GetHeight());

	//设置视图
	m_PNGEncircleFrame.GetEncircleInfo(m_EncircleInfoFrame);

	//设置视图
	m_EncircleInfoView.nBBorder=8;
	m_EncircleInfoView.nTBorder=m_EncircleInfoFrame.nTBorder;
	m_EncircleInfoView.nLBorder=m_EncircleInfoFrame.nLBorder;
	m_EncircleInfoView.nRBorder=m_EncircleInfoFrame.nRBorder;

	//创建字体
	if (m_Font.GetSafeHandle()==NULL)
	{
		m_Font.CreateFont(-12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET,  
			OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,  
			DEFAULT_PITCH, TEXT("宋体"));
	}

	return;
}
	
//析构函数
CSkinDialogAttribute::~CSkinDialogAttribute()
{
	//销毁资源
	if (m_brBackGround.GetSafeHandle()!=NULL)
	{
		m_brBackGround.DeleteObject();
	}

	//销毁资源
	if (m_brLabelRound.GetSafeHandle()!=NULL)
	{
		m_brLabelRound.DeleteObject();
	}

	//销毁资源
	if (m_Font.GetSafeHandle()!=NULL)
	{
		m_Font.DeleteObject();
	}

	return;
}

//配置资源
bool CSkinDialogAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//标题颜色
	m_crTitleText=RGB(0,0,0);
	m_crTitleFrame=RGB(211,198,170);

	//控件颜色
	m_crControlText=RGB(10,10,10);
	m_crBackGround=RGB(136,77,32);
	m_crLabelRound=RGB(189,173,139);

	//设置资源
	m_brBackGround.DeleteObject();
	m_brBackGround.CreateSolidBrush(m_crBackGround);

	//设置资源
	m_brLabelRound.DeleteObject();
	m_brLabelRound.CreateSolidBrush(m_crLabelRound);	

	//创建字体
	if (m_Font.GetSafeHandle()==NULL)
	{
		m_Font.CreateFont(-12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET,  
			OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,  
			DEFAULT_PITCH, TEXT("宋体"));
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinLayered::CSkinLayered()
{	
	//设置变量	
	m_hwndControl=NULL;
	m_OriParentProc=NULL;

	return;
}

//析构函数
CSkinLayered::~CSkinLayered()
{
}

//创建窗口
bool CSkinLayered::CreateLayered(HWND hWnd)
{
	//效验参数
	ASSERT(hWnd!=NULL);
	if(hWnd==NULL) return false;

	//设置变量
	m_hwndControl=hWnd;

	//设置变量
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//变量定义
	WNDCLASSEX ShadowClass;
	memset(&ShadowClass, 0, sizeof(ShadowClass));

	//设置类型
	ShadowClass.cbSize = sizeof(WNDCLASSEX); 
	ShadowClass.style  = CS_HREDRAW | CS_VREDRAW;
	ShadowClass.lpfnWndProc	= ::DefWindowProc;
	ShadowClass.cbClsExtra = 0;
	ShadowClass.cbWndExtra = 0;
	ShadowClass.hInstance = hInstance;
	ShadowClass.hIcon = NULL;
	ShadowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	ShadowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	ShadowClass.lpszMenuName = NULL;
	ShadowClass.lpszClassName = strWndClassName;
	ShadowClass.hIconSm = NULL;

	//注册窗口类
	RegisterClassEx(&ShadowClass);

	//获取位置
	CRect rcWindow;	
	::GetWindowRect(m_hwndControl,&rcWindow);

	//创建窗口
	CreateEx(WS_EX_LAYERED|WS_EX_TRANSPARENT,strWndClassName,NULL,WS_POPUPWINDOW,rcWindow.left,rcWindow.top,rcWindow.Width(),rcWindow.Height(),m_hwndControl, NULL);

	//获取窗口过程
	m_OriParentProc = GetWindowLong(m_hwndControl, GWL_WNDPROC);

	//设置新窗口过程
	SetWindowLong(m_hwndControl, GWL_WNDPROC, (LONG)ParentProc);

	//设置变量
	m_SkinLayeredMap[m_hwndControl]=this;

	return true;
}

//设置区域
VOID CSkinLayered::InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild)
{
	//效验参数
	ASSERT((pDCImage!=NULL)&&(pDCImage->m_hDC!=NULL));
	if ((pDCImage==NULL)||(pDCImage->m_hDC==NULL)) return;

	//变量定义
	BITMAP Bitmap;
	ZeroMemory(&Bitmap,sizeof(Bitmap));

	//获取图像
	CBitmap * pBitmap=pDCImage->GetCurrentBitmap();
	if (pBitmap!=NULL) pBitmap->GetBitmap(&Bitmap);

	//获取大小
	CSize SizeImage;
	SizeImage.SetSize(Bitmap.bmWidth,Bitmap.bmHeight);

	//效验大小
	ASSERT((SizeImage.cx>0)&&(SizeImage.cy>0));
	if ((SizeImage.cx==0)||(SizeImage.cy==0)) return;

	//变量定义
	BLENDFUNCTION BlendFunction;
	ZeroMemory(&BlendFunction,sizeof(BlendFunction));

	//设置参数
	BlendFunction.BlendOp=0;
	BlendFunction.BlendFlags=0;
	BlendFunction.AlphaFormat=AC_SRC_ALPHA;
	BlendFunction.SourceConstantAlpha=cbAlpha;

	//设置分层
	CPoint ImagePoint(0,0);
	CClientDC ClientDC(this);
	UpdateLayeredWindow(&ClientDC,NULL,&SizeImage,pDCImage,&ImagePoint,0L,&BlendFunction,ULW_ALPHA);

	//创建区域
	CRgn RegionResult;
	RegionResult.CreateRectRgn(0,0,SizeImage.cx,SizeImage.cy);

	//窗口排除
	if (bUnLayeredChild==true)
	{
		//变量定义
		tagEnumChildInfo EnumChildInfo;
		ZeroMemory(&EnumChildInfo,sizeof(EnumChildInfo));

		//设置变量
		EnumChildInfo.pWndLayered=this;
		EnumChildInfo.hWndControl=m_hwndControl;
		EnumChildInfo.pRegionResult=&RegionResult;

		//枚举窗口
		ASSERT(m_hwndControl!=NULL);
		EnumChildWindows(m_hwndControl,EnumChildProc,(LPARAM)&EnumChildInfo);
	}

	//区域排除
	if (rcUnLayered.IsRectEmpty()==FALSE)
	{
		//创建区域
		CRgn RegionUnLayered;
		RegionUnLayered.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,PointRound.x,PointRound.y);

		//合并区域
		RegionResult.CombineRgn(&RegionResult,&RegionUnLayered,RGN_DIFF);
	}

	//设置区域
	SetWindowRgn(RegionResult,TRUE);

	return;
}

//设置区域
VOID CSkinLayered::InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild)
{
	//创建缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(Image.GetWidth(),Image.GetHeight(),32);

	//绘画界面
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

	//绘画界面
	ASSERT(pBufferDC!=NULL);
	if (pBufferDC!=NULL) Image.DrawImage(pBufferDC,0,0);

	//创建分层
	InitLayeredArea(pBufferDC,cbAlpha,rcUnLayered,PointRound,bUnLayeredChild);

	return;
}

//关闭消息
VOID CSkinLayered::OnClose()
{
	if(m_hwndControl!=NULL)
	{
		::PostMessage(m_hwndControl,WM_CLOSE,0,0);
	}
}

//设置焦点
VOID CSkinLayered::OnSetFocus(CWnd* pOldWnd)
{
	if(m_hwndControl!=NULL)
	{
		::SetFocus(m_hwndControl);
	}
}

//枚举函数
BOOL CALLBACK CSkinLayered::EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	//获取位置
	CRect rcWindow;
	::GetWindowRect(hWndChild,&rcWindow);

	//创建区域
	if ((rcWindow.Width()>0)&&(rcWindow.Height()>0))
	{
		//变量定义
		ASSERT(lParam!=0L);
		tagEnumChildInfo * pEnumChildInfo=(tagEnumChildInfo *)lParam;

		//窗口判断
		HWND hWndParent=::GetParent(hWndChild);
		if (hWndParent!=pEnumChildInfo->hWndControl)
		{
			return TRUE;
		}

		//转换位置
		ASSERT(pEnumChildInfo->hWndControl!=NULL);
		CWnd::FromHandle(pEnumChildInfo->hWndControl)->ScreenToClient(&rcWindow);

		//创建区域
		CRgn RgnChild;
		RgnChild.CreateRectRgn(rcWindow.left,rcWindow.top,rcWindow.right,rcWindow.bottom);

		//合并区域
		ASSERT(pEnumChildInfo->pRegionResult!=NULL);
		pEnumChildInfo->pRegionResult->CombineRgn(pEnumChildInfo->pRegionResult,&RgnChild,RGN_DIFF);
	}

	return TRUE;
}

//窗口过程
LRESULT CALLBACK CSkinLayered::ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//查找对象
	CSkinLayered * pSkinLayered=NULL;
	m_SkinLayeredMap.Lookup(hwnd,pSkinLayered);
	if(pSkinLayered==NULL || pSkinLayered->m_hwndControl!=hwnd) return 1L;

	//获取窗口过程
	WNDPROC pDefaultProc = (WNDPROC)pSkinLayered->m_OriParentProc;

	switch(uMsg)
	{
	case WM_MOVE:
		{
			if(pSkinLayered->IsWindowVisible()==TRUE)
			{
				CRect ParentRect;
				::GetWindowRect(hwnd,&ParentRect);
				pSkinLayered->SetWindowPos(NULL,ParentRect.left,ParentRect.top,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
			}
		}
		break;
	case WM_WINDOWPOSCHANGED: 
		{
			LRESULT lResult = pDefaultProc(hwnd, uMsg, wParam, lParam);
			if(pSkinLayered->m_hWnd != NULL)
			{
				WINDOWPOS * lpWndPos=(WINDOWPOS *)lParam;
				if((lpWndPos->cx>0)&&(lpWndPos->cy>0))
				{
					pSkinLayered->SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER|SWP_NOACTIVATE);
				}
			}
			return lResult;
		}
		break;	
	case WM_SIZE:
		{
			if(pSkinLayered->IsWindowEnabled()==TRUE)
			{
				if(wParam==SIZE_MINIMIZED)
				{
					pSkinLayered->ShowWindow(SW_HIDE);
				}
				else
				{
					LONG lParentStyle = GetWindowLong(hwnd, GWL_STYLE);
					if(WS_VISIBLE & lParentStyle) pSkinLayered->ShowWindow(SW_SHOW);
				}
			}
		}
		break;
	case WM_SHOWWINDOW:
		{
			if(pSkinLayered->IsWindowEnabled()==TRUE)
			{
				LRESULT lResult = pDefaultProc(hwnd, uMsg, wParam, lParam);
				pSkinLayered->ShowWindow(wParam?SW_SHOW:SW_HIDE);

				return lResult;
			}
		}
		break;
	case WM_NCDESTROY:
		{
			m_SkinLayeredMap.RemoveKey(hwnd);	
		}
		break;
	}

	return pDefaultProc(hwnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd) : CFGuiDialog(nIDTemplate,pParentWnd)
{
	//变量定义
	m_bInitDialog=false;
	m_bLableRound=true;
	m_rcRoundSpace.SetRect(ROUND_XSPACE,ROUND_YSPACE,ROUND_XSPACE,ROUND_YSPACE);

	//窗口变量
	m_nIDTemplate=nIDTemplate;
	ZeroMemory(m_pButton,sizeof(m_pButton));

	return;
}

//析构函数
CSkinDialog::~CSkinDialog()
{
}

//初始控件
void CSkinDialog::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//标签环绕	
	if(m_bLableRound==true)
	{
		m_pLabelRound = new CLabelUI();
		m_pLabelRound->SetFloat();
		m_pLabelRound->SetManager(&m_PaintManager,pParent,true);
		m_pLabelRound->SetBkImage(TEXT("file='IMAGE_ROUND_FRAME' restype='PNG' corner='6,6,6,6'"));	
		pParent->AddAt(m_pLabelRound,0);
	}

	//最小按钮
	m_pButton[0] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButton[0]->SetStatusImage(TEXT("file='BT_SKIN_CLOSE' restype='PNG'"));
	m_pButton[0]->SetVisible(false);

	//最大按钮
	m_pButton[1] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButton[1]->SetStatusImage(TEXT("file='BT_SKIN_MAX' restype='PNG'"));
	m_pButton[1]->SetVisible(false);

	//关闭按钮
	m_pButton[2] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButton[2]->SetStatusImage(TEXT("file='BT_SKIN_MIN' restype='PNG'"));
	m_pButton[2]->SetVisible(false);

	//设置标题区域
	CRect rcCaption(0,0,0,m_SkinAttribute.m_EncircleInfoFrame.nTBorder);
	m_PaintManager.SetCaptionRect(rcCaption); 

	//设置变量
	m_bInitDialog=true;
}

//消息提醒
void CSkinDialog::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//最小按钮
		if(m_pButton[0]==pControlUI) 
		{			
			PostMessage(WM_CLOSE,0,0);

			return;
		}

		//最大按钮
		if(m_pButton[1]==pControlUI) 
		{
			ShowWindow((IsZoomed()==FALSE)?SW_MAXIMIZE:SW_RESTORE);

			return;
		}

		//关闭按钮
		if(m_pButton[2]==pControlUI) 
		{
			ShowWindow(SW_MINIMIZE);

			return;
		}		
	}
}

//开始绘画
void CSkinDialog::OnEndPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取区域
	CRect rcClient;
	GetClientRect(rcClient);

	//设置缓冲
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画标题
	OnDrawSystemTitle(pDC,rcClient.Width(),rcClient.Height());

	//绘画界面
	OnDrawClientAreaEnd(pDC,rcClient.Width(),rcClient.Height());
}

//开始绘画
void CSkinDialog::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取区域
	CRect rcClient;
	GetClientRect(rcClient);

	//绘画界面
	DrawControlView(pDC,rcClient.Width(),rcClient.Height());
}

//配置函数
BOOL CSkinDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//设置属性
	ModifyStyle(WS_CAPTION,0,SWP_DRAWFRAME);
	//m_SkinDPI.Attach(AfxFindResourceHandle(MAKEINTRESOURCE(m_nIDTemplate), RT_DIALOG),m_hWnd,/*m_lpszTemplateName*/m_nIDTemplate,96.0);

	//设置字体
	SetWndFont(this,NULL);

	//按钮状态
	ZeroMemory(m_cbButtonState,sizeof(m_cbButtonState));

	//属性判断
	LONG lWindowStyte=GetWindowLongPtr(m_hWnd,GWL_STYLE);
	if (lWindowStyte&WS_SYSMENU)
	{
		//设置按钮
		m_cbButtonState[BST_CLOSE]=BUT_EXIST|BUT_NORMAL;
		m_cbButtonState[BST_MAX_SIZE]=BUT_EXIST|((lWindowStyte&WS_MAXIMIZEBOX)?BUT_NORMAL:BUT_DISABLE);
		m_cbButtonState[BST_MIN_SIZE]=BUT_EXIST|(((lWindowStyte&WS_MINIMIZEBOX)||(GetParent()==NULL))?BUT_NORMAL:BUT_DISABLE);

		//按钮调整
		if (((m_cbButtonState[BST_MAX_SIZE]&BUT_DISABLE)!=0)&&((m_cbButtonState[BST_MIN_SIZE]&BUT_DISABLE)!=0))
		{
			m_cbButtonState[BST_MAX_SIZE]=0;
			m_cbButtonState[BST_MIN_SIZE]=0;
		}
	}

	//创建分层
	m_SkinLayered.CreateLayered(m_hWnd);

	//调整资源
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());

	return FALSE;
}

//绘画消息
VOID CSkinDialog::OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//调整资源
VOID CSkinDialog::RectifyResource(INT nWidth, INT nHeight)
{
	if(m_bInitDialog==false) return;

	//调整判断
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//调整标签
		if(m_pLabelRound!=NULL && m_bLableRound==true)
		{
			//变量定义
			INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
			INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
			INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
			INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

			//调整控件
			m_pLabelRound->SetFixedWidth(nWidth-nLViewBorder-nRViewBorder-m_rcRoundSpace.left-m_rcRoundSpace.right);
			m_pLabelRound->SetFixedHeight(nHeight-nTViewBorder-nBViewBorder-m_rcRoundSpace.top-m_rcRoundSpace.bottom);
			m_pLabelRound->SetPos(CRect(nLViewBorder+m_rcRoundSpace.left,nTViewBorder+m_rcRoundSpace.top,nWidth-nRViewBorder-m_rcRoundSpace.right,nHeight-nBViewBorder-m_rcRoundSpace.bottom));
		}

		//调整按钮
		CaleTitleButton(nWidth,nHeight);

		//框架位置
		CRect rcFrame;
		rcFrame.SetRect(SHADOW_CX,SHADOW_CY,nWidth-SHADOW_CX,nHeight-SHADOW_CY);

		//窗口区域
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,ROUND_CX,ROUND_CY);

		//设置区域
		SetWindowRgn(RegionWindow,TRUE);

		//分层窗口
		if (m_SkinLayered.m_hWnd!=NULL)
		{
			//建立缓冲
			CImage ImageBuffer;
			ImageBuffer.Create(nWidth,nHeight,32);

			//创建 DC
			CImageDC ImageDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(ImageDC);

			//绘画界面
			DrawControlView(pBufferDC,nWidth,nHeight);

			//更新分层
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}

	return;
}

//调整按钮
VOID CSkinDialog::CaleTitleButton(INT nWidth, INT nHeight)
{
	//调整按钮
	for (INT i=0;i<CountArray(m_pButton);i++)
	{
		//计算偏移
		INT nYPos=6;
		INT nXPos=nWidth-(m_SkinAttribute.m_SizeButton.cx+2)*(i+1)-12;

		//设置位置
		CRect rcPos(nXPos,nYPos,nXPos+m_SkinAttribute.m_SizeButton.cx,nYPos+m_SkinAttribute.m_SizeButton.cy);
		m_pButton[i]->SetPos(rcPos);	

		//可视设置		
		m_pButton[i]->SetVisible(false);		
		if ((m_cbButtonState[i]&BUT_EXIST)!=0) m_pButton[i]->SetVisible();

		//使能设置
		m_pButton[i]->SetEnabled(true);
		if ((m_cbButtonState[i]&BUT_DISABLE)!=0) m_pButton[i]->SetEnabled(false);
	}

	//设置资源
    if(IsZoomed()==FALSE)
	{
		m_pButton[1]->SetStatusImage(TEXT("file='BT_SKIN_MAX' restype='PNG'"));
	}
	else
	{
		m_pButton[1]->SetStatusImage(TEXT("file='BT_SKIN_RESORE' restype='PNG'"));
	}

	return;
}

//绘画阴影
VOID CSkinDialog::DrawControlView(CDC * pDC, INT nWdith, INT nHeight)
{
	//设置缓冲
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//框架位置
	CRect rcFrame;
	rcFrame.SetRect(0,0,nWdith,nHeight);

	//视图位置
	CRect rcClientView;
	rcClientView.top=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	rcClientView.left=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	rcClientView.right=nWdith-m_SkinAttribute.m_EncircleInfoFrame.nRBorder;
	rcClientView.bottom=nHeight-m_SkinAttribute.m_EncircleInfoFrame.nBBorder;

	//绘画视图
	pDC->FillSolidRect(&rcClientView,m_SkinAttribute.m_crBackGround);

	//绘画框架
	m_SkinAttribute.m_PNGEncircleFrame.DrawEncircleFrame(pDC,rcFrame);

	//绘画界面
	OnDrawClientAreaBegin(pDC,nWdith,nHeight);

	return;
}

//绘画标题
VOID CSkinDialog::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//获取标题
	CString strTitle;
	GetWindowText(strTitle);

	//加载图标
	CPngImage ImageLogo;
	ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	//绘画图标
	if (ImageLogo.IsNull()==false)
	{
		ImageLogo.DrawImage(pDC,11,4);
	}

	//绘画标题
	if (strTitle.IsEmpty()==false)
	{
		//计算区域
		CRect rcTitle;
		rcTitle.SetRect(48,16,nWdith-45,28);

		//变量定义
		INT nXMove[8]={1,1,1,0,-1,-1,-1,0};
		INT nYMove[8]={-1,0,1,1,1,0,-1,-1};

		//设置环境
		pDC->SetTextColor(m_SkinAttribute.m_crTitleFrame);

		//绘画边框
		for (INT i=0;i<CountArray(nXMove);i++)
		{
			//计算位置
			CRect rcTitleFrame;
			rcTitleFrame.top=rcTitle.top+nYMove[i];
			rcTitleFrame.left=rcTitle.left+nXMove[i];
			rcTitleFrame.right=rcTitle.right+nXMove[i];
			rcTitleFrame.bottom=rcTitle.bottom+nYMove[i];

			//绘画字符
			pDC->DrawText(strTitle,&rcTitleFrame,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
		}

		//绘画文字
		pDC->SetTextColor(m_SkinAttribute.m_crTitleText);
		pDC->DrawText(strTitle,&rcTitle,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
	}

	return;
}

//绘画消息
VOID CSkinDialog::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//位置消息
VOID CSkinDialog::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//调整资源
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());

	return;
}

//颜色消息
HBRUSH CSkinDialog::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:		
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlText);
			return m_bLableRound?CSkinDialog::m_SkinAttribute.m_brLabelRound:CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//标题消息
LRESULT	CSkinDialog::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//默认调用
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return lResult;
}

//枚举函数
BOOL CALLBACK CSkinDialog::EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	//获取位置
	CRect rcWindow;
	::GetWindowRect(hWndChild,&rcWindow);

	//创建区域
	if ((rcWindow.Width()>0)&&(rcWindow.Height()>0))
	{
		//变量定义
		ASSERT(lParam!=0L);
		tagEnumChildParam * pEnumChildInfo=(tagEnumChildParam *)lParam;

		//窗口判断
		HWND hWndParent=::GetParent(hWndChild);
		if (hWndParent!=pEnumChildInfo->hWndControl)
		{
			return TRUE;
		}

		//设置字体
		CWnd *pChildWnd = CWnd::FromHandle(hWndChild);
		if(pChildWnd)pChildWnd->SetFont(pEnumChildInfo->pFont);
	}

	return TRUE;
}

//设置字体
VOID CALLBACK CSkinDialog::SetWndFont(CWnd *pWnd, CFont *pFont)
{
	//参数效验
	if(pWnd==NULL) return;

	//定义变量
	CFont *pSetFont=pFont;
	if(pSetFont==NULL)pSetFont=&m_SkinAttribute.m_Font;

	//设置字体
	pWnd->SetFont(pSetFont);

	//设置控件
	tagEnumChildParam ChildParam;
	ChildParam.hWndControl=pWnd->GetSafeHwnd();
	ChildParam.pFont = pSetFont;
	EnumChildWindows(pWnd->GetSafeHwnd(),CSkinDialog::EnumChildProc,(LPARAM)&ChildParam);
}
//////////////////////////////////////////////////////////////////////////////////
