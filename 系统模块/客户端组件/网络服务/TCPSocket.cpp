
#include "Stdafx.h"
#include "TCPSocket.h"

#define ID_SOCKET_WND		10 //  SOCKET 窗口 ID
#define WM_SOCKET_NOTIFY	WM_USER+10 //  SOCKET 消息

BEGIN_MESSAGE_MAP(CTCPSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY,OnSocketNotify)
END_MESSAGE_MAP()

// 构造函数
CTCPSocket::CTCPSocket()
{
	m_wRecvSize  =  0;
	m_cbSendRound  =  0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_hSocket = INVALID_SOCKET;
	m_pITCPSocketSink = NULL;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
}

// 析构函数
CTCPSocket::~CTCPSocket()
{
	CloseSocket();
}

// 接口查询
void * CTCPSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocket,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocket,Guid,dwQueryVer);
	return NULL;
}

// 代理服务器
bool CTCPSocket::SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	m_ProxyServerType = cbProxyType;
	m_ProxyServerInfo = ProxyServer;
	return true;
}

// 设置接口
bool CTCPSocket::SetTCPSocketSink(IUnknownEx* pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pITCPSocketSink = (ITCPSocketSink*)pIUnknownEx->QueryInterface(IID_ITCPSocketSink,VER_ITCPSocketSink);
	ASSERT(m_pITCPSocketSink != NULL);
	return (m_pITCPSocketSink != NULL);
}

// 获取接口
void* CTCPSocket::GetTCPSocketSink(const IID& Guid, DWORD dwQueryVer)
{
	if (m_pITCPSocketSink == NULL) return NULL;
	return m_pITCPSocketSink->QueryInterface(Guid,dwQueryVer);
}

// 连接服务器
BYTE CTCPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	// 效验参数
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(m_cbSocketStatus == SOCKET_STATUS_IDLE);

	// 效验状态
	if (m_hSocket!= INVALID_SOCKET) throw TEXT("连接 SOCKET 句柄已经存在");
	if (m_cbSocketStatus != SOCKET_STATUS_IDLE) throw TEXT("连接状态不是等待连接状态");
	if (dwServerIP == INADDR_NONE) throw TEXT("目标服务器地址格式不正确，请检查后再次尝试！");

	// 设置参数
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0xA8C543FF;
	m_dwRecvXorKey = 0xA8C543FF;
	m_dwSendTickCount = GetTickCount()/1000L;
	m_dwRecvTickCount = GetTickCount()/1000L;

	try
	{
		// 建立 SOCKET
		m_hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET 创建失败");
		if ((m_hWnd == NULL)&&(!Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),ID_SOCKET_WND,NULL)))
			throw TEXT("SOCKET 内部窗口创建失败");

		// 填写服务器地址
		m_wSocketID = (WORD)m_hSocket;
		SOCKADDR_IN SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_family = AF_INET;
		SocketAddr.sin_port = htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

		// 连接服务器
		int iErrorCode = 0;
		WSASetLastError(0);
		// if (m_ProxyInfo.cbProxyType! = PROXY_NONE)
		// {
		// 	throw TEXT("未支持代理服务器");
		// }
		// else
		{
			// 绑定窗口
			iErrorCode = WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ|FD_CONNECT|FD_CLOSE);
			if (iErrorCode == SOCKET_ERROR) throw TEXT("绑定内部窗口错误");

			// 连接服务器
			iErrorCode = connect(m_hSocket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr));
			if (iErrorCode == SOCKET_ERROR)
			{
				iErrorCode = WSAGetLastError();
				if (iErrorCode != WSAEWOULDBLOCK)
				{
					static TCHAR szBuffer[64];
					_sntprintf(szBuffer, sizeof(szBuffer), TEXT("连接发生错误，错误代码 %d"), iErrorCode);
					throw szBuffer;
				}
			}

			// 设置变量
			m_cbSocketStatus = SOCKET_STATUS_WAIT;
		}

		return CONNECT_SUCCESS;
	}
	catch (LPCTSTR pszError)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
		throw pszError;
		return CONNECT_FAILURE;
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
		throw TEXT("连接产生未知异常错误");
		return CONNECT_EXCEPTION;
	}

	
}

// 连接服务器
BYTE CTCPSocket::Connect(const LPCTSTR szServerIP, WORD wPort)
{
	// 效验数据
	ASSERT(wPort != 0);
	ASSERT(szServerIP != NULL);
	if ((szServerIP == NULL)||(wPort == 0)) return false;
	return Connect(TranslateAddr(szServerIP),wPort);
}

// 发送函数
WORD CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	// 效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	// 构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head* pHead = (TCP_Head*)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	// 加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	// 发送数据
	return SendDataBuffer(cbDataBuffer,wSendSize);
}

// 发送函数
WORD CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void* pData, WORD wDataSize)
{
	// 效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	// 效验大小
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	// 构造数据
	if (m_dwSendPacketCount == 0)
	{
		BYTE cbDataBuffer[SOCKET_TCP_BUFFER+2*sizeof(DWORD)];
		TCP_Head* pHead = (TCP_Head*)cbDataBuffer;
		pHead->CommandInfo.wMainCmdID = wMainCmdID;
		pHead->CommandInfo.wSubCmdID = wSubCmdID;
		if (wDataSize>0)
		{
			ASSERT(pData != NULL);
			CopyMemory(pHead+1,pData,wDataSize);
		}

		// 加密数据
		WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

		// 发送数据
		return SendDataBuffer(cbDataBuffer,wSendSize);
	}
	else
	{
		BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
		TCP_Head* pHead = (TCP_Head*)cbDataBuffer;
		pHead->CommandInfo.wMainCmdID = wMainCmdID;
		pHead->CommandInfo.wSubCmdID = wSubCmdID;
		if (wDataSize>0)
		{
			ASSERT(pData != NULL);
			CopyMemory(pHead+1,pData,wDataSize);
		}

		// 加密数据
		WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

		// 发送数据
		return SendDataBuffer(cbDataBuffer,wSendSize);
	}
}

// 关闭连接
void CTCPSocket::CloseSocket()
{
	// 关闭连接
	bool bClose = (m_hSocket != INVALID_SOCKET);
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	if (m_hSocket != INVALID_SOCKET)
	{
		WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		m_cbSocketStatus = SOCKET_STATUS_IDLE;
	}
	if ((bClose == true) && (m_pITCPSocketSink != NULL))
	{
		ASSERT(m_pITCPSocketSink != NULL);
		try { m_pITCPSocketSink->OnEventTCPSocketShut(m_wSocketID, SHUT_REASON_NORMAL); }
		catch (...) {}
	}

	// 恢复数据
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;

	return;
}

// 关闭连接
VOID CTCPSocket::CloseSocket(BYTE cbShutReason)
{
	// 关闭连接
	bool bClose = (m_hSocket != INVALID_SOCKET);
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	if (m_hSocket != INVALID_SOCKET)
	{
		WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		m_cbSocketStatus = SOCKET_STATUS_IDLE;
	}
	if ((bClose == true) && (m_pITCPSocketSink != NULL))
	{
		ASSERT(m_pITCPSocketSink != NULL);
		try { m_pITCPSocketSink->OnEventTCPSocketShut(m_wSocketID, cbShutReason); }
		catch (...) {}
	}

	// 恢复数据
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;

	return;
}

// 发送数据
WORD CTCPSocket::SendDataBuffer(void* pBuffer, WORD wSendSize)
{
	// 效验参数
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);
	
	// 发送数据
	WORD wSended = 0;
	while (wSended < wSendSize)
	{
		int iErrorCode = send(m_hSocket, (char*)pBuffer+wSended, wSendSize-wSended,0);
		if (iErrorCode == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				m_dwSendTickCount = GetTickCount() / 1000L;
				return true;
			}
			return 0;
		}
		wSended += iErrorCode;
	}
	m_dwSendTickCount = GetTickCount() / 1000L;

	return wSended;
}

//随机映射
WORD CTCPSocket::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold*241103L+2533101L)>>16);
}

//映射发送数据
BYTE CTCPSocket::MapSendByte(BYTE const cbData)
{
	BYTE cbMap;
	cbMap = g_SendByteMap[cbData];
	return cbMap;
}

//映射接收数据
BYTE CTCPSocket::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap;
	cbMap = g_RecvByteMap[cbData];
	return cbMap;
}

// 解释服务器地址
DWORD CTCPSocket::TranslateAddr(LPCTSTR pszServerAddr)
{
	// 转化地址
	DWORD dwServerIP = inet_addr(CT2A(pszServerAddr));

	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(CT2A(pszServerAddr));
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

BYTE CTCPSocket::ProxyServerTesting()
{
	return 0;
}

// 加密数据
WORD CTCPSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	WORD i = 0;
	//效验参数
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));

	//填写信息头
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.wPacketSize = wDataSize;
	pHead->TCPInfo.cbDataKind = DK_MAPPED;


	BYTE checkCode = 0;

	for (WORD i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		checkCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}
	pHead->TCPInfo.cbCheckCode = ~checkCode+1;

	//设置变量
	m_dwSendPacketCount++;

	return wDataSize;
}

// 解密数据
WORD CTCPSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	WORD i = 0;
	//效验参数
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//效验码与字节映射
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	for (i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
	}

	return wDataSize;
}

// 网络
LRESULT CTCPSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	// 判断状态
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0) m_cbSocketStatus = SOCKET_STATUS_CONNECT;
	else CloseSocket(SHUT_REASON_TIME_OUT);

	// 发送通知
	m_pITCPSocketSink->OnEventTCPSocketLink(m_wSocketID,iErrorCode);

	return 1;
}

// 网络读取
LRESULT CTCPSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		// 读取数据
		int iRetCode = recv(m_hSocket, (char*)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("网络连接关闭，读取数据失败");
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		// 变量定义
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
		TCP_Head* pHead = (TCP_Head*)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(TCP_Head))
		{
			// 效验参数
			wPacketSize = pHead->TCPInfo.wPacketSize;
			ASSERT(pHead->TCPInfo.cbDataKind == DK_MAPPED);
			ASSERT(wPacketSize <= SOCKET_TCP_BUFFER);
			if (pHead->TCPInfo.cbDataKind != DK_MAPPED) throw TEXT("只支持映射加密");
			if (wPacketSize > SOCKET_TCP_BUFFER) throw TEXT("数据包太大");
			if (m_wRecvSize < wPacketSize) return 1;

			// 拷贝数据
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer,m_cbRecvBuf,wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);

			// 解密数据
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(TCP_Head));

			// 解释数据
			WORD wDataSize = wRealySize-sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head*)cbDataBuffer)->CommandInfo;

			// 内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET: // 网络检测
					{
						// 发送数据
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}

			// 处理数据
			bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(m_wSocketID, Command, pDataBuffer, wDataSize);
			if (bSuccess == false) throw TEXT("网络数据包处理失败");
		};
	}
	catch (...) 
	{ 
		CloseSocket(SHUT_REASON_EXCEPTION); 
	}

	return 1;
}

// 网络关闭
LRESULT CTCPSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket(SHUT_REASON_REMOTE);
	return 1;
}

// SOCKET 消息处理程序
LRESULT	CTCPSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT: // 网络连接
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
	case FD_READ: // 数据读取
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
	case FD_CLOSE: // 网络关闭
		{ 
			return OnSocketNotifyClose(wParam,lParam);	
		}
	}

	return 0;
}

// 建立对象函数
DECLARE_CREATE_MODULE(TCPSocket)
