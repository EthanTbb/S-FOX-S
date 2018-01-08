#ifndef DLG_MATCH_AWARDS_HEAD_FILE
#define DLG_MATCH_AWARDS_HEAD_FILE

#pragma once

#include "afxwin.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////

//比赛奖状
class GAME_FRAME_CLASS CDlgMatchAwards : public CFGuiDialog
{
	//控制变量
public:
	CMD_GR_MatchResult *				m_pMatchResult;					//比赛结果

	//资源变量
protected:
	CFont								m_RewardFont;					//奖励字体
	CFont								m_AccountsFont;					//账号字体	
	CFont								m_WatermarkFont;				//水印字体	

	//组件接口
protected:
	IClientKernel *						m_pIClientKernel;				//内核接口
	IStringMessage *					m_pIStringMessage;				//消息接口

	//界面控件
protected:
	CButtonUI *							m_pbtClose;						//关闭按钮
	CButtonUI *							m_pbtTakePhoto;					//拍照按钮
	CSkinLayered						m_SkinLayered;					//分层窗口

	//函数定义
public:
	//构造函数
	CDlgMatchAwards();
	//析构函数
	virtual ~CDlgMatchAwards();

	//重载函数
protected:
	//初始化窗口
	virtual BOOL OnInitDialog();

	//消息函数
public:
	//左键按下
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

    //接口函数
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);	

	//绘画接口
public:
	//开始绘画
	virtual VOID OnBeginPaintWindow(HDC hDC);
	
	//功能函数
public:
	//设置参数
	VOID SetMatchResult(CMD_GR_MatchResult * pMatchResult);
	
	//截屏函数
protected:
	//拷贝屏幕
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
	//游戏截屏
	bool CaptureGameScreen(LPCTSTR pszSavePath);
	//拷贝位图
	bool CopyBitmapToClipBoard(HBITMAP hBitmap);

	//消息函数
public:
	//关闭消息
	VOID OnBnClickedClose();
	//拍照消息
	VOID OnBnClickedTakePhoto();
};

#endif