#include "StdAfx.h"
#include "Resource.h"
#include "SkinDialog.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//����С
#define ROUND_XSPACE				3									//������
#define ROUND_YSPACE				3									//������

//��Ӱ����
#define SHADOW_CX					4									//��Ӱ���
#define SHADOW_CY					4									//��Ӱ�߶�

//��ť��ʶ
#define BST_CLOSE					0									//�رհ�ť
#define BST_MAX_SIZE				1									//��ԭ��ť
#define BST_MIN_SIZE				2									//��С��ť

//��ť״̬
#define BUT_EXIST					0x01								//��ť����
#define BUT_NORMAL					0x02								//��ť����
#define BUT_DISABLE					0x04								//��ť��ֹ

//////////////////////////////////////////////////////////////////////////////////
//���ɱ���

//ʱ���ʶ
#define IDI_TRANSITION				10									//���ɱ�ʶ
#define TIME_TRANSITION				30									//����ʱ��

//��������
#define MIN_TRANSITION_INDEX		0									//��������
#define MAX_TRANSITION_INDEX		8									//��������

//////////////////////////////////////////////////////////////////////////////////

//ö�ٽṹ
struct tagEnumChildInfo
{
	//CWnd *							pWndControl;						//���ƴ���
	HWND							hWndControl;						//���ھ��
	CWnd *							pWndLayered;						//�ֲ㴰��
	CRgn *							pRegionResult;						//�������
};

//////////////////////////////////////////////////////////////////////////////////

//ö�ٽṹ
struct tagEnumChildParam
{
	//CWnd *							pWndControl;						//���ƴ���
	HWND							hWndControl;						//���ھ��
	CFont *							pFont;						        //��������
};

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CSkinDialogAttribute				CSkinDialog::m_SkinAttribute;		//��Դ����
CSkinLayeredMap						CSkinLayered::m_SkinLayeredMap;		//�ֲ��б�

//////////////////////////////////////////////////////////////////////////////////

//��������
const TCHAR * strWndClassName = TEXT("PerrySkinWnd");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinLayered, CWnd)
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinDialog, CFGuiDialog)
	
	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()

	//�Զ���Ϣ
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinDialogAttribute::CSkinDialogAttribute()
{
	//������ɫ
	m_crTitleText=RGB(0,0,0);
	m_crTitleFrame=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	m_crControlText=RGB(0,0,0);
	m_crLabelRound=RGB(0,0,0);

	//��������
	tagEncircleResource EncircleResource;
	HINSTANCE hResInstance=GetModuleHandle(SKIN_CONTROL_DLL_NAME);

	//������Դ
	EncircleResource.pszImageTL=TEXT("SKIN_WND_TL");
	EncircleResource.pszImageTM=TEXT("SKIN_WND_TM");
	EncircleResource.pszImageTR=TEXT("SKIN_WND_TR");
	EncircleResource.pszImageML=TEXT("SKIN_WND_ML");
	EncircleResource.pszImageMR=TEXT("SKIN_WND_MR");
	EncircleResource.pszImageBL=TEXT("SKIN_WND_BL");
	EncircleResource.pszImageBM=TEXT("SKIN_WND_BM");
	EncircleResource.pszImageBR=TEXT("SKIN_WND_BR");
	m_PNGEncircleFrame.InitEncircleResource(EncircleResource,hResInstance);

	//��ȡ��С
	CBitImage ImageButtonClose;
	ImageButtonClose.LoadFromResource(hResInstance,IDB_SKIN_WND_CLOSE);
	m_SizeButton.SetSize(ImageButtonClose.GetWidth()/5,ImageButtonClose.GetHeight());

	//������ͼ
	m_PNGEncircleFrame.GetEncircleInfo(m_EncircleInfoFrame);

	//������ͼ
	m_EncircleInfoView.nBBorder=8;
	m_EncircleInfoView.nTBorder=m_EncircleInfoFrame.nTBorder;
	m_EncircleInfoView.nLBorder=m_EncircleInfoFrame.nLBorder;
	m_EncircleInfoView.nRBorder=m_EncircleInfoFrame.nRBorder;

	//��������
	if (m_Font.GetSafeHandle()==NULL)
	{
		m_Font.CreateFont(-12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET,  
			OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,  
			DEFAULT_PITCH, TEXT("����"));
	}

	return;
}
	
