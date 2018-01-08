
#include "StdAfx.h"
#include "udpsocket.h"

// 构造函数
CUDPSocket::CUDPSocket()
{
}

// 析构函数
CUDPSocket::~CUDPSocket()
{
}

// 接口查询
VOID* CUDPSocket::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	//QUERYINTERFACE(IUDPSocket,Guid,dwQueryVer);
	//QUERYINTERFACE_IUNKNOWNEX(IUDPSocket,Guid,dwQueryVer);
	return NULL;
}

//设置标识
VOID CUDPSocket::SetSocketID(WORD wSocketID)
{

}

//设置接口
bool CUDPSocket::SetUDPSocketSink(IUnknownEx * pIUnknownEx)
{
	//ASSERT(pIUnknownEx != NULL);
	//m_pITCPSocketSink = (ITCPSocketSink*)pIUnknownEx->QueryInterface(IID_IUDPSocketSink,VER_IUDPSocketSink);
	//ASSERT(m_pITCPSocketSink != NULL);
	//return (m_pITCPSocketSink != NULL);

	return false;
}

//网络状态
BYTE CUDPSocket::GetSocketStatus()
{
	return SOCKET_STATUS_IDLE;
}

//发送间隔
DWORD CUDPSocket::GetLastSendTick()
{
	return 0;
}

//接收间隔
DWORD CUDPSocket::GetLastRecvTick()
{
	return 0;
}

//关闭连接
VOID CUDPSocket::CloseSocket()
{

}

//连接操作
BYTE CUDPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	return CONNECT_FAILURE;
}

//连接操作
BYTE CUDPSocket::Connect(LPCTSTR pszServerIP, WORD wPort)
{
	return CONNECT_FAILURE;
}

//发送函数
WORD CUDPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	return 0;
}

//发送函数
WORD CUDPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////

// 建立对象函数
DECLARE_CREATE_MODULE(UDPSocket)
