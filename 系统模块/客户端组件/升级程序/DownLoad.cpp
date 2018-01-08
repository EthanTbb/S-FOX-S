#include "StdAfx.h"
#include "DownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//链接文件
#pragma comment (lib,"Wininet.lib")

//////////////////////////////////////////////////////////////////////////////////

//宏定义
#define ID_SOCKET_WND				100									//窗口标识
#define WM_SOCKET_NOTIFY_EX			WM_USER+100							//消息标识

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUWEBSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY_EX,OnSocketNotify)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////


//构造函数
CUHeadTranslate::CUHeadTranslate()
{
	//设置变量
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//数据信息
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//析构函数
CUHeadTranslate::~CUHeadTranslate()
{
}

//清理参数
VOID CUHeadTranslate::CleanData()
{
	//设置变量
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//数据信息
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//设置参数
VOID CUHeadTranslate::SetHeadData(BYTE * pcbHeadData, WORD wDataLength)
{
	//设置变量
	m_wDataLength=wDataLength;
	m_pcbHeadData=pcbHeadData;

	//变量定义
	WORD wStartIndex=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//解释字符
	LPCSTR pszEntityTag="ETag: \"";
	LPCSTR pszFileLength="Content-Length: ";

	//解释数据
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//换行判断
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//数据属性
			WORD wLineLength=i-wStartIndex,wFlagLength=0;
			LPCSTR pszLineData=pszHeadString+wStartIndex;

			//设置变量
			wStartIndex=(i+2);

			//文件标识
			wFlagLength=(WORD)strlen(pszEntityTag);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszEntityTag,wFlagLength*sizeof(CHAR))==0))
			{
				//变量定义
				CHAR szFileEntityTag[CountArray(m_szFileEntityTag)]="";
				WORD wCopyCount=__min(CountArray(m_szFileEntityTag),wLineLength-wFlagLength-1);

				//拷贝数据
				ASSERT((wLineLength-wFlagLength-1)<=CountArray(szFileEntityTag));
				CopyMemory(szFileEntityTag,pszLineData+wFlagLength,wCopyCount*sizeof(CHAR));

				//设置数据
				szFileEntityTag[wCopyCount]=0;
				lstrcpyn(m_szFileEntityTag,(LPCTSTR)(CA2CT(szFileEntityTag)),CountArray(m_szFileEntityTag));

				continue;
			}

			//文件长度
			wFlagLength=(WORD)strlen(pszFileLength);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszFileLength,wFlagLength*sizeof(CHAR))==0))
			{
				//设置数据
				m_dwTotalFileSize=atol(pszLineData+wFlagLength);

				continue;
			}
		}
	}

	return;
}

//获取数据
WORD CUHeadTranslate::GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount)
{
	//效验状态
	ASSERT((m_pcbHeadData!=NULL)&&(m_wDataLength>0));
	if ((m_pcbHeadData==NULL)||(m_wDataLength==0)) return 0;

	//变量定义
	WORD wStartIndex=0;
	WORD wLineSearch=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//提取数据
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//换行判断
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//结束判断
			if (wLineSearch==wLineIndex)
			{
				//设置长度
				ASSERT(wCharCount>=(i-wStartIndex));
				WORD wCopyCount=__min(wCharCount,i-wStartIndex);

				//设置结果
				szDataBuffer[wCopyCount]=0;
				CopyMemory(szDataBuffer,pszHeadString+wStartIndex,wCopyCount*sizeof(CHAR));

				return wCopyCount;
			}

			//设置变量
			wLineSearch++;
			wStartIndex=(i+2);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUWEBSocket::CUWEBSocket()
{
	//辅助变量
	m_wRequestID=1;
	m_bHeadValid=false;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;

	//数据包头
	m_wHeadSize=0;
	ZeroMemory(m_szHeadData,sizeof(m_szHeadData));

	//缓冲变量
	m_dwBufferData=0L;
	m_dwBufferSize=0L;
	m_bNeedBuffer=false;
	m_pcbDataBuffer=NULL;

	//内核变量
	m_pIWEBSocketSink=NULL;
	m_hSocket=INVALID_SOCKET;

	//代理信息
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return;
}

//析构函数
CUWEBSocket::~CUWEBSocket()
{
	//关闭连接
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//删除缓冲
	SafeDeleteArray(m_pcbDataBuffer);

	return;
}

//设置接口
bool CUWEBSocket::SetWEBSocketSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IWEBSocketSink)!=NULL);
		m_pIWEBSocketSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IWEBSocketSink);

		//成功判断
		if (m_pIWEBSocketSink==NULL) return false;
	}
	else m_pIWEBSocketSink=NULL;

	return true;
}


//设置代理
bool CUWEBSocket::SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	switch (cbProxyType)
	{
	case PROXY_NONE:	//没有代理
		{
			//设置变量
			m_cbProxyType=cbProxyType;
			ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

			return true;
		}
	case PROXY_HTTP:	//HTTP 代理
	case PROXY_SOCKS4:	//SOKKS4 代理
	case PROXY_SOCKS5:	//SOKKS5 代理
		{
			//设置变量
			m_cbProxyType=cbProxyType;
			m_ProxyServer=ProxyServer;

			return true;
		}
	default: 
		{ 
			ASSERT(FALSE); 
			return false;
		}
	}

	return false;
}

//关闭连接
VOID CUWEBSocket::CloseSocket(BYTE cbShutReason)
{
	//关闭窗口
	DestroyWindow();

	//缓冲变量
	m_dwBufferData=0L;
	m_dwBufferSize=0L;
	m_bNeedBuffer=false;
	SafeDeleteArray(m_pcbDataBuffer);

	//状态变量
	m_bHeadValid=false;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;
	m_wRequestID=__max(1,m_wRequestID+1);

	//关闭判断
	if (m_hSocket!=INVALID_SOCKET)
	{
		//关闭连接
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;

		//事件通知
		if (cbShutReason!=SHUT_REASON_INSIDE)
		{
			ASSERT(m_pIWEBSocketSink!=NULL);
			if (m_pIWEBSocketSink!=NULL) m_pIWEBSocketSink->OnSocketShut(cbShutReason);
		}
	}

	return;
}

//连接操作
BYTE CUWEBSocket::ConnectServer(LPCTSTR pszServer, WORD wPort)
{
	//效验参数
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

	//设置参数
	m_wHeadSize=0;
	ZeroMemory(m_szHeadData,sizeof(m_szHeadData));

	//转化地址
	CT2CA HttpServer(pszServer);
	DWORD dwServerIP=inet_addr(HttpServer);

	//域名解释
	if (dwServerIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(HttpServer);
		if (lpHost!=NULL) dwServerIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	try
	{
		//建立 SOCKET
		m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket==INVALID_SOCKET) 
		{
			throw CONNECT_EXCEPTION;
		}

		//创建窗口
		if (m_hWnd==NULL)
		{
			CRect rcSocketWnd;
			Create(NULL,NULL,WS_CHILD,rcSocketWnd,GetDesktopWindow(),100);
		}

		//变量定义
		SOCKADDR_IN SocketAddr;
		ZeroMemory(&SocketAddr,sizeof(SocketAddr));

		//设置变量
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr=dwServerIP;

		//代理连接
		if (m_cbProxyType!=PROXY_NONE)
		{
			//连接代理
			DWORD dwResult=ConnectProxyServer();
			if (dwResult!=CONNECT_SUCCESS) throw dwResult;

			//请求连接
			switch (m_cbProxyType)
			{
			case PROXY_HTTP:	//HTTP 代理
				{
					//构造请求
					char cbDataBuffer[512];
					BYTE * pcbServerIP=(BYTE *)&dwServerIP;
					_snprintf(cbDataBuffer,CountArray(cbDataBuffer),"CONNECT %d.%d.%d.%d:%d HTTP/1.0\r\nUser-Agent: WHPlatform/0.1\r\n\r\n",
						pcbServerIP[0],pcbServerIP[1],pcbServerIP[2],pcbServerIP[3],wPort);

					//连接请求
					INT nRequestLen=(INT)strlen(cbDataBuffer);
					if (send(m_hSocket,cbDataBuffer,nRequestLen,0)!=nRequestLen) throw CONNECT_PROXY_FAILURE;

					//接收响应
					INT nRecvCount=recv(m_hSocket,cbDataBuffer,sizeof(cbDataBuffer),0);
					if (nRecvCount>0) cbDataBuffer[nRecvCount]=0;

					//结果判断
					bool bSuccess=false;
					for (INT i=0;i<nRecvCount;i++)
					{
						if ((cbDataBuffer[i]==' ')&&(cbDataBuffer[i+1]=='2')&&(cbDataBuffer[i+2]=='0')&&(cbDataBuffer[i+3]=='0'))
						{
							bSuccess=true;
							break;
						}
					}

					//结果判断
					if (bSuccess==false) throw CONNECT_PROXY_FAILURE;

					break;
				}
			case PROXY_SOCKS5:	//Socks5 代理
				{
					//构造请求
					INT nOff=4;
					char cbDataBuffer[256]={5,1,0,1};

					//连接地址
					CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
					nOff+=sizeof(SocketAddr.sin_addr);
					
					//连接端口
					*(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
					nOff+=sizeof(WORD);

					//连接请求
					if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) throw CONNECT_PROXY_FAILURE;

					//接收响应
					INT nRecvCount=recv(m_hSocket,cbDataBuffer,4,0);
					if (nRecvCount!=4) throw CONNECT_PROXY_FAILURE;
					if (cbDataBuffer[0]!=5) throw CONNECT_PROXY_FAILURE;

					//失败判断
					if (cbDataBuffer[1]==0) throw CONNECT_PROXY_FAILURE;

					//接收数据
					INT nLeftDataLen=(cbDataBuffer[3]==1)?6:cbDataBuffer[4]+2;
					if (recv(m_hSocket,cbDataBuffer,nLeftDataLen,0)!=nLeftDataLen) throw CONNECT_PROXY_FAILURE;

					break;
				}
			case PROXY_SOCKS4:	//Socks4 代理
				{
					//构造请求
					INT nOff=2;
					char cbDataBuffer[256]={4,1};

					//连接端口
					*(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
					nOff+=sizeof(WORD);

					//连接地址
					CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
					nOff+=sizeof(SocketAddr.sin_addr);

					//保留字段
					cbDataBuffer[nOff++]=0;

					//连接请求
					INT nResult=send(m_hSocket,cbDataBuffer,nOff,0);
					if (nResult!=nOff) throw CONNECT_PROXY_FAILURE;

					//接收响应
					nResult=recv(m_hSocket,cbDataBuffer,8,0);
					if (nResult!=8) throw CONNECT_PROXY_FAILURE;
					if (cbDataBuffer[0]!=0) throw CONNECT_PROXY_FAILURE;

					//失败判断
					if (cbDataBuffer[1]!=90) throw CONNECT_PROXY_FAILURE;

					break;
				}
			default:		//默认处理
				{
					ASSERT(FALSE);
					throw CONNECT_PROXY_FAILURE;
				}
			}

			//连接成功
			WSASetLastError(0);

			//设置变量
			m_cbSocketStatus=SOCKET_STATUS_CONNECT;

			//发送消息
			PostMessage(WM_SOCKET_NOTIFY_EX,0,MAKELONG(FD_CONNECT,0));

			//绑定窗口
			WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY_EX,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);
		}
		else
		{
			//绑定窗口
			WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY_EX,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);

			//发起连接
			INT nErrorCode=connect(m_hSocket,(SOCKADDR *)&SocketAddr,sizeof(SocketAddr));
			if ((nErrorCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSAEWOULDBLOCK)) throw CONNECT_EXCEPTION;

			//设置变量
			m_cbSocketStatus=SOCKET_STATUS_WAIT;
		}
	}
	catch (...)
	{
		//关闭连接
		CloseSocket(SHUT_REASON_INSIDE);

		return CONNECT_EXCEPTION;
	}

	return CONNECT_SUCCESS;
}

//发送请求
WORD CUWEBSocket::SendRequestData(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT((wDataSize!=0)&&(pData!=NULL));
	if ((wDataSize==0)||(pData==NULL)) return 0L;

	//效验状态
	if (m_hSocket==INVALID_SOCKET) return 0L;
	if (m_cbSocketStatus!=SOCKET_STATUS_CONNECT) return 0L;

	//设置变量
	m_wHeadSize=0;
	m_bHeadValid=false;
	m_wRequestID=__max(1,m_wRequestID+1);

	//变量定义
	WORD wTotalCount=0;

	//发送数据
	while ((m_bNeedBuffer==false)&&(wTotalCount<wDataSize))
	{
		//发生数据
		INT nSendCount=send(m_hSocket,(char *)pData+wTotalCount,wDataSize-wTotalCount,0);

		//错误判断
		if (nSendCount==SOCKET_ERROR)
		{
			//缓冲判断
			if (WSAGetLastError()==WSAEWOULDBLOCK)
			{
				AmortizeBuffer((LPBYTE)pData+wTotalCount,wDataSize-wTotalCount);
				return wDataSize;
			}

			//关闭连接
			CloseSocket(SHUT_REASON_EXCEPTION);

			return 0L;
		}
		else
		{
			//设置变量
			wTotalCount+=nSendCount;
		}
	}

	//缓冲数据
	if (wTotalCount>wDataSize)
	{
		AmortizeBuffer((LPBYTE)pData+wTotalCount,wDataSize-wTotalCount);
	}

	return wDataSize;
}

//连接代理
BYTE CUWEBSocket::ConnectProxyServer()
{
	//效验状态
	ASSERT(m_cbProxyType!=PROXY_NONE);
	if (m_cbProxyType==PROXY_NONE) return CONNECT_EXCEPTION;

	//转化地址
	CT2CA ProxyServer(m_ProxyServer.szProxyServer);
	DWORD dwProxyServer=inet_addr(ProxyServer);

	//域名解释
	if (dwProxyServer==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(ProxyServer);
		if (lpHost!=NULL) dwProxyServer=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	//变量定义
	SOCKADDR_IN SocketAddrProxy;
	ZeroMemory(&SocketAddrProxy,sizeof(SocketAddrProxy));

	//设置变量
	SocketAddrProxy.sin_family=AF_INET;
	SocketAddrProxy.sin_addr.S_un.S_addr=dwProxyServer;
	SocketAddrProxy.sin_port=htons(m_ProxyServer.wProxyPort);

	//连接代理
	INT nErrorCode=connect(m_hSocket,(sockaddr *)&SocketAddrProxy,sizeof(SocketAddrProxy));
	if (nErrorCode==SOCKET_ERROR) return CONNECT_PROXY_FAILURE;

	//发送数据
	if (m_cbProxyType==PROXY_SOCKS5)
	{
		//变量定义
		INT nOff=2;
		char cbDataBuffer[256]={5,1};

		//发送数据
		cbDataBuffer[nOff++]=(m_ProxyServer.szUserName[0]==0)?0:2;
		if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) return CONNECT_PROXY_FAILURE;

		//接收数据
		if (recv(m_hSocket,cbDataBuffer,2,0)!=2) return CONNECT_PROXY_FAILURE;

		//数据分析
		if (cbDataBuffer[0]!=5) return CONNECT_PROXY_FAILURE;
		if ((cbDataBuffer[1]!=0)&&(cbDataBuffer[1]!=2)) return CONNECT_PROXY_FAILURE;

		//身份验证
		if (cbDataBuffer[1]==2)
		{
			//变量定义
			INT nStringLen=0;
			CT2CA strUserName(m_ProxyServer.szUserName);
			CT2CA strPassword(m_ProxyServer.szPassword);

			//登录帐号
			nOff=0;
			cbDataBuffer[nOff++]=1;
			nStringLen=lstrlenA(strUserName);
			cbDataBuffer[nOff++]=(char)nStringLen;
			CopyMemory(cbDataBuffer+nOff,(LPCSTR)strUserName,nStringLen);
			nOff+=nStringLen;

			//登录密码
			nStringLen=lstrlenA(strPassword);
			cbDataBuffer[nOff++]=(char)nStringLen;
			CopyMemory(cbDataBuffer+nOff,(LPCSTR)strPassword,nStringLen);
			nOff+=nStringLen;

			//发送数据
			if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) return CONNECT_PROXY_FAILURE;

			//接受数据
			if (recv(m_hSocket,cbDataBuffer,2,0)!=2) return CONNECT_PROXY_FAILURE;

			//数据分析
			if ((cbDataBuffer[0]!=1)||(cbDataBuffer[1]!=0)) return CONNECT_PROXY_USER_INVALID;
		}
	}

	return CONNECT_SUCCESS;
}

//缓冲数据
VOID CUWEBSocket::AmortizeBuffer(VOID * pData, WORD wDataSize)
{
	//申请缓冲
	if ((m_dwBufferData+wDataSize)>m_dwBufferSize)
	{
		//变量定义
		LPBYTE pcbDataBuffer=NULL;
		LPBYTE pcbDeleteBuffer=m_pcbDataBuffer;

		//计算大小
		DWORD dwNeedSize=m_dwBufferData+wDataSize;
		DWORD dwApplySize=__max(dwNeedSize,m_dwBufferSize*2L);

		//申请缓冲
		try
		{
			pcbDataBuffer=new BYTE [dwApplySize];
		}
		catch (...)	{ }

		//失败判断
		if (pcbDataBuffer==NULL)
		{
			CloseSocket(SHUT_REASON_EXCEPTION);
			return;
		}

		//设置变量
		m_dwBufferSize=dwApplySize;
		m_pcbDataBuffer=pcbDataBuffer;
		CopyMemory(m_pcbDataBuffer,pcbDeleteBuffer,m_dwBufferData);

		//删除缓冲
		SafeDeleteArray(pcbDeleteBuffer);
	}

	//设置变量
	m_bNeedBuffer=true;
	m_dwBufferData+=wDataSize;
	CopyMemory(m_pcbDataBuffer+m_dwBufferData-wDataSize,pData,wDataSize);

	return;
}

//网络读取
LRESULT CUWEBSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//读取包头
		if (m_bHeadValid==false)
		{
			//读取数据
			INT nResult=recv(m_hSocket,m_szHeadData+m_wHeadSize,sizeof(m_szHeadData)-m_wHeadSize,0);
			if (nResult==SOCKET_ERROR) throw TEXT("网络连接关闭，读取数据失败");

			//搜索头尾
			WORD wRequestID=m_wRequestID;
			WORD wWholeSize=m_wHeadSize+nResult;
			LPCSTR pszFinality=strstr((LPCSTR)m_szHeadData,"\r\n\r\n");

			//完成判断
			if (pszFinality!=NULL)
			{
				//设置变量
				m_bHeadValid=true;
				m_wHeadSize=(WORD)(pszFinality-m_szHeadData)+(WORD)strlen("\r\n\r\n");

				//提取状态
				INT nStatusCode=0;
				for (WORD i=0;i<m_wHeadSize;i++)
				{
					if (m_szHeadData[i]==' ')
					{
						//变量定义
						WORD nStatusOff=0;
						CHAR szStatusString[16]="";

						//提取字符
						do
						{
							szStatusString[nStatusOff++]=m_szHeadData[i++];
						} while (m_szHeadData[i]!=' ');

						//设置变量
						szStatusString[nStatusOff]=0;
						nStatusCode=atol(szStatusString);

						break;
					}
				}

				//包头事件
				ASSERT(m_pIWEBSocketSink!=NULL);
				m_pIWEBSocketSink->OnSocketDataHead(m_szHeadData,m_wHeadSize,nStatusCode);

				//流包事件
				if ((wWholeSize>m_wHeadSize)&&(wRequestID==m_wRequestID))
				{
					ASSERT(m_pIWEBSocketSink!=NULL);
					m_pIWEBSocketSink->OnSocketDataMain(m_szHeadData+m_wHeadSize,wWholeSize-m_wHeadSize);
				}
			}

			return 1;
		}
		else
		{
			//读取数据
			char cbReadBuffer[4096];
			INT nReadSize=recv(m_hSocket,cbReadBuffer,sizeof(cbReadBuffer),0);
			if (nReadSize==SOCKET_ERROR) throw TEXT("网络连接关闭，读取数据失败");

			//事件通知
			ASSERT(m_pIWEBSocketSink!=NULL);
			m_pIWEBSocketSink->OnSocketDataMain(cbReadBuffer,nReadSize);
		}
	}
	catch (...) 
	{ 
		CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}

//网络发送
LRESULT CUWEBSocket::OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam)
{
	//缓冲判断
	if ((m_bNeedBuffer==true)&&(m_dwBufferData>0L))
	{
		//变量定义
		DWORD dwTotalCount=0;
		DWORD dwPacketSize=4096;

		//发送数据
		while (dwTotalCount<m_dwBufferData)
		{
			//发生数据
			WORD wSendSize=(WORD)__min(dwPacketSize,m_dwBufferData-dwTotalCount);
			INT nSendCount=send(m_hSocket,(char *)m_pcbDataBuffer+dwTotalCount,wSendSize,0);

			//错误判断
			if (nSendCount==SOCKET_ERROR)
			{
				//缓冲判断
				if (WSAGetLastError()==WSAEWOULDBLOCK)
				{
					//设置变量
					m_bNeedBuffer=false;
					m_dwBufferData-=dwTotalCount;

					//移动内存
					if (m_dwBufferData>0L)
					{
						m_bNeedBuffer=true;
						MoveMemory(m_pcbDataBuffer,m_pcbDataBuffer+dwTotalCount,m_dwBufferData);
					}

					return 1L;
				}

				//关闭连接
				CloseSocket(SHUT_REASON_EXCEPTION);

				return 1L;
			}

			//设置变量
			dwTotalCount+=nSendCount;
		}

		//设置变量
		m_dwBufferData=0L;
		m_bNeedBuffer=false;
	}

	return 1L;
}

//网络关闭
LRESULT CUWEBSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	//关闭连接
	CloseSocket(SHUT_REASON_REMOTE);

	return 1;
}

//网络连接
LRESULT CUWEBSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//获取错误
	INT nErrorCode=WSAGETSELECTERROR(lParam);

	//判断状态
	if (nErrorCode==0)
	{
		m_cbSocketStatus=SOCKET_STATUS_CONNECT;
	}
	else
	{
		CloseSocket(SHUT_REASON_INSIDE);
	}

	//事件通知
	ASSERT(m_pIWEBSocketSink!=NULL);
	m_pIWEBSocketSink->OnSocketLink(nErrorCode);

	return 1;
}

//网络消息
LRESULT	CUWEBSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:		//数据读取
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
	case FD_CLOSE:		//网络关闭
		{ 
			return OnSocketNotifyClose(wParam,lParam);	
		}
	case FD_CONNECT:	//网络连接
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////


//构造函数
CUDownLoad::CUDownLoad()
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
CUDownLoad::~CUDownLoad()
{
	//关闭下载
	CloseDownLoad();

	return;
}

//接口查询
VOID * CUDownLoad::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWEBSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWEBSocketSink,Guid,dwQueryVer);
	return NULL;
}

//关闭下载
bool CUDownLoad::CloseDownLoad()
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
bool CUDownLoad::PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx,DWORD dwDownLoadPos)
{
	//状态效验
	ASSERT(m_DownLoadStatus==DownLoadStatus_Conclude);
	if (m_DownLoadStatus!=DownLoadStatus_Conclude) return false;

	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink)!=NULL);
	m_pIDownLoadSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink);

	//设置变量
	m_dwDownLoadPos=dwDownLoadPos;

	//添加参数
	TCHAR szWholeUrl[256]=TEXT("");
	_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s?x=%d"),pszUrl,GetTickCount());

	//发起连接
	if (ConnectServer(szWholeUrl)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//连接消息
bool CUDownLoad::OnSocketLink(INT nErrorCode)
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
bool CUDownLoad::OnSocketShut(BYTE cbShutReason)
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
VOID CUDownLoad::OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)
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
VOID CUDownLoad::OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)
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
				_sntprintf(m_DownLoadInfo.szUrlPath,CountArray(m_DownLoadInfo.szUrlPath),TEXT("%s?x=%I64d"),m_DownLoadInfo.szUrlPath,GetTickCount());

				//发送请求
				SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);
			}
			else
			{
				//关闭连接
				m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//添加参数
				TCHAR szWholeUrl[256]=TEXT("");
				_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s"),((CA2CT)szLocation));
				_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s?x=%d"),szWholeUrl,GetTickCount());

				//请求连接
				ConnectServer((LPCTSTR)szWholeUrl);
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
bool CUDownLoad::ConnectServer(LPCTSTR pszUrl)
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
bool CUDownLoad::SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath)
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
INTERNET_SCHEME CUDownLoad::GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo)
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