//��������
CSkinDialogAttribute::~CSkinDialogAttribute()
{
	//������Դ
	if (m_brBackGround.GetSafeHandle()!=NULL)
	{
		m_brBackGround.DeleteObject();
	}

	//������Դ
	if (m_brLabelRound.GetSafeHandle()!=NULL)
	{
		m_brLabelRound.DeleteObject();
	}

	//������Դ
	if (m_Font.GetSafeHandle()!=NULL)
	{
		m_Font.DeleteObject();
	}

	return;
}

//������Դ
bool CSkinDialogAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//������ɫ
	m_crTitleText=RGB(0,0,0);
	m_crTitleFrame=RGB(211,198,170);

	//�ؼ���ɫ
	m_crControlText=RGB(10,10,10);
	m_crBackGround=RGB(136,77,32);
	m_crLabelRound=RGB(189,173,139);

	//������Դ
	m_brBackGround.DeleteObject();
	m_brBackGround.CreateSolidBrush(m_crBackGround);

	//������Դ
	m_brLabelRound.DeleteObject();
	m_brLabelRound.CreateSolidBrush(m_crLabelRound);	

	//��������
	if (m_Font.GetSafeHandle()==NULL)
	{
		m_Font.CreateFont(-12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET,  
			OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,  
			DEFAULT_PITCH, TEXT("����"));
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinLayered::CSkinLayered()
{	
	//���ñ���	
	m_hwndControl=NULL;
	m_OriParentProc=NULL;

	return;
}

//��������
CSkinLayered::~CSkinLayered()
{
}

//��������
bool CSkinLayered::CreateLayered(HWND hWnd)
{
	//Ч�����
	ASSERT(hWnd!=NULL);
	if(hWnd==NULL) return false;

	//���ñ���
	m_hwndControl=hWnd;

	//���ñ���
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//��������
	WNDCLASSEX ShadowClass;
	memset(&ShadowClass, 0, sizeof(ShadowClass));

	//��������
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

	//ע�ᴰ����
	RegisterClassEx(&ShadowClass);

	//��ȡλ��
	CRect rcWindow;	
	::GetWindowRect(m_hwndControl,&rcWindow);

	//��������
	CreateEx(WS_EX_LAYERED|WS_EX_TRANSPARENT,strWndClassName,NULL,WS_POPUPWINDOW,rcWindow.left,rcWindow.top,rcWindow.Width(),rcWindow.Height(),m_hwndControl, NULL);

	//��ȡ���ڹ���
	m_OriParentProc = GetWindowLong(m_hwndControl, GWL_WNDPROC);

	//�����´��ڹ���
	SetWindowLong(m_hwndControl, GWL_WNDPROC, (LONG)ParentProc);

	//���ñ���
	m_SkinLayeredMap[m_hwndControl]=this;

	return true;
}

//��������
VOID CSkinLayered::InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild)
{
	//Ч�����
	ASSERT((pDCImage!=NULL)&&(pDCImage->m_hDC!=NULL));
	if ((pDCImage==NULL)||(pDCImage->m_hDC==NULL)) return;

	//��������
	BITMAP Bitmap;
	ZeroMemory(&Bitmap,sizeof(Bitmap));

	//��ȡͼ��
	CBitmap * pBitmap=pDCImage->GetCurrentBitmap();
	if (pBitmap!=NULL) pBitmap->GetBitmap(&Bitmap);

	//��ȡ��С
	CSize SizeImage;
	SizeImage.SetSize(Bitmap.bmWidth,Bitmap.bmHeight);

	//Ч���С
	ASSERT((SizeImage.cx>0)&&(SizeImage.cy>0));
	if ((SizeImage.cx==0)||(SizeImage.cy==0)) return;

	//��������
	BLENDFUNCTION BlendFunction;
	ZeroMemory(&BlendFunction,sizeof(BlendFunction));

	//���ò���
	BlendFunction.BlendOp=0;
	BlendFunction.BlendFlags=0;
	BlendFunction.AlphaFormat=AC_SRC_ALPHA;
	BlendFunction.SourceConstantAlpha=cbAlpha;

	//���÷ֲ�
	CPoint ImagePoint(0,0);
	CClientDC ClientDC(this);
	UpdateLayeredWindow(&ClientDC,NULL,&SizeImage,pDCImage,&ImagePoint,0L,&BlendFunction,ULW_ALPHA);

	//��������
	CRgn RegionResult;
	RegionResult.CreateRectRgn(0,0,SizeImage.cx,SizeImage.cy);

	//�����ų�
	if (bUnLayeredChild==true)
	{
		//��������
		tagEnumChildInfo EnumChildInfo;
		ZeroMemory(&EnumChildInfo,sizeof(EnumChildInfo));

		//���ñ���
		EnumChildInfo.pWndLayered=this;
		EnumChildInfo.hWndControl=m_hwndControl;
		EnumChildInfo.pRegionResult=&RegionResult;

		//ö�ٴ���
		ASSERT(m_hwndControl!=NULL);
		EnumChildWindows(m_hwndControl,EnumChildProc,(LPARAM)&EnumChildInfo);
	}

	//�����ų�
	if (rcUnLayered.IsRectEmpty()==FALSE)
	{
		//��������
		CRgn RegionUnLayered;
		RegionUnLayered.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,PointRound.x,PointRound.y);

		//�ϲ�����
		RegionResult.CombineRgn(&RegionResult,&RegionUnLayered,RGN_DIFF);
	}

	//��������
	SetWindowRgn(RegionResult,TRUE);

	return;
}

//��������
VOID CSkinLayered::InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild)
{
	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(Image.GetWidth(),Image.GetHeight(),32);

	//�滭����
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

	//�滭����
	ASSERT(pBufferDC!=NULL);
	if (pBufferDC!=NULL) Image.DrawImage(pBufferDC,0,0);

	//�����ֲ�
	InitLayeredArea(pBufferDC,cbAlpha,rcUnLayered,PointRound,bUnLayeredChild);

	return;
}

//�ر���Ϣ
VOID CSkinLayered::OnClose()
{
	if(m_hwndControl!=NULL)
	{
		::PostMessage(m_hwndControl,WM_CLOSE,0,0);
	}
}

//���ý���
VOID CSkinLayered::OnSetFocus(CWnd* pOldWnd)
{
	if(m_hwndControl!=NULL)
	{
		::SetFocus(m_hwndControl);
	}
}

//ö�ٺ���
BOOL CALLBACK CSkinLayered::EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	//��ȡλ��
	CRect rcWindow;
	::GetWindowRect(hWndChild,&rcWindow);

	//��������
	if ((rcWindow.Width()>0)&&(rcWindow.Height()>0))
	{
		//��������
		ASSERT(lParam!=0L);
		tagEnumChildInfo * pEnumChildInfo=(tagEnumChildInfo *)lParam;

		//�����ж�
		HWND hWndParent=::GetParent(hWndChild);
		if (hWndParent!=pEnumChildInfo->hWndControl)
		{
			return TRUE;
		}

		//ת��λ��
		ASSERT(pEnumChildInfo->hWndControl!=NULL);
		CWnd::FromHandle(pEnumChildInfo->hWndControl)->ScreenToClient(&rcWindow);

		//��������
		CRgn RgnChild;
		RgnChild.CreateRectRgn(rcWindow.left,rcWindow.top,rcWindow.right,rcWindow.bottom);

		//�ϲ�����
		ASSERT(pEnumChildInfo->pRegionResult!=NULL);
		pEnumChildInfo->pRegionResult->CombineRgn(pEnumChildInfo->pRegionResult,&RgnChild,RGN_DIFF);
	}

	return TRUE;
}

//���ڹ���
LRESULT CALLBACK CSkinLayered::ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//���Ҷ���
	CSkinLayered * pSkinLayered=NULL;
	m_SkinLayeredMap.Lookup(hwnd,pSkinLayered);
	if(pSkinLayered==NULL || pSkinLayered->m_hwndControl!=hwnd) return 1L;

	//��ȡ���ڹ���
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

//���캯��
CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd) : CFGuiDialog(nIDTemplate,pParentWnd)
{
	//��������
	m_bInitDialog=false;
	m_bLableRound=true;
	m_rcRoundSpace.SetRect(ROUND_XSPACE,ROUND_YSPACE,ROUND_XSPACE,ROUND_YSPACE);

	//���ڱ���
	m_nIDTemplate=nIDTemplate;
	ZeroMemory(m_pButton,sizeof(m_pButton));

	return;
}

//��������
CSkinDialog::~CSkinDialog()
{
}

//��ʼ�ؼ�
void CSkinDialog::InitControlUI()
{
	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��ǩ����	
	if(m_bLableRound==true)
	{
		m_pLabelRound = new CLabelUI();
		m_pLabelRound->SetFloat();
		m_pLabelRound->SetManager(&m_PaintManager,pParent,true);
		m_pLabelRound->SetBkImage(TEXT("file='IMAGE_ROUND_FRAME' restype='PNG' corner='6,6,6,6'"));	
		pParent->AddAt(m_pLabelRound,0);
	}

	//��С��ť
	m_pButton[0] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButton[0]->SetStatusImage(TEXT("file='BT_SKIN_CLOSE' restype='PNG'"));
	m_pButton[0]->SetVisible(false);

	//���ť
	m_pButton[1] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButton[1]->SetStatusImage(TEXT("file='BT_SKIN_MAX' restype='PNG'"));
	m_pButton[1]->SetVisible(false);

	//�رհ�ť
	m_pButton[2] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButton[2]->SetStatusImage(TEXT("file='BT_SKIN_MIN' restype='PNG'"));
	m_pButton[2]->SetVisible(false);

	//���ñ�������
	CRect rcCaption(0,0,0,m_SkinAttribute.m_EncircleInfoFrame.nTBorder);
	m_PaintManager.SetCaptionRect(rcCaption); 

	//���ñ���
	m_bInitDialog=true;
}

//��Ϣ����
void CSkinDialog::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//��С��ť
		if(m_pButton[0]==pControlUI) 
		{			
			PostMessage(WM_CLOSE,0,0);

			return;
		}

		//���ť
		if(m_pButton[1]==pControlUI) 
		{
			ShowWindow((IsZoomed()==FALSE)?SW_MAXIMIZE:SW_RESTORE);

			return;
		}

		//�رհ�ť
		if(m_pButton[2]==pControlUI) 
		{
			ShowWindow(SW_MINIMIZE);

			return;
		}		
	}
}

//��ʼ�滭
void CSkinDialog::OnEndPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡ����
	CRect rcClient;
	GetClientRect(rcClient);

	//���û���
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	OnDrawSystemTitle(pDC,rcClient.Width(),rcClient.Height());

	//�滭����
	OnDrawClientAreaEnd(pDC,rcClient.Width(),rcClient.Height());
}

//��ʼ�滭
void CSkinDialog::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡ����
	CRect rcClient;
	GetClientRect(rcClient);

	//�滭����
	DrawControlView(pDC,rcClient.Width(),rcClient.Height());
}

//���ú���
BOOL CSkinDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	ModifyStyle(WS_CAPTION,0,SWP_DRAWFRAME);
	//m_SkinDPI.Attach(AfxFindResourceHandle(MAKEINTRESOURCE(m_nIDTemplate), RT_DIALOG),m_hWnd,/*m_lpszTemplateName*/m_nIDTemplate,96.0);

	//��������
	SetWndFont(this,NULL);

	//��ť״̬
	ZeroMemory(m_cbButtonState,sizeof(m_cbButtonState));

	//�����ж�
	LONG lWindowStyte=GetWindowLongPtr(m_hWnd,GWL_STYLE);
	if (lWindowStyte&WS_SYSMENU)
	{
		//���ð�ť
		m_cbButtonState[BST_CLOSE]=BUT_EXIST|BUT_NORMAL;
		m_cbButtonState[BST_MAX_SIZE]=BUT_EXIST|((lWindowStyte&WS_MAXIMIZEBOX)?BUT_NORMAL:BUT_DISABLE);
		m_cbButtonState[BST_MIN_SIZE]=BUT_EXIST|(((lWindowStyte&WS_MINIMIZEBOX)||(GetParent()==NULL))?BUT_NORMAL:BUT_DISABLE);

		//��ť����
		if (((m_cbButtonState[BST_MAX_SIZE]&BUT_DISABLE)!=0)&&((m_cbButtonState[BST_MIN_SIZE]&BUT_DISABLE)!=0))
		{
			m_cbButtonState[BST_MAX_SIZE]=0;
			m_cbButtonState[BST_MIN_SIZE]=0;
		}
	}

	//�����ֲ�
	m_SkinLayered.CreateLayered(m_hWnd);

	//������Դ
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());

	return FALSE;
}

//�滭��Ϣ
VOID CSkinDialog::OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//������Դ
VOID CSkinDialog::RectifyResource(INT nWidth, INT nHeight)
{
	if(m_bInitDialog==false) return;

	//�����ж�
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//������ǩ
		if(m_pLabelRound!=NULL && m_bLableRound==true)
		{
			//��������
			INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
			INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
			INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
			INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

			//�����ؼ�
			m_pLabelRound->SetFixedWidth(nWidth-nLViewBorder-nRViewBorder-m_rcRoundSpace.left-m_rcRoundSpace.right);
			m_pLabelRound->SetFixedHeight(nHeight-nTViewBorder-nBViewBorder-m_rcRoundSpace.top-m_rcRoundSpace.bottom);
			m_pLabelRound->SetPos(CRect(nLViewBorder+m_rcRoundSpace.left,nTViewBorder+m_rcRoundSpace.top,nWidth-nRViewBorder-m_rcRoundSpace.right,nHeight-nBViewBorder-m_rcRoundSpace.bottom));
		}

		//������ť
		CaleTitleButton(nWidth,nHeight);

		//���λ��
		CRect rcFrame;
		rcFrame.SetRect(SHADOW_CX,SHADOW_CY,nWidth-SHADOW_CX,nHeight-SHADOW_CY);

		//��������
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,ROUND_CX,ROUND_CY);

		//��������
		SetWindowRgn(RegionWindow,TRUE);

		//�ֲ㴰��
		if (m_SkinLayered.m_hWnd!=NULL)
		{
			//��������
			CImage ImageBuffer;
			ImageBuffer.Create(nWidth,nHeight,32);

			//���� DC
			CImageDC ImageDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(ImageDC);

			//�滭����
			DrawControlView(pBufferDC,nWidth,nHeight);

			//���·ֲ�
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}

	return;
}

