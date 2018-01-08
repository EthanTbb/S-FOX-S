#ifndef UI_FRFIENDS_HEAD_FILE
#define UI_FRFIENDS_HEAD_FILE

//�����ļ�
#include <math.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//��������
struct tagTaskListItemInfo
{
	bool								bIsFolder;							//Ŀ¼��ʶ
	bool								bIsEmpty;							//�հױ�ʶ
	bool								bShowStatus;						//��ʾ״̬
	WORD								wTaskID;							//�����ʶ
	BYTE								cbTaskStatus;						//����״̬
	TCHAR								szTaskName[128];					//��������
};

//�ڵ�����
struct NodeData
{
	int									nlevelID;							//�ȼ�ID
	bool								bIsFolder;							//Ŀ¼��ʶ
	bool								bChildVisible;						//��ʾ�ӽڵ�
	bool								bHasChild;							//ӵ���ӽڵ�
	bool								bShowStatus;						//��ʾ״̬
	WORD								wTaskID;							//�����ʶ
	BYTE								cbTaskStatus;						//����״̬
	TCHAR								szTaskName[128];					//��������	
	CListContainerElementUI*			plistElment;						//Ԫ��ָ��
};

double CalculateDelay(double state);

//��������
class Node;
class CTaskUI;

//���Ͷ���
typedef std::vector <Node*>				CNodeVector;

////////////////////////////////////////////////////////////////////////////////////////////////

//����ڵ�
class Node
{
	//��������
private:
	CNodeVector							m_ChildrenList;						//�����б�
	Node*								m_pParent;							//�ڵ�ָ��
	NodeData							m_NodeData;							//�ڵ�����

	//��������
public:
	//���캯��
	Node();
	//���캯��
	explicit Node(NodeData t);
	//���캯��
	Node(NodeData t, Node* parent);
	//��������
	~Node();

	//���ܺ���
public:
	//��ȡ����
	NodeData & Data();	
	//��ȡ�ӽڵ�
	Node* Child(int i);
	//��ȡ���ڵ�
	Node* Parent();
	//��ȡ�ӽڵ�
	Node* GetLastChild();
	//Ŀ¼��ʶ
	bool IsFolder() const;	
	//�ӽڵ�
	bool HasChildren() const;
	//������Ŀ
	int NumOfChildren() const;
	//��ӽڵ�
	void AddChild(Node* child);
	//�Ƴ��ڵ�
	void RemoveChild(Node* child);	

	//��������
private:
	//���ø��ڵ�
	void SetParent(Node* parent);
};

////////////////////////////////////////////////////////////////////////////////////////////////
//����ؼ�
class CTaskUI : public CListUI
{
	//��������
private:
	Node*								m_pRootNode;						//���ڵ�

	//��������
protected:
	LONG								m_lDelayDeltaY;						//��ʱ����
	DWORD								m_dwDelayNumber;					//��ʾ����
	DWORD								m_dwDelayLeft;						//ʣ����ʱ

	//λ�ñ���
protected:	
	int									m_LevelTextStartPos;				//��ʼλ��
	int									m_LevelTextStartPosFolder;			//��ʼλ��
	int									m_LevelTextStartPosChild;			//��ʼλ��
	int									m_LevelTextStartPosStatus;			//��ʼλ��

	//��Դ����
protected:
	CPaintManagerUI &					m_PaintManager;						//��Ⱦ����
    CDialogBuilder						m_DlgBuilder;						//��������

	//��������
public:
	//���캯��
	CTaskUI(CPaintManagerUI& PaintManager);
	//��������
	~CTaskUI();

	//���ܺ���
public:
	//��ӿؼ�
	bool Add(CControlUI* pControl);
	//��ӿؼ�
	bool AddAt(CControlUI* pControl, int iIndex);
	//�Ƴ��ؼ�
	bool Remove(CControlUI* pControl);
	//�Ƴ�����
	bool RemoveAt(int iIndex);
	//�Ƴ�����
	void RemoveAll();
	//�¼�����
	void DoEvent(TEventUI& event);
	//��ȡ�ڵ�
	Node* GetRoot();
	//��ӽڵ�
	Node* AddNode(const tagTaskListItemInfo& item, Node* parent = NULL);
	//�Ƴ��ڵ�
	void RemoveNode(Node* node);
	//���ҽڵ�
	Node* SearchNode(Node* Parent,WORD wTaskID);
	//��������
	void SetChildVisible(Node* node, bool visible);
	//չ���ж�
	bool CanExpand(Node* node) const;
	//���½ڵ�
	void UpdateNode(Node* node);
	//ѡ������
	bool SelectItem(int iIndex);
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif 