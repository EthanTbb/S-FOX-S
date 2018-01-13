#include "stdafx.h"
#include "MyEdit.h"
#include ".\myedit.h"
// CMyEdit
IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{
	//初始化为系统字体和窗口颜色
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_crBackGnd = GetSysColor(COLOR_WINDOW);
	m_font.CreatePointFont(60,TEXT("宋体")); 
	m_brBackGnd.CreateSolidBrush(GetSysColor(COLOR_WINDOW));

}

CMyEdit::~CMyEdit()
{
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
//WM_CTLCOLOR的消息反射
ON_WM_CTLCOLOR_REFLECT()
ON_WM_CHAR()
ON_WM_NCCALCSIZE()
ON_WM_CREATE()
ON_WM_VSCROLL()
END_MESSAGE_MAP()

void CMyEdit::SetBackColor(COLORREF rgb)
{
   //设置文字背景颜色
   m_crBackGnd = rgb;

   //释放旧的画刷
   if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
   //使用文字背景颜色创建新的画刷,使得文字框背景和文字背景一致
   m_brBackGnd.CreateSolidBrush(rgb);
   //redraw
   Invalidate(TRUE);
}
void CMyEdit::SetTextColor(COLORREF rgb)
{
   //设置文字颜色
   m_crText = rgb;
   //redraw
   Invalidate(TRUE);
}
void CMyEdit::SetTextFont(const LOGFONT &lf)
{
//创建新的字体,并设置为CEDIT的字体
	if(m_font.GetSafeHandle())
	{
	m_font.DeleteObject();
	}
	
	m_font.CreateFontIndirect(&lf);
	//CEdit::SetFont(&m_font);
	CRichEditCtrl::SetFont(&m_font);
	//redraw
	Invalidate(TRUE);
}
BOOL CMyEdit::GetTextFont(LOGFONT &lf)
{ 
	if(m_font.GetLogFont(&lf)!=0)
	{      return TRUE;   
	}
	return FALSE;
}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//刷新前更改文本颜色
	pDC->SetTextColor(m_crText);

	//刷新前更改文本背景
	pDC->SetBkColor(m_crBackGnd);

	//返回画刷,用来绘制整个控件背景
	return m_brBackGnd;
}
 


// void CMyEdit::PreSubclassWindow()
// {
// 	// TODO: 在此添加专用代码和/或调用基类
// 
// 	//CEdit::PreSubclassWindow();
// 	CSkinRichEdit::PreSubclassWindow();
// 	
// 	//设置滚动
// 	//m_SkinScrollBar.InitScroolBar(this);
// }

void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return;
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}


void CMyEdit::Init()
{
	InitializeFlatSB(this->m_hWnd);
	FlatSB_EnableScrollBar(this->m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

	CRect windowRect;
	GetWindowRect(&windowRect);

	int nTitleBarHeight = 0;

	CWnd* pParent = GetParent();

	if(pParent->GetStyle() & WS_CAPTION)
		nTitleBarHeight = GetSystemMetrics(SM_CYSIZE);


	int nDialogFrameHeight = 0;
	int nDialogFrameWidth = 0;
	if((pParent->GetStyle() & WS_BORDER))
	{
		nDialogFrameHeight = GetSystemMetrics(SM_CYDLGFRAME);
		nDialogFrameWidth = GetSystemMetrics(SM_CYDLGFRAME);
	}

	if(pParent->GetStyle() & WS_THICKFRAME)
	{
		nDialogFrameHeight+=1;
		nDialogFrameWidth+=1;
	}

	CRect rect1(windowRect.right-nDialogFrameWidth,windowRect.top-nTitleBarHeight-nDialogFrameHeight-1,windowRect.right+12-nDialogFrameWidth,windowRect.bottom+11-nTitleBarHeight-nDialogFrameHeight);
	CRect rect2(windowRect.left-nDialogFrameWidth,windowRect.bottom-nTitleBarHeight-nDialogFrameHeight-1,windowRect.right+1-nDialogFrameWidth,windowRect.bottom+11-nTitleBarHeight-nDialogFrameHeight);

	m_SkinVerticleScrollbar.Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE|WS_GROUP,rect1, pParent);
	
	m_SkinVerticleScrollbar.pEdit = (CEdit  *)this;
	
}

void CMyEdit::ReSetLoc( int cx, int cy)
{
	CRect windowRect,clientRect;
	GetWindowRect(&windowRect);

	GetClientRect(&clientRect);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//读取位置
	CSize SizeRestrict(cx,cy);
	//正常位置
	CRect rcNormalSize;			

	//位置调整
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//移动窗口
	rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	rcNormalSize.right=rcNormalSize.left+SizeRestrict.cx;
	rcNormalSize.bottom=rcNormalSize.top+SizeRestrict.cy;

	m_SkinVerticleScrollbar.MoveWindow(rcNormalSize.Width()-235+clientRect.Width()+10+16,rcNormalSize.Height()-116+clientRect.top,16,clientRect.Height());
	
}

void CMyEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ModifyStyle(WS_HSCROLL | WS_VSCROLL,0,0); 
	CRichEditCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
		Init();
	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CMyEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRichEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	m_SkinVerticleScrollbar.UpdateThumbPosition();
}

//插入字串
bool CMyEdit::InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat)
{
	//变量定义
	bool bResumeSelect;
	CHARRANGE CharRange;

	//保存状态
	GetSel(CharRange.cpMin,CharRange.cpMax);
	bResumeSelect=(CharRange.cpMax!=CharRange.cpMin);

	//搜索变量
	LPCTSTR pszHttp=TEXT("http://");
	const INT nHttpLength=lstrlen(pszHttp);
	const INT nStringLength=lstrlen(pszString);

	//索引定义
	INT nStringStart=0;
	INT nStringPause=0;

	//字符解释
	for (INT i=0;i<nStringLength;i++)
	{
		//变量定义
		INT nUrlPause=i;

		

		//终止字符
		if (nUrlPause<=(i+nHttpLength)) nStringPause=(i+1);

		//插入字符
		if ((i==(nStringLength-1))||(nUrlPause>(i+nHttpLength)))
		{
			//普通字符
			if (nStringPause>nStringStart)
			{
				//获取缓冲
				CString strNormalString;
				LPTSTR pszNormalString=strNormalString.GetBuffer(nStringPause-nStringStart+1);

				//拷贝字符
				pszNormalString[nStringPause-nStringStart]=0;
				CopyMemory(pszNormalString,&pszString[nStringStart],(nStringPause-nStringStart)*sizeof(TCHAR));

				//释放缓冲
				strNormalString.ReleaseBuffer();

				//插入消息
				SetSel(-1L,-1L);
				SetWordCharFormat(CharFormat);
				ReplaceSel((LPCTSTR)strNormalString);
			}

			//连接地址
			if (nUrlPause>(i+nHttpLength))
			{
				//获取缓冲
				CString strUrlString;
				LPTSTR pszUrlString=strUrlString.GetBuffer((nUrlPause-i)+1);

				//拷贝字符
				pszUrlString[nUrlPause-i]=0;
				CopyMemory(pszUrlString,&pszString[i],(nUrlPause-i)*sizeof(TCHAR));

				//释放缓冲
				strUrlString.ReleaseBuffer();

				//构造格式
				CHARFORMAT2 CharFormatHyper;
				CharFormatHyper=CharFormat;

			

				//插入消息
				SetSel(-1L,-1L);
				SetWordCharFormat(CharFormatHyper);
				ReplaceSel((LPCTSTR)strUrlString);
			}

			//设置索引
			nStringStart=__max(i,nUrlPause);
		}

		//设置索引
		i+=(nUrlPause-i);
	}

	//状态设置
	if (bResumeSelect==true) SetSel(CharRange);
	else PostMessage(WM_VSCROLL,SB_BOTTOM,0);

	return true;
}
