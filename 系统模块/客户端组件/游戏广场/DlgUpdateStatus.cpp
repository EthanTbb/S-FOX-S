#include "StdAfx.h"
#include "Resource.h"
#include "DlgUpdateStatus.h"
//#include "PlatformFrame.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//ʱ�Ӷ���
#define IDI_UPDATE_STATUS			100									//����ʱ��
#define IDI_UPDATE_DELAYEND			101									//��ʱ����

//ʱ�䶨��
#define TIMER_UPDATE_STATUS			60									//����ʱ��
#define TIMER_UPDATE_DELAYEND		50									//��ʱʱ��

//��������
#define MAX_RETRY_COUNT				3									//���Դ���

//����״̬
#define DOWN_LOAD_IDLE				0									//����״̬
#define DOWN_LOAD_LINK				1									//����״̬
#define DOWN_LOAD_READ				2									//��ȡ״̬
#define DOWN_LOAD_ERROR				3									//����״̬
#define DOWN_LOAD_FINISH			4									//�������

//��������
#define UPDATE_TIMESPACE			30									//���¼��	
#define ANIMAL_FRAME_COUNT			12									//����֡��

//������Ϣ
#define WM_DOWNLOAD_TASK_FINISH		(WM_USER+100)						//�������
#define WM_DOWNLOAD_TASK_UPDATE		(WM_USER+101)						//���ظ���

//�����Ϣ
#define WM_UPDATECHECK_ERROR		(WM_USER+102)						//������
#define WM_UPDATECHECK_FINISH		(WM_USER+103)						//������
#define WM_UPDATECHECK_UPDATEINFO	(WM_USER+104)						//������Ϣ
#define WM_UPDATECHECK_UPDATEITEM	(WM_USER+105)						//��������

//UI��Ϣ
#define WM_UI_UPDATECHECK		(WM_USER+111)							//���¼��
#define WM_UI_UPDATEDOWNLOAD	(WM_USER+112)							//��������
//////////////////////////////////////////////////////////////////////////////////
//��Ϣӳ��

BEGIN_MESSAGE_MAP(CWndUpdateMessage, CWnd)

	//�Զ���Ϣ
	ON_MESSAGE(WM_DOWNLOAD_TASK_FINISH,OnMessageDownLoadTaskFinish)
	ON_MESSAGE(WM_DOWNLOAD_TASK_UPDATE,OnMessageDownLoadTaskUpdate)

	//�Զ���Ϣ
	ON_MESSAGE(WM_UPDATECHECK_ERROR,OnMessageUpdateCheckError)
	ON_MESSAGE(WM_UPDATECHECK_FINISH,OnMessageUpdateCheckFinish)
	ON_MESSAGE(WM_UPDATECHECK_UPDATEINFO,OnMessageUpdateCheckUpdateInfo)
	ON_MESSAGE(WM_UPDATECHECK_UPDATEITEM,OnMessageUpdateCheckUpdateItem)

	//�Զ���Ϣ
	ON_MESSAGE(WM_UI_UPDATECHECK,OnMessageUpdateUICheck)
	ON_MESSAGE(WM_UI_UPDATEDOWNLOAD,OnMessageUpdateUIDownload)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadTaskSink::CDownLoadTaskSink()
{
	//�����ٶ�
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//�ļ���Ϣ
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;

	//״̬����
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//��������
	ZeroMemory(m_szTaskName,sizeof(m_szTaskName));

	//�ں���Ϣ
	m_pUpdateMessage=NULL;

	return;
}

//��������
CDownLoadTaskSink::~CDownLoadTaskSink()
{
}

//�ӿڲ�ѯ
VOID * CDownLoadTaskSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);

	return NULL;
}

//�ر�����
bool CDownLoadTaskSink::CloseDownLoad()
{
	//�رմ���
	DestroyWindow();

	//�ر�����
	m_DownLoad.CloseDownLoad();

	//����״̬
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//�ر��ļ�
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();

	return true;
}

//ִ������
bool CDownLoadTaskSink::PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad)
{
	//Ч��״̬
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//�����ٶ�
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=GetTickCount();

	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_LINK;
	lstrcpyn(m_szTaskName,pszFileName,CountArray(m_szTaskName));

	//�����ַ
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s/%s"),pszNetAddress,pszFileName);

	//�ϵ�����
	if(bBreakpointDownLoad && LoadLocationFile(szDownLoadUrl)) 
	{
		//��������
		DWORD dwFileSize = (DWORD)m_DownFile.GetLength();		

		//ִ������
		m_DownLoad.PerformDownLoad(szDownLoadUrl,dwFileSize,QUERY_ME_INTERFACE(IUnknownEx));
	}
	else
	{
		//ִ������
		m_DownLoad.PerformDownLoad(szDownLoadUrl,0,QUERY_ME_INTERFACE(IUnknownEx));
	}

	return true;
}

//���ñ���
VOID CDownLoadTaskSink::ResetDownLoadSink()
{
	//�����ٶ�
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//�ļ���Ϣ
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;

	//״̬����
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//��������
	ZeroMemory(m_szTaskName,sizeof(m_szTaskName));		
}

//�����쳣
bool CDownLoadTaskSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//������Ϣ
	if(m_pUpdateMessage!=NULL ) 
	{
		if(m_DownFile.m_hFile!=INVALID_HANDLE_VALUE && m_DownFile.GetLength()>0)
		{
			//�ر��ļ�
			m_DownFile.Close();
		}

		//��Ϣ֪ͨ
		m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_FINISH,FALSE);
	}

	return true;
}

//����״̬
bool CDownLoadTaskSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//����״̬
	switch (DownLoadStatus)
	{
	case DownLoadStatus_Service:		//����״̬
		{
			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_READ;

			break;
		}
	case DownLoadStatus_Conclude:		//���״̬
		{
			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//�ر��ļ�
			m_DownFile.Close();

			//������Ϣ
			if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_FINISH,TRUE);
			
			break;
		}
	}

	return true;
}

//��������
bool CDownLoadTaskSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//д���ļ�
	m_dwDownLoadSize+=wStreamSize;
	m_DownFile.Write(pcbMailData,wStreamSize);

	//�����ٶ�
	if (GetTickCount()>=(m_dwLastCalcTime+2000L))
	{
		//�����ٶ�
		m_dwDownSpeed=(m_dwDownLoadSize-m_dwLastCalcSize)/(GetTickCount()-m_dwLastCalcTime);

		//���ñ���
		m_dwLastCalcTime=GetTickCount();
		m_dwLastCalcSize=m_dwDownLoadSize;
	}

	//������Ϣ
	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_UPDATE,m_dwDownLoadSize==m_dwTotalFileSize);
	
	tagUpdateUIDownload UIInfo;
	UIInfo.dwTotalFileSize = m_dwTotalFileSize;
	UIInfo.dwDownSpeed = m_dwDownSpeed;
	UIInfo.dwDownLoadSize = m_dwDownLoadSize;
	ZeroMemory(UIInfo.szCheckFile,sizeof(UIInfo.szCheckFile));
	lstrcpyn(UIInfo.szCheckFile,m_szTaskName,CountArray(UIInfo.szCheckFile));

	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_UI_UPDATEDOWNLOAD,(WPARAM)&UIInfo);

	return true;
}

//������Ϣ
bool CDownLoadTaskSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//���ñ���	
	m_dwTotalFileSize+=dwTotalFileSize;

	//�ϵ�����
	if(bPartialConent==true)
	{
		//���ñ���
		m_dwTotalFileSize += (DWORD)m_DownFile.GetLength();
		m_dwDownLoadSize += (DWORD)m_DownFile.GetLength();
	}
	else		
	{
		//�ͷ��ļ�
		if(m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();
		if(CreateDownLoadFile(pszLocation)==false)
		{
			//�ر�����
			CloseDownLoad();

			return false;
		}
	}

	//������Ϣ
	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_DOWNLOAD_TASK_UPDATE);

	return true;
}

//�����ļ�
bool CDownLoadTaskSink::LoadLocationFile(LPCTSTR pszLocation)
{
	//Ч��״̬
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//Ŀ��Ŀ¼
	TCHAR szDownLoadPath[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadPath,CountArray(szDownLoadPath),TEXT("%s\\DownLoad"),szWorkDirectory);

	//�ļ�����
	DWORD dwIndex=lstrlen(pszLocation);
	while ((dwIndex>0)&&(pszLocation[dwIndex]!=TEXT('/'))) dwIndex--;

	//�ļ�·��
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	LPCTSTR pszFileName=&pszLocation[dwIndex+1];
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\%s"),szDownLoadPath,pszFileName);

	//�����ļ�
	if (m_DownFile.Open(szFilePath,CFile::modeRead|CFile::modeWrite|CFile::shareDenyNone)==FALSE) return false;

	//�ļ�ƫ��
	m_DownFile.SeekToEnd();

	return true;
}

//�����ļ�
bool CDownLoadTaskSink::CreateDownLoadFile(LPCTSTR pszLocation)
{
	//Ч��״̬
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//Ŀ���ļ�
	TCHAR szLocationPath[MAX_PATH]=TEXT("");
	_sntprintf(szLocationPath,CountArray(szLocationPath),TEXT("%s\\%s"),szWorkDirectory,m_szTaskName);

	//Ŀ��Ŀ¼
	TCHAR szLocationDirectory[MAX_PATH]=TEXT("");
	lstrcpyn(szLocationDirectory,szLocationPath,CountArray(szLocationDirectory));

	//�ļ�����
	DWORD dwIndex=lstrlen(szLocationDirectory);
	while ((dwIndex>0)&&(szLocationDirectory[dwIndex]!=TEXT('\\'))) dwIndex--;

	//�ļ�·��
	szLocationDirectory[dwIndex+1]=0;

	//����Ŀ¼
	SHCreateDirectoryEx(NULL,szLocationDirectory,NULL);

	//�����ļ�
	if (m_DownFile.Open(szLocationPath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUpdateCheckSink::CUpdateCheckSink()
{
	//״̬����
	m_bCheckData=false;
	m_hCheckThread=NULL;
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//�ļ���Ϣ
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;	

	//�ں���Ϣ
	m_pFileData=NULL;
	m_pUpdateMessage=NULL;

	return;
}

//��������
CUpdateCheckSink::~CUpdateCheckSink()
{	
	//�ر�����
	m_DownLoad.CloseDownLoad();

	//�ͷ���Դ
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);

	//�رվ��
	if(m_hCheckThread!=NULL) CloseHandle(m_hCheckThread);
}

//�ӿڲ�ѯ
VOID * CUpdateCheckSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);

	return NULL;
}

//�ر�����
bool CUpdateCheckSink::CloseDownLoad()
{
	//�رմ���
	DestroyWindow();

	//�ر�����
	m_DownLoad.CloseDownLoad();

	//����״̬
	m_bCheckData=false;
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//�ͷ���Դ
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);

	//�رվ��
	if(m_hCheckThread!=NULL) 
	{
		CloseHandle(m_hCheckThread);
		m_hCheckThread=NULL;
	}

	return true;
}

//ִ������
bool CUpdateCheckSink::PerformDownLoad(LPCTSTR pszDownLoadFile)
{
	//Ч��״̬
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//���ñ���
	m_bCheckData=true;
	m_cbDownLoadStatus=DOWN_LOAD_LINK;

	//ִ������
	m_DownLoad.PerformDownLoad(pszDownLoadFile,0,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//�����쳣
bool CUpdateCheckSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//�����쳣
	if(m_pUpdateMessage!=NULL) m_pUpdateMessage->SendMessage(WM_UPDATECHECK_ERROR);

	return true;
}

//����״̬
bool CUpdateCheckSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//����״̬
	switch (DownLoadStatus)
	{
	case DownLoadStatus_Service:		//����״̬
		{
			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_READ;

			break;
		}
	case DownLoadStatus_Conclude:		//���״̬
		{
			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//�����߳�
			m_hCheckThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PerformCheckSum,(LPVOID)this,0L,0L);	

			break;
		}
	}

	return true;
}

//��������
bool CUpdateCheckSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//д���ļ�	
	CopyMemory(&m_pFileData[m_dwDownLoadSize],pcbMailData,wStreamSize);

	//���´�С
	m_dwDownLoadSize+=wStreamSize;

	return true;
}

//������Ϣ
bool CUpdateCheckSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//���ñ���
	m_dwTotalFileSize+=dwTotalFileSize;

	//�����ڴ�
	try
	{
		//�����ڴ�
		m_pFileData = new BYTE[m_dwTotalFileSize];
		if(m_pFileData==NULL) throw TEXT("�ڴ治��,��Դ����ʧ�ܣ�");

		//��ʼ����
		ZeroMemory(m_pFileData,sizeof(m_pFileData));

		return true;
	}
	catch(...)
	{
		ASSERT(FALSE);

		//�ͷ���Դ
		SafeDeleteArray(m_pFileData);

		//�ر�����
		CloseDownLoad();
	}

	return false;
}

//У�麯��
VOID CUpdateCheckSink::PerformCheckSum(LPVOID pThreadData)
{
	//��ȡ����
	CUpdateCheckSink * pUpdateCheckSink = (CUpdateCheckSink *)pThreadData;
	CWndUpdateMessage * pUpdateMessage = pUpdateCheckSink->m_pUpdateMessage;

	//�ж�״̬
	if(pUpdateCheckSink->m_bCheckData == false) return;

	//��������
	DWORD dwUnCompressSize=0;
	BYTE * pUnCompressData=NULL;

	try
	{
		//�����С
		dwUnCompressSize=pUpdateCheckSink->m_dwTotalFileSize*10;

		//�����ڴ�		
		pUnCompressData=new BYTE[dwUnCompressSize];
		if(pUnCompressData==NULL) throw TEXT("�ڴ治��,��Դ����ʧ�ܣ�");

		//���ݽ�ѹ��
		dwUnCompressSize=CWHService::UnCompressData(pUpdateCheckSink->m_pFileData,pUpdateCheckSink->m_dwTotalFileSize,pUnCompressData,dwUnCompressSize);

		//��ѹʧ��
		if(dwUnCompressSize==0) 
		{
			//���֪ͨ
			if(pUpdateMessage->m_hWnd != NULL) pUpdateMessage->PostMessage(WM_UPDATECHECK_FINISH);

			return;
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
		SafeDeleteArray(pUnCompressData);

		return;
	}	

	//��������
	WORD wDataIndex=0;
	tagUpdateInfo UpdateInfo;
	tagUpdateItem UpdateItem;
	tagUpdateUICheck UIInfo;

	//��ȡ����
	UpdateInfo.wStructSize=*(WORD *)pUnCompressData;	
	CopyMemory(&UpdateInfo,pUnCompressData,UpdateInfo.wStructSize);
	wDataIndex += UpdateInfo.wStructSize;	

	//������Ϣ
	if(UpdateInfo.wUpdateCount>0 && pUpdateMessage->m_hWnd)
	{
		pUpdateMessage->SendMessage(WM_UPDATECHECK_UPDATEINFO,(WPARAM)&UpdateInfo);
	}

	//��������
	CString strFileCheckSum;
	TCHAR szFilePath[MAX_PATH]=TEXT("");	

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	UIInfo.wTotalNum = UpdateInfo.wUpdateCount;
	UIInfo.wCurrNum = 0;	
	//�ļ�У��
	for(INT_PTR nIndex=0;nIndex<UpdateInfo.wUpdateCount;nIndex++)
	{

		
		//״̬�ж�
		if(pUpdateCheckSink->m_bCheckData == false) return;

		//�������
		ZeroMemory(&UpdateItem,sizeof(UpdateItem));

		//��ȡ����
		UpdateItem.wStructSize=*(WORD *)&pUnCompressData[wDataIndex];	
		CopyMemory(&UpdateItem,&pUnCompressData[wDataIndex],UpdateItem.wStructSize);

		//���ñ���
		wDataIndex += UpdateItem.wStructSize;

		//����·��
		_sntprintf(szFilePath,sizeof(szFilePath),TEXT("%s/%s"),szWorkDirectory,CA2CT(UpdateItem.szFileName));
		
		//����UI
		UIInfo.wCurrNum++;
		ZeroMemory(UIInfo.szCheckFile,sizeof(UIInfo.szCheckFile));
		_sntprintf(UIInfo.szCheckFile,sizeof(UIInfo.szCheckFile),TEXT("%s"),CA2CT(UpdateItem.szFileName));
		if(pUpdateMessage->m_hWnd != NULL)
		{
			pUpdateMessage->SendMessage(WM_UI_UPDATECHECK,(WPARAM)&UIInfo);
		}

		//��ȡУ���
		strFileCheckSum =CWHMD5Checksum::GetMD5(szFilePath);

		//У��ͱȽ�
		if(lstrcmp(strFileCheckSum,CA2CT(UpdateItem.szFileCheckSum))!=0)
		{
			//��������
			if(pUpdateMessage->m_hWnd != NULL)
			{
				pUpdateMessage->SendMessage(WM_UPDATECHECK_UPDATEITEM,(WPARAM)&UpdateItem);
			}
		}
	}

	//�ͷ���Դ
	SafeDeleteArray(pUnCompressData);

	//���֪ͨ
	if(pUpdateMessage->m_hWnd != NULL) pUpdateMessage->PostMessage(WM_UPDATECHECK_FINISH);
}

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CWndUpdateMessage::CWndUpdateMessage()
{
	//���ýӿ�
	m_pIUpdateViewSink=NULL;
}

//��������
CWndUpdateMessage::~CWndUpdateMessage()
{
}

//���ýӿ�
VOID CWndUpdateMessage::SetUpdateViewSink(IUpdateViewSink * pIUpdateViewSink)
{
	m_pIUpdateViewSink=pIUpdateViewSink;
}

//�����Ϣ
LRESULT CWndUpdateMessage::OnMessageDownLoadTaskFinish(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventDownLoadTaskFinish(wParam==TRUE);
	}

	return 0;
}

//������Ϣ
LRESULT CWndUpdateMessage::OnMessageDownLoadTaskUpdate(WPARAM wParam,LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventDownLoadTaskUpdate(wParam==TRUE);
	}

	return 0;
}

//�����¼�
LRESULT CWndUpdateMessage::OnMessageUpdateCheckError(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckError();
	}

	return 0;
}

//����¼�
LRESULT CWndUpdateMessage::OnMessageUpdateCheckFinish(WPARAM wParam, LPARAM lParam)
{

	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckFinish();
	}

	return 0;
}

//��ַ�¼�
LRESULT CWndUpdateMessage::OnMessageUpdateCheckUpdateInfo(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckUpdateInfo((tagUpdateInfo*)wParam);
	}

	return 0;
}

//����¼�
LRESULT CWndUpdateMessage::OnMessageUpdateCheckUpdateItem(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateCheckUpdateItem((tagUpdateItem*)wParam);
	}

	return 0;
}

LRESULT CWndUpdateMessage::OnMessageUpdateUICheck(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateUICheck((tagUpdateUICheck *)wParam);
	}

	return 0;
}

LRESULT CWndUpdateMessage::OnMessageUpdateUIDownload(WPARAM wParam, LPARAM lParam)
{
	if(m_pIUpdateViewSink!=NULL)
	{
		m_pIUpdateViewSink->OnEventUpdateUIDownload((tagUpdateUIDownload *)wParam);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

//���캯��
CUpdateStatusView::CUpdateStatusView()
{
	//״̬����
	m_uImageIndex=0;
	
	//��������
	m_dwTickCount=0;
	m_dwTotalTaskCount = 0;
	m_dwTaskFinished=0;
	m_cbCurrRetryCount=0;

	//��������
	m_szFileName[0]=0;
	m_szDescribe[0]=0;
	m_szUpdateFile[0]=0;
	m_szDownLoadAddress[0]=0;

	m_szDownDirectory[0]=0;

	//�ؼ�����
	m_pUpdateCheckSink=NULL;
	m_pDownLoadTaskSink=NULL;	

	//�ӿڱ���
	m_pIUpdateStatusSink=NULL;
}

//��������
CUpdateStatusView::~CUpdateStatusView()
{	
	//��������
	EndUpdateStatus();
}

BOOL CUpdateStatusView::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CUpdateStatusView::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(szProduct);

	//���д���
	CenterWindow();
	//���ÿؼ�
	/*SetTimer(IDT_STATUS,TIMER_STATUS,NULL);*/


	return TRUE;
}

void CUpdateStatusView::InitControlUI()
{
	__super::InitControlUI();
	return;
}

void CUpdateStatusView::Notify(TNotifyUI & msg)
{
    
}

LPCTSTR CUpdateStatusView::GetSkinFile()
{
	return TEXT("DlgUpdateStatus.xml");
}

////����λ��
//void CUpdateStatusView::SetPos(RECT rc)
//{
//	__super::SetPos(rc);
//}
//
////�¼�����
//void CUpdateStatusView::DoEvent(TEventUI& event)
//{
//	if( event.Type == UIEVENT_TIMER )
//	{
//		//����״̬
//		if (event.wParam==IDI_UPDATE_STATUS)
//		{
//			//��������
//			m_uImageIndex=(m_uImageIndex+1)%ANIMAL_FRAME_COUNT;
//
//			//���½���
//			Invalidate();
//
//			return;
//		}
//
//		//��ʱ����
//		if(event.wParam==IDI_UPDATE_DELAYEND)
//		{
//			//�رն�ʱ��
//			m_pManager->KillTimer(this,IDI_UPDATE_DELAYEND);
//
//			//��������
//			EndUpdateStatus();
//
//			//���֪ͨ
//			if(m_pIUpdateStatusSink) 
//			{
//				m_pIUpdateStatusSink->OnEventDownLoadTaskFinish();
//			}	
//
//			return;
//		}
//	}
//
//	CContainerUI::DoEvent(event);
//}
//
////�滭����
//void CUpdateStatusView::DoPaint(HDC hDC, const RECT& rcPaint)
//{
//	//��ȡ�豸
//	CDC * pDC = CDC::FromHandle(hDC);
//
//	//���û���
//	pDC->SetBkMode(TRANSPARENT);
//	pDC->SelectObject(m_pManager->GetFont(0));
//
//	//��ȡλ��
//	CRect rcClient(m_rcItem);	
//
//	//��ȡ���
//	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);
//
//	//�滭����
//	CPngImage ImageStatus;
//	ImageStatus.LoadImage(hInstance,TEXT("STATUS_BACK"));
//
//	//��������
//	CPngImage ImageStatusWait;	
//	ImageStatusWait.LoadImage(hInstance,TEXT("STATUS_WAITING"));
//
//	//��ȡ��С
//	CSize SizeStatusBack(ImageStatus.GetWidth(),ImageStatus.GetHeight());
//	CSize SizeStatusWait(ImageStatusWait.GetWidth()/ANIMAL_FRAME_COUNT,ImageStatusWait.GetHeight());
//
//	//���Ʊ���
//	ImageStatus.DrawImage(pDC,(m_rcItem.left+rcClient.Width()-SizeStatusBack.cx)/2,m_rcItem.top+(rcClient.Height()-SizeStatusBack.cy)/2);
//	
//	//������
//	if(m_pDownLoadTaskSink==NULL)
//	{
//		//���ƹ�Ȧ
//		ImageStatusWait.DrawImage(pDC,m_rcItem.left+(rcClient.Width()-SizeStatusBack.cx)/2,m_rcItem.top+(rcClient.Height()-SizeStatusBack.cy)/2,SizeStatusWait.cx,SizeStatusWait.cy,m_uImageIndex*SizeStatusWait.cx,0,SizeStatusWait.cx,SizeStatusWait.cy); 
//
//		//��������
//		CPngImage ImageUpdateWords;
//		ImageUpdateWords.LoadImage(hInstance,TEXT("STATUS_UPDATE_WORDS"));		
//
//		//�滭����
//		ImageUpdateWords.DrawImage(pDC,m_rcItem.left+rcClient.Width()/2-ImageUpdateWords.GetWidth()/2,m_rcItem.top+rcClient.Height()/2-ImageUpdateWords.GetHeight()/2);
//	}
//	else
//	{
//		//��������
//		INT nCurrProgress=0,nTotalProgress=0;
//
//		//��������
//		DWORD  dwTotalFileSize = m_pDownLoadTaskSink->m_dwTotalFileSize;
//		DWORD  dwDownLoadFileSize = m_pDownLoadTaskSink->m_dwDownLoadSize;
//
//		//�������		
//		if(dwTotalFileSize>0)  nCurrProgress = ((double)dwDownLoadFileSize/(double)dwTotalFileSize)*100;
//		if(m_dwTotalTaskCount>0)
//		{
//			nTotalProgress = ((double)m_dwTaskFinished/(double)m_dwTotalTaskCount)*100+nCurrProgress/m_dwTotalTaskCount;
//		}
//
//		//��������
//		CPngImage ImageNumber;
//		CPngImage ImagePercent;		
//
//		//������Դ
//		ImageNumber.LoadImage(hInstance,TEXT("STATUS_NUMBER"));
//		ImagePercent.LoadImage(hInstance,TEXT("STATUS_PERCENT"));		
//
//		//�ߴ綨��		
//		CSize SizeNumber(ImageNumber.GetWidth()/10,ImageNumber.GetHeight());
//		CSize SizePercent(ImagePercent.GetWidth(),ImagePercent.GetHeight());
//
//		//���Ƶȴ�
//		ImageStatusWait.DrawImage(pDC,m_rcItem.left+(rcClient.Width()-SizeStatusBack.cx)/2,m_rcItem.top+(rcClient.Height()-SizeStatusBack.cy)/2,SizeStatusWait.cx,SizeStatusWait.cy,m_uImageIndex*SizeStatusWait.cx,0,SizeStatusWait.cx,SizeStatusWait.cy); 
//
//		//������
//		int nNumberWidth = DrawNumber(pDC,m_rcItem.left+rcClient.Width()/2+12,m_rcItem.top+rcClient.Height()/2,&ImageNumber,TEXT("0123456789"),nTotalProgress,TEXT("%d"),DT_CALCRECT);	
//
//		//ƫ��λ��
//		int nXOffset=(nNumberWidth-SizePercent.cx)/2;
//
//		//�滭�ٷֺ�
//		ImagePercent.DrawImage(pDC,m_rcItem.left+rcClient.Width()/2+nXOffset,m_rcItem.top+(rcClient.Height()-SizePercent.cy)/2+4);
//
//		//�滭����
//		DrawNumber(pDC,m_rcItem.left+rcClient.Width()/2+nXOffset,m_rcItem.top+rcClient.Height()/2,&ImageNumber,TEXT("0123456789"),nTotalProgress,TEXT("%d"),DT_RIGHT|DT_VCENTER);	
//	}
//}

//������Ϣ
VOID CUpdateStatusView::OnEventDownLoadTaskUpdate(bool bUpdate)
{
	//�жϼ��
	if(GetTickCount()-m_dwTickCount>UPDATE_TIMESPACE || bUpdate)
	{
		//���½���
		m_dwTickCount=GetTickCount();		
		Invalidate();
	}
}

//�����Ϣ
VOID CUpdateStatusView::OnEventDownLoadTaskFinish(bool bSuccessed)
{
	//��������
	m_pDownLoadTaskSink->ResetDownLoadSink();

	//�Ƴ�Ԫ��
	if(bSuccessed==true)
	{
		//���ñ���
		++m_dwTaskFinished;
		m_cbCurrRetryCount=0;			

		//�ͷŶ���
		SafeDelete(m_DownLoadFileArray[0]);
		m_DownLoadFileArray.RemoveAt(0);
	}
	else
	{
		//�������Դ���
		if(++m_cbCurrRetryCount==MAX_RETRY_COUNT)
		{
			//ʧ����ʾ
			TCHAR szMessage[128]=TEXT("");
			CDlgInformation Information;
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�ļ� %s �޷�����,����������ԭ�����,���Ժ����ԣ�"),m_DownLoadFileArray.GetAt(0)->szDownloadFile);
			Information.ShowMessageBox(TEXT("��ܰ��ʾ"),szMessage);

			//��������
			EndUpdateStatus();

			return;
		}
	}	

	//�ļ�����
	if(m_DownLoadFileArray.GetCount()>0)
	{
		//��ȡʱ��
		m_dwTickCount=GetTickCount();

		//ִ������
		m_pDownLoadTaskSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile,!bSuccessed);

		//�л�����
		SwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);		
	}
	else
	{
		//��ʱ����
		//m_pManager->SetTimer(this,IDI_UPDATE_DELAYEND,TIMER_UPDATE_DELAYEND);
		m_pIUpdateStatusSink->OnEventDownLoadTaskFinish();
		EndUpdateStatus();
	}

	return;
}

//������
VOID CUpdateStatusView::OnEventUpdateCheckError()
{
	//��������
	EndUpdateStatus();
}

//����¼�
VOID CUpdateStatusView::OnEventUpdateCheckFinish()
{
#if defined(_DEBUG) || defined(RELEASE_MANAGER) 

	//���֪ͨ
	if(m_pIUpdateStatusSink)
	{
		m_pIUpdateStatusSink->OnEventUpdateCheckFinish(false);
	}

	//��������
	return EndUpdateStatus();
	
#else

	//������
	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(_T("������"));
	}

	//��������
	bool bNeedUpdate = m_DownLoadFileArray.GetCount()>0;

	//���֪ͨ
	if(m_pIUpdateStatusSink)
	{
		m_pIUpdateStatusSink->OnEventUpdateCheckFinish(bNeedUpdate);
	}

	if(bNeedUpdate == true) 
	{
		//��������
		return StartDownLoadTask();		
	}
	else
	{
		//��������
		return EndUpdateStatus();
	}

#endif
}

//���µ�ַ
VOID CUpdateStatusView::OnEventUpdateCheckUpdateInfo(tagUpdateInfo * UpdateInfo)
{
	//������ַ
	lstrcpyn(m_szDownLoadAddress,CA2CT(UpdateInfo->szDownLoadAddress),CountArray(m_szDownLoadAddress));
}

//����¼�
VOID CUpdateStatusView::OnEventUpdateCheckUpdateItem(tagUpdateItem * UpdateItem)
{
	try
	{
		//��������
		tagDownloadItem * pDownloadItem = new tagDownloadItem;	
		if(pDownloadItem==NULL) throw TEXT("�ڴ���Դ����,���󴴽�ʧ�ܣ�");

		//��������
		lstrcpyn(pDownloadItem->szDownloadFile,CA2CT(UpdateItem->szFileName),CountArray(pDownloadItem->szDownloadFile));
		lstrcpyn(pDownloadItem->szFileCheckSum,CA2CT(UpdateItem->szFileCheckSum),CountArray(pDownloadItem->szFileCheckSum));

		//��������
		m_dwTotalTaskCount++;
		m_DownLoadFileArray.Add(pDownloadItem);
	}
	catch(...)
	{
		ASSERT(FALSE);
	}

	return;
}

VOID CUpdateStatusView::OnEventUpdateUICheck(tagUpdateUICheck * UIInfo)
{
	//������Ϣ
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("����ļ�:%s [%d/%d]"),UIInfo->szCheckFile,UIInfo->wCurrNum,UIInfo->wTotalNum);

	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(szDownLoadUrl);
	}

	int nProgress = 0;
	nProgress = (UIInfo->wCurrNum * 100)/UIInfo->wTotalNum ;
	CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(_T("StatusProgress"));
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetValue(nProgress);
	}
}

VOID CUpdateStatusView::OnEventUpdateUIDownload(tagUpdateUIDownload * UIInfo)
{
	//������Ϣ
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("�����ļ�:%s [%d/%d]"),UIInfo->szCheckFile,UIInfo->dwDownLoadSize,UIInfo->dwTotalFileSize,UIInfo->dwDownSpeed);

	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(szDownLoadUrl);
	}

	int nProgress = 0;
	nProgress = (m_dwTaskFinished * 100)/m_dwTotalTaskCount ;
	CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(_T("StatusProgress"));
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetValue(nProgress);
	}
}

//�������
VOID CUpdateStatusView::StartUpdateCheck()
{
	//��������
	if(m_pUpdateCheckSink==NULL)
	{
		try
		{
			m_pUpdateCheckSink = new CUpdateCheckSink;
			ASSERT(m_pUpdateCheckSink);
		}
		catch(...)
		{
			ASSERT(FALSE);
			SafeDelete(m_pUpdateCheckSink);

			return;
		};
	}

	//��ʼ����
	if(m_pUpdateCheckSink!=NULL) 
	{
		m_pUpdateCheckSink->m_pUpdateMessage=&m_UpdateMessage;
		m_pUpdateCheckSink->PerformDownLoad(m_szUpdateFile);
	}

	//���ñ���
	m_uImageIndex=0;
	lstrcpyn(m_szDescribe,TEXT("���¼��"),CountArray(m_szDescribe));

	//����ʱ��
	//m_pManager->SetTimer(this,IDI_UPDATE_STATUS,TIMER_UPDATE_STATUS);
}

//��������
VOID CUpdateStatusView::StartDownLoadTask()
{
	//ָ���ж�
	if(m_pDownLoadTaskSink==NULL)
	{
		try
		{
			//����ָ��
			m_pDownLoadTaskSink = new CDownLoadTaskSink();
			ASSERT(m_pDownLoadTaskSink!=NULL);
			if(m_pDownLoadTaskSink==NULL) return;
		}
		catch(...)
		{
			ASSERT(FALSE);
			return;
		}
	}

	//�����ļ�
	if(m_pDownLoadTaskSink!=NULL)
	{
		//����ָ��
		m_pDownLoadTaskSink->m_pUpdateMessage=&m_UpdateMessage;

		//�ļ�����
		if(m_DownLoadFileArray.GetCount()>0)
		{
			//ִ������
			m_pDownLoadTaskSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile);

			//�л�����
			SwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);
		}
	}

	return;
}

//��������
VOID CUpdateStatusView::EndUpdateStatus()
{
	//���ñ���
	m_dwTickCount=0;	
	m_dwTaskFinished=0;
	m_cbCurrRetryCount=0;
	m_dwTotalTaskCount=0;
	m_pIUpdateStatusSink=NULL;

	//�رն�ʱ��
	//m_pManager->KillTimer(this,IDI_UPDATE_STATUS);

	//���ٴ���
	m_UpdateMessage.DestroyWindow();
	m_UpdateMessage.SetUpdateViewSink(NULL);

	//�Ƴ�����
	for(INT_PTR nIndex=0;nIndex<m_DownLoadFileArray.GetCount();nIndex++)
	{
		SafeDelete(m_DownLoadFileArray[nIndex]);		
	}

	//�Ƴ�Ԫ��
	m_DownLoadFileArray.RemoveAll();

	//�ͷŶ���
	if(m_pUpdateCheckSink!=NULL)
	{
		m_pUpdateCheckSink->CloseDownLoad();
		SafeDelete(m_pUpdateCheckSink);
	}

	//�ͷŶ���
	if(m_pDownLoadTaskSink!=NULL) 
	{
		m_pDownLoadTaskSink->CloseDownLoad();
		SafeDelete(m_pDownLoadTaskSink);
	}

	////�Ƴ��Լ�
	//CContainerUI * pParent = static_cast<CContainerUI *>(GetParent());	
	//if(pParent != NULL)
	//{
	//	pParent->Remove(this,false);

	//	//���¸���
	//	pParent->Invalidate();
	//}
	DestroyWindow();
}

//��ʼ����
VOID CUpdateStatusView::InitUpdateStatus(IUpdateStatusSink * pIUpdateStatusSink, LPCTSTR pszProgressName)
{

	CControlUI *pControlUI =  (CControlUI * )GetControlByName(_T("LableStatus"));
	if(NULL != pControlUI)
	{
		pControlUI->SetText(_T("��ʼ���"));
	}

	//���ñ���
	m_pIUpdateStatusSink=pIUpdateStatusSink;

	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	_sntprintf(m_szUpdateFile,CountArray(m_szUpdateFile),TEXT("%s/DownLoad/Client/%s.txt"),pGlobalWebLink->GetPlatformLink(),pszProgressName);

	//��������	
	if(m_UpdateMessage.m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		m_UpdateMessage.Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);

		//���ýӿ�
		m_UpdateMessage.SetUpdateViewSink(this);
	}

	//�������
	StartUpdateCheck();
}

VOID CUpdateStatusView::SetDownDirectory(LPCTSTR pszProgressName)
{
	//������ַ
	lstrcpyn(m_szDownDirectory,pszProgressName,CountArray(m_szDownDirectory));
}

//�л�����
VOID CUpdateStatusView::SwitchDownLoadTask(LPCTSTR pszFileName)
{
	//����У��
	if(pszFileName == NULL) return;

	//�����ַ�
	DWORD dwIndex = lstrlen(pszFileName);
	while(--dwIndex>0 && pszFileName[dwIndex]!=TEXT('/'));
	
	//�����ַ�
	lstrcpyn(m_szFileName,pszFileName+(dwIndex>0?++dwIndex:0),CountArray(m_szFileName));
}

VOID CUpdateStatusView::OnTimer(UINT nIDEvent)
{

}



/////////////////////////////////////////////////////////////////////////////////////////////////
