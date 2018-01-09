#ifndef FACE_CUSTOM_CONTROL_HEAD_FILE
#define FACE_CUSTOM_CONTROL_HEAD_FILE

#pragma once

#include "FaceItemControl.h"
#include "AvatarControlHead.h"
#include "ImageEditorControl.h"

//////////////////////////////////////////////////////////////////////////////////

//自定头像
class AVATAR_CONTROL_CLASS CFaceItemCustomWnd : public CFGuiWnd
{

	//函数定义
public:
	//构造函数
	CFaceItemCustomWnd();
	//析构函数
	virtual ~CFaceItemCustomWnd();

	//接口函数
public:
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgCustomAvatar.xml"); }
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);	

	//重载函数
protected:
	//绘画消息
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//配置函数
public:
	//设置接口
	VOID SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent);

	//图片管理
public:
	//加载判断
	bool IsNull();
	//重置图片
	VOID DestoryImage();
	//获取图片
	VOID DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//消息映射
protected:
	//位置信息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//按钮消息
protected:
	//旋转图像
	VOID OnBnClickedTurnLeft();
	//旋转图像
	VOID OnBnClickedTurnRight();
	//显示虚线
	VOID OnBnClickedShowVLine();
	//加载图像
	VOID OnBnClickedLoadImage();
	//加载图像
	VOID OnBnClickedLoadCamera();

	DECLARE_MESSAGE_MAP()

	//控制按钮
protected:
	CButtonUI *			 			m_pbtTurnLeft;						//左转按钮		
	CButtonUI *			 			m_pbtTurnRight;						//右转按钮
	CButtonUI *			 			m_pbtShowVLine;						//虚线按钮

	//控制按钮
protected:
	CButtonUI *			 			m_pbtLoadImage;						//加载图片
	CButtonUI *			 			m_pbtLoadCamera;					//开启摄像
	CTabLayoutUI*					m_pTabLayoutCustom;					//视图切换


	//控件变量
protected:
	CImageEditorControl    			m_ImageEditorControl;				//编辑控件
public:
	bool							m_bLoadCustom;						//成功加载
};

//////////////////////////////////////////////////////////////////////////////////

#endif