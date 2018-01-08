#include "StdAfx.h"
#include "DownLoad.h"

/////////////////////////////////////////////////////////////////////////////////

//链接文件
#pragma comment (lib,"Wininet.lib")

//////////////////////////////////////////////////////////////////////////////////

//宏定义
#define ID_SOCKET_WND				100									//窗口标识

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDownLoad::CDownLoad()
{
	//文件信息
	m_dwTotalSize=0L;
	m_dwDownLoadSize=0L;
	m_dwDownLoadPos=0;

	//状态变量
	m_DownLoadStatus=DownLoadStatus_Conclude;
	ZeroMemory(&m_DownLoadInfo,sizeof(m_DownLoadInfo));

	//接口变量
	m_pIDownLoadSink=NULL;

	//设置接口
	m_WEBSocket.SetWEBSocketSink(QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//析构函数
CDownLoad::~CDownLoad()
{
	//关闭下载
	CloseDownLoad();

	return;
}

//接口查询
VOID * CDownLoad::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWEBSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWEBSocketSink,Guid,dwQueryVer);
	return NULL;
}

//关闭下载
bool CDownLoad::CloseDownLoad()
{
	//设置变量
	m_dwTotalSize=0L;
	m_dwDownLoadSize=0L;
	m_DownLoadStatus=DownLoadStatus_Conclude;

	//状态处理
	m_HeadTranslate.CleanData();

	//关闭连接
	m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

	return true;
}

//执行下载
bool CDownLoad::PerformDownLoad(LPCTSTR pszUrl,DWORD dwDownLoadPos,IUnknownEx * pIUnknownEx)
{
	//状态效验
	ASSERT(m_DownLoadStatus==DownLoadStatus_Conclude);
	if (m_DownLoadStatus!=DownLoadStatus_Conclude) return false;

	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink)!=NULL);
	m_pIDownLoadSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink);

	//设置变量
	m_dwDownLoadPos=dwDownLoadPos;

	////添加参数
	//TCHAR szWholeUrl[256]=TEXT("");
	//_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s?x=%d"),pszUrl,GetTickCount());

	//发起连接
	if (ConnectServer(pszUrl)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//连接消息
bool CDownLoad::OnSocketLink(INT nErrorCode)
{
	//错误处理
	if (nErrorCode!=0)
	{
		//关闭下载
		CloseDownLoad();

		//状态通知
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Connect);

		return false;
	}

	//发送请求
	SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);

	return true;
}

//关闭消息
bool CDownLoad::OnSocketShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//关闭下载
		CloseDownLoad();

		//状态通知
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);
	}

	return true;
}

//数据包流
VOID CDownLoad::OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)
{
	//效验状态
	ASSERT(m_DownLoadStatus==DownLoadStatus_Service);
	if (m_DownLoadStatus!=DownLoadStatus_Service) return;

	//设置变量
	m_dwDownLoadSize+=wStreamSize;

	//数据处理
	ASSERT(m_pIDownLoadSink!=NULL);
	if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDataStream(pcbMailData,wStreamSize);

	//完成判断
	if (m_dwDownLoadSize==m_dwTotalSize)
	{
		//关闭下载
		CloseDownLoad();

		//状态通知
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Conclude);
	}

	return;
}

//数据包头
VOID CDownLoad::OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)
{
	switch (nStatusCode)
	{
	case HTTP_STATUS_OK:				//获取成功
	case HTTP_STATUS_PARTIAL_CONTENT:	//断点续传
		{
			//设置变量
			m_DownLoadStatus=DownLoadStatus_Service;
			m_HeadTranslate.SetHeadData((BYTE *)pcbHeadData,wHeadSize);

			//文件信息
			m_dwDownLoadSize=0L;
			m_dwTotalSize=m_HeadTranslate.GetTotalFileSize();

			//状态通知
			ASSERT(m_pIDownLoadSink!=NULL);
			if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Service);

			//事件通知
			LPCTSTR pszEntityTag=m_HeadTranslate.GetFileEntityTag();
			m_pIDownLoadSink->OnDataInformation(nStatusCode==HTTP_STATUS_PARTIAL_CONTENT,m_dwTotalSize,pszEntityTag,m_DownLoadInfo.szUrlPath);

			return;
		}
	case HTTP_STATUS_MOVED:				//目标跳转
	case HTTP_STATUS_REDIRECT:			//目标跳转
	case HTTP_STATUS_REDIRECT_METHOD:	//目标跳转
		{
			//提取位置
			LPCSTR pszLocation=strstr((CHAR *)pcbHeadData,"Location: ");
			if (pszLocation==NULL) m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

			//变量定义
			CHAR szLocation[MAX_PATH]="";
			WORD wStartPos=(WORD)strlen("Location: ");
			
			//构造地址
			WORD wOff=0;
			while (pszLocation[wStartPos+wOff]!='\r')
			{
				//效验数据
				ASSERT(wOff<CountArray(szLocation));
				if (wOff>=CountArray(szLocation)) m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//设置字符
				WORD wIndex=wOff++;
				szLocation[wIndex]=pszLocation[wStartPos+wIndex];
			}
			szLocation[wOff]=0;

			//转向分析
			if (szLocation[0]=='/')
			{
				//设置请求
				CA2CT strLocation(szLocation);
				lstrcpyn(m_DownLoadInfo.szUrlPath,(LPCTSTR)strLocation,CountArray(m_DownLoadInfo.szUrlPath));

				//添加参数
				//_sntprintf(m_DownLoadInfo.szUrlPath,CountArray(m_DownLoadInfo.szUrlPath),TEXT("%s?x=%I64d"),m_DownLoadInfo.szUrlPath,GetTickCount());

				//发送请求
				SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);
			}
			else
			{
				//关闭连接
				m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//添加参数
				/*TCHAR szWholeUrl[256]=TEXT("");
				_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s"),((CA2CT)szLocation));
				_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s?x=%d"),szWholeUrl,GetTickCount());*/

				//请求连接
				ConnectServer((CA2CT)szLocation);
			}

			return;
		}
	default:							//默认处理
		{
			//关闭下载
			CloseDownLoad();

			//状态通知
			ASSERT(m_pIDownLoadSink!=NULL);
			if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);

			return;
		}
	}

	return;
}

