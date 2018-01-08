#include "StdAfx.h"
#include "DownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#pragma comment (lib,"Wininet.lib")

//////////////////////////////////////////////////////////////////////////////////

//�궨��
#define ID_SOCKET_WND				100									//���ڱ�ʶ
#define WM_SOCKET_NOTIFY_EX			WM_USER+100							//��Ϣ��ʶ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUWEBSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY_EX,OnSocketNotify)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////


//���캯��
CUHeadTranslate::CUHeadTranslate()
{
	//���ñ���
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//������Ϣ
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//��������
CUHeadTranslate::~CUHeadTranslate()
{
}

//�������
VOID CUHeadTranslate::CleanData()
{
	//���ñ���
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//������Ϣ
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//���ò���
VOID CUHeadTranslate::SetHeadData(BYTE * pcbHeadData, WORD wDataLength)
{
	//���ñ���
	m_wDataLength=wDataLength;
	m_pcbHeadData=pcbHeadData;

	//��������
	WORD wStartIndex=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//�����ַ�
	LPCSTR pszEntityTag="ETag: \"";
	LPCSTR pszFileLength="Content-Length: ";

	//��������
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//�����ж�
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//��������
			WORD wLineLength=i-wStartIndex,wFlagLength=0;
			LPCSTR pszLineData=pszHeadString+wStartIndex;

			//���ñ���
			wStartIndex=(i+2);

			//�ļ���ʶ
			wFlagLength=(WORD)strlen(pszEntityTag);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszEntityTag,wFlagLength*sizeof(CHAR))==0))
			{
				//��������
				CHAR szFileEntityTag[CountArray(m_szFileEntityTag)]="";
				WORD wCopyCount=__min(CountArray(m_szFileEntityTag),wLineLength-wFlagLength-1);

				//��������
				ASSERT((wLineLength-wFlagLength-1)<=CountArray(szFileEntityTag));
				CopyMemory(szFileEntityTag,pszLineData+wFlagLength,wCopyCount*sizeof(CHAR));

				//��������
				szFileEntityTag[wCopyCount]=0;
				lstrcpyn(m_szFileEntityTag,(LPCTSTR)(CA2CT(szFileEntityTag)),CountArray(m_szFileEntityTag));

				continue;
			}

			//�ļ�����
			wFlagLength=(WORD)strlen(pszFileLength);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszFileLength,wFlagLength*sizeof(CHAR))==0))
			{
				//��������
				m_dwTotalFileSize=atol(pszLineData+wFlagLength);

				continue;
			}
		}
	}

	return;
}

//��ȡ����
WORD CUHeadTranslate::GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount)
{
	//Ч��״̬
	ASSERT((m_pcbHeadData!=NULL)&&(m_wDataLength>0));
	if ((m_pcbHeadData==NULL)||(m_wDataLength==0)) return 0;

	//��������
	WORD wStartIndex=0;
	WORD wLineSearch=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//��ȡ����
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//�����ж�
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//�����ж�
			if (wLineSearch==wLineIndex)
			{
				//���ó���
				ASSERT(wCharCount>=(i-wStartIndex));
				WORD wCopyCount=__min(wCharCount,i-wStartIndex);

				//���ý��
				szDataBuffer[wCopyCount]=0;
				CopyMemory(szDataBuffer,pszHeadString+wStartIndex,wCopyCount*sizeof(CHAR));

				return wCopyCount;
			}

			//���ñ���
			wLineSearch++;
			wStartIndex=(i+2);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUWEBSocket::CUWEBSocket()
{
	//��������
	m_wRequestID=1;
	m_bHeadValid=false;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;

	//���ݰ�ͷ
	m_wHeadSize=0;
	ZeroMemory(m_szHeadData,sizeof(m_szHeadData));

	//�������
	m_dwBufferData=0L;
	m_dwBufferSize=0L;
	m_bNeedBuffer=false;
	m_pcbDataBuffer=NULL;

	//�ں˱���
	m_pIWEBSocketSink=NULL;
	m_hSocket=INVALID_SOCKET;

	//������Ϣ
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return;
}

//��������
CUWEBSocket::~CUWEBSocket()
{
	//�ر�����
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//ɾ������
	SafeDeleteArray(m_pcbDataBuffer);

	return;
}

//���ýӿ�
bool CUWEBSocket::SetWEBSocketSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IWEBSocketSink)!=NULL);
		m_pIWEBSocketSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IWEBSocketSink);

		//�ɹ��ж�
		if (m_pIWEBSocketSink==NULL) return false;
	}
	else m_pIWEBSocketSink=NULL;

	return true;
}


//���ô���
bool CUWEBSocket::SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	switch (cbProxyType)
	{
	case PROXY_NONE:	//û�д���
		{
			//���ñ���
			m_cbProxyType=cbProxyType;
			ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

			return true;
		}
	case PROXY_HTTP:	//HTTP ����
	case PROXY_SOCKS4:	//SOKKS4 ����
	case PROXY_SOCKS5:	//SOKKS5 ����
		{
			//���ñ���
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

//�ر�����
VOID CUWEBSocket::CloseSocket(BYTE cbShutReason)
{
	//�رմ���
	DestroyWindow();

	//�������
	m_dwBufferData=0L;
	m_dwBufferSize=0L;
	m_bNeedBuffer=false;
	SafeDeleteArray(m_pcbDataBuffer);

	//״̬����
	m_bHeadValid=false;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;
	m_wRequestID=__max(1,m_wRequestID+1);

	//�ر��ж�
	if (m_hSocket!=INVALID_SOCKET)
	{
		//�ر�����
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;

		//�¼�֪ͨ
		if (cbShutReason!=SHUT_REASON_INSIDE)
		{
			ASSERT(m_pIWEBSocketSink!=NULL);
			if (m_pIWEBSocketSink!=NULL) m_pIWEBSocketSink->OnSocketShut(cbShutReason);
		}
	}

	return;
}

//���Ӳ���
BYTE CUWEBSocket::ConnectServer(LPCTSTR pszServer, WORD wPort)
{
	//Ч�����
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

	//���ò���
	m_wHeadSize=0;
	ZeroMemory(m_szHeadData,sizeof(m_szHeadData));

	//ת����ַ
	CT2CA HttpServer(pszServer);
	DWORD dwServerIP=inet_addr(HttpServer);

	//��������
	if (dwServerIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(HttpServer);
		if (lpHost!=NULL) dwServerIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	try
	{
		//���� SOCKET
		m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket==INVALID_SOCKET) 
		{
			throw CONNECT_EXCEPTION;
		}

		//��������
		if (m_hWnd==NULL)
		{
			CRect rcSocketWnd;
			Create(NULL,NULL,WS_CHILD,rcSocketWnd,GetDesktopWindow(),100);
		}

		//��������
		SOCKADDR_IN SocketAddr;
		ZeroMemory(&SocketAddr,sizeof(SocketAddr));

		//���ñ���
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr=dwServerIP;

		//��������
		if (m_cbProxyType!=PROXY_NONE)
		{
			//���Ӵ���
			DWORD dwResult=ConnectProxyServer();
			if (dwResult!=CONNECT_SUCCESS) throw dwResult;

			//��������
			switch (m_cbProxyType)
			{
			case PROXY_HTTP:	//HTTP ����
				{
					//��������
					char cbDataBuffer[512];
					BYTE * pcbServerIP=(BYTE *)&dwServerIP;
					_snprintf(cbDataBuffer,CountArray(cbDataBuffer),"CONNECT %d.%d.%d.%d:%d HTTP/1.0\r\nUser-Agent: WHPlatform/0.1\r\n\r\n",
						pcbServerIP[0],pcbServerIP[1],pcbServerIP[2],pcbServerIP[3],wPort);

					//��������
					INT nRequestLen=(INT)strlen(cbDataBuffer);
					if (send(m_hSocket,cbDataBuffer,nRequestLen,0)!=nRequestLen) throw CONNECT_PROXY_FAILURE;

					//������Ӧ
					INT nRecvCount=recv(m_hSocket,cbDataBuffer,sizeof(cbDataBuffer),0);
					if (nRecvCount>0) cbDataBuffer[nRecvCount]=0;

					//����ж�
					bool bSuccess=false;
					for (INT i=0;i<nRecvCount;i++)
					{
						if ((cbDataBuffer[i]==' ')&&(cbDataBuffer[i+1]=='2')&&(cbDataBuffer[i+2]=='0')&&(cbDataBuffer[i+3]=='0'))
						{
							bSuccess=true;
							break;
						}
					}

					//����ж�
					if (bSuccess==false) throw CONNECT_PROXY_FAILURE;

					break;
				}
			case PROXY_SOCKS5:	//Socks5 ����
				{
					//��������
					INT nOff=4;
					char cbDataBuffer[256]={5,1,0,1};

					//���ӵ�ַ
					CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
					nOff+=sizeof(SocketAddr.sin_addr);
					
					//���Ӷ˿�
					*(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
					nOff+=sizeof(WORD);

					//��������
					if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) throw CONNECT_PROXY_FAILURE;

					//������Ӧ
					INT nRecvCount=recv(m_hSocket,cbDataBuffer,4,0);
					if (nRecvCount!=4) throw CONNECT_PROXY_FAILURE;
					if (cbDataBuffer[0]!=5) throw CONNECT_PROXY_FAILURE;

					//ʧ���ж�
					if (cbDataBuffer[1]==0) throw CONNECT_PROXY_FAILURE;

					//��������
					INT nLeftDataLen=(cbDataBuffer[3]==1)?6:cbDataBuffer[4]+2;
					if (recv(m_hSocket,cbDataBuffer,nLeftDataLen,0)!=nLeftDataLen) throw CONNECT_PROXY_FAILURE;

					break;
				}
			case PROXY_SOCKS4:	//Socks4 ����
				{
					//��������
					INT nOff=2;
					char cbDataBuffer[256]={4,1};

					//���Ӷ˿�
					*(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
					nOff+=sizeof(WORD);

					//���ӵ�ַ
					CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
					nOff+=sizeof(SocketAddr.sin_addr);

					//�����ֶ�
					cbDataBuffer[nOff++]=0;

					//��������
					INT nResult=send(m_hSocket,cbDataBuffer,nOff,0);
					if (nResult!=nOff) throw CONNECT_PROXY_FAILURE;

					//������Ӧ
					nResult=recv(m_hSocket,cbDataBuffer,8,0);
					if (nResult!=8) throw CONNECT_PROXY_FAILURE;
					if (cbDataBuffer[0]!=0) throw CONNECT_PROXY_FAILURE;

					//ʧ���ж�
					if (cbDataBuffer[1]!=90) throw CONNECT_PROXY_FAILURE;

					break;
				}
			default:		//Ĭ�ϴ���
				{
					ASSERT(FALSE);
					throw CONNECT_PROXY_FAILURE;
				}
			}

			//���ӳɹ�
			WSASetLastError(0);

			//���ñ���
			m_cbSocketStatus=SOCKET_STATUS_CONNECT;

			//������Ϣ
			PostMessage(WM_SOCKET_NOTIFY_EX,0,MAKELONG(FD_CONNECT,0));

			//�󶨴���
			WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY_EX,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);
		}
		else
		{
			//�󶨴���
			WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY_EX,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);

			//��������
			INT nErrorCode=connect(m_hSocket,(SOCKADDR *)&SocketAddr,sizeof(SocketAddr));
			if ((nErrorCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSAEWOULDBLOCK)) throw CONNECT_EXCEPTION;

			//���ñ���
			m_cbSocketStatus=SOCKET_STATUS_WAIT;
		}
	}
	catch (...)
	{
		//�ر�����
		CloseSocket(SHUT_REASON_INSIDE);

		return CONNECT_EXCEPTION;
	}

	return CONNECT_SUCCESS;
}

//��������
WORD CUWEBSocket::SendRequestData(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT((wDataSize!=0)&&(pData!=NULL));
	if ((wDataSize==0)||(pData==NULL)) return 0L;

	//Ч��״̬
	if (m_hSocket==INVALID_SOCKET) return 0L;
	if (m_cbSocketStatus!=SOCKET_STATUS_CONNECT) return 0L;

	//���ñ���
	m_wHeadSize=0;
	m_bHeadValid=false;
	m_wRequestID=__max(1,m_wRequestID+1);

	//��������
	WORD wTotalCount=0;

	//��������
	while ((m_bNeedBuffer==false)&&(wTotalCount<wDataSize))
	{
		//��������
		INT nSendCount=send(m_hSocket,(char *)pData+wTotalCount,wDataSize-wTotalCount,0);

		//�����ж�
		if (nSendCount==SOCKET_ERROR)
		{
			//�����ж�
			if (WSAGetLastError()==WSAEWOULDBLOCK)
			{
				AmortizeBuffer((LPBYTE)pData+wTotalCount,wDataSize-wTotalCount);
				return wDataSize;
			}

			//�ر�����
			CloseSocket(SHUT_REASON_EXCEPTION);

			return 0L;
		}
		else
		{
			//���ñ���
			wTotalCount+=nSendCount;
		}
	}

	//��������
	if (wTotalCount>wDataSize)
	{
		AmortizeBuffer((LPBYTE)pData+wTotalCount,wDataSize-wTotalCount);
	}

	return wDataSize;
}

//���Ӵ���
BYTE CUWEBSocket::ConnectProxyServer()
{
	//Ч��״̬
	ASSERT(m_cbProxyType!=PROXY_NONE);
	if (m_cbProxyType==PROXY_NONE) return CONNECT_EXCEPTION;

	//ת����ַ
	CT2CA ProxyServer(m_ProxyServer.szProxyServer);
	DWORD dwProxyServer=inet_addr(ProxyServer);

	//��������
	if (dwProxyServer==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(ProxyServer);
		if (lpHost!=NULL) dwProxyServer=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	//��������
	SOCKADDR_IN SocketAddrProxy;
	ZeroMemory(&SocketAddrProxy,sizeof(SocketAddrProxy));

	//���ñ���
	SocketAddrProxy.sin_family=AF_INET;
	SocketAddrProxy.sin_addr.S_un.S_addr=dwProxyServer;
	SocketAddrProxy.sin_port=htons(m_ProxyServer.wProxyPort);

	//���Ӵ���
	INT nErrorCode=connect(m_hSocket,(sockaddr *)&SocketAddrProxy,sizeof(SocketAddrProxy));
	if (nErrorCode==SOCKET_ERROR) return CONNECT_PROXY_FAILURE;

	//��������
	if (m_cbProxyType==PROXY_SOCKS5)
	{
		//��������
		INT nOff=2;
		char cbDataBuffer[256]={5,1};

		//��������
		cbDataBuffer[nOff++]=(m_ProxyServer.szUserName[0]==0)?0:2;
		if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) return CONNECT_PROXY_FAILURE;

		//��������
		if (recv(m_hSocket,cbDataBuffer,2,0)!=2) return CONNECT_PROXY_FAILURE;

		//���ݷ���
		if (cbDataBuffer[0]!=5) return CONNECT_PROXY_FAILURE;
		if ((cbDataBuffer[1]!=0)&&(cbDataBuffer[1]!=2)) return CONNECT_PROXY_FAILURE;

		//�����֤
		if (cbDataBuffer[1]==2)
		{
			//��������
			INT nStringLen=0;
			CT2CA strUserName(m_ProxyServer.szUserName);
			CT2CA strPassword(m_ProxyServer.szPassword);

			//��¼�ʺ�
			nOff=0;
			cbDataBuffer[nOff++]=1;
			nStringLen=lstrlenA(strUserName);
			cbDataBuffer[nOff++]=(char)nStringLen;
			CopyMemory(cbDataBuffer+nOff,(LPCSTR)strUserName,nStringLen);
			nOff+=nStringLen;

			//��¼����
			nStringLen=lstrlenA(strPassword);
			cbDataBuffer[nOff++]=(char)nStringLen;
			CopyMemory(cbDataBuffer+nOff,(LPCSTR)strPassword,nStringLen);
			nOff+=nStringLen;

			//��������
			if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) return CONNECT_PROXY_FAILURE;

			//��������
			if (recv(m_hSocket,cbDataBuffer,2,0)!=2) return CONNECT_PROXY_FAILURE;

			//���ݷ���
			if ((cbDataBuffer[0]!=1)||(cbDataBuffer[1]!=0)) return CONNECT_PROXY_USER_INVALID;
		}
	}

	return CONNECT_SUCCESS;
}

//��������
VOID CUWEBSocket::AmortizeBuffer(VOID * pData, WORD wDataSize)
{
	//���뻺��
	if ((m_dwBufferData+wDataSize)>m_dwBufferSize)
	{
		//��������
		LPBYTE pcbDataBuffer=NULL;
		LPBYTE pcbDeleteBuffer=m_pcbDataBuffer;

		//�����С
		DWORD dwNeedSize=m_dwBufferData+wDataSize;
		DWORD dwApplySize=__max(dwNeedSize,m_dwBufferSize*2L);

		//���뻺��
		try
		{
			pcbDataBuffer=new BYTE [dwApplySize];
		}
		catch (...)	{ }

		//ʧ���ж�
		if (pcbDataBuffer==NULL)
		{
			CloseSocket(SHUT_REASON_EXCEPTION);
			return;
		}

		//���ñ���
		m_dwBufferSize=dwApplySize;
		m_pcbDataBuffer=pcbDataBuffer;
		CopyMemory(m_pcbDataBuffer,pcbDeleteBuffer,m_dwBufferData);

		//ɾ������
		SafeDeleteArray(pcbDeleteBuffer);
	}

	//���ñ���
	m_bNeedBuffer=true;
	m_dwBufferData+=wDataSize;
	CopyMemory(m_pcbDataBuffer+m_dwBufferData-wDataSize,pData,wDataSize);

	return;
}

//�����ȡ
LRESULT CUWEBSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//��ȡ��ͷ
		if (m_bHeadValid==false)
		{
			//��ȡ����
			INT nResult=recv(m_hSocket,m_szHeadData+m_wHeadSize,sizeof(m_szHeadData)-m_wHeadSize,0);
			if (nResult==SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");

			//����ͷβ
			WORD wRequestID=m_wRequestID;
			WORD wWholeSize=m_wHeadSize+nResult;
			LPCSTR pszFinality=strstr((LPCSTR)m_szHeadData,"\r\n\r\n");

			//����ж�
			if (pszFinality!=NULL)
			{
				//���ñ���
				m_bHeadValid=true;
				m_wHeadSize=(WORD)(pszFinality-m_szHeadData)+(WORD)strlen("\r\n\r\n");

				//��ȡ״̬
				INT nStatusCode=0;
				for (WORD i=0;i<m_wHeadSize;i++)
				{
					if (m_szHeadData[i]==' ')
					{
						//��������
						WORD nStatusOff=0;
						CHAR szStatusString[16]="";

						//��ȡ�ַ�
						do
						{
							szStatusString[nStatusOff++]=m_szHeadData[i++];
						} while (m_szHeadData[i]!=' ');

						//���ñ���
						szStatusString[nStatusOff]=0;
						nStatusCode=atol(szStatusString);

						break;
					}
				}

				//��ͷ�¼�
				ASSERT(m_pIWEBSocketSink!=NULL);
				m_pIWEBSocketSink->OnSocketDataHead(m_szHeadData,m_wHeadSize,nStatusCode);

				//�����¼�
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
			//��ȡ����
			char cbReadBuffer[4096];
			INT nReadSize=recv(m_hSocket,cbReadBuffer,sizeof(cbReadBuffer),0);
			if (nReadSize==SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");

			//�¼�֪ͨ
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

//���緢��
LRESULT CUWEBSocket::OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((m_bNeedBuffer==true)&&(m_dwBufferData>0L))
	{
		//��������
		DWORD dwTotalCount=0;
		DWORD dwPacketSize=4096;

		//��������
		while (dwTotalCount<m_dwBufferData)
		{
			//��������
			WORD wSendSize=(WORD)__min(dwPacketSize,m_dwBufferData-dwTotalCount);
			INT nSendCount=send(m_hSocket,(char *)m_pcbDataBuffer+dwTotalCount,wSendSize,0);

			//�����ж�
			if (nSendCount==SOCKET_ERROR)
			{
				//�����ж�
				if (WSAGetLastError()==WSAEWOULDBLOCK)
				{
					//���ñ���
					m_bNeedBuffer=false;
					m_dwBufferData-=dwTotalCount;

					//�ƶ��ڴ�
					if (m_dwBufferData>0L)
					{
						m_bNeedBuffer=true;
						MoveMemory(m_pcbDataBuffer,m_pcbDataBuffer+dwTotalCount,m_dwBufferData);
					}

					return 1L;
				}

				//�ر�����
				CloseSocket(SHUT_REASON_EXCEPTION);

				return 1L;
			}

			//���ñ���
			dwTotalCount+=nSendCount;
		}

		//���ñ���
		m_dwBufferData=0L;
		m_bNeedBuffer=false;
	}

	return 1L;
}

//����ر�
LRESULT CUWEBSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	//�ر�����
	CloseSocket(SHUT_REASON_REMOTE);

	return 1;
}

//��������
LRESULT CUWEBSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	INT nErrorCode=WSAGETSELECTERROR(lParam);

	//�ж�״̬
	if (nErrorCode==0)
	{
		m_cbSocketStatus=SOCKET_STATUS_CONNECT;
	}
	else
	{
		CloseSocket(SHUT_REASON_INSIDE);
	}

	//�¼�֪ͨ
	ASSERT(m_pIWEBSocketSink!=NULL);
	m_pIWEBSocketSink->OnSocketLink(nErrorCode);

	return 1;
}

//������Ϣ
LRESULT	CUWEBSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:		//���ݶ�ȡ
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
	case FD_CLOSE:		//����ر�
		{ 
			return OnSocketNotifyClose(wParam,lParam);	
		}
	case FD_CONNECT:	//��������
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////


//���캯��
CUDownLoad::CUDownLoad()
{
	//�ļ���Ϣ
	m_dwTotalSize=0L;
	m_dwDownLoadSize=0L;
	m_dwDownLoadPos=0;

	//״̬����
	m_DownLoadStatus=DownLoadStatus_Conclude;
	ZeroMemory(&m_DownLoadInfo,sizeof(m_DownLoadInfo));

	//�ӿڱ���
	m_pIDownLoadSink=NULL;

	//���ýӿ�
	m_WEBSocket.SetWEBSocketSink(QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//��������
CUDownLoad::~CUDownLoad()
{
	//�ر�����
	CloseDownLoad();

	return;
}

//�ӿڲ�ѯ
VOID * CUDownLoad::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWEBSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWEBSocketSink,Guid,dwQueryVer);
	return NULL;
}

//�ر�����
bool CUDownLoad::CloseDownLoad()
{
	//���ñ���
	m_dwTotalSize=0L;
	m_dwDownLoadSize=0L;
	m_DownLoadStatus=DownLoadStatus_Conclude;

	//״̬����
	m_HeadTranslate.CleanData();

	//�ر�����
	m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

	return true;
}

//ִ������
bool CUDownLoad::PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx,DWORD dwDownLoadPos)
{
	//״̬Ч��
	ASSERT(m_DownLoadStatus==DownLoadStatus_Conclude);
	if (m_DownLoadStatus!=DownLoadStatus_Conclude) return false;

	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink)!=NULL);
	m_pIDownLoadSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink);

	//���ñ���
	m_dwDownLoadPos=dwDownLoadPos;

	//��Ӳ���
	TCHAR szWholeUrl[256]=TEXT("");
	_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s?x=%d"),pszUrl,GetTickCount());

	//��������
	if (ConnectServer(szWholeUrl)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//������Ϣ
bool CUDownLoad::OnSocketLink(INT nErrorCode)
{
	//������
	if (nErrorCode!=0)
	{
		//�ر�����
		CloseDownLoad();

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Connect);

		return false;
	}

	//��������
	SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);

	return true;
}

//�ر���Ϣ
bool CUDownLoad::OnSocketShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//�ر�����
		CloseDownLoad();

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);
	}

	return true;
}

//���ݰ���
VOID CUDownLoad::OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)
{
	//Ч��״̬
	ASSERT(m_DownLoadStatus==DownLoadStatus_Service);
	if (m_DownLoadStatus!=DownLoadStatus_Service) return;

	//���ñ���
	m_dwDownLoadSize+=wStreamSize;

	//���ݴ���
	ASSERT(m_pIDownLoadSink!=NULL);
	if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDataStream(pcbMailData,wStreamSize);

	//����ж�
	if (m_dwDownLoadSize==m_dwTotalSize)
	{
		//�ر�����
		CloseDownLoad();

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Conclude);
	}

	return;
}

//���ݰ�ͷ
VOID CUDownLoad::OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)
{
	switch (nStatusCode)
	{
	case HTTP_STATUS_OK:				//��ȡ�ɹ�
	case HTTP_STATUS_PARTIAL_CONTENT:	//�ϵ�����
		{
			//���ñ���
			m_DownLoadStatus=DownLoadStatus_Service;
			m_HeadTranslate.SetHeadData((BYTE *)pcbHeadData,wHeadSize);

			//�ļ���Ϣ
			m_dwDownLoadSize=0L;
			m_dwTotalSize=m_HeadTranslate.GetTotalFileSize();

			//״̬֪ͨ
			ASSERT(m_pIDownLoadSink!=NULL);
			if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Service);

			//�¼�֪ͨ
			LPCTSTR pszEntityTag=m_HeadTranslate.GetFileEntityTag();
			m_pIDownLoadSink->OnDataInformation(nStatusCode==HTTP_STATUS_PARTIAL_CONTENT,m_dwTotalSize,pszEntityTag,m_DownLoadInfo.szUrlPath);

			return;
		}
	case HTTP_STATUS_MOVED:				//Ŀ����ת
	case HTTP_STATUS_REDIRECT:			//Ŀ����ת
	case HTTP_STATUS_REDIRECT_METHOD:	//Ŀ����ת
		{
			//��ȡλ��
			LPCSTR pszLocation=strstr((CHAR *)pcbHeadData,"Location: ");
			if (pszLocation==NULL) m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

			//��������
			CHAR szLocation[MAX_PATH]="";
			WORD wStartPos=(WORD)strlen("Location: ");
			
			//�����ַ
			WORD wOff=0;
			while (pszLocation[wStartPos+wOff]!='\r')
			{
				//Ч������
				ASSERT(wOff<CountArray(szLocation));
				if (wOff>=CountArray(szLocation)) m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//�����ַ�
				WORD wIndex=wOff++;
				szLocation[wIndex]=pszLocation[wStartPos+wIndex];
			}
			szLocation[wOff]=0;

			//ת�����
			if (szLocation[0]=='/')
			{
				//��������
				CA2CT strLocation(szLocation);
				lstrcpyn(m_DownLoadInfo.szUrlPath,(LPCTSTR)strLocation,CountArray(m_DownLoadInfo.szUrlPath));

				//��Ӳ���
				_sntprintf(m_DownLoadInfo.szUrlPath,CountArray(m_DownLoadInfo.szUrlPath),TEXT("%s?x=%I64d"),m_DownLoadInfo.szUrlPath,GetTickCount());

				//��������
				SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);
			}
			else
			{
				//�ر�����
				m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//��Ӳ���
				TCHAR szWholeUrl[256]=TEXT("");
				_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s"),((CA2CT)szLocation));
				_sntprintf(szWholeUrl,CountArray(szWholeUrl),TEXT("%s?x=%d"),szWholeUrl,GetTickCount());

				//��������
				ConnectServer((LPCTSTR)szWholeUrl);
			}

			return;
		}
	default:							//Ĭ�ϴ���
		{
			//�ر�����
			CloseDownLoad();

			//״̬֪ͨ
			ASSERT(m_pIDownLoadSink!=NULL);
			if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);

			return;
		}
	}

	return;
}

//��������
bool CUDownLoad::ConnectServer(LPCTSTR pszUrl)
{
	//��������
	BYTE cbResult=CONNECT_EXCEPTION;
	INTERNET_SCHEME nScheme=GetDownLoadInfo(pszUrl,m_DownLoadInfo);

	//���ͷ���
	switch (nScheme)
	{
	case INTERNET_SERVICE_FTP:		//FTP ����
		{
			cbResult=m_WEBSocket.ConnectServer(m_DownLoadInfo.szHostName,m_DownLoadInfo.wHostPort);
			break;
		}
	case INTERNET_SERVICE_HTTP:		//HTTP ����
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

	//�������
	if (cbResult==CONNECT_SUCCESS)
	{
		//����״̬
		m_DownLoadStatus=DownLoadStatus_Connect;

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Connect);
	}
	else
	{
		//ֹͣ����
		CloseDownLoad();

		//�¼�֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);

		return false;
	}

	return true;
}

//��������
bool CUDownLoad::SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath)
{
	//��������
	CHAR szRequestData[1024]="";
	CT2CA strUrlPath(pszUrlPath);
	CT2CA strHostName(pszHostName);

	//��������
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

	//��������
	m_WEBSocket.SendRequestData(szRequestData,(WORD)strlen(szRequestData));

	return true;
}

//��ȡ��Ϣ
INTERNET_SCHEME CUDownLoad::GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo)
{
	//��������
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	ZeroMemory(&DownLoadInfo,sizeof(DownLoadInfo));

	//�淶��ַ
	DWORD dwMaxLength=CountArray(szDownLoadUrl);
	DWORD dwFlags=ICU_BROWSER_MODE|ICU_NO_ENCODE|ICU_NO_META|ICU_ENCODE_SPACES_ONLY;
	if (InternetCanonicalizeUrl(pszUrl,szDownLoadUrl,&dwMaxLength,dwFlags)==FALSE) return INTERNET_SCHEME_UNKNOWN;

	//��ʽ��ַ
	URL_COMPONENTS Components;
	ZeroMemory(&Components,sizeof(Components));

	//���ñ���
	Components.dwStructSize=sizeof(Components);
	Components.lpszUrlPath=DownLoadInfo.szUrlPath;
	Components.lpszHostName=DownLoadInfo.szHostName;
	Components.dwUrlPathLength=CountArray(DownLoadInfo.szUrlPath);
	Components.dwHostNameLength=CountArray(DownLoadInfo.szHostName);
	if (InternetCrackUrl(szDownLoadUrl,0,0,&Components)==FALSE) return INTERNET_SCHEME_UNKNOWN;

	//���ñ���
	DownLoadInfo.wHostPort=Components.nPort;

	return Components.nScheme;
}

//////////////////////////////////////////////////////////////////////////////////
