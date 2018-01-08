#include "StdAfx.h"
#include "DlgUpdateCheck.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//ʱ���ʶ
#define IDT_STATUS					20									//��һ״̬
#define IDT_STATUS_SPEED            3                                   //�ڶ�״̬
#define TIMER_STATUS				15									//״̬ʱ��
#define TIMER_STATUS_SPEED		    5									//״̬ʱ��

//ʱ���ʶ
#define IDI_CHECK_PROCESS			200									//���̼��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUpdateCheckSink, CWnd)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgUpdateCheck, CDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//�ֲ���Ϣ
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()

	//�Զ���Ϣ
	ON_MESSAGE(WM_DOWNLOAD_FINISH,OnDownLoadConfigFinish)

END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUpdateCheckSink::CUpdateCheckSink()
{
	//�ļ���Ϣ
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;
	m_nUpdateCount=0;

	//״̬����
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//�ں���Ϣ
	m_pFileData=NULL;
	m_pDlgUpdateCheck=NULL;

	return;
}

//��������
CUpdateCheckSink::~CUpdateCheckSink()
{
	//�ͷ���Դ
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);
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
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//�ͷ���Դ
	if(m_pFileData!=NULL) SafeDeleteArray(m_pFileData);

	return true;
}

//ִ������
bool CUpdateCheckSink::PerformDownLoad(LPCTSTR pszDownLoadFile)
{
	//Ч��״̬
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_LINK;

	//ִ������
	m_DownLoad.PerformDownLoad(pszDownLoadFile,QUERY_ME_INTERFACE(IUnknownEx),0);

	return true;
}

//�����쳣
bool CUpdateCheckSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//���ñ���
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//�����쳣
	if(m_pDlgUpdateCheck!=NULL) m_pDlgUpdateCheck->OnEventDownLoadError(IDOK);	

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
			m_cbDownLoadStatus=DOWN_LOAD_SETUP;

			//�������
			if(m_pDlgUpdateCheck!=NULL) m_pDlgUpdateCheck->OnEventCheckFinish(PerformCheckSum());			

			break;
		}
	}

	return true;
}

//��������
bool CUpdateCheckSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//д������
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

//ִ��У��
bool CUpdateCheckSink::PerformCheckSum()
{
	//��������
	DWORD dwUnCompressSize=0;
	BYTE * pUnCompressData=NULL;

	try
	{
		//�����С
		dwUnCompressSize=m_dwTotalFileSize*5;

		//�����ڴ�		
		pUnCompressData=new BYTE[dwUnCompressSize];
		if(pUnCompressData==NULL) throw TEXT("�ڴ治��,��Դ����ʧ�ܣ�");

		//���ݽ�ѹ��
		dwUnCompressSize=CWHService::UnCompressData(m_pFileData,m_dwTotalFileSize,pUnCompressData,dwUnCompressSize);

		//��ѹʧ��
		if(dwUnCompressSize==0) return false;
	}
	catch(...)
	{
		ASSERT(FALSE);
		SafeDeleteArray(pUnCompressData);

		return false;
	}	

	//��������
	WORD wDataIndex=0;
	tagUpdateInfo UpdateInfo;
	tagUpdateItem UpdateItem;

	//��ȡ����
	UpdateInfo.wStructSize=*(WORD *)pUnCompressData;	
	CopyMemory(&UpdateInfo,pUnCompressData,UpdateInfo.wStructSize);
	wDataIndex += UpdateInfo.wStructSize;	

	//��������
	CString strFileCheckSum;
	TCHAR szFilePath[MAX_PATH]=TEXT("");	

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//Ŀ��Ŀ¼
	TCHAR szConfigSavePath[MAX_PATH]=TEXT("");
	_sntprintf(szConfigSavePath,CountArray(szConfigSavePath),TEXT("%s\\Update.WHD"),szWorkDirectory);

	//��������
	TCHAR  szFileName[MAX_PATH]=TEXT("");
	TCHAR  szFileCheckSum[64]=TEXT("");
	TCHAR  szExeName[MAX_PATH]=TEXT("");

	//��������
	CString  szKeyText,szLocalFileCheckSum;
	
	//��ʽ�ַ�
	_sntprintf(szExeName,CountArray(szExeName),TEXT("%s.exe"),AfxGetApp()->m_pszExeName);

	//��������
	WORD wUpdateIndex=0;

	//�ļ�У��
	for(INT_PTR nIndex=0;nIndex<UpdateInfo.wUpdateCount;nIndex++)
	{
		//�������
		ZeroMemory(&UpdateItem,sizeof(UpdateItem));

		//��ȡ����
		UpdateItem.wStructSize=*(WORD *)&pUnCompressData[wDataIndex];	
		CopyMemory(&UpdateItem,&pUnCompressData[wDataIndex],UpdateItem.wStructSize);

		//���ñ���
		wDataIndex += UpdateItem.wStructSize;

		//��������
		CString  szFileNameEx(UpdateItem.szFileName);

		//�����Լ�
		if(lstrcmp(szExeName,szFileNameEx.Right(lstrlen(szExeName)))==0)
		{
			//��ȡУ��
			_sntprintf(szFilePath,sizeof(szFilePath),TEXT("%s/%s"),szWorkDirectory,szExeName);
		}
		else
		{
			//��ȡУ��
			_sntprintf(szFilePath,sizeof(szFilePath),TEXT("%s/%s"),szWorkDirectory,CA2CT(UpdateItem.szFileName));
		}
		
		//��ȡУ���
		szLocalFileCheckSum =CWHMD5Checksum::GetMD5(szFilePath);

		//У��Ƚ�
		if(lstrcmp(szLocalFileCheckSum,CA2CT(UpdateItem.szFileCheckSum))!=0)
		{
			//д���ļ�
			szKeyText.Format(TEXT("FileIndex%d"),m_nUpdateCount);			
			WritePrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,CA2CT(UpdateItem.szFileName),szConfigSavePath);

			//д���ļ�
			szKeyText.Format(TEXT("FileCheckNum%d"),m_nUpdateCount++);			
			WritePrivateProfileString(TEXT("UPDATEFILELIST"),szKeyText,CA2CT(UpdateItem.szFileCheckSum),szConfigSavePath);
		}
	}

	//��Ŀ�ж�
	if(m_nUpdateCount>0)
	{
		//��������
		CString szUpdateFileCount;
		szUpdateFileCount.Format(TEXT("%d"),m_nUpdateCount);	

		//д���ļ�
		WritePrivateProfileString(TEXT("UPDATEFILELIST"),TEXT("UPDATEFILECOUNT"),szUpdateFileCount,szConfigSavePath);
		WritePrivateProfileString(TEXT("UPDATEFILELIST"),TEXT("FILENETADDRESS"),CA2CT(UpdateInfo.szDownLoadAddress),szConfigSavePath);
	}

	//�ͷ��ڴ�
	SafeDeleteArray(pUnCompressData);

	return m_nUpdateCount>0;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUpdateCheck::CDlgUpdateCheck() : CDialog(IDD_DLG_UPDATE_CHECK)
{
	//״̬����
	m_uImageIndex=0;

	//���ݱ���
	m_pUpdateCheckSink = NULL;

	m_bCheckFinish = FALSE;

	m_bNeedUpdate = FALSE;

	return;
}

//��������
CDlgUpdateCheck::~CDlgUpdateCheck()
{
	//�ͷŶ���
	SafeDelete(m_pUpdateCheckSink);
}

//�ؼ���
VOID CDlgUpdateCheck::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��Ϣ����
BOOL CDlgUpdateCheck::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgUpdateCheck::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(szProduct);

	//���ÿؼ�
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	//������Դ
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));

	//���ô�С
	CSize SizeWindow(ImageStatus.GetWidth(),ImageStatus.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//���д���
	CenterWindow();

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=8;
	rcUnLayered.left=8;
	rcUnLayered.right=rcWindow.Width()-8;
	rcUnLayered.bottom=rcWindow.Height()-8;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageStatus,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	//���´���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	//����Ŀ¼
	TCHAR szMouduleFileName[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szMouduleFileName,CountArray(szMouduleFileName));

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	m_pszDownLoadFile.Format(TEXT("%s/DownLoad/Client/Update.txt"),pGlobalWebLink->GetUpdateLink());

	//��������
	DownLoadConfig(m_pszDownLoadFile);

	return TRUE;
}

//������Ŀ
INT CDlgUpdateCheck::GetUpdateCount()
{
	//ָ���ж�
	if(m_pUpdateCheckSink==NULL) return 0;

	return m_pUpdateCheckSink->m_nUpdateCount;
}

//��������
VOID CDlgUpdateCheck::DownLoadConfig(LPCTSTR pszConfigName)
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
		};
	}

	//��ʼ����
	if(m_pUpdateCheckSink!=NULL) 
	{
		m_pUpdateCheckSink->m_pDlgUpdateCheck=this;
		m_pUpdateCheckSink->PerformDownLoad(pszConfigName);
	}

	return;
}

//������Ϣ
VOID CDlgUpdateCheck::SetWindowDescribe(LPCTSTR pszDescribe)
{
	//��������
	m_strDescribe=pszDescribe;

	return;
}

//֪ͨ�¼�
VOID CDlgUpdateCheck::OnEventCheckFinish(BOOL bNeedUpdate)
{
	m_bNeedUpdate = bNeedUpdate;
	m_bCheckFinish = TRUE;
}

//�����¼�
VOID CDlgUpdateCheck::OnEventDownLoadError(int nResult)
{
	//����ģ̬
	EndDialog(nResult);
}

//ʱ����Ϣ
VOID CDlgUpdateCheck::OnTimer(UINT nIDEvent)
{
	//���ý���
	if (nIDEvent==IDT_STATUS)
	{
		//��������
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		if (m_bCheckFinish&&m_uImageIndex>30)
		{
			KillTimer(nIDEvent);
			//���ÿؼ�
			SetTimer(IDT_STATUS_SPEED,TIMER_STATUS_SPEED,NULL);

			if (m_bNeedUpdate)
			{
				SetWindowDescribe(TEXT("׼������..."));
			}
			else
			{
				SetWindowDescribe(TEXT("������,��������..."));
			}
			
		}

		return;
	}
	else if ( nIDEvent = IDT_STATUS_SPEED)
	{
		//��������
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+5L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		if (m_bCheckFinish && m_uImageIndex>=100)
		{
			KillTimer(nIDEvent);
			//����ģ̬
			EndDialog(m_bNeedUpdate==TRUE?IDCANCEL:IDOK);
		}

	}

	__super::OnTimer(nIDEvent);
}

//�滭����
BOOL CDlgUpdateCheck::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);

	//�滭����
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));
	ImageStatus.DrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0);

	//������Դ
	CPngImage ImageStatusLine;
	ImageStatusLine.LoadImage(AfxGetInstanceHandle(),TEXT("PROGRESS_LINE"));

	//��ȡ��С
	CSize SizeStatusLine;
	SizeStatusLine.SetSize(ImageStatusLine.GetWidth(),ImageStatusLine.GetHeight()/2);

	//�滭����
	ImageStatusLine.DrawImage(pBufferDC,(rcClient.Width()-SizeStatusLine.cx)/2,50,SizeStatusLine.cx,SizeStatusLine.cy,0,0);
	ImageStatusLine.DrawImage(pBufferDC,(rcClient.Width()-SizeStatusLine.cx)/2,50,SizeStatusLine.cx*m_uImageIndex/100L,SizeStatusLine.cy,0,SizeStatusLine.cy);


	//����λ��
	CRect rcDescribe;
	rcDescribe.SetRect(140,30,140+184,30+18);

	//��������
	CFont NewFont;
	VERIFY(NewFont.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("����")));
	CFont * pOldFont = pBufferDC->SelectObject(&NewFont);

	//�滭����
	pBufferDC->SetTextColor(RGB(255,135,0));
	pBufferDC->DrawText(m_strDescribe,rcDescribe,DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);	

	//��ԭ����
	pBufferDC->SelectObject(pOldFont);
	NewFont.DeleteObject();	

	return TRUE;
}

//��ʾ��Ϣ
VOID CDlgUpdateCheck::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//�����Ϣ
VOID CDlgUpdateCheck::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//λ�øı�
VOID CDlgUpdateCheck::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

LRESULT CDlgUpdateCheck::OnDownLoadConfigFinish(WPARAM wParam, LPARAM lParam)
{
	//�ļ�У��
	if(m_pUpdateCheckSink!=NULL) m_pUpdateCheckSink->PerformCheckSum();

	return 0;
}