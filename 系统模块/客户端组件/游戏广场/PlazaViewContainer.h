#ifndef PLAZA_VIEW_CONTAINER_HEAD_FILE
#define PLAZA_VIEW_CONTAINER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////
//��ͼ����
enum emViewItemArea
{
	VIA_Top=0,
	VIA_Bottom,
	VIA_Left,
	VIA_Right,
	VIA_Center
};

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagViewItemInfo
{
	CWnd *							pItemView;							//������ͼ
	emViewItemArea					ItemViewArea;						//��������
};

//�����¼�
interface IViewItemEvent
{
	//�����¼�
	virtual VOID OnViewItemEvent(WORD wItemIndex, CWnd * pItemView)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//��ͼ����
class CPlazaViewContainer : public CWnd
{
	//������Ϣ
protected:
	CWHArray<tagViewItemInfo *>		m_ViewItemArray;					//��������

	//��̬����
protected:
	static CPlazaViewContainer *   m_pPlazaViewContainer;				//ָ�����

	//��������
public:
	//���캯��
	CPlazaViewContainer();
	//��������
	virtual ~CPlazaViewContainer();

	//��������
public:	
	//�Ƴ���ͼ
	VOID RemoveViewItem(CWnd * pViewWnd);
	//�Ƴ���ͼ
	VOID RemoveViewItem(emViewItemArea ItemViewArea);	
	//������ͼ
	BOOL InsertViewItem(CWnd * pViewWnd,emViewItemArea ItemViewArea);	

	//��������
public:
	//��������
	VOID RectifyViewItem();
	//��������
	VOID RectifyViewItem(INT nWidth,INT nHeight);

	//��������
public:
	//��ȡ��ͼ
	CWnd * GetViewItemByArea(emViewItemArea ItemViewArea);

	//�ڲ�����
private:
	//��������
	WORD GetViewItemIndex(CWnd * pItemView);
	//��������
	WORD GetViewItemIndex(emViewItemArea ItemViewArea);
	//��Ч����
	BOOL IsValidViewItem(tagViewItemInfo * pViewItemInfo);

	//��̬����
public:
	//��ȡ����
	static CPlazaViewContainer * GetInstance() { return m_pPlazaViewContainer; }

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//���ڴ�С
	VOID OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif