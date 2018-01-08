#ifndef PLAZA_VIEW_SERVER_HEAD_FILE
#define PLAZA_VIEW_SERVER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"
#include "DlgWhisper.h"
#include "ServerListData.h"
#include "ProcessManager.h"
#include "DlgCollocateRoom.h"
#include "DlgSearchTable.h"
#include "DlgServerManager.h"
#include "DlgTaskCenter.h"
#include "DlgExchange.h"
#include "DlgManageUser.h"
#include "PlatformPublicize.h"
#include "ChatHistory.h"
#include "TableViewFrame.h"
#include "PlazaViewServerCtrl.h"
#include "DlgShop.h"
#include "DlgBag.h"
#include "DlgPropertyUse.h"
#include "DlgInsure.h"
#include "DlgCheckIn.h"
#include "DlgBaseEnsure.h"
#include "DlgTask.h"
//////////////////////////////////////////////////////////////////////////

//��Ϊ����
#define WM_ADD_FRIEND				(WM_USER+400)						//��Ϊ����
#define WM_SET_DETEST				(WM_USER+401)						//��Ϊ���

//////////////////////////////////////////////////////////////////////////////////

//����״̬
enum enServiceStatus
{
	ServiceStatus_Unknow,			//δ֪״̬
	ServiceStatus_Entering,			//����״̬
	ServiceStatus_Validate,			//��֤״̬
	ServiceStatus_RecvInfo,			//��ȡ״̬
	ServiceStatus_ServiceIng,		//����״̬
	ServiceStatus_NetworkDown,		//�ж�״̬
};

//��������
struct tagSearchTable
{
	bool							bOneNull;							//һ����λ
	bool							bTwoNull;							//������λ
	bool							bAllNull;							//ȫ��λ��
	bool							bNotFull;							//��ȫ��λ
	bool							bFilterPass;						//��������
	WORD							wStartTableID;						//��ʼ����
	WORD							wResultTableID;						//�������
	WORD							wResultChairID;						//�������
};

//�û�����
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//������ͼ
class CPlazaViewServer :  public CFGuiDialog, public IServerViewItem, public ITCPSocketSink,public ITableViewFrameSink, public IUserManagerSink, public IProcessManagerSink,public IGamePropertySink, public IStatusViewSink, public IFlashControlSink,public IGamePropertyUseSink
{
	
	//�������
protected:
	bool							m_bCreateFlag;						//������־

	//��������
protected:
	WORD							m_wReqTableID;						//��������
	WORD							m_wReqChairID;						//����λ��
	WORD							m_wFindTableID;						//��������
	WORD							m_wInvitedTableID;					//��������
	bool							m_bUserInRoom;						//�Ƿ��ڷ���

	//�������
protected:
	DWORD							m_dwChatTime;						//����ʱ��
	CString							m_strChatString;					//������Ϣ

	//�û�����
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
	
	//��������
protected:
	WORD							m_wServerType;						//��������
	DWORD							m_dwServerRule;						//�������
	enServiceStatus					m_ServiceStatus;					//����״̬
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ
	TCHAR							m_szServerPassword[LEN_PASSWORD];	//��������

	//������Ϣ
protected:
	BYTE							m_cbColumnCount;					//�б���Ŀ
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//�б�����

	//��������
protected:
	WORD							m_wChairCount;						//��������
	tagGameKind						m_GameKind;							//������Ϣ
	tagGameServer					m_GameServer;						//������Ϣ

	//���ò���
protected:
	CParameterGame *				m_pParameterGame;					//��Ϸ����
	CParameterServer *				m_pParameterServer;					//��������

	//�������
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//�û�����

	//�������
protected:
	CDlgStatus						m_DlgStatus;						//״̬����	
	CFlashControl					m_FlashControl;						//�����ؼ�
	CProcessManager					m_ProcessManager;					//���̹���
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//˽������	
	CChatHistoryManager				m_ChatHistoryManager;				//�����¼����

	//�ؼ�����
protected:
	CDlgSearchTable *				m_pDlgSearchTable;					//��������	
	CDlgInsureServerNew *			m_pDlgInsureServer;					//���ж���	
	CDlgServerManager *				m_pDlgServerManager;				//�������
	CDlgExchangeServer *			m_pDlgExchangeServer;				//����һ�
	CPlazaViewServerCtrl *			m_pPlazaViewServerCtrl;				//��ܿؼ�
	CDlgTaskServer *				m_pDlgTaskServerCenter;				//��������	
	CDlgShopServer*					m_pDlgShopServer;					//�����̳�
	CDlgBagServer*					m_pDlgBagServer;					//�ҵı���
	CDlgPropertyUseServer*			m_pDlgPropertyUse;					//����ʹ��
	CDlgCheckInServer*				m_pDlgCheckIn;                      //ǩ������
	CDlgBaseEnsureServer*           m_pDlgBaseEnsure;                   //�ͱ�����

	//�������
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//��������
	CGameLevelParserHelper			m_GameLevelParserModule;			//��Ϸ�ȼ�
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//�û�����

	//�������
public:
	CSkinButton						m_btControl;						//���ư�ť
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

	//�ӿڱ���
public:
	ITableViewFrame *				m_pITableViewFrame;					//���ӿ��

	//��Դ����
protected:
	CEncircleBMP					m_TableEncircle;					//������Դ
	CEncircleBMP					m_MainEncircle;						//��������Դ
	CPngImage						m_ImageGameName;					//��Ϸ����

	//����ؼ�
public:
	CDlgManageUser *				m_pDlgManageUser;					//�û�����

public:
	bool							m_bPlaying;
	//��������
public:
	//���캯��
	CPlazaViewServer();
	//��������
	virtual ~CPlazaViewServer();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("ServerView.xml"); } 

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ӽӿ�
public:
	//���˫��
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//�������
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//�Ҽ�����
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);
	//���Ӱ�ť
	virtual VOID OnHitTableButton(BYTE cbButton);

	//����ӿ�
protected:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//���߽ӿ�
protected:
	//�������
	virtual VOID OnEventFlashFinish();
	//��ǰ���
	virtual bool OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage);
	//�������
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);
	//��ѯ�û�
	virtual IClientUserItem * OnQueryClientUserItem() { return m_pIMySelfUserItem; };
	//ʹ�õ���
	virtual bool OnEventUseProperty(WORD wPropertyIndex);

	//���̽ӿ�
public:
	//���̴���
	virtual bool OnGameProcessError();
	//���̹ر�
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//��������
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//��������
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�û��ӿ�
public:
	//�û�����
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//�û�ɾ��
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib);

	//�¼��ӿ�
protected:
	//ȡ������
	virtual VOID OnStatusCancel();

	//�˵��ӿ�
public:
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount);

	//��Ϣ����
public:
	//�����ʶ
	virtual WORD GetServerID() { return m_GameServer.wServerID; }
	//��������
	virtual WORD GetServerType() { return m_wServerType; }
	//�������
	virtual DWORD GetServerRule() { return m_dwServerRule; }
	//��Ϸ��ʶ
	virtual WORD GetKindID() { return m_GameKind.wKindID; }
	//��������
	virtual LPCTSTR GetKindName() { return m_GameKind.szKindName; }
	//��������
	virtual LPCTSTR GetServerName() { return m_GameServer.szServerName; }
	//����״̬
	virtual enServiceStatus GetServiceStatus() { return m_ServiceStatus; }
	//��Ϸ����
	virtual CParameterGame * GetParameterGame() { return m_pParameterGame; }
	//��������
	virtual CParameterServer * GetParameterServer() { return m_pParameterServer; }

	//�������
public:
	//�������
	virtual ITCPSocket * GetTCPSocket() { return m_TCPSocketModule.GetInterface(); }	
	//������ͼ
	virtual ITableViewFrame * GetTableViewFrame() { return m_pITableViewFrame; }
	//�������
	virtual CProcessManager * GetProcessManager() { return &m_ProcessManager; }
	//�ȼ����
	virtual IGameLevelParser * GetGameLevelParser() { return m_GameLevelParserModule.GetInterface(); }
	//�û����
	virtual IPlazaUserManager * GetPlazaUserManager() { return m_PlazaUserManagerModule.GetInterface(); }

	//���ܺ���
public:
	//��ʾ����
	VOID ShowIndividual(); 
	//��ʾ�ͱ�
	VOID ShowBaseEnsure(); 
	//��ʾǩ��
	VOID ShowCheckIn();
	//��ʾ����
	VOID ShowInsureView();
	//��ʾ����
	VOID ShowTask();
	//��ʾ�һ�
	VOID ShowExchange();
	//��ʾ�̳�
	VOID ShowShop();
	//��ʾ����
	VOID ShowBag();
	//��ʾ����ʹ��
	VOID ShowPropertyUse(IClientUserItem* pClientUserItem);
	//��ѯ����
	bool QuerySendPresentMisson();

	//���ܺ���
public:	
	//�ϰ��¼�
	VOID NotifyBossCome(bool bBossCome);
	//��������
	VOID SearchGameTable(tagSearchTable & SearchTable);
	//����ͷ��
	VOID UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);
	//��ѯ��Ϸ�ͻ���
	bool IsGameClientReady();
	//�Լ�״̬
	bool IsPlayingMySelf();
	//�����û�
	IClientUserItem * SearchUserByUserID (DWORD dwUserID);
	//����������
	bool SitDownInviteTable(WORD wTableID, bool bInRoom);

	//˽�ĺ���
public:
	//˽�ĻỰ
	VOID WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount);
	//����˽��
	CDlgWhisper * CreateWhisperWnd(DWORD dwConversationID,DWORD dwSendUserID,DWORD dwUserID[], WORD wUserCount, bool bCreate);

	//�����¼�
protected:
	//�û�����
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��¼����
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���ô���
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//״̬����
	bool OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���д���
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������
	bool OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�һ�����
	bool OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���ߴ���
	bool OnSocketMainProperty(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ϵͳ����
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	bool OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ǩ������
	bool OnSocketMainCheckIn(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ա����
	bool OnSocketMainMember(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�ͱ�����
	bool OnSocketMainBaseEnsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//�û�����
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//�û���Ϸ����
	bool OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//�û�˽��
	bool OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize);
	//˽�ı���
	bool OnSocketSubWisperExpression(VOID * pData, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);
	//�������
	bool OnSocketSubUserInvite(VOID * pData, WORD wDataSize);
	//���߳ɹ�
	bool OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize);
	//����ЧӦ
	bool OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnSocketPropertyMessage(VOID * pData, WORD wDataSize);
	//��������
	bool OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize);
	//�ȴ�����
	bool OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize);
	//����Ч��
	bool OnSocketSubUserPropBuff(VOID *pData, WORD wDataSize);
	//��ұ���
	bool OnSocketSubUserBackpack(VOID *pData, WORD wDataSize);
	//����ʹ��
	bool OnSocketSubPropertyUse(VOID *pData, WORD wDataSize);
	//��ѯ����
	bool OnSocketSubQuerySendPresent(VOID *pData, WORD wDataSize);
	//���͵���
	bool OnSocketSubPropertyPresent(VOID *pData, WORD wDataSize);
	//��ȡ����
	bool OnSocketSubGetSendPresent(VOID *pData, WORD wDataSize);
	//�������
	bool OnSocketSubUserTrumpet(VOID *pData, WORD wDataSize);

	//���溯��
public:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//���÷���
	VOID InitServerViewItem(CGameServerItem * pGameServerItem,WORD wAVServerPort,DWORD dwAVServerAddr);
	//���ý���
	VOID InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager);

	//��������
public:
	//���͹���
	bool SendUserRulePacket();
	//���͵�¼
	bool SendLogonPacket(LPCTSTR pszServerPasswd);	
	//�����Թ�
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//��������
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword=NULL);
	//��������
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);
	//���ͱ���
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);
	//��������
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//��������
public:
	//ִ���Թ�
	bool PerformLookonAction(WORD wTableID, WORD wChairID);
	//ִ������
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//ִ������
	bool PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass);
	//ִ�й���
	bool PerformBuyProperty(BYTE cbRequestArea,LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);

	//��������
protected:
	//����״̬
	VOID SetServiceStatus(enServiceStatus ServiceStatus);
	//��ԴĿ¼
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);

	//��ť�¼�
protected:
	//���밴ť
	VOID OnTableButton1Clicked();
	//���Ұ�ť
	VOID OnTableButton2Clicked();
	//���ܰ�ť
	VOID OnTableButton3Clicked();
	//���ܰ�ť
	VOID OnTableButton4Clicked();
	//���ఴť
	VOID OnTableButton5Clicked();

	//�ڲ�����
protected:	
	//������ͼ���
	bool CreateViewFrame();
	//��������˵�
	VOID CreateServerManagerMenu( CSkinMenu &ManageMenu );	
	//����Ч��
	bool EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe);

	//��������
protected:
	//����ť
	VOID OnBnClickedServerManage();
	//�������
	VOID OnManageServer();
	//�û�����
	VOID OnManageUser();
	//ϵͳ��Ϣ����
	VOID OnManageSystemMsg();
	//���ӹ������
	VOID OnManageTableRule();
	//�����˹���
	VOID OnManageUserAndior();

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	//�ؼ���Ϣ
protected:
	//��Ϊ����
	LRESULT OnWhisperAddFriend(WPARAM wParam, LPARAM lPara);
	//��Ϊ���
	LRESULT OnWhisperSetDetest(WPARAM wParam, LPARAM lPara);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif