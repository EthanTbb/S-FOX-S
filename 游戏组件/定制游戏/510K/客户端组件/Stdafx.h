#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>
#include <AtlBase.h>
#include <AtlDbcli.h>
#include <AfxDtctl.h>

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//���뻷��
#include "..\..\..\SDKCondition.h"
//ƽ̨�ļ�
#include "..\��Ϣ����\CMD_Game.h"


//////////////////////////////////////////////////////////////////////////////////
//������ӿ�


#ifdef SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////////////
//��������

#include "..\..\..\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/WHImage.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/WHImage.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

#include "..\..\..\\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../..//���ӿ�/Ansi/WHImage.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/GameEngine.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/SkinControl.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../..//���ӿ�/Unicode/WHImage.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/GameEngine.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/SkinControl.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../..//���ӿ�/Ansi/WHImageD.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../..//���ӿ�/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../..//���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../..//���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif