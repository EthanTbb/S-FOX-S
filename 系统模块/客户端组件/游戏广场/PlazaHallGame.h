#ifndef PLAZA_HALL_GAME_HEAD_FILE
#define PLAZA_HALL_GAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"
#include "DlgDownLoad.h"
#include "DlgUpdateStatus.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//������ͼ
class CPlazaHallGame : public IDownLoadStateSink,public IUpdateStatusSink
{
	//��������
public:
	//���캯��
	CPlazaHallGame(CPaintManagerUI*	pPaintManager);
	//��������
	virtual ~CPlazaHallGame();
	//���غ���
public:
	//��Ϣ����
	bool Notify(TNotifyUI &  msg);
	//��ʼ�ؼ�
	bool InitControlUI();
	//�ж���Ϣ
	bool IsNotify(TNotifyUI &  msg);
	//��ȡ�ؼ�
	CControlUI * GetControlByName(LPCTSTR pszControlName);
	//�¼���Ϣ
public:
	//�¼���Ϣ
	bool OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	//���ؽӿ�
public:
	//�첽����
	virtual bool SendMessage( UINT message, WPARAM wParam , LPARAM lParam );
public:
	//��ʼ����
	VOID InitData();

	//��������
	VOID ShowPlazaHallGame();

	//ˢ�½���
	VOID UpdateUI();

	//ˢ���Ƽ�
	VOID UpdateAdUI();

	//ˢ������
	VOID UpdateDownUI(CPlazaHallGameDownLoadSink * pDownLoadSink);

	//ˢ��״̬
	VOID UpdateDownStateUI(CContainerUI * pDwon,int nState);

	//ˢ�·���
	VOID UpdateRoomUI(CGameKindItem * pGameKindItem);

	//ˢ��ɾ��
	VOID UpdateDeleteUI();

	//ҵ����
public:
	//��ѯ����
	bool CheckUpdate(CGameKindItem *vMyGameKind);

	//������Ϸ
	bool DownLoadGame(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//������Ϸ
	bool SaveGameKind();

	//������Ϸ
	bool LoadDataFromMemory();

	//������Ϸ
	bool LoadDataFromFile();

	//ƥ����Ϸ
	bool FilterGameKind();

	//������Ϸ
	bool AddGameKind();

	//ɾ����Ϸ
	bool DeleteGameKind(CGameKindItem * pGameKindItem);

	//������Ϸ
	bool EnterGameKind(CGameKindItem * pGameKindItem);

	//�����Ƽ���Ϸ
	bool EnterGameAdKind(CGameKindItem * pGameKindItem);

	//���뷿��
	bool EnterGameServer(CGameServerItem * pGameServerItem);

	//�ر�ɾ��
	VOID CloseDeleteState();

	//���ش���
	VOID ReturnHallMain();

	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);
public:
	virtual VOID OnEventDownLoadTaskFinish() ;

	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate);
	//��������
private:
	//��������
	inline void GetProcessName(CGameKindItem *vMyGameKind,TCHAR szProcessName[LEN_PROCESS]);



	

	//��ʾ����
protected:
	CPaintManagerUI*		m_pPaintManager;
	CWHArray<CPlazaHallGameDownLoadSink *>		m_DownLoadSinkArray;				//��������
	vector<CPlazaHallGameDownLoadSink *>		m_vDwonLoadSink;
protected:
	vector<CGameKindItem *> m_vMyGameKind;
	vector<CGameKindItem *> m_vAdGameKind;
	vector<CGameKindItem *> m_vGameKind;
	vector<int>				m_vGameKindID;
	vector<CGameServerItem *> m_vGameServer;
	vector<CControlUI *>    m_vMyGameDelete;
	vector<CContainerUI *>  m_vMyGameDown;

	CGameKindItem * m_pEnterGame;

	//�ؼ�����
protected:
	CUpdateStatusView *				m_pUpdateStatusView;				//������ͼ

};

//////////////////////////////////////////////////////////////////////////////////

#endif