//������ť
VOID CSkinDialog::CaleTitleButton(INT nWidth, INT nHeight)
{
	//������ť
	for (INT i=0;i<CountArray(m_pButton);i++)
	{
		//����ƫ��
		INT nYPos=6;
		INT nXPos=nWidth-(m_SkinAttribute.m_SizeButton.cx+2)*(i+1)-12;

		//����λ��
		CRect rcPos(nXPos,nYPos,nXPos+m_SkinAttribute.m_SizeButton.cx,nYPos+m_SkinAttribute.m_SizeButton.cy);
		m_pButton[i]->SetPos(rcPos);	

		//��������		
		m_pButton[i]->SetVisible(false);		
		if ((m_cbButtonState[i]&BUT_EXIST)!=0) m_pButton[i]->SetVisible();

		//ʹ������
		m_pButton[i]->SetEnabled(true);
		if ((m_cbButtonState[i]&BUT_DISABLE)!=0) m_pButton[i]->SetEnabled(false);
	}

	//������Դ
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

//�滭��Ӱ
VOID CSkinDialog::DrawControlView(CDC * pDC, INT nWdith, INT nHeight)
{
	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//���λ��
	CRect rcFrame;
	rcFrame.SetRect(0,0,nWdith,nHeight);

	//��ͼλ��
	CRect rcClientView;
	rcClientView.top=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	rcClientView.left=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	rcClientView.right=nWdith-m_SkinAttribute.m_EncircleInfoFrame.nRBorder;
	rcClientView.bottom=nHeight-m_SkinAttribute.m_EncircleInfoFrame.nBBorder;

	//�滭��ͼ
	pDC->FillSolidRect(&rcClientView,m_SkinAttribute.m_crBackGround);

	//�滭���
	m_SkinAttribute.m_PNGEncircleFrame.DrawEncircleFrame(pDC,rcFrame);

	//�滭����
	OnDrawClientAreaBegin(pDC,nWdith,nHeight);

	return;
}

//�滭����
VOID CSkinDialog::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//��ȡ����
	CString strTitle;
	GetWindowText(strTitle);

	//����ͼ��
	CPngImage ImageLogo;
	ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	//�滭ͼ��
	if (ImageLogo.IsNull()==false)
	{
		ImageLogo.DrawImage(pDC,11,4);
	}

	//�滭����
	if (strTitle.IsEmpty()==false)
	{
		//��������
		CRect rcTitle;
		rcTitle.SetRect(48,16,nWdith-45,28);

		//��������
		INT nXMove[8]={1,1,1,0,-1,-1,-1,0};
		INT nYMove[8]={-1,0,1,1,1,0,-1,-1};

		//���û���
		pDC->SetTextColor(m_SkinAttribute.m_crTitleFrame);

		//�滭�߿�
		for (INT i=0;i<CountArray(nXMove);i++)
		{
			//����λ��
			CRect rcTitleFrame;
			rcTitleFrame.top=rcTitle.top+nYMove[i];
			rcTitleFrame.left=rcTitle.left+nXMove[i];
			rcTitleFrame.right=rcTitle.right+nXMove[i];
			rcTitleFrame.bottom=rcTitle.bottom+nYMove[i];

			//�滭�ַ�
			pDC->DrawText(strTitle,&rcTitleFrame,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
		}

		//�滭����
		pDC->SetTextColor(m_SkinAttribute.m_crTitleText);
		pDC->DrawText(strTitle,&rcTitle,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
	}

	return;
}

//�滭��Ϣ
VOID CSkinDialog::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//λ����Ϣ
VOID CSkinDialog::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//������Դ
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());

	return;
}

//��ɫ��Ϣ
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

//������Ϣ
LRESULT	CSkinDialog::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�ϵ���
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//���½���
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return lResult;
}

//ö�ٺ���
BOOL CALLBACK CSkinDialog::EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	//��ȡλ��
	CRect rcWindow;
	::GetWindowRect(hWndChild,&rcWindow);

	//��������
	if ((rcWindow.Width()>0)&&(rcWindow.Height()>0))
	{
		//��������
		ASSERT(lParam!=0L);
		tagEnumChildParam * pEnumChildInfo=(tagEnumChildParam *)lParam;

		//�����ж�
		HWND hWndParent=::GetParent(hWndChild);
		if (hWndParent!=pEnumChildInfo->hWndControl)
		{
			return TRUE;
		}

		//��������
		CWnd *pChildWnd = CWnd::FromHandle(hWndChild);
		if(pChildWnd)pChildWnd->SetFont(pEnumChildInfo->pFont);
	}

	return TRUE;
}

//��������
VOID CALLBACK CSkinDialog::SetWndFont(CWnd *pWnd, CFont *pFont)
{
	//����Ч��
	if(pWnd==NULL) return;

	//�������
	CFont *pSetFont=pFont;
	if(pSetFont==NULL)pSetFont=&m_SkinAttribute.m_Font;

	//��������
	pWnd->SetFont(pSetFont);

	//���ÿؼ�
	tagEnumChildParam ChildParam;
	ChildParam.hWndControl=pWnd->GetSafeHwnd();
	ChildParam.pFont = pSetFont;
	EnumChildWindows(pWnd->GetSafeHwnd(),CSkinDialog::EnumChildProc,(LPARAM)&ChildParam);
}
//////////////////////////////////////////////////////////////////////////////////
