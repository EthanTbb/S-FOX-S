#ifndef PLAZA_VIEW_CONTAINER_HEAD_FILE
#define PLAZA_VIEW_CONTAINER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////
//视图区域
enum emViewItemArea
{
	VIA_Top=0,
	VIA_Bottom,
	VIA_Left,
	VIA_Right,
	VIA_Center
};

//////////////////////////////////////////////////////////////////////////////////

//子项信息
struct tagViewItemInfo
{
	CWnd *							pItemView;							//子项视图
	emViewItemArea					ItemViewArea;						//子项区域
};

//控制事件
interface IViewItemEvent
{
	//激活事件
	virtual VOID OnViewItemEvent(WORD wItemIndex, CWnd * pItemView)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//视图容器
class CPlazaViewContainer : public CWnd
{
	//子项信息
protected:
	CWHArray<tagViewItemInfo *>		m_ViewItemArray;					//控制数组

	//静态变量
protected:
	static CPlazaViewContainer *   m_pPlazaViewContainer;				//指针变量

	//函数定义
public:
	//构造函数
	CPlazaViewContainer();
	//析构函数
	virtual ~CPlazaViewContainer();

	//操作函数
public:	
	//移除视图
	VOID RemoveViewItem(CWnd * pViewWnd);
	//移除视图
	VOID RemoveViewItem(emViewItemArea ItemViewArea);	
	//插入视图
	BOOL InsertViewItem(CWnd * pViewWnd,emViewItemArea ItemViewArea);	

	//调整函数
public:
	//调整子项
	VOID RectifyViewItem();
	//调整子项
	VOID RectifyViewItem(INT nWidth,INT nHeight);

	//函数定义
public:
	//获取视图
	CWnd * GetViewItemByArea(emViewItemArea ItemViewArea);

	//内部函数
private:
	//对象索引
	WORD GetViewItemIndex(CWnd * pItemView);
	//对象索引
	WORD GetViewItemIndex(emViewItemArea ItemViewArea);
	//有效子项
	BOOL IsValidViewItem(tagViewItemInfo * pViewItemInfo);

	//静态函数
public:
	//获取对象
	static CPlazaViewContainer * GetInstance() { return m_pPlazaViewContainer; }

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//窗口大小
	VOID OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif