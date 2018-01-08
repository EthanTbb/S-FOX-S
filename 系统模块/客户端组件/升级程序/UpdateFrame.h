#ifndef DLG_DOWN_LOAD_HEAD_FILE
#define DLG_DOWN_LOAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "DownLoad.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//����״̬
#define DOWN_LOAD_IDLE					0								//����״̬
#define DOWN_LOAD_LINK					1								//����״̬
#define DOWN_LOAD_READ					2								//��ȡ״̬
#define DOWN_LOAD_SETUP					3								//��װ״̬
#define DOWN_LOAD_ERROR					4								//����״̬

//��Ϣ����
#define WM_ITEM_DOWNLOAD_Finish			(WM_USER+100)					//״̬����
#define WM_ITEM_DOWNLOAD_UPDATE			(WM_USER+101)					//״̬����

////////////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ
struct tagDownloadItem
{
	TCHAR								szDownloadFile[MAX_PATH];		//�����ļ�
	TCHAR								szFileCheckSum[LEN_MD5];		//�ļ�У���
};

//���ؽ��
struct tagDownLoadResult
{
	bool								bDownLoadSuccess;				//���سɹ�
	TCHAR								szSourceFilePath[MAX_PATH];		//ԭʼ·��
	TCHAR								szTargetFilePath[MAX_PATH];		//Ŀ��·��
};

///////////////////////////////////////////////////////////////////////////////////////////////
//���Ͷ���
typedef CWHArray<tagDownloadItem *>		CDownloadItemArray;				//��������

///////////////////////////////////////////////////////////////////////////////////////////////


//���ػص�
class CUDownLoadSink :public CWnd,public IDownLoadSink
{	
	friend class  CDownLoadManager;
	friend class  CGameUpdateFrame;

	//״̬����
protected:
	BYTE							m_cbDownLoadStatus;					//����״̬

	//�����ٶ�
protected:
	DWORD							m_dwDownSpeed;						//�����ٶ�
	DWORD							m_dwLastCalcSize;					//�ϴδ�С
	DWORD							m_dwLastCalcTime;					//�ϴ�ʱ��

	//�ļ���Ϣ
protected:
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//��������
protected:
	TCHAR							m_szTaskName[MAX_PATH];				//��������

	//�ں���Ϣ
protected:
	CFile							m_DownFile;							//�����ļ�
	CUDownLoad					m_DownLoad;							//�������
	CDownLoadManager *				m_pDownLoadManager;  				//����ӿ�

	//��������
public:
	//���캯��
	CUDownLoadSink();
	//��������
	virtual ~CUDownLoadSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

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
protected:
	//�����ļ�
	bool CreateDownFile(LPCTSTR pszLocation);
	//�����ļ�
	bool LoadDownFile(LPCTSTR pszLocation);
	//�ر�����
	bool CloseDownLoad();
	//ִ������
	bool PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad=false);
	//ִ�а�װ
	VOID PerformInstall();
	//���ñ���
	VOID ResetDownLoadSink();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////////////////

//���ع���
class CDownLoadManager : public CWnd
{
	friend class  CGameUpdateFrame;

	//��������
protected:
	DWORD							m_dwTickCount;						//ʱ����
	DWORD                           m_dwTotalTaskCount;                 //��������
	DWORD                           m_dwTaskFinished;                   //�������
	BYTE							m_cbCurrRetryCount;					//���Դ���

	//������Ϣ
protected:
	TCHAR                           m_szDownLoadAddress[MAX_PATH];      //���ص�ַ 
	CDownloadItemArray              m_DownLoadFileArray;                //�����б�
	CStringArray					m_SourcePathArray;					//·���б�
	CStringArray					m_TargetPathArray;					//·���б�	

	//���ı���
protected:
	CUDownLoadSink *                 m_pDownLoadSink;                    //�ص��ӿ�
	CGameUpdateFrame *              m_pGameUpdateFrame;                 //��ͼ�ӿ�  

	//��������
public:
	//���캯��
	CDownLoadManager();
	//��������
	virtual ~CDownLoadManager();

	//�ڲ�����
protected:
	//��ȡ��Ϣ
	bool ReadDownLoadInfo();
	//ִ������
	bool StartDownLoadTask();
	//�ƶ��ļ�
	VOID PerformMoveFiles();

	//�Զ���Ϣ
protected:
	//״̬��Ϣ
	LRESULT OnItemDownLoadFinishMessage(WPARAM wParam, LPARAM lParam);
	//״̬��Ϣ
	LRESULT OnItemDownLoadUpdateMessage(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////////////////

//�������
class CGameUpdateFrame : public CFrameWnd
{
	//�ַ�����
protected:
	TCHAR                           m_pszFileName[128];                  //�ļ����� 	

	//�ؼ�����
protected:	
	CSkinButton						m_btClose;							//�رհ�ť
	CSkinButton						m_btSelfUpdate;						//�ֶ�����
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��	

	//ָ�����
protected:
	CDownLoadManager *				m_pDownLoadManager;	     			//���ع���

	//��������
public:
	//���캯��
	CGameUpdateFrame();
	//��������
	virtual ~CGameUpdateFrame();

	//�¼�����
public:
	//���ؽ���
	VOID OnEventDownLoadEnd();
	//�л�����
	VOID OnEventSwitchDownLoadTask(LPCTSTR pszFileName);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���غ���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//��������
	VOID StartDownLoadTask();

	//��������
private:
	//������Դ
	VOID RectifyWindow(INT nWidth, INT nHeight);

	//��ť��Ϣ
protected:
	//�رհ�ť
	VOID OnBnClickedClose();

	//�ֶ���ť
	VOID OnBnClickedSelfUpdate(); 

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();
	//���ٷǿͻ�
	VOID OnNcDestroy();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);	

	DECLARE_MESSAGE_MAP()	
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif