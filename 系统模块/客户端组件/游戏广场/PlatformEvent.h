#ifndef PLAT_FORM_EVENT_HEAD_FILE
#define PLAT_FORM_EVENT_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//�¼�����

//�û��¼�
#define EVENT_USER_LOGON			1									//��¼�ɹ�
#define EVENT_USER_LOGOUT			2									//ע���ɹ�
#define EVENT_USER_INFO_UPDATE		3									//���ϸ���
#define EVENT_USER_MOOR_MACHINE		4									//�󶨻���
#define EVENT_USER_SHOW_CHECKIN		5									//��ʾǩ��
#define EVENT_USER_SHOW_TASKVIEW	6									//��ʾǩ��
#define EVENT_USER_SHOW_MY_SPREAD   7									//��ʾ�ƹ�	
#define EVENT_USER_SHOW_RECHARGE    8									//��ʾ��ֵ
#define EVENT_USER_TRACK_ACCOUNTS	9									//׷���ʺ�
#define EVENT_USER_SHOW_PERSONER	10									//��������
#define EVENT_USER_SHOW_BASE_ENSURE	11									//��ȡ�ͱ�
#define EVENT_USER_SHOW_BAG			12									//��ʾ����
#define EVENT_USER_SHOW_SHOP		13									//��ʾ�̳�
#define EVENT_USER_SHOW_EXCHANGE	14									//��ʾ�һ�
#define EVENT_USER_SHOW_MY_INSURE	15									//��ʾ����
#define EVENT_USER_SHOW_REAL_TIP	16									//��ʾʵ��
#define EVENT_USER_SHOW_UPDATE_BUFF	17									//����BUff
#define EVENT_USER_HIDE_REAL_TIP	18									//����ʵ��
#define EVENT_USER_SHOW_BAG_RECORD	19									//��ʾ��¼
#define EVENT_DOWN_LOAD_FINISH		20									//���سɹ�
#define EVENT_SKIN_CONFIG_UPDATE	21									//��Դ����
#define EVENT_USER_CLOSE_GAME_AD	22									//��Ϸ�

//////////////////////////////////////////////////////////////////////////////////

//֪ͨ��Ϣ
#define WM_PLATFORM_EVENT			(WM_USER+1000)						//ƽ̨�¼�

//////////////////////////////////////////////////////////////////////////////////

//�¼�����
class CPlatformEvent
{
	//��������
protected:
	CWHArray<HWND>					m_EventWndArray;					//�¼�����

	//��̬����
protected:
	static CPlatformEvent *			m_pPlatformEvent;					//����ָ��

	//��������
public:
	//���캯��
	CPlatformEvent();
	//��������
	virtual ~CPlatformEvent();

	//ע�ᴰ��
public:
	//ע�ᴰ��
	bool RegisterEventWnd(HWND hEventWnd);
	//ע������
	bool UnRegisterEventWnd(HWND hEventWnd);

	//�����¼�
public:
	//�����¼�
	VOID SendPlatformEvent(WORD wEventID, LPARAM lParam);
	//�����¼�
	VOID PostPlatformEvent(WORD wEventID, LPARAM lParam);

	//��̬����
public:
	//��ȡ����
	static CPlatformEvent * GetInstance() { return m_pPlatformEvent; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif