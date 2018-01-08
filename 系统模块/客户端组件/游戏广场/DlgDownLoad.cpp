#include "StdAfx.h"
#include "DlgDownLoad.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_ITEM				100									//进程检测
#define IDI_UPDATE_STATUS			101									//更新状态

//时间标识
#define IDI_CHECK_PROCESS			200									//进程检测

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaHallGameDownLoadSink, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaHallGameDownLoadSink::CPlazaHallGameDownLoadSink()
{
	//下载速度
	m_dwDownSpeed=0L;
	m_lLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//文件信息
	m_lDownLoadSize=0L;
	m_lTotalFileSize=0L;

	//状态变量
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//任务属性
	m_wKindID=0;
	m_wServerID=0;
	ZeroMemory(m_szClientName,sizeof(m_szClientName));

	//内核信息
	m_pDlgDownLoad=NULL;
	ZeroMemory(&m_ProcessInfoMation,sizeof(m_ProcessInfoMation));

	return;
}

//析构函数
CPlazaHallGameDownLoadSink::~CPlazaHallGameDownLoadSink()
{
}

//接口查询
VOID * CPlazaHallGameDownLoadSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//关闭下载
bool CPlazaHallGameDownLoadSink::CloseDownLoad()
{
	//关闭窗口
	DestroyWindow();

	//关闭下载
	m_DownLoad.CloseDownLoad();

	//设置状态
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//关闭文件
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();

	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//执行下载
bool CPlazaHallGameDownLoadSink::PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//效验状态
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//下载速度
	m_dwDownSpeed=0L;
	m_lLastCalcSize=0L;
	m_dwLastCalcTime=GetTickCount();

	//设置变量
	m_wKindID=wKindID;
	m_wServerID=wServerID;
	m_cbDownLoadStatus=DOWN_LOAD_LINK;
	lstrcpyn(m_szClientName,pszClientName,CountArray(m_szClientName));

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s/DownLoad.aspx?KindID=%ld"),pGlobalWebLink->GetPlatformLink(),wKindID);

	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	//执行下载
	m_DownLoad.PerformDownLoad(szDownLoadUrl,0,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//下载异常
bool CPlazaHallGameDownLoadSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//下载状态
bool CPlazaHallGameDownLoadSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//设置状态
	switch (DownLoadStatus)
	{
	case DownLoadStatus_Service:		//服务状态
		{
			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_READ;

			//发送状态
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

			break;
		}
	case DownLoadStatus_Conclude:		//完成状态
		{
			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_SETUP;

			//安装文件
			if (PerformInstall()==false)
			{
				//关闭下载
				CloseDownLoad();

				return false;
			}

			//发送状态
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

			break;
		}
	}

	return true;
}

//下载数据
bool CPlazaHallGameDownLoadSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//写入文件
	m_lDownLoadSize+=wStreamSize;
	m_DownFile.Write(pcbMailData,wStreamSize);

	//计算速度
	if (GetTickCount()>=(m_dwLastCalcTime+2000L))
	{
		//计算速度
		m_dwDownSpeed=DWORD((m_lDownLoadSize-m_lLastCalcSize)/(GetTickCount()-m_dwLastCalcTime));

		//设置变量
		m_dwLastCalcTime=GetTickCount();
		m_lLastCalcSize=m_lDownLoadSize;
	}
	
	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//下载信息
bool CPlazaHallGameDownLoadSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//设置变量
	m_lTotalFileSize+=dwTotalFileSize;

	//创建文件
	if (CreateDownLoadFile(pszLocation)==false)
	{
		//关闭下载
		CloseDownLoad();

		return false;
	}

	return true;
}

//执行安装
bool CPlazaHallGameDownLoadSink::PerformInstall()
{
	//下载文件
	TCHAR szTargetName[MAX_PATH]=TEXT("");
	CString strSourceName=m_DownFile.GetFilePath();

	//变量定义
	INT nTailCount=lstrlen(TEXT(".WHD"));
	INT nSourceLength=lstrlen(strSourceName);

	//构造名字
	szTargetName[nSourceLength-nTailCount]=0;
	CopyMemory(szTargetName,(LPCTSTR)strSourceName,sizeof(TCHAR)*(nSourceLength-nTailCount));

	//关闭文件
	m_DownFile.Close();

	//删除文件
	DeleteFile(szTargetName);

	//移动文件
	if (MoveFileWithProgress(strSourceName,szTargetName,NULL,NULL,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}
	//构造命令
	TCHAR szCommandLine[MAX_PATH];
	TCHAR szCommandName[]=TEXT("VERYSILENT");
	if(m_wKindID != 0)
	{
		//_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s /VERYSILENT"),szTargetName);
		_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s /%s"),szTargetName,szCommandName);
	}
	else
	{
		_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s"),szTargetName);
	}


	//变量定义
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//启动进程
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWDEFAULT;
	if (CreateProcess(NULL,szCommandLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_ProcessInfoMation)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建窗口
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
	}

	//设置时间
	SetTimer(IDI_CHECK_PROCESS,100,NULL);

	return true;
}

//创建文件
bool CPlazaHallGameDownLoadSink::CreateDownLoadFile(LPCTSTR pszLocation)
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
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\%s.WHD"),szDownLoadPath,pszFileName);

	//创建目录
	CreateDirectory(szDownLoadPath,NULL);

	//创建文件
	if (m_DownFile.Open(szFilePath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//时间消息
VOID CPlazaHallGameDownLoadSink::OnTimer(UINT nIDEvent)
{
	//进程检测
	if (nIDEvent==IDI_CHECK_PROCESS)
	{
		//状态效验
		if (m_ProcessInfoMation.hProcess==NULL)
		{
			//错误断言
			ASSERT(FALSE);

			//关闭下载
			CloseDownLoad();

			return;
		}

		//进程检测
		if (WaitForSingleObject(m_ProcessInfoMation.hProcess,0L)==WAIT_OBJECT_0)
		{
			//关闭窗口
			DestroyWindow();

			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//发送状态
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