//请求连接
bool CDownLoad::ConnectServer(LPCTSTR pszUrl)
{
	//变量定义
	BYTE cbResult=CONNECT_EXCEPTION;
	INTERNET_SCHEME nScheme=GetDownLoadInfo(pszUrl,m_DownLoadInfo);

	//类型分析
	switch (nScheme)
	{
	case INTERNET_SERVICE_FTP:		//FTP 下载
		{
			cbResult=m_WEBSocket.ConnectServer(m_DownLoadInfo.szHostName,m_DownLoadInfo.wHostPort);
			break;
		}
	case INTERNET_SERVICE_HTTP:		//HTTP 下载
		{
			cbResult=m_WEBSocket.ConnectServer(m_DownLoadInfo.szHostName,m_DownLoadInfo.wHostPort);
			break;
		}
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	//结果处理
	if (cbResult==CONNECT_SUCCESS)
	{
		//设置状态
		m_DownLoadStatus=DownLoadStatus_Connect;

		//状态通知
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Connect);
	}
	else
	{
		//停止下载
		CloseDownLoad();

		//事件通知
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);

		return false;
	}

	return true;
}

//发送请求
bool CDownLoad::SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath)
{
	//变量定义
	CHAR szRequestData[1024]="";
	CT2CA strUrlPath(pszUrlPath);
	CT2CA strHostName(pszHostName);

	//构造请求
	if(m_dwDownLoadPos==0)
	{
		_snprintf(szRequestData,CountArray(szRequestData),"GET %s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\n"
			"User-Agent:Mozilla/4.0 (compatible; MSIE 6.00; Windows NT 5.1)\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nConnection:Keep-Alive\r\n\r\n",(LPCSTR)strUrlPath,(LPCSTR)strHostName);
	}
	else
	{
		_snprintf(szRequestData,CountArray(szRequestData),"GET %s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\n"
			"User-Agent:Mozilla/4.0 (compatible; MSIE 6.00; Windows NT 5.1)\r\nRange: bytes=%d-\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nConnection:Keep-Alive\r\n\r\n",(LPCSTR)strUrlPath,(LPCSTR)strHostName,m_dwDownLoadPos);
	}

	//发送请求
	m_WEBSocket.SendRequestData(szRequestData,(WORD)strlen(szRequestData));

	return true;
}

//获取信息
INTERNET_SCHEME CDownLoad::GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo)
{
	//变量定义
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	ZeroMemory(&DownLoadInfo,sizeof(DownLoadInfo));

	//规范地址
	DWORD dwMaxLength=CountArray(szDownLoadUrl);
	DWORD dwFlags=ICU_BROWSER_MODE|ICU_NO_ENCODE|ICU_NO_META|ICU_ENCODE_SPACES_ONLY;
	if (InternetCanonicalizeUrl(pszUrl,szDownLoadUrl,&dwMaxLength,dwFlags)==FALSE) return INTERNET_SCHEME_UNKNOWN;

	//格式地址
	URL_COMPONENTS Components;
	ZeroMemory(&Components,sizeof(Components));

	//设置变量
	Components.dwStructSize=sizeof(Components);
	Components.lpszUrlPath=DownLoadInfo.szUrlPath;
	Components.lpszHostName=DownLoadInfo.szHostName;
	Components.dwUrlPathLength=CountArray(DownLoadInfo.szUrlPath);
	Components.dwHostNameLength=CountArray(DownLoadInfo.szHostName);
	if (InternetCrackUrl(szDownLoadUrl,0,0,&Components)==FALSE) return INTERNET_SCHEME_UNKNOWN;

	//设置变量
	DownLoadInfo.wHostPort=Components.nPort;

	return Components.nScheme;
}

//////////////////////////////////////////////////////////////////////////////////
