
#ifndef UDP_SOCKET_HEAD_FILE
#define UDP_SOCKET_HEAD_FILE
#pragma once

#include "WHSocketHead.h"

//UDP 网络连接(没有实现)
class CUDPSocket : public CWnd, public IUDPSocket
{
	// 函数定义
public:
	// 构造函数
	CUDPSocket();
	// 析构函数
	virtual ~CUDPSocket();

	// 基础接口
public:
	// 释放对象
	virtual VOID Release() { delete this; }
	// 接口查询
	virtual VOID* QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//设置标识
	virtual VOID SetSocketID(WORD wSocketID);
	//设置接口
	virtual bool SetUDPSocketSink(IUnknownEx * pIUnknownEx);

	//信息接口
public:
	//网络状态
	virtual BYTE GetSocketStatus();
	//发送间隔
	virtual DWORD GetLastSendTick();
	//接收间隔
	virtual DWORD GetLastRecvTick();

	//操作接口
public:
	//关闭连接
	virtual VOID CloseSocket();
	//连接操作
	virtual BYTE Connect(DWORD dwServerIP, WORD wPort);
	//连接操作
	virtual BYTE Connect(LPCTSTR pszServerIP, WORD wPort);
	//发送函数
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

#endif //UDP_SOCKET_HEAD_FILE
