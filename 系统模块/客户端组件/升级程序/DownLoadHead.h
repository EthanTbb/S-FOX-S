#ifndef DOWN_LOAD_HEAD_FILE
#define DOWN_LOAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//平台文件

//系统文件
#include "Wininet.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//关闭原因
#define SHUT_REASON_INSIDE			0									//内部原因
#define SHUT_REASON_NORMAL			1									//正常关闭
#define SHUT_REASON_REMOTE			2									//远程关闭
#define SHUT_REASON_TIME_OUT		3									//网络超时
#define SHUT_REASON_EXCEPTION		4									//异常关闭

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//网络状态
#define SOCKET_STATUS_IDLE			0									//空闲状态
#define SOCKET_STATUS_WAIT			1									//等待状态
#define SOCKET_STATUS_CONNECT		2									//连接状态

//连接错误
#define CONNECT_SUCCESS				0									//连接成功
#define CONNECT_FAILURE				1									//连接失败
#define CONNECT_EXCEPTION			2									//参数异常
#define CONNECT_PROXY_FAILURE		3									//连接成功
#define CONNECT_PROXY_USER_INVALID	4									//用户无效

//代理类型
#define PROXY_NONE					0									//没有代理
#define PROXY_HTTP					1									//代理类型
#define PROXY_SOCKS4				2									//代理类型
#define PROXY_SOCKS5				3									//代理类型

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//代理描述
struct tagProxyDescribe
{
	BYTE							cbProxyType;						//代理类型
	TCHAR							szDescribe[32];						//描述字符
};

//代理信息
struct tagProxyServer
{
	WORD							wProxyPort;							//代理端口
	TCHAR							szUserName[32];						//代理用户
	TCHAR							szPassword[32];						//代理密码
	TCHAR							szProxyServer[32];					//代理地址
};

//错误状态
enum enDownLoadError
{
	DownLoadError_Server,			//服务错误
	DownLoadError_Connect,			//连接错误
};

//下载状态
enum enDownLoadStatus
{
	DownLoadStatus_Connect,			//连接状态
	DownLoadStatus_Service,			//服务状态
	DownLoadStatus_Conclude,		//完成状态
};

//////////////////////////////////////////////////////////////////////////////////

//请求信息
struct tagDownLoadInfo
{
	WORD							wHostPort;							//连接端口
	TCHAR							szUrlPath[MAX_PATH];				//请求路径
	TCHAR							szHostName[MAX_PATH];				//服务器名
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x09843e16,0x0f5f,0x4997,0xb9,0x23,0xdb,0xf1,0x00,0x05,0xbb,0x46};
#else
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x30c4a8e2,0xdb6b,0x4e5f,0xbb,0xdf,0x05,0xc9,0xc3,0x97,0x47,0xc3};
#endif

//下载回调
interface IWEBSocketSink : public IUnknownEx
{
	//状态接口
public:
	//连接消息
	virtual bool OnSocketLink(INT nErrorCode)=NULL;
	//关闭消息
	virtual bool OnSocketShut(BYTE cbShutReason)=NULL;

	//数据接口
public:
	//数据包流
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)=NULL;
	//数据包头
	virtual VOID OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDownLoadSink INTERFACE_VERSION(1,1)
	static const GUID IID_IDownLoadSink={0xb98886a0,0x3b99,0x401d,0x009b,0xf9,0x56,0x04,0xa8,0x59,0x7b,0x76};
#else
	#define VER_IDownLoadSink INTERFACE_VERSION(1,1)
	static const GUID IID_IDownLoadSink={0x419b6da4,0x13bb,0x4463,0x00a5,0x4d,0x4a,0xe2,0xcb,0x35,0xb9,0x81};
#endif

//下载回调
interface IDownLoadSink : public IUnknownEx
{
	//状态接口
public:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError)=NULL;
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus)=NULL;

	//序列接口
public:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize)=NULL;
	//下载信息
	virtual bool OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#endif
