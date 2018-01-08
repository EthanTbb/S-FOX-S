#include "StdAfx.h"
#include "DlgUpdateCheck.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//时间标识
#define IDT_STATUS					20									//第一状态
#define IDT_STATUS_SPEED            3                                   //第二状态
#define TIMER_STATUS				15									//状态时间
#define TIMER_STATUS_SPEED		    5									//状态时间

//时间标识
#define IDI_CHECK_PROCESS			200									//进程检测

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUpdateCheckSink, CWnd)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgUpdateCheck, CDialog)

	//系统消息
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//分层消息
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()

	//自定消息
	ON_MESSAGE(WM_DOWNLOAD_FINISH,OnDownLoadConfigFinish)

END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUpdateCheckSink::CUpdateCheckSink()
{
	//文件信息
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;
	m_nUpdateCount=0;

	//状态变量
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//内核信息
	m_pFileData=NULL;
	m_pDlgUpdateCheck=NULL;

	return;
}

//析构函数
CUpdateCheckSink::~CUpdateCheckSink()
{
	//释放资源
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);
}

//接口查询
VOID * CUpdateCheckSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//关闭下载
bool CUpdateCheckSink::CloseDownLoad()
{
	//关闭窗口
	DestroyWindow();

	//关闭下载
	m_DownLoad.CloseDownLoad();

	//设置状态
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//释放资源
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);

	return true;
}

//执行下载
bool CUpdateCheckSink::PerformDownLoad(LPCTSTR pszDownLoadFile)
{
	//效验状态
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_LINK;

	//执行下载
	m_DownLoad.PerformDownLoad(pszDownLoadFile,QUERY_ME_INTERFACE(IUnknownEx),0);

	return true;
}

//下载异常
bool CUpdateCheckSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//下载异常
	if(m_pDlgUpdateCheck!=NULL) m_pDlgUpdateCheck->OnEventDownLoadError(IDOK);	

	return true;
}

//下载状态
bool CUpdateCheckSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//设置状态
	switch (DownLoadStatus)
	{
	case DownLoadStatus_Service:		//服务状态
		{
			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_READ;

			break;
		}
	case DownLoadStatus_Conclude:		//完成状态
		{
			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_SETUP;

			//下载完成
			if(m_pDlgUpdateCheck!=NULL) m_pDlgUpdateCheck->OnEventCheckFinish(PerformCheckSum());			

			break;
		}
	}

	return true;
}

//下载数据
bool CUpdateCheckSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//写入数据
	CopyMemory(&m_pFileData[m_dwDownLoadSize],pcbMailData,wStreamSize);

	//更新大小
	m_dwDownLoadSize+=wStreamSize;

	return true;
}

//下载信息
bool CUpdateCheckSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//设置变量
	m_dwTotalFileSize+=dwTotalFileSize;

	//分配内存
	try
	{
		//分配内存
		m_pFileData = new BYTE[m_dwTotalFileSize];
		if(m_pFileData==NULL) throw TEXT("内存不足,资源申请失败！");

		//初始数据
		ZeroMemory(m_pFileData,sizeof(m_pFileData));

		return true;
	}
	catch(...)
	{
		ASSERT(FALSE);

		//释放资源
		SafeDeleteArray(m_pFileData);

		//关闭下载
		CloseDownLoad();
	}

	return false;
}

