#include "Stdafx.h"
#include "Resource.h"
#include "GameUpdate.h"
#include "UpdateFrame.h"
#include "DlgUpdateCheck.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameUpdateApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameUpdateApp::CGameUpdateApp()
{
	//���ñ���
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//��ʼ����
BOOL CGameUpdateApp::InitInstance()
{
	__super::InitInstance();

	//��������
	LPCTSTR szUpdateClass = TEXT("WHGameUpdate");

#ifndef _DEBUG
	//��������
	bool bUpdateExist=false;
	CMutex Mutex(FALSE,szUpdateClass,NULL);
	if (Mutex.Lock(0)==FALSE) bUpdateExist=true;

	//��������
	CWnd * pWndGameUpdate=CWnd::FindWindow(szUpdateClass,NULL);
	if (pWndGameUpdate!=NULL) 
	{
		//���ñ���
		bUpdateExist=true;

		//��ԭ����
		if (pWndGameUpdate->IsIconic()) 
		{
			pWndGameUpdate->ShowWindow(SW_RESTORE);
		}

		//�����
		pWndGameUpdate->SetActiveWindow();
		pWndGameUpdate->BringWindowToTop();
		pWndGameUpdate->SetForegroundWindow();
	}

	//�������
	if (bUpdateExist==true) return FALSE;
#endif

	//��������
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//��ʼ��Socket
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		::AfxMessageBox(TEXT("Socket ��ʼ��ʧ��!"));
		return FALSE;
	}

	//��������
	m_pIDispatch=new CImpIDispatch;
	m_pCustomOccManager=new CCustomOccManager;
	AfxEnableControlContainer(m_pCustomOccManager); 

	//��������
	CGlobalWebLink GlobalWebLink;
	GlobalWebLink.LoadPaltformLink();

	//���¼��
	CDlgUpdateCheck DlgUpdateCheck;
	DlgUpdateCheck.SetWindowDescribe(TEXT("���ڼ�����,���Ժ�..."));
	INT_PTR nResultID = DlgUpdateCheck.DoModal();
	if(nResultID==IDOK) 
	{
		//��ȡĿ¼
		TCHAR szMouduleFileName[MAX_PATH]=TEXT("");		
		CWHService::GetWorkDirectory(szMouduleFileName,CountArray(szMouduleFileName));

		//�����ַ
		TCHAR szMainSetupPath[MAX_PATH]=TEXT("");
		_sntprintf(szMainSetupPath,CountArray(szMainSetupPath),TEXT("%s/GamePlaza.exe"),szMouduleFileName);
		
		//������������
		ShellExecute(NULL,TEXT("open"),szMainSetupPath,TEXT("UPDATED"),NULL,true);

		return FALSE;
	}
	
	//��Ŀ�ж�
	if(DlgUpdateCheck.GetUpdateCount()==0)
	{
		return FALSE;
	}

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style=CS_DBLCLKS;
	WndClasss.hIcon=LoadIcon(IDI_DOWNLOAD);
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=szUpdateClass;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//��������
	CGameUpdateFrame * pGameUpdateFrame = new CGameUpdateFrame();
	pGameUpdateFrame->Create(szUpdateClass,szProduct,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));

	//���ñ���
	m_pMainWnd=pGameUpdateFrame;

	return TRUE;
}

//�˳�����
INT CGameUpdateApp::ExitInstance()
{
	//��ֹSocket
	WSACleanup();

	//ɾ������
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CGameUpdateApp theApp;

//////////////////////////////////////////////////////////////////////////////////
