#ifndef PLATFORM_RESOURCE_MANAGER_HEAD_FILE
#define PLATFORM_RESOURCE_MANAGER_HEAD_FILE
#pragma once

//�����ļ�
#include "Platformresourcehead.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

//��Դ����
class CPlatformResourceManager : public IPlatformResource
{
	//��������
public:
	//���캯��
	CPlatformResourceManager();
	//��������
	virtual ~CPlatformResourceManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//������Դ
	virtual bool LoadDefaultResource(LPCTSTR pszXmlName,HINSTANCE hInstance);
};

/////////////////////////////////////////////////////////////////////////////////////////////////

#endif
