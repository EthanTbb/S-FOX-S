#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"

//�����ļ�
#include "Other\IDispimp.h"
#include "Other\Custsite.h"

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó���
class CGameUpdateApp : public CWinApp
{
	//��������
public:
	CImpIDispatch *					m_pIDispatch;						//�Զ��ӿ�
	CCustomOccManager *				m_pCustomOccManager;				//�������

	//��Դ���
public:
	//CSkinRenderManager				m_SkinRenderManager;				//��Ⱦ����
	//CSkinResourceManager			m_SkinResourceManager;				//��Դ����

	//��������
public:
	//���캯��
	CGameUpdateApp();

	//���غ���
public:
	//�˳�����
	virtual INT ExitInstance();
	//���뺯��
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�������
extern CGameUpdateApp theApp;

//////////////////////////////////////////////////////////////////////////////////
