
#include "StdAfx.h"
#include "udpsocket.h"

// ���캯��
CUDPSocket::CUDPSocket()
{
}

// ��������
CUDPSocket::~CUDPSocket()
{
}

// �ӿڲ�ѯ
VOID* CUDPSocket::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	//QUERYINTERFACE(IUDPSocket,Guid,dwQueryVer);
	//QUERYINTERFACE_IUNKNOWNEX(IUDPSocket,Guid,dwQueryVer);
	return NULL;
}

//���ñ�ʶ
VOID CUDPSocket::SetSocketID(WORD wSocketID)
{

}

//���ýӿ�
bool CUDPSocket::SetUDPSocketSink(IUnknownEx * pIUnknownEx)
{
	//ASSERT(pIUnknownEx != NULL);
	//m_pITCPSocketSink = (ITCPSocketSink*)pIUnknownEx->QueryInterface(IID_IUDPSocketSink,VER_IUDPSocketSink);
	//ASSERT(m_pITCPSocketSink != NULL);
	//return (m_pITCPSocketSink != NULL);

	return false;
}

//����״̬
BYTE CUDPSocket::GetSocketStatus()
{
	return SOCKET_STATUS_IDLE;
}

//���ͼ��
DWORD CUDPSocket::GetLastSendTick()
{
	return 0;
}

//���ռ��
DWORD CUDPSocket::GetLastRecvTick()
{
	return 0;
}

//�ر�����
VOID CUDPSocket::CloseSocket()
{

}

//���Ӳ���
BYTE CUDPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	return CONNECT_FAILURE;
}

//���Ӳ���
BYTE CUDPSocket::Connect(LPCTSTR pszServerIP, WORD wPort)
{
	return CONNECT_FAILURE;
}

//���ͺ���
WORD CUDPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	return 0;
}

//���ͺ���
WORD CUDPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////

// ����������
DECLARE_CREATE_MODULE(UDPSocket)
