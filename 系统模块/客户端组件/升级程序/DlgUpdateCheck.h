#ifndef DLG_UPDATE_CHECK_HEAD_FILE
#define DLG_UPDATE_CHECK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "DownLoadHead.h"
#include "DownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//����״̬
#define DOWN_LOAD_IDLE					0								//����״̬
#define DOWN_LOAD_LINK					1								//����״̬
#define DOWN_LOAD_READ					2								//��ȡ״̬
#define DOWN_LOAD_SETUP					3								//��װ״̬
#define DOWN_LOAD_ERROR					4								//����״̬
#define DOWN_LOAD_FINISH				5								//��װ���

//��Ϣ����
#define WM_DOWNLOAD_FINISH			   (WM_USER+100)					//�������

//////////////////////////////////////////////////////////////////////////////////

//���ػص�
class CUpdateCheckSink : public CWnd, public IDownLoadSink
{
	//��Ա����
	friend class CDlgUpdateCheck;

	//״̬����
protected:
	BYTE							m_cbDownLoadStatus;					//����״̬
	INT								m_nUpdateCount;						//������Ŀ

	//�ļ���Ϣ
protected:
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//�ں���Ϣ
protected:	
	BYTE *							m_pFileData;						//�ļ�����
	CUDownLoad						m_DownLoad;							//�������
	CDlgUpdateCheck *				m_pDlgUpdateCheck;					//���ش���

	//��������
public:
	//���캯��
	CUpdateCheckSink();
	//��������
	virtual ~CUpdateCheckSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܺ���
public:
	//�ر�����
	bool CloseDownLoad();
	//ִ������
	bool PerformDownLoad(LPCTSTR pszDownLoadFile);

	//״̬�ӿ�
public:
	//�����쳣
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//����״̬
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//���нӿ�
public:
	//��������
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//������Ϣ
	virtual bool OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//�ڲ�����
private:
	//ִ��У��
	bool PerformCheckSum();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//���¼��
class CDlgUpdateCheck : public CDialog
{
	//״̬����
protected:
	UINT							m_uImageIndex;						//λͼλ��
	CString							m_strDescribe;						//������Ϣ
	CString                         m_pszDownLoadFile;					//����·��
	BOOL                            m_bCheckFinish;                     //������
	BOOL                            m_bNeedUpdate;                      //������

	//�ؼ�����
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//���ݱ���
protected:
	CUpdateCheckSink *              m_pUpdateCheckSink;                 //�ص�����

	//��������
public:
	//���캯��
	CDlgUpdateCheck();
	//��������
	virtual ~CDlgUpdateCheck();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();

	//���ܺ���
public:
	//������Ŀ
	INT GetUpdateCount();
	//��������
	VOID DownLoadConfig(LPCTSTR pszConfigName);
	//������Ϣ
	VOID SetWindowDescribe(LPCTSTR pszDescribe);
	//����¼�
	VOID OnEventCheckFinish(BOOL bNeedUpdate);
	//�����¼�
	VOID OnEventDownLoadError(int nResult);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);

	//�Զ���Ϣ
protected:
	//״̬��Ϣ
	LRESULT OnDownLoadConfigFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif