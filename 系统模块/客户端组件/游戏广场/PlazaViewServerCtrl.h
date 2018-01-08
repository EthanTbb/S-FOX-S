#ifndef PLAZA_VIEW_SERVER_CTRL_HEAD_FILE
#define PLAZA_VIEW_SERVER_CTRL_HEAD_FILE

#pragma once
#include "PlatformPublicize.h"
#include "DlgTrumpetItem.h"

//////////////////////////////////////////////////////////////////////////////////
//�������
class CPlazaViewServerCtrl : public CFGuiDialog,public IGamePropertySink,public IGamePropertyUseSink,public IChatControlSink,public IExpressionSink 
{
	//�������
protected:
	bool							m_bCreateFlag;						//������־

	//��������
protected:
	WORD							m_wChairCount;						//������Ŀ
	WORD							m_wServerType;						//��������
	DWORD							m_dwServerRule;						//�������

	//�û�����
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��

	//�������
private:
	DWORD							m_dwChatTime;						//����ʱ��
	CString							m_strChatString;					//������Ϣ

	//�ؼ�����
private:
	CRichEditMessage				m_ChatMessage;						//������Ϣ

	//�ؼ�����
protected:
	CWndChatControl					m_ChatControl;						//�������
	CDlgTrumpet *               m_pTrumpetItem;                     //���ȴ��� 
	CExpressionControl *			m_pExpressionControl;				//���鴰��
	//��������
public:
	DWORD							m_dwTrumpetCount;					//С������
	DWORD							m_dwTyphonCount;					//��������	

	//�������
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//�û�����

	//�ؼ�����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CUserListControl				m_UserListControl;					//�û��б�
	CPlatformPublicize				m_BillPublicize;					//����ؼ�

	//�ӿڱ���
protected:
	ITCPSocket *					m_pITCPSocket;						//TCP����
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
	IClientUserItem *				m_pISelectedUserItem;				//ѡ��ָ��	
	IGameLevelParser *				m_pIGameLevelParser;				//�ȼ��ӿ�
	IPlazaUserManager *				m_pIPlazaUserManager;				//�û�����

	//��Դ����
protected:
	CEncircleBMP					m_ChatEncircle;						//������Դ
	CEncircleBMP					m_UserListEncircle;					//�б���Դ

	//��������
public:
	//���캯��
	CPlazaViewServerCtrl();
	//��������
	virtual ~CPlazaViewServerCtrl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
public:	
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("ServerCtrl.xml"); } 

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);
	
	//��������
public:
	//�����¼�
	VOID SetServerInfo(WORD wChairCount,WORD wServerType,DWORD dwServerRule,DWORD dwUserRight,DWORD dwMasterRight);

	//���غ���
protected:	
	//���ú���
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�ӿں���
public:
	//ʹ�õ���
	virtual bool OnEventUseProperty(WORD wPropertyIndex);
	virtual bool PerformShowBag();
	virtual bool PerformShowShop();
	//�б�����
	virtual VOID SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount);	
	
	//��������
public:
	int  GetTrumpetCount();
	VOID SetTrumpetCount(const DWORD dwCount);
	int  GetTyphonCount();
	VOID SetTyphonCount(const DWORD dwCount);

	//�û��˵�
public:
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount);

	//�ؼ��ӿ�
public:
	//��Ϣ�ӿ�
	IStringMessage * GetStringMessage() { return QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage);}

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem);
	//�û�״̬
	virtual VOID OnEventUserUpdate(IClientUserItem * pIClientUserItem);

	//���߽ӿ�
protected:
	//��ǰ���
	virtual bool OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage);
	//�������
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);
	//��ѯ�û�
	virtual IClientUserItem * OnQueryClientUserItem() { return m_pIMySelfUserItem; };

	//�ؼ��¼�
protected:
	//�û�ѡ��
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem);
	//�����¼�
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//�����¼�
public:
	//�����¼�
	VOID OnEventPropertyTrumpet();

	//���溯��
private:
	//�����б�
	VOID InitSilderControl();
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);	

	//�ڲ�����
protected:
	//����Ч��
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//��������
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);
	//���ͱ���
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);

	//��������
protected:
	//��ֹ�û���������
	VOID OnLimitUserRoomChat();
	//�����û���������
	VOID OnAllowUserRoomChat();
	//��ֹ�û���Ϸ����
	VOID OnLimitUserGameChat();
	//�����û���Ϸ����
	VOID OnAllowUserGameChat();
	//��ֹ�û�˽��
	VOID OnLimitUserWhispChat();
	//�����û�˽��
	VOID OnAllowUserWhispChat();

	//��������
protected:
	//������ɫ
	VOID OnSelectMoreColor();
	//ѡ����ɫ
	VOID OnSelectChatColor(UINT nCommandID);
	//ѡ�����
	VOID OnSelectChatShort(UINT nCommandID);

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�Ҽ��б�
	VOID OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//�����б�
	VOID OnNMClickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//˫���б�
	VOID OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif