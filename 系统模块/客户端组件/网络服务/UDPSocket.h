
#ifndef UDP_SOCKET_HEAD_FILE
#define UDP_SOCKET_HEAD_FILE
#pragma once

#include "WHSocketHead.h"

//UDP ��������(û��ʵ��)
class CUDPSocket : public CWnd, public IUDPSocket
{
	// ��������
public:
	// ���캯��
	CUDPSocket();
	// ��������
	virtual ~CUDPSocket();

	// �����ӿ�
public:
	// �ͷŶ���
	virtual VOID Release() { delete this; }
	// �ӿڲ�ѯ
	virtual VOID* QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//���ñ�ʶ
	virtual VOID SetSocketID(WORD wSocketID);
	//���ýӿ�
	virtual bool SetUDPSocketSink(IUnknownEx * pIUnknownEx);

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual BYTE GetSocketStatus();
	//���ͼ��
	virtual DWORD GetLastSendTick();
	//���ռ��
	virtual DWORD GetLastRecvTick();

	//�����ӿ�
public:
	//�ر�����
	virtual VOID CloseSocket();
	//���Ӳ���
	virtual BYTE Connect(DWORD dwServerIP, WORD wPort);
	//���Ӳ���
	virtual BYTE Connect(LPCTSTR pszServerIP, WORD wPort);
	//���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

#endif //UDP_SOCKET_HEAD_FILE
