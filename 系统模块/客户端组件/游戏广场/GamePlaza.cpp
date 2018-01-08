#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

#define MAX_SHARE_BUFFER			64

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePlazaApp::CGamePlazaApp()
{
	//���ñ���
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//��ʼ����
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();
	//CRichEditUI::Initialize();



#ifndef _DEBUG
#ifndef RELEASE_MANAGER

	//��������
	bool bProcessExit=false;

	//�����ж�
	CMutex Mutex(FALSE,szPlazaClass,NULL);
	if (Mutex.Lock(0)==FALSE) bProcessExit=true;

	//��������
	CWnd * pWndGamePlaza=CWnd::FindWindow(szPlazaClass,NULL);
	if (pWndGamePlaza!=NULL) 
	{
		//���ñ���
		bProcessExit=true;

		//��ԭ����
		if (pWndGamePlaza->IsIconic()) 
		{
			pWndGamePlaza->ShowWindow(SW_RESTORE);
		}

		//�����
		pWndGamePlaza->SetActiveWindow();
		pWndGamePlaza->BringWindowToTop();
		pWndGamePlaza->SetForegroundWindow();
	}

	//�������
	if (bProcessExit==true) return FALSE;

#endif
#endif

	//��������
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//�������
	srand((DWORD)time(NULL));

	//���ñ���
	SetRegistryKey(szProductKey);

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��װĿ¼
	CWHRegKey RegInstallPath;
	RegInstallPath.OpenRegKey(TEXT(""),true);
	RegInstallPath.WriteString(TEXT("InstallPath"),szDirectory);

	//��������	
	m_ParameterGlobal.LoadParameter();
	m_GlobalWebLink.LoadPaltformLink();
	//m_UserInformation.LoadInformation();
	m_ExpressionManager.LoadExpression();

	//����Ŀ¼
	SetCurrentDirectory(szDirectory);
	m_SkinResourceManager.SetSkinResource(m_ParameterGlobal.m_SkinRenderInfo);

#ifndef _DEBUG 
#ifndef RELEASE_MANAGER
	//�����ж�
	if(lstrcmp(AfxGetApp()->m_lpCmdLine,TEXT("UPDATED"))!=0)
	{
		//Ŀ��Ŀ¼
		TCHAR szTargetPath[MAX_PATH]=TEXT("");
		_sntprintf(szTargetPath,CountArray(szTargetPath),TEXT("%s\\Update.exe"),szDirectory);

		//��������
		ShellExecute(NULL,TEXT("open"),szTargetPath,TEXT(""),NULL,true);

		return FALSE;
	}
#endif
#endif



	//��������
	m_pIDispatch=new CImpIDispatch;
	m_pCustomOccManager=new CCustomOccManager;
	AfxEnableControlContainer(m_pCustomOccManager); 

	//�������
	if (m_FaceItemControlModule.CreateInstance()==false) return false;
	if (m_UserOrderParserModule.CreateInstance()==false) return false;
	if (m_PlatformResourceModule.CreateInstance()==false) return false;

	//�û�Ԫ��
	m_UserItemElement.SetUserOrderParser(m_UserOrderParserModule.GetInterface());
	m_UserItemElement.SetFaceItemControl(m_FaceItemControlModule.GetInterface());

	//ƽ̨��Դ
	m_PlatformResourceModule->LoadDefaultResource(TEXT("PlatformConfig.xml"),GetModuleHandle(NULL));

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style=CS_DBLCLKS;
	WndClasss.hIcon=LoadIcon(IDR_MAINFRAME);
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=szPlazaClass;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//��������
	CPlatformFrame * pPlatformFrame=new CPlatformFrame();
	pPlatformFrame->Create(szPlazaClass,szProduct,WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_SYSMENU,CRect(0,0,0,0));

	//���ñ���
	m_pMainWnd=pPlatformFrame;

	//��������
	m_D3DSound.CreateD3DSound(m_pMainWnd->GetSafeHwnd());

	return TRUE;
}

//�˳�����
INT CGamePlazaApp::ExitInstance()
{
	//��������
	m_ParameterGlobal.SaveParameter();

	//ɾ������
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);
	//CRichEditUI::Uninitialize();
	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////////////
