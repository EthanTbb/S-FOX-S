#ifndef GLOBAL_WEBLINK_HEAD_FILE
#define GLOBAL_WEBLINK_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////

//全局服务器
class PLATFORM_DATA_CLASS CGlobalWebLink
{
	//变量定义
protected:
	TCHAR							m_szSpreadLink[MAX_PATH];					//推广链接	
	TCHAR							m_szUpdateLink[MAX_PATH];					//更新链接
	TCHAR							m_szPlatformLink[MAX_PATH];					//平台链接		
	TCHAR							m_szValidateLink[MAX_PATH];					//验证链接

	//静态变量
protected:
	static CGlobalWebLink *			m_pGlobalWebLink;							//全局链接

	//函数定义
public:
	//构造函数
	CGlobalWebLink();
	//析构函数
	virtual ~CGlobalWebLink();

	//功能函数
public:
	//读取链接
	VOID LoadPaltformLink();	

	//链接信息
public:
	//验证链接
	LPCTSTR GetValidateLink();
	//推广链接
	LPCTSTR GetSpreadLink() { return m_szSpreadLink; }
	//更新链接
	LPCTSTR GetUpdateLink() { return m_szUpdateLink; }
	//平台链接
	LPCTSTR GetPlatformLink() { return m_szPlatformLink; }

	//读取函数
protected:
	//读取数值
	UINT ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault);
	//读取数据
	VOID ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount);

	//静态函数
public:
	//获取对象
	static CGlobalWebLink * GetInstance() { return m_pGlobalWebLink; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif