#ifndef DLG_CAMERA_HEAD_FILE
#define DLG_CAMERA_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//摄像头类
class AVATAR_CONTROL_CLASS CCameraControl : public CFGuiDialog
{
	//友元定义
	friend class CFaceItemCustomWnd;

	//变量定义
protected:
	bool							m_bInit;							//配置标识
	HWND							m_hWndCamera;						//显示窗口
	CImage							m_ImageResult;						//结果图像
	
	//设备信息
protected:
	CAPSTATUS						m_CapStatus;						//当前状态
	CAPTUREPARMS					m_CapTureparms;						//捕捉参数
	CAPDRIVERCAPS					m_CapDrivercaps;					//驱动性能

	//界面控件
protected:
	CLabelUI *						m_pLabelText;						//文本标签

	//界面控件
protected:
	CButtonUI *						m_pbtOk;							//保存编辑
	CButtonUI *						m_pbtRepeat;						//重新拍摄
	CButtonUI *						m_pbtCameraDict;					//捕获单帧

	//其他控件
protected:
	CStatic							m_CameraControl;					//视频窗口

	//函数定义
public:
	//构造函数
	CCameraControl(CWnd * pParent);
	//析构函数
	virtual ~CCameraControl();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();

	//接口函数
public:
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgCamera.xml"); } 
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	
	//消息函数
protected:
	//重新拍摄
	VOID OnBnClickedRepeat();
	//拍摄图片
	VOID OnBnClickedCameraDict();
};

//////////////////////////////////////////////////////////////////////////////////

#endif