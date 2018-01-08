#include "Stdafx.h"
#include "Resource.h"
#include "AndroidManager.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePlazaApp::CGamePlazaApp()
{
	//设置变量
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//初始函数
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

	//环境配置
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//随机种子
	srand((DWORD)time(NULL));

	//设置表名
	SetRegistryKey(szProductKey);

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//安装目录
	CWHRegKey RegInstallPath;
	RegInstallPath.OpenRegKey(TEXT(""),true);
	RegInstallPath.WriteString(TEXT("InstallPath"),szDirectory);

	//设置目录
	SetCurrentDirectory(szDirectory);
	m_SkinResourceManager.SetSkinResource(m_ParameterGlobal.m_SkinRenderInfo);

	//创建组件
	if (m_PlatformResourceModule.CreateInstance()==false) return false;

	//平台资源
	m_PlatformResourceModule->LoadDefaultResource(TEXT("PlatformConfig.xml"),GetModuleHandle(NULL));

	//创建对象
	m_pIDispatch=new CImpIDispatch;
	m_pCustomOccManager=new CCustomOccManager;
	AfxEnableControlContainer(m_pCustomOccManager); 

	//建立窗口
	CPlatformFrame * pPlatformFrame = new CPlatformFrame;
	pPlatformFrame->Create(IDD_PLATFORM_FRAME,NULL);

	//设置变量
	m_pMainWnd=pPlatformFrame;

	return TRUE;
}

//退出函数
INT CGamePlazaApp::ExitInstance()
{
	//保存配置
	m_ParameterGlobal.SaveParameter();

	//删除对象
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//应用程序对象
CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////////////