//执行校验
bool CUpdateCheckSink::PerformCheckSum()
{
	//变量定义
	DWORD dwUnCompressSize=0;
	BYTE * pUnCompressData=NULL;

	try
	{
		//计算大小
		dwUnCompressSize=m_dwTotalFileSize*5;

		//分配内存		
		pUnCompressData=new BYTE[dwUnCompressSize];
		if(pUnCompressData==NULL) throw TEXT("内存不足,资源申请失败！");

		//数据解压缩
		dwUnCompressSize=CWHService::UnCompressData(m_pFileData,m_dwTotalFileSize,pUnCompressData,dwUnCompressSize);

		//解压失败
		if(dwUnCompressSize==0) return false;
	}
	catch(...)
	{
		ASSERT(FALSE);
		SafeDeleteArray(pUnCompressData);

		return false;
	}	

	//变量定义
	WORD wDataIndex=0;
	tagUpdateInfo UpdateInfo;
	tagUpdateItem UpdateItem;

	//读取数据
	UpdateInfo.wStructSize=*(WORD *)pUnCompressData;	
	CopyMemory(&UpdateInfo,pUnCompressData,UpdateInfo.wStructSize);
	wDataIndex += UpdateInfo.wStructSize;	

	//变量定义
	CString strFileCheckSum;
	TCHAR szFilePath[MAX_PATH]=TEXT("");	

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//目标目录
	TCHAR szConfigSavePath[MAX_PATH]=TEXT("");
	_sntprintf(szConfigSavePath,CountArray(szConfigSavePath),TEXT("%s\\Update.WHD"),szWorkDirectory);

	//变量定义
	TCHAR  szFileName[MAX_PATH]=TEXT("");
	TCHAR  szFileCheckSum[64]=TEXT("");
	TCHAR  szExeName[MAX_PATH]=TEXT("");

	//变量定义
	CString  szKeyText,szLocalFileCheckSum;
	
	//格式字符
	_sntprintf(szExeName,CountArray(szExeName),TEXT("%s.exe"),AfxGetApp()->m_pszExeName);

	//变量定义
	WORD wUpdateIndex=0;

	//文件校验
	for(INT_PTR nIndex=0;nIndex<UpdateInfo.wUpdateCount;nIndex++)
	{
		//清除数据
		ZeroMemory(&UpdateItem,sizeof(UpdateItem));

		//读取数据
		UpdateItem.wStructSize=*(WORD *)&pUnCompressData[wDataIndex];	
		CopyMemory(&UpdateItem,&pUnCompressData[wDataIndex],UpdateItem.wStructSize);

		//设置变量
		wDataIndex += UpdateItem.wStructSize;

		//变量定义
		CString  szFileNameEx(UpdateItem.szFileName);

		//更新自己
		if(lstrcmp(szExeName,szFileNameEx.Right(lstrlen(szExeName)))==0)
		{
			//获取校验
			_sntprintf(szFilePath,sizeof(szFilePath),TEXT("%s/%s"),szWorkDirectory,szExeName);
		}
		else
		{
			//获取校验
			_sntprintf(szFilePath,sizeof(szFilePath),TEXT("%s/%s"),szWorkDirectory,CA2CT(UpdateItem.szFileName));
		}
		
		//获取校验和
		szLocalFileCheckSum =CWHMD5Checksum::GetMD5(szFilePath);

		//校验比较
		if(lstrcmp(szLocalFileCheckSum,CA2CT(UpdateItem.szFileCheckSum))!=0)
		{
			//写入文件
			szKeyText.Format(TEXT("FileIndex%d"),m_nUpdateCount);			
			WritePrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,CA2CT(UpdateItem.szFileName),szConfigSavePath);

			//写入文件
			szKeyText.Format(TEXT("FileCheckNum%d"),m_nUpdateCount++);			
			WritePrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,CA2CT(UpdateItem.szFileCheckSum),szConfigSavePath);
		}
	}

	//数目判断
	if(m_nUpdateCount>0)
	{
		//变量定义
		CString szUpdateFileCount;
		szUpdateFileCount.Format(TEXT("%d"),m_nUpdateCount);	

		//写入文件
		WritePrivateProfileString(TEXT("UPDATEFILELIST"),TEXT("UPDATEFILECOUNT"),szUpdateFileCount,szConfigSavePath);
		WritePrivateProfileString(TEXT("UPDATEFILELIST"),TEXT("FILENETADDRESS"),CA2CT(UpdateInfo.szDownLoadAddress),szConfigSavePath);
	}

	//释放内存
	SafeDeleteArray(pUnCompressData);

	return m_nUpdateCount>0;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgUpdateCheck::CDlgUpdateCheck() : CDialog(IDD_DLG_UPDATE_CHECK)
{
	//状态变量
	m_uImageIndex=0;

	//数据变量
	m_pUpdateCheckSink = NULL;

	m_bCheckFinish = FALSE;

	m_bNeedUpdate = FALSE;

	return;
}

//析构函数
CDlgUpdateCheck::~CDlgUpdateCheck()
{
	//释放对象
	SafeDelete(m_pUpdateCheckSink);
}

//控件绑定
VOID CDlgUpdateCheck::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//消息解释
BOOL CDlgUpdateCheck::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgUpdateCheck::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(szProduct);

	//设置控件
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	//加载资源
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));

	//设置大小
	CSize SizeWindow(ImageStatus.GetWidth(),ImageStatus.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//居中窗口
	CenterWindow();

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=8;
	rcUnLayered.left=8;
	rcUnLayered.right=rcWindow.Width()-8;
	rcUnLayered.bottom=rcWindow.Height()-8;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageStatus,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	//更新窗口
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	//工作目录
	TCHAR szMouduleFileName[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szMouduleFileName,CountArray(szMouduleFileName));

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	m_pszDownLoadFile.Format(TEXT("%s/DownLoad/Client/Update.txt"),pGlobalWebLink->GetUpdateLink());

	//下载配置
	DownLoadConfig(m_pszDownLoadFile);

	return TRUE;
}

