#include "StdAfx.h"
#include "PlazaViewContainer.h"
#include ".\plazaviewcontainer.h"

/////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewContainer, CWnd)

	//系统消息
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////////////
//静态定义
CPlazaViewContainer * CPlazaViewContainer::m_pPlazaViewContainer=NULL;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewContainer::CPlazaViewContainer()
{
	if(m_pPlazaViewContainer==NULL) m_pPlazaViewContainer=this;
	return;
}

//析构函数
CPlazaViewContainer::~CPlazaViewContainer()
{
	if(m_pPlazaViewContainer==this) m_pPlazaViewContainer=NULL;

	//释放资源
	for(INT_PTR nIndex=0;nIndex<m_ViewItemArray.GetCount();nIndex++)
	{
		SafeDelete(m_ViewItemArray[nIndex]);
	}
	return;
}

//删除子项
VOID CPlazaViewContainer::RemoveViewItem(CWnd * pItemView)
{
	//效验状态
	ASSERT(GetViewItemIndex(pItemView)!=INVALID_WORD);
	ASSERT(m_ViewItemArray[GetViewItemIndex(pItemView)]!=NULL);

	//获取对象
	WORD wItemIndex=GetViewItemIndex(pItemView);
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];

	//设置窗口
	if(IsWindow(pViewItemInfo->pItemView->m_hWnd))
	{
		pViewItemInfo->pItemView->SetParent(NULL);
		pViewItemInfo->pItemView->ShowWindow(SW_HIDE);
	}

	//删除对象	
	SafeDelete(pViewItemInfo);
	m_ViewItemArray.RemoveAt(wItemIndex);

	return;
}

//移除视图
VOID CPlazaViewContainer::RemoveViewItem(emViewItemArea ItemViewArea)
{
	//效验状态
	/*ASSERT(GetViewItemIndex(ItemViewArea)!=INVALID_WORD);
	ASSERT(m_ViewItemArray[GetViewItemIndex(ItemViewArea)]!=NULL);*/

	//获取对象
	WORD wItemIndex=GetViewItemIndex(ItemViewArea);
	if(wItemIndex==INVALID_WORD) return;
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];

	//设置窗口
	if(IsWindow(pViewItemInfo->pItemView->m_hWnd))
	{
		pViewItemInfo->pItemView->SetParent(NULL);
		pViewItemInfo->pItemView->ShowWindow(SW_HIDE);
	}

	//删除对象
	SafeDelete(pViewItemInfo);
	m_ViewItemArray.RemoveAt(wItemIndex);

	return;
}

//获取视图
CWnd * CPlazaViewContainer::GetViewItemByArea(emViewItemArea ItemViewArea)
{
	//获取对象
	WORD wItemIndex=GetViewItemIndex(ItemViewArea);
	if(wItemIndex==INVALID_WORD) return NULL;

	//获取视图
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];
	if(pViewItemInfo==NULL) return NULL;

	return pViewItemInfo->pItemView;
}

//插入视图
BOOL CPlazaViewContainer::InsertViewItem(CWnd * pViewWnd,emViewItemArea ItemViewArea)
{
	//参数校验
	ASSERT(pViewWnd!=NULL && m_hWnd!=NULL);
	if(pViewWnd==NULL || m_hWnd==NULL) return FALSE;

	//变量定义
	tagViewItemInfo * pViewItemInfo=NULL;

	//重复判断
	WORD wViewItemIndex = GetViewItemIndex(ItemViewArea);
	if(wViewItemIndex!=INVALID_WORD)
	{
		pViewItemInfo = m_ViewItemArray[wViewItemIndex];
		if(pViewItemInfo!=NULL && IsWindow(pViewItemInfo->pItemView->m_hWnd))
		{
			if(pViewItemInfo->pItemView==pViewWnd) return TRUE;
			pViewItemInfo->pItemView->ShowWindow(SW_HIDE);
		}
		pViewItemInfo->pItemView=pViewWnd;
		pViewItemInfo->pItemView->SetParent(this);
		return TRUE;
	}

	//构造信息
	try
	{
		//创建对象
		pViewItemInfo = new tagViewItemInfo;
		if(pViewItemInfo==NULL) throw(TEXT("内存不足！"));

		//设置父类
		pViewWnd->SetParent(this);

		//设置对象
		pViewItemInfo->pItemView = pViewWnd;
		pViewItemInfo->ItemViewArea=ItemViewArea;
		
		//添加子项
		m_ViewItemArray.Add(pViewItemInfo);
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}		

	return TRUE;
}

//调整子项
VOID CPlazaViewContainer::RectifyViewItem()
{
	//变量定义
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyViewItem(rcClient.Width(),rcClient.Height());
}

//调整子项
VOID CPlazaViewContainer::RectifyViewItem(INT nWidth,INT nHeight)
{
	if(nWidth==0 || nHeight==0) return;

	//变量定义
	WORD wViewItemIndex = INVALID_WORD;
	tagViewItemInfo * pTopViewItemInfo=NULL;
	tagViewItemInfo * pBottomViewItemInfo=NULL;
	tagViewItemInfo * pLeftViewItemInfo=NULL;
	tagViewItemInfo * pRightViewItemInfo=NULL;
	tagViewItemInfo * pCenterViewItemInfo=NULL;

	//顶部视图
	CRect rcTopViewItem(0,0,0,0);
	wViewItemIndex = GetViewItemIndex(VIA_Top);
	if(wViewItemIndex!=INVALID_WORD)
	{
		pTopViewItemInfo = m_ViewItemArray[wViewItemIndex];
		if(IsValidViewItem(pTopViewItemInfo)==TRUE)
		{
			pTopViewItemInfo->pItemView->GetWindowRect(&rcTopViewItem);
		}
	}

	//底部视图
	CRect rcBottomViewItem(0,0,0,0);
	wViewItemIndex = GetViewItemIndex(VIA_Bottom);
	if(wViewItemIndex!=INVALID_WORD)
	{
		pBottomViewItemInfo = m_ViewItemArray[wViewItemIndex];
		if(IsValidViewItem(pBottomViewItemInfo)==TRUE)
		{
			pBottomViewItemInfo->pItemView->GetWindowRect(&rcBottomViewItem);
		}
	}

	//左边视图
	CRect rcLeftViewItem(0,0,0,0);
	wViewItemIndex = GetViewItemIndex(VIA_Left);
	if(wViewItemIndex!=INVALID_WORD)
	{
		pLeftViewItemInfo = m_ViewItemArray[wViewItemIndex];
		if(IsValidViewItem(pLeftViewItemInfo)==TRUE)
		{
			pLeftViewItemInfo->pItemView->GetWindowRect(&rcLeftViewItem);
		}
	}

	//右边视图
	CRect rcRightViewItem(0,0,0,0);
	wViewItemIndex = GetViewItemIndex(VIA_Right);
	if(wViewItemIndex!=INVALID_WORD)
	{
		pRightViewItemInfo = m_ViewItemArray[wViewItemIndex];
		if(IsValidViewItem(pRightViewItemInfo)==TRUE)
		{
			pRightViewItemInfo->pItemView->GetWindowRect(&rcRightViewItem);
		}
	}

	//中间视图
	CRect rcCenterViewItem(0,0,0,0);
	wViewItemIndex = GetViewItemIndex(VIA_Center);
	if(wViewItemIndex!=INVALID_WORD)
	{
		pCenterViewItemInfo = m_ViewItemArray[wViewItemIndex];
		if(IsValidViewItem(pCenterViewItemInfo)==TRUE)
		{
			pCenterViewItemInfo->pItemView->GetWindowRect(&rcCenterViewItem);
		}
	}

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_SHOWWINDOW;

	//顶端
	if(IsValidViewItem(pTopViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pTopViewItemInfo->pItemView->m_hWnd,NULL,rcLeftViewItem.Width(),0,nWidth-rcLeftViewItem.Width()-rcRightViewItem.Width(),rcTopViewItem.Height(),uFlags);
	}

	//底端
	if(IsValidViewItem(pBottomViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pBottomViewItemInfo->pItemView->m_hWnd,NULL,rcLeftViewItem.Width(),nHeight-rcBottomViewItem.Height(),nWidth-rcLeftViewItem.Width()-rcRightViewItem.Width(),rcBottomViewItem.Height(),uFlags);
	}

	//左端
	if(IsValidViewItem(pLeftViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pLeftViewItemInfo->pItemView->m_hWnd,NULL,0,0,rcLeftViewItem.Width(),nHeight,uFlags);
	}

	//右端
	if(IsValidViewItem(pRightViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pRightViewItemInfo->pItemView->m_hWnd,NULL,nWidth-rcRightViewItem.Width(),0,rcRightViewItem.Width(),nHeight,uFlags);
	}

	//中端
	if(IsValidViewItem(pCenterViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pCenterViewItemInfo->pItemView->m_hWnd,NULL,rcLeftViewItem.Width(),rcTopViewItem.Height(),nWidth-rcLeftViewItem.Width()-rcRightViewItem.Width(),nHeight-rcTopViewItem.Height()-rcBottomViewItem.Height(),uFlags);
	}

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();
}

//对象索引
WORD CPlazaViewContainer::GetViewItemIndex(CWnd * pItemView)
{
	//搜索对象
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		if (m_ViewItemArray[i]->pItemView==pItemView)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//对象索引
WORD CPlazaViewContainer::GetViewItemIndex(emViewItemArea ItemViewArea)
{
	//搜索对象
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		if (m_ViewItemArray[i]->ItemViewArea==ItemViewArea)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//有效子项
BOOL CPlazaViewContainer::IsValidViewItem(tagViewItemInfo * pViewItemInfo)
{
	if(pViewItemInfo==NULL || pViewItemInfo->pItemView==NULL) return FALSE;

	return IsWindow(pViewItemInfo->pItemView->m_hWnd) && (pViewItemInfo->pItemView->GetParent()==this);
}

//绘画背景
BOOL CPlazaViewContainer::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//填充背景
	pDC->FillSolidRect(&rcClient,RGB(0,0,0));

	return TRUE;
}

//窗口大小
VOID CPlazaViewContainer::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//调整子项
	RectifyViewItem(cx,cy);
}

//////////////////////////////////////////////////////////////////////////////////