#ifndef SERVER_LIST_VIEW_HEAD
#define SERVER_LIST_VIEW_HEAD

//引入文件
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////////

//类型声明
class ListNode;
class CListItemLabelUI;

//节点数据
struct ListNodeData
{
	int									nLevel;							//层级标识		
    bool								bChildVisible;					//显示标识
	UINT								nImageIndex;					//图像索引
	//TCHAR								szItemTitle[64];				//子项标题
	CListItemLabelUI *					pListItem;						//列表子项
	CGameListItem *						pGameListItem;					//子项数据    
}; 

//类型定义
typedef CList<WORD>	 CServerIDList;										//房间标识
typedef CWHArray<ListNode*,ListNode*>	Children;						//列表节点	
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//游戏图标

//////////////////////////////////////////////////////////////////////////////////////
//接口定义
interface IListItemSink
{
	//绘画背景
	virtual void DrawListItem(HDC hDC, const RECT& rcPaint, CListItemLabelUI * pTreeItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////////
//列表节点
class ListNode
{
	//变量定义
protected:
    Children							m_Children;						//子节点		
    ListNode*							m_pParent;						//父节点
    ListNodeData						m_NodeData;						//节点数据

	//函数定义
public:
	//构造函数
	ListNode();
	//构造函数
    explicit ListNode(ListNodeData Data);
	//构造函数
    ListNode(ListNodeData Data, ListNode * parent);
	//析构函数
    virtual ~ListNode(); 

	//私有函数
private:
	//设置节点
    void SetParent(ListNode* pParent) { m_pParent=pParent; }

	//功能函数
public: 
	//节点数据
    ListNodeData & Data() { return m_NodeData; }	
	//子节点数目
	int NumChildren() const { return (int)m_Children.GetCount(); }
	//子节点
    ListNode * Child(int i)	{ return m_Children[i]; }
	//父节点
    ListNode * Parent() { return m_pParent;	}
	//节点标识
    bool HasChildren() const {	return NumChildren()>0; }	

	//操作函数
public: 
	//获取节点
    ListNode * GetLastChild();	
	//添加子节点
    void AddChild(ListNode * pChild);
	//添加子节点
    void AddChildAt(ListNode * pChild,int nIndex);	
	//移除子节点
    void RemoveChildAt(int nIndex);
	//移除子节点
    void RemoveChild(ListNode* pChild);		
};

//////////////////////////////////////////////////////////////////////////////////////
//列表标签
class CListItemLabelUI : public CListLabelElementUI
{
	//接口变量 
protected:
	IListItemSink *						m_IListItemSink;				//子项核心	

	//函数定义
public:
	//构造函数
	CListItemLabelUI(IListItemSink * pIListItemSink);

	//功能函数
public:
	//获取类名
	LPCTSTR GetClass() const;
	//获取接口
    LPVOID GetInterface(LPCTSTR pstrName);
	//估算大小
	SIZE EstimateSize(SIZE szAvailable);
	//绘画背景
    void DoPaint(HDC hDC, const RECT& rcPaint);
};

//////////////////////////////////////////////////////////////////////////////////////
//列表控件
class CServerListUI : public CListUI, public IListItemSink
{
	//变量定义
private:
	ListNode *							m_RootNode;							//根部节点
	ListNode *							m_ListAssistant;					//辅助节点
	CGameTypeItem						m_LastServerItem;                   //房间子项
	CGameTypeItem *						m_MatchCenterItem;					//比赛子项

	//动画变量
protected:    
    DWORD								m_dwDelayNum;						//延时数量
    DWORD								m_dwDelayLeft;						//延时剩余
	LONG								m_dwDelayDeltaY;					//延时增量

	//资源变量
protected:		
	CImageList							m_ImageList;						//图片资源
	CPngImage							m_ImageExpand;						//展开图像
	CGameImageMap						m_GameImageMap;						//图形索引
	CServerIDList						m_LastPlayGameList;                 //最近玩过游戏

	//函数定义
public:
	//构造函数
    CServerListUI();
	//析构函数
    virtual ~CServerListUI();

	//事件函数
public:
	//事件处理
	void DoEvent(TEventUI& event);		

	//功能函数
public:	
	//移除控件
	void RemoveAll();
	//移除控件
	bool RemoveAt(int iIndex);
	//移除控件
	bool Remove(CControlUI* pControl);
	//添加控件
	bool Add(CControlUI* pControl);
	//添加控件
	bool AddAt(CControlUI* pControl, int iIndex);
	//获取根节点
    ListNode * GetRoot() { return m_RootNode; }	
	//移除节点
	void RemoveNode(ListNode * pNode);
	//可视设置
	void SetChildVisible(ListNode * pNode,bool bVisible,bool bUpdateVisible=true);	
	//添加节点
	ListNode * AddInsideNode(LPCTSTR pszTitle,UINT nImageIndex,DWORD dwInsideID,ListNode * pParent);
	//添加节点
	ListNode * AddListNode(LPCTSTR pszTitle,UINT nImageIndex,CGameListItem * pGameListItem, ListNode * pParent);	

	//接口函数
public:
	//绘画子项
	virtual void DrawListItem(HDC hDC, const RECT& rcPaint, CListItemLabelUI * pTreeItem);

	//状态通知
public:
	//完成通知
	VOID OnGameItemFinish();	
	//完成通知
	VOID OnGameMatchFinish();	
	//更新通知
	VOID OnGameItemUpdateFinish();
	//完成通知
	VOID OnGameKindFinish(WORD wKindID);

	//更新通知
public:
	//插入通知
	VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//图标函数
private:
	//获取图标
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//获取图标
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//标题函数
private:
	//获取标题
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//获取标题
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//功能函数
public:
	//添加记录
	VOID AddLastPlayGame(WORD wServerID, CGameServerItem *pGameServerItem);

	//配置函数
public:
	//配置函数
	VOID InitServerListUI();
	//快速通道
	VOID InitAssistantItem();

	//辅助函数
public:
	//加载记录
	VOID LoadLastPlayGame();
	//保存记录
	VOID SaveLastPlayGame();	
	//判断记录
	bool IsLastPlayGame(WORD wServerID);	
	//枚举记录
	ListNode * EmunGameServerItem(ListNode * pParentNode, WORD wServerID);
	//获得房间负载信息
	LPCTSTR GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize);
	//获得房间负载信息
	LPCTSTR GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize);
	//获得房间负载信息
	LPCTSTR GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize);

	//辅助函数
public:		
	//计算延时
	inline double CalculateDelay(double state);
	//获取展开大小
	SIZE GetExpanderSizeX(ListNode * pNode);
	//删除更新
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//修改子项
	VOID ModifyGameListItem(ListNode * pListNode, LPCTSTR pszTitle, UINT uImage);
};

//////////////////////////////////////////////////////////////////////////////////////
//房间列表
class CLMTPlazaServerList : public CFGuiWnd, public IServerListDataSink
{
	//界面控件
public:
	CServerListUI *					m_pServerListUI;				    //列表控件

	//控件变量
public:
	CEncircleBMP					m_FrameEncircle;					//环绕信息

	//函数定义
public:
	//构造函数
	CLMTPlazaServerList();
	//析构函数
	virtual ~CLMTPlazaServerList();

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("PlazaServerList.xml"); }

	//事件通知
public:
	//接收准备
	virtual VOID OnDataReceivePrepare();

	//状态通知
public:
	//完成通知
	virtual VOID OnGameItemFinish();	
	//完成通知
	virtual VOID OnGameMatchFinish();	
	//更新通知
	virtual VOID OnGameItemUpdateFinish();
	//完成通知
	virtual VOID OnGameKindFinish(WORD wKindID);

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//功能函数
public:
	//添加记录
	VOID AddLastPlayGame(WORD wServerID, CGameServerItem *pGameServerItem);

	//消息函数
protected:
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////////


#endif
