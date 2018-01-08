#include "StdAfx.h"
#include "PlazaViewContainer.h"
#include ".\plazaviewcontainer.h"

/////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewContainer, CWnd)

	//ϵͳ��Ϣ
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////////////
//��̬����
CPlazaViewContainer * CPlazaViewContainer::m_pPlazaViewContainer=NULL;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaViewContainer::CPlazaViewContainer()
{
	if(m_pPlazaViewContainer==NULL) m_pPlazaViewContainer=this;
	return;
}

//��������
CPlazaViewContainer::~CPlazaViewContainer()
{
	if(m_pPlazaViewContainer==this) m_pPlazaViewContainer=NULL;

	//�ͷ���Դ
	for(INT_PTR nIndex=0;nIndex<m_ViewItemArray.GetCount();nIndex++)
	{
		SafeDelete(m_ViewItemArray[nIndex]);
	}
	return;
}

//ɾ������
VOID CPlazaViewContainer::RemoveViewItem(CWnd * pItemView)
{
	//Ч��״̬
	ASSERT(GetViewItemIndex(pItemView)!=INVALID_WORD);
	ASSERT(m_ViewItemArray[GetViewItemIndex(pItemView)]!=NULL);

	//��ȡ����
	WORD wItemIndex=GetViewItemIndex(pItemView);
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];

	//���ô���
	if(IsWindow(pViewItemInfo->pItemView->m_hWnd))
	{
		pViewItemInfo->pItemView->SetParent(NULL);
		pViewItemInfo->pItemView->ShowWindow(SW_HIDE);
	}

	//ɾ������	
	SafeDelete(pViewItemInfo);
	m_ViewItemArray.RemoveAt(wItemIndex);

	return;
}

//�Ƴ���ͼ
VOID CPlazaViewContainer::RemoveViewItem(emViewItemArea ItemViewArea)
{
	//Ч��״̬
	/*ASSERT(GetViewItemIndex(ItemViewArea)!=INVALID_WORD);
	ASSERT(m_ViewItemArray[GetViewItemIndex(ItemViewArea)]!=NULL);*/

	//��ȡ����
	WORD wItemIndex=GetViewItemIndex(ItemViewArea);
	if(wItemIndex==INVALID_WORD) return;
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];

	//���ô���
	if(IsWindow(pViewItemInfo->pItemView->m_hWnd))
	{
		pViewItemInfo->pItemView->SetParent(NULL);
		pViewItemInfo->pItemView->ShowWindow(SW_HIDE);
	}

	//ɾ������
	SafeDelete(pViewItemInfo);
	m_ViewItemArray.RemoveAt(wItemIndex);

	return;
}

//��ȡ��ͼ
CWnd * CPlazaViewContainer::GetViewItemByArea(emViewItemArea ItemViewArea)
{
	//��ȡ����
	WORD wItemIndex=GetViewItemIndex(ItemViewArea);
	if(wItemIndex==INVALID_WORD) return NULL;

	//��ȡ��ͼ
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];
	if(pViewItemInfo==NULL) return NULL;

	return pViewItemInfo->pItemView;
}

//������ͼ
BOOL CPlazaViewContainer::InsertViewItem(CWnd * pViewWnd,emViewItemArea ItemViewArea)
{
	//����У��
	ASSERT(pViewWnd!=NULL && m_hWnd!=NULL);
	if(pViewWnd==NULL || m_hWnd==NULL) return FALSE;

	//��������
	tagViewItemInfo * pViewItemInfo=NULL;

	//�ظ��ж�
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

	//������Ϣ
	try
	{
		//��������
		pViewItemInfo = new tagViewItemInfo;
		if(pViewItemInfo==NULL) throw(TEXT("�ڴ治�㣡"));

		//���ø���
		pViewWnd->SetParent(this);

		//���ö���
		pViewItemInfo->pItemView = pViewWnd;
		pViewItemInfo->ItemViewArea=ItemViewArea;
		
		//�������
		m_ViewItemArray.Add(pViewItemInfo);
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}		

	return TRUE;
}

//��������
VOID CPlazaViewContainer::RectifyViewItem()
{
	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyViewItem(rcClient.Width(),rcClient.Height());
}

//��������
VOID CPlazaViewContainer::RectifyViewItem(INT nWidth,INT nHeight)
{
	if(nWidth==0 || nHeight==0) return;

	//��������
	WORD wViewItemIndex = INVALID_WORD;
	tagViewItemInfo * pTopViewItemInfo=NULL;
	tagViewItemInfo * pBottomViewItemInfo=NULL;
	tagViewItemInfo * pLeftViewItemInfo=NULL;
	tagViewItemInfo * pRightViewItemInfo=NULL;
	tagViewItemInfo * pCenterViewItemInfo=NULL;

	//������ͼ
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

	//�ײ���ͼ
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

	//�����ͼ
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

	//�ұ���ͼ
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

	//�м���ͼ
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

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_SHOWWINDOW;

	//����
	if(IsValidViewItem(pTopViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pTopViewItemInfo->pItemView->m_hWnd,NULL,rcLeftViewItem.Width(),0,nWidth-rcLeftViewItem.Width()-rcRightViewItem.Width(),rcTopViewItem.Height(),uFlags);
	}

	//�׶�
	if(IsValidViewItem(pBottomViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pBottomViewItemInfo->pItemView->m_hWnd,NULL,rcLeftViewItem.Width(),nHeight-rcBottomViewItem.Height(),nWidth-rcLeftViewItem.Width()-rcRightViewItem.Width(),rcBottomViewItem.Height(),uFlags);
	}

	//���
	if(IsValidViewItem(pLeftViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pLeftViewItemInfo->pItemView->m_hWnd,NULL,0,0,rcLeftViewItem.Width(),nHeight,uFlags);
	}

	//�Ҷ�
	if(IsValidViewItem(pRightViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pRightViewItemInfo->pItemView->m_hWnd,NULL,nWidth-rcRightViewItem.Width(),0,rcRightViewItem.Width(),nHeight,uFlags);
	}

	//�ж�
	if(IsValidViewItem(pCenterViewItemInfo)==TRUE)
	{
		DeferWindowPos(hDwp,pCenterViewItemInfo->pItemView->m_hWnd,NULL,rcLeftViewItem.Width(),rcTopViewItem.Height(),nWidth-rcLeftViewItem.Width()-rcRightViewItem.Width(),nHeight-rcTopViewItem.Height()-rcBottomViewItem.Height(),uFlags);
	}

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();
}

//��������
WORD CPlazaViewContainer::GetViewItemIndex(CWnd * pItemView)
{
	//��������
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		if (m_ViewItemArray[i]->pItemView==pItemView)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//��������
WORD CPlazaViewContainer::GetViewItemIndex(emViewItemArea ItemViewArea)
{
	//��������
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		if (m_ViewItemArray[i]->ItemViewArea==ItemViewArea)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//��Ч����
BOOL CPlazaViewContainer::IsValidViewItem(tagViewItemInfo * pViewItemInfo)
{
	if(pViewItemInfo==NULL || pViewItemInfo->pItemView==NULL) return FALSE;

	return IsWindow(pViewItemInfo->pItemView->m_hWnd) && (pViewItemInfo->pItemView->GetParent()==this);
}

//�滭����
BOOL CPlazaViewContainer::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��䱳��
	pDC->FillSolidRect(&rcClient,RGB(0,0,0));

	return TRUE;
}

//���ڴ�С
VOID CPlazaViewContainer::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//��������
	RectifyViewItem(cx,cy);
}

//////////////////////////////////////////////////////////////////////////////////