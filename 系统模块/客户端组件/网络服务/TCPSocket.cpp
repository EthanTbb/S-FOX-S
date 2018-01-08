
#include "Stdafx.h"
#include "TCPSocket.h"

#define ID_SOCKET_WND		10 //  SOCKET ���� ID
#define WM_SOCKET_NOTIFY	WM_USER+10 //  SOCKET ��Ϣ

BEGIN_MESSAGE_MAP(CTCPSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY,OnSocketNotify)
END_MESSAGE_MAP()

// ���캯��
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

// ��������
CTCPSocket::~CTCPSocket()
{
	CloseSocket();
}

// �ӿڲ�ѯ
void * CTCPSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocket,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocket,Guid,dwQueryVer);
	return NULL;
}

// ���������
bool CTCPSocket::SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	m_ProxyServerType = cbProxyType;
	m_ProxyServerInfo = ProxyServer;
	return true;
}

// ���ýӿ�
bool CTCPSocket::SetTCPSocketSink(IUnknownEx* pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pITCPSocketSink = (ITCPSocketSink*)pIUnknownEx->QueryInterface(IID_ITCPSocketSink,VER_ITCPSocketSink);
	ASSERT(m_pITCPSocketSink != NULL);
	return (m_pITCPSocketSink != NULL);
}

// ��ȡ�ӿ�
void* CTCPSocket::GetTCPSocketSink(const IID& Guid, DWORD dwQueryVer)
{
	if (m_pITCPSocketSink == NULL) return NULL;
	return m_pITCPSocketSink->QueryInterface(Guid,dwQueryVer);
}

// ���ӷ�����
BYTE CTCPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	// Ч�����
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(m_cbSocketStatus == SOCKET_STATUS_IDLE);

	// Ч��״̬
	if (m_hSocket!= INVALID_SOCKET) throw TEXT("���� SOCKET ����Ѿ�����");
	if (m_cbSocketStatus != SOCKET_STATUS_IDLE) throw TEXT("����״̬���ǵȴ�����״̬");
	if (dwServerIP == INADDR_NONE) throw TEXT("Ŀ���������ַ��ʽ����ȷ��������ٴγ��ԣ�");

	// ���ò���
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0xA8C543FF;
	m_dwRecvXorKey = 0xA8C543FF;
	m_dwSendTickCount = GetTickCount()/1000L;
	m_dwRecvTickCount = GetTickCount()/1000L;

	try
	{
		// ���� SOCKET
		m_hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET ����ʧ��");
		if ((m_hWnd == NULL)&&(!Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),ID_SOCKET_WND,NULL)))
			throw TEXT("SOCKET �ڲ����ڴ���ʧ��");

		// ��д��������ַ
		m_wSocketID = (WORD)m_hSocket;
		SOCKADDR_IN SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_family = AF_INET;
		SocketAddr.sin_port = htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

		// ���ӷ�����
		int iErrorCode = 0;
		WSASetLastError(0);
		// if (m_ProxyInfo.cbProxyType! = PROXY_NONE)
		// {
		// 	throw TEXT("δ֧�ִ��������");
		// }
		// else
		{
			// �󶨴���
			iErrorCode = WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ|FD_CONNECT|FD_CLOSE);
			if (iErrorCode == SOCKET_ERROR) throw TEXT("���ڲ����ڴ���");

			// ���ӷ�����
			iErrorCode = connect(m_hSocket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr));
			if (iErrorCode == SOCKET_ERROR)
			{
				iErrorCode = WSAGetLastError();
				if (iErrorCode != WSAEWOULDBLOCK)
				{
					static TCHAR szBuffer[64];
					_sntprintf(szBuffer, sizeof(szBuffer), TEXT("���ӷ������󣬴������ %d"), iErrorCode);
					throw szBuffer;
				}
			}

			// ���ñ���
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
		throw TEXT("���Ӳ���δ֪�쳣����");
		return CONNECT_EXCEPTION;
	}

	
}

// ���ӷ�����
BYTE CTCPSocket::Connect(const LPCTSTR szServerIP, WORD wPort)
{
	// Ч������
	ASSERT(wPort != 0);
	ASSERT(szServerIP != NULL);
	if ((szServerIP == NULL)||(wPort == 0)) return false;
	return Connect(TranslateAddr(szServerIP),wPort);
}

// ���ͺ���
WORD CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	// Ч��״̬
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	// ��������
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head* pHead = (TCP_Head*)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	// ��������
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	// ��������
	return SendDataBuffer(cbDataBuffer,wSendSize);
}

// ���ͺ���
WORD CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void* pData, WORD wDataSize)
{
	// Ч��״̬
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	// Ч���С
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	// ��������
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

		// ��������
		WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

		// ��������
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

		// ��������
		WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

		// ��������
		return SendDataBuffer(cbDataBuffer,wSendSize);
	}
}

// �ر�����
void CTCPSocket::CloseSocket()
{
	// �ر�����
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

	// �ָ�����
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

// �ر�����
VOID CTCPSocket::CloseSocket(BYTE cbShutReason)
{
	// �ر�����
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

	// �ָ�����
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

// ��������
WORD CTCPSocket::SendDataBuffer(void* pBuffer, WORD wSendSize)
{
	// Ч�����
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);
	
	// ��������
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

//���ӳ��
WORD CTCPSocket::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold*241103L+2533101L)>>16);
}

//ӳ�䷢������
BYTE CTCPSocket::MapSendByte(BYTE const cbData)
{
	BYTE cbMap;
	cbMap = g_SendByteMap[cbData];
	return cbMap;
}

//ӳ���������
BYTE CTCPSocket::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap;
	cbMap = g_RecvByteMap[cbData];
	return cbMap;
}

// ���ͷ�������ַ
DWORD CTCPSocket::TranslateAddr(LPCTSTR pszServerAddr)
{
	// ת����ַ
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

// ��������
WORD CTCPSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));

	//��д��Ϣͷ
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

	//���ñ���
	m_dwSendPacketCount++;

	return wDataSize;
}

// ��������
WORD CTCPSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//Ч�������ֽ�ӳ��
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	for (i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
	}

	return wDataSize;
}

// ����
LRESULT CTCPSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	// �ж�״̬
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0) m_cbSocketStatus = SOCKET_STATUS_CONNECT;
	else CloseSocket(SHUT_REASON_TIME_OUT);

	// ����֪ͨ
	m_pITCPSocketSink->OnEventTCPSocketLink(m_wSocketID,iErrorCode);

	return 1;
}

// �����ȡ
LRESULT CTCPSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		// ��ȡ����
		int iRetCode = recv(m_hSocket, (char*)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		// ��������
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
		TCP_Head* pHead = (TCP_Head*)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(TCP_Head))
		{
			// Ч�����
			wPacketSize = pHead->TCPInfo.wPacketSize;
			ASSERT(pHead->TCPInfo.cbDataKind == DK_MAPPED);
			ASSERT(wPacketSize <= SOCKET_TCP_BUFFER);
			if (pHead->TCPInfo.cbDataKind != DK_MAPPED) throw TEXT("ֻ֧��ӳ�����");
			if (wPacketSize > SOCKET_TCP_BUFFER) throw TEXT("���ݰ�̫��");
			if (m_wRecvSize < wPacketSize) return 1;

			// ��������
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer,m_cbRecvBuf,wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);

			// ��������
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(TCP_Head));

			// ��������
			WORD wDataSize = wRealySize-sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head*)cbDataBuffer)->CommandInfo;

			// �ں�����
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET: // ������
					{
						// ��������
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}

			// ��������
			bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(m_wSocketID, Command, pDataBuffer, wDataSize);
			if (bSuccess == false) throw TEXT("�������ݰ�����ʧ��");
		};
	}
	catch (...) 
	{ 
		CloseSocket(SHUT_REASON_EXCEPTION); 
	}

	return 1;
}

// ����ر�
LRESULT CTCPSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket(SHUT_REASON_REMOTE);
	return 1;
}

// SOCKET ��Ϣ�������
LRESULT	CTCPSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT: // ��������
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
	case FD_READ: // ���ݶ�ȡ
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
	case FD_CLOSE: // ����ر�
		{ 
			return OnSocketNotifyClose(wParam,lParam);	
		}
	}

	return 0;
}

// ����������
DECLARE_CREATE_MODULE(TCPSocket)
