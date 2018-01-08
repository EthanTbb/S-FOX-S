
#include "StdAfx.h"
#include "whnetwork.h"

//构造函数
CWHNetwork::CWHNetwork()
{
}

//析构函数
CWHNetwork::~CWHNetwork()
{
}

// 接口查询
VOID* CWHNetwork::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWHNetwork,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWHNetwork,Guid,dwQueryVer);
	return NULL;
}

//地址转换
DWORD CWHNetwork::ConversionAddr(LPCTSTR pszAddress)
{
	// 转化地址
	DWORD dwServerIP = inet_addr(CT2A(pszAddress));

	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(CT2A(pszAddress));
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

//获取地址
DWORD CWHNetwork::GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount)
{
	//服务地址
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

//枚举代理
bool CWHNetwork::EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe)
{
	return false;
}

//代理测试
BYTE CWHNetwork::ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	return PROXY_NONE;
}

// 建立对象函数
DECLARE_CREATE_MODULE(WHNetwork)
