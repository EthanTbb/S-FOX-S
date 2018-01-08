#ifndef DLG_WHISPER_HEAD_FILE
#define DLG_WHISPER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CDlgWhisper;
class CPlazaViewServer;

//���鶨��
typedef CWHArray<CDlgWhisper *> CDlgWhisperItemArray;					//˽������
typedef CWHArray<IClientUserItem *> CClientUserItemArray;				//�û�����

//////////////////////////////////////////////////////////////////////////////////

//���ýṹ
struct tagWhisper
{
	//���ݱ���
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������

	//�ӿڱ���
	ITCPSocket *					pITCPSocket;						//�������
	IClientUserItem *				pIMySelfUserItem;					//�Լ�ָ��
	IUserOrderParser *				pIUserOrderParser;					//�ȼ��ӿ�
	IGameLevelParser *				pIGameLevelParser;					//�ȼ��ӿ�
};

//////////////////////////////////////////////////////////////////////////////////

//˽�Ĵ���
class CDlgWhisper : public CFGuiDialog, public IExpressionSink
{
	//״̬����
protected:
	bool							m_bCreateFlag;						//������־
	bool							m_bShowHistory;						//�����¼��־
	bool							m_bLableRound;

	//���ݱ���
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��

	//�ӿڱ���
protected:
	ITCPSocket *					m_pITCPSocket;						//�������
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
	IUserOrderParser *				m_pIUserOrderParser;				//�ȼ����
	IGameLevelParser *				m_pIGameLevelParser;				//�ȼ��ӿ�
	IPlazaUserManager *				m_pIPlazaUserManager;				//�û��ӿ�

	//��������
protected:
	DWORD							m_dwSendUserID;						//�û���ʶ
	DWORD							m_dwConversationID;					//�Ự��ʶ
	DWORD							m_dwChatHistroyID;					//��¼��ʶ
	CClientUserItemArray			m_ClientUserItemArray;				//�û�����

	//����ؼ�
protected:
	CLabelUI *						m_pLabelToolBar;					//�������
	CLabelUI *						m_pLabelRound;						//��������
	CLabelUI *						m_pLabelChatArea;					//���컷��
	CLabelUI *						m_pLabelUserInfoArea;				//��Ϣ����
	CLabelUI *						m_pLabelFunctionArea;				//���ܻ���
	CLabelUI *						m_pLabelChatHistory;				//�����¼
	CLabelUI *						m_pLabelHistoryArea;				//��¼����

	//����ؼ�
protected:
	CButtonUI *						m_pbtCancel;						//�رհ�ť
	CButtonUI *						m_pbtSendChat;						//��������
	CButtonUI *						m_pbtSendControl;					//���Ϳ���
	CButtonUI *						m_pbtUserMenu;						//�û��˵�

	//����ؼ�
protected:
	CButtonUI *						m_pbtChatButton1;					//��ɫ����
	CButtonUI *						m_pbtChatButton2;					//���鰴ť
	CButtonUI *						m_pbtChatButton3;					//���Ȱ�ť
	CButtonUI *						m_pbtChatHistory;					//�����¼	

	//����ؼ�
protected:	
	CButtonUI *						m_pbtAddFriend;						//��Ϊ����
	CButtonUI *						m_pbtSetDetest;						//��Ϊ���
	CButtonUI *						m_pbtMessageSetting;				//��Ϣ����
	CButtonUI *						m_pbtShieldThisChat;				//��������
	
	//���ư�ť
protected:	
	CSkinComboBox					m_ComboAndroidList;					//�����б�		

	//����ؼ�
protected:
	CEdit							m_ChatInput;						//��������
	CRichEditMessage				m_ChatMessage;						//������Ϣ
	CRichEditMessage				m_HistoryEdit;						//�����¼

	//��¼�ؼ�
protected:
	IChatHistory*					m_pIChatHistory;					//�����¼
	IChatHistoryManager*			m_pIChatHistroyManager;				//��¼����ӿ�

	//�������
protected:
	CPlazaViewServer *				m_pServerViewItem;					//����ӿ�
	CExpressionControl *			m_pExpressionControl;				//���鴰��

	//��������
public:
	//���캯��
	CDlgWhisper();
	//��������
	virtual ~CDlgWhisper();

	//���غ���
protected:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgWhisper.xml"); } 
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ú���
protected:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���ú���
protected:
	//�滭����
	//virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//�滭��Ϣ
	virtual VOID DrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);
	//�����¼�
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	virtual void OnEndPaintWindow(HDC hDC);

	//���ú���
public:
	//����Ȩ��
	VOID SetUserRight(DWORD dwUserRight) { m_dwUserRight = dwUserRight; }
	//����Ȩ��
	VOID SetMasterRight(DWORD dwMasterRight) { m_dwMasterRight = dwMasterRight; }
	//���ýӿ�
	VOID SetTCPSocket(ITCPSocket * pITCPSocket) { m_pITCPSocket=pITCPSocket; }
	//���ýӿ�
	VOID SetServerViewItem(CPlazaViewServer * pServerViewItem) { m_pServerViewItem=pServerViewItem; }
	//���ýӿ�
	VOID SetMySelfUserItem(IClientUserItem * pIMySelfUserItem) { m_pIMySelfUserItem=pIMySelfUserItem; }
	//���ýӿ�
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser) { m_pIUserOrderParser=pIUserOrderParser; }
	//���ýӿ�
	VOID SetGameLevelParser(IGameLevelParser * pIGameLevelParser) { m_pIGameLevelParser=pIGameLevelParser; }
	//���ýӿ�
	VOID SetPlazaUserManager(IPlazaUserManager* pIPlazaUserManager) { m_pIPlazaUserManager=pIPlazaUserManager; }
	//���������¼����ӿ�
	VOID SetChatHistoryManager(IChatHistoryManager *pIChatHistoryManager) { m_pIChatHistroyManager = pIChatHistoryManager; }

	//���ܺ���
public:
	//�����ж�
	bool DisuseEstimate();
	//�û��Ƚ�
	bool CompareUserItem(DWORD dwUserID[], WORD wUserCount);
	//����Ч��
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//��������
	bool CreateWhisperWnd(DWORD dwConversationID,DWORD dwSendUserID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd);

	//�¼�����
public:
	//�û�����
	VOID OnEventUserEnter(IClientUserItem * pIClientUserItem);
	//�û��뿪
	VOID OnEventUserLeave(IClientUserItem * pIClientUserItem);
	//�û�״̬
	VOID OnEventUserStatus(IClientUserItem * pIClientUserItem);
	//�û�����
	VOID OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//�û�˽��
	VOID OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor);

	//�ڲ�����
protected:
	//���±���
	VOID RectifyTitle();
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//������
	VOID SortUserIDData(DWORD dwUserID[], WORD wItemCount);
	//�滭����
	VOID DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
	//�滭�û�
	VOID DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��������
protected:
	//���ͱ���
	bool SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//��������
	bool SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString);
	//���ͻỰ
	bool SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString);

	//��������
protected:
	//���������¼
	VOID LoadChatHistory();
	//���ػ���
	VOID LoadAndroidList();

	//��ť��Ϣ
protected:
	//�û��˵�
	VOID OnBnClickedUserMenu();
	//������Ϣ
	VOID OnBnClickedSendChat();
	//���Ϳ���
	VOID OnBnClickedSendControl();
	//���찴ť
	VOID OnBnClickedChatButton1();
	//���찴ť
	VOID OnBnClickedChatButton2();
	//���찴ť
	VOID OnBnClickedChatButton3();
	//�����¼
	VOID OnBnClickedChatHistory();
	//����˽��
	VOID OnBnClickedShieldThisChat();
	//��Ϊ����
	VOID OnBnClickAddFriend();
	//��Ϊ���
	VOID OnBnClickSetDetest();
	//����˽��
	VOID OnBnClickMessageSetting();

	//�˵�����
protected:
	//������ɫ
	VOID OnSelectMoreColor();
	//ѡ����ɫ
	VOID OnSelectChatColor(UINT nCommandID);

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();	
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//��ȡ��С
	VOID OnGetMinMaxInfo(MINMAXINFO* lpMMI);	

	//�ؼ���Ϣ
protected:
	//ѡ����
	VOID OnCbnSelchangeComboxAndroid();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif