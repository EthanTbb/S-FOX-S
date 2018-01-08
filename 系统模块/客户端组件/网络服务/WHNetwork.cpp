
#include "StdAfx.h"
#include "whnetwork.h"

//���캯��
CWHNetwork::CWHNetwork()
{
}

//��������
CWHNetwork::~CWHNetwork()
{
}

// �ӿڲ�ѯ
VOID* CWHNetwork::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWHNetwork,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWHNetwork,Guid,dwQueryVer);
	return NULL;
}

//��ַת��
DWORD CWHNetwork::ConversionAddr(LPCTSTR pszAddress)
{
	// ת����ַ
	DWORD dwServerIP = inet_addr(CT2A(pszAddress));

	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(CT2A(pszAddress));
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

//��ȡ��ַ
DWORD CWHNetwork::GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount)
{
	//�����ַ
	CHAR szHostName[64];
	gethostname(szHostName,CountArray(szHostName));
	HOSTENT * pHostent=gethostbyname(szHostName);
	LPIN_ADDR * lpAddrList=(LPIN_ADDR *)pHostent->h_addr_list;
	DWORD dwCurrentCount = 0;
	while (*lpAddrList!=NULL && dwCurrentCount < dwMaxCount)
	{
		dwAddrList[dwCurrentCount++] = (*lpAddrList)->S_un.S_addr;
		lpAddrList++;
	}

	return dwCurrentCount;
}

//ö�ٴ���
bool CWHNetwork::EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe)
{
	return false;
}

//�������
BYTE CWHNetwork::ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	return PROXY_NONE;
}

// ����������
DECLARE_CREATE_MODULE(WHNetwork)
