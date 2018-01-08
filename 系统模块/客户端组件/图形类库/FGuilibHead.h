#ifndef FGUI_LIB_HEAD_FILE
#define FGUI_LIB_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//宏定义
#ifdef FGUI_LIB_DLL
#define FGUI_LIB_CLASS __declspec(dllexport)
#else
#define FGUI_LIB_CLASS __declspec(dllimport)
#endif

//模块定义
#ifndef _DEBUG
	#define FGUILIB_DLL_NAME	TEXT("FGuilib.dll")			//组件名字
#else
	#define FGUILIB_DLL_NAME	TEXT("FGuilibD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
#include <gdiplus.h>
#include "Utils/wke.h"

//包含文件
#include "Core/UIDefine.h"
#include "Core/UIBase.h"
#include "Core/UIManager.h"
#include "Core/UIRender.h"	
#include "Core/UIDelegate.h"
#include "Utils/UIShadow.h"
#include "Utils/Utils.h"
#include "Core/UIControl.h"	
#include "Core/UIContainer.h"
#include "Control/UIAnimation.h"
#include "Layout/UIVerticalLayout.h"
#include "Layout/UIHorizontalLayout.h"
#include "Layout/UITileLayout.h"
#include "Layout/UIDialogLayout.h"
#include "Layout/UITabLayout.h"
#include "Layout/UIPageLayout.h"
#include "Layout/UIAnimPageLayout.h"
#include "Layout/UIDeskListLayout.h"
#include "Control/UILabel.h"
#include "Control/UIButton.h"
#include "Control/UITabButton.h"
#include "Control/UICheckButton.h"
#include "Control/UIRadioButton.h"
#include "Control/UIOption.h"
#include "Control/UIText.h"
#include "Control/UIProgress.h"
#include "Control/UISlider.h"
#include "Control/UIEdit.h"
#include "Control/UIScrollBar.h"
#include "Control/UIPowerScale.h"
#include "Control/UIVerifyCode.h"
#include "Control/UINumber.h"
#include "Control/UIList.h"
#include "Control/UIListElementUIEx.h"
#include "Control/UIListEx.h"
#include "Control/UICombo.h"
#include "Control/UIEditCombo.h"
#include "Control/UIRichEdit.h"
#include "Control/UIFadeButton.h"
#include "Control/UIHotKey.h"
#include "Control/UIImage.h"
#include "Control/UIActiveX.h"
#include "Control/UIWebBrowser.h"
#include "Control/UIWkeBrowser.h"
#include "Core/UIMarkup.h"
#include "Core/UIResLoader.h"
#include "Core/UIDlgBuilder.h"
#include "Control/UIMenu.h"
#include "Control/UIToast.h"
#include "Utils/IImageOle.h"
#include "Utils/WinImplBase.h"
//////////////////////////////////////////////////////////////////////////////////
#pragma comment( lib, "GdiPlus.lib" )

#endif
