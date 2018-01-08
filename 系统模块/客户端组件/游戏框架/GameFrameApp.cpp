#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameApp.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameApp::CGameFrameApp()
{
	//���ñ���
	m_pGameFrameWnd=NULL;
	m_pGameFrameEngine=NULL;

	//���ñ���
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//��������
CGameFrameApp::~CGameFrameApp()
{
}

//���ú���
BOOL CGameFrameApp::InitInstance()
{
	__super::InitInstance();

	//�ڲ���ʼ��
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	try
	{
		//�������
		if (m_PlatformResourceModule.CreateInstance()==false) return false;
		m_PlatformResourceModule->LoadDefaultResource(TEXT("PlatformConfig.xml"),GetModuleHandle(GAME_FRAME_DLL_NAME));

		//��������
		ASSERT(CGlobalUnits::GetInstance()!=NULL);
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

		//ȫ�ֵ�Ԫ
		bool bSuccess=pGlobalUnits->InitGlobalUnits();
		if (bSuccess==false) throw TEXT("��Ϸ������ʼ��ʧ��");

		//��������	
		m_GlobalWebLink.LoadPaltformLink();		

		//��������
		m_pGameFrameWnd=GetGameFrameWnd();
		if (m_pGameFrameWnd==NULL) throw TEXT("��Ϸ��ܴ��ڴ���ʧ��");

		//��������
		m_pGameFrameEngine=GetGameFrameEngine(VERSION_FRAME_SDK);
		if (m_pGameFrameEngine==NULL) throw TEXT("��Ϸ������󴴽�ʧ��");		

		//��������
		m_pIDispatch=new CImpIDispatch;
		m_pCustomOccManager=new CCustomOccManager;
		AfxEnableControlContainer(m_pCustomOccManager); 

		//�������
		m_pMainWnd=m_pGameFrameWnd;
		m_pGameFrameWnd->Create(NULL,NULL,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));

		//��������
		m_D3DSound.CreateD3DSound(m_pMainWnd->GetSafeHwnd());

		//��������
		if (m_GameFrameService.CreateGameFrameService()==false)
		{
			throw TEXT("�ں����洴��ʧ��");
		}

		//���ӹ㳡
		IClientKernel * pIClientKernel=pGlobalUnits->m_ClientKernelModule.GetInterface();
		if ((pIClientKernel!=NULL)&&(pIClientKernel->IsSingleMode()==false)) pIClientKernel->CreateConnect();

		return TRUE;
	}
	catch (LPCTSTR pszMesssage)
	{
		//������Ϣ
		CString strBuffer;
		strBuffer.Format(TEXT("���� [ %s ] ��Ϸ��������ʧ�ܡ�"),pszMesssage);

		//��ʾ��Ϣ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��Ϸ���"),strBuffer,MB_ICONSTOP,30L);

		//ɾ������
		SafeRelease(m_pGameFrameWnd);
		SafeRelease(m_pGameFrameEngine);

		return FALSE;
	}

	return TRUE;
}

//�˳�����
BOOL CGameFrameApp::ExitInstance()
{
	//��Ƶ����
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	if (pVideoServiceManager!=NULL) pVideoServiceManager->LogoutVideoSystem();

	//ɾ������
	SafeRelease(m_pGameFrameEngine);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//�ر�����
	IClientKernel * pIClientKernel=pGlobalUnits->m_ClientKernelModule.GetInterface();
	if ((pIClientKernel!=NULL)&&(pIClientKernel->IsSingleMode()==false)) pIClientKernel->IntermitConnect();

	//ע�����
	pGlobalUnits->UnInitGlobalUnits();

	//ɾ������
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//���к���
int CGameFrameApp::Run()
{
	return __super::Run();

	ASSERT_VALID(this);
	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	//��������
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ѯ�ӿ�
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//��Ϣѭ��
	for (;;)
	{
		while (bIdle && !::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
		{
			if (!OnIdle(lIdleCount++)) bIdle = FALSE;
            if (pIGameFrameView) pIGameFrameView->RenderGameView();
		}

		do
		{
			if (!PumpMessage()) return ExitInstance();

			if (IsIdleMessage(&(pState->m_msgCur)))
			{
				bIdle = TRUE;
				lIdleCount = 0;				
			}

		} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
	}
}

//��������
CGameFrameWnd * CGameFrameApp::GetGameFrameWnd()
{
	return new CGameFrameWnd;
}

//////////////////////////////////////////////////////////////////////////////////
