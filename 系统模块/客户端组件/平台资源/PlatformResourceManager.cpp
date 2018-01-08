#include "StdAfx.h"
#include "Platformresourcemanager.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CPlatformResourceManager::CPlatformResourceManager(void)
{
}

//��������
CPlatformResourceManager::~CPlatformResourceManager(void)
{
	CPaintManagerUI::ReleaseZip();
}

//�ӿڲ�ѯ
VOID * CPlatformResourceManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IPlatformResource,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IPlatformResource,Guid,dwQueryVer);
	return NULL;
}

//������Դ
bool CPlatformResourceManager::LoadDefaultResource(LPCTSTR pszXmlName,HINSTANCE hInstance)
{

#if _DEBUG
#else
#define USE_FILE_ZIP
#endif

	//������Դ·��	
	CPaintManagerUI::SetInstance(hInstance);  
	CPaintManagerUI::SetResourceDll(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME));
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+TEXT("Resource"));
#ifdef USE_FILE_ZIP
	CPaintManagerUI::SetFileZip(CPaintManagerUI::GetInstancePath()+TEXT("Resource\\Resource.dll"), true);
#endif
	//��������
	CResourceLoader RecourceLoader;
	return RecourceLoader.LoadResource(pszXmlName,NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(PlatformResourceManager);

/////////////////////////////////////////////////////////////////////////////////////////////////
