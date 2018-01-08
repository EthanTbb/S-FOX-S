#ifndef DLG_CUSTOM_FACE_HEAD_FILE
#define DLG_CUSTOM_FACE_HEAD_FILE

#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////

//头像选择
class CDlgCustomFace : public CFGuiDialog, public CMissionItem, public IImageEditorEvent, public IFaceItemSelectEvent
{
	//函数定义
public:
	//构造函数
	CDlgCustomFace();
	//析构函数
	virtual ~CDlgCustomFace();

	//编辑接口
public:
	//图像事件
	virtual VOID OnEventFaceSelect(WORD wFaceID);
	//图像事件
	virtual VOID OnEventImageSelect(bool bLoadImage, bool bOperateing);

	//重置函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//接口函数
public:
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgModifyAvatar.xml"); } 
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//窗口函数
protected:
	//绘画标题
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);

	virtual void OnEndPaintWindow(HDC hDC);
	//重载函数
protected:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	//设置接口
	bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx);

	//内部函数
private:
	//控件控制
	VOID EnableControl(bool bEnableControl);

private:
	bool LoadSysAvatar();
	bool UpdateAvatarPreview(const WORD wFaceID);
	bool UpdateAvatarPreview(DWORD* dwCustomFace, DWORD dwSize);

	DECLARE_MESSAGE_MAP()

	//状态变量
protected:
	BYTE							m_cbMode;							//编辑模式
	WORD							m_wFaceID;							//选择头像
	tagCustomFaceInfo				m_CustomFaceInfo;					//自定头像
	tagCustomFaceInfo96				m_CustomFaceInfo96;					//两个尺寸

	//接口变量
protected:
	ICustomFaceEvent *				m_pICustomFaceEvent;				//头像事件

	//功能按钮
protected:
	CButtonUI *						m_pbtOk;							//存储退出
	CButtonUI *						m_pbtCancel;						//存储退出
	CTileLayoutUI*					m_pTileLayoutSysAvatar;				//系统头像布局
	CImageUI*						m_pAvatarPreview;					//头像预览
	//控件变量
protected:
	//CSkinTabCtrl		 			m_TabControl;						//选择类型
	CMissionManager					m_MissionManager;					//任务管理
	CFaceItemCustomWnd				m_FaceItemCustomWnd;				//头像自定
	//CFaceItemSelectWnd	 			m_FaceItemSelectWnd;				//头像选择

};

//////////////////////////////////////////////////////////////////////////////////

#endif