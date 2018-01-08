#ifndef GAME_PROPERTY_CTRL_HEAD_FILE
#define GAME_PROPERTY_CTRL_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_PROPERTY_CLASS CGamePropertyCtrl : public CFGuiWnd
{
	//״̬����
protected:
	bool							m_bHovering;						//������־
	WORD							m_wDownItem;						//�������
	WORD							m_wHoverItem;						//��������
	BYTE                            m_cbCurrType;                       //��ǰ����

	//���ñ���
protected:
	CSize							m_SizeImageItem;					//����ͼƬ
	tagPropertyResource				m_PropertyResource;					//��Դ����

	//��ť����
protected:
	CButtonUI *						m_btLeftMove;					    //���ư�ť
	CButtonUI *						m_btRightMove;					    //���ư�ť

	//��������
protected:
	BYTE                            m_cbPresentStartIndex;              //��������
	BYTE                            m_cbPropertyStartIndex;             //��������
	BYTE                            m_cbPresentCount;                   //������Ŀ 
	BYTE							m_cbPropertyCount;					//������Ŀ
	CGamePropertyBase *             m_pCurrPropertyItem;                //��ǰ����
	CGamePropertyBase *             m_pGamePreSentItem[MAX_PROPERTY];   //��������
	CGamePropertyBase *				m_pGamePropertyItem[MAX_PROPERTY];	//��������

	//�ӿڱ���
protected:
	IGamePropertySink *				m_pIGamePropertySink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CGamePropertyCtrl();
	//��������
	virtual ~CGamePropertyCtrl();

	//�̳к���
public:	
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//���ú���
public:
	//���ýӿ�
	bool SetGamePropertySink(IUnknownEx * pIUnknownEx);
	//������Դ
	bool SetPropertyResource(tagPropertyResource & PropertyResource);
	//���õ���
	bool SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount, WORD wIssueArea);

	//�¼�����
public:
	//�ı��¼�
	VOID OnSelectedUserItemChange(IClientUserItem * pIClintUserItem);

	//�ڲ�����
protected:
	//������Ŀ
	INT CalcItemCount();
	//����ж�
	WORD SwitchItemFromPoint(CPoint MousePoint);

	//��Ϣӳ��
protected:
	//��������
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//���ư�ť
	VOID OnBtnLeftMoveClicked();
	//���ư�ť
	VOID OnBtnRightMoveClicked();

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////////////

#endif