
#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_
#pragma once

#include "WinSock2.h"
#include "WHSocketHead.h"

//  TCP ��������
class CTCPSocket : public CWnd, public ITCPSocket
{
	// ��������
public:
	// ���캯��
	CTCPSocket();
	// ��������
	virtual ~CTCPSocket();

	// �����ӿ�
public:
	// �ͷŶ���
	virtual void Release() { delete this; }
	// �ӿڲ�ѯ
	virtual void* QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	// ��ʶ�ӿ�
public:
	// ��ȡ��ʶ
	virtual WORD GetSocketID() { return m_wSocketID;}
	// ���ñ�ʶ
	virtual void SetSocketID(WORD wSocketID) { m_wSocketID = wSocketID; }

	// ���ýӿ�
public:
	// ���ýӿ�
	virtual bool SetTCPSocketSink(IUnknownEx* pIUnknownEx);
	// ��ȡ�ӿ�
	virtual void* GetTCPSocketSink(const IID& Guid, DWORD dwQueryVer);

	// ��Ϣ�ӿ�
public:
	// ��ȡ״̬
	virtual BYTE GetSocketStatus() { return m_cbSocketStatus; }
	// ���ͼ��
	virtual DWORD GetLastSendTick() { return m_dwSendTickCount; }
	// ���ռ��
	virtual DWORD GetLastRecvTick() { return m_dwRecvTickCount; }

	// ����ӿ�
public:
	// �������
	virtual BYTE ProxyServerTesting();
	//���ô���
	virtual bool SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer);

	// �����ӿ�
public:
	// �ر�����
	virtual void CloseSocket();
	// ���Ӳ���
	virtual BYTE Connect(DWORD dwServerIP, WORD wPort);
	// ���Ӳ���
	virtual BYTE Connect(LPCTSTR szServerIP, WORD wPort);
	// ���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	// ���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, void* pData, WORD wDataSize);

	// ��������
protected:
	// ���Ӵ���
	DWORD ConnectProxyServer();
	// ��ַ����
	DWORD TranslateAddr(LPCTSTR pszServerAddr);

	// ��������
protected:
	// �ر�����
	void CloseSocket(BYTE cbShutReason);
	// ��������
	WORD SendDataBuffer(void* pBuffer, WORD wSendSize);

	// ���ܺ���
protected:
	// ��������
	WORD CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize);
	// ��������
	WORD EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	// ��������
private:
	// �ֽ�ӳ��
	inline WORD SeedRandMap(WORD wSeed);
	// ����ӳ��
	inline BYTE MapSendByte(BYTE cbData);
	// ����ӳ��
	inline BYTE MapRecvByte(BYTE cbData);

	// ��Ϣӳ��
protected:
	// ʱ����Ϣ
	void OnTimer(UINT_PTR nIDEvent);
	// ������Ϣ
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	// ��Ϣ����
protected:
	// �����ȡ
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	// ���緢��
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	// ����ر�
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	// ��������
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// ��������
private:
	WORD m_wSocketID; //  �����ʶ
	BYTE m_cbSocketStatus; //  ����״̬

	//  �ں˱���
private:
	SOCKET m_hSocket; //  ���Ӿ��
	ITCPSocketSink* m_pITCPSocketSink; //  �ص��ӿ�

	//  ������Ϣ
private:
	BYTE m_ProxyServerType; //  ��������
	tagProxyServer m_ProxyServerInfo; //  ������Ϣ

	//  ���ձ���
private:
	WORD m_wRecvSize; //  ���ճ���
	BYTE m_cbRecvBuf[SOCKET_TCP_BUFFER*10]; //  ���ջ���

	//  �������
private:
	bool m_bNeedBuffer; //  ����״̬
	DWORD m_dwBufferData; //  ��������
	DWORD m_dwBufferSize; //  �����С
	LPBYTE m_pcbDataBuffer; //  ��������

	//  ��������
private:
	BYTE m_cbSendRound; //  �ֽ�ӳ��
	BYTE m_cbRecvRound; //  �ֽ�ӳ��
	DWORD m_dwSendXorKey; //  ������Կ
	DWORD m_dwRecvXorKey; //  ������Կ

	//  ��������
private:
	DWORD m_dwSendTickCount; //  ����ʱ��
	DWORD m_dwRecvTickCount; //  ����ʱ��
	DWORD m_dwSendPacketCount; //  ���ͼ���
	DWORD m_dwRecvPacketCount; //  ���ܼ���
};

#endif //  TCP_SOCKET_H_