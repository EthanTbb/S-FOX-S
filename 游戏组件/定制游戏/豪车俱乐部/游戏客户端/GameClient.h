#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameFrameWindow.h"

//////////////////////////////////////////////////////////////////////////////////

//应用程序
class CGameClientApp : public CGameFrameApp
{

	//变量定义
protected:
	//CGameFrameWindow *				m_pGameFrameWnd;					//框架窗口
	//CGameFrameEngine *				m_pGameFrameEngine;					//框架引擎

	//函数定义
public:
	//构造函数
	CGameClientApp();
	//析构函数
	virtual ~CGameClientApp();


	//重载函数
public:
	//配置函数
	//virtual BOOL InitInstance();
	////退出函数
	//virtual BOOL ExitInstance();

	//继承函数
public:
	//创建框架
		virtual CGameFrameWnd * GetGameFrameWnd();
	//创建引擎
	virtual CGameFrameEngine * GetGameFrameEngine(DWORD dwSDKVersion);
};

//////////////////////////////////////////////////////////////////////////////////

//程序对象
extern CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////////////
