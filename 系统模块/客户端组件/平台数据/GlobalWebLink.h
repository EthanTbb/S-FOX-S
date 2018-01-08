#ifndef GLOBAL_WEBLINK_HEAD_FILE
#define GLOBAL_WEBLINK_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ȫ�ַ�����
class PLATFORM_DATA_CLASS CGlobalWebLink
{
	//��������
protected:
	TCHAR							m_szSpreadLink[MAX_PATH];					//�ƹ�����	
	TCHAR							m_szUpdateLink[MAX_PATH];					//��������
	TCHAR							m_szPlatformLink[MAX_PATH];					//ƽ̨����		
	TCHAR							m_szValidateLink[MAX_PATH];					//��֤����

	//��̬����
protected:
	static CGlobalWebLink *			m_pGlobalWebLink;							//ȫ������

	//��������
public:
	//���캯��
	CGlobalWebLink();
	//��������
	virtual ~CGlobalWebLink();

	//���ܺ���
public:
	//��ȡ����
	VOID LoadPaltformLink();	

	//������Ϣ
public:
	//��֤����
	LPCTSTR GetValidateLink();
	//�ƹ�����
	LPCTSTR GetSpreadLink() { return m_szSpreadLink; }
	//��������
	LPCTSTR GetUpdateLink() { return m_szUpdateLink; }
	//ƽ̨����
	LPCTSTR GetPlatformLink() { return m_szPlatformLink; }

	//��ȡ����
protected:
	//��ȡ��ֵ
	UINT ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault);
	//��ȡ����
	VOID ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount);

	//��̬����
public:
	//��ȡ����
	static CGlobalWebLink * GetInstance() { return m_pGlobalWebLink; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif