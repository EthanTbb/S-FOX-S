#ifndef PLATFORM_RESOURCE_HEAD_HEAD_FILE
#define PLATFORM_RESOURCE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//系统文件
#include <VFW.h>

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\..\客户端组件\图形类库\FGuilibHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef PLATFORM_RESOURCE_CLASS
	#ifdef  PLATFORM_RESOURCE_DLL
		#define PLATFORM_RESOURCE_CLASS _declspec(dllexport)
	#else
		#define PLATFORM_RESOURCE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define PLATFORM_RESOURCE_DLL_NAME	TEXT("PlatformResource.dll")		//组件名字
#else
	#define PLATFORM_RESOURCE_DLL_NAME	TEXT("PlatformResourceD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IPlatformResource INTERFACE_VERSION(1,1)
	static const GUID IID_IPlatformResource={0xbcd7ff05,0x0a79,0x4482,0x9b,0xa6,0x79,0xdf,0x4b,0xd9,0x80,0x16};
#else
	#define VER_IPlatformResource INTERFACE_VERSION(1,1)
	static const GUID IID_IPlatformResource={0xe5694b07,0x888e,0x45aa,0x85,0xa7,0xfe,0x89,0x84,0x1b,0x79,0x20};
#endif

//资源管理
interface IPlatformResource : public IUnknownEx
{
	//加载资源
	virtual bool LoadDefaultResource(LPCTSTR pszXmlName,HINSTANCE hInstance)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//组件辅助类

DECLARE_MODULE_HELPER(PlatformResource,PLATFORM_RESOURCE_DLL_NAME,"CreatePlatformResourceManager")

//////////////////////////////////////////////////////////////////////////////////

#endif