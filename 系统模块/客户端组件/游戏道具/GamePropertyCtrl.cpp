#include "StdAfx.h"
#include "Resource.h"
#include "GamePropertyCtrl.h"
#include "GamePropertyManager.h"
#include ".\gamepropertyctrl.h"

//////////////////////////////////////////////////////////////////////////////////

#define  IDC_BT_LEFT_MOVE                 100                    //���ư�ť
#define  IDC_BT_RIGHT_MOVE                101                    //���ư�ť  

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePropertyCtrl, CFGuiWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//ϵͳ��Ϣ
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyCtrl::CGamePropertyCtrl()
{
	//״̬����
	m_bHovering=false;
	m_wDownItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;
	m_cbCurrType=0;

	//���ñ���
	m_pIGamePropertySink=NULL;
	m_pCurrPropertyItem=NULL;

	//����λ��
	m_SizeImageItem.SetSize(0,0);
	ZeroMemory(&m_PropertyResource,sizeof(m_PropertyResource));

	//���ñ���
	m_cbPresentStartIndex=0;
	m_cbPropertyStartIndex=0;
	m_cbPresentCount=0;
	m_cbPropertyCount=0;
	ZeroMemory(m_pGamePropertyItem,sizeof(m_pGamePropertyItem));
	ZeroMemory(m_pGamePreSentItem,sizeof(m_pGamePreSentItem));	

	return;
}

//��������
CGamePropertyCtrl::~CGamePropertyCtrl()
{
	//֪ͨ�˳�
	if(m_pCurrPropertyItem!=NULL) m_pCurrPropertyItem->OnEventPerformCommand(CMD_CLOSE_WND,NULL);
}

//���ýӿ�
bool CGamePropertyCtrl::SetGamePropertySink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertySink)!=NULL);
		m_pIGamePropertySink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertySink);

		//�ɹ��ж�
		if (m_pIGamePropertySink==NULL) return false;
	}
	else m_pIGamePropertySink=NULL;

	return true;
}

//������Դ
bool CGamePropertyCtrl::SetPropertyResource(tagPropertyResource & PropertyResource)
{
	//���ñ���
	m_PropertyResource=PropertyResource;

	//������Դ
	CImage ImageGamePropertyItem;
	ImageGamePropertyItem.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageItem);

	//���ñ���
	m_SizeImageItem.SetSize(ImageGamePropertyItem.GetWidth(),ImageGamePropertyItem.GetHeight());

	return true;
}

//���õ���
bool CGamePropertyCtrl::SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount, WORD wIssueArea)
{
	//���ñ���
	m_cbPropertyCount=0;
	m_cbPresentCount=0;
	m_cbPresentStartIndex=0;
	m_cbPropertyStartIndex=0;
	ZeroMemory(m_pGamePropertyItem,sizeof(m_pGamePropertyItem));
	ZeroMemory(m_pGamePreSentItem,sizeof(m_pGamePreSentItem));

	//���õ���
	for (BYTE i=0;i<cbPropertyCount;i++)
	{
		if ((PropertyInfo[i].wUseArea&PT_ISSUE_AREA_SERVER)!=0)
		{
			//����ж�
			ASSERT(m_cbPropertyCount<CountArray(m_pGamePropertyItem));
			if (m_cbPropertyCount>=CountArray(m_pGamePropertyItem)) break;

			//����ж�
			ASSERT(m_cbPresentCount<CountArray(m_pGamePreSentItem));
			if (m_cbPresentCount>=CountArray(m_pGamePreSentItem)) break;

			//��������
			CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
			CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->CreatePropertyItem(PropertyInfo[i]);

			//�������
			if (pGamePropertyItem!=NULL)
			{
				//��������
				if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
					m_pGamePreSentItem[m_cbPresentCount++]=pGamePropertyItem;

				//��������
				if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
					m_pGamePropertyItem[m_cbPropertyCount++]=pGamePropertyItem;
			}
		}
	}

	if( ::IsWindow(m_hWnd) )
	{
		//�����ؼ�
		CRect rcClient;
		GetClientRect(&rcClient);
		RectifyControl(rcClient.Width(),rcClient.Height());
	}
	return true;
}

//�ı��¼�
VOID  CGamePropertyCtrl::OnSelectedUserItemChange(IClientUserItem * pIClintUserItem)
{
	//����У��
	if(pIClintUserItem==NULL) return;

	//�����ж�
	if(m_pCurrPropertyItem==NULL) return;

	//�¼�����
	m_pCurrPropertyItem->OnEventPerformCommand(CMD_USER_CHANGE,(VOID *)pIClintUserItem);
}

