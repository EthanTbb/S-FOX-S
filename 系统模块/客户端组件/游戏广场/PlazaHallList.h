#ifndef PLAZA_HALL_LIST_HEAD_FILE
#define PLAZA_HALL_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"
#include "DlgDownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//������ͼ
class CPlazaHallList : public IDownLoadStateSink
{
	//��������
public:
	//���캯��
	CPlazaHallList(CPaintManagerUI*	pPaintManager);
	//��������
	virtual ~CPlazaHallList();

	//���غ���
public:
	//��ʼ�ؼ�
	bool InitControlUI();
	//��Ϣ����
	bool Notify(TNotifyUI &  msg);

	//�û�����
public:
	//��ȡ�ؼ�
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//��Ϣ����
public:
	//�������
	void OnNotifyClickAddGame(TNotifyUI &  msg);
	//���չ��
	void OnNotifyClickExpandAll(TNotifyUI &  msg);
	//�������
	void OnNotifyClickCollapseAll(TNotifyUI &  msg);
	//չ����Ϣ
	void OnNotifyItemExpand(TNotifyUI &  msg);
	//������Ϣ
	void OnNotifyItemCollapse(TNotifyUI &  msg);
	//�����б�
	void OnNotifyListClickNode(TNotifyUI &  msg);
	//�����б�
	void OnNotifyListActivateNode(TNotifyUI &  msg);


public:
	//�¼���Ϣ
	bool OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
public:
	//�첽����
	virtual bool SendMessage( UINT message, WPARAM wParam , LPARAM lParam );


public:

	//��ʾ�б�
	void ShowPlazaHallList();

	//��������
	void LoadDataFromMemory();

	//��ʼ����
	void InitUI();

	//����ҳ��
	void CreateUI();

	//ˢ�½���
	void UpdateUI();

	//ˢ������
	void UpdateDownUI(CPlazaHallGameDownLoadSink * pDownLoadSink);

	//����ICO
	void UpdateItemIco(CContainerUI* pContainerUI, CGameKindItem* pGameKindItem);

	//��ʾ����
protected:
	vector<CGameTypeItem *>   m_vGameType;
	vector<CGameKindItem *>   m_vGameKind;
	vector<CGameServerItem *> m_vGameServer;
	vector<CContainerUI *>	  m_vGameDown;
private:
	CPaintManagerUI*		m_pPaintManager;	
};

//////////////////////////////////////////////////////////////////////////////////

#endif