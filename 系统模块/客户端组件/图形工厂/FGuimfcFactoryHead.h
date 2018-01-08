#ifndef FGUIMFC_FACTORY_HEAD_FILE
#define FGUIMFC_FACTORY_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\图形类库\FGuilibHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef FGUIMFC_FACTORY_CLASS
	#ifdef  FGUIMFC_FACTORY_DLL
		#define FGUIMFC_FACTORY_CLASS _declspec(dllexport)
	#else
		#define FGUIMFC_FACTORY_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define FGUIMFC_FACTORY_DLL_NAME	TEXT("FGuimfcFactory.dll")		//组件名字
#else
	#define FGUIMFC_FACTORY_DLL_NAME	TEXT("FGuimfcFactoryD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef FGUIMFC_FACTORY_DLL
	#include "FGuimfcWindow.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif