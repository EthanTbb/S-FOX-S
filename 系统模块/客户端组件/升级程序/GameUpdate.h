#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"

//其他文件
#include "Other\IDispimp.h"
#include "Other\Custsite.h"

//////////////////////////////////////////////////////////////////////////////////

//应用程序
class CGameUpdateApp : public CWinApp
{
	//变量定义
public:
	CImpIDispatch *					m_pIDispatch;						//自动接口
	CCustomOccManager *				m_pCustomOccManager;				//管理对象

	//资源组件
public:
	//CSkinRenderManager				m_SkinRenderManager;				//渲染管理
	//CSkinResourceManager			m_SkinResourceManager;				//资源管理

	//函数定义
public:
	//构造函数
	CGameUpdateApp();

	//重载函数
public:
	//退出函数
	virtual INT ExitInstance();
	//进入函数
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//程序对象
extern CGameUpdateApp theApp;

//////////////////////////////////////////////////////////////////////////////////
