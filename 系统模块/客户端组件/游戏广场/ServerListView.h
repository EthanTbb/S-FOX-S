#ifndef SERVER_LIST_VIEW_HEAD
#define SERVER_LIST_VIEW_HEAD

//�����ļ�
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////////

//��������
class ListNode;
class CListItemLabelUI;

//�ڵ�����
struct ListNodeData
{
	int									nLevel;							//�㼶��ʶ		
    bool								bChildVisible;					//��ʾ��ʶ
	UINT								nImageIndex;					//ͼ������
	//TCHAR								szItemTitle[64];				//�������
	CListItemLabelUI *					pListItem;						//�б�����
	CGameListItem *						pGameListItem;					//��������    
}; 

//���Ͷ���
typedef CList<WORD>	 CServerIDList;										//�����ʶ
typedef CWHArray<ListNode*,ListNode*>	Children;						//�б�ڵ�	
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//��Ϸͼ��

//////////////////////////////////////////////////////////////////////////////////////
//�ӿڶ���
interface IListItemSink
{
	//�滭����
	virtual void DrawListItem(HDC hDC, const RECT& rcPaint, CListItemLabelUI * pTreeItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////////
//�б�ڵ�
class ListNode
{
	//��������
protected:
    Children							m_Children;						//�ӽڵ�		
    ListNode*							m_pParent;						//���ڵ�
    ListNodeData						m_NodeData;						//�ڵ�����

	//��������
public:
	//���캯��
	ListNode();
	//���캯��
    explicit ListNode(ListNodeData Data);
	//���캯��
    ListNode(ListNodeData Data, ListNode * parent);
	//��������
    virtual ~ListNode(); 

	//˽�к���
private:
	//���ýڵ�
    void SetParent(ListNode* pParent) { m_pParent=pParent; }

	//���ܺ���
public: 
	//�ڵ�����
    ListNodeData & Data() { return m_NodeData; }	
	//�ӽڵ���Ŀ
	int NumChildren() const { return (int)m_Children.GetCount(); }
	//�ӽڵ�
    ListNode * Child(int i)	{ return m_Children[i]; }
	//���ڵ�
    ListNode * Parent() { return m_pParent;	}
	//�ڵ��ʶ
    bool HasChildren() const {	return NumChildren()>0; }	

	//��������
public: 
	//��ȡ�ڵ�
    ListNode * GetLastChild();	
	//����ӽڵ�
    void AddChild(ListNode * pChild);
	//����ӽڵ�
    void AddChildAt(ListNode * pChild,int nIndex);	
	//�Ƴ��ӽڵ�
    void RemoveChildAt(int nIndex);
	//�Ƴ��ӽڵ�
    void RemoveChild(ListNode* pChild);		
};

//////////////////////////////////////////////////////////////////////////////////////
//�б��ǩ
class CListItemLabelUI : public CListLabelElementUI
{
	//�ӿڱ��� 
protected:
	IListItemSink *						m_IListItemSink;				//�������	

	//��������
public:
	//���캯��
	CListItemLabelUI(IListItemSink * pIListItemSink);

	//���ܺ���
public:
	//��ȡ����
	LPCTSTR GetClass() const;
	//��ȡ�ӿ�
    LPVOID GetInterface(LPCTSTR pstrName);
	//�����С
	SIZE EstimateSize(SIZE szAvailable);
	//�滭����
    void DoPaint(HDC hDC, const RECT& rcPaint);
};

//////////////////////////////////////////////////////////////////////////////////////
//�б�ؼ�
class CServerListUI : public CListUI, public IListItemSink
{
	//��������
private:
	ListNode *							m_RootNode;							//�����ڵ�
	ListNode *							m_ListAssistant;					//�����ڵ�
	CGameTypeItem						m_LastServerItem;                   //��������
	CGameTypeItem *						m_MatchCenterItem;					//��������

	//��������
protected:    
    DWORD								m_dwDelayNum;						//��ʱ����
    DWORD								m_dwDelayLeft;						//��ʱʣ��
	LONG								m_dwDelayDeltaY;					//��ʱ����

	//��Դ����
protected:		
	CImageList							m_ImageList;						//ͼƬ��Դ
	CPngImage							m_ImageExpand;						//չ��ͼ��
	CGameImageMap						m_GameImageMap;						//ͼ������
	CServerIDList						m_LastPlayGameList;                 //��������Ϸ

	//��������
public:
	//���캯��
    CServerListUI();
	//��������
    virtual ~CServerListUI();

	//�¼�����
public:
	//�¼�����
	void DoEvent(TEventUI& event);		

	//���ܺ���
public:	
	//�Ƴ��ؼ�
	void RemoveAll();
	//�Ƴ��ؼ�
	bool RemoveAt(int iIndex);
	//�Ƴ��ؼ�
	bool Remove(CControlUI* pControl);
	//��ӿؼ�
	bool Add(CControlUI* pControl);
	//��ӿؼ�
	bool AddAt(CControlUI* pControl, int iIndex);
	//��ȡ���ڵ�
    ListNode * GetRoot() { return m_RootNode; }	
	//�Ƴ��ڵ�
	void RemoveNode(ListNode * pNode);
	//��������
	void SetChildVisible(ListNode * pNode,bool bVisible,bool bUpdateVisible=true);	
	//��ӽڵ�
	ListNode * AddInsideNode(LPCTSTR pszTitle,UINT nImageIndex,DWORD dwInsideID,ListNode * pParent);
	//��ӽڵ�
	ListNode * AddListNode(LPCTSTR pszTitle,UINT nImageIndex,CGameListItem * pGameListItem, ListNode * pParent);	

	//�ӿں���
public:
	//�滭����
	virtual void DrawListItem(HDC hDC, const RECT& rcPaint, CListItemLabelUI * pTreeItem);

	//״̬֪ͨ
public:
	//���֪ͨ
	VOID OnGameItemFinish();	
	//���֪ͨ
	VOID OnGameMatchFinish();	
	//����֪ͨ
	VOID OnGameItemUpdateFinish();
	//���֪ͨ
	VOID OnGameKindFinish(WORD wKindID);

	//����֪ͨ
public:
	//����֪ͨ
	VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//ͼ�꺯��
private:
	//��ȡͼ��
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//��ȡͼ��
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//���⺯��
private:
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//���ܺ���
public:
	//��Ӽ�¼
	VOID AddLastPlayGame(WORD wServerID, CGameServerItem *pGameServerItem);

	//���ú���
public:
	//���ú���
	VOID InitServerListUI();
	//����ͨ��
	VOID InitAssistantItem();

	//��������
public:
	//���ؼ�¼
	VOID LoadLastPlayGame();
	//�����¼
	VOID SaveLastPlayGame();	
	//�жϼ�¼
	bool IsLastPlayGame(WORD wServerID);	
	//ö�ټ�¼
	ListNode * EmunGameServerItem(ListNode * pParentNode, WORD wServerID);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize);

	//��������
public:		
	//������ʱ
	inline double CalculateDelay(double state);
	//��ȡչ����С
	SIZE GetExpanderSizeX(ListNode * pNode);
	//ɾ������
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//�޸�����
	VOID ModifyGameListItem(ListNode * pListNode, LPCTSTR pszTitle, UINT uImage);
};

//////////////////////////////////////////////////////////////////////////////////////
//�����б�
class CLMTPlazaServerList : public CFGuiWnd, public IServerListDataSink
{
	//����ؼ�
public:
	CServerListUI *					m_pServerListUI;				    //�б�ؼ�

	//�ؼ�����
public:
	CEncircleBMP					m_FrameEncircle;					//������Ϣ

	//��������
public:
	//���캯��
	CLMTPlazaServerList();
	//��������
	virtual ~CLMTPlazaServerList();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//�����ؼ�
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("PlazaServerList.xml"); }

	//�¼�֪ͨ
public:
	//����׼��
	virtual VOID OnDataReceivePrepare();

	//״̬֪ͨ
public:
	//���֪ͨ
	virtual VOID OnGameItemFinish();	
	//���֪ͨ
	virtual VOID OnGameMatchFinish();	
	//����֪ͨ
	virtual VOID OnGameItemUpdateFinish();
	//���֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//���ܺ���
public:
	//��Ӽ�¼
	VOID AddLastPlayGame(WORD wServerID, CGameServerItem *pGameServerItem);

	//��Ϣ����
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////////


#endif