//更新数目
INT CDlgUpdateCheck::GetUpdateCount()
{
	//指针判断
	if(m_pUpdateCheckSink==NULL) return 0;

	return m_pUpdateCheckSink->m_nUpdateCount;
}

//下载配置
VOID CDlgUpdateCheck::DownLoadConfig(LPCTSTR pszConfigName)
{
	//创建对象
	if(m_pUpdateCheckSink==NULL)
	{
		try
		{
			m_pUpdateCheckSink = new CUpdateCheckSink;
			ASSERT(m_pUpdateCheckSink);
		}
		catch(...)
		{
			ASSERT(FALSE);
			SafeDelete(m_pUpdateCheckSink);
		};
	}

	//开始下载
	if(m_pUpdateCheckSink!=NULL) 
	{
		m_pUpdateCheckSink->m_pDlgUpdateCheck=this;
		m_pUpdateCheckSink->PerformDownLoad(pszConfigName);
	}

	return;
}

//设置消息
VOID CDlgUpdateCheck::SetWindowDescribe(LPCTSTR pszDescribe)
{
	//设置描述
	m_strDescribe=pszDescribe;

	return;
}

//通知事件
VOID CDlgUpdateCheck::OnEventCheckFinish(BOOL bNeedUpdate)
{
	m_bNeedUpdate = bNeedUpdate;
	m_bCheckFinish = TRUE;
}

//出错事件
VOID CDlgUpdateCheck::OnEventDownLoadError(int nResult)
{
	//结束模态
	EndDialog(nResult);
}

//时间消息
VOID CDlgUpdateCheck::OnTimer(UINT nIDEvent)
{
	//设置进度
	if (nIDEvent==IDT_STATUS)
	{
		//设置索引
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		if (m_bCheckFinish&&m_uImageIndex>30)
		{
			KillTimer(nIDEvent);
			//设置控件
			SetTimer(IDT_STATUS_SPEED,TIMER_STATUS_SPEED,NULL);

			if (m_bNeedUpdate)
			{
				SetWindowDescribe(TEXT("准备更新..."));
			}
			else
			{
				SetWindowDescribe(TEXT("检查完成,启动大厅..."));
			}
			
		}

		return;
	}
	else if ( nIDEvent = IDT_STATUS_SPEED)
	{
		//设置索引
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+5L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		if (m_bCheckFinish && m_uImageIndex>=100)
		{
			KillTimer(nIDEvent);
			//结束模态
			EndDialog(m_bNeedUpdate==TRUE?IDCANCEL:IDOK);
		}

	}

	__super::OnTimer(nIDEvent);
}

//绘画背景
BOOL CDlgUpdateCheck::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置环境
	pBufferDC->SetBkMode(TRANSPARENT);

	//绘画背景
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));
	ImageStatus.DrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0);

	//加载资源
	CPngImage ImageStatusLine;
	ImageStatusLine.LoadImage(AfxGetInstanceHandle(),TEXT("PROGRESS_LINE"));

	//获取大小
	CSize SizeStatusLine;
	SizeStatusLine.SetSize(ImageStatusLine.GetWidth(),ImageStatusLine.GetHeight()/2);

	//绘画进度
	ImageStatusLine.DrawImage(pBufferDC,(rcClient.Width()-SizeStatusLine.cx)/2,50,SizeStatusLine.cx,SizeStatusLine.cy,0,0);
	ImageStatusLine.DrawImage(pBufferDC,(rcClient.Width()-SizeStatusLine.cx)/2,50,SizeStatusLine.cx*m_uImageIndex/100L,SizeStatusLine.cy,0,SizeStatusLine.cy);


	//文字位置
	CRect rcDescribe;
	rcDescribe.SetRect(140,30,140+184,30+18);

	//变量定义
	CFont NewFont;
	VERIFY(NewFont.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体")));
	CFont * pOldFont = pBufferDC->SelectObject(&NewFont);

	//绘画文字
	pBufferDC->SetTextColor(RGB(255,135,0));
	pBufferDC->DrawText(m_strDescribe,rcDescribe,DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);	

	//还原设置
	pBufferDC->SelectObject(pOldFont);
	NewFont.DeleteObject();	

	return TRUE;
}

//显示消息
VOID CDlgUpdateCheck::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//鼠标消息
VOID CDlgUpdateCheck::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//位置改变
VOID CDlgUpdateCheck::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

LRESULT CDlgUpdateCheck::OnDownLoadConfigFinish(WPARAM wParam, LPARAM lParam)
{
	//文件校验
	if(m_pUpdateCheckSink!=NULL) m_pUpdateCheckSink->PerformCheckSum();

	return 0;
}