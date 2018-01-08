#include "Stdafx.h"
#include "Resource.h"
#include "AndroidManager.h"
#include "PlatformFrame.h"

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

	//����Ŀ¼
	SetCurrentDirectory(szDirectory);
	m_SkinResourceManager.SetSkinResource(m_ParameterGlobal.m_SkinRenderInfo);

	//�������
	if (m_PlatformResourceModule.CreateInstance()==false) return false;

	//ƽ̨��Դ
	m_PlatformResourceModule->LoadDefaultResource(TEXT("PlatformConfig.xml"),GetModuleHandle(NULL));

	//��������
	m_pIDispatch=new CImpIDispatch;
	m_pCustomOccManager=new CCustomOccManager;
	AfxEnableControlContainer(m_pCustomOccManager); 

	//��������
	CPlatformFrame * pPlatformFrame = new CPlatformFrame;
	pPlatformFrame->Create(IDD_PLATFORM_FRAME,NULL);

	//���ñ���
	m_pMainWnd=pPlatformFrame;

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

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////////////
