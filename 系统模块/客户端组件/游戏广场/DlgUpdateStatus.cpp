#include "StdAfx.h"
#include "Resource.h"
#include "DlgUpdateStatus.h"
//#include "PlatformFrame.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//时钟定义
#define IDI_UPDATE_STATUS			100									//更新时间
#define IDI_UPDATE_DELAYEND			101									//延时结束

//时间定义
#define TIMER_UPDATE_STATUS			60									//更新时间
#define TIMER_UPDATE_DELAYEND		50									//延时时间

//常量定义
#define MAX_RETRY_COUNT				3									//重试次数

//下载状态
#define DOWN_LOAD_IDLE				0									//空闲状态
#define DOWN_LOAD_LINK				1									//连接状态
#define DOWN_LOAD_READ				2									//读取状态
#define DOWN_LOAD_ERROR				3									//错误状态
#define DOWN_LOAD_FINISH			4									//下载完成

//常量定义
#define UPDATE_TIMESPACE			30									//更新间隔	
#define ANIMAL_FRAME_COUNT			12									//动画帧数

//下载消息
#define WM_DOWNLOAD_TASK_FINISH		(WM_USER+100)						//下载完成
#define WM_DOWNLOAD_TASK_UPDATE		(WM_USER+101)						//下载更新

//检查消息
#define WM_UPDATECHECK_ERROR		(WM_USER+102)						//检查错误
#define WM_UPDATECHECK_FINISH		(WM_USER+103)						//检查完成
#define WM_UPDATECHECK_UPDATEINFO	(WM_USER+104)						//更新信息
#define WM_UPDATECHECK_UPDATEITEM	(WM_USER+105)						//更新子项

//UI消息
#define WM_UI_UPDATECHECK		(WM_USER+111)							//更新检查
#define WM_UI_UPDATEDOWNLOAD	(WM_USER+112)							//更新下载
//////////////////////////////////////////////////////////////////////////////////
//消息映射

BEGIN_MESSAGE_MAP(CWndUpdateMessage, CWnd)

	//自定消息
	ON_MESSAGE(WM_DOWNLOAD_TASK_FINISH,OnMessageDownLoadTaskFinish)
	ON_MESSAGE(WM_DOWNLOAD_TASK_UPDATE,OnMessageDownLoadTaskUpdate)

	//自定消息
	ON_MESSAGE(WM_UPDATECHECK_ERROR,OnMessageUpdateCheckError)
	ON_MESSAGE(WM_UPDATECHECK_FINISH,OnMessageUpdateCheckFinish)
	ON_MESSAGE(WM_UPDATECHECK_UPDATEINFO,OnMessageUpdateCheckUpdateInfo)
	ON_MESSAGE(WM_UPDATECHECK_UPDATEITEM,OnMessageUpdateCheckUpdateItem)

	//自定消息
	ON_MESSAGE(WM_UI_UPDATECHECK,OnMessageUpdateUICheck)
	ON_MESSAGE(WM_UI_UPDATEDOWNLOAD,OnMessageUpdateUIDownload)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDownLoadTaskSink::CDownLoadTaskSink()
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
	m_pUpdateMessage=NULL;

	return;
}

//析构函数
CDownLoadTaskSink::~CDownLoadTaskSink()
{
}

//接口查询
VOID * CDownLoadTaskSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);

	return NULL;
}

//关闭下载
bool CDownLoadTaskSink::CloseDownLoad()
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
bool CDownLoadTaskSink::PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad)
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
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s/%s"),pszNetAddress,pszFileName);

	//断点续传
	if(bBreakpointDownLoad && LoadLocationFile(szDownLoadUrl)) 
	{
		//变量定义
		DWORD dwFileSize = (DWORD)m_DownFile.GetLength();		

		//执行下载
		m_DownLoad.PerformDownLoad(szDownLoadUrl,dwFileSize,QUERY_ME_INTERFACE(IUnknownEx));
	}
	else
	{
		//执行下载
		m_DownLoad.PerformDownLoad(szDownLoadUrl,0,QUERY_ME_INTERFACE(IUnknownEx));
	}

	return true;
}

//重置变量
VOID CDownLoadTaskSink::ResetDownLoadSink()
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
bool CDownLoadTaskSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//发送消息
	if(m_pUpdateMessage!=NULL ) 
	{
		if(m_DownFile.m_hFile!=INVALID_HANDLE_VALUE && m_DownFile.GetLength()>0)
		{
			//关闭文件
			m_DownFile.Close();
		}

		//消息通知
		m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_FINISH,FALSE);
	}

	return true;
}

//下载状态
bool CDownLoadTaskSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
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
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//关闭文件
			m_DownFile.Close();

			//发送消息
			if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_FINISH,TRUE);
			
			break;
		}
	}

	return true;
}

//下载数据
bool CDownLoadTaskSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
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
	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_UPDATE,m_dwDownLoadSize==m_dwTotalFileSize);
	
	tagUpdateUIDownload UIInfo;
	UIInfo.dwTotalFileSize = m_dwTotalFileSize;
	UIInfo.dwDownSpeed = m_dwDownSpeed;
	UIInfo.dwDownLoadSize = m_dwDownLoadSize;
	ZeroMemory(UIInfo.szCheckFile,sizeof(UIInfo.szCheckFile));
	lstrcpyn(UIInfo.szCheckFile,m_szTaskName,CountArray(UIInfo.szCheckFile));

	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_UI_UPDATEDOWNLOAD,(WPARAM)&UIInfo);

	return true;
}

//下载信息
bool CDownLoadTaskSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
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
		if(CreateDownLoadFile(pszLocation)==false)
		{
			//关闭下载
			CloseDownLoad();

			return false;
		}
	}

	//发送消息
	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_UPDATE);

	return true;
}

//加载文件
bool CDownLoadTaskSink::LoadLocationFile(LPCTSTR pszLocation)
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
bool CDownLoadTaskSink::CreateDownLoadFile(LPCTSTR pszLocation)
{
	//效验状态
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//目标文件
	TCHAR szLocationPath[MAX_PATH]=TEXT("");
	_sntprintf(szLocationPath,CountArray(szLocationPath),TEXT("%s\\%s"),szWorkDirectory,m_szTaskName);

	//目标目录
	TCHAR szLocationDirectory[MAX_PATH]=TEXT("");
	lstrcpyn(szLocationDirectory,szLocationPath,CountArray(szLocationDirectory));

	//文件名字
	DWORD dwIndex=lstrlen(szLocationDirectory);
	while ((dwIndex>0)&&(szLocationDirectory[dwIndex]!=TEXT('\\'))) dwIndex--;

	//文件路径
	szLocationDirectory[dwIndex+1]=0;

	//创建目录
	SHCreateDirectoryEx(NULL,szLocationDirectory,NULL);

	//创建文件
	if (m_DownFile.Open(szLocationPath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUpdateCheckSink::CUpdateCheckSink()
{
	//状态变量
	m_bCheckData=false;
	m_hCheckThread=NULL;
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//文件信息
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;	

	//内核信息
	m_pFileData=NULL;
	m_pUpdateMessage=NULL;

	return;
}

//析构函数
CUpdateCheckSink::~CUpdateCheckSink()
{	
	//关闭下载
	m_DownLoad.CloseDownLoad();

	//释放资源
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);

	//关闭句柄
	if(m_hCheckThread!=NULL) CloseHandle(m_hCheckThread);
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
	m_bCheckData=false;
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//释放资源
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);

	//关闭句柄
	if(m_hCheckThread!=NULL) 
	{
		CloseHandle(m_hCheckThread);
		m_hCheckThread=NULL;
	}

	return true;
}

//执行下载
bool CUpdateCheckSink::PerformDownLoad(LPCTSTR pszDownLoadFile)
{
	//效验状态
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//设置变量
	m_bCheckData=true;
	m_cbDownLoadStatus=DOWN_LOAD_LINK;

	//执行下载
	m_DownLoad.PerformDownLoad(pszDownLoadFile,0,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//下载异常
bool CUpdateCheckSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//下载异常
	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_UPDATECHECK_ERROR);

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
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//创建线程
			m_hCheckThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PerformCheckSum,(LPVOID)this,0L,0L);	

			break;
		}
	}

	return true;
}

//下载数据
bool CUpdateCheckSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//写入文件	
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

//校验函数
VOID CUpdateCheckSink::PerformCheckSum(LPVOID pThreadData)
{
	//获取对象
	CUpdateCheckSink * pUpdateCheckSink = (CUpdateCheckSink *)pThreadData;
	CWndUpdateMessage * pUpdateMessage = pUpdateCheckSink->m_pUpdateMessage;

	//判断状态
	if(pUpdateCheckSink->m_bCheckData == false) return;

	//变量定义
	DWORD dwUnCompressSize=0;
	BYTE * pUnCompressData=NULL;

	try
	{
		//计算大小
		dwUnCompressSize=pUpdateCheckSink->m_dwTotalFileSize*10;

		//分配内存		
		pUnCompressData=new BYTE[dwUnCompressSize];
		if(pUnCompressData==NULL) throw TEXT("内存不足,资源申请失败！");

		//数据解压缩
		dwUnCompressSize=CWHService::UnCompressData(pUpdateCheckSink->m_pFileData,pUpdateCheckSink->m_dwTotalFileSize,pUnCompressData,dwUnCompressSize);

		//解压失败
		if(dwUnCompressSize==0) 
		{
			//完成通知
			if(pUpdateMessage->m_hWnd != NULL) pUpdateMessage->PostMessage(WM_UPDATECHECK_FINISH);

			return;
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
		SafeDeleteArray(pUnCompressData);

		return;
	}	

	//变量定义
	WORD wDataIndex=0;
	tagUpdateInfo UpdateInfo;
	tagUpdateItem UpdateItem;
	tagUpdateUICheck UIInfo;

	//读取数据
	UpdateInfo.wStructSize=*(WORD *)pUnCompressData;	
	CopyMemory(&UpdateInfo,pUnCompressData,UpdateInfo.wStructSize);
	wDataIndex += UpdateInfo.wStructSize;	

	//更新信息
	if(UpdateInfo.wUpdateCount>0 && pUpdateMessage->m_hWnd)
	{
		pUpdateMessage->SendMessage(WM_UPDATECHECK_UPDATEINFO,(WPARAM)&UpdateInfo);
	}

	//变量定义
	CString strFileCheckSum;
	TCHAR szFilePath[MAX_PATH]=TEXT("");	

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	UIInfo.wTotalNum = UpdateInfo.wUpdateCount;
	UIInfo.wCurrNum = 0;	
	//文件校验
	for(INT_PTR nIndex=0;nIndex<UpdateInfo.wUpdateCount;nIndex++)
	{

		
		//状态判断
		if(pUpdateCheckSink->m_bCheckData == false) return;

		//清除数据
		ZeroMemory(&UpdateItem,sizeof(UpdateItem));

		//读取数据
		UpdateItem.wStructSize=*(WORD *)&pUnCompressData[wDataIndex];	
		CopyMemory(&UpdateItem,&pUnCompressData[wDataIndex],UpdateItem.wStructSize);

		//设置变量
		wDataIndex += UpdateItem.wStructSize;

		//设置路径
		_sntprintf(szFilePath,sizeof(szFilePath),TEXT("%s/%s"),szWorkDirectory,CA2CT(UpdateItem.szFileName));
		
		//更新UI
		UIInfo.wCurrNum++;
		ZeroMemory(UIInfo.szCheckFile,sizeof(UIInfo.szCheckFile));
		_sntprintf(UIInfo.szCheckFile,sizeof(UIInfo.szCheckFile),TEXT("%s"),CA2CT(UpdateItem.szFileName));
		if(pUpdateMessage->m_hWnd != NULL)
		{
			pUpdateMessage->SendMessage(WM_UI_UPDATECHECK,(WPARAM)&UIInfo);
		}

		//获取校验和
		strFileCheckSum =CWHMD5Checksum::GetMD5(szFilePath);

		//校验和比较
		if(lstrcmp(strFileCheckSum,CA2CT(UpdateItem.szFileCheckSum))!=0)
		{
			//更新子项
			if(pUpdateMessage->m_hWnd != NULL)
			{
				pUpdateMessage->SendMessage(WM_UPDATECHECK_UPDATEITEM,(WPARAM)&UpdateItem);
			}
		}
	}

	//释放资源
	SafeDeleteArray(pUnCompressData);

	//完成通知
	if(pUpdateMessage->m_hWnd != NULL) pUpdateMessage->PostMessage(WM_UPDATECHECK_FINISH);
}

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CWndUpdateMessage::CWndUpdateMessage()
{
	//设置接口
	m_pIUpdateViewSink=NULL;
}

//析构函数
CWndUpdateMessage::~CWndUpdateMessage()
{
}

//设置接口
VOID CWndUpdateMessage::SetUpdateViewSink(IUpdateViewSink * pIUpdateViewSink)
{
	m_pIUpdateViewSink=pIUpdateViewSink;
}

//完成消息
LRESULT CWndUpdateMessage::OnMessageDownLoadTaskFinish(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventDownLoadTaskFinish(wParam==TRUE);
	}

	return 0;
}

//更新消息
LRESULT CWndUpdateMessage::OnMessageDownLoadTaskUpdate(WPARAM wParam,LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventDownLoadTaskUpdate(wParam==TRUE);
	}

	return 0;
}

//出错事件
LRESULT CWndUpdateMessage::OnMessageUpdateCheckError(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckError();
	}

	return 0;
}

//完成事件
LRESULT CWndUpdateMessage::OnMessageUpdateCheckFinish(WPARAM wParam, LPARAM lParam)
{

	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckFinish();
	}

	return 0;
}

//地址事件
LRESULT CWndUpdateMessage::OnMessageUpdateCheckUpdateInfo(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckUpdateInfo((tagUpdateInfo*)wParam);
	}

	return 0;
}

//添加事件
LRESULT CWndUpdateMessage::OnMessageUpdateCheckUpdateItem(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckUpdateItem((tagUpdateItem*)wParam);
	}

	return 0;
}

LRESULT CWndUpdateMessage::OnMessageUpdateUICheck(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateUICheck((tagUpdateUICheck *)wParam);
	}

	return 0;
}

LRESULT CWndUpdateMessage::OnMessageUpdateUIDownload(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateUIDownload((tagUpdateUIDownload *)wParam);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

//构造函数
CUpdateStatusView::CUpdateStatusView()
{
	//状态变量
	m_uImageIndex=0;
	
	//任务属性
	m_dwTickCount=0;
	m_dwTotalTaskCount = 0;
	m_dwTaskFinished=0;
	m_cbCurrRetryCount=0;

	//描述变量
	m_szFileName[0]=0;
	m_szDescribe[0]=0;
	m_szUpdateFile[0]=0;
	m_szDownLoadAddress[0]=0;

	m_szDownDirectory[0]=0;

	//控件变量
	m_pUpdateCheckSink=NULL;
	m_pDownLoadTaskSink=NULL;	

	//接口变量
	m_pIUpdateStatusSink=NULL;
}

//析构函数
CUpdateStatusView::~CUpdateStatusView()
{	
	//结束更新
	EndUpdateStatus();
}

BOOL CUpdateStatusView::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CUpdateStatusView::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(szProduct);

	//居中窗口
	CenterWindow();
	//设置控件
	/*SetTimer(IDT_STATUS,TIMER_STATUS,NULL);*/


	return TRUE;
}

void CUpdateStatusView::InitControlUI()
{
	__super::InitControlUI();
	return;
}

void CUpdateStatusView::Notify(TNotifyUI & msg)
{
    
}

LPCTSTR CUpdateStatusView::GetSkinFile()
{
	return TEXT("DlgUpdateStatus.xml");
}

////设置位置
//void CUpdateStatusView::SetPos(RECT rc)
//{
//	__super::SetPos(rc);
//}
//
////事件处理
//void CUpdateStatusView::DoEvent(TEventUI& event)
//{
//	if( event.Type == UIEVENT_TIMER )
//	{
//		//更新状态
//		if (event.wParam==IDI_UPDATE_STATUS)
//		{
//			//设置索引
//			m_uImageIndex=(m_uImageIndex+1)%ANIMAL_FRAME_COUNT;
//
//			//更新界面
//			Invalidate();
//
//			return;
//		}
//
//		//延时结束
//		if(event.wParam==IDI_UPDATE_DELAYEND)
//		{
//			//关闭定时器
//			m_pManager->KillTimer(this,IDI_UPDATE_DELAYEND);
//
//			//结束更新
//			EndUpdateStatus();
//
//			//完成通知
//			if(m_pIUpdateStatusSink) 
//			{
//				m_pIUpdateStatusSink->OnEventDownLoadTaskFinish();
//			}	
//
//			return;
//		}
//	}
//
//	CContainerUI::DoEvent(event);
//}
//
////绘画背景
//void CUpdateStatusView::DoPaint(HDC hDC, const RECT& rcPaint)
//{
//	//获取设备
//	CDC * pDC = CDC::FromHandle(hDC);
//
//	//设置环境
//	pDC->SetBkMode(TRANSPARENT);
//	pDC->SelectObject(m_pManager->GetFont(0));
//
//	//获取位置
//	CRect rcClient(m_rcItem);	
//
//	//获取句柄
//	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);
//
//	//绘画背景
//	CPngImage ImageStatus;
//	ImageStatus.LoadImage(hInstance,TEXT("STATUS_BACK"));
//
//	//变量定义
//	CPngImage ImageStatusWait;	
//	ImageStatusWait.LoadImage(hInstance,TEXT("STATUS_WAITING"));
//
//	//获取大小
//	CSize SizeStatusBack(ImageStatus.GetWidth(),ImageStatus.GetHeight());
//	CSize SizeStatusWait(ImageStatusWait.GetWidth()/ANIMAL_FRAME_COUNT,ImageStatusWait.GetHeight());
//
//	//绘制背景
//	ImageStatus.DrawImage(pDC,(m_rcItem.left+rcClient.Width()-SizeStatusBack.cx)/2,m_rcItem.top+(rcClient.Height()-SizeStatusBack.cy)/2);
//	
//	//检查更新
//	if(m_pDownLoadTaskSink==NULL)
//	{
//		//绘制光圈
//		ImageStatusWait.DrawImage(pDC,m_rcItem.left+(rcClient.Width()-SizeStatusBack.cx)/2,m_rcItem.top+(rcClient.Height()-SizeStatusBack.cy)/2,SizeStatusWait.cx,SizeStatusWait.cy,m_uImageIndex*SizeStatusWait.cx,0,SizeStatusWait.cx,SizeStatusWait.cy); 
//
//		//更新文字
//		CPngImage ImageUpdateWords;
//		ImageUpdateWords.LoadImage(hInstance,TEXT("STATUS_UPDATE_WORDS"));		
//
//		//绘画文字
//		ImageUpdateWords.DrawImage(pDC,m_rcItem.left+rcClient.Width()/2-ImageUpdateWords.GetWidth()/2,m_rcItem.top+rcClient.Height()/2-ImageUpdateWords.GetHeight()/2);
//	}
//	else
//	{
//		//变量定义
//		INT nCurrProgress=0,nTotalProgress=0;
//
//		//变量定义
//		DWORD  dwTotalFileSize = m_pDownLoadTaskSink->m_dwTotalFileSize;
//		DWORD  dwDownLoadFileSize = m_pDownLoadTaskSink->m_dwDownLoadSize;
//
//		//计算进度		
//		if(dwTotalFileSize>0)  nCurrProgress = ((double)dwDownLoadFileSize/(double)dwTotalFileSize)*100;
//		if(m_dwTotalTaskCount>0)
//		{
//			nTotalProgress = ((double)m_dwTaskFinished/(double)m_dwTotalTaskCount)*100+nCurrProgress/m_dwTotalTaskCount;
//		}
//
//		//变量定义
//		CPngImage ImageNumber;
//		CPngImage ImagePercent;		
//
//		//加载资源
//		ImageNumber.LoadImage(hInstance,TEXT("STATUS_NUMBER"));
//		ImagePercent.LoadImage(hInstance,TEXT("STATUS_PERCENT"));		
//
//		//尺寸定义		
//		CSize SizeNumber(ImageNumber.GetWidth()/10,ImageNumber.GetHeight());
//		CSize SizePercent(ImagePercent.GetWidth(),ImagePercent.GetHeight());
//
//		//绘制等待
//		ImageStatusWait.DrawImage(pDC,m_rcItem.left+(rcClient.Width()-SizeStatusBack.cx)/2,m_rcItem.top+(rcClient.Height()-SizeStatusBack.cy)/2,SizeStatusWait.cx,SizeStatusWait.cy,m_uImageIndex*SizeStatusWait.cx,0,SizeStatusWait.cx,SizeStatusWait.cy); 
//
//		//计算宽度
//		int nNumberWidth = DrawNumber(pDC,m_rcItem.left+rcClient.Width()/2+12,m_rcItem.top+rcClient.Height()/2,&ImageNumber,TEXT("0123456789"),nTotalProgress,TEXT("%d"),DT_CALCRECT);	
//
//		//偏移位置
//		int nXOffset=(nNumberWidth-SizePercent.cx)/2;
//
//		//绘画百分号
//		ImagePercent.DrawImage(pDC,m_rcItem.left+rcClient.Width()/2+nXOffset,m_rcItem.top+(rcClient.Height()-SizePercent.cy)/2+4);
//
//		//绘画数字
//		DrawNumber(pDC,m_rcItem.left+rcClient.Width()/2+nXOffset,m_rcItem.top+rcClient.Height()/2,&ImageNumber,TEXT("0123456789"),nTotalProgress,TEXT("%d"),DT_RIGHT|DT_VCENTER);	
//	}
//}

//更新消息
VOID CUpdateStatusView::OnEventDownLoadTaskUpdate(bool bUpdate)
{
	//判断间隔
	if(GetTickCount()-m_dwTickCount>UPDATE_TIMESPACE || bUpdate)
	{
		//更新界面
		m_dwTickCount=GetTickCount();		
		Invalidate();
	}
}

//完成消息
VOID CUpdateStatusView::OnEventDownLoadTaskFinish(bool bSuccessed)
{
	//重置任务
	m_pDownLoadTaskSink->ResetDownLoadSink();

	//移除元素
	if(bSuccessed==true)
	{
		//设置变量
		++m_dwTaskFinished;
		m_cbCurrRetryCount=0;			

		//释放对象
		SafeDelete(m_DownLoadFileArray[0]);
		m_DownLoadFileArray.RemoveAt(0);
	}
	else
	{
		//超出重试次数
		if(++m_cbCurrRetryCount==MAX_RETRY_COUNT)
		{
			//失败提示
			TCHAR szMessage[128]=TEXT("");
			CDlgInformation Information;
			_sntprintf(szMessage,CountArray(szMessage),TEXT("文件 %s 无法下载,可能由网络原因造成,请稍后重试！"),m_DownLoadFileArray.GetAt(0)->szDownloadFile);
			Information.ShowMessageBox(TEXT("温馨提示"),szMessage);

			//结束更新
			EndUpdateStatus();

			return;
		}
	}	

	//文件下载
	if(m_DownLoadFileArray.GetCount()>0)
	{
		//获取时间
		m_dwTickCount=GetTickCount();

		//执行下载
		m_pDownLoadTaskSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile,!bSuccessed);

		//切换任务
		SwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);		
	}
	else
	{
		//延时结束
		//m_pManager->SetTimer(this,IDI_UPDATE_DELAYEND,TIMER_UPDATE_DELAYEND);
		m_pIUpdateStatusSink->OnEventDownLoadTaskFinish();
		EndUpdateStatus();
	}

	return;
}

//检查出错
VOID CUpdateStatusView::OnEventUpdateCheckError()
{
	//结束更新
	EndUpdateStatus();
}

//完成事件
VOID CUpdateStatusView::OnEventUpdateCheckFinish()
{
#if defined(_DEBUG) || defined(RELEASE_MANAGER) 

	//完成通知
	if(m_pIUpdateStatusSink)
	{
		m_pIUpdateStatusSink->OnEventUpdateCheckFinish(false);
	}

	//结束更新
	return EndUpdateStatus();
	
#else

	//检查完成
	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(_T("检查完成"));
	}

	//变量定义
	bool bNeedUpdate = m_DownLoadFileArray.GetCount()>0;

	//完成通知
	if(m_pIUpdateStatusSink)
	{
		m_pIUpdateStatusSink->OnEventUpdateCheckFinish(bNeedUpdate);
	}

	if(bNeedUpdate == true) 
	{
		//启动任务
		return StartDownLoadTask();		
	}
	else
	{
		//结束更新
		return EndUpdateStatus();
	}

#endif
}

//更新地址
VOID CUpdateStatusView::OnEventUpdateCheckUpdateInfo(tagUpdateInfo * UpdateInfo)
{
	//拷贝地址
	lstrcpyn(m_szDownLoadAddress,CA2CT(UpdateInfo->szDownLoadAddress),CountArray(m_szDownLoadAddress));
}

//添加事件
VOID CUpdateStatusView::OnEventUpdateCheckUpdateItem(tagUpdateItem * UpdateItem)
{
	try
	{
		//创建对象
		tagDownloadItem * pDownloadItem = new tagDownloadItem;	
		if(pDownloadItem==NULL) throw TEXT("内存资源不对,对象创建失败！");

		//复制数据
		lstrcpyn(pDownloadItem->szDownloadFile,CA2CT(UpdateItem->szFileName),CountArray(pDownloadItem->szDownloadFile));
		lstrcpyn(pDownloadItem->szFileCheckSum,CA2CT(UpdateItem->szFileCheckSum),CountArray(pDownloadItem->szFileCheckSum));

		//加入数组
		m_dwTotalTaskCount++;
		m_DownLoadFileArray.Add(pDownloadItem);
	}
	catch(...)
	{
		ASSERT(FALSE);
	}

	return;
}

VOID CUpdateStatusView::OnEventUpdateUICheck(tagUpdateUICheck * UIInfo)
{
	//构造信息
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("检查文件:%s [%d/%d]"),UIInfo->szCheckFile,UIInfo->wCurrNum,UIInfo->wTotalNum);

	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(szDownLoadUrl);
	}

	int nProgress = 0;
	nProgress = (UIInfo->wCurrNum * 100)/UIInfo->wTotalNum ;
	CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(_T("StatusProgress"));
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetValue(nProgress);
	}
}

VOID CUpdateStatusView::OnEventUpdateUIDownload(tagUpdateUIDownload * UIInfo)
{
	//构造信息
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("下载文件:%s [%d/%d]"),UIInfo->szCheckFile,UIInfo->dwDownLoadSize,UIInfo->dwTotalFileSize,UIInfo->dwDownSpeed);

	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(szDownLoadUrl);
	}

	int nProgress = 0;
	nProgress = (m_dwTaskFinished * 100)/m_dwTotalTaskCount ;
	CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(_T("StatusProgress"));
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetValue(nProgress);
	}
}

//启动检查
VOID CUpdateStatusView::StartUpdateCheck()
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

			return;
		};
	}

	//开始下载
	if(m_pUpdateCheckSink!=NULL) 
	{
		m_pUpdateCheckSink->m_pUpdateMessage=&m_UpdateMessage;
		m_pUpdateCheckSink->PerformDownLoad(m_szUpdateFile);
	}

	//设置变量
	m_uImageIndex=0;
	lstrcpyn(m_szDescribe,TEXT("更新检测"),CountArray(m_szDescribe));

	//设置时钟
	//m_pManager->SetTimer(this,IDI_UPDATE_STATUS,TIMER_UPDATE_STATUS);
}

//启动下载
VOID CUpdateStatusView::StartDownLoadTask()
{
	//指针判断
	if(m_pDownLoadTaskSink==NULL)
	{
		try
		{
			//创建指针
			m_pDownLoadTaskSink = new CDownLoadTaskSink();
			ASSERT(m_pDownLoadTaskSink!=NULL);
			if(m_pDownLoadTaskSink==NULL) return;
		}
		catch(...)
		{
			ASSERT(FALSE);
			return;
		}
	}

	//下载文件
	if(m_pDownLoadTaskSink!=NULL)
	{
		//设置指针
		m_pDownLoadTaskSink->m_pUpdateMessage=&m_UpdateMessage;

		//文件下载
		if(m_DownLoadFileArray.GetCount()>0)
		{
			//执行下载
			m_pDownLoadTaskSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile);

			//切换任务
			SwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);
		}
	}

	return;
}

//结束更新
VOID CUpdateStatusView::EndUpdateStatus()
{
	//设置变量
	m_dwTickCount=0;	
	m_dwTaskFinished=0;
	m_cbCurrRetryCount=0;
	m_dwTotalTaskCount=0;
	m_pIUpdateStatusSink=NULL;

	//关闭定时器
	//m_pManager->KillTimer(this,IDI_UPDATE_STATUS);

	//销毁窗口
	m_UpdateMessage.DestroyWindow();
	m_UpdateMessage.SetUpdateViewSink(NULL);

	//移除任务
	for(INT_PTR nIndex=0;nIndex<m_DownLoadFileArray.GetCount();nIndex++)
	{
		SafeDelete(m_DownLoadFileArray[nIndex]);		
	}

	//移除元素
	m_DownLoadFileArray.RemoveAll();

	//释放对象
	if(m_pUpdateCheckSink!=NULL)
	{
		m_pUpdateCheckSink->CloseDownLoad();
		SafeDelete(m_pUpdateCheckSink);
	}

	//释放对象
	if(m_pDownLoadTaskSink!=NULL) 
	{
		m_pDownLoadTaskSink->CloseDownLoad();
		SafeDelete(m_pDownLoadTaskSink);
	}

	////移除自己
	//CContainerUI * pParent = static_cast<CContainerUI *>(GetParent());	
	//if(pParent != NULL)
	//{
	//	pParent->Remove(this,false);

	//	//更新父类
	//	pParent->Invalidate();
	//}
	DestroyWindow();
}

//初始更新
VOID CUpdateStatusView::InitUpdateStatus(IUpdateStatusSink * pIUpdateStatusSink, LPCTSTR pszProgressName)
{

	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(_T("开始检查"));
	}

	//设置变量
	m_pIUpdateStatusSink=pIUpdateStatusSink;

	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	_sntprintf(m_szUpdateFile,CountArray(m_szUpdateFile),TEXT("%s/DownLoad/Client/%s.txt"),pGlobalWebLink->GetPlatformLink(),pszProgressName);

	//创建窗口	
	if(m_UpdateMessage.m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		m_UpdateMessage.Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);

		//设置接口
		m_UpdateMessage.SetUpdateViewSink(this);
	}

	//启动检查
	StartUpdateCheck();
}

VOID CUpdateStatusView::SetDownDirectory(LPCTSTR pszProgressName)
{
	//拷贝地址
	lstrcpyn(m_szDownDirectory,pszProgressName,CountArray(m_szDownDirectory));
}

//切换任务
VOID CUpdateStatusView::SwitchDownLoadTask(LPCTSTR pszFileName)
{
	//参数校验
	if(pszFileName == NULL) return;

	//查找字符
	DWORD dwIndex = lstrlen(pszFileName);
	while(--dwIndex>0 && pszFileName[dwIndex]!=TEXT('/'));
	
	//拷贝字符
	lstrcpyn(m_szFileName,pszFileName+(dwIndex>0?++dwIndex:0),CountArray(m_szFileName));
}

VOID CUpdateStatusView::OnTimer(UINT nIDEvent)
{

}



/////////////////////////////////////////////////////////////////////////////////////////////////
