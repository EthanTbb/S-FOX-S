#ifndef PLATFORM_RESOURCE_MANAGER_HEAD_FILE
#define PLATFORM_RESOURCE_MANAGER_HEAD_FILE
#pragma once

//引入文件
#include "Platformresourcehead.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

//资源管理
class CPlatformResourceManager : public IPlatformResource
{
	//函数定义
public:
	//构造函数
	CPlatformResourceManager();
	//析构函数
	virtual ~CPlatformResourceManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//接口函数
public:
	//加载资源
	virtual bool LoadDefaultResource(LPCTSTR pszXmlName,HINSTANCE hInstance);
};

/////////////////////////////////////////////////////////////////////////////////////////////////

#endif
