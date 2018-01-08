#ifndef TABLE_VIEW_FRAME_HEAD_FILE
#define TABLE_VIEW_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define INDEX_ENTER_CHAIR			MAX_CHAIR							//��������

//��ť��ʶ
#define BT_JOIN_MATCH				1									//�������
#define BT_GROUP_MATCH				2									//�������
#define BT_QUIT_MATCH				3									//�˳�����
#define BT_REVIVE_MATH				4									//�������	
#define BT_JOIN_GAME				5									//������Ϸ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagTableAttribute
{
	//���ӱ�־
	bool							bLocker;							//�����־
	bool							bPlaying;							//��Ϸ��־
	bool							bFocusFrame;						//��ܱ�־

	//����״̬
	WORD							wWatchCount;						//�Թ���Ŀ
	DWORD							dwTableOwnerID;						//��������

	//���Ա���
	WORD							wTableID;							//���Ӻ���
	WORD							wChairCount;						//������Ŀ
	IClientUserItem *				pIClientUserItem[MAX_CHAIR];		//�û���Ϣ
};

//����λ��
struct tagMatchPosition
{	
	//ģʽ��Ϣ
	BYTE							cbPostionMode;						//λ��ģʽ

	//����λ��
	CPoint							ptMatchShow;						//չʾλ��
	CPoint							ptMatchText;						//�ı�λ��
	CPoint							ptMatchWaitPlay;					//�ȴ���ʼ

	//����λ��
	CPoint							ptMatchRank;						//����λ��	
	CPoint							ptMatchReward;						//����λ��
	CPoint							ptRewardSize;						//������С
	CPoint							ptRewardOffset;						//ƫ��λ��	

	//��Ϣλ��
	CRect							rcMatchMode;						//����ģʽ
	CRect							rcMatchCondition;					//��ʼ����
	CRect							rcMatchFee;							//��������
	CRect							rcMatchStartTime;					//����ʱ��	
	CRect							rcMatchEndTime;						//����ʱ��
	CRect							rcMatchSingupEnd;					//������ֹ
	CRect							rcStartCount;						//��ʼ����
	CRect							rcPalyCount;						//��Ϸ����
	CRect							rcMatchPlayCount;					//���پ���
	CRect							rcRankingMode;						//������ʽ

	//����λ��
	CPoint							ptNumber1;							//����λ��
	CPoint							ptNumber2;							//����λ��
	CPoint							ptNumber3;							//����λ��

	//��ťλ��
	CPoint							ptMatchJoin;						//�������
	CPoint							ptMatchDetails;						//��������
};

//////////////////////////////////////////////////////////////////////////////////

//������Դ
class CTableResource
{
	//��������
public:
	CSize							m_SizeChair;						//���ӳߴ�
	CSize							m_SizeTable;						//���ӳߴ�
	CSize							m_SizeEnter;						//����ߴ�
	CSize                           m_SizeNumberBK;                     //���ųߴ�
	CSize                           m_SizeNumberNB;                     //���ųߴ�

	//���Ա���
public:
	bool							m_bShowUser;						//��ʾ�û�
	bool							m_bShowEnter;						//��ʾ����
	bool							m_bShowChair;						//��ʾ����
	bool							m_bShowTable;						//��ʾ����
	bool							m_bShowFocus;						//��ʾ����
	bool							m_bCustomUser;						//�Զ��û�
	bool							m_bShowTableID;						//��ʾ����
	bool							m_bRenderImage;						//��Ⱦ��־
	bool							m_bShowCellScore;					//��ʾ��ע

	//��Ŀ����
public:
	WORD							m_wTableItemCount;					//������Ŀ
	WORD							m_wChairItemCount;					//������Ŀ

	//������Ŀ
public:
	WORD							m_wUserGenreCount;					//������Ŀ
	WORD							m_wTableGenreCount;					//������Ŀ
	WORD							m_wChairGenreCount;					//������Ŀ

	//��������
public:
	CPoint							m_ptAnimation;						//����λ��
	INT								m_nAnimationFrame;					//����֡��		

	//λ������
public:
	CPoint							m_ptLock;							//����λ��
	CPoint							m_ptEnter;							//����λ��
	CPoint							m_ptTableID;						//����λ��
	CPoint							m_ptReadyArray[MAX_CHAIR];			//׼��λ��	

	//���λ��
public:
	CRect							m_rcNameArray[MAX_CHAIR];			//����λ��
	CRect							m_rcChairArray[MAX_CHAIR];			//����λ��

	//��ɫ����
public:
	COLORREF						m_crName;							//������ɫ
	COLORREF						m_crMember;							//��Ա��ɫ
	COLORREF						m_crMaster;							//������ɫ

	//��������
public:
	INT								m_nDrawStyle[MAX_CHAIR];			//�����ʽ
	TCHAR							m_szDirectory[MAX_PATH];			//��Դ·��

	//�ⲿ��Դ
public:
	CPngImage						m_ImageEnter;						//����λͼ
	CBitImage						m_ImageChair;						//����λͼ
	CBitImage						m_ImageTable;						//����λͼ	
	CBitImage						m_ImageGround;						//����λͼ
	CBitImage						m_ImageWaitDistribute;				//����λͼ
	CPngImage						m_ImageAnimation;					//���鶯��
	CPngImage						m_ImageCustomUser;					//�Զ�λͼ
	CPngImage						m_ImageCellNumber;					//��עλͼ

	//�ڲ���Դ
public:
	CPngImage						m_ImageReady;						//׼��λͼ
	CPngImage						m_ImageLocker;						//����λͼ
	CPngImage						m_ImageNumberNB;					//����λͼ
	CPngImage						m_ImageNumberBK;					//���뱳��
	//CBitImage						m_ImageUserFace;					//�û�ͷ��
	CPngImage						m_ImgGroupUserFace[255];			//�û�ͷ��

	//��̬����
protected:
	static CTableResource *			m_pTableResource;					//������Դ

	//��������
public:
	//���캯��
	CTableResource();
	//��������
	virtual ~CTableResource();

	//���ܺ���
public:
	//�ͷ���Դ
	VOID DestroyResource();
	//������Դ
	bool LoadResource(LPCTSTR pszResDirectory, WORD wChairCount, bool bAvertCheatMode);	
	//������Դ
	bool UpdateResource(CSkinRenderManager * pSkinRenderManager);

	//��������
public:
	//��ȡ��ֵ
	LONG ReadStringValue(LPCTSTR & pszSring);
	//��ȡ����
	bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//��ȡ����
	bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//��ȡ��ɫ
	bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//��̬����
public:
	static CTableResource * GetInstance() { return m_pTableResource; }
};

//////////////////////////////////////////////////////////////////////////////////

//������ͼ
class CTableView : public ITableView
{
	//״̬����
protected:
	bool							m_bMouseDown;						//�Ƿ���
	WORD							m_wHoverChairID;					//��������	
	LONG							m_lCellScore;						//���ӵ׷�
	tagTableAttribute				m_TableAttribute;					//��������

	//����ӿ�
protected:
	ITableViewFrame *				m_pITableViewFrame;					//���ӽӿ�

	//��������
public:
	//���캯��
	CTableView();
	//��������
	virtual ~CTableView();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this;}
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//��������
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//���ú���
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame);

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);

	//״̬�ӿ�
public:
	//��갴��
	virtual VOID SetMouseDown(bool bMouseDown);
	//������
	virtual VOID SetFocusFrame(bool bFocusFrame);
	//����λ��
	virtual VOID SetHoverStatus(WORD wHoverChairID);
	//����״̬ 
	virtual VOID SetTableStatus(bool bPlaying, bool bLocker,LONG lCellScore);

	//��ѯ�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag() { return m_TableAttribute.bPlaying; }
	//�����־
	virtual bool GetLockerFlag() { return m_TableAttribute.bLocker; }

	//���ܺ���
public:
	//�滭����
	VOID DrawTableView(CDC * pDC, DWORD dwServerRule, CTableResource * pTableResource);
	//�滭����
	VOID DrawTableName(CDC * pDC, WORD wChairID, LPCTSTR pszNickName, CTableResource * pTableResource);
	//�滭ͷ��
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem, CTableResource * pTableResource);
	//�滭ͷ��
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, IClientUserItem * pIClientUserItem, CTableResource * pTableResource);
	//�滭����
	VOID DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign=TA_LEFT);
};

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<CTableView *>		CTableViewArray;					//��������


//////////////////////////////////////////////////////////////////////////////////

//���ӿ��
class CTableViewFrame : public CWnd, public ITableViewFrame
{
	//��Ԫ����
	friend class CTableView;

	//״̬��Ϣ
protected:
	bool							m_bHovering;						//������־
	bool							m_bLMouseDown;						//�Ҽ���־
	bool							m_bRMouseDown;						//�����־
	bool                            m_bWaitDistribute;                  //�ȴ�����
	BYTE							m_cbButtonDown;						//��ť����
	BYTE							m_cbButtonHover;					//��ť����
	INT								m_nAnimalFrameIndex;				//��������

	//��������
protected:
	WORD							m_wDownTableID;						//���Ӻ���
	WORD							m_wDownChairID;						//���Ӻ���

	//���Ա���
protected:
	WORD							m_wTableCount;						//��Ϸ����
	WORD							m_wChairCount;						//������Ŀ
	DWORD							m_dwServerRule;						//�������
	WORD							m_wServerType;						//��������

	//������Ϣ
protected:
	INT								m_nXExcursion;						//����ƫ��
	INT								m_nYExcursion;						//����ƫ��
	INT								m_nXTableCount;						//��������
	INT								m_nYTableCount;						//��������

	//������Ϣ
protected:
	INT								m_nScrollPos;						//����λ��
	INT								m_nScrollPosMax;					//���λ��
	INT								m_nVisibleLineCount;				//�ɼ�����

	//�ؼ�����
public:
	CSkinScrollBar					m_SkinScrollBar;					//��������
	CTableResource					m_TableResource;					//������Դ
	CTableViewArray					m_TableViewArray;					//��������

	//����ӿ�
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//֪ͨ�ӿ�

	//��������
public:
	//���캯��
	CTableViewFrame();
	//��������
	virtual ~CTableViewFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��������
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);
	//��������
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID,LPCTSTR pszResDirectory);

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual WORD GetTableCount() { return m_wTableCount; }
	//������Ŀ
	virtual WORD GetChairCount() { return m_wChairCount; }
	//��ȡ��Դ
	virtual CTableResource * GetTableResource() { return &m_TableResource; }

	//��ͼ�ӿ�
public:
	//��ͼ����
	virtual CWnd * GetViewWnd() { return this; }
	//��ͼ����
	virtual bool OnEventViewData(WORD wCmdID, VOID * pData, WORD wDataSize) { return true; } 

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//״̬�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag(WORD wTableID);
	//�����־
	virtual bool GetLockerFlag(WORD wTableID);
	//������
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame);
	//����״̬ 
	virtual VOID SetTableStatus(bool bWaitDistribute);
	//����״̬ 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker,LONG lCellScore);	

	//��ͼ����
public:
	//���ӿ���
	virtual bool VisibleTable(WORD wTableID);
	//��������
	virtual bool FlashGameTable(WORD wTableID);
	//��������
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID);

	//���ܽӿ�
