#ifndef DLG_UPDATE_STATUS_HEAD_FILE
#define DLG_UPDATE_STATUS_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDownLoadTaskSink;
class CUpdateCheckSink;
class CWndUpdateMessage;
class CUpdateStatusView;

////////////////////////////////////////////////////////////////////////////////////////////////
struct tagUpdateUICheck
{
	TCHAR								szCheckFile[MAX_PATH];		//����ļ�
	WORD								wCurrNum;					//��ǰ����
	WORD								wTotalNum;					//�ܹ�����
};

struct tagUpdateUIDownload
{
	TCHAR								szCheckFile[MAX_PATH];		//�����ļ�
	DWORD								dwDownSpeed;				//�����ٶ�
	DWORD								dwDownLoadSize;				//���ش�С
	DWORD								dwTotalFileSize;			//�ļ���С
};

////////////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ
struct tagDownloadItem
{
	TCHAR								szDownloadFile[MAX_PATH];		//�����ļ�
	TCHAR								szFileCheckSum[LEN_MD5];		//�ļ�У���
};

//���Ͷ���
typedef CWHArray<tagDownloadItem *>		CDownloadItemArray;				//��������

//�ӿڶ���
interface IUpdateStatusSink
{
	//�����¼�
public:
	//�������
	virtual VOID OnEventDownLoadTaskFinish()=NULL;
	//������
	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate)=NULL;	
};

//�ӿڶ���
interface IUpdateViewSink
{
	//�����¼�
public:	
	//������Ϣ
	virtual VOID OnEventDownLoadTaskUpdate(bool bUpdate)=NULL;
	//�����Ϣ
	virtual VOID OnEventDownLoadTaskFinish(bool bSuccessed)=NULL;

	//����¼�
public:
	//������
	virtual VOID OnEventUpdateCheckError()=NULL;	
	//����¼�
	virtual VOID OnEventUpdateCheckFinish()=NULL;
	//��ַ�¼�
	virtual VOID OnEventUpdateCheckUpdateInfo(tagUpdateInfo * UpdateInfo)=NULL;
	//����¼�
	virtual VOID OnEventUpdateCheckUpdateItem(tagUpdateItem * UpdateItem)=NULL;

public:
	//���¼��
	virtual VOID OnEventUpdateUICheck(tagUpdateUICheck * UIInfo)=NULL;
	//��������
	virtual VOID OnEventUpdateUIDownload(tagUpdateUIDownload * UIInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//���ػص�
class CDownLoadTaskSink : public CWnd,public IDownLoadSink
{	
	friend class  CUpdateStatusView;

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
	CDownLoad						m_DownLoad;							//�������
	CWndUpdateMessage *				m_pUpdateMessage;					//���´���

	//��������
public:
	//���캯��
	CDownLoadTaskSink();
	//��������
	virtual ~CDownLoadTaskSink();

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

	//��������
protected:
	//�ر�����
	bool CloseDownLoad();
	//���ñ���
	VOID ResetDownLoadSink();
	//�����ļ�
	bool LoadLocationFile(LPCTSTR pszLocation);
	//�����ļ�
	bool CreateDownLoadFile(LPCTSTR pszLocation);		
	//ִ������
	bool PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad=false);
};

//////////////////////////////////////////////////////////////////////////////////

//���»ص�
class CUpdateCheckSink : public CWnd, public IDownLoadSink
{
	//��Ա����
	friend class CUpdateStatusView;		

	//״̬����
protected:
	volatile bool					m_bCheckData;						//У������
	HANDLE 							m_hCheckThread;   					//У���߳�
	BYTE							m_cbDownLoadStatus;					//����״̬

	//�ļ���Ϣ
protected:
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//�ں���Ϣ
protected:	
	BYTE *							m_pFileData;						//�ļ�����
	CDownLoad						m_DownLoad;							//�������
	CWndUpdateMessage *				m_pUpdateMessage;					//���´���

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

	//��̬����
private:
	//У�麯��
	static VOID PerformCheckSum(LPVOID pThreadData);
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ
class CWndUpdateMessage : public CWnd
{
	//�ӿڱ���
protected:
	IUpdateViewSink *				m_pIUpdateViewSink;					//������ͼ			

	//��������
public:
	//���캯��
	CWndUpdateMessage();
	//��������
	virtual ~CWndUpdateMessage();

	//���ܺ���
public:
	//���ýӿ�
	VOID SetUpdateViewSink(IUpdateViewSink * pIUpdateViewSink);

	//�Զ���Ϣ
protected:
	//�����Ϣ
	LRESULT OnMessageDownLoadTaskFinish(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageDownLoadTaskUpdate(WPARAM wParam,LPARAM lParam);
	//�����¼�
	LRESULT OnMessageUpdateCheckError(WPARAM wParam, LPARAM lParam);	
	//����¼�
	LRESULT OnMessageUpdateCheckFinish(WPARAM wParam, LPARAM lParam);
	//��ַ�¼�
	LRESULT OnMessageUpdateCheckUpdateInfo(WPARAM wParam, LPARAM lParam);
	//����¼�
	LRESULT OnMessageUpdateCheckUpdateItem(WPARAM wParam, LPARAM lParam);	

	//���¼��
	LRESULT OnMessageUpdateUICheck(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnMessageUpdateUIDownload(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////////////

//��ͼ����
class CUpdateStatusView : public CFGuiDialog,public IUpdateViewSink
{
	
protected:
	//����Ŀ¼
	TCHAR                           m_szDownDirectory[64];				//����Ŀ¼
	//״̬����
protected:
	UINT							m_uImageIndex;						//λͼλ��	

	//���ر���
protected:
	DWORD							m_dwTickCount;						//ʱ�����
	DWORD                           m_dwTotalTaskCount;                 //��������
	DWORD                           m_dwTaskFinished;                   //�������
	BYTE							m_cbCurrRetryCount;					//���Դ���

	//��������
protected:
	TCHAR                           m_szFileName[64];					//�ļ����� 	
	TCHAR							m_szDescribe[128];					//״̬����
	TCHAR							m_szUpdateFile[MAX_PATH];			//�����ļ�

	//������Ϣ
protected:
	TCHAR                           m_szDownLoadAddress[MAX_PATH];      //���ص�ַ 
	CDownloadItemArray              m_DownLoadFileArray;                //�����б�

	//ָ�����
protected:
	CUpdateCheckSink *              m_pUpdateCheckSink;                 //�ص�����
	CDownLoadTaskSink *             m_pDownLoadTaskSink;                //�ص�����

	//�ؼ�����
protected:
	CWndUpdateMessage 				m_UpdateMessage;					//��Ϣ����	

	//�ӿڱ���
protected:
	IUpdateStatusSink *				m_pIUpdateStatusSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CUpdateStatusView();
	//��������
	virtual ~CUpdateStatusView();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();

	//�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile();



	//�����¼�
public:	
	//������Ϣ
	virtual VOID OnEventDownLoadTaskUpdate(bool bUpdate);
	//�����Ϣ
	virtual VOID OnEventDownLoadTaskFinish(bool bSuccessed);

	//����¼�
public:
	//������
	virtual VOID OnEventUpdateCheckError();	
	//����¼�
	virtual VOID OnEventUpdateCheckFinish();
	//���µ�ַ
	virtual VOID OnEventUpdateCheckUpdateInfo(tagUpdateInfo * UpdateInfo);
	//��������
	virtual VOID OnEventUpdateCheckUpdateItem(tagUpdateItem * UpdateItem);
public:
	//���¼��
	virtual VOID OnEventUpdateUICheck(tagUpdateUICheck * UIInfo);
	//��������
	virtual VOID OnEventUpdateUIDownload(tagUpdateUIDownload * UIInfo);

	//���ܺ���
public:
	//��������
	VOID EndUpdateStatus();
	//��ʼ����
	VOID InitUpdateStatus(IUpdateStatusSink * pIUpdateStatusSink, LPCTSTR pszProgressName);

	//����Ŀ¼
	VOID SetDownDirectory(LPCTSTR pszProgressName);

	//��������
protected:	
	//�������
	VOID StartUpdateCheck();
	//��������
	VOID StartDownLoadTask();		
	//�л�����
	VOID SwitchDownLoadTask(LPCTSTR pszFileName);

	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	//DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////
#endif