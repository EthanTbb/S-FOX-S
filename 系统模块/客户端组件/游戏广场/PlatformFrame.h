#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"

//�ؼ��ļ�
#include "DlgWhisper.h"
#include "DlgCollocate.h"
#include "DlgTaskCenter.h"

//���ڿؼ�
#include "PlazaViewServerCtrl.h"

//�����ļ�
#include "MissionList.h"
#include "MissionLogon.h"

//���崰��
#include "PlazaViewServer.h"
#include "PlazaViewContainer.h"

//��Ϸ�б�
#include "PlazaUserInfo.h"
#include "PlazaHallList.h"
#include "PlazaHallGame.h"
//////////////////////////////////////////////////////////////////////////////////

//�����
#define IDM_CLOSE					105                                 //�رմ���
#define IDM_RESTORE                 106                                 //��ԭ��ʾ
#define IDM_MINIMIZE                107                                 //��С����ʾ
#define IDM_MAXIMIZE                108                                 //�����ʾ

//�����
#define IDM_SHOW_LOGON				200									//��ʾ��¼

//////////////////////////////////////////////////////////////////////////////////
class CDlgEnquire;

//�б�״̬
enum enServerListControl
{
	ServerListControl_Hide,			//�����б�
	ServerListControl_Show,			//��ʾ�б�
	ServerListControl_Turn,			//��ת�б�
};

//����˵��
typedef CWHArray<CPlazaViewServer *>	CPlazaViewServerArray;				//��������

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨���
class CPlatformFrame : public CFGuiFrameWnd, public ICustomFaceEvent,public IServerListDataSink
{
	//�������
protected:
	bool							m_bMaxShow;							//����־
	bool							m_bRectify;							//������־
	CRect							m_rcNormalSize;						//����λ��

	//״̬����
protected:
	bool							m_bWhisperIcon;						//˽��ͼ��

	//��Ϸ�б�
protected:
	CServerListData					m_ServerListData;					//��Ϸ����
	CGamePropertyListData			m_GamePropertyListData;				//��������

protected:
	CPlazaUserInfo					m_PlazaUserInfo;
	CPlazaHallList					m_PlazaHallList;
	CPlazaHallGame					m_PlazaHallGame;


	//��ͼ�ؼ�
protected:	
	//CPlazaViewGame					m_PlazaViewGame;					//��Ϸ�㳡
	//CPlazaViewAffiche				m_PlazaViewAffiche;					//���洰��	
	//CPlazaViewGameType				m_PlazaViewGameType;				//��Ϸ����
	CPlazaViewServerCtrl			m_PlazaViewServerCtrl;				//�������

	//��ͼ����
protected:
	CPlazaViewContainer				m_PlazaViewContainer;				//��ͼ����

	//�ؼ�����
protected:
	CSystemTrayIcon					m_SystemTrayIcon;					//����ͼ��

	//�������
protected:
	CMissionList					m_MissionList;						//�б�����
	CMissionLogon					m_MissionLogon;						//��¼����
	CMissionManager					m_MissionManager;					//�������
	CDlgEnquire*					m_pDlgEnquire;				

	//�ؼ�����
protected:
	CPlazaViewServer 				m_PlazaViewServer;					//��ǰ����
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//˽������

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

	//��̬����
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//���ָ��

	//��������
public:
	//���캯��
	CPlatformFrame();
	//��������
	virtual ~CPlatformFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ͷ��ӿ�
protected:
	//����ʧ��
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID);
	//ͷ������
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//״̬֪ͨ
public:
	//��ȡ֪ͨ
	virtual VOID OnGameItemFinish();
	//���֪ͨ
	virtual VOID OnGameMatchFinish();
	//��ȡ֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���غ���
public:
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//���ܺ���
public:
	//��������
	CRect MapSelectedTypeRect();
	//��������
	VOID ShowTaskNotifyer(LPCTSTR pszTaskName);
	//���ҳ��
	bool WebBrowse(LPCTSTR pszURL, bool bAutoFullView);	
	//��Ϸ����
	bool DownLoadGame(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//˽�ĺ���
public:
	//��ȡ˽��
	bool ShowWhisperItem();
	//����˽��
	bool InsertWhisperItem(CDlgWhisper * pDlgWhisper);
	//ɾ��˽��
	bool RemoveWhisperItem(CDlgWhisper * pDlgWhisper);

	//�������
public:
	//�����
	bool ActiveServerViewItem();
	//ɾ������
	bool DeleteServerViewItem();	
	//���뷿��
	bool EntranceServerItem(CGameServerItem * pGameServerItem);
	//������ʶ
	bool IsEnteredServerItem() { return m_PlazaViewServer.m_hWnd!=NULL; }
	//��ȡ����
	CPlazaViewServer * GetPlazaViewServer() { return &m_PlazaViewServer; }

	//��ͼ�ؼ�
public:
	//��Ϸ��ͼ
	//CPlazaViewGame * GetPlazaViewGame() { return &m_PlazaViewGame; }
	//������ͼ
	//CPlazaViewGameType * GetPlazaViewGameType() { return &m_PlazaViewGameType; }
// 	//������ͼ
// 	CPlazaViewAffiche * GetPlazaViewAffiche() { return &m_PlazaViewAffiche; }
	//������ͼ
	CPlazaViewServerCtrl * GetPlazaViewServerCtrl() { return &m_PlazaViewServerCtrl; }

public:
	//��������
	CControlUI * GetPlazaControlByName(LPCTSTR pszControlName);

	//���ش���
	void ReturnHallMain();

	//�����ڸ�
	void ShowWindowMaskLayer(bool bShow =true);
	//��Ƶ����
public:
	void SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr);

	//���溯��
protected:
	//��ԭ����
	bool RestoreWindow();
	//��󴰿�
	bool MaxSizeWindow(bool bRecordPosition=true);

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//�Զ���Ϣ
protected:
	//�ȼ���Ϣ
	LRESULT OnMessageHotKey(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageTaskBar(WPARAM wParam, LPARAM lParam);
	//ͼ����Ϣ
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam);

public:
		//�Զ�����Ϣ
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	void OnWkeURLHidden();

	//���ܺ���
public:
	//��ȡʵ��
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }


	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////////////

#endif