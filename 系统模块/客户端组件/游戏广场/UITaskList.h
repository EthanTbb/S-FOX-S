#ifndef UI_FRFIENDS_HEAD_FILE
#define UI_FRFIENDS_HEAD_FILE

//引入文件
#include <math.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//任务子项
struct tagTaskListItemInfo
{
	bool								bIsFolder;							//目录标识
	bool								bIsEmpty;							//空白标识
	bool								bShowStatus;						//显示状态
	WORD								wTaskID;							//任务标识
	BYTE								cbTaskStatus;						//任务状态
	TCHAR								szTaskName[128];					//任务名称
};

//节点数据
struct NodeData
{
	int									nlevelID;							//等级ID
	bool								bIsFolder;							//目录标识
	bool								bChildVisible;						//显示子节点
	bool								bHasChild;							//拥有子节点
	bool								bShowStatus;						//显示状态
	WORD								wTaskID;							//任务标识
	BYTE								cbTaskStatus;						//任务状态
	TCHAR								szTaskName[128];					//子项描述	
	CListContainerElementUI*			plistElment;						//元素指针
};

double CalculateDelay(double state);

//类型声明
class Node;
class CTaskUI;

//类型定义
typedef std::vector <Node*>				CNodeVector;

////////////////////////////////////////////////////////////////////////////////////////////////

//任务节点
class Node
{
	//变量定义
private:
	CNodeVector							m_ChildrenList;						//子项列表
	Node*								m_pParent;							//节点指针
	NodeData							m_NodeData;							//节点数据

	//函数定义
public:
	//构造函数
	Node();
	//构造函数
	explicit Node(NodeData t);
	//构造函数
	Node(NodeData t, Node* parent);
	//析构函数
	~Node();

	//功能函数
public:
	//获取数据
	NodeData & Data();	
	//获取子节点
	Node* Child(int i);
	//获取父节点
	Node* Parent();
	//获取子节点
	Node* GetLastChild();
	//目录标识
	bool IsFolder() const;	
	//子节点
	bool HasChildren() const;
	//子项数目
	int NumOfChildren() const;
	//添加节点
	void AddChild(Node* child);
	//移除节点
	void RemoveChild(Node* child);	

	//辅助函数
private:
	//设置父节点
	void SetParent(Node* parent);
};

////////////////////////////////////////////////////////////////////////////////////////////////
//任务控件
class CTaskUI : public CListUI
{
	//变量定义
private:
	Node*								m_pRootNode;						//根节点

	//滚动变量
protected:
	LONG								m_lDelayDeltaY;						//延时增量
	DWORD								m_dwDelayNumber;					//演示数字
	DWORD								m_dwDelayLeft;						//剩余延时

	//位置变量
protected:	
	int									m_LevelTextStartPos;				//开始位置
	int									m_LevelTextStartPosFolder;			//开始位置
	int									m_LevelTextStartPosChild;			//开始位置
	int									m_LevelTextStartPosStatus;			//开始位置

	//资源变量
protected:
	CPaintManagerUI &					m_PaintManager;						//渲染管理
    CDialogBuilder						m_DlgBuilder;						//创建对象

	//函数定义
public:
	//构造函数
	CTaskUI(CPaintManagerUI& PaintManager);
	//析构函数
	~CTaskUI();

	//功能函数
public:
	//添加控件
	bool Add(CControlUI* pControl);
	//添加控件
	bool AddAt(CControlUI* pControl, int iIndex);
	//移除控件
	bool Remove(CControlUI* pControl);
	//移除子项
	bool RemoveAt(int iIndex);
	//移除子项
	void RemoveAll();
	//事件处理
	void DoEvent(TEventUI& event);
	//获取节点
	Node* GetRoot();
	//添加节点
	Node* AddNode(const tagTaskListItemInfo& item, Node* parent = NULL);
	//移除节点
	void RemoveNode(Node* node);
	//查找节点
	Node* SearchNode(Node* Parent,WORD wTaskID);
	//可视设置
	void SetChildVisible(Node* node, bool visible);
	//展开判断
	bool CanExpand(Node* node) const;
	//更新节点
	void UpdateNode(Node* node);
	//选择子项
	bool SelectItem(int iIndex);
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif 