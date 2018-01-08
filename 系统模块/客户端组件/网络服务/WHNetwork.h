
#ifndef WH_NETWORK_HEAD_FILE
#define WH_NETWORK_HEAD_FILE
#pragma once

#include "WHSocketHead.h"

// 网络辅助
class CWHNetwork : public IWHNetwork
{
	// 函数定义
public:
	// 构造函数
	CWHNetwork();
	// 析构函数
	virtual ~CWHNetwork();

	// 基础接口
public:
	// 释放对象
	virtual VOID Release() { delete this; }
	// 接口查询
	virtual VOID* QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能函数
public:
	//地址转换
	virtual DWORD ConversionAddr(LPCTSTR pszAddress);
	//获取地址
	virtual DWORD GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount);

	//代理接口
public:
	//枚举代理
	virtual bool EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe);
	//代理测试
	virtual BYTE ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer);
};

#endif //WH_NETWORK_HEAD_FILE
