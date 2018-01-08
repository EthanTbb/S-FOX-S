#include "StdAfx.h"
#include "Platformresourcemanager.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CPlatformResourceManager::CPlatformResourceManager(void)
{
}

//析构函数
CPlatformResourceManager::~CPlatformResourceManager(void)
{
	CPaintManagerUI::ReleaseZip();
}

//接口查询
VOID * CPlatformResourceManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IPlatformResource,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IPlatformResource,Guid,dwQueryVer);
	return NULL;
}

//加载资源
bool CPlatformResourceManager::LoadDefaultResource(LPCTSTR pszXmlName,HINSTANCE hInstance)
{

#if _DEBUG
#else
#define USE_FILE_ZIP
#endif

	//设置资源路径	
	CPaintManagerUI::SetInstance(hInstance);  
	CPaintManagerUI::SetResourceDll(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME));
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+TEXT("Resource"));
#ifdef USE_FILE_ZIP
	CPaintManagerUI::SetFileZip(CPaintManagerUI::GetInstancePath()+TEXT("Resource\\Resource.dll"), true);
#endif
	//变量定义
	CResourceLoader RecourceLoader;
	return RecourceLoader.LoadResource(pszXmlName,NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(PlatformResourceManager);

/////////////////////////////////////////////////////////////////////////////////////////////////
