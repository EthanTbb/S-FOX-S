#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameFrameWindow.h"

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó���
class CGameClientApp : public CGameFrameApp
{

	//��������
protected:
	//CGameFrameWindow *				m_pGameFrameWnd;					//��ܴ���
	//CGameFrameEngine *				m_pGameFrameEngine;					//�������

	//��������
public:
	//���캯��
	CGameClientApp();
	//��������
	virtual ~CGameClientApp();


	//���غ���
public:
	//���ú���
	//virtual BOOL InitInstance();
	////�˳�����
	//virtual BOOL ExitInstance();

	//�̳к���
public:
	//�������
		virtual CGameFrameWnd * GetGameFrameWnd();
	//��������
	virtual CGameFrameEngine * GetGameFrameEngine(DWORD dwSDKVersion);
};

//////////////////////////////////////////////////////////////////////////////////

//�������
extern CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////////////
