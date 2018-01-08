#include "stdafx.h"
#include "UITaskList.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//��������
const int nScrollTextMerid = 10;
const int nFriendListItemNormalHeight = 22;
const int nFriendListItemSelectedHeight = 22;

//�ؼ�����
const TCHAR* const szTaskNameControlName = TEXT("Taskname");

//ͼƬ����
const TCHAR* const szLevelExpandImage=TEXT("<i file='TASK_ARROW_EXPAND' restype='PNG'>");
const TCHAR* const szLevelCollapseImage=TEXT("<i file='TASK_ARROW_COLLPASE' restype='PNG'>");

/////////////////////////////////////////////////////////////////////////////////////////////

//������ʱ
double CalculateDelay(double state)
{
    return pow(state, 2);
};

//���캯��
Node::Node() : m_pParent(NULL)
{
}

//���캯��
Node::Node(NodeData t) : m_NodeData(t),m_pParent(NULL)
{
}

//���캯��
Node::Node(NodeData t, Node* parent) : m_NodeData(t),m_pParent(parent)
{
}

//��������
Node::~Node() 
{
	for(int i=0;i<NumOfChildren();++i)
	{
		SafeDelete(m_ChildrenList[i]); 
	}
}

//�ڵ�����
NodeData& Node::Data()
{
	return m_NodeData;
}

//�ӽڵ���Ŀ
int Node::NumOfChildren() const
{
	return static_cast<int>(m_ChildrenList.size());
}

//��ȡ�ӽڵ�
Node* Node::Child(int i)
{
	return m_ChildrenList[i];
}

//��ȡ���ڵ�
Node* Node::Parent()
{
	return m_pParent;
}

//
bool Node::HasChildren() const
{
	return NumOfChildren() > 0;
}

//Ŀ¼��ʶ
bool Node::IsFolder() const
{
	return m_NodeData.bIsFolder;
}

//����ӽڵ�
void Node::AddChild(Node* child)
{
	child->SetParent(this); 
	m_ChildrenList.push_back(child); 
}

//�Ƴ��ӽڵ�
void Node::RemoveChild(Node* child)
{
	CNodeVector::iterator iter = m_ChildrenList.begin();
	for (; iter < m_ChildrenList.end(); ++iter)
	{
		if (*iter == child) 
		{
			m_ChildrenList.erase(iter);
			return;
		}
	}
}

//��ȡ�ڵ�
Node* Node::GetLastChild()
{
	if (NumOfChildren())
	{
		return Child(NumOfChildren()-1)->GetLastChild();
	}
	return this;
}

//���ø��ڵ�
void Node::SetParent(Node* parent)
{
	m_pParent = parent;
}

/////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CTaskUI::CTaskUI(CPaintManagerUI& PaintManager)
	: m_pRootNode(NULL)
	, m_lDelayDeltaY(0)
	, m_dwDelayNumber(0)
	, m_dwDelayLeft(0)
	, m_LevelTextStartPos(4)
	, m_LevelTextStartPosFolder(4)
	, m_LevelTextStartPosChild(14)
	, m_LevelTextStartPosStatus(4)
	, m_PaintManager(PaintManager)
{
	//��������
	SetItemShowHtml(true);

	//�������ڵ�
	m_pRootNode = new Node;
	m_pRootNode->Data().nlevelID = -1;
	m_pRootNode->Data().bChildVisible = true;
	m_pRootNode->Data().bHasChild = true;
	m_pRootNode->Data().plistElment = NULL;
}

//��������
CTaskUI::~CTaskUI()
{
	if (m_pRootNode) SafeDelete(m_pRootNode);
}

//����ӽڵ�
bool CTaskUI::Add(CControlUI* pControl)
{
	if (!pControl) return false;

	if (lstrcmp(pControl->GetClass(), TEXT("ListContainerElementUI")) == 0)
		return false;

	return CListUI::Add(pControl);
}

//����ӽڵ�
bool CTaskUI::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl) return false;

	if (lstrcmp(pControl->GetClass(), TEXT("ListContainerElementUI")) == 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

//�Ƴ��ڵ�
bool CTaskUI::Remove(CControlUI* pControl)
{
	if (!pControl) return false;

	if (lstrcmp(pControl->GetClass(), TEXT("ListContainerElementUI")) == 0)
		return false;

	return CListUI::Remove(pControl);
}

//�Ƴ��ڵ�
bool CTaskUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)	return false;

	if (lstrcmp(pControl->GetClass(), TEXT("ListContainerElementUI")) == 0)
		return false;

	return CListUI::RemoveAt(iIndex);
}

//�Ƴ��ڵ�
void CTaskUI::RemoveAll()
{
	CListUI::RemoveAll();
	SafeDelete(m_pRootNode);

	//�������ڵ�
	m_pRootNode = new Node;
	m_pRootNode->Data().nlevelID = -1;
	m_pRootNode->Data().bChildVisible = true;
	m_pRootNode->Data().bHasChild = true;
	m_pRootNode->Data().plistElment = NULL;
}

//�¼�����
void CTaskUI::DoEvent(TEventUI& event) 
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
			m_pParent->DoEvent(event);
		else
			CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_TIMER && event.wParam == nScrollTextMerid)
	{
		if (m_dwDelayLeft > 0)
		{
			--m_dwDelayLeft;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)m_dwDelayLeft / m_dwDelayNumber) * m_lDelayDeltaY);
			if ((lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ))
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		m_lDelayDeltaY = 0;
		m_dwDelayNumber = 0;
		m_dwDelayLeft = 0;
		m_pManager->KillTimer(this, nScrollTextMerid);
		return;
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (m_dwDelayNumber > 0)
			lDeltaY =  (LONG)(CalculateDelay((double)m_dwDelayLeft / m_dwDelayNumber) * m_lDelayDeltaY);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (m_lDelayDeltaY >= 0)
				m_lDelayDeltaY = lDeltaY + 8;
			else
				m_lDelayDeltaY = lDeltaY + 12;
			break;
		case SB_LINEDOWN:
			if (m_lDelayDeltaY <= 0)
				m_lDelayDeltaY = lDeltaY - 8;
			else
				m_lDelayDeltaY = lDeltaY - 12;
			break;
		}
		if
			(m_lDelayDeltaY > 100) m_lDelayDeltaY = 100;
		else if
			(m_lDelayDeltaY < -100) m_lDelayDeltaY = -100;

		m_dwDelayNumber = (DWORD)sqrt((double)abs(m_lDelayDeltaY)) * 5;
		m_dwDelayLeft = m_dwDelayNumber;
		m_pManager->SetTimer(this, nScrollTextMerid, 50U);
		return;
	}

	CListUI::DoEvent(event);
}

//��ȡ���ڵ�
Node* CTaskUI::GetRoot()
{
	return m_pRootNode;
}

//��ť�¼�
static bool OnLogoButtonEvent(void* event) 
{
    if( ((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN ) 
	{
        CControlUI* pButton = ((TEventUI*)event)->pSender;
        if( pButton != NULL ) 
		{
            CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
            if( pListElement != NULL ) pListElement->DoEvent(*(TEventUI*)event);
        }
    }
    return true;
}

//��ӽڵ�
Node* CTaskUI::AddNode(const tagTaskListItemInfo& item, Node* parent)
{
	//�������ڵ�
	if (!parent) parent = m_pRootNode;

	//��������
	TCHAR szBuffer[MAX_PATH] = {0};
    CListContainerElementUI* pListElement = NULL;

	//��������
    if(m_DlgBuilder.GetMarkup()==NULL || !m_DlgBuilder.GetMarkup()->IsValid()) 
	{
        pListElement = static_cast<CListContainerElementUI*>(m_DlgBuilder.Create(TEXT("TaskListItem.xml"),(UINT)0,NULL,&m_PaintManager));
    }
    else 
	{
        pListElement = static_cast<CListContainerElementUI*>(m_DlgBuilder.Create((UINT)0, &m_PaintManager));
    }
    if (pListElement == NULL) return NULL;

	//�����ڵ�
	Node* node = new Node;
	if(node==NULL) return NULL;
	
	//���ýڵ�
	node->Data().nlevelID = parent->Data().nlevelID + 1;
	if (item.bIsFolder)
		node->Data().bHasChild = !item.bIsEmpty;
	else
		node->Data().bHasChild = false;

	//���ýڵ�
	node->Data().bIsFolder = item.bIsFolder;
	node->Data().bShowStatus = item.bShowStatus;
	node->Data().wTaskID = item.wTaskID;
	node->Data().cbTaskStatus = item.cbTaskStatus;
	node->Data().bChildVisible = (node->Data().nlevelID == 0);
	node->Data().plistElment = pListElement;	
	lstrcpyn(node->Data().szTaskName,item.szTaskName,CountArray(node->Data().szTaskName));

	if (!parent->Data().bChildVisible) pListElement->SetVisible(false);
	if (parent != m_pRootNode && !parent->Data().plistElment->IsVisible()) pListElement->SetVisible(false);

	////ƴ���ı�
	//CString strHtmlText;
	//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>"),m_LevelTextStartPos);
	//strHtmlText += szBuffer;
	//if (node->Data().bHasChild)
	//{
	//	if (node->Data().bChildVisible)
	//		strHtmlText += szLevelExpandImage;
	//	else
	//		strHtmlText += szLevelCollapseImage;

	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>"),m_LevelTextStartPosFolder);
	//}
	//else
	//{
	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>"),m_LevelTextStartPosChild);		
	//}	

	////Ŀ¼�ڵ�
	//strHtmlText += szBuffer;
	//strHtmlText += node->Data().szTaskName;
	//if (node->Data().bHasChild)
	//{
	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>(0)"),m_LevelTextStartPosStatus);
	//	strHtmlText+=szBuffer;
	//}

	//if (item.szTaskName[0]!=0)
	//{
	//	CLabelUI* pLabelDescription = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, szTaskNameControlName));
	//	if (pLabelDescription != NULL)
	//	{
	//		pLabelDescription->SetShowHtml(true);
	//		pLabelDescription->SetText(strHtmlText);
	//	}
	//}

	pListElement->SetFixedHeight(nFriendListItemNormalHeight);
	pListElement->SetTag((UINT_PTR)node);
	int index = 0;
	if (parent->HasChildren())
	{
		Node* prev = parent->GetLastChild();
		index = prev->Data().plistElment->GetIndex() + 1;
	}
	else 
	{
		if (parent == m_pRootNode) index = 0;
		else index = parent->Data().plistElment->GetIndex() + 1;
	}
	if (!CListUI::AddAt(pListElement, index))
	{
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->AddChild(node);
	UpdateNode(node);
	if(parent->Data().nlevelID>=0) UpdateNode(parent);

	return node;
}

//�Ƴ��ڵ�
void CTaskUI::RemoveNode(Node* node)
{
	if (!node || node == m_pRootNode) return;

	for (int i = 0; i < node->NumOfChildren(); ++i)
	{
		Node* child = node->Child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->Data().plistElment);
	node->Parent()->RemoveChild(node);
	SafeDelete(node);
}

//���ҽڵ�
Node* CTaskUI::SearchNode(Node* Parent,WORD wTaskID)
{
	if (Parent==NULL || Parent == m_pRootNode) return NULL;

	for (int i = 0; i < Parent->NumOfChildren(); ++i)
	{
		Node* child = Parent->Child(i);
		if(child->Data().wTaskID==wTaskID) return child;
	}

	return NULL;
}

//��������
void CTaskUI::SetChildVisible(Node* node, bool visible)
{
	if (!node || node == m_pRootNode)
		return;

	if (node->Data().bChildVisible == visible)
		return;

	node->Data().bChildVisible = visible;
	UpdateNode(node);

	if (!node->Data().plistElment->IsVisible())
		return;

	if (!node->HasChildren())
		return;

	Node* begin = node->Child(0);
	Node* end = node->GetLastChild();
	for (int i = begin->Data().plistElment->GetIndex(); i <= end->Data().plistElment->GetIndex(); ++i)
	{
		CControlUI* control = GetItemAt(i);
		if (lstrcmp(control->GetClass(), TEXT("ListContainerElementUI")) == 0)
		{
			if (visible) 
			{
				Node* localparent = ((Node*)control->GetTag())->Parent();
				if (localparent->Data().bChildVisible && localparent->Data().plistElment->IsVisible())
				{
					control->SetVisible(true);
				}
			}
			else
			{
				control->SetVisible(false);
			}
		}
	}
}

//չ���ж�
bool CTaskUI::CanExpand(Node* node) const
{
	if (!node || node == m_pRootNode)
		return false;

	return node->Data().bHasChild;
}

//���½ڵ�
void CTaskUI::UpdateNode(Node* node)
{
	if(node==NULL) return;

	//��������
	CString strHtmlText;
	TCHAR szBuffer[MAX_PATH] = {0};	

	//��ȡ����
	NodeData TaskNodeData=node->Data();

	//��ʼλ��
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>"),m_LevelTextStartPos);
	strHtmlText += szBuffer;

	//����λ��
	if (TaskNodeData.bIsFolder==true)
	{
		if (TaskNodeData.bChildVisible) strHtmlText += szLevelExpandImage;
		else strHtmlText += szLevelCollapseImage;

		//Ŀ¼λ��
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>%s<x %d>(%d)"),m_LevelTextStartPosFolder,TaskNodeData.szTaskName,					m_LevelTextStartPosStatus,node->NumOfChildren());
		strHtmlText += szBuffer;
	}
	else
	{
		//����λ��
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>%s"),m_LevelTextStartPosChild,TaskNodeData.szTaskName);	
		strHtmlText += szBuffer;

		//����״̬
		LPCTSTR pszTaskStatus[] = {TEXT("������"),TEXT("��ȡ����"),TEXT("����ʧ��")};
		if(TaskNodeData.bShowStatus && TaskNodeData.cbTaskStatus<CountArray(pszTaskStatus))
		{
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("<x %d>(%s)"),m_LevelTextStartPosStatus,pszTaskStatus[TaskNodeData.cbTaskStatus]);
			strHtmlText+=szBuffer;
		}
	}
	
	//�����ı�
	if (TaskNodeData.szTaskName[0]!=0)
	{
		CLabelUI* pLabelDescription = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(TaskNodeData.plistElment, szTaskNameControlName));
		if (pLabelDescription != NULL)
		{
			pLabelDescription->SetShowHtml(true);
			pLabelDescription->SetText(strHtmlText);
		}
	}
}

//ѡ������
bool CTaskUI::SelectItem(int iIndex)
{
    if( iIndex == m_iCurSel ) return true;
    if( m_iCurSel >= 0 ) 
	{
        CControlUI* pControl = GetItemAt(m_iCurSel);
        if( pControl != NULL) 
		{
            IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(TEXT("ListItem")));
            if( pListItem != NULL )
			{
				pListItem->Select(false);
			}
        }

        m_iCurSel = -1;
    }

    if( iIndex < 0 ) return false;
	if (!__super::SelectItem(iIndex)) return false;

	return true;
}
