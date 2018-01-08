#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "Dlgmatchawards.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//屏幕位置
#define LAYERED_SIZE						8									//分层大小
#define ROUND_XSIZE							0									//圆角大小	
#define ROUND_YSIZE							0									//圆角大小	
#define CAPTION_SIZE						50									//标题大小	

//////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMatchAwards::CDlgMatchAwards() : CFGuiDialog(IDD_DLG_MATCH_AWARDS)
{
	//设置变量
	m_pMatchResult=NULL;

	//创建字体
	m_RewardFont.CreateFont(14,0,0,0,1200,0,0,0,134,3,2,1,2,TEXT("宋体"));
	m_AccountsFont.CreateFont(14,0,0,0,1200,0,0,0,134,3,2,1,2,TEXT("宋体"));
	m_WatermarkFont.CreateFont(21,0,0,0,100,0,0,0,134,3,2,1,2,TEXT("宋体"));
}

CDlgMatchAwards::~CDlgMatchAwards()
{
	//释放资源
	m_RewardFont.DeleteObject();
	m_AccountsFont.DeleteObject();
	m_WatermarkFont.DeleteObject();
}

//设置参数
VOID CDlgMatchAwards::SetMatchResult(CMD_GR_MatchResult *pMatchResult) 
{ 
	//设置变量
	m_pMatchResult=pMatchResult; 

	//刷新界面
	InvalidateRect(NULL);
}

//初始控件
VOID CDlgMatchAwards::InitControlUI()
{
	//设置标题
	CRect rcCaption(0,0,0,200);
	m_PaintManager.SetCaptionRect(rcCaption);

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//返回按钮
	m_pbtClose = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtClose->SetStatusImage(TEXT("file='BT_AWARD_CLOSE' restype='PNG'"));	
	m_pbtClose->SetRelativeSide(RelativeRight);
	m_pbtClose->SetRelativePos(CPoint(48,36));

	//拍照按钮
	m_pbtTakePhoto = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtTakePhoto->SetStatusImage(TEXT("file='BT_TAKE_PHOTO' restype='PNG'"));	
	m_pbtTakePhoto->SetRelativeSide(RelativeRight);
	m_pbtTakePhoto->SetRelativePos(CPoint(83,36));
}

//消息提醒
VOID CDlgMatchAwards::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//退出按钮
		if (m_pbtClose==pControlUI) return OnBnClickedClose(); 

		//拍照按钮
		if (m_pbtTakePhoto==pControlUI) return OnBnClickedTakePhoto(); 
	}
}

//开始绘画
VOID CDlgMatchAwards::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);

	//获取区域
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义	
	CPngImage ImageStar,ImageAwards,ImageStamp;

	//资源句柄
	HINSTANCE hResInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//加载图片
	ImageStar.LoadImage(hResInstance,TEXT("MATCH_STAR"));
	ImageAwards.LoadImage(hResInstance,TEXT("MATCH_AWARDS"));

	//计算位置
	INT nXPos=(rcClient.Width()-ImageAwards.GetWidth())/2;
	INT nYPos=(rcClient.Height()-ImageAwards.GetHeight())/2;

	//绘画背景
	ImageAwards.DrawImage(pDC,nXPos,nYPos);

	//绘画奖励
	if(m_pMatchResult!=NULL)
	{	
		//设置字体
		pDC->SetTextColor(RGB(255,255,255));

		//设置环境
		pDC->SetTextColor(RGB(86,24,20));
		CFont * pOldFont=pDC->SelectObject(&m_AccountsFont);		

		//绘画昵称
		TCHAR szNickName[64];		
		_sntprintf(szNickName,CountArray(szNickName),TEXT("%s:"),m_pMatchResult->szNickName);
		pDC->DrawText(szNickName,&CRect(nXPos+20,nYPos+74,nXPos+360,nYPos+90),DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//绘画赛事名称
		pDC->SetTextColor(RGB(190,40,20));
		pDC->DrawText(m_pMatchResult->szMatchName,&CRect(nXPos+115,nYPos+104,nXPos+310,nYPos+120),DT_CENTER|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//绘画名次
		TCHAR szMatchRank[5];
		_sntprintf(szMatchRank,CountArray(szMatchRank),TEXT("%d"),m_pMatchResult->wMatchRank);
		pDC->DrawText(szMatchRank,&CRect(nXPos+371,nYPos+103,nXPos+414,nYPos+119),DT_CENTER|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//设置环境	
		pDC->SelectObject(&m_RewardFont);
		pDC->SetTextColor(RGB(190,40,20));

		//变量定义
		int nIndex=0;
		CString strText;

		//游戏币奖励
		if (m_pMatchResult->lGold>0)
		{
			strText.Format(TEXT("游戏币: %I64d"), m_pMatchResult->lGold);			
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+159,nXPos+360,nYPos+184),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//奖牌奖励
		if (m_pMatchResult->lIngot>0)
		{
			strText.Format(TEXT("元	  宝: %I64d"), m_pMatchResult->lIngot);			
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+159+nIndex*25,nXPos+360,nYPos+184+nIndex*25),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//实物奖励
		if (m_pMatchResult->dwExperience>0)
		{
			strText.Format(TEXT("经	  验: %d"), m_pMatchResult->dwExperience);
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+159+nIndex*25,nXPos+360,nYPos+184+nIndex*25),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//设置环境
		pDC->SetTextColor(RGB(247,57,23));
		pDC->SelectObject(&m_WatermarkFont);
	}
}

//初始消息
BOOL CDlgMatchAwards::OnInitDialog()
{
	__super::OnInitDialog();

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//查询接口
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIStringMessage=m_pIClientKernel->GetIStringMessage();

	//变量定义
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//变量定义
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());

	//设置窗口
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.top=LAYERED_SIZE+16;	
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_XSIZE,ROUND_YSIZE);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_XSIZE,ROUND_YSIZE),false);

	//居中窗口
    CenterWindow();

	return TRUE; 
}

//左键按下
VOID CDlgMatchAwards::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);
	
	//模拟按标题
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}
}


//拷贝屏幕
HBITMAP CDlgMatchAwards::CopyScreenToBitmap(LPRECT lpRect)
{
	//参数校验
	if (IsRectEmpty(lpRect)) return NULL; 

	//变量定义
	HDC hScrDC,hMemDC; 
	HBITMAP hOldBitmap,hBitmap;
	int nX, nY, nX2, nY2; 
	int nWidth, nHeight; 
	int xScrn, yScrn; 
	
	//创建设备
	hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hScrDC); 

	//设置变量
	nX = lpRect->left; 
	nY = lpRect->top; 
	nX2 = lpRect->right; 
	nY2 = lpRect->bottom; 

	//获取设备信息
	xScrn = GetDeviceCaps(hScrDC, HORZRES); 
	yScrn = GetDeviceCaps(hScrDC, VERTRES); 

	//调整变量
	if (nX < 0) nX = 0; 
	if (nY < 0) nY = 0; 
	if (nX2 > xScrn) nX2 = xScrn; 
	if (nY2 > yScrn) nY2 = yScrn; 

	//设置变量
	nWidth = nX2 - nX; 
	nHeight = nY2 - nY; 

	//创建兼容位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 	

	//拷贝画布
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY|CAPTUREBLT); 

	//还原设置
	hBitmap =(HBITMAP)SelectObject(hMemDC, hOldBitmap); 

	//释放设备
	DeleteDC(hScrDC); 
	DeleteDC(hMemDC); 

	return hBitmap; 
}

//游戏截屏
bool CDlgMatchAwards::CaptureGameScreen(LPCTSTR pszSavePath)
{
	if(AfxGetMainWnd()==NULL) return false;

	//变量定义
	CRect rcWindow;
	m_SkinLayered.GetWindowRect(&rcWindow);

	//拷贝屏幕
	HBITMAP hBitmap = CopyScreenToBitmap(rcWindow);

	//拷贝到粘贴板
	CopyBitmapToClipBoard(hBitmap);

	//变量定义
	HDC hDC; 
	int iBits; 
	WORD wBitCount; 
	DWORD dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten; 
	BITMAP Bitmap; 
	BITMAPFILEHEADER bmfHdr; 
	BITMAPINFOHEADER bi; 
	LPBITMAPINFOHEADER lpbi; 
	HANDLE fh,hDib,hPal,hOldPal=NULL; 

	//创建设备上下文
	HDC hWndDC = CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL); 
	hDC = ::CreateCompatibleDC( hWndDC ) ; 
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
	DeleteDC(hDC); 

	//设置比特位
	if (iBits <= 1) wBitCount = 1; 
	else if (iBits <= 4) wBitCount = 4; 
	else if (iBits <= 8) wBitCount = 8; 
	else if (iBits <= 24) wBitCount = 24; 
	else wBitCount = 24 ; 

	//计算调色板大小 
	if (wBitCount <= 8) 
	dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD); 

	//设置位图信息头结构 
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap); 
	bi.biSize = sizeof(BITMAPINFOHEADER); 
	bi.biWidth = Bitmap.bmWidth; 
	bi.biHeight = Bitmap.bmHeight; 
	bi.biPlanes = 1; 
	bi.biBitCount = wBitCount; 
	bi.biCompression = BI_RGB; 
	bi.biSizeImage = 0; 
	bi.biXPelsPerMeter = 0; 
	bi.biYPelsPerMeter = 0; 
	bi.biClrUsed = 0; 
	bi.biClrImportant = 0; 

	//计算位图大小
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ; 

	//分配内存 
	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER)); 
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	*lpbi = bi; 

	// 处理调色板 
	hPal = GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hDC = ::GetDC(NULL); 
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
		RealizePalette(hDC); 
	} 

	// 获取该调色板下新的像素值 
	GetDIBits(hDC,hBitmap,0,(UINT) Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize,(LPBITMAPINFO ) 
	lpbi,DIB_RGB_COLORS); 

	//恢复调色板 
	if (hOldPal) 
	{ 
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC); 
		::ReleaseDC(NULL, hDC); 
	} 

	//创建位图文件 
	fh = CreateFile(pszSavePath,GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE) return false; 

	// 设置位图文件头 
	bmfHdr.bfType = 0x4D42;
	dwDIBSize = sizeof(BITMAPFILEHEADER) 
	+ sizeof(BITMAPINFOHEADER) 
	+ dwPaletteSize + dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) 
	+ (DWORD)sizeof(BITMAPINFOHEADER) 
	+ dwPaletteSize; 

	// 写入位图文件头 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 

	// 写入位图文件其余内容 
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,&dwWritten, NULL); 

	//清除 
	GlobalUnlock(hDib); 
	GlobalFree(hDib); 
	CloseHandle(fh); 

	return true; 
}

//拷贝位图
bool CDlgMatchAwards::CopyBitmapToClipBoard(HBITMAP hBitmap)
{
	//变量定义
	HWND hWnd=AfxGetMainWnd()->GetSafeHwnd();

	//打开剪切板
	if (::OpenClipboard(hWnd)==FALSE) return false;
	if (::EmptyClipboard()==FALSE) { CloseClipboard(); return false; }

	//复制位图
	::SetClipboardData(CF_BITMAP,hBitmap);  

	//环境设置
	::CloseClipboard();

	return true;
}

//关闭消息
VOID CDlgMatchAwards::OnBnClickedClose()
{
	__super::OnCancel();

	//销毁窗口
	DestroyWindow();

	//关闭主窗口
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
}

//拍照消息
VOID CDlgMatchAwards::OnBnClickedTakePhoto()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	TCHAR szBitmapFilePath[MAX_PATH]={0};
	_sntprintf(szBitmapFilePath,CountArray(szBitmapFilePath),TEXT("%s\\MatchAwards"),szDirectory);

	//创建目录
	SHCreateDirectoryEx(NULL,szBitmapFilePath,NULL);

	//变量定义
	TCHAR szBitmapFile[MAX_PATH]={0};
	_sntprintf(szBitmapFile,CountArray(szBitmapFile),TEXT("%s\\%d_%d.jpg"),szBitmapFilePath,time(NULL),GetTickCount());	

	//执行截屏
	if(CaptureGameScreen(szBitmapFile))
	{
		//变量定义
		TCHAR szMessage[MAX_PATH]={0};
		_sntprintf(szMessage,CountArray(szMessage),TEXT("复制成功，可右键粘贴在QQ对话框,图像已保存为: %s"),szBitmapFile);

		//插入消息
		if(m_pIStringMessage!=NULL)
		{
			m_pIStringMessage->InsertPromptString(szMessage);
		}

		//播放声音
		g_GlobalUnits.PlayGameSound(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("IDR_TAKE_PHOTO"));
	}
}