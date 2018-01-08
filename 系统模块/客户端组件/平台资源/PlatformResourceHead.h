#ifndef PLATFORM_RESOURCE_HEAD_HEAD_FILE
#define PLATFORM_RESOURCE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//ϵͳ�ļ�
#include <VFW.h>

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//����ļ�
#include "..\..\�ͻ������\ͼ�����\FGuilibHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef PLATFORM_RESOURCE_CLASS
	#ifdef  PLATFORM_RESOURCE_DLL
		#define PLATFORM_RESOURCE_CLASS _declspec(dllexport)
	#else
		#define PLATFORM_RESOURCE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define PLATFORM_RESOURCE_DLL_NAME	TEXT("PlatformResource.dll")		//�������
#else
	#define PLATFORM_RESOURCE_DLL_NAME	TEXT("PlatformResourceD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IPlatformResource INTERFACE_VERSION(1,1)
	static const GUID IID_IPlatformResource={0xbcd7ff05,0x0a79,0x4482,0x9b,0xa6,0x79,0xdf,0x4b,0xd9,0x80,0x16};
#else
	#define VER_IPlatformResource INTERFACE_VERSION(1,1)
	static const GUID IID_IPlatformResource={0xe5694b07,0x888e,0x45aa,0x85,0xa7,0xfe,0x89,0x84,0x1b,0x79,0x20};
#endif

//��Դ����
interface IPlatformResource : public IUnknownEx
{
	//������Դ
	virtual bool LoadDefaultResource(LPCTSTR pszXmlName,HINSTANCE hInstance)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//���������

DECLARE_MODULE_HELPER(PlatformResource,PLATFORM_RESOURCE_DLL_NAME,"CreatePlatformResourceManager")

//////////////////////////////////////////////////////////////////////////////////

#endif