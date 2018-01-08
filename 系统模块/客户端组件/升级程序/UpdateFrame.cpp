#include "StdAfx.h"
#include "UpdateFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//阴影定义
#define SHADOW_CX					5									//阴影宽度
#define SHADOW_CY					5									//阴影高度

//位置定义
#define CX_BORAD					5									//边框宽度
#define CY_BORAD					5									//边框高度

//屏幕位置
#define BORAD_SIZE					5									//边框大小
#define CAPTION_SIZE				79									//标题大小
#define LESS_SCREEN_CX              1024                                //屏幕大小  
#define LESS_SCREEN_CY              768                                 //屏幕大小

//数目定义
#define MIN_LINE					3									//可视数目
#define MAX_LINE					6									//可视数目
#define MAX_RETRY_COUNT				3									//重试次数
#define FLASH_ITEM_TIMES			8									//闪烁次数

//时间标识
#define IDI_FLASH_ITEM				100									//进程检测
#define IDI_UPDATE_STATUS			101									//更新状态

//时间标识
#define IDI_CHECK_PROCESS			200									//进程检测

//窗口标识
#define IDC_CLOSE					300									//关闭按钮
#define IDC_DOWNLOAD_MANAGER        301                                 //窗口标识
#define IDC_SELFUPDATE				302									//手动按钮

//更新间隔
#define UPDATE_TIMESPACE			24									//更新间隔	

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUDownLoadSink, CWnd)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDownLoadManager, CWnd)

	//自定消息
	ON_MESSAGE(WM_ITEM_DOWNLOAD_Finish,OnItemDownLoadFinishMessage)
	ON_MESSAGE(WM_ITEM_DOWNLOAD_UPDATE,OnItemDownLoadUpdateMessage)

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CGameUpdateFrame, CFrameWnd)
	
	//系统消息
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	ON_WM_NCDESTROY()

	//按钮消息
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_SELFUPDATE, OnBnClickedSelfUpdate)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////


//构造函数
CUDownLoadSink::CUDownLoadSink()
{
	//下载速度
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//文件信息
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;

	//状态变量
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//任务属性
	ZeroMemory(m_szTaskName,sizeof(m_szTaskName));

	//内核信息
	m_pDownLoadManager=NULL;

	return;
}

//析构函数
CUDownLoadSink::~CUDownLoadSink()
{
}

//接口查询
VOID * CUDownLoadSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//关闭下载
bool CUDownLoadSink::CloseDownLoad()
{
	//关闭窗口
	DestroyWindow();

	//关闭下载
	m_DownLoad.CloseDownLoad();

	//设置状态
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//关闭文件
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();

	return true;
}

//执行下载
bool CUDownLoadSink::PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad)
{
	//效验状态
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//下载速度
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=GetTickCount();

	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_LINK;
	lstrcpyn(m_szTaskName,pszFileName,CountArray(m_szTaskName));

	//构造地址
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s\\%s"),pszNetAddress,pszFileName);

	//断点续传
	if(bBreakpointDownLoad && LoadDownFile(szDownLoadUrl)) 
	{
		//变量定义
		DWORD dwFileSize = (DWORD)m_DownFile.GetLength();		

		//执行下载
		m_DownLoad.PerformDownLoad(szDownLoadUrl,QUERY_ME_INTERFACE(IUnknownEx),dwFileSize);
	}
	else
	{
		//执行下载
		m_DownLoad.PerformDownLoad(szDownLoadUrl,QUERY_ME_INTERFACE(IUnknownEx),0);
	}

	//发送消息
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd())) 
	{
		m_pDownLoadManager->PostMessage(WM_ITEM_DOWNLOAD_UPDATE,0,0);
	}

	return true;
}

VOID CUDownLoadSink::PerformInstall()
{
	//下载文件
	TCHAR szTargetName[MAX_PATH]=TEXT("");
	TCHAR strSourceName[MAX_PATH]=TEXT("");
	_sntprintf(strSourceName,CountArray(strSourceName),TEXT("%s"),m_DownFile.GetFilePath());

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//文件路径
	_sntprintf(szTargetName,CountArray(szTargetName),TEXT("%s\\%s"),szWorkDirectory,m_szTaskName);

	//修改目录
	INT nIndex = lstrlen(szTargetName);
	while(--nIndex>=0) if(szTargetName[nIndex]==TEXT('/')) szTargetName[nIndex]=TEXT('\\');

	//关闭文件
	m_DownFile.Close();

	//变量定义
	tagDownLoadResult * pDownLoadResult = new tagDownLoadResult;
	pDownLoadResult->bDownLoadSuccess=true;
	lstrcpyn(pDownLoadResult->szSourceFilePath,strSourceName,CountArray(pDownLoadResult->szSourceFilePath));
	lstrcpyn(pDownLoadResult->szTargetFilePath,szTargetName,CountArray(pDownLoadResult->szTargetFilePath));

	//发送消息
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd()))
	{
		m_pDownLoadManager->PostMessage(WM_ITEM_DOWNLOAD_Finish,(WPARAM)pDownLoadResult);
	}

	return;
}

//重置变量
VOID CUDownLoadSink::ResetDownLoadSink()
{
	//下载速度
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//文件信息
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;

	//状态变量
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//任务属性
	ZeroMemory(m_szTaskName,sizeof(m_szTaskName));
}

//下载异常
bool CUDownLoadSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//发送消息
	if(m_pDownLoadManager!=NULL ) 
	{
		if(m_DownFile.m_hFile!=INVALID_HANDLE_VALUE && m_DownFile.GetLength()>0)
		{
			//关闭文件
			m_DownFile.Close();
		}

		//变量定义
		tagDownLoadResult * pDownLoadResult = new tagDownLoadResult;
		pDownLoadResult->bDownLoadSuccess=false;
		pDownLoadResult->szSourceFilePath[0]=0;
		pDownLoadResult->szTargetFilePath[0]=0;

		//发送通知
		m_pDownLoadManager->PostMessage(WM_ITEM_DOWNLOAD_Finish,(WPARAM)pDownLoadResult);
	}

	return true;
}

//下载状态
bool CUDownLoadSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
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

			//执行安装
			PerformInstall();
			
			break;
		}
	}

	return true;
}

//下载数据
bool CUDownLoadSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//写入文件
	m_dwDownLoadSize+=wStreamSize;
	m_DownFile.Write(pcbMailData,wStreamSize);

	//计算速度
	if (GetTickCount()>=(m_dwLastCalcTime+2000L))
	{
		//计算速度
		m_dwDownSpeed=(m_dwDownLoadSize-m_dwLastCalcSize)/(GetTickCount()-m_dwLastCalcTime);

		//设置变量
		m_dwLastCalcTime=GetTickCount();
		m_dwLastCalcSize=m_dwDownLoadSize;
	}

	//发送消息
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd())) 
	{
		m_pDownLoadManager->SendMessage(WM_ITEM_DOWNLOAD_UPDATE,m_dwDownLoadSize==m_dwTotalFileSize);
	}
	
	return true;
}

//下载信息
bool CUDownLoadSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//设置变量
	m_dwTotalFileSize+=dwTotalFileSize;

	//断点续传
	if(bPartialConent==true)
	{
		//设置变量
		m_dwTotalFileSize += (DWORD)m_DownFile.GetLength();
		m_dwDownLoadSize += (DWORD)m_DownFile.GetLength();
	}
	else		
	{
		//释放文件
		if(m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();

		if(CreateDownFile(m_szTaskName)==false)
		{
			//关闭下载
			CloseDownLoad();

			return false;
		}
	}

	//发送消息
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd())) 
	{
		m_pDownLoadManager->SendMessage(WM_ITEM_DOWNLOAD_UPDATE);
	}

	return true;
}


//加载文件
bool CUDownLoadSink::LoadDownFile(LPCTSTR pszLocation)
{
	//效验状态
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//目标目录
	TCHAR szDownLoadPath[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadPath,CountArray(szDownLoadPath),TEXT("%s\\DownLoad"),szWorkDirectory);

	//文件名字
	DWORD dwIndex=lstrlen(pszLocation);
	while ((dwIndex>0)&&(pszLocation[dwIndex]!=TEXT('/'))) dwIndex--;

	//文件路径
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	LPCTSTR pszFileName=&pszLocation[dwIndex+1];
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\%s"),szDownLoadPath,pszFileName);

	//创建文件
	if (m_DownFile.Open(szFilePath,CFile::modeRead|CFile::modeWrite|CFile::shareDenyNone)==FALSE) return false;

	//文件偏移
	m_DownFile.SeekToEnd();

	return true;
}

//创建文件
bool CUDownLoadSink::CreateDownFile(LPCTSTR pszLocation)
{
	//效验状态
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//设置变量
	LPTSTR pszDelete = (_tcsrchr(pszLocation, TEXT('?'))); 
	if(pszDelete!=NULL) pszDelete[0]=0;

	//文件路径
	TCHAR szFilePath[MAX_PATH]=TEXT("");	
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\DownLoad\\%s"),szWorkDirectory,pszLocation);	

	//目标目录
	TCHAR szDownLoadPath[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadPath,CountArray(szDownLoadPath),TEXT("%s\\DownLoad\\%s"),szWorkDirectory,pszLocation);
	
	//文件名字
	DWORD dwIndex=lstrlen(szDownLoadPath);
	while ((dwIndex>0)&&(szDownLoadPath[dwIndex]!=TEXT('\\'))) dwIndex--;

	//截取目录
	szDownLoadPath[dwIndex]=0;	

	//创建目录
	SHCreateDirectoryEx(NULL,szDownLoadPath,NULL);

	//创建文件
	if (m_DownFile.Open(szFilePath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDownLoadManager::CDownLoadManager()
{
	//任务属性
	m_dwTickCount=0;	
	m_dwTaskFinished=0;
	m_cbCurrRetryCount=0;
	m_dwTotalTaskCount=0;
	m_szDownLoadAddress[0]=0;

	//内核信息
	m_pDownLoadSink=NULL;
	m_pGameUpdateFrame=NULL;	

	return;
}

//析构函数
CDownLoadManager::~CDownLoadManager()
{
	//释放对象
	if(m_pDownLoadSink!=NULL)
	{
		m_pDownLoadSink->CloseDownLoad();
		SafeDelete(m_pDownLoadSink);
	}

	//释放对象
	while(m_DownLoadFileArray.GetCount()>0)
	{
		SafeDelete(m_DownLoadFileArray[0]);
		m_DownLoadFileArray.RemoveAt(0);
	}
}

//读取信息
bool CDownLoadManager::ReadDownLoadInfo()
{
	//变量定义
	TCHAR  szTargetFilePath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szTargetFilePath,CountArray(szTargetFilePath));
	_sntprintf(szTargetFilePath,CountArray(szTargetFilePath),TEXT("%s\\Update.WHD"),szTargetFilePath);

	//变量定义
	int nUpdateFileCount = GetPrivateProfileInt(TEXT("UPDATEFILELIST"),TEXT("UPDATEFILECOUNT"),0,szTargetFilePath);

	//网络地址
	GetPrivateProfileString(TEXT("UPDATEFILELIST"),TEXT("FILENETADDRESS"),TEXT(""),m_szDownLoadAddress,sizeof(m_szDownLoadAddress),szTargetFilePath);

	//变量定义
	TCHAR szKeyText[32]=TEXT("");

	//读取文件
	for(int nIndex=0;nIndex<nUpdateFileCount;nIndex++)
	{
		tagDownloadItem * pDownloadItem = new tagDownloadItem;
		if(pDownloadItem==NULL) break;

		//格式字符
		_sntprintf(szKeyText,CountArray(szKeyText),TEXT("FileIndex%d"),nIndex);
		GetPrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,TEXT(""),pDownloadItem->szDownloadFile,sizeof(pDownloadItem->szDownloadFile),szTargetFilePath);

		//格式字符
		_sntprintf(szKeyText,CountArray(szKeyText),TEXT("FileCheckNum%d"),nIndex);
		GetPrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,TEXT(""),pDownloadItem->szFileCheckSum,sizeof(pDownloadItem->szFileCheckSum),szTargetFilePath);

		//加入列表
		m_DownLoadFileArray.Add(pDownloadItem);
	}

	//设置变量
	m_dwTotalTaskCount = static_cast<DWORD>(m_DownLoadFileArray.GetCount());

	//删除文件
	DeleteFile(szTargetFilePath);

	return true;
}

//执行下载
bool CDownLoadManager::StartDownLoadTask()
{
	//指针判断
	if(m_pDownLoadSink==NULL)
	{
		try
		{
			//创建指针
			m_pDownLoadSink = new CUDownLoadSink();
			ASSERT(m_pDownLoadSink!=NULL);
			if(m_pDownLoadSink==NULL) return false;
		}
		catch(...)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//下载文件
	if(m_pDownLoadSink!=NULL)
	{
		//设置指针
		m_pDownLoadSink->m_pDownLoadManager=this;

		//文件下载
		if(m_DownLoadFileArray.GetCount()>0)
		{
			//执行下载
			m_pDownLoadSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile);

			//切换任务
			if(m_pGameUpdateFrame!=NULL) m_pGameUpdateFrame->OnEventSwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);
		}
	}

	return true;
}

//移动文件
VOID CDownLoadManager::PerformMoveFiles()
{
	//循环移动
	while(m_SourcePathArray.GetCount()>0)
	{
		//变量定义
		LPCTSTR szSourceFilePath,szTargetFilePath;

		//设置变量
		szSourceFilePath = m_SourcePathArray.GetAt(0);
		szTargetFilePath = m_TargetPathArray.GetAt(0);

		//文件目录
		TCHAR szFileDirectory[MAX_PATH]=TEXT("");
		_sntprintf(szFileDirectory,CountArray(szFileDirectory),TEXT("%s"),szTargetFilePath);
		(_tcsrchr(szFileDirectory, TEXT('\\')))[0] = 0; 

		//创建目录
		SHCreateDirectoryEx(NULL,szFileDirectory,NULL);

		//删除文件
		if (DeleteFile(szTargetFilePath)==FALSE && ::GetLastError()!=ERROR_FILE_NOT_FOUND)
		{
			//变量定义
			TCHAR szTargetNameEx[MAX_PATH]=TEXT("");	
			_sntprintf(szTargetNameEx,CountArray(szTargetNameEx),TEXT("%s.old"),szSourceFilePath);

			//删除文件
			DeleteFile(szTargetNameEx);

			//移动文件
			if (MoveFileEx(szTargetFilePath,szTargetNameEx,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
			{
				ASSERT(FALSE);	
			}

			//标记删除
			if (MoveFileEx(szTargetNameEx,NULL,MOVEFILE_DELAY_UNTIL_REBOOT|MOVEFILE_REPLACE_EXISTING)==FALSE)
			{
				ASSERT(FALSE);
			}
		}

		//移动文件
		if (MoveFileEx(szSourceFilePath,szTargetFilePath,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING))
		{
			//删除文件
			DeleteFile(szSourceFilePath);
		}

		//移除元素
		m_SourcePathArray.RemoveAt(0);
		m_TargetPathArray.RemoveAt(0);
	}

	return;
}

//状态消息
LRESULT CDownLoadManager::OnItemDownLoadFinishMessage(WPARAM wParam, LPARAM lParam)
{
	//提取数据
	tagDownLoadResult * pDownLoadResult=(tagDownLoadResult *)wParam;
	if(pDownLoadResult==NULL) return 1L;

	//变量定义
	tagDownLoadResult DownLoadResult;
	CopyMemory(&DownLoadResult,pDownLoadResult,sizeof(DownLoadResult));

	//释放内存
	SafeDelete(pDownLoadResult);

	//变量定义
	bool bDownLoadSuccess = DownLoadResult.bDownLoadSuccess;

	//移除元素
	if(bDownLoadSuccess==true)
	{
		//文件校验
		CString szLocalFileCheckSum =CWHMD5Checksum::GetMD5(DownLoadResult.szSourceFilePath);
		if(lstrcmp(szLocalFileCheckSum,m_DownLoadFileArray[0]->szFileCheckSum)!=0)
		{
			//退出提示
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("文件 %s 校验结果与服务器不匹配,请与客服人员联系！"),m_DownLoadFileArray[0]->szDownloadFile);

			tagDownloadItem * pDownLoadItem = m_DownLoadFileArray[0];

			MessageBox(szMessage,TEXT("温馨提示"));		

			//移动文件
			PerformMoveFiles();

			//关闭进程
			AfxGetMainWnd()->PostMessage(WM_CLOSE);

			return 0L;
		}

		//设置变量
		m_cbCurrRetryCount=0;
		++m_dwTaskFinished;	

		//释放对象
		SafeDelete(m_DownLoadFileArray[0]);
		m_DownLoadFileArray.RemoveAt(0);
	}
	else
	{
		if(++m_cbCurrRetryCount==MAX_RETRY_COUNT)
		{
			//退出提示
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("文件 %s 无法下载,可能由网络原因造成,请稍后重试！"),m_DownLoadFileArray.GetAt(0)->szDownloadFile);
			MessageBox(szMessage,TEXT("温馨提示"));

			//移动文件
			PerformMoveFiles();

			//关闭进程
			AfxGetMainWnd()->PostMessage(WM_CLOSE);

			return 0L;
		}
	}

	if(m_pDownLoadSink!=NULL)
	{
		//状态重置
		m_pDownLoadSink->ResetDownLoadSink();

		//记录文件
		if(DownLoadResult.szSourceFilePath[0]!=0 && DownLoadResult.szTargetFilePath[0]!=0)
		{
			m_SourcePathArray.Add(DownLoadResult.szSourceFilePath);
			m_TargetPathArray.Add(DownLoadResult.szTargetFilePath);
		}

		//文件下载
		if(m_DownLoadFileArray.GetCount()>0)
		{
			//记录时间戳
			m_dwTickCount=GetTickCount();

			//执行下载
			m_pDownLoadSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile,!bDownLoadSuccess);

			//切换任务
			if(m_pGameUpdateFrame!=NULL) m_pGameUpdateFrame->OnEventSwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);
		}
		else
		{
			//移动文件
			PerformMoveFiles();

			//下载完成
			if(m_pGameUpdateFrame!=NULL) m_pGameUpdateFrame->OnEventDownLoadEnd();
		}
	}

	return 0L;
}

//状态消息
LRESULT CDownLoadManager::OnItemDownLoadUpdateMessage(WPARAM wParam,LPARAM lParam)
{
	//更新界面
	if(m_pGameUpdateFrame!=NULL)
	{
		//判断间隔
		if(GetTickCount()-m_dwTickCount>=UPDATE_TIMESPACE || wParam==TRUE)
		{
			//更新界面
			m_dwTickCount=GetTickCount();		
			m_pGameUpdateFrame->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameUpdateFrame::CGameUpdateFrame()
{
	//指针变量
	m_pDownLoadManager = NULL;

	return;
}

//析构函数
CGameUpdateFrame::~CGameUpdateFrame()
{
	//释放资源
	if(m_pDownLoadManager!=NULL) 
	{
		SafeDelete(m_pDownLoadManager);
	}
}

//下载结束
VOID CGameUpdateFrame::OnEventDownLoadEnd()
{
	//工作目录
	TCHAR szMouduleFileName[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szMouduleFileName,CountArray(szMouduleFileName));

	//构造地址
	TCHAR szMainSetupPath[MAX_PATH]=TEXT("");
	_sntprintf(szMainSetupPath,CountArray(szMainSetupPath),TEXT("%s/GamePlaza.exe"),szMouduleFileName);

	//关闭窗口
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	//启动大厅
	ShellExecute(0,TEXT("open"),szMainSetupPath,TEXT("UPDATED"),NULL,true );

	return;
}

//切换任务
VOID CGameUpdateFrame::OnEventSwitchDownLoadTask(LPCTSTR pszFileName)
{
	//查找字符
	DWORD dwIndex = lstrlen(pszFileName);
	if(dwIndex>0)
	{
		while(--dwIndex>0 && pszFileName[dwIndex]!=TEXT('\\'));
	}
	
	//拷贝字符
	CopyMemory(m_pszFileName,pszFileName+(dwIndex>0?++dwIndex:0),CountArray(m_pszFileName));

	return;
}

//控件绑定
VOID CGameUpdateFrame::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//消息解释
BOOL CGameUpdateFrame::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//关闭消息
VOID CGameUpdateFrame::OnClose()
{
	__super::OnClose();

	return;
}

//销毁非客户区
VOID CGameUpdateFrame::OnNcDestroy()
{
	//销毁窗口
	if(m_pDownLoadManager!=NULL)
	{
		m_pDownLoadManager->DestroyWindow();
	}

	CFrameWnd::OnNcDestroy();
}


//创建函数
INT CGameUpdateFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置图标
	SetWindowText(TEXT("大厅升级"));

	//设置窗口
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//资源句柄
	HINSTANCE hInstance=AfxGetResourceHandle();

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btClose.SetButtonImage(IDB_BT_CLOSE_BACK,TEXT("BT_CLOSE"),hInstance,false,false);

	////创建按钮
	//CRect rcCreateSelf(0,0,0,0);
	//m_btSelfUpdate.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreateSelf,this,IDC_SELFUPDATE);
	//m_btSelfUpdate.SetButtonImage(IDB_BT_SELFUPDATE_BACK,TEXT("BT_SELFUPDATE"),hInstance,false,false);
	

	//加载图片
	CPngImage UpdateFrameBack;
	UpdateFrameBack.LoadImage(hInstance,TEXT("UPDATE_FRAME_BACK"));	

	//窗口位置
	CRect rcArce,rcFrame;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//设置变量
	rcFrame.left=(rcArce.Width()-UpdateFrameBack.GetWidth())/2;
	rcFrame.top=(rcArce.Height()-UpdateFrameBack.GetHeight())/2;
	rcFrame.right=rcFrame.left + UpdateFrameBack.GetWidth();
	rcFrame.bottom=rcFrame.top + UpdateFrameBack.GetHeight();

	//设置窗体
	SetWindowPos(NULL,rcFrame.left,rcFrame.top,rcFrame.Width(),rcFrame.Height(),SWP_NOZORDER);	

	//创建分层
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_SkinLayered.CreateLayered(m_hWnd);
	
	//调整资源
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyWindow(rcClient.Width(),rcClient.Height());

	//启动下载
	StartDownLoadTask();

	return 0;
}

//绘画背景
BOOL CGameUpdateFrame::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

	//绘画界面
	OnDrawClientArea(pBufferDC,rcClient.Width(),rcClient.Height());

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//调整大小
VOID CGameUpdateFrame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整资源
	RectifyWindow(cx,cy);
}

//显示消息
VOID CGameUpdateFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//位置改变
VOID CGameUpdateFrame::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//鼠标消息
VOID  CGameUpdateFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟按标题
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}


//绘画消息
VOID CGameUpdateFrame::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));

	//加载资源
	CPngImage ImageFrameBack;
	ImageFrameBack.LoadImage(AfxGetInstanceHandle(),TEXT("UPDATE_FRAME_BACK"));

	//绘画背景
	ImageFrameBack.DrawImage(pDC,0,0);

	//加载图标
	CPngImage ImageLogo;
	ImageLogo.LoadImage(AfxGetInstanceHandle(),TEXT("WND_LOGO"));

	//加载标题
	CPngImage ImageTitle;
	ImageTitle.LoadImage(AfxGetInstanceHandle(),TEXT("WND_TITLE"));

	//绘画背景
	ImageLogo.DrawImage(pDC,12,1);
	ImageTitle.DrawImage(pDC,35,2);

	//变量定义
	CPngImage ImageProgressLine;

	//加载图像
	ImageProgressLine.LoadImage(AfxGetInstanceHandle(),TEXT("PROGRESS_LINE"));

	//变量定义
	CRect rcClient(0,0,0,0);
	GetClientRect(rcClient);
	CSize ProgressLineSize(ImageProgressLine.GetWidth(),ImageProgressLine.GetHeight()/2);

	//变量定义
	int nProgressLeft = (rcClient.Width()-ProgressLineSize.cx)/2;

	//进度条背景
	ImageProgressLine.DrawImage(pDC,nProgressLeft,67,ProgressLineSize.cx,ProgressLineSize.cy,0,0);
	ImageProgressLine.DrawImage(pDC,nProgressLeft,112,ProgressLineSize.cx,ProgressLineSize.cy,0,0);

	//绘制进度
	if(m_pDownLoadManager!=NULL && m_pDownLoadManager->m_pDownLoadSink!=NULL)
	{
		//变量定义
		TCHAR szTotalProgress[64] = TEXT("");
		TCHAR szCurrProgress[64] = TEXT("");
		TCHAR szNetSpeed[32] = TEXT("");

		//变量定义
		DWORD  dwTotalTaskCount = m_pDownLoadManager->m_dwTotalTaskCount;
		DWORD  dwTaskFinished = m_pDownLoadManager->m_dwTaskFinished;
		DWORD  dwCurrTaskIndex = __min(dwTaskFinished+1,dwTotalTaskCount);
		DWORD  dwTotalFileSize = m_pDownLoadManager->m_pDownLoadSink->m_dwTotalFileSize;
		DWORD  dwDownLoadFileSize = m_pDownLoadManager->m_pDownLoadSink->m_dwDownLoadSize;

		//变量定义
		DWORD dwTotalProgress = 0,dwCurrProgress = 0;

		//设置变量
		if(dwTotalTaskCount>0) dwTotalProgress = (DWORD)(((double)dwTaskFinished/(double)dwTotalTaskCount)*(double)(ProgressLineSize.cx));
		if(dwTotalFileSize>0)  dwCurrProgress = (DWORD)(((double)dwDownLoadFileSize/(double)dwTotalFileSize)*(double)ProgressLineSize.cx);
		if(dwTotalTaskCount>0) dwTotalProgress += dwCurrProgress/dwTotalTaskCount; 

		//绘制进度
		ImageProgressLine.DrawImage(pDC,nProgressLeft,67,dwTotalProgress,ProgressLineSize.cy,0,ProgressLineSize.cy);
		ImageProgressLine.DrawImage(pDC,nProgressLeft,112,dwCurrProgress,ProgressLineSize.cy,0,ProgressLineSize.cy);

		//变量定义
		CFont NewFont;
		VERIFY(NewFont.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体")));

		//设置字体
		CFont * pOldFont = pDC->SelectObject(&NewFont);
		pDC->SetTextColor(RGB(255,135,0));

		//格式字符
		_sntprintf(szTotalProgress,CountArray(szTotalProgress),TEXT("%d个文件需要更新,当前更新第%d个..."),dwTotalTaskCount,dwCurrTaskIndex);
		_sntprintf(szCurrProgress,CountArray(szCurrProgress),TEXT("正在下载 %s"),m_pszFileName);
		_sntprintf(szNetSpeed,CountArray(szNetSpeed),TEXT("%d/%d KB"),dwDownLoadFileSize/1024,dwTotalFileSize/1024);

		//绘制信息
		pDC->TextOut(nProgressLeft+10,52,szTotalProgress);
		pDC->TextOut(nProgressLeft+10,95,szCurrProgress);
		pDC->TextOut(312,95,szNetSpeed);

		//还原字体
		pDC->SelectObject(pOldFont);
		NewFont.DeleteObject();
	}

	return;
}

//启动下载
VOID CGameUpdateFrame::StartDownLoadTask()
{
	//指针判断
	if(m_pDownLoadManager==NULL)
	{
		try
		{
			//创建指针
			m_pDownLoadManager = new CDownLoadManager();
			ASSERT(m_pDownLoadManager!=NULL);
			if(m_pDownLoadManager==NULL) return;

			//创建窗口
			m_pDownLoadManager->Create(NULL,NULL,WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,IDC_DOWNLOAD_MANAGER);
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	//启动下载
	if(m_pDownLoadManager!=NULL)
	{
		m_pDownLoadManager->m_pGameUpdateFrame = this;
		m_pDownLoadManager->ReadDownLoadInfo();
		m_pDownLoadManager->StartDownLoadTask();
	}

	return;
}

//调整资源
VOID CGameUpdateFrame::RectifyWindow(INT nWidth, INT nHeight)
{
	//调整判断
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//设置按钮		
		m_btClose.SetWindowPos(NULL,354,7,0,0,SWP_NOZORDER|SWP_NOSIZE);
		//m_btSelfUpdate.SetWindowPos(NULL,300,7,0,0,SWP_NOZORDER|SWP_NOSIZE);

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
			OnDrawClientArea(pBufferDC,nWidth,nHeight);

			//更新分层
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}
}

//关闭按钮
VOID CGameUpdateFrame::OnBnClickedClose()
{
	PostMessage(WM_CLOSE,0,0);
}

//手动按钮
VOID CGameUpdateFrame::OnBnClickedSelfUpdate()
{
	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("http://www.foxuc.cn/"));

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	PostMessage(WM_CLOSE,0,0);
}
//////////////////////////////////////////////////////////////////////////////////
