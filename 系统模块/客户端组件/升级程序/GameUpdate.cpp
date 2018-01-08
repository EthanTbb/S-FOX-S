#include "Stdafx.h"
#include "Resource.h"
#include "GameUpdate.h"
#include "UpdateFrame.h"
#include "DlgUpdateCheck.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameUpdateApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameUpdateApp::CGameUpdateApp()
{
	//设置变量
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//初始函数
BOOL CGameUpdateApp::InitInstance()
{
	__super::InitInstance();

	//常量定义
	LPCTSTR szUpdateClass = TEXT("WHGameUpdate");

#ifndef _DEBUG
	//变量定义
	bool bUpdateExist=false;
	CMutex Mutex(FALSE,szUpdateClass,NULL);
	if (Mutex.Lock(0)==FALSE) bUpdateExist=true;

	//搜索窗口
	CWnd * pWndGameUpdate=CWnd::FindWindow(szUpdateClass,NULL);
	if (pWndGameUpdate!=NULL) 
	{
		//设置变量
		bUpdateExist=true;

		//还原窗口
		if (pWndGameUpdate->IsIconic()) 
		{
			pWndGameUpdate->ShowWindow(SW_RESTORE);
		}

		//激活窗口
		pWndGameUpdate->SetActiveWindow();
		pWndGameUpdate->BringWindowToTop();
		pWndGameUpdate->SetForegroundWindow();
	}

	//结果处理
	if (bUpdateExist==true) return FALSE;
#endif

	//环境配置
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//初始化Socket
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		::AfxMessageBox(TEXT("Socket 初始化失败!"));
		return FALSE;
	}

	//创建对象
	m_pIDispatch=new CImpIDispatch;
	m_pCustomOccManager=new CCustomOccManager;
	AfxEnableControlContainer(m_pCustomOccManager); 

	//加载链接
	CGlobalWebLink GlobalWebLink;
	GlobalWebLink.LoadPaltformLink();

	//更新检查
	CDlgUpdateCheck DlgUpdateCheck;
	DlgUpdateCheck.SetWindowDescribe(TEXT("正在检查更新,请稍候..."));
	INT_PTR nResultID = DlgUpdateCheck.DoModal();
	if(nResultID==IDOK) 
	{
		//获取目录
		TCHAR szMouduleFileName[MAX_PATH]=TEXT("");		
		CWHService::GetWorkDirectory(szMouduleFileName,CountArray(szMouduleFileName));

		//构造地址
		TCHAR szMainSetupPath[MAX_PATH]=TEXT("");
		_sntprintf(szMainSetupPath,CountArray(szMainSetupPath),TEXT("%s/GamePlaza.exe"),szMouduleFileName);
		
		//启动宿主程序
		ShellExecute(NULL,TEXT("open"),szMainSetupPath,TEXT("UPDATED"),NULL,true);

		return FALSE;
	}
	
	//数目判断
	if(DlgUpdateCheck.GetUpdateCount()==0)
	{
		return FALSE;
	}

	//变量定义
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//注册窗口
	WndClasss.style=CS_DBLCLKS;
	WndClasss.hIcon=LoadIcon(IDI_DOWNLOAD);
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=szUpdateClass;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//创建窗口
	CGameUpdateFrame * pGameUpdateFrame = new CGameUpdateFrame();
	pGameUpdateFrame->Create(szUpdateClass,szProduct,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));

	//设置变量
	m_pMainWnd=pGameUpdateFrame;

	return TRUE;
}

//退出函数
INT CGameUpdateApp::ExitInstance()
{
	//终止Socket
	WSACleanup();

	//删除对象
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//应用程序对象
CGameUpdateApp theApp;

//////////////////////////////////////////////////////////////////////////////////