//������Ŀ
INT CGamePropertyCtrl::CalcItemCount()
{
	//��ȡ����
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CPoint LButtonExcursion=m_PropertyResource.LButtonExcursion;
	CPoint RButtonExcursion=m_PropertyResource.RButtonExcursion;

	//������Ŀ
	INT nItemCount=(rcClient.Width()-LButtonExcursion.x-RButtonExcursion.x)/m_SizeImageItem.cx;
	if(m_btLeftMove!=NULL)
	{
		nItemCount=(rcClient.Width()-LButtonExcursion.x-RButtonExcursion.x-2*m_btLeftMove->GetFixedWidth())/m_SizeImageItem.cx;
	}

	return nItemCount;
}

//����ж�
WORD CGamePropertyCtrl::SwitchItemFromPoint(CPoint MousePoint)
{
	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CSize SizeExcursion=m_PropertyResource.SizeExcursion;
	CPoint PointExcursion=m_PropertyResource.PointExcursion;

	//����λ��
	INT nItemCount=CalcItemCount();
	INT nItemStart=(rcClient.Width()-nItemCount*m_SizeImageItem.cx)/2L;

	//��Χ�ж�
	if ((MousePoint.x<nItemStart)||(MousePoint.x>=(nItemStart+nItemCount*SizeExcursion.cx))) return INVALID_WORD;
	if ((MousePoint.y<PointExcursion.y)||(MousePoint.y>(PointExcursion.y+2L*SizeExcursion.cy))) return INVALID_WORD;

	//����ж�
	if (((MousePoint.x-nItemStart-PointExcursion.x)%SizeExcursion.cx)>=PROPERTY_IMAGE_CX) return INVALID_WORD;
	if (((MousePoint.y-PointExcursion.y)%SizeExcursion.cy)>=PROPERTY_IMAGE_CY) return INVALID_WORD;

	//��������
	INT nXIndex=(MousePoint.x-nItemStart)/SizeExcursion.cx;
	INT nYIndex=(MousePoint.y-PointExcursion.y)/SizeExcursion.cy;

	//��������
	m_cbCurrType = nYIndex==0?PT_TYPE_PROPERTY:PT_TYPE_PRESENT;

	//��������
	WORD wSwitchIndex=/*nYIndex*nItemCount+*/nXIndex;
	if (m_cbCurrType==PT_TYPE_PROPERTY && wSwitchIndex>=m_cbPropertyCount) return INVALID_WORD;
	if (m_cbCurrType==PT_TYPE_PRESENT && wSwitchIndex>=m_cbPresentCount) return INVALID_WORD;	

	return wSwitchIndex;
}

//��ʼ�滭
void CGamePropertyCtrl::OnBeginPaintWindow(HDC hDC)
{
    //��ȡ����
	CDC * pDC = CDC::FromHandle(hDC);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Դ
	CImage ImageGamePropertyL;
	CImage ImageGamePropertyM;
	CImage ImageGamePropertyR;
	CImage ImageGamePropertyItem;
	ImageGamePropertyL.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageL);
	ImageGamePropertyM.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageM);
	ImageGamePropertyR.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageR);
	ImageGamePropertyItem.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageItem);

	//��������
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//��Ⱦ��Դ
	pSkinRenderManager->RenderImage(ImageGamePropertyL);
	pSkinRenderManager->RenderImage(ImageGamePropertyM);
	pSkinRenderManager->RenderImage(ImageGamePropertyR);
	pSkinRenderManager->RenderImage(ImageGamePropertyItem);

	//�滭���
	ImageGamePropertyL.BitBlt(hDC,0,0);
	ImageGamePropertyR.BitBlt(hDC,rcClient.Width()-ImageGamePropertyR.GetWidth(),0);

	//��������
	INT nXStartPos=ImageGamePropertyL.GetWidth();
	INT nXConcludePos=rcClient.Width()-ImageGamePropertyR.GetWidth();

	//����λ��
	INT nItemCount=CalcItemCount();
	INT nItemStart=(rcClient.Width()-nItemCount*m_SizeImageItem.cx)/2;

	//�滭�м�
	for (INT nXPos=nXStartPos;nXPos<nXConcludePos;nXPos+=ImageGamePropertyM.GetWidth())
	{
		INT nCXDraw=__min(ImageGamePropertyM.GetWidth(),nXConcludePos-nXPos);
		ImageGamePropertyM.BitBlt(hDC,nXPos,0,nCXDraw,ImageGamePropertyM.GetHeight(),0,0);
	}

	//�滭����
	for (INT i=0;i<nItemCount;i++)
	{
		ImageGamePropertyItem.BitBlt(hDC,nItemStart+i*ImageGamePropertyItem.GetWidth(),0);
	}

	//��������
	CSize SizeExcursion=m_PropertyResource.SizeExcursion;
	CPoint PointExcursion=m_PropertyResource.PointExcursion;
	INT nXHoverPos=-1;
	INT nYHoverPos=-1;

	//�滭����
	for (BYTE i=0;i<m_cbPropertyCount;i++)
	{
		//Ч�����
		ASSERT(m_pGamePropertyItem[i]!=NULL);
		if (m_pGamePropertyItem[i]==NULL) break;

		//��ȡ��Ϣ
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem[i]->GetImageInfo(PropertyImage);

		//������Դ
		CPngImage ImageProperty;
		ImageProperty.LoadImage(PropertyImage.hResInstance,PropertyImage.pszImageID);

		//�����ж�
		if(i<m_cbPropertyStartIndex) continue;
		BYTE cbIndex=i-m_cbPropertyStartIndex;

		//��Ŀ�ж�
		if(cbIndex>=nItemCount) break;

		//�滭����
		if (ImageProperty.IsNull()==false)
		{
			//��ȡ��С
			CSize SizeProperty;
			SizeProperty.SetSize(ImageProperty.GetWidth()/3L,ImageProperty.GetHeight());

			//����λ��
			INT nImagePos=0;
			if (m_wDownItem==cbIndex && m_cbCurrType==PT_TYPE_PROPERTY) nImagePos=SizeProperty.cx;
			if ((m_wDownItem!=cbIndex)&&(m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PROPERTY)) nImagePos=SizeProperty.cx*2;

			//�滭ͼ��
			INT nXDrawPos=(cbIndex%nItemCount)*SizeExcursion.cx+PointExcursion.x;
			INT nYDrawPos=PointExcursion.y;
			ImageProperty.DrawImage(pDC,nItemStart+nXDrawPos,nYDrawPos,PROPERTY_IMAGE_CX,PROPERTY_IMAGE_CY,nImagePos,0,SizeProperty.cx,SizeProperty.cy);

			//�滭���
			if (m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PROPERTY)
			{
				//������Դ
				CPngImage ImageHoverFrame;
				ImageHoverFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("HOVER_FRAME"));

				//��ȡ��С
				CSize SizeHoverFrame;
				SizeHoverFrame.SetSize(ImageHoverFrame.GetWidth(),ImageHoverFrame.GetHeight());

				//�滭���
				nXHoverPos=nXDrawPos+(PROPERTY_IMAGE_CX-SizeHoverFrame.cx)/2L;
				nYHoverPos=nYDrawPos+(PROPERTY_IMAGE_CY-SizeHoverFrame.cy)/2L;
				ImageHoverFrame.DrawImage(pDC,nItemStart+nXHoverPos,nYHoverPos);
			}
		}
	}

	//�滭����
	for (BYTE i=0;i<m_cbPresentCount;i++)
	{
		//Ч�����
		ASSERT(m_pGamePreSentItem[i]!=NULL);
		if (m_pGamePreSentItem[i]==NULL) break;

		//��ȡ��Ϣ
		tagPropertyImage PropertyImage;
		m_pGamePreSentItem[i]->GetImageInfo(PropertyImage);

		//������Դ
		CPngImage ImageProperty;
		ImageProperty.LoadImage(PropertyImage.hResInstance,PropertyImage.pszImageID);

		//�����ж�
		if(i<m_cbPresentStartIndex) continue;
		BYTE cbIndex=i-m_cbPresentStartIndex;

		//��Ŀ�ж�
		if(cbIndex>=nItemCount) break;

		//�滭����
		if (ImageProperty.IsNull()==false)
		{
			//��ȡ��С
			CSize SizeProperty;
			SizeProperty.SetSize(ImageProperty.GetWidth()/3L,ImageProperty.GetHeight());

			//����λ��
			INT nImagePos=0;
			if (m_wDownItem==cbIndex && m_cbCurrType==PT_TYPE_PRESENT) nImagePos=SizeProperty.cx;
			if ((m_wDownItem!=cbIndex)&&(m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PRESENT)) nImagePos=SizeProperty.cx*2;

			//�滭ͼ��
			INT nXDrawPos=(cbIndex%nItemCount)*SizeExcursion.cx+PointExcursion.x;
			INT nYDrawPos=SizeExcursion.cy+PointExcursion.y;
			ImageProperty.DrawImage(pDC,nItemStart+nXDrawPos,nYDrawPos,PROPERTY_IMAGE_CX,PROPERTY_IMAGE_CY,nImagePos,0,SizeProperty.cx,SizeProperty.cy);

			//�滭���
			if (m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PRESENT)
			{
				//������Դ
				CPngImage ImageHoverFrame;
				ImageHoverFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("HOVER_FRAME"));

				//��ȡ��С
				CSize SizeHoverFrame;
				SizeHoverFrame.SetSize(ImageHoverFrame.GetWidth(),ImageHoverFrame.GetHeight());

				//�滭���
				nXHoverPos=nXDrawPos+(PROPERTY_IMAGE_CX-SizeHoverFrame.cx)/2L;
				nYHoverPos=nYDrawPos+(PROPERTY_IMAGE_CY-SizeHoverFrame.cy)/2L;
				ImageHoverFrame.DrawImage(pDC,nItemStart+nXHoverPos,nYHoverPos);
			}
		}
	}

	//��ʾ
	if(m_wHoverItem != INVALID_WORD /*&& nXHoverPos != -1 && nYHoverPos != -1*/)
	{
		//��ȡ��Ϣ
		TCHAR *pName=NULL;
		if(m_cbCurrType==PT_TYPE_PROPERTY)
		{
			pName = m_pGamePropertyItem[m_wHoverItem+m_cbPropertyStartIndex]->GetPropertyAttrib()->szPropertyName;
		}
		else if(m_cbCurrType == PT_TYPE_PRESENT)
		{
			pName = m_pGamePreSentItem[m_wHoverItem+m_cbPresentStartIndex]->GetPropertyAttrib()->szPropertyName;
		}

		//λ������
		if(m_wHoverItem <= nItemCount/2) nXHoverPos += (PROPERTY_IMAGE_CX+20);
		else nXHoverPos -= (PROPERTY_IMAGE_CX+20);
		nYHoverPos += (PROPERTY_IMAGE_CY/2)+5;

		//������Ϣ
		CRect rcTip(nXHoverPos,nYHoverPos,nXHoverPos+80,nYHoverPos+20);
		pDC->FillSolidRect(&rcTip, RGB(240,240,240));
		pDC->Draw3dRect(&rcTip,RGB(153,149,103),RGB(153,149,103));
		if(pName)
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->DrawText(pName, -1, &rcTip, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}

	return;
}

//��ʼ�ؼ�
void CGamePropertyCtrl::InitControlUI()
{
	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//���ư�ť
	m_btLeftMove = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btLeftMove->SetStatusImage(TEXT("file='BT_LEFT' restype='PNG'"));

	//���ư�ť
	m_btRightMove = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btRightMove->SetStatusImage(TEXT("file='BT_RIGHT' restype='PNG'"));
}

//��Ϣ����
void CGamePropertyCtrl::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//���ư�ť
		if(pControlUI==m_btLeftMove)
		{
			return OnBtnLeftMoveClicked();
		}

		//���ư�ť
		if(pControlUI==m_btRightMove)
		{
			return OnBtnRightMoveClicked();
		}
	}
}

//�����ؼ�
VOID CGamePropertyCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//��������
	if (nWidth==0 || nHeight==0) return;
	if (m_SizeImageItem.cx==0) return;

	//��������
	CSize SizeButton;
    
	//����λ��
	SizeButton.SetSize(m_btLeftMove->GetFixedWidth(),m_btLeftMove->GetFixedHeight());
	CRect rcLeft(m_PropertyResource.LButtonExcursion.x,m_PropertyResource.LButtonExcursion.y,m_PropertyResource.LButtonExcursion.x+SizeButton.cx,m_PropertyResource.LButtonExcursion.y+SizeButton.cy);
	m_btLeftMove->SetPos(rcLeft);

	//����λ��
	SizeButton.SetSize(m_btRightMove->GetFixedWidth(),m_btRightMove->GetFixedHeight());
	CRect rcRight(nWidth-SizeButton.cx-m_PropertyResource.RButtonExcursion.x,m_PropertyResource.RButtonExcursion.y,nWidth-m_PropertyResource.RButtonExcursion.x,m_PropertyResource.RButtonExcursion.y+SizeButton.cy);
	m_btRightMove->SetPos(rcRight);

	//��������
	INT nItemCount=CalcItemCount();

	//��������
	if(m_cbPresentStartIndex+nItemCount>=m_cbPresentCount)
		m_cbPresentStartIndex=__max(m_cbPresentCount-nItemCount,0);

	//��������
	if(m_cbPropertyStartIndex+nItemCount>=m_cbPropertyCount)
		m_cbPropertyStartIndex=__max(m_cbPropertyCount-nItemCount,0);
	
	//���ư�ť
	if(m_cbPresentStartIndex+nItemCount>=m_cbPresentCount && m_cbPropertyStartIndex+nItemCount>=m_cbPropertyCount)
		m_btRightMove->SetEnabled(false);
	else
		m_btRightMove->SetEnabled(true);

	//���ư�ť
	if(m_cbPresentStartIndex==0 && m_cbPropertyStartIndex==0)
		m_btLeftMove->SetEnabled(false);
	else
		m_btLeftMove->SetEnabled(true);

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//λ����Ϣ
VOID CGamePropertyCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//�����Ϣ
VOID CGamePropertyCtrl::OnMouseMove(UINT nFlags, CPoint MousePoint)
{
	__super::OnMouseMove(nFlags, MousePoint);

	//�����ж�
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//�����Ϣ
VOID CGamePropertyCtrl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//�ͷŲ���
	ReleaseCapture();

	//��������
	if (m_wDownItem!=INVALID_WORD)
	{
		//��������
		WORD wLastDown=m_wDownItem;
		BYTE cbLastType=m_cbCurrType;

		//���ñ���
		m_wDownItem=INVALID_WORD;
		m_wHoverItem=SwitchItemFromPoint(MousePoint);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		//��������
		if (wLastDown==m_wHoverItem && cbLastType==m_cbCurrType)
		{
			//��������
			if(m_cbCurrType==PT_TYPE_PROPERTY)
			{
				ASSERT(m_pGamePropertyItem[m_cbPropertyStartIndex+wLastDown]!=NULL);
				if(m_cbPropertyStartIndex+wLastDown>=m_cbPropertyCount) return;
				m_pCurrPropertyItem = m_pGamePropertyItem[m_cbPropertyStartIndex+wLastDown];
				m_pCurrPropertyItem->ShowBuyPropertyWnd(m_pIGamePropertySink);				
			}

            //��������
			if(m_cbCurrType==PT_TYPE_PRESENT)
			{
				ASSERT(m_pGamePreSentItem[m_cbPresentStartIndex+wLastDown]!=NULL);
				if(m_cbPresentStartIndex+wLastDown>=m_cbPresentCount) return;
				m_pCurrPropertyItem = m_pGamePreSentItem[m_cbPresentStartIndex+wLastDown];
				m_pCurrPropertyItem->ShowBuyPropertyWnd(m_pIGamePropertySink);				
			}

		}
	}

	return;
}

