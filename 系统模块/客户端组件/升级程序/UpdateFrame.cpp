#include "StdAfx.h"
#include "UpdateFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//��Ӱ����
#define SHADOW_CX					5									//��Ӱ���
#define SHADOW_CY					5									//��Ӱ�߶�

//λ�ö���
#define CX_BORAD					5									//�߿���
#define CY_BORAD					5									//�߿�߶�

//��Ļλ��
#define BORAD_SIZE					5									//�߿��С
#define CAPTION_SIZE				79									//�����С
#define LESS_SCREEN_CX              1024                                //��Ļ��С  
#define LESS_SCREEN_CY              768                                 //��Ļ��С

//��Ŀ����
#define MIN_LINE					3									//������Ŀ
#define MAX_LINE					6									//������Ŀ
#define MAX_RETRY_COUNT				3									//���Դ���
#define FLASH_ITEM_TIMES			8									//��˸����

//ʱ���ʶ
#define IDI_FLASH_ITEM				100									//���̼��
#define IDI_UPDATE_STATUS			101									//����״̬

//ʱ���ʶ
#define IDI_CHECK_PROCESS			200									//���̼��

//���ڱ�ʶ
#define IDC_CLOSE					300									//�رհ�ť
#define IDC_DOWNLOAD_MANAGER        301                                 //���ڱ�ʶ
#define IDC_SELFUPDATE				302									//�ֶ���ť

//���¼��
#define UPDATE_TIMESPACE			24									//���¼��	

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUDownLoadSink, CWnd)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDownLoadManager, CWnd)

	//�Զ���Ϣ
	ON_MESSAGE(WM_ITEM_DOWNLOAD_Finish,OnItemDownLoadFinishMessage)
	ON_MESSAGE(WM_ITEM_DOWNLOAD_UPDATE,OnItemDownLoadUpdateMessage)

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CGameUpdateFrame, CFrameWnd)
	
	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	ON_WM_NCDESTROY()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_SELFUPDATE, OnBnClickedSelfUpdate)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////


//���캯��
CUDownLoadSink::CUDownLoadSink()
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
	m_pDownLoadManager=NULL;

	return;
}

//��������
CUDownLoadSink::~CUDownLoadSink()
{
}

//�ӿڲ�ѯ
VOID * CUDownLoadSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//�ر�����
bool CUDownLoadSink::CloseDownLoad()
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
bool CUDownLoadSink::PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad)
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
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s\\%s"),pszNetAddress,pszFileName);

	//�ϵ�����
	if(bBreakpointDownLoad && LoadDownFile(szDownLoadUrl)) 
	{
		//��������
		DWORD dwFileSize = (DWORD)m_DownFile.GetLength();		

		//ִ������
		m_DownLoad.PerformDownLoad(szDownLoadUrl,QUERY_ME_INTERFACE(IUnknownEx),dwFileSize);
	}
	else
	{
		//ִ������
		m_DownLoad.PerformDownLoad(szDownLoadUrl,QUERY_ME_INTERFACE(IUnknownEx),0);
	}

	//������Ϣ
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd())) 
	{
		m_pDownLoadManager->PostMessage(WM_ITEM_DOWNLOAD_UPDATE,0,0);
	}

	return true;
}

VOID CUDownLoadSink::PerformInstall()
{
	//�����ļ�
	TCHAR szTargetName[MAX_PATH]=TEXT("");
	TCHAR strSourceName[MAX_PATH]=TEXT("");
	_sntprintf(strSourceName,CountArray(strSourceName),TEXT("%s"),m_DownFile.GetFilePath());

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//�ļ�·��
	_sntprintf(szTargetName,CountArray(szTargetName),TEXT("%s\\%s"),szWorkDirectory,m_szTaskName);

	//�޸�Ŀ¼
	INT nIndex = lstrlen(szTargetName);
	while(--nIndex>=0) if(szTargetName[nIndex]==TEXT('/')) szTargetName[nIndex]=TEXT('\\');

	//�ر��ļ�
	m_DownFile.Close();

	//��������
	tagDownLoadResult * pDownLoadResult = new tagDownLoadResult;
	pDownLoadResult->bDownLoadSuccess=true;
	lstrcpyn(pDownLoadResult->szSourceFilePath,strSourceName,CountArray(pDownLoadResult->szSourceFilePath));
	lstrcpyn(pDownLoadResult->szTargetFilePath,szTargetName,CountArray(pDownLoadResult->szTargetFilePath));

	//������Ϣ
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd()))
	{
		m_pDownLoadManager->PostMessage(WM_ITEM_DOWNLOAD_Finish,(WPARAM)pDownLoadResult);
	}

	return;
}

//���ñ���
VOID CUDownLoadSink::ResetDownLoadSink()
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
bool CUDownLoadSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//������Ϣ
	if(m_pDownLoadManager!=NULL ) 
	{
		if(m_DownFile.m_hFile!=INVALID_HANDLE_VALUE && m_DownFile.GetLength()>0)
		{
			//�ر��ļ�
			m_DownFile.Close();
		}

		//��������
		tagDownLoadResult * pDownLoadResult = new tagDownLoadResult;
		pDownLoadResult->bDownLoadSuccess=false;
		pDownLoadResult->szSourceFilePath[0]=0;
		pDownLoadResult->szTargetFilePath[0]=0;

		//����֪ͨ
		m_pDownLoadManager->PostMessage(WM_ITEM_DOWNLOAD_Finish,(WPARAM)pDownLoadResult);
	}

	return true;
}

//����״̬
bool CUDownLoadSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
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
			m_cbDownLoadStatus=DOWN_LOAD_SETUP;

			//ִ�а�װ
			PerformInstall();
			
			break;
		}
	}

	return true;
}

//��������
bool CUDownLoadSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
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
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd())) 
	{
		m_pDownLoadManager->SendMessage(WM_ITEM_DOWNLOAD_UPDATE,m_dwDownLoadSize==m_dwTotalFileSize);
	}
	
	return true;
}

//������Ϣ
bool CUDownLoadSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
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

		if(CreateDownFile(m_szTaskName)==false)
		{
			//�ر�����
			CloseDownLoad();

			return false;
		}
	}

	//������Ϣ
	if(m_pDownLoadManager!=NULL && IsWindow(m_pDownLoadManager->GetSafeHwnd())) 
	{
		m_pDownLoadManager->SendMessage(WM_ITEM_DOWNLOAD_UPDATE);
	}

	return true;
}


//�����ļ�
bool CUDownLoadSink::LoadDownFile(LPCTSTR pszLocation)
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
bool CUDownLoadSink::CreateDownFile(LPCTSTR pszLocation)
{
	//Ч��״̬
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//���ñ���
	LPTSTR pszDelete = (_tcsrchr(pszLocation, TEXT('?'))); 
	if(pszDelete!=NULL) pszDelete[0]=0;

	//�ļ�·��
	TCHAR szFilePath[MAX_PATH]=TEXT("");	
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\DownLoad\\%s"),szWorkDirectory,pszLocation);	

	//Ŀ��Ŀ¼
	TCHAR szDownLoadPath[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadPath,CountArray(szDownLoadPath),TEXT("%s\\DownLoad\\%s"),szWorkDirectory,pszLocation);
	
	//�ļ�����
	DWORD dwIndex=lstrlen(szDownLoadPath);
	while ((dwIndex>0)&&(szDownLoadPath[dwIndex]!=TEXT('\\'))) dwIndex--;

	//��ȡĿ¼
	szDownLoadPath[dwIndex]=0;	

	//����Ŀ¼
	SHCreateDirectoryEx(NULL,szDownLoadPath,NULL);

	//�����ļ�
	if (m_DownFile.Open(szFilePath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadManager::CDownLoadManager()
{
	//��������
	m_dwTickCount=0;	
	m_dwTaskFinished=0;
	m_cbCurrRetryCount=0;
	m_dwTotalTaskCount=0;
	m_szDownLoadAddress[0]=0;

	//�ں���Ϣ
	m_pDownLoadSink=NULL;
	m_pGameUpdateFrame=NULL;	

	return;
}

//��������
CDownLoadManager::~CDownLoadManager()
{
	//�ͷŶ���
	if(m_pDownLoadSink!=NULL)
	{
		m_pDownLoadSink->CloseDownLoad();
		SafeDelete(m_pDownLoadSink);
	}

	//�ͷŶ���
	while(m_DownLoadFileArray.GetCount()>0)
	{
		SafeDelete(m_DownLoadFileArray[0]);
		m_DownLoadFileArray.RemoveAt(0);
	}
}

//��ȡ��Ϣ
bool CDownLoadManager::ReadDownLoadInfo()
{
	//��������
	TCHAR  szTargetFilePath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szTargetFilePath,CountArray(szTargetFilePath));
	_sntprintf(szTargetFilePath,CountArray(szTargetFilePath),TEXT("%s\\Update.WHD"),szTargetFilePath);

	//��������
	int nUpdateFileCount = GetPrivateProfileInt(TEXT("UPDATEFILELIST"),TEXT("UPDATEFILECOUNT"),0,szTargetFilePath);

	//�����ַ
	GetPrivateProfileString(TEXT("UPDATEFILELIST"),TEXT("FILENETADDRESS"),TEXT(""),m_szDownLoadAddress,sizeof(m_szDownLoadAddress),szTargetFilePath);

	//��������
	TCHAR szKeyText[32]=TEXT("");

	//��ȡ�ļ�
	for(int nIndex=0;nIndex<nUpdateFileCount;nIndex++)
	{
		tagDownloadItem * pDownloadItem = new tagDownloadItem;
		if(pDownloadItem==NULL) break;

		//��ʽ�ַ�
		_sntprintf(szKeyText,CountArray(szKeyText),TEXT("FileIndex%d"),nIndex);
		GetPrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,TEXT(""),pDownloadItem->szDownloadFile,sizeof(pDownloadItem->szDownloadFile),szTargetFilePath);

		//��ʽ�ַ�
		_sntprintf(szKeyText,CountArray(szKeyText),TEXT("FileCheckNum%d"),nIndex);
		GetPrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,TEXT(""),pDownloadItem->szFileCheckSum,sizeof(pDownloadItem->szFileCheckSum),szTargetFilePath);

		//�����б�
		m_DownLoadFileArray.Add(pDownloadItem);
	}

	//���ñ���
	m_dwTotalTaskCount = static_cast<DWORD>(m_DownLoadFileArray.GetCount());

	//ɾ���ļ�
	DeleteFile(szTargetFilePath);

	return true;
}

//ִ������
bool CDownLoadManager::StartDownLoadTask()
{
	//ָ���ж�
	if(m_pDownLoadSink==NULL)
	{
		try
		{
			//����ָ��
			m_pDownLoadSink = new CUDownLoadSink();
			ASSERT(m_pDownLoadSink!=NULL);
			if(m_pDownLoadSink==NULL) return false;
		}
		catch(...)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//�����ļ�
	if(m_pDownLoadSink!=NULL)
	{
		//����ָ��
		m_pDownLoadSink->m_pDownLoadManager=this;

		//�ļ�����
		if(m_DownLoadFileArray.GetCount()>0)
		{
			//ִ������
			m_pDownLoadSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile);

			//�л�����
			if(m_pGameUpdateFrame!=NULL) m_pGameUpdateFrame->OnEventSwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);
		}
	}

	return true;
}

//�ƶ��ļ�
VOID CDownLoadManager::PerformMoveFiles()
{
	//ѭ���ƶ�
	while(m_SourcePathArray.GetCount()>0)
	{
		//��������
		LPCTSTR szSourceFilePath,szTargetFilePath;

		//���ñ���
		szSourceFilePath = m_SourcePathArray.GetAt(0);
		szTargetFilePath = m_TargetPathArray.GetAt(0);

		//�ļ�Ŀ¼
		TCHAR szFileDirectory[MAX_PATH]=TEXT("");
		_sntprintf(szFileDirectory,CountArray(szFileDirectory),TEXT("%s"),szTargetFilePath);
		(_tcsrchr(szFileDirectory, TEXT('\\')))[0] = 0; 

		//����Ŀ¼
		SHCreateDirectoryEx(NULL,szFileDirectory,NULL);

		//ɾ���ļ�
		if (DeleteFile(szTargetFilePath)==FALSE && ::GetLastError()!=ERROR_FILE_NOT_FOUND)
		{
			//��������
			TCHAR szTargetNameEx[MAX_PATH]=TEXT("");	
			_sntprintf(szTargetNameEx,CountArray(szTargetNameEx),TEXT("%s.old"),szSourceFilePath);

			//ɾ���ļ�
			DeleteFile(szTargetNameEx);

			//�ƶ��ļ�
			if (MoveFileEx(szTargetFilePath,szTargetNameEx,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
			{
				ASSERT(FALSE);	
			}

			//���ɾ��
			if (MoveFileEx(szTargetNameEx,NULL,MOVEFILE_DELAY_UNTIL_REBOOT|MOVEFILE_REPLACE_EXISTING)==FALSE)
			{
				ASSERT(FALSE);
			}
		}

		//�ƶ��ļ�
		if (MoveFileEx(szSourceFilePath,szTargetFilePath,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING))
		{
			//ɾ���ļ�
			DeleteFile(szSourceFilePath);
		}

		//�Ƴ�Ԫ��
		m_SourcePathArray.RemoveAt(0);
		m_TargetPathArray.RemoveAt(0);
	}

	return;
}

//״̬��Ϣ
LRESULT CDownLoadManager::OnItemDownLoadFinishMessage(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	tagDownLoadResult * pDownLoadResult=(tagDownLoadResult *)wParam;
	if(pDownLoadResult==NULL) return 1L;

	//��������
	tagDownLoadResult DownLoadResult;
	CopyMemory(&DownLoadResult,pDownLoadResult,sizeof(DownLoadResult));

	//�ͷ��ڴ�
	SafeDelete(pDownLoadResult);

	//��������
	bool bDownLoadSuccess = DownLoadResult.bDownLoadSuccess;

	//�Ƴ�Ԫ��
	if(bDownLoadSuccess==true)
	{
		//�ļ�У��
		CString szLocalFileCheckSum =CWHMD5Checksum::GetMD5(DownLoadResult.szSourceFilePath);
		if(lstrcmp(szLocalFileCheckSum,m_DownLoadFileArray[0]->szFileCheckSum)!=0)
		{
			//�˳���ʾ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�ļ� %s У�������������ƥ��,����ͷ���Ա��ϵ��"),m_DownLoadFileArray[0]->szDownloadFile);

			tagDownloadItem * pDownLoadItem = m_DownLoadFileArray[0];

			MessageBox(szMessage,TEXT("��ܰ��ʾ"));		

			//�ƶ��ļ�
			PerformMoveFiles();

			//�رս���
			AfxGetMainWnd()->PostMessage(WM_CLOSE);

			return 0L;
		}

		//���ñ���
		m_cbCurrRetryCount=0;
		++m_dwTaskFinished;	

		//�ͷŶ���
		SafeDelete(m_DownLoadFileArray[0]);
		m_DownLoadFileArray.RemoveAt(0);
	}
	else
	{
		if(++m_cbCurrRetryCount==MAX_RETRY_COUNT)
		{
			//�˳���ʾ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�ļ� %s �޷�����,����������ԭ�����,���Ժ����ԣ�"),m_DownLoadFileArray.GetAt(0)->szDownloadFile);
			MessageBox(szMessage,TEXT("��ܰ��ʾ"));

			//�ƶ��ļ�
			PerformMoveFiles();

			//�رս���
			AfxGetMainWnd()->PostMessage(WM_CLOSE);

			return 0L;
		}
	}

	if(m_pDownLoadSink!=NULL)
	{
		//״̬����
		m_pDownLoadSink->ResetDownLoadSink();

		//��¼�ļ�
		if(DownLoadResult.szSourceFilePath[0]!=0 && DownLoadResult.szTargetFilePath[0]!=0)
		{
			m_SourcePathArray.Add(DownLoadResult.szSourceFilePath);
			m_TargetPathArray.Add(DownLoadResult.szTargetFilePath);
		}

		//�ļ�����
		if(m_DownLoadFileArray.GetCount()>0)
		{
			//��¼ʱ���
			m_dwTickCount=GetTickCount();

			//ִ������
			m_pDownLoadSink->PerformDownLoad(m_szDownLoadAddress,m_DownLoadFileArray[0]->szDownloadFile,!bDownLoadSuccess);

			//�л�����
			if(m_pGameUpdateFrame!=NULL) m_pGameUpdateFrame->OnEventSwitchDownLoadTask(m_DownLoadFileArray[0]->szDownloadFile);
		}
		else
		{
			//�ƶ��ļ�
			PerformMoveFiles();

			//�������
			if(m_pGameUpdateFrame!=NULL) m_pGameUpdateFrame->OnEventDownLoadEnd();
		}
	}

	return 0L;
}

//״̬��Ϣ
LRESULT CDownLoadManager::OnItemDownLoadUpdateMessage(WPARAM wParam,LPARAM lParam)
{
	//���½���
	if(m_pGameUpdateFrame!=NULL)
	{
		//�жϼ��
		if(GetTickCount()-m_dwTickCount>=UPDATE_TIMESPACE || wParam==TRUE)
		{
			//���½���
			m_dwTickCount=GetTickCount();		
			m_pGameUpdateFrame->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameUpdateFrame::CGameUpdateFrame()
{
	//ָ�����
	m_pDownLoadManager = NULL;

	return;
}

//��������
CGameUpdateFrame::~CGameUpdateFrame()
{
	//�ͷ���Դ
	if(m_pDownLoadManager!=NULL) 
	{
		SafeDelete(m_pDownLoadManager);
	}
}

//���ؽ���
VOID CGameUpdateFrame::OnEventDownLoadEnd()
{
	//����Ŀ¼
	TCHAR szMouduleFileName[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szMouduleFileName,CountArray(szMouduleFileName));

	//�����ַ
	TCHAR szMainSetupPath[MAX_PATH]=TEXT("");
	_sntprintf(szMainSetupPath,CountArray(szMainSetupPath),TEXT("%s/GamePlaza.exe"),szMouduleFileName);

	//�رմ���
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	//��������
	ShellExecute(0,TEXT("open"),szMainSetupPath,TEXT("UPDATED"),NULL,true );

	return;
}

//�л�����
VOID CGameUpdateFrame::OnEventSwitchDownLoadTask(LPCTSTR pszFileName)
{
	//�����ַ�
	DWORD dwIndex = lstrlen(pszFileName);
	if(dwIndex>0)
	{
		while(--dwIndex>0 && pszFileName[dwIndex]!=TEXT('\\'));
	}
	
	//�����ַ�
	CopyMemory(m_pszFileName,pszFileName+(dwIndex>0?++dwIndex:0),CountArray(m_pszFileName));

	return;
}

//�ؼ���
VOID CGameUpdateFrame::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��Ϣ����
BOOL CGameUpdateFrame::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//�ر���Ϣ
VOID CGameUpdateFrame::OnClose()
{
	__super::OnClose();

	return;
}

//���ٷǿͻ���
VOID CGameUpdateFrame::OnNcDestroy()
{
	//���ٴ���
	if(m_pDownLoadManager!=NULL)
	{
		m_pDownLoadManager->DestroyWindow();
	}

	CFrameWnd::OnNcDestroy();
}


//��������
INT CGameUpdateFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//����ͼ��
	SetWindowText(TEXT("��������"));

	//���ô���
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//��Դ���
	HINSTANCE hInstance=AfxGetResourceHandle();

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btClose.SetButtonImage(IDB_BT_CLOSE_BACK,TEXT("BT_CLOSE"),hInstance,false,false);

	////������ť
	//CRect rcCreateSelf(0,0,0,0);
	//m_btSelfUpdate.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreateSelf,this,IDC_SELFUPDATE);
	//m_btSelfUpdate.SetButtonImage(IDB_BT_SELFUPDATE_BACK,TEXT("BT_SELFUPDATE"),hInstance,false,false);
	

	//����ͼƬ
	CPngImage UpdateFrameBack;
	UpdateFrameBack.LoadImage(hInstance,TEXT("UPDATE_FRAME_BACK"));	

	//����λ��
	CRect rcArce,rcFrame;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//���ñ���
	rcFrame.left=(rcArce.Width()-UpdateFrameBack.GetWidth())/2;
	rcFrame.top=(rcArce.Height()-UpdateFrameBack.GetHeight())/2;
	rcFrame.right=rcFrame.left + UpdateFrameBack.GetWidth();
	rcFrame.bottom=rcFrame.top + UpdateFrameBack.GetHeight();

	//���ô���
	SetWindowPos(NULL,rcFrame.left,rcFrame.top,rcFrame.Width(),rcFrame.Height(),SWP_NOZORDER);	

	//�����ֲ�
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_SkinLayered.CreateLayered(m_hWnd);
	
	//������Դ
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyWindow(rcClient.Width(),rcClient.Height());

	//��������
	StartDownLoadTask();

	return 0;
}

//�滭����
BOOL CGameUpdateFrame::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

	//�滭����
	OnDrawClientArea(pBufferDC,rcClient.Width(),rcClient.Height());

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//������С
VOID CGameUpdateFrame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//������Դ
	RectifyWindow(cx,cy);
}

//��ʾ��Ϣ
VOID CGameUpdateFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//λ�øı�
VOID CGameUpdateFrame::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//�����Ϣ
VOID  CGameUpdateFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}


//�滭��Ϣ
VOID CGameUpdateFrame::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));

	//������Դ
	CPngImage ImageFrameBack;
	ImageFrameBack.LoadImage(AfxGetInstanceHandle(),TEXT("UPDATE_FRAME_BACK"));

	//�滭����
	ImageFrameBack.DrawImage(pDC,0,0);

	//����ͼ��
	CPngImage ImageLogo;
	ImageLogo.LoadImage(AfxGetInstanceHandle(),TEXT("WND_LOGO"));

	//���ر���
	CPngImage ImageTitle;
	ImageTitle.LoadImage(AfxGetInstanceHandle(),TEXT("WND_TITLE"));

	//�滭����
	ImageLogo.DrawImage(pDC,12,1);
	ImageTitle.DrawImage(pDC,35,2);

	//��������
	CPngImage ImageProgressLine;

	//����ͼ��
	ImageProgressLine.LoadImage(AfxGetInstanceHandle(),TEXT("PROGRESS_LINE"));

	//��������
	CRect rcClient(0,0,0,0);
	GetClientRect(rcClient);
	CSize ProgressLineSize(ImageProgressLine.GetWidth(),ImageProgressLine.GetHeight()/2);

	//��������
	int nProgressLeft = (rcClient.Width()-ProgressLineSize.cx)/2;

	//����������
	ImageProgressLine.DrawImage(pDC,nProgressLeft,67,ProgressLineSize.cx,ProgressLineSize.cy,0,0);
	ImageProgressLine.DrawImage(pDC,nProgressLeft,112,ProgressLineSize.cx,ProgressLineSize.cy,0,0);

	//���ƽ���
	if(m_pDownLoadManager!=NULL && m_pDownLoadManager->m_pDownLoadSink!=NULL)
	{
		//��������
		TCHAR szTotalProgress[64] = TEXT("");
		TCHAR szCurrProgress[64] = TEXT("");
		TCHAR szNetSpeed[32] = TEXT("");

		//��������
		DWORD  dwTotalTaskCount = m_pDownLoadManager->m_dwTotalTaskCount;
		DWORD  dwTaskFinished = m_pDownLoadManager->m_dwTaskFinished;
		DWORD  dwCurrTaskIndex = __min(dwTaskFinished+1,dwTotalTaskCount);
		DWORD  dwTotalFileSize = m_pDownLoadManager->m_pDownLoadSink->m_dwTotalFileSize;
		DWORD  dwDownLoadFileSize = m_pDownLoadManager->m_pDownLoadSink->m_dwDownLoadSize;

		//��������
		DWORD dwTotalProgress = 0,dwCurrProgress = 0;

		//���ñ���
		if(dwTotalTaskCount>0) dwTotalProgress = (DWORD)(((double)dwTaskFinished/(double)dwTotalTaskCount)*(double)(ProgressLineSize.cx));
		if(dwTotalFileSize>0)  dwCurrProgress = (DWORD)(((double)dwDownLoadFileSize/(double)dwTotalFileSize)*(double)ProgressLineSize.cx);
		if(dwTotalTaskCount>0) dwTotalProgress += dwCurrProgress/dwTotalTaskCount; 

		//���ƽ���
		ImageProgressLine.DrawImage(pDC,nProgressLeft,67,dwTotalProgress,ProgressLineSize.cy,0,ProgressLineSize.cy);
		ImageProgressLine.DrawImage(pDC,nProgressLeft,112,dwCurrProgress,ProgressLineSize.cy,0,ProgressLineSize.cy);

		//��������
		CFont NewFont;
		VERIFY(NewFont.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("����")));

		//��������
		CFont * pOldFont = pDC->SelectObject(&NewFont);
		pDC->SetTextColor(RGB(255,135,0));

		//��ʽ�ַ�
		_sntprintf(szTotalProgress,CountArray(szTotalProgress),TEXT("%d���ļ���Ҫ����,��ǰ���µ�%d��..."),dwTotalTaskCount,dwCurrTaskIndex);
		_sntprintf(szCurrProgress,CountArray(szCurrProgress),TEXT("�������� %s"),m_pszFileName);
		_sntprintf(szNetSpeed,CountArray(szNetSpeed),TEXT("%d/%d KB"),dwDownLoadFileSize/1024,dwTotalFileSize/1024);

		//������Ϣ
		pDC->TextOut(nProgressLeft+10,52,szTotalProgress);
		pDC->TextOut(nProgressLeft+10,95,szCurrProgress);
		pDC->TextOut(312,95,szNetSpeed);

		//��ԭ����
		pDC->SelectObject(pOldFont);
		NewFont.DeleteObject();
	}

	return;
}

//��������
VOID CGameUpdateFrame::StartDownLoadTask()
{
	//ָ���ж�
	if(m_pDownLoadManager==NULL)
	{
		try
		{
			//����ָ��
			m_pDownLoadManager = new CDownLoadManager();
			ASSERT(m_pDownLoadManager!=NULL);
			if(m_pDownLoadManager==NULL) return;

			//��������
			m_pDownLoadManager->Create(NULL,NULL,WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,IDC_DOWNLOAD_MANAGER);
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	//��������
	if(m_pDownLoadManager!=NULL)
	{
		m_pDownLoadManager->m_pGameUpdateFrame = this;
		m_pDownLoadManager->ReadDownLoadInfo();
		m_pDownLoadManager->StartDownLoadTask();
	}

	return;
}

//������Դ
VOID CGameUpdateFrame::RectifyWindow(INT nWidth, INT nHeight)
{
	//�����ж�
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//���ð�ť		
		m_btClose.SetWindowPos(NULL,354,7,0,0,SWP_NOZORDER|SWP_NOSIZE);
		//m_btSelfUpdate.SetWindowPos(NULL,300,7,0,0,SWP_NOZORDER|SWP_NOSIZE);

		//���λ��
		CRect rcFrame;
		rcFrame.SetRect(SHADOW_CX,SHADOW_CY,nWidth-SHADOW_CX,nHeight-SHADOW_CY);

		//��������
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,ROUND_CX,ROUND_CY);

		//��������
		SetWindowRgn(RegionWindow,TRUE);

		//�ֲ㴰��
		if (m_SkinLayered.m_hWnd!=NULL)
		{
			//��������
			CImage ImageBuffer;
			ImageBuffer.Create(nWidth,nHeight,32);

			//���� DC
			CImageDC ImageDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(ImageDC);

			//�滭����
			OnDrawClientArea(pBufferDC,nWidth,nHeight);

			//���·ֲ�
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}
}

//�رհ�ť
VOID CGameUpdateFrame::OnBnClickedClose()
{
	PostMessage(WM_CLOSE,0,0);
}

//�ֶ���ť
VOID CGameUpdateFrame::OnBnClickedSelfUpdate()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("http://www.foxuc.cn/"));

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	PostMessage(WM_CLOSE,0,0);
}
//////////////////////////////////////////////////////////////////////////////////
