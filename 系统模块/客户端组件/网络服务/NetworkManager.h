#ifndef NETWORK_MANAGER_HEAD_FILE
#define NETWORK_MANAGER_HEAD_FILE

#pragma once

//#include "NetworkServiceHead.h"
#include "WHSocketHead.h"

//////////////////////////////////////////////////////////////////////////

//网络管理
class CNetworkManager : public IWHNetwork
{
	//函数定义
public:
	//构造函数
	CNetworkManager();
	//析构函数
	virtual ~CNetworkManager();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { return; }
	//接口查询
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//性能查询
	//地址转换
	virtual DWORD ConversionAddr(LPCTSTR pszAddress);
	//获取地址
	virtual DWORD GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount);

public:
	//枚举代理
	virtual bool  EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyTypeDescribe);
	//代理测试
	virtual BYTE  ProxyServerTesting(BYTE ProxyServerType, const tagProxyServer & ProxyServerInfo);
};

//////////////////////////////////////////////////////////////////////////

#endif