//�����Ϣ
VOID CGamePropertyCtrl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//����˻�
	SetCapture();

	//��������
	if (m_wDownItem==INVALID_WORD)
	{
		//���ñ���
		m_wDownItem=SwitchItemFromPoint(MousePoint);

		//���½���
		if (m_wDownItem!=INVALID_WORD)
		{
			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//�����Ϣ
LRESULT CGamePropertyCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_bHovering=false;

	//���ý���
	if (m_wHoverItem!=INVALID_WORD) 
	{
		//���ñ���
		m_wHoverItem=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return 0;
}

//�����Ϣ
BOOL CGamePropertyCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//����ж�
	WORD wLastItem=m_wHoverItem;
	WORD wHoverItem=SwitchItemFromPoint(MousePoint);

	//��ͬ�ж�
	if (wHoverItem!=m_wHoverItem)
	{
		//���ñ���
		m_wHoverItem=wHoverItem;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	//���ù��
	if (m_wHoverItem!=INVALID_WORD)
	{
		SetCursor(LoadCursor(GetModuleHandle(GAME_PROPERTY_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//���ư�ť
VOID CGamePropertyCtrl::OnBtnLeftMoveClicked()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	if(m_cbPresentStartIndex>0) m_cbPresentStartIndex--;

	//��������
	if(m_cbPropertyStartIndex>0) m_cbPropertyStartIndex--;

	//�����ؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());
}

//���ư�ť
VOID CGamePropertyCtrl::OnBtnRightMoveClicked()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Ŀ
	INT nItemCount=CalcItemCount();

	//��������
	if(m_cbPresentStartIndex+nItemCount<m_cbPresentCount) m_cbPresentStartIndex++;

	//��������
	if(m_cbPropertyStartIndex+nItemCount<m_cbPropertyCount) m_cbPropertyStartIndex++;

	//�����ؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());	
}

//��������
int CGamePropertyCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////