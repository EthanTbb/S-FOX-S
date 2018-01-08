
#ifndef WH_NETWORK_HEAD_FILE
#define WH_NETWORK_HEAD_FILE
#pragma once

#include "WHSocketHead.h"

// ���縨��
class CWHNetwork : public IWHNetwork
{
	// ��������
public:
	// ���캯��
	CWHNetwork();
	// ��������
	virtual ~CWHNetwork();

	// �����ӿ�
public:
	// �ͷŶ���
	virtual VOID Release() { delete this; }
	// �ӿڲ�ѯ
	virtual VOID* QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܺ���
public:
	//��ַת��
	virtual DWORD ConversionAddr(LPCTSTR pszAddress);
	//��ȡ��ַ
	virtual DWORD GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount);

	//����ӿ�
public:
	//ö�ٴ���
	virtual bool EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe);
	//�������
	virtual BYTE ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer);
};

#endif //WH_NETWORK_HEAD_FILE