public:
	//��������
	virtual bool UpdateTableView(WORD wTableID);
	//��ȡ����
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//��������
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��������
protected:
	//���Ӳ���
	WORD SwitchTableID(POINT MousePoint);
	//���Ӳ���
	WORD SwitchChairID(POINT MousePoint);
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	VOID DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign=TA_LEFT);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//������Ϣ
	VOID OnNcDestroy();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);	
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	VOID OnRButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�����Ϣ
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//������Ϣ
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	//�Զ���Ϣ
protected:
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class CMatchViewFrame : public CFGuiWnd, public ITableViewFrame
{
	//��Ԫ����
	friend class CTableView;

	//״̬��Ϣ
protected:
	bool							m_bHovering;						//������־
	BYTE							m_cbButtonDown;						//��ť����
	BYTE							m_cbButtonHover;					//��ť����

	//���Ա���
protected:
	WORD							m_wServerID;						//�����ʶ
	WORD							m_wTableCount;						//��Ϸ����
	WORD							m_wChairCount;						//������Ŀ

	//���Ա���
protected:
	tagGameMatch 					m_GameMatch;						//������Ϣ
	tagMatchPosition				m_MatchPosition;					//����λ��

	//�ؼ�����
protected:	
	CButtonUI *						m_pBtMatchJoin;						//�μӱ���
	CButtonUI *						m_pBtMatchGroup;					//��������
	CButtonUI *						m_pBtMatchRevive;					//��������	
	CButtonUI *						m_pBtMatchQuit;						//�˳�����
	CButtonUI *						m_pBtMatchDetails;					//��������

	//�ؼ�����
public:
	CTableViewArray					m_TableViewArray;					//��������

//	//��������
//public:
//	DWORD							m_dwWaitting;						//�ȴ�����
//	DWORD							m_dwTotalUser;						//��������
//	DWORD							m_dwBestRank;						//��óɼ�
//	DWORD							m_dwMatchCount;						//��������

	//��������
public:
	CTime							m_tStartTime;						//��ʼʱ��
	DWORD							m_dwWaitting;						//�ȴ�����
	DWORD							m_dwTotalUser;						//��������				
	WORD							m_wLackUserCount;					//ȱ������
	BYTE							m_cbMatchStatus;					//����״̬
	BYTE							m_cbUserMatchStatus;				//����״̬

	//��Դ����
public:
	CBitImage						m_ImageGround;						//��������
	CPngImage						m_ImageStatus;						//״̬����
	CPngImage						m_ImageNumber1;						//����ͼƬ
	CPngImage						m_ImageNumber2;						//ʱ������	
	CPngImage						m_ImageMatchRank;					//��������
	CPngImage						m_ImageMatchShow;					//����չʾ
	CPngImage						m_ImageMatchText;					//��������
	CPngImage						m_ImageMatchBack;					//��������	
	CPngImage						m_ImageMatchWaitPlay;				//״̬����

	//����ӿ�
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//֪ͨ�ӿ�

	//��������
public:
	//���캯��
	CMatchViewFrame();
	//��������
	virtual ~CMatchViewFrame();

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

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���ýӿ�
public:
	//��������
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);
	//��������
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID,LPCTSTR pszResDirectory);

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual WORD GetTableCount() { return m_wTableCount; }
	//������Ŀ
	virtual WORD GetChairCount() { return m_wChairCount; }

	//��ͼ�ӿ�
public:
	//��ͼ����
	virtual CWnd * GetViewWnd() { return this; }
	//��ͼ����
	virtual bool OnEventViewData(WORD wCmdID, VOID * pData, WORD wDataSize);

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//״̬�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag(WORD wTableID);
	//�����־
	virtual bool GetLockerFlag(WORD wTableID);
	//������
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame);
	//����״̬ 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker,LONG lCellScore);
	//����״̬ 
	virtual VOID SetTableStatus(bool bWaitDistribute);

	//��ͼ����
public:
	//���ӿ���
	virtual bool VisibleTable(WORD wTableID);
	//��������
	virtual bool FlashGameTable(WORD wTableID);
	//��������
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID);

	//���ܽӿ�
public:
	//��������
	virtual bool UpdateTableView(WORD wTableID);
	//��ȡ����
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//��������
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//��������
protected:			
	//����״̬
	VOID SetMatchStatus(BYTE cbMatchStatus);
	//��Ҳ���״̬
	VOID SetUserMatchStatus(BYTE cbUserMatchStatus);
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//������Դ
	bool LoadResource(LPCTSTR pszResDirectory);
	//�����ؼ�
	bool CreateControlUI(LPCTSTR pszMatchDirectory);
	//�ж�Ŀ¼
	bool IsDirectoryExist(LPCTSTR pszDirectofy);

	//�滭����
protected:
	//��������
	VOID DrawWords(CDC * pDC,INT nXPos,INT nYPos,CPngImage * pWordsImage,LPCTSTR pszWords,LPCTSTR pszText,UINT nFormat=DT_LEFT);
	//����ͼƬ
	VOID DrawNumber(CDC * pDC,INT nXPos,INT nYPos,CPngImage * pNumberImage,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat=DT_LEFT);

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

#endif