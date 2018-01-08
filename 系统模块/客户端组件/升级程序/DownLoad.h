#ifndef UPDATE_DOWN_LOAD_HEAD_FILE
#define UPDATE_DOWN_LOAD_HEAD_FILE

#pragma once

#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//Э�鷭��
class CUHeadTranslate
{
	//��������
protected:
	WORD							m_wDataLength;						//���ݳ���
	BYTE *							m_pcbHeadData;						//����ָ��

	//������Ϣ
protected:
	DWORD							m_dwTotalFileSize;					//�ļ���С
	TCHAR							m_szFileEntityTag[32];				//�ļ���ʶ

	//��������
public:
	//���캯��
	CUHeadTranslate();
	//��������
	virtual ~CUHeadTranslate();

	//������Ϣ
public:
	//�ļ���С
	DWORD GetTotalFileSize() { return m_dwTotalFileSize; }
	//�ļ���ʶ
	LPCTSTR GetFileEntityTag() { return m_szFileEntityTag; }
	
	//���ܺ���
public:
	//�������
	VOID CleanData();
	//���ò���
	VOID SetHeadData(BYTE * pcbHeadData, WORD wDataLength);
	//��ȡ����
	WORD GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class CUWEBSocket : public CWnd
{
	//״̬����
protected:
	WORD							m_wRequestID;						//�����ʶ
	bool							m_bHeadValid;						//��Ч��־
	BYTE							m_cbSocketStatus;					//����״̬

	//���ݰ�ͷ
protected:
	WORD							m_wHeadSize;						//��ͷ����
	CHAR							m_szHeadData[1024];					//���ݰ�ͷ

	//�������
protected:
	bool							m_bNeedBuffer;						//����״̬
	DWORD							m_dwBufferData;						//��������
	DWORD							m_dwBufferSize;						//�����С
	LPBYTE							m_pcbDataBuffer;					//��������

	//�ں˱���
protected:
	SOCKET							m_hSocket;							//���Ӿ��
	IWEBSocketSink *				m_pIWEBSocketSink;					//�ص��ӿ�

	//������Ϣ
protected:
	BYTE							m_cbProxyType;						//��������
	tagProxyServer					m_ProxyServer;						//������Ϣ

	//��������
public:
	//���캯��
	CUWEBSocket();
	//��������
	virtual ~CUWEBSocket();

	//���ú���
public:
	//���ýӿ�
	bool SetWEBSocketSink(IUnknownEx * pIUnknownEx);
	//���ô���
	bool SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer);

	//�����ӿ�
public:
	//�ر�����
	VOID CloseSocket(BYTE cbShutReason);
	//���Ӳ���
	BYTE ConnectServer(LPCTSTR pszServer, WORD wPort);
	//��������
	WORD SendRequestData(VOID * pData, WORD wDataSize);

	//��������
protected:
	//���Ӵ���
	BYTE ConnectProxyServer();
	//��������
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);

	//������
protected:
	//�����ȡ
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//���緢��
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//����ر�
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
protected:
	//������Ϣ
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��վ����
class CUDownLoad : public IWEBSocketSink
{
	//�ļ���Ϣ
protected:
	DWORD							m_dwTotalSize;						//�ļ���С
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwDownLoadPos;					//����λ��

	//״̬����
protected:
	tagDownLoadInfo					m_DownLoadInfo;						//������Ϣ
	enDownLoadStatus				m_DownLoadStatus;					//����״̬

	//�������
protected:
	CUWEBSocket						m_WEBSocket;						//��������
	CUHeadTranslate					m_HeadTranslate;					//��ͷ����

	//�ӿڱ���
protected:
	IDownLoadSink *					m_pIDownLoadSink;					//״̬�ص�

	//��������
public:
	//���캯��
	CUDownLoad();
	//��������
	virtual ~CUDownLoad();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//״̬�ӿ�
protected:
	//������Ϣ
	virtual bool OnSocketLink(INT nErrorCode);
	//�ر���Ϣ
	virtual bool OnSocketShut(BYTE cbShutReason);

	//���ݽӿ�
protected:
	//���ݰ���
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize);
	//���ݰ�ͷ
	virtual VOID OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode);

	//��Ϣ����
public:
	//�ļ���С
	DWORD GetTotalSize() { return m_dwTotalSize; }
	//���ش�С
	DWORD GetDownLoadSize() { return m_dwDownLoadSize; }
	//����״̬
	enDownLoadStatus GetDownLoadStatus() { return m_DownLoadStatus; }

	//���ܺ���
public:
	//�ر�����
	bool CloseDownLoad();
	//ִ������
	bool PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx,DWORD dwDownLoadPos);

	//��������
protected:
	//��������
	bool ConnectServer(LPCTSTR pszUrl);
	//��������
	bool SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath);
	//��ȡ��Ϣ
	INTERNET_SCHEME GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif