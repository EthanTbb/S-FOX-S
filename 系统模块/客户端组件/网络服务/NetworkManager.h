#ifndef NETWORK_MANAGER_HEAD_FILE
#define NETWORK_MANAGER_HEAD_FILE

#pragma once

//#include "NetworkServiceHead.h"
#include "WHSocketHead.h"

//////////////////////////////////////////////////////////////////////////

//�������
class CNetworkManager : public IWHNetwork
{
	//��������
public:
	//���캯��
	CNetworkManager();
	//��������
	virtual ~CNetworkManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܲ�ѯ
	//��ַת��
	virtual DWORD ConversionAddr(LPCTSTR pszAddress);
	//��ȡ��ַ
	virtual DWORD GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount);

public:
	//ö�ٴ���
	virtual bool  EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyTypeDescribe);
	//�������
	virtual BYTE  ProxyServerTesting(BYTE ProxyServerType, const tagProxyServer & ProxyServerInfo);
};

//////////////////////////////////////////////////////////////////////////

#endif