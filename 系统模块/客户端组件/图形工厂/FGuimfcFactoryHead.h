#ifndef FGUIMFC_FACTORY_HEAD_FILE
#define FGUIMFC_FACTORY_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//����ļ�
#include "..\ͼ�����\FGuilibHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef FGUIMFC_FACTORY_CLASS
	#ifdef  FGUIMFC_FACTORY_DLL
		#define FGUIMFC_FACTORY_CLASS _declspec(dllexport)
	#else
		#define FGUIMFC_FACTORY_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define FGUIMFC_FACTORY_DLL_NAME	TEXT("FGuimfcFactory.dll")		//�������
#else
	#define FGUIMFC_FACTORY_DLL_NAME	TEXT("FGuimfcFactoryD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef FGUIMFC_FACTORY_DLL
	#include "FGuimfcWindow.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif