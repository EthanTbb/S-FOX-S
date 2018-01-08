#include "StdAfx.h"
#include "DlgDownLoad.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_ITEM				100									//���̼��
#define IDI_UPDATE_STATUS			101									//����״̬

//ʱ���ʶ
#define IDI_CHECK_PROCESS			200									//���̼��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaHallGameDownLoadSink, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaHallGameDownLoadSink::CPlazaHallGameDownLoadSink()
{
	//�����ٶ�
	m_dwDownSpeed=0L;
	m_lLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//�ļ���Ϣ
	m_lDownLoadSize=0L;
	m_lTotalFileSize=0L;

	//״̬����
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//��������
	m_wKindID=0;
	m_wServerID=0;
	ZeroMemory(m_szClientName,sizeof(m_szClientName));

	//�ں���Ϣ
	m_pDlgDownLoad=NULL;
	ZeroMemory(&m_ProcessInfoMation,sizeof(m_ProcessInfoMation));

	return;
}

//��������
CPlazaHallGameDownLoadSink::~CPlazaHallGameDownLoadSink()
{
}

//�ӿڲ�ѯ
VOID * CPlazaHallGameDownLoadSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//�ر�����
bool CPlazaHallGameDownLoadSink::CloseDownLoad()
{
	//�رմ���
	DestroyWindow();

	//�ر�����
	m_DownLoad.CloseDownLoad();

	//����״̬
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//�ر��ļ�
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();

	//����״̬
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//ִ������
bool CPlazaHallGameDownLoadSink::PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//Ч��״̬
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//�����ٶ�
	m_dwDownSpeed=0L;
	m_lLastCalcSize=0L;
	m_dwLastCalcTime=GetTickCount();

	//���ñ���
	m_wKindID=wKindID;
	m_wServerID=wServerID;
	m_cbDownLoadStatus=DOWN_LOAD_LINK;
	lstrcpyn(m_szClientName,pszClientName,CountArray(m_szClientName));

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s/DownLoad.aspx?KindID=%ld"),pGlobalWebLink->GetPlatformLink(),wKindID);

	//����״̬
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	//ִ������
	m_DownLoad.PerformDownLoad(szDownLoadUrl,0,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//�����쳣
bool CPlazaHallGameDownLoadSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//����״̬
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//����״̬
bool CPlazaHallGameDownLoadSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//����״̬
	switch (DownLoadStatus)
	{
	case DownLoadStatus_Service:		//����״̬
		{
			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_READ;

			//����״̬
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

			break;
		}
	case DownLoadStatus_Conclude:		//���״̬
		{
			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_SETUP;

			//��װ�ļ�
			if (PerformInstall()==false)
			{
				//�ر�����
				CloseDownLoad();

				return false;
			}

			//����״̬
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

			break;
		}
	}

	return true;
}

//��������
bool CPlazaHallGameDownLoadSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//д���ļ�
	m_lDownLoadSize+=wStreamSize;
	m_DownFile.Write(pcbMailData,wStreamSize);

	//�����ٶ�
	if (GetTickCount()>=(m_dwLastCalcTime+2000L))
	{
		//�����ٶ�
		m_dwDownSpeed=DWORD((m_lDownLoadSize-m_lLastCalcSize)/(GetTickCount()-m_dwLastCalcTime));

		//���ñ���
		m_dwLastCalcTime=GetTickCount();
		m_lLastCalcSize=m_lDownLoadSize;
	}
	
	//����״̬
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//������Ϣ
bool CPlazaHallGameDownLoadSink::OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//���ñ���
	m_lTotalFileSize+=dwTotalFileSize;

	//�����ļ�
	if (CreateDownLoadFile(pszLocation)==false)
	{
		//�ر�����
		CloseDownLoad();

		return false;
	}

	return true;
}

//ִ�а�װ
bool CPlazaHallGameDownLoadSink::PerformInstall()
{
	//�����ļ�
	TCHAR szTargetName[MAX_PATH]=TEXT("");
	CString strSourceName=m_DownFile.GetFilePath();

	//��������
	INT nTailCount=lstrlen(TEXT(".WHD"));
	INT nSourceLength=lstrlen(strSourceName);

	//��������
	szTargetName[nSourceLength-nTailCount]=0;
	CopyMemory(szTargetName,(LPCTSTR)strSourceName,sizeof(TCHAR)*(nSourceLength-nTailCount));

	//�ر��ļ�
	m_DownFile.Close();

	//ɾ���ļ�
	DeleteFile(szTargetName);

	//�ƶ��ļ�
	if (MoveFileWithProgress(strSourceName,szTargetName,NULL,NULL,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}
	//��������
	TCHAR szCommandLine[MAX_PATH];
	TCHAR szCommandName[]=TEXT("VERYSILENT");
	if(m_wKindID != 0)
	{
		//_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s /VERYSILENT"),szTargetName);
		_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s /%s"),szTargetName,szCommandName);
	}
	else
	{
		_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s"),szTargetName);
	}


	//��������
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//��������
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWDEFAULT;
	if (CreateProcess(NULL,szCommandLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_ProcessInfoMation)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
	}

	//����ʱ��
	SetTimer(IDI_CHECK_PROCESS,100,NULL);

	return true;
}

//�����ļ�
bool CPlazaHallGameDownLoadSink::CreateDownLoadFile(LPCTSTR pszLocation)
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
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\%s.WHD"),szDownLoadPath,pszFileName);

	//����Ŀ¼
	CreateDirectory(szDownLoadPath,NULL);

	//�����ļ�
	if (m_DownFile.Open(szFilePath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//ʱ����Ϣ
VOID CPlazaHallGameDownLoadSink::OnTimer(UINT nIDEvent)
{
	//���̼��
	if (nIDEvent==IDI_CHECK_PROCESS)
	{
		//״̬Ч��
		if (m_ProcessInfoMation.hProcess==NULL)
		{
			//�������
			ASSERT(FALSE);

			//�ر�����
			CloseDownLoad();

			return;
		}

		//���̼��
		if (WaitForSingleObject(m_ProcessInfoMation.hProcess,0L)==WAIT_OBJECT_0)
		{
			//�رմ���
			DestroyWindow();

			//����״̬
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//����״̬
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
