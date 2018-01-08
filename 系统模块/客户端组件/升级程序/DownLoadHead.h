#ifndef DOWN_LOAD_HEAD_FILE
#define DOWN_LOAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//ƽ̨�ļ�

//ϵͳ�ļ�
#include "Wininet.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//////////////////////////////////////////////////////////////////////////////////
//��������

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣
#define CONNECT_PROXY_FAILURE		3									//���ӳɹ�
#define CONNECT_PROXY_USER_INVALID	4									//�û���Ч

//��������
#define PROXY_NONE					0									//û�д���
#define PROXY_HTTP					1									//��������
#define PROXY_SOCKS4				2									//��������
#define PROXY_SOCKS5				3									//��������

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagProxyDescribe
{
	BYTE							cbProxyType;						//��������
	TCHAR							szDescribe[32];						//�����ַ�
};

//������Ϣ
struct tagProxyServer
{
	WORD							wProxyPort;							//����˿�
	TCHAR							szUserName[32];						//�����û�
	TCHAR							szPassword[32];						//��������
	TCHAR							szProxyServer[32];					//�����ַ
};

//����״̬
enum enDownLoadError
{
	DownLoadError_Server,			//�������
	DownLoadError_Connect,			//���Ӵ���
};

//����״̬
enum enDownLoadStatus
{
	DownLoadStatus_Connect,			//����״̬
	DownLoadStatus_Service,			//����״̬
	DownLoadStatus_Conclude,		//���״̬
};

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagDownLoadInfo
{
	WORD							wHostPort;							//���Ӷ˿�
	TCHAR							szUrlPath[MAX_PATH];				//����·��
	TCHAR							szHostName[MAX_PATH];				//��������
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x09843e16,0x0f5f,0x4997,0xb9,0x23,0xdb,0xf1,0x00,0x05,0xbb,0x46};
#else
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x30c4a8e2,0xdb6b,0x4e5f,0xbb,0xdf,0x05,0xc9,0xc3,0x97,0x47,0xc3};
#endif

//���ػص�
interface IWEBSocketSink : public IUnknownEx
{
	//״̬�ӿ�
public:
	//������Ϣ
	virtual bool OnSocketLink(INT nErrorCode)=NULL;
	//�ر���Ϣ
	virtual bool OnSocketShut(BYTE cbShutReason)=NULL;

	//���ݽӿ�
public:
	//���ݰ���
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)=NULL;
	//���ݰ�ͷ
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

//���ػص�
interface IDownLoadSink : public IUnknownEx
{
	//״̬�ӿ�
public:
	//�����쳣
	virtual bool OnDownLoadError(enDownLoadError DownLoadError)=NULL;
	//����״̬
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus)=NULL;

	//���нӿ�
public:
	//��������
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize)=NULL;
	//������Ϣ
	virtual bool OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#endif
