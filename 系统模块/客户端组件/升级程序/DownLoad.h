#ifndef UPDATE_DOWN_LOAD_HEAD_FILE
#define UPDATE_DOWN_LOAD_HEAD_FILE

#pragma once

#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//协议翻译
class CUHeadTranslate
{
	//变量定义
protected:
	WORD							m_wDataLength;						//数据长度
	BYTE *							m_pcbHeadData;						//数据指针

	//数据信息
protected:
	DWORD							m_dwTotalFileSize;					//文件大小
	TCHAR							m_szFileEntityTag[32];				//文件标识

	//函数定义
public:
	//构造函数
	CUHeadTranslate();
	//析构函数
	virtual ~CUHeadTranslate();

	//数据信息
public:
	//文件大小
	DWORD GetTotalFileSize() { return m_dwTotalFileSize; }
	//文件标识
	LPCTSTR GetFileEntityTag() { return m_szFileEntityTag; }
	
	//功能函数
public:
	//清理参数
	VOID CleanData();
	//设置参数
	VOID SetHeadData(BYTE * pcbHeadData, WORD wDataLength);
	//获取数据
	WORD GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount);
};

//////////////////////////////////////////////////////////////////////////////////

//下载连接
class CUWEBSocket : public CWnd
{
	//状态变量
protected:
	WORD							m_wRequestID;						//请求标识
	bool							m_bHeadValid;						//有效标志
	BYTE							m_cbSocketStatus;					//网络状态

	//数据包头
protected:
	WORD							m_wHeadSize;						//包头长度
	CHAR							m_szHeadData[1024];					//数据包头

	//缓冲变量
protected:
	bool							m_bNeedBuffer;						//缓冲状态
	DWORD							m_dwBufferData;						//缓冲数据
	DWORD							m_dwBufferSize;						//缓冲大小
	LPBYTE							m_pcbDataBuffer;					//缓冲数据

	//内核变量
protected:
	SOCKET							m_hSocket;							//连接句柄
	IWEBSocketSink *				m_pIWEBSocketSink;					//回调接口

	//代理信息
protected:
	BYTE							m_cbProxyType;						//代理类型
	tagProxyServer					m_ProxyServer;						//代理信息

	//函数定义
public:
	//构造函数
	CUWEBSocket();
	//析构函数
	virtual ~CUWEBSocket();

	//配置函数
public:
	//设置接口
	bool SetWEBSocketSink(IUnknownEx * pIUnknownEx);
	//设置代理
	bool SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer);

	//操作接口
public:
	//关闭连接
	VOID CloseSocket(BYTE cbShutReason);
	//连接操作
	BYTE ConnectServer(LPCTSTR pszServer, WORD wPort);
	//发送请求
	WORD SendRequestData(VOID * pData, WORD wDataSize);

	//辅助函数
protected:
	//连接代理
	BYTE ConnectProxyServer();
	//缓冲数据
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);

	//处理函数
protected:
	//网络读取
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//网络发送
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//网络关闭
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//网络连接
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	//消息映射
protected:
	//网络消息
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//网站下载
class CUDownLoad : public IWEBSocketSink
{
	//文件信息
protected:
	DWORD							m_dwTotalSize;						//文件大小
	DWORD							m_dwDownLoadSize;					//下载大小
	DWORD							m_dwDownLoadPos;					//下载位置

	//状态变量
protected:
	tagDownLoadInfo					m_DownLoadInfo;						//下载信息
	enDownLoadStatus				m_DownLoadStatus;					//下载状态

	//组件变量
protected:
	CUWEBSocket						m_WEBSocket;						//网络连接
	CUHeadTranslate					m_HeadTranslate;					//包头解释

	//接口变量
protected:
	IDownLoadSink *					m_pIDownLoadSink;					//状态回调

	//函数定义
public:
	//构造函数
	CUDownLoad();
	//析构函数
	virtual ~CUDownLoad();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//状态接口
protected:
	//连接消息
	virtual bool OnSocketLink(INT nErrorCode);
	//关闭消息
	virtual bool OnSocketShut(BYTE cbShutReason);

	//数据接口
protected:
	//数据包流
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize);
	//数据包头
	virtual VOID OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode);

	//信息函数
public:
	//文件大小
	DWORD GetTotalSize() { return m_dwTotalSize; }
	//下载大小
	DWORD GetDownLoadSize() { return m_dwDownLoadSize; }
	//下载状态
	enDownLoadStatus GetDownLoadStatus() { return m_DownLoadStatus; }

	//功能函数
public:
	//关闭下载
	bool CloseDownLoad();
	//执行下载
	bool PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx,DWORD dwDownLoadPos);

	//辅助函数
protected:
	//请求连接
	bool ConnectServer(LPCTSTR pszUrl);
	//发送请求
	bool SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath);
	//获取信息
	INTERNET_SCHEME GